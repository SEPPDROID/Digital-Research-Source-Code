#define VN "1.0 02/11/83"
/* FIND - This utility displays either a map showing on which disks and
   in which user numbers files matching the specified ambiguous
   file name are found, or it can display the actual names matched. */

#include <LIBRARY.H>

struct _dirpb dir_pb;		/* directory management parameter block */
struct _dir *dir_entry;		/* pointer to directory entry (somewhere in
			   dir_pb) */
struct _scb scb;		/* search control block */

char file_name[20];		/* formatted for display : un/d:FILENAME.TYP */

short cur_disk;			/* current logical disk at start of program */
int mcount;			/* match count (no. of file names matched) */
int dmcount;			/* per disk match count */
int lcount;			/* line count (for lines displayed) */

int map_flag;			/* 0 = show file names of matched files,
		   NZ = show map of number of files */

	/* The array below is used to tabulate the results for each
	   disk drive, and for each user number on the drive.
	   In addition, two extra "users" have been added for "Free"
	   and "Used" values. */

unsigned disk_map[16][18];	/* Disk A -> P, Users 0 -> 15, Free, Used */
#define USED_COUNT 16		/* "user" number for Used entities */
#define FREE_COUNT 17		/* "user" number for Free entities */


main(argc,argv)
short argc;		/* argument count */
char *argv[];		/* argument vector (pointer to an array of chars) */
{

printf("\nFIND Version %s (Library %s)",VN,LIBVN);
chk_use(argc);			/* check usage */
cur_disk = bdos(GETDISK);	/* get current default disk */

dm_clr(disk_map);		/* reset disk map */

	/* set search control block 
	   disks, name, type, user number, extent number,
	   and number of bytes to compare - in this case, match all users,
	   but only extent 0 */
setscb(scb,argv[1],'?',0,13);	/* set disks, name, type */

map_flag = usstrcmp("NAMES",argv[2]);	/* set flag for map option */

lcount = dmcount = mcount = 0;		/* initialize counts */

for (scb.scb_disk = 0;		/* starting with logical disk A: */
     scb.scb_disk < 16;		/* until logical disk P: */
     scb.scb_disk++)		/* move to next logical disk */
	{

		/* check if current disk has been selected for search */
	if (!(scb.scb_adisks & (1 << scb.scb_disk)))
		continue;	/* no - so bypass this disk */

	printf("\nSearching disk : %c",(scb.scb_disk + 'A'));
	lcount++;		/* update line count */

	dir_pb.dp_disk = scb.scb_disk;	/* set to disk to be searched*/
	dmcount = 0;			/* reset disk matched count */

	if (!map_flag)		/* if file names are to be displayed */
		putchar('\n');	/* move to column 1 */

	/* set the directory to "closed", and force the get_nde
	   function to open it. */
	dir_pb.dp_open = 0;

		/* while not at the end of the directory, set a pointer to the
		   next directory entry. */
	while(dir_entry = get_nde(dir_pb))
		{
		/* check if entry in use in order to update
		   the free/used counts */

		if (dir_entry -> de_userno == 0xE5)	/* unused */
			disk_map[scb.scb_disk][FREE_COUNT]++;
		else	/* in use */
			disk_map[scb.scb_disk][USED_COUNT]++;

		/* select only those active entries that are the
		   first extent (numbered 0) of a file that matches
		   the name supplied by the user. */
		if (
		    (dir_entry -> de_userno != 0xE5) &&
		    (dir_entry -> de_extent == 0) &&
		    (comp_fname(scb,dir_entry) == NAME_EQ)
		   )
			{

			mcount++;	/* update matched counts */
			dmcount++;	/* per disk count */

			if (map_flag)	/* check map option */
				{
					/* update disk map */
				disk_map[scb.scb_disk][dir_entry -> de_userno]++;
				}
			else		/* display names */
				{
				conv_dfname(scb.scb_disk,dir_entry,file_name);
				printf("%s  ",file_name);

					/* check if need to start new line */
				if (!(dmcount % 4))
					{
					putchar('\n');
					if (++lcount > 18)
						{
						lcount = 0;
						printf("\nPress Space Bar to continue....");
						getchar();
						putchar('\n');
						}
					}
				}
			}
		} /* end of directory */
	} /* all disks searched */

if (map_flag)
{
printf("\n                Numbers show files in each User Number.");
printf("\n                          --- User Numbers ---                     Dir. Entries");

dm_disp(disk_map,scb.scb_adisks);	/* display disk map */
}

if (mcount == 0)
printf("\n --- File Not Found --- ");

bdos(SETDISK,cur_disk);	/* reset to current disk */
}


chk_use(argc)		/* check usage */
/* This function checks that the correct number of
   parameters has been specified, outputting instructions
   if not.
*/
/* Entry Parameter */
int argc;	/* Count of the number of arguments on the command line */
{

/* The minimum value of argc is 1 (for the program name itself),
   so argc is always one greater than the number of parameters
   on the command line */

if (argc == 1 || argc > 3)
{
printf("\nUsage :");
printf("\n\tFIND d:filename.typ {NAMES}");
printf("\n\t     *:filename.typ (All disks)");
printf("\n\t     ABCD..OP:filename.typ (Selected Disks)");
printf("\n\tNAMES option shows actual names rather than map.");
exit();
}
 
}
