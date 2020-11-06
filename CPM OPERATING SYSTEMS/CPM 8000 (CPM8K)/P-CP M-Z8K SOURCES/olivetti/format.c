/*
 * This program formats a 5 1/4 inch floppy disk in drive 0 or drive 1
 * of the Olivetti M20.  The format is ECMA standard for the headers.
 * The data fields are filled with 0xE5, which CP/M thinks is a new
 * IBM disk.  This gives the disk the appearance of having an empty
 * directory as far as CP/M is concerned.
 */

#include "stdio.h"

/* densities */
#define	SINGLE 0
#define DOUBLE 1

#define BADTRACK -1
#define MAXBAD 2

#define NTRACKS 40

/* Space for humongous buffer */
#define TRACKSIZ 10000
char	trakbuf[TRACKSIZ];
int	track,
	sector,
	side,
	density,
	drive;

int	intrlv[16] =   { 9, 1, 10, 2,
			11, 3, 12, 4,
			13, 5, 14, 6,
			15, 7, 16, 8};

extern	long map_adr();
long	physaddr;		/* Segmented address of trakbuf */

main(argc,argv)
int	argc;
char	*argv[];
{
	register char c, *p;

	physaddr = map_adr((long)trakbuf,0);	/* get seg addr	*/
	while(--argc) {
		p = *++argv;
		if (strcmp(p,"a:") == 0) {
			drive = 0;
			warn(p);
			format(p);
		}
		else if (strcmp(p,"b:") == 0) {
			drive = 1;
			warn(p);
			format(p);
		}
		else printf("Unknown drive: %s\n",p);
	}
}

/* Routine to format a disk in a given drive */
format(p)
char *p;
{
	register int i,j,k;
	int	badtracks;

	badtracks = 0;

/* Start with track 0; this track is special. */
	printf("Formatting track %2d",0);
	track = 0;
	density = SINGLE;
	if ( (j = make_zero()) == BADTRACK) {
		printf("Can't format track zero; bad diskette\n");
		return(-1);
	}

/* Now do the other tracks */
	density = DOUBLE;
	for(track = 1; track<=40; track++) {
		printf("%2d",track);
		if( (j = dotrack(track)) == BADTRACK) badtracks++;
		if (badtracks > MAXBAD) {
			printf("%d bad tracks; too many.\n",badtracks);
			return(-1);
		}
	}
	printf("\nFormatting of disk in drive %s complete.\n\n",p);
	return(badtracks);
}

/* Routine to do one track */
dotrack(track)
{
	register int j;

	if( (j = doside(0,track)) == BADTRACK) return(BADTRACK);
	if( (j = doside(1,track)) == BADTRACK) return(BADTRACK);
	return(track);
}

/* Routine to do one double density side */
doside(side,track)
int side,track;
{	register int j,sector;
	char *p;

	p = trakbuf;
	for(j = 0; j < 32; j++) *p++ = 0x4e;	/* Index Gap: 32 hx 4E's */
	for(sector = 0; sector < 16; sector++) {
		for(j = 0; j < 12; j++) *p++ = '\0';
		for(j = 0; j < 3; j++) *p++ = 0xf5;
		*p++ = 0xfe;
		*p++ = track;
		*p++ = side;
		*p++ = intrlv[sector]; /* use interleave table */
		*p++ = (char)DOUBLE;
		*p++ = 0xf7;	/* Puts two CRC bytes out */
		for(j = 0; j < 22; j++) *p++ = 0x4e;
		for(j = 0; j < 12; j++) *p++ = '\0';
		for(j = 0; j < 3; j++) *p++ = 0xf5;
		*p++ = 0xfb;
/* Magic with the data buffer: fill it with hex E5's, and CP/M will think */
/*  that it has an empty directory!					  */
		for(j = 0; j < 256; j++) *p++ = 0xe5;
		*p++ = 0xf7;	/* More CRC's */
		for(j = 0; j < 50; j++) *p++ = 0x4e;	/* sector gap */
	}
/* Now do the end of track gap: 400 hex 4E's	*/
	for(j = 0; j < 400; j++) *p++ = 0x4e;

/* Finally, use assembler magic to write the track image.	*/
	return(writ_im(drive,track,side,physaddr));
}

/* Routine to make track zero, which is special.  It's single-density
 * on side zero, and double density on side one.  Each of these has a
 * different header encoding standard.
 */

make_zero()
{
	register int j,sector;
	char *p;

	p = trakbuf;
	for( j = 0; j < 16; j++) *p++ = 0xff;	/* Index Gap */
	for(sector = 0; sector < 16; sector++) {
		for(j = 0; j < 6; j++) *p++ = '\0';
		*p++ = 0xfe;
		*p++ = 0;	/* Track # */
		*p++ = 0;	/* Side # */
		*p++ = intrlv[sector];
		*p++ = '\0';
		*p++ = 0xf7;		/* CRC */
		for(j = 0; j < 11; j++) *p++ = 0xff;
		for(j = 0; j < 6; j++) *p++ = '\0';
		*p++ = 0xfb;
		for(j = 0; j < 128; j++) *p++ = 0xe5;	/* Magic data */
		*p++ = 0xf7;		/* CRC */
		for(j = 0; j < 24; j++) *p++ = 0xff;	/* Sector Gap */
	}
	for(j = 0; j < 200; j++) *p++ = 0xff;	/* End-of-track Gap */
	j = writ_im(drive,0,0,physaddr);	/* Write side 0 special */
	if(j == BADTRACK) return(j);
	return(doside(1,0));			/* Write side 1 normal  */
}

warn(p)
char *p;
{
	register int c,cc;

	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
	printf("Warning:  This will destroy all files on disk %s!\n\n",p);
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
	printf("Do you wish to continue? ");
	c = getchar();
	if(c == '\n') exit(1);
	else while( (cc = getchar()) != '\n') ;
	if(c != 'y' && c != 'Y') exit(1);
	return(0);
}
