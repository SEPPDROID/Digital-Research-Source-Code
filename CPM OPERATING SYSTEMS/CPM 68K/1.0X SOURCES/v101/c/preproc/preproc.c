#
#define	MMU	0
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/
#include "preproc.h"

struct symbol symtab[HSIZE]={0};

	/*buffered I/O structure*/
struct {
	int fd;
	int nc;
	char *bp;
	char buffer[512];
} inbuf[NINCS]={0}, outbuf={0};
int mfail=0;							/*macro error flag*/
int skip=0;							/*skipping current line*/
char *defap=0;						/*pointer to available define area*/
char *defp=0;							/*pointer to next available define byte*/
int defcount=0;						/*bytes left in define area*/
int defused=0;						/*number of bytes used in define area*/
int defmax=0;							/*maximum define area used*/
int pflag=0;
int asflag=0;

	/*line to output after macro substitution*/
char line[LINESIZE+2]={0};				/*line buffer*/
char *linep=0;						/*current line pointer*/
int loverflow=0;						/*line overflow flag*/

	/*push back buffer*/
char pbbuf[PBSIZE]={0};					/*push back buffer*/
char *pbp=0;							/*push back pointer*/
int pbflag=0;							/*checks for recursive definition*/
