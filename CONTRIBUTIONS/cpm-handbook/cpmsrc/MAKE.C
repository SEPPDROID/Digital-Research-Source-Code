#define VN "1.0 02/12/83"
/* MAKE - This utility is really two very similar programs 
   according to the parameter specified on the command line.

   INVISIBLE finds all of the specified files and moves them
   to user number 0 and sets them to be System and Read Only
   status. These files can then be accessed from user numbers
   other than 0 when the public files feature is enabled in the
   BIOS.

   VISIBLE is the "opposite" in that the specified files are
   moved to the current user number and changed to Directory 
   and Read Write status. */

#include <LIBRARY.H>

struct _dirpb dir_pb;		/* directory management parameter block */
struct _dir *dir_entry;		/* pointer to directory entry */
struct _scb scb;		/* search control block */
short to_user;			/* user number to which files will be set */
short from_user;		/* user number from which files will be moved */

char file_name[20];		/* formatted for display : un/d:FILENAME.TYP */
short name_flag;		/* NZ to display names of files moved */

short cur_disk;			/* current logical disk at start of program */

int mcount;			/* match count (no. of file names matched) */

short invisible;		/* NZ when parameter specifies Invisible */
char *operation;		/* pointer to either "Invisible" or "Visible" */

main(argc,argv)
short argc;		/* argument count */
char *argv[];		/* argument vector (pointer to an array of chars) */
{

printf("\nMAKE Version %s (Library %s)",VN,LIBVN);
chk_use(argc);			/* check usage */
cur_disk = bdos(GETDISK);	/* get current default disk */
mcount = 0;			/* initialize count */


	/* set the invisible flag according to the parameter */
if (usstrcmp("VISIBLE",argv[2]) == 0)
	invisible = 0;
else if (usstrcmp("INVISIBLE",argv[2]) == 0)
	invisible = 1;
else
	{
	printf("\n\007Error : '%s' can only be INVISIBLE/VISIBLE.",argv[2]);
	exit();
	}

	/* set the from_user and to_user numbers depending on which
	   program is to be built, and the parameters specified. */

if (invisible)
	{
	from_user = bdos(GETUSER,0xFF);	/* get current user number */
	to_user = 0;	/* always move files to user 0 */
	operation = "Invisible";	/* set pointer to string */
	}
else	/* visible */
	{
	from_user = 0;			/* always move from user 0 */
	to_user = bdos(GETUSER,0xFF);	/* get current user */
	operation = "Visible";		/* set pointer to string */
	}

	/* set search control block disks, name, type, user number,
	   extent number and number of bytes to compare - in this
	   case, match the 'from' user, all extents. */
setscb(scb,argv[1],from_user,'?',13);	/* set disks, name, type */

name_flag = usstrcmp("NAMES",argv[3]);	/* set name suppress flag from param. 3 */

	/* to simplify the logic below, name_flag must be made
	   NZ if it is equal to NAME_EQ, 0 if it is any other value */
name_flag = (name_flag == NAME_EQ ? 1 : 0);


	/* convert search user number and name for output */
conv_dfname(scb.scb_disk,scb,file_name);
printf("\n\nMoving files from User %d to %d and making them %s.",
	from_user,to_user,operation);

for (scb.scb_disk = 0;		/* starting with logical disk A: */
     scb.scb_disk < 16;		/* until logical disk P: */
     scb.scb_disk++)		/* move to next logical disk */
	{

		/* check if current disk has been selected for search */
	if (!(scb.scb_adisks & (1 << scb.scb_disk)))
		continue;	/* no - so bypass this disk */

	printf("\nSearching disk : %c",(scb.scb_disk + 'A'));

	dir_pb.dp_disk = scb.scb_disk;	/* set to disk to be searched*/

	if (name_flag)		/* if file names are to be displayed */
		putchar('\n');	/* move to column 1 */


		/* set the directory to "closed", and force the get_nde
		   function to open it. */
	dir_pb.dp_open = 0;

		/* while not at the end of the directory, 
		   set a pointer to the next directory entry. */
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
			mcount++;	/* update matched counts */

			if (invisible)
				{	/* set ms bits */
				dir_entry -> de_fname[8] |= 0x80;
				dir_entry -> de_fname[9] |= 0x80;
				}
			else	/* visible */
				{	/* clear ms bits */
				dir_entry -> de_fname[8] &= 0x7F;
				dir_entry -> de_fname[9] &= 0x7F;
				}

				/* move to correct user number */
			dir_entry -> de_userno = to_user;
		
				/* indicate sector to be written back */
			dir_pb.dp_write = 1;

				/* check if name to be displayed */
			if (name_flag)
				{
				conv_dfname(scb.scb_disk,dir_entry,file_name);
				printf("\n\t%s made %s in User %d.",
					file_name,operation,to_user);
				}
			}
		}	/* all directory entries processed */
	}	/* all disks processed */

if (mcount == 0)
	printf("\n --- No Files Processed --- ");

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

if (argc == 3 || argc == 4)
	return;
else
	{
	printf("\nUsage :");
	printf("\n\tMAKE d:filename.typ INVISIBLE {NAMES}");
	printf("\n\t                    VISIBLE");
	printf("\n\t     *:filename.typ (All disks)");
	printf("\n\t     ABCD..OP:filename.typ (Selected Disks)");
	printf("\n\tNAMES option shows names of files processed.");
	exit();
	}
 
}
	
