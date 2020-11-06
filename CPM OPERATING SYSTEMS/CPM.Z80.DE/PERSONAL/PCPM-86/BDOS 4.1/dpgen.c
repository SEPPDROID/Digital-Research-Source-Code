
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdio.h"

cmd_desc dosplus_sys, bdos_cmd, xios_cmd, ccp_cmd, dos_cmd;

int gl_base = 0x60;

void dump_cmd(char *caption, cmd_desc *cmd)
{
	printf("%s CS: base=%04x length=%05lx\n",
		caption, cmd->cs_base, cmd->cs_len);
	printf("%s DS: base=%04x length=%05lx\n",
		caption, cmd->ds_base, cmd->ds_len);
}

int main(int argc, char **argv)
{
	char *boo;
	long o = 0;
	long xios_cs, dos_cs, dos_ds, ccp_cs, ccp_ds;
	int n;
	/* Load the component parts */

	for (n = 1; n < argc; n++)
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

	boo = load_cmd("bdos.cmd", &bdos_cmd);
	if (boo) { fprintf(stderr, "bdos.cmd: %s\n", boo); return 1; }	
	boo = load_cmd("xios.cmd", &xios_cmd);
	if (boo) { fprintf(stderr, "xios.cmd: %s\n", boo); return 1; }	
	boo = load_cmd("ccp.cmd", &ccp_cmd);
	if (boo) { fprintf(stderr, "ccp.cmd: %s\n", boo); return 1; }	
	load_cmd("dos.cmd", &dos_cmd);

	/* Work out segment sizes */
	dosplus_sys.cs_max = 
	dosplus_sys.cs_len = bdos_cmd.cs_len + xios_cmd.cs_len + 
			     ccp_cmd.cs_len + ccp_cmd.ds_len + 
			     dos_cmd.cs_len + dos_cmd.ds_len;
	dosplus_sys.cs_base = gl_base;
	dosplus_sys.ds_len = xios_cmd.ds_len;
	dosplus_sys.ds_max = xios_cmd.ds_max;
	dosplus_sys.ds_base = gl_base + (dosplus_sys.cs_len >> 4);

	boo = alloc_cmd(&dosplus_sys, dosplus_sys.cs_len, dosplus_sys.ds_len);
	if (boo) { fprintf(stderr, "%s", boo); return 1; }	

	printf("Code segment at %04x:0000 length 0x%05lx\n", 
			dosplus_sys.cs_base, 
			dosplus_sys.cs_len);
	printf("Data segment at %04x:0000 length 0x%05lx\n", 
			dosplus_sys.ds_base, 
			dosplus_sys.ds_len);

	/* OK. Now copy the modules into the SYS file */

	memcpy(dosplus_sys.cs + o, bdos_cmd.cs, bdos_cmd.cs_len); 
	o += bdos_cmd.cs_len;
	xios_cs = o;
	memcpy(dosplus_sys.cs + o, xios_cmd.cs, xios_cmd.cs_len);
	o += xios_cmd.cs_len;
	if (dos_cmd.cs_len)
	{
		dos_cs = o;
		memcpy(dosplus_sys.cs + o, dos_cmd.cs, dos_cmd.cs_len);
		o += dos_cmd.cs_len;
	}
	ccp_cs = o;
	memcpy(dosplus_sys.cs + o, ccp_cmd.cs, ccp_cmd.cs_len);
	o += ccp_cmd.cs_len;
	ccp_ds = o;
	memcpy(dosplus_sys.cs + o, ccp_cmd.ds, ccp_cmd.ds_len);
	o += ccp_cmd.ds_len;
	if (dos_cmd.ds_len)
	{
		dos_ds = o;
		memcpy(dosplus_sys.cs + o, dos_cmd.ds, dos_cmd.ds_len);
		o += dos_cmd.ds_len;
	}
	memcpy(dosplus_sys.ds, xios_cmd.ds, xios_cmd.ds_len);
	memcpy(dosplus_sys.ds, bdos_cmd.ds, bdos_cmd.ds_len);

	/* Set up the pointers to modules correctly */
	pokew(dosplus_sys.cs, 6, dosplus_sys.ds_base);
	pokew(dosplus_sys.cs, xios_cs + 6, dosplus_sys.ds_base);
	if (dos_cmd.ds_len)
	{
		pokew(dosplus_sys.cs, dos_cs + 6, dosplus_sys.ds_base);
		pokew(dosplus_sys.cs, dos_cs + 8, (dos_ds >> 4) + gl_base); 
	}
	pokew(dosplus_sys.cs, ccp_ds    , ccp_cmd.cs_len);
	pokew(dosplus_sys.cs, ccp_ds + 3, (ccp_cs >> 4) + gl_base);
	pokew(dosplus_sys.cs, ccp_ds + 6, ccp_cmd.ds_len);
	pokew(dosplus_sys.cs, ccp_ds + 9, (ccp_ds >> 4) + gl_base);
	pokew(dosplus_sys.ds, 0x2A, (xios_cs >> 4) + gl_base);
	pokew(dosplus_sys.ds, 0x2E, (xios_cs >> 4) + gl_base);
	if (dos_cmd.ds_len)
		pokew(dosplus_sys.ds, 0x42, (dos_cs >> 4) + gl_base);
	else	pokew(dosplus_sys.ds, 0x42, 0);
	pokew(dosplus_sys.ds, 0x44, (ccp_ds >> 4) + gl_base);
	pokew(dosplus_sys.ds, 0x48, (dosplus_sys.cs_max + 
				     dosplus_sys.ds_max + 0x10 * gl_base) >> 4); 
	/* SYS file has been generated. Write it. */

	dump_cmd("BDOS ", &bdos_cmd);
	dump_cmd("XIOS ", &xios_cmd);
	if (dos_cmd.cs_len) dump_cmd("DOS  ", &dos_cmd);
	dump_cmd("CCP  ", &ccp_cmd);
	dump_cmd("SYS  ", &dosplus_sys);

	/* Now start writing the stuff out */
	boo           = save_cmd("new.sys", &dosplus_sys, 1);
	if (boo) fprintf(stderr, "new.sys: %s\n", boo);
	free_cmd(&dosplus_sys); 
	free_cmd(&bdos_cmd); 
	free_cmd(&xios_cmd);
	free_cmd(&ccp_cmd);
	free_cmd(&dos_cmd);
	return 0;
}


