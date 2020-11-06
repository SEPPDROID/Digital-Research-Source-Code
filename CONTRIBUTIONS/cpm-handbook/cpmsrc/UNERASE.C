#define VN "1.0 02/12/83"
/* UNERASE - This does the inverse of ERASE. It revives the specified files,
   by changing the first byte of their directory entries from 0xE5
   back to the specified user number. */

#include <LIBRARY.H>

struct _dirpb dir_pb;		/* directory management parameter block */
struct _dir *dir_entry;		/* pointer to directory entry */
struct _scb scb;		/* search control block */
struct _scb scba;		/* scb setup to match all files */
struct _dpb dpb;		/* CP/M's disk parameter block */
struct _bv inuse_bv;		/* bit vector for blocks in use */
struct _bv file_bv;		/* bit vector for file to be unerased */
struct _bv extents;		/* bit vector for those extents unerased */

char file_name[20];		/* formatted for display : un/d:FILENAME.TYP */

short cur_disk;			/* current logical disk at start of program
		   NZ = show map of number of files */
int count;			/* used to access the allocation block numbers
		   in each directory entry */
int user;			/* user in which the file is to be revived */


main(argc,argv)
short argc;		/* argument count */
char *argv[];		/* argument vector (pointer to an array of chars) */
{

printf("\nUNERASE Version %s (Library %s)",VN,LIBVN);
chk_use(argc);			/* check usage */
cur_disk = bdos(GETDISK);	/* get current default disk */


	/* using a special version of set search control block,
	   set the disk, name, type (no ambiguous names), the user number
	   to match only erased entries, and the length to compare
	   the user, name and type.
	   This special version also returns the disk_id taken from
	   the file name on the command line.  */
if ((dir_pb.dp_disk = ssetscb(scb,argv[1],0xE5,12)) == 0)
	{	/* use default disk */
	dir_pb.dp_disk = cur_disk;
	}
else
	{	/* make disk A = 0, B = 1 (for SELDSK) */
	dir_pb.dp_disk--;
	}
printf("\nSearching disk %d.",dir_pb.dp_disk);

if(strscn(scb,"?"))	/* check if ambiguous name */
	{
	printf("\nError - UNERASE can only revive a single file at a time.");
	exit();
	}


	/* set up a special search control block that will match with
	   all existing files. */
ssetscb(scba,"*.*",'?',12);	/* set file name and initialize scb */


if (argc == 2)			/* no user number specified */
	user = bdos(GETUSER,0xFF);	/* get current user number */
else
{
	user = atoi(argv[2]);		/* get specified number */
	if (user > 15)
		{
		printf("\nUser number can only be between 0 - 15.");
		exit();
		}
}

/* build a bit vector that shows the allocation blocks
   currently in use. scba has been set up to match all
   active directory entries on the disk. */
build_bv(inuse_bv,scba);

/* build a bit vector for the file to be restored showing
   which allocation blocks will be needed for the file. */
if (!build_bv(file_bv,scb))
	{
	printf("\nNo directory entries found for file %s.",
		argv[1]);
	exit();
	}
/* perform a boolean AND of the two bit vectors. */
bv_and(file_bv,inuse_bv,file_bv);

/* check if the result is non-zero - if so, then one or more
   of the allocation blocks required by the erased file is
   already in use for an existing file and the file cannot
   be restored. */
if (bv_nz(file_bv))
{
	printf("\n--- This file cannot be restored as some parts of it");
	printf("\n    have been re-used for other files! ---");
	exit();
}

/* continue on to restore the file by changing all the entries
   in the directory to have the specified user number.
   Note : the problem is complicated by the fact that there
   may be several entries in the directory that are for the
   same file name and type, and even have the same extent
   number. For this reason, a bit map is kept of the extent
   numbers unerased - duplicate extent numbers will not be
   unerased. */

/* set up the bit vector for up to 127 unerased extents */
bv_make(extents,16);		/* 16 * 8 bits */


/* set the directory to "closed", and force the get_nde
   function to open it. */
dir_pb.dp_open = 0;

/* while not at the end of the directory, return a pointer to
   the next entry in the directory. */
while(dir_entry = get_nde(dir_pb))
{

	/* check if user = 0xE5 and name, type match */
if (comp_fname(scb,dir_entry) == NAME_EQ)
	{
		/* test if this extent has already been
		   unerased */
	if (bv_test(extents,dir_entry -> de_extent))
		{		/* yes it has */
		printf("\n\t\tExtent #%d of %s ignored.",
			dir_entry -> de_extent,argv[1]);
		continue;	/* do not unerase this one */
		}
	else			/* indicate this extent unerased */
		{
		bv_set(extents,dir_entry -> de_extent);
		dir_entry -> de_userno = user; /* unerase entry */
		dir_pb.dp_write = 1;	/* need to write sector back */
		printf("\n\tExtent #%d of %s unerased.",
			dir_entry -> de_extent,argv[1]);
		}
	}
}

printf("\n\nFile %s unerased in User Number %d.",
argv[1],user);

bdos(SETDISK,cur_disk);	/* reset to current disk */
}


build_bv(bv,scb)	/* build bit vector (from directory) */
/* This function scans the directory of the disk specified in
   the directory parameter block (declared as a global variable),
   and builds the specified bit vector showing all the allocation
   blocks used by files matching the name in the search control
   block. */

/* Entry Parameters */
struct _bv *bv;		/* pointer to the bit vector */
struct _scb *scb;	/* pointer to search control block */
/* also uses : directory parameter block (dir_pb) */

/* Exit Parameters
   The specified bit vector will be created, and will have 1-bits
   set wherever an allocation block is found in a directory
   entry that matches the search control block.
   It also returns the number of directory entries matched. */
{
unsigned abno;		/* allocation block number */
struct _dpb *dpb;	/* pointer to the disk parameter block in the BIOS */
int mcount;		/* match count of dir. entries matched */

mcount = 0;		/* initialize match count */
dpb = get_dpb(dir_pb.dp_disk);	/* get disk parameter block address */

/* make the bit vector with one byte for each 8 allocation
   blocks + 1 */
if (!(bv_make(bv,(dpb -> dpb_maxabn >>3)+1)))
	{
	printf("\nError - Insufficient memory to make a bit vector.");
	exit();
	}


/* set directory to "closed" to force the get_nde
   function to open it. */
dir_pb.dp_open = 0;

/* now scan the directory building the bit vector */
while(dir_entry = get_nde(dir_pb))
	{
		/* compare user number (which can legitimately be
		   0xE5), the file name and the type). */
	if (comp_fname(scb,dir_entry) == NAME_EQ)
		{
		++mcount;		/* update match count */
		for (count = 0;		/* start with the first alloc. block */
		    count < dir_pb.dp_nabpde;	/* for number of alloc. blks per dir. entry */
		    count++)
			{
				/* set the appropriate bit number for
				   each non-zero allocation block number */
			if (dir_pb.dp_nabpde == 8)	/* assume 8 2-byte numbers */
				{
				abno = dir_entry -> _dirab.de_long[count];
				}
			else	/* assume 16 1-byte numbers */
				{
				abno = dir_entry -> _dirab.de_short[count];
				}
			if (abno) bv_set(bv,abno); /* set the bit */
			}
		}
	}	
return mcount;		/* return number of dir entries matched */
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

if (argc == 1 || argc > 3)
	{
	printf("\nUsage :");
	printf("\n\tUNERASE {d:}filename.typ {user}");
		printf("\n\tOnly a single unambiguous file name can be used.)");
	exit();
	}
} /* end chk_use */


ssetscb(scb,fname,user,length)	/* special version of set search control block */
/* This function sets up a search control block according
   to the file name, type, user number and number of bytes to
   compare. 
   The file name can take the following forms :

	filename
	filename.typ
	d:filename.typ

   It sets the bit map according to which disks should be searched.
   For each selected disk, it checks to see if an error is generated
   when selecting the disk (i.e. if there are disk tables in the BIOS
   for the disk). */

/* Entry Parameters */
struct _scb *scb;	/* pointer to search control block */
char *fname;		/* pointer to the file name */
short user;		/* user number to be matched */
int length;		/* number of bytes to compare */

/* Exit Parameters
   Disk number to be searched. (A = 1, B = 2...)
*/
{
short disk_id;		/* disk number to search */

setfcb(scb,fname);	/* set search control block as though it
			   is a file control block. */
disk_id = scb -> scb_userno;	/* set disk_id before it gets overwritten
				   by the user number */
scb -> scb_userno = user;	/* set user number */
scb -> scb_length = length;	/* set number of bytes to compare */
return disk_id;
} /* end setscb */

