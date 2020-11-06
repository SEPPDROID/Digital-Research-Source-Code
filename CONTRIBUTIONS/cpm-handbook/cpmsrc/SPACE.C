#define VN "1.0 02/11/83"
/* SPACE - This utility displays a map showing on the amount of space
   (expressed as relative percentages) occupied in each user number
   for each logical disk). It also shows the relative amount of space
   free. */

#include <LIBRARY.H>

struct _dirpb dir_pb;		/* directory management parameter block */
struct _dir *dir_entry;		/* pointer to directory entry */
struct _scb scb;		/* search control block */
struct _dpb dpb;		/* CP/M's disk parameter block */

char file_name[20];		/* formatted for display : un/d:FILENAME.TYP */

short cur_disk;		/* current logical disk at start of program
			   NZ = show map of number of files */
int count;		/* used to access the allocation block numbers
			   in each directory entry */
int user;		/* used to access the disk map when calculating */

/* The array below is used to tabulate the results for each
   disk drive, and for each user number on the drive.
   In addition, two extra "users" have been added for "Free"
   and "Used" values.
*/
unsigned disk_map[16][18];	/* Disk A -> P, Users 0 -> 15, Free, Used */
#define USED_COUNT 16		/* "user" number for Used entities */
#define FREE_COUNT 17		/* "user" number for Free entities */


main(argc,argv)
short argc;		/* argument count */
char *argv[];		/* argument vector (pointer to an array of chars) */
{

printf("\nSPACE Version %s (Library %s)",VN,LIBVN);
chk_use(argc);			/* check usage */
cur_disk = bdos(GETDISK);	/* get current default disk */

dm_clr(disk_map);		/* reset disk map */

ssetscb(scb,argv[1]);		/* special version : set disks, 
				   name, type */

for (scb.scb_disk = 0;		/* starting with logical disk A: */
     scb.scb_disk < 16;		/* until logical disk P: */
     scb.scb_disk++)		/* move to next logical disk */
	{

	/* check if current disk has been selected for search */
	if (!(scb.scb_adisks & (1 << scb.scb_disk)))
		continue;	/* no - so bypass this disk */

	printf("\nSearching disk : %c",(scb.scb_disk + 'A'));

	dir_pb.dp_disk = scb.scb_disk;	/* set to disk to be searched*/

	/* set the directory to "closed", and force the get_nde
	   function to open it. */
	dir_pb.dp_open = 0;

	/* while not at the end of the directory, set a pointer
	   to the next entry in the directory */
	while (dir_entry = get_nde(dir_pb))
		{
		if (dir_entry -> de_userno == 0xE5)
			continue;	/* bypass inactive entries */

		for (count = 0;		/* start with the first alloc. block */
		     count < dir_pb.dp_nabpde;	/* for number of alloc. blks per dir. entry */
		     count++)
			{
			if (dir_pb.dp_nabpde == 8)	/* assume 8 2-byte numbers */
				{
				disk_map[scb.scb_disk][dir_entry -> de_userno]
					+= (dir_entry -> _dirab.de_long[count] > 0 ? 1 : 0);
				}
			else	/* assume 16 1-byte numbers */
				{
				disk_map[scb.scb_disk][dir_entry -> de_userno]
					+= (dir_entry -> _dirab.de_short[count] > 0 ? 1 : 0);
				}
			}	/* all allocation blocks processed */
		}	/* end of directory for this disk */


	/* Compute the storage used by multiplying the number of
	   allocation blocks counted by the number of Kbytes in
	   each allocation block. */

	for (user = 0;	/* start with user 0 */
	     user < 16;	/* end with user 15 */
	     user ++)	/* move to next user number */
		{
			/* compute size occupied in Kbytes */
		disk_map[scb.scb_disk][user] *= dir_pb.dp_absize;
			/* build up sum for this disk */
		disk_map[scb.scb_disk][USED_COUNT] += disk_map[scb.scb_disk][user];
		}

	/* free space = (# of alloc. blks * # of kbyte per blk) 
		- used Kbytes
		- (directory entries * 32) / 1024 ... or divide by 32 */
	disk_map[scb.scb_disk][FREE_COUNT] = (dir_pb.dp_nab * dir_pb.dp_absize)
		- disk_map[scb.scb_disk][USED_COUNT]
		- (dir_pb.dp_nument >> 5);	/* same as / 32 */
	}	/* all disks processed */


printf("\n                  Numbers show space used in kilobytes.");
printf("\n                          --- User Numbers ---                       Space (Kb)");

dm_disp(disk_map,scb.scb_adisks);	/* display disk map */


bdos(SETDISK,cur_disk);	/* reset to current disk */
}
	
ssetscb(scb,ldisks)	/* special version of -  set search control block */
/* This function sets up a search control block according
   to just the logical disks specified. The disk are specified as 
   a single string of characters without any separators. An
   asterisk means "all disks". For example -

	ABGH	(disks A:, B:, G: and H: )
	*	(all disks for which SELDSK has tables)

   It sets the bit map according to which disks should be searched.
   For each selected disk, it checks to see if an error is generated
   when selecting the disk (i.e. if there are disk tables in the BIOS
   for the disk).
   The file name, type and extent number are all set to '?' to match
   all possible entries in the directory. */

/* Entry Parameters */
struct _scb *scb;	/* pointer to search control block */
char *ldisks;		/* pointer to the logical disks */

/* Exit Parameters
   None.
*/
{
int disk;		/* disk number currently being checked */
unsigned adisks;	/* bit map for active disks */

adisks = 0;		/* assume no disks to search */

if (*ldisks)		/* some values specified */
	{
	if (*ldisks == '*')	/* check if "all disks" */
		{
		adisks = 0xFFFF;	/* set all bits */
		}
	else			/* set specific disks */
		{
		while(*ldisks)	/* until end of disks reached */
			{
			/* build the bit map by getting the next disk
			   id. (A - P), converting it to a number
			   in the range 0 - 15, and shifting a 1-bit
			   left that many places and OR-ing it into
			   the current active disks.
			*/
			adisks |= 1 << (toupper(*ldisks) - 'A');
			++ldisks;	/* move to next character */
			}
		}
	}
else	/* use only current default disk */
	{
	/* set just the bit corresponding to the current disk */
	adisks = 1 << bdos(GETDISK);
	}

	/* set the user number, file name, type and extent to '?'
	   so that all active directory entries will match */
		/*         0123456789012	*/
strcpy(&scb -> scb_userno,"?????????????");

	/* Make calls to the BIOS SELDSK routine to make sure that
	   all of the active disk drives indeed do have disk tables
	   for them in the BIOS. If they don't, turn off the corresponding
	   bits in the bit map. */


for (disk = 0;		/* start with disk A: */
     disk < 16;		/* until disk P: */
     disk++)		/* use next disk */
	{
	if ( !((1 << disk) & adisks) )	/* avoid unnecessary selects */
		continue;
	if (biosh(SELDSK,disk) == 0)	/* make BIOS SELDSK call */
		{			/* returns 0 if invalid disk */
		/* turn OFF corresponding bit in mask
		   by ANDing it with bit mask having
		   all the other bits set = 1. */
		adisks &= ((1 << disk) ^ 0xFFFF);
		}		
	}

scb -> scb_adisks = adisks;	/* set bit map in scb */

} /* end ssetscb */



chk_use(argc)		/* check usage */
/* This function checks that the correct number of
   parameters has been specified, outputting instructions
   if not. */

/* Entry Parameter */
int argc;	/* Count of the number of arguments on the command line */
{

	/* The minimum value of argc is 1 (for the program name itself),
	   so argc is always one greater than the number of parameters
	   on the command line */

if (argc != 2)
	{
	printf("\nUsage :");
	printf("\n\tSPACE *        (All disks)");
	printf("\n\tSPACE ABCD..OP (Selected Disks)");
	exit();
	}
} /* end chk_use */
