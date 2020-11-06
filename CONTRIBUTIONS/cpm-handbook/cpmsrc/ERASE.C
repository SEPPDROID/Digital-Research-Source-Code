#define VN "1.0 02/24/83"
/* ERASE
   This utility erases the specified file(s). It performs a logical
   erasure by using a BDOS Delete function. */

#include <LIBRARY.H>

struct _fcb amb_fcb;		/* ambiguous name file control block */
struct _fcb fcb;		/* used for BDOS Search Functions */

char file_name[20];		/* formatted for display : d:FILENAME.TYP */
short cur_disk;			/* current logical disk at start of program */
				/* ERASE saves up the FCB's of the all the
				   files that need to be erased in the
				   following array. */
#define MAXERA 1024
struct _fcb era_fcb[MAXERA];
int ecount;			/* count of number of files to be erased */
int count;			/* used to access era_fcb during erasing */

main(argc,argv)
short argc;		/* argument count */
char *argv[];		/* argument vector (pointer to an array of chars) */
{

printf("\nERASE Version %s (Library %s)",VN,LIBVN);
chk_use(argc);			/* check usage */
cur_disk = bdos(GETDISK);	/* get current default disk */

ecount = 0;			/* initialize count of files to erase */

setfcb(amb_fcb,argv[1]);	/* set ambiguous file name */
if (amb_fcb.fcb_disk)		/* check if default disk to be used */
	{
	bdos(SETDISK,amb_fcb.fcb_disk + 1);	/* set to specified disk */
	}

	/* convert ambiguous file name for output */
conv_fname(amb_fcb,file_name);
printf("\n\nSearching for file(s) matching %s.",file_name);

	/* set the file control block to indicate a "first" search */
fcb.fcb_disk |= 0x80;	/* OR in the ms bit */

	/* while not at the end of the directory, set the FCB
	   to the next name that matches */
while(get_nfn(amb_fcb,fcb))
	{
	conv_fname(fcb,file_name);
		/* ask whether to erase file or not */
	printf("\n\tErase %s y/n? ",file_name);
	if (toupper(getchar()) == 'Y')
		{
		printf(" <== Will be Erased!");
			/* add current fcb to array of fcb's */
		movmem(fcb,&era_fcb[ecount++],FCBSIZE);
			/* check that the table is not full */
		if (ecount == MAXERA)
			{
			printf("\nWarning : Internal Table now full. No more files can be erased");
			printf("\n   until those already specified have been erased.");
			break;	/* break out of while loop */
			}
		}
	}	/* all directory entries processed */

if (ecount)
	printf("\n\nErasing files now...");

	/* now process each fcb in the array, erasing the files */
for (count = 0;		/* starting with the first file in the array */
     count < ecount;	/* until all active entries processed */
     count++)		/* move to next fcb */
	{
	conv_fname(&era_fcb[count],file_name);	
	if (bdos(DELETEF,&era_fcb[count]) == -1)	/* error? */
		printf("\n\007Error trying to erase %s",file_name);
	else	/* file erased */
		printf("\n\tFile %s erased.",file_name);
	}
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

if (argc != 2)
	{
	printf("\nUsage :");
	printf("\n\tERASE {d:}file_name.typ");
	exit();
	}
 
}

