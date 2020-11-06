#define VN "1.0 02/10/83"
/* MOVE - This utility moves file(s) from one user number to another,
   but on the SAME logical disk. Files are not actually copied -
   rather their directory entries are changed. */

#include <LIBRARY.H>

struct _dirpb dir_pb;		/* directory management parameter block */
struct _dir *dir_entry;		/* pointer to directory entry */
struct _scb scb;		/* search control block */


#define DIR_BSZ 128		/* directory buffer size */
char dir_buffer[DIR_BSZ];	/* directory buffer */

char file_name[20];		/* formatted for display : un/d:FILENAME.TYP */
short name_flag;		/* NZ to display names of files moved */

short cur_disk;			/* current logical disk at start of program */
int from_user;			/* user number from which to move files */
int to_user;			/* user number to which files will be moved */

int mcount;			/* match count (no. of file names matched) */
int dmcount;			/* per disk match count */
int lcount;			/* line count (for lines displayed) */


main(argc,argv)
short argc;		/* argument count */
char *argv[];		/* argument vector (pointer to an array of chars) */
{

printf("\nMOVE Version %s (Library %s)",VN,LIBVN);

chk_use(argc);			/* check usage */

to_user = atoi(argv[2]);	/* convert user no. to integer */
	/* set and chk destination user number */
if(to_user > 15)
	{
	printf("\nError - the destination user number can not be greater than 15.");
	}

	/* set the current user number */
from_user = bdos(GETUSER,0xFF);

	/* check if source user number specified */
if (isdigit(argv[3][0]))
	{
		/* set and check source user number */
	if((from_user = atoi(argv[3])) > 15)
		{
		printf("\nError - the source user number can not be greater than 15.");
		exit();
		}

		/* set name suppress flag from parameter #4 */
	name_flag = usstrcmp("NAMES",argv[4]);
	}
else		/* no source user specified */
	{
		/* set name suppress flag from parameter #3 */
	name_flag = usstrcmp("NAMES",argv[3]);
	}

	/* to simplify the logic below, name_flag must be made
	   NZ if it is equal to NAME_EQ, 0 if it is any other value */
name_flag = (name_flag == NAME_EQ ? 1 : 0);

if (to_user == from_user)	/* to = from */
	{
	printf("\nError - 'to' user number is the same as the 'from' user number.");
	exit();
	}

	/* set the search control block file name, type, user number
	   extent number and length - length matches user number, file
	   name and type. As the extent number does not enter into the
	   comparison, all extents of a given file will be found. */
setscb(scb,argv[1],from_user,'?',13);

cur_disk = bdos(GETDISK);	/* get current default disk */
lcount = dmcount = mcount = 0;		/* initialize counts */

for (scb.scb_disk = 0;		/* starting with logical disk A: */
     scb.scb_disk < 16;		/* until logical disk P: */
     scb.scb_disk++)		/* move to next logical disk */
	{
		/* check if current disk has been selected for search */
	if (!(scb.scb_adisks & (1 << scb.scb_disk)))
		continue;	/* no - so bypass this disk */

		/* convert search user number and name for output */
	conv_dfname(scb.scb_disk,scb,file_name);
	printf("\n\nMoving file(s) %s -> User %d.",file_name,to_user);

	lcount++;		/* update line count */

	dir_pb.dp_disk = scb.scb_disk;	/* set to disk to be searched*/
	dmcount = 0;			/* reset disk matched count */

	if (name_flag)		/* if file names are to be displayed */
		putchar('\n');	/* move to column 1 */

		/* set the directory to "closed" to force the get_nde
		   function to open it. */
	dir_pb.dp_open = 0;

		/* while not at the end of the directory, set a pointer
		   to the next directory entry */
	while(dir_entry = get_nde(dir_pb))
		{
			/* match those entries that have the correct
			   user number, file name, type, and any
			   extent number. */
		if (
		    (dir_entry -> de_userno != 0xE5) &&
		    (comp_fname(scb,dir_entry) == NAME_EQ)
		   )
			{

			dir_entry -> de_userno = to_user;	/* move to new user */
				/* request sector to be written back */
			dir_pb.dp_write = 1;

			mcount++;	/* update matched counts */
			dmcount++;	/* per disk count */

			if (name_flag)	/* check map option */
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
		}
	}

if (mcount == 0)
	printf("\n --- No Files Moved --- ");

bdos(SETDISK,cur_disk);	/* reset to current disk */
}


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

if (argc == 1 || argc > 5)
	{
	printf("\nUsage :");
	printf("\n\tMOVE d:filename.typ to_user {from_user} {NAMES}");
	printf("\n\t     *:filename.typ (All disks)");
	printf("\n\t     ABCD..OP:filename.typ (Selected Disks)");
	printf("\n\tNAMES option shows names of files moved.");
	exit();
	}
 
}

