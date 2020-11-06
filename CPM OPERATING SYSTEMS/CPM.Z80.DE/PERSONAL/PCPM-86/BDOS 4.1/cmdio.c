
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdio.h"

static byte zeroes[16];

void zero_cmd(cmd_desc *cmdfile)
{
	cmdfile->cs      = cmdfile->ds = NULL;
	cmdfile->cs_base = cmdfile->ds_base = 0;
	cmdfile->cs_len  = cmdfile->ds_len = 0;
	cmdfile->cs_max  = cmdfile->ds_max = 0;
}

void free_cmd (cmd_desc *cmdfile)
{
	if (cmdfile->ds) free(cmdfile->ds);
	if (cmdfile->cs) free(cmdfile->cs);
	zero_cmd(cmdfile);	
}

char *alloc_cmd(cmd_desc *cmdfile, long cs_len, long ds_len)
{
	if (!cs_len || !ds_len) return "cs_len or ds_len is 0.";
	cmdfile->cs      = malloc(cs_len);
	cmdfile->ds      = malloc(ds_len);
	if (!cmdfile->ds || !cmdfile->cs) 
	{
		free_cmd(cmdfile); 
		return "Out of memory.";
	}
	return NULL;
}

char *load_cmd(char *filename, cmd_desc *cmdfile)
{
	FILE *fp;
	char *boo;
	byte cmd_header[128];

	fp = fopen(filename, "rb");
	if (!fp) return "Failed to open file.";

	if (fread(cmd_header, 1, sizeof(cmd_header), fp) < sizeof(cmd_header)) 	
	{
		fclose(fp);
		return "Too short to be a CMD file.";
	}
	cmdfile->cs_len  = peekw(cmd_header, 1);
	cmdfile->cs_base = peekw(cmd_header, 3);
	cmdfile->cs_max  = peekw(cmd_header, 5);
	cmdfile->ds_len  = peekw(cmd_header, 10);
	cmdfile->ds_base = peekw(cmd_header, 12);
	cmdfile->ds_max  = peekw(cmd_header, 14);
	
	if (!cmdfile->cs_len || !cmdfile->ds_len) 
	{
		fclose(fp);
		return "Not a 2-segment CMD file";
	}
	boo = alloc_cmd(cmdfile, 16 * cmdfile->cs_max, 16 * cmdfile->ds_max);
	if (boo)
	{
		fclose(fp);
		return boo;
	}
	if (fread(cmdfile->cs, 16, cmdfile->cs_len,  fp) < cmdfile->cs_len)
	{
		fclose(fp);
		free_cmd(cmdfile);
		return "Failed to load code segment.";
	}
	if (fread(cmdfile->ds, 16, cmdfile->ds_len,  fp) < cmdfile->ds_len)
	{
		fclose(fp);
		free_cmd(cmdfile);
		return "Failed to load data segment.";
	}
	fclose(fp);
	cmdfile->cs_len *= 16L;
	cmdfile->ds_len *= 16L;
	cmdfile->cs_max *= 16L;
	cmdfile->ds_max *= 16L;
	return NULL;
}

void pokew(byte *base, word a, word v)
{
	base[a  ] = (v) & 0xFF;
	base[a+1] = (v >> 8) & 0xFF;
}

word peekw(byte *base, word a)
{
        return base[a] | (((word)base[a+1]) << 8);
}


char *save_cmd(char *cmdname, cmd_desc *cmdfile, int absolute)
{
	FILE *fp;
	word csparas, cmparas;
	word dsparas, dmparas;
	byte header[128];

	csparas = cmdfile->cs_len >> 4;
	dsparas = cmdfile->ds_len >> 4;
	cmparas = cmdfile->cs_max >> 4;
	dmparas = cmdfile->ds_max >> 4;

	memset(header, 0, sizeof(header));
	header[0] = 1;	/* Code segment */
	pokew(header, 1, csparas);
	if (absolute) pokew(header, 3, cmdfile->cs_base);
	pokew(header, 5, cmparas);
	header[9] = 2;	/* Data segment */
	pokew(header, 10, dsparas);
	if (absolute) pokew(header, 12, cmdfile->ds_base);
	pokew(header, 14, dmparas);

	fp = fopen(cmdname, "wb");
	if (!fp) 
	{
		return "Cannot open file to write.";
	}
	if (fwrite(header, 1, 128, fp) < 128      ||
            fwrite(cmdfile->cs, 16, csparas, fp) < csparas ||
	    fwrite(cmdfile->ds, 16, dsparas, fp) < dsparas)
	{
		fclose(fp);
		return "Error writing to CMD file";
	}
	/* Pack out to a multiple of 128 bytes */
	while ((csparas + dsparas) & 7)
	{
		fwrite(zeroes, 1, 16, fp);
		++dsparas;
	}	
	fclose(fp);
	return NULL;
}

