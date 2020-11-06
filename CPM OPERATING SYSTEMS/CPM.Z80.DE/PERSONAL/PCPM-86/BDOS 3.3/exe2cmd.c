/*    Convert the Personal CP/M-86 BDOS (compiled as a small-model EXE) 
 *    to CMD.
 *    Copyright (C) 2003, 2007  John Elliott <jce@seasip.demon.co.uk>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Library General Public
 *    License as published by the Free Software Foundation; either
 *    version 2 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Library General Public License for more details.
 *
 *    You should have received a copy of the GNU Library General Public
 *    License along with this library; if not, write to the Free
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <string.h>

#define DS_LEN 0xF0	/* Length of data segment (paras) */

int gl_base = 0x60;

/* EXE header (plus first 8 bytes of EXE file) */
unsigned char exe_header[0x208];

/* CMD header */
unsigned char cmd_header[0x80];


/* Read a word from the EXE header */
unsigned short peek(int offset)
{
    unsigned short l = exe_header[offset];
    unsigned short h = exe_header[offset + 1];
    return (h << 8) | l;
}

/* Add a group to the CMD header */
void addgroup(int type, unsigned short paras, unsigned short base,
			  unsigned short minparas, unsigned short maxparas)
{
	static unsigned char *group = cmd_header;
	*group++ = type;
	*group++ = (paras & 0xFF);
	*group++ = (paras >> 8);
	*group++ = (base & 0xFF);
	*group++ = (base >> 8);
	*group++ = (minparas & 0xFF);
	*group++ = (minparas >> 8);
	*group++ = (maxparas & 0xFF);
	*group++ = (maxparas >> 8);
	printf("Group %d: len=%04x base=%04x min=%04x max=%04x\n",
			type, paras, base, minparas, maxparas);
}

/* Copy bytes from one file to another. 
 * TODO: Warn if error. */
void mvbytes(FILE *fpexe, FILE *fpcmd, long count)
{
	int c;
	
	while (count)
	{
		c = fgetc(fpexe);
		fputc(c, fpcmd);
		--count;
	}
}


int main(int argc, char **argv)
{
	FILE *fpexe, *fpcmd;
	unsigned short cseg,dseg,sseg,dstop;
	unsigned short dsmin, dsmax, exeparas;
	unsigned long exelen, dsbytes;
	long pos;
	int n;

	if (argc < 3)
	{
		fprintf(stderr, "Syntax: exe2cmd <exefile> <cmdfile>\n");
		return 1;
	}
        for (n = 3; n < argc; n++)
        {
                if (!strncmp(argv[n], "base=", 5))
                {
                        if (!sscanf(&argv[n][5], "%x", &gl_base))
                        {
				fprintf(stderr, "base= option incorrect.\n");
				return 1;
			}
		}
	}

	/* Load EXE header, and extract the segment info. This code depends
	 * on the word at CS:6 being the length of the code segment in 
	 * paragraphs. */
	fpexe = fopen(argv[1], "rb");
	if (!fpexe)
	{
		perror(argv[1]);
		return 1;
	} 
	if (fread(exe_header, 1, sizeof(exe_header), fpexe) < (int)sizeof(exe_header) 
	/* Check for CP/M-86 BDOS:
	 *   32 header paragraphs
	 *   First relocation is at address 6 or less */
	|| peek(8) != 32 || peek(32) > 6)
	{
		fclose(fpexe);
		fprintf(stderr, "%s is not a BDOS image\n", argv[1]);
		return 1; 
	}
	cseg = 0;
	dseg = peek(0x206);
	sseg = peek(14);
	exelen = 512L * peek(4);
	if (peek(2)) exelen -= (512 - peek(2));
	exeparas = (exelen + 15) >> 4;
/*
	dstop = exeparas - peek(8);
	ssparas = dstop - sseg;
	dsmin = dstop - dseg + peek(10);
	dsmax = dstop - dseg + peek(12);
*/
	/* This is CP/M-86, the DSEG is a fixed size */
	dsmin = dsmax = DS_LEN;
	dstop = dseg + DS_LEN;
	printf("EXE length     = %04lx\n", exelen);
	printf("EXE header len = %04x\n", peek(8));
	printf("EXE paras      = %04x\n", exeparas);
	printf("DS top         = %04x\n", dstop);
	printf("CS: %04x length %04x\n", cseg, dseg  - cseg);
	printf("DS: %04x length %04x\n", dseg, dstop - dseg);

	/* Now generate CP/M-86 header. */
	addgroup(1, dseg  - cseg, 0, dseg - cseg, 0);
	addgroup(2, dstop - dseg, 0, dsmin, 0);

	fpcmd = fopen(argv[2], "wb");
	if (fpcmd == NULL)
	{
		perror(argv[2]);
		fclose(fpexe);
		return 2;
	}
	if (fwrite(cmd_header, 1, sizeof(cmd_header), fpcmd) < (int)sizeof(cmd_header)
	||  fwrite(exe_header+0x200, 1, 8, fpcmd) < 8)
	{
		fclose(fpcmd);
		fclose(fpexe);
		perror(argv[2]);
		return 2;
	}
	/* Write the code segment. The first 8 bytes have been done. */
	mvbytes(fpexe, fpcmd, 16L * (dseg - cseg) - 8);
	/* Write the data segment. */
	dsbytes = 16L * (dstop - dseg);
	mvbytes(fpexe, fpcmd, 40);
	/* Apply fixups to offsets within the data segment */
	for (n = 0; n < 8; n++)
	{
		unsigned seg;
		if (n == 6)
		{
			mvbytes(fpexe, fpcmd, 4);/* Address of DOS module */
			continue;
		}
		mvbytes(fpexe, fpcmd, 2);	/* Offset */
		fread(exe_header, 1, 2, fpexe);
		seg = peek(0) + gl_base;
		fputc(seg & 0xFF, fpcmd);
		fputc(seg >> 8, fpcmd);
	}
	mvbytes(fpexe, fpcmd, dsbytes - 72);
/* Pad the CMD file out to a whole number of records */
	pos = ftell(fpcmd);
	while (pos & 0x7F)
	{
		fputc(0, fpcmd);
		++pos;
	}
	fclose(fpcmd);
	fclose(fpexe);
	return 0;
}
