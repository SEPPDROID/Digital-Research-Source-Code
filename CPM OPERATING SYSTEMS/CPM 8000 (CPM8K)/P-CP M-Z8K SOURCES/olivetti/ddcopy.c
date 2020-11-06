/*=======================================================================*/
/*+---------------------------------------------------------------------+*/
/*|									|*/
/*|     Disk-Disk copy program for the OLIVETTI M20 (Z8000)		|*/
/*|									|*/
/*|     Copyright 1983, Zilog Incorporated.				|*/
/*|									|*/
/*+---------------------------------------------------------------------+*/
/*=======================================================================*/


char copyright[] = "Copyright 1983, Zilog Incorporated";


/* HISTORY
**
**	830221	F. Zlotnick (Zilog) -- with assembler routines stolen
**		from S. Savitzky's BIOS.
**
*/

/************************************************************************/ 



#define SECSIZ 	256
#define NSECS	16
#define TRKSIZE	NSECS*SECSIZ
#define NTRACKS	40

#define READ	0			/* Controller command	*/
#define WRITE	1			/* Controller command	*/

long	map_adr();			/* for physical addresses	*/

int	bigbuff[NSECS*SECSIZ*5];	/* Will hold 5 tracks, 2 sides each */
int	drive1,				/* Source drive name	*/
	drive2,				/* Target drive name	*/
	err;				/* error return from disk_io routine */

main(argc,argv)
int	argc;
char	*argv[];
{
	register int	i,j;

	if(argc != 3) usage();
	if( strcmp( argv[1], "a:") == 0 ) {
		drive1 = 0;
		if( strcmp( argv[2], "b:") != 0) usage();
		else drive2 = 1;
	}
	else if( strcmp( argv[1], "b:") == 0 ) {
		drive1 = 1;
		if( strcmp( argv[2], "a:") != 0) usage();
		else drive2 = 0;
	}
	else usage();
	if( dotrk0() ) {		/* Track 0 is special */
		printf("Error copying track 0\n");
		exit(1);
	}
	for( i = 1; i<= 40; i += 5)
	    if( j = do5trks(i)) {	/* Do the rest, 5 tracks at a time */
		printf( "Error %d on track %d\n", err, j);
		exit(1);
	}
}

/*
 * The following routine copies track 0, both sides
 */
dotrk0()
{
	register int j;
	long	physaddr;

	physaddr = map_adr( (long) bigbuff, 0);
	if( j = disk_io(drive1, READ, 16, 0, physaddr) )
		return(j);	/* Do the read... */
	if( j = disk_io(drive1, READ, 16, 16, physaddr + TRKSIZE ) )
		return(j);
	if( j = disk_io(drive2, WRITE, 16, 0, physaddr) )
		return(j); 	/* ...and the write */
	if( j = disk_io(drive2, WRITE, 16, 16, physaddr + TRKSIZE ) )
		return(j);
	return(0);
}

do5trks(i)
int	i;
{
	register int track, secnum;
	long physaddr, offset;

	physaddr = map_adr((long) bigbuff, 0);
	offset = 0L;
	for ( track = i; track < i + 5; track++) {
		secnum = track * 32;
		if(err = disk_io(drive1,READ,NSECS,secnum,physaddr + offset))
			return(track);
		secnum += NSECS;
		offset += TRKSIZE;
		if(err = disk_io(drive1,READ,NSECS,secnum,physaddr + offset))
			return(track);
		offset += TRKSIZE;
	}
	offset = 0L;
	for ( track = i; track < i + 5; track++) {
		secnum = track * 32;
		if(err = disk_io(drive2,WRITE,NSECS,secnum,physaddr + offset))
			return(track);
		secnum += NSECS;
		offset += TRKSIZE;
		if(err = disk_io(drive2,WRITE,NSECS,secnum,physaddr + offset))
			return(track);
		offset += TRKSIZE;
	}
	return(0);
}

/*
 * Usage routine tells user he's invoked command incorrectly
 */
usage()
{
	printf("usage:\n\tddcopy source dest\n");
	printf("where source is 'a:' and dest is 'b:' or vice versa.\n");
	exit(1);
}
