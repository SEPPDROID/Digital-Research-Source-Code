/* Library of commonly-used functions */
/* 09:00 02/24/83 */

#include <LIBRARY.H>		/*  standard defines and structures */

/*	Configuration Block Access 	*/

/*==============================================================*/
char
*get_cba(code)			/* get configuration block address */
/*==============================================================*/
/* This function makes a call to a "private" entry in the BIOS
   Jump vector to return the address of a specific data object in
   the BIOS. The code indicates which object is required.
   Strictly speaking, each program using this function could make
   a direct call to the BIOS using the biosh() function provided
   by BDS C. This function serves a common point to which debug
   code can be added to display the addresses handed back. */
/* Entry Parameters */
int code;	/* code that specifies the object whose address is required */

/* Exit Parameters
	The address returned by the BIOS routine. */
{
char *retval;		/* value returned by the BIOS */

	retval = biosh(CBGADDR,code);
 /* printf("\nget_cba : code %d address %4x",code,retval); */
	return retval;
}

	

/*	Character manipulation functions	*/

/*==============================================================*/
strscn(string,key)		/* String Scan */
/*==============================================================*/
/* This function scans a 00-terminated character string looking
   for a key string in it. If the key string is found withing the
   string, the function returns a pointer to it, otherwise it
   returns a value of 0. */
/* Entry Parameters */
char *string;		/* string to be searched */
char *key;		/* key string to be searched for */

/* Exit Parameters
   Pointer to key string within searched string, or
   0 if key not found.
*/

{
while (*string)		/* for all non-null chars in string */
	{
	if ((*string == *key) &&	/* first char matches */
	    (sstrcmp(string,key) == 0)	/* perform substring compare on rest */
	   )
		return string;		/* substring matches, return pointer */

	string++;			/* move to next char in string */
	}
return 0;		/* indicate no match found */
} /* end of strscn */


/*==============================================================*/
ustrcmp(string1,string2)		/* UPPER CASE string compare */
/*==============================================================*/
/* This function is similar to the normal strcmp function;
   it differs only in that the characters are folded to upper
   case before comparison is made. */
/* Entry Parameters */
char *string1;		/* pointer to first string */
char *string2;		/* pointer to second string */

{
/* Exit Parameters
   0 - if string 1 = string 2
   -ve integer if string 1 > string 2
   +ve integer if string 1 < string 2
*/
int count;		/* used to access chars in both strings */

count = 0;	/* start with the first character of both */

	/* while string 1 characters are non-null, and
	   match their counterparts in string 2. */
while (string1[count] == string2[count])
	{
	if (string1[++count] == '\0')	/* last char in string1 */
		return 0;		/* indicate equality */
	}
return string2[count] - string1[count];	/* "compare" chars */
} /* end of sstrcmp */



/*==============================================================*/
sstrcmp(string,substring)		/* substring compare */
/*==============================================================*/
/* This function compares two strings. The first, string, need not
   be 00-terminated. The second, substring, must be 00-terminated.
   It is very similar to the standard function strcmp, except
   that in this function, the length of the substring controls
   how many characters are compared. */
/* Entry Parameters */
char *string;		/* pointer to main string */
char *substring;	/* pointer to substring */

/* Exit Parameters
   0 - substring matches corresponding characters in string
   -ve integer if char in string is > char	in substring
   +ve integer if char in string is < char in substring
*/
{
int count;		/* used to access chars in both string & substring */

count = 0;	/* start with the first character of both */

	/* while substring characters are non-null, and
	   match their counterparts in string. */
while (string[count] == substring[count])
	{
	if (substring[++count] == '\0')	/* last char in substring */
		return 0;		/* indicate equality */
	}
return substring[count] - string[count];	/* "compare" chars */
} /* end of sstrcmp */



/*==============================================================*/
usstrcmp(string,substring)		/* UPPER CASE substring compare */
/*==============================================================*/
/* This function compares two strings. The first, string, need not
   be 00-terminated. The second, substring, must be 00-terminated.
   It is very similar to the substring compare above, except all
   characters are folded to UPPER CASE. */
/* Entry Parameters */
char *string;		/* pointer to main string */
char *substring;	/* pointer to substring */

/* Exit Parameters
   0 - substring matches corresponding characters in string
   -ve integer if char in string is > char	in substring
   +ve integer if char in string is < char in substring
*/
{
int count;		/* used to access chars in both string & substring */

count = 0;	/* start with the first character of both */

	/* while substring characters are non-null, and
	   match their counterparts in string. */
while (toupper(string[count]) == toupper(substring[count]))
	{
	if (substring[++count] == '\0')	/* last char in substring */
		return 0;		/* indicate equality */
	}
return substring[count] - string[count];	/* "compare" chars */
} /* end of usstrcmp */


/*==============================================================*/
comp_fname(scb,name)		/* Compare file names */
/*==============================================================*/
/* This function compares a possibly ambiguous file name
   to the name in the specified character string. The number of
   bytes compared are determined by the number of characters in
   the mask.
   This function can be used to compare file names and types,
   or by appending an extra byte to the mask, the file names,
   types, and extent numbers.
   In the case of file directory entries, an extra byte can be
   prefixed to the mask and the function used to compare
   user number, file name, type and extent.
   Note that a '?' in the first character of the mask will NOT
   match with a value of 0xE5 (this value is used to indicate
   an inactive directory entry). */
/* Entry Parameters */
struct _scb *scb;	/* pointer to search control block */
char *name;		/* pointer to file name */

/* Exit Parameter
   NAME_EQ if the names match the mask
   NAME_LT if the name is less than the mask
   NAME_GT if the name is greater than the mask
   NAME_NE if the name is not equal to the mask (but the outcome
   	is ambiguous because of the wildcards in the mask)
*/

{
int count;		/* count of the number of chars processed */
short ambiguous;	/* NZ when the mask is ambiguous */
char *mask;		/* pointer to bytes at front of scb */


/* set pointer to characters at front of search control block */
mask = scb;

	/* ambiguous match on user number matches
	   only users 0 - 15, and not inactive entries */
if (mask[0] == '?')
	{
	if (name[0] == 0xE5)
		return NAME_NE;	/* indicate inequality */
	}
else	/* first char. of mask is not '?' */
	{
	if (mask[0] != name[0])	/* user numbers do not match */
		return NAME_NE; /* indicate inequality */
	}

/* no check the name (and, if the length is such, the extent */
for (count = 1;		/* start with first name character */
     count <= scb -> scb_length;	/* for all required characters */
     count++)		/* move to next character */
	{
	if (mask[count] == '?')	/* wildcard character in mask */
		{
		ambiguous = 1;	/* indicate ambiguous name in mask */
		continue;	/* do not make any comparisons */
		}
	if (mask[count] != (name[count] & 0x7F))
		{		/* mask char not equal to fcb char */
		if (ambiguous)	/* if previous wildcard, indicate NE */
			return NAME_NE;
		else
			/* compare chars to determine relationship */
			return (mask[count] > name[count] ?
				NAME_LT : NAME_GT);
		}
	}
	/* If control reaches here, then all characters of the
	mask and name have been processed, and either there
	were wildcards in the mask, or they all matched. */
return NAME_EQ;		/* indicate mask and name are "equal" */
} /* end of comp_fname */



/*==============================================================*/
conv_fname(fcb,fn)		/* Convert file name for output */
/*==============================================================*/
/* This function converts the contents of a file control
   block and converts into a printable string 'D:FILENAME.TYP'. */
/* Entry Parameters */
struct _fcb *fcb;		/* Pointer to file control block */
char *fn;			/* Pointer to area to receive name */

{
	/* if the disk specification in the 
	   fcb is 0, use the current disk */
*fn++ = (fcb -> fcb_disk) ? (fcb -> fcb_disk + ('A'-1)) :
	(bdos(GETDISK) + 'A');

*fn++ = ':';				/* insert disk id. delimiter */

movmem(&fcb -> fcb_fname,fn,8);		/* move file name  */
fn += 8;				/* update pointer */
*fn++ = '.';				/* insert file name/type delimiter */
movmem(&fcb -> fcb_fname+8,fn,3);	/* move file type */
*fn++ &= 0x7F;				/* remove any attribute bits */
*fn++ &= 0x7F;				/* remove any attribute bits */
*fn++ &= 0x7F;				/* remove any attribute bits */
*fn = '\0';				/* terminator */

} /*  end of conv_fname  */


/*==============================================================*/
conv_dfname(disk,dir,fn)	/* Convert directory file name for output */
/*==============================================================*/
/* This function converts the contents of a file directory entry
   block and converts into a printable string 'D:FILENAME.TYP'. */
/* Entry Parameters */
short disk;			/* Disk Id. (A = 0, B = 1) */
struct _dir *dir;		/* Pointer to file control block */
char *fn;			/* Pointer to area to receive name */

{
	/* convert user number and disk id. */
sprintf(fn,"%2d/%c:",dir -> de_userno,disk + 'A'); 
fn += 5;			/* update pointer to file name */

movmem(&dir -> de_fname,fn,8);		/* move file name  */
fn += 8;				/* update pointer */
*fn++ = '.';				/* insert file name/type delimiter */
movmem(&dir -> de_fname+8,fn,3);	/* move file type */
*fn++ &= 0x7F;				/* remove any attribute bits */
*fn++ &= 0x7F;				/* remove any attribute bits */
*fn++ &= 0x7F;				/* remove any attribute bits */
*fn = '\0';				/* terminator */

} /*  end of conv_dfname  */


/*==============================================================*/
get_nfn(amb_fname,next_fname)	/* get next file name */
/*==============================================================*/
/* This function sets the FCB at 'next_fname' to contain the 
   directory entry found that matches the ambiguous file name 
   in 'amb_fname'.
   On the first entry for a given file name, the most significant
   bit in the FCB's disk field must be set to 1  (this causes a 
   Search First BDOS call to be made). */

/* Entry Parameters */
struct _fcb *amb_fname;	/* ambiguous file name */
struct _fcb *next_fname;/* first byte must have ms bit set for
			   first time entry)*/

/* Exit Parameters
   0 = No further name found
   1 = Further name found (and setup in next_fname)
*/
{
char bdos_func;		/* set to either Search First or Next */
char *pfname;		/* pointer to file name in directory entry */

	/* initialize tail-end of next file FCB to 0 */
setmem(&next_fname -> fcb_extent,FCBSIZE-12,0);

bdos_func = SEARCHF;	/* assume a Search First must be given */

if (!(next_fname -> fcb_disk & 0x80))	/* if not first time */
	{
		/* Search First on previous name */
	srch_file(next_fname,SEARCHF);
	bdos_func = SEARCHN;		/* then do a Search Next */
	}
else	/* first time */
	next_fname -> fcb_disk &= 0x7F;	/* reset first time flag */

	/* refresh next_fname from ambiguous file name
	   (move disk, name, type) */
movmem(amb_fname,next_fname,12);

	/* if first time, issue Search First, otherwise
	   issue a Search Next call. 'srch_file' returns
	   a pointer to the directory entry that matches
	   the ambiguous file name, or 0 if no match */
if (!(pfname = srch_file(next_fname,bdos_func)) )
	{
	return 0;	/* indicate no match */
	}
	/* move file name and type */
movmem(pfname,&next_fname -> fcb_fname,11);
return 1;		/* indicate match found */
} /*  end of get_nfn  */


/*==============================================================*/
char *srch_file(fcb,bdos_code)
/*==============================================================*/
/* This function issues either a Search First or Search Next
   BDOS call. */
/* Entry Parameters */
struct _fcb *fcb;	/* pointer to file control block */
short bdos_code;	/* either SEARCHF or SEARCHN */

/* Exit Parameters
   0 = No match found
   NZ = Pointer to entry matched (currently in buffer)
*/

{
unsigned r_code;	/* Return code from Search Function.
			   This is either 255 for no match, or 0, 1, 2 or 3
			   being the ordinal of the 32-byte entry in the
			   buffer that matched the name. */
char *dir_entry;	/* pointer to directory entry */

	/* the BDS C compiler always sets the BDOS DMA
	   to location 0x80 */

r_code = bdos(bdos_code,fcb);	/* issue the bdos call */
if (r_code == 255)		/* no match found */
	return 0;

	/* set a pointer to the matching
	   entry by multiplying return code by 128
	   and adding on to the buffer address (0x80),
	   also add on 1 to point to first character of name */

return (r_code << 5) + 0x81;
}	/* end of srch_file */


/*==============================================================*/
rd_disk(drb)		/* read disk (via BIOS) */
/*==============================================================*/
/* This function uses the parameters previously setup in the
   incoming request block, and using the BIOS directly,
   executes the disk read. */

/* Entry Parameters */
struct _drb *drb;	/* disk request block (disk, track, sector, buffer */

/* Exit Parameters
   0 = No data available
   1 = Data available
*/
{
if (!set_disk(drb))	/* call SELDSK, SETTRK, SETSEC */
	return 0;	/* if SELDSK fails, indicate no data available */
if (bios(DREAD))	/* execute BIOS read */
	return 0;	/* indicate no data available if error returned */

return 1;		/* indicate data available */
} /*  end of rd_disk  */


/*==============================================================*/
wrt_disk(drb)		/* write disk (via BIOS) */
/*==============================================================*/
/* This function uses the parameters previously setup in the
   incoming request block, and using the BIOS directly,
   executes the disk write. */

/* Entry Parameters */
struct _drb *drb;	/* disk request block (disk, track, sector, buffer) */

/* Exit Parameters
   0 = Error during write
   1 = Data written OK
*/
{
if (!set_disk(drb))	/* call SELDSK, SETTRK, SETSEC, SETDMA */
	return 0;	/* if SELDSK fails, indicate no data written */
if (bios(DWRITE))	/* execute BIOS write */
	return 0;	/* indicate error returned */

return 1;		/* indicate data written */
} /*  end of wrt_disk  */



/*==============================================================*/
short set_disk(drb)	/* set disk parameters */
/*==============================================================*/
/* This function sets up the BIOS variables in anticipation of
   a subsequent disk read or write. */
/* Entry Parameters */
struct _drb *drb;	/* disk request block (disk, track, sector, buffer) */

/* Exit Parameters 
   0 = Invalid disk (do NOT perform Read/Write)
   1 = BIOS now setup for Read/Write
*/
{
	/* the sector in the disk request block contains a
	   LOGICAL sector. If necessary (as determined by the
	   value in the disk parameter header), this must be
	   converted into the PHYSICAL sector.
	   NOTE: skewtab is declared as a pointer to a pointer to
	   a short integer (single byte). */
short **skewtab;	/* skewtab -> disk parameter header -> skewtable */
short phy_sec;		/* physical sector */

	/* Call the SELDSK BIOS entry point. If this returns
	   a 0, then the disk is invalid. Otherwise, it returns
	   a pointer to the pointer to the skewtable */
if ( !(skewtab = biosh(SELDSK,drb -> dr_disk)) )
	return 0;		/*  invalid disk  */

bios(SETTRK,drb -> dr_track);	/* set track */

	/* Note that the biosh function puts the sector into
	   registers BC, and a pointer to the skewtable in
	   registers HL. It returns the value in HL on exit
	   from the BIOS */
phy_sec = biosh(SECTRN,drb -> dr_sector,*skewtab); /* get physical sector */
bios(SETSEC,phy_sec);		/* set sector */
bios(SETDMA,drb -> dr_buffer);	/* set buffer address */

return 1;		/* indicate no problems */
} /* end of setp_disk */


/*	Directory Management Functions		*/

	
/*==============================================================*/
get_nde(dir_pb)		/* get next directory entry */
/*==============================================================*/
/* This function returns a pointer to the next directory entry.
   If the directory has not been opened, it opens it.
   When necessary, the next directory sector is read in.
   If the current sector has been modified and needs to be written back
   on to the disk, this will be done before reading in the next sector. */
/* Entry Parameters */
struct _dirpb *dir_pb;		/* pointer to the disk parameter block */

/* Exit Parameters
   Returns a pointer to the next directory entry in the buffer.
   The "directory open" and "write sector" flags in the parameter
   block are reset as necessary. */

{

if(!dir_pb -> dp_open)	/* directory not yet opened */
	{
	if (!open_dir(dir_pb))	/* initialize and open directory */
		{
		err_dir(O_DIR,dir_pb);		/* report error on open */
		exit();
		}
		/* deliberately set the directory entry pointer to the end
		   of the buffer to force a read of a directory sector */
	dir_pb -> dp_entry = dir_pb -> dp_buffer + DIR_BSZ;
	dir_pb -> dp_write = 0;		/* reset write sector flag */
	}

	/* update the directory entry pointer to the next entry in
	   the buffer and check if the pointer is now "off the end"
	   of the buffer and another sector needs to be read. */
if (++dir_pb -> dp_entry < dir_pb -> dp_buffer + DIR_BSZ)
	{
	return dir_pb -> dp_entry;	/* return pointer to next entry */
	}

	/* need to move to next sector and read it in */

	/* do not check if at end of directory or move to 
	   the next sector if the directory has just been
	   opened (but the opened flag has not yet been set */
if (!dir_pb -> dp_open)
	dir_pb -> dp_open = 1;	/* indicate that the directory is now open */
else
	{
	/* check if the sector currently in the buffer needs to be
	   written back out to the disk (it having been changed). */
	if (dir_pb -> dp_write)
		{
		dir_pb -> dp_write = 0;		/* reset the flag */
		if(!rw_dir(W_DIR,dir_pb))	/* write the directory sector */
			{
			err_dir(W_DIR,dir_pb);	/* report error on writing */
			exit();
			}
		}

		/* count down on number of directory entries left to process.
		   always 4 32-byte entries per 128-byte sector */
	dir_pb -> dp_entrem -= 4;

		/* set directory end flag true if number of entries now < 0 */
	if (dir_pb -> dp_entrem == 0)	/* now at end of directory */
		{
		dir_pb -> dp_end = 1;		/* indicate end */
		dir_pb -> dp_open = 0;		/* indicate directory now closed */
		return 0;			/* indicate no more entries */
		}

		/* update sector (and if need be track and sector) */
	if (++dir_pb -> dp_sector == dir_pb -> dp_sptrk)
		{
		++dir_pb -> dp_track;		/* update track */
		dir_pb -> dp_sector = 0;	/* reset sector */
		}
	}

if(!rw_dir(R_DIR,dir_pb))	/* read next directory sector */
	{
	err_dir(R_DIR,dir_pb);	/* report error on reading */
	exit();
	}

	/* reset directory entry pointer to the first entry in the buffer */
return dir_pb -> dp_entry = dir_pb -> dp_buffer;

} /* end of get_nde */



/*==============================================================*/
open_dir(dir_pb)	/* open directory */
/*==============================================================*/
/* This function "opens" up the file directory
   on a specified disk for subsequent processing
   by rw_dir, next_dir functions. */
/* Entry Parameters */
struct _dirpb *dir_pb;	/* pointer to directory parameter block */

/* Exit Parameters 
   0 = Error, directory not opened
   1 = Directory open for processing
*/
{
struct _dpb *dpb;		/* CP/M Disk Parameter Block */

	/* get disk parameter block address for the disk specified in 
	   the directory parameter block */
if ((dpb = get_dpb(dir_pb -> dp_disk)) == 0)
	return 0;	/* return indicating no dpb for this disk */

	/* set the remaining fields in the parameter block */
dir_pb -> dp_sptrk = dpb -> dpb_sptrk;	/* sectors per track  */
dir_pb -> dp_track = dpb -> dpb_trkoff;	/* track offset of the directory  */
dir_pb -> dp_sector = 0;		/* beginning of directory  */
dir_pb -> dp_nument = dpb -> dpb_maxden+1; /*  # of directory entries  */
dir_pb -> dp_entrem = dir_pb -> dp_nument; /*  entries remaining to process */
dir_pb -> dp_end = 0;			/* indicate not at end  */

	/* set number of allocation blocks per directory entry to
	   either 8 or 16 depending on the number of allocation blocks */
dir_pb -> dp_nabpde = (dpb -> dpb_maxabn > 255 ? 8 : 16);
	/* set number of allocation blocks (1 more than number of
	   highest block */
dir_pb -> dp_nab = dpb -> dpb_maxabn;

	/* set the allocation block size based on the block shift.
	   The possible values are: 3 = 1k, 4 = 2K, 5 = 4K, 6 = 8K, 7 = 16K.
	   So a value of 16 is shifted right by (7 - bshift) bits. */
dir_pb -> dp_absize = 16 >> (7 - dpb -> dpb_bshift);

return 1;		/* indicate that directory now opened */
} /*  end of open_dir  */




/*==============================================================*/
rw_dir(read_op,dir_pb)	/* read/write directory */
/*==============================================================*/
/* This function reads/writes the next 128-byte
   sector from/to the currently open directory. */
/* Entry Parameters */
short read_op;			/* true to read, false (0) to write */
struct _dirpb *dir_pb;	/* directory parameter block */

/* Exit Parameters 
   0 = Error - operation not performed
   1 = Operation completed
*/
{
struct _drb drb;		/* disk request (for BIOS read/write) */


drb.dr_disk = dir_pb -> dp_disk;	/* set up disk request */
drb.dr_track = dir_pb -> dp_track;
drb.dr_sector = dir_pb -> dp_sector;
drb.dr_buffer = dir_pb -> dp_buffer;

if (read_op)
	{
	if (!rd_disk(&drb))	/* issue read command */
		return 0;	/* indicate error - no data available */
	}
else
	{
	if (!wrt_disk(&drb))	/* issue write command */
		return 0;	/* indicate error - no data written */
	}
return 1;			/* indicate operation complete */
} /* end of rd_dir */


/*==============================================================*/
err_dir(opcode,dir_pb)
/*==============================================================*/
/* This function displays an error message to report an error
   detected in the directory management functions open_dir and rw_dir. */
short opcode;			/* operation being attempted */
struct _dirpb *dir_pb;	/* pointer to directory parameter block */
{

printf("\n\007Error during ");

switch(opcode)
	{
	case R_DIR:
		printf("Reading");
		break;
	case W_DIR:
		printf("Writing");
		break;
	case O_DIR:
		printf("Opening");
		break;
	default:
		printf("Unknown Operation (%d) on",opcode);
	}

printf(" Directory on disk %c:. ",dir_pb -> dp_disk + 'A');
} /* end of err_dir */

/*==============================================================*/
setscb(scb,fname,user,extent,length)	/* set search control block */
/*==============================================================*/
/* This function sets up a search control block according
   to the filename specified. The file name can take the 
   following forms :

	filename
	filename.typ
	d:filename.typ
	*:filename.typ (meaning "all disks")
	ABCD...NOP:filename.typ (meaning "just the specified disks")

   It sets the bit map according to which disks should be searched.
   For each selected disk, it checks to see if an error is generated
   when selecting the disk (i.e. if there are disk tables in the BIOS
   for the disk). */
/* Entry Parameters */
struct _scb *scb;	/* pointer to search control block */
char *fname;		/* pointer to the file name */
short user;		/* user number to search for */
short extent;		/* extent number to search for */
int length;		/* number of bytes to compare */

/* Exit Parameters
   None.
*/
{
int disk;		/* disk number currently being checked */
unsigned adisks;	/* bit map for active disks */

adisks = 0;		/* assume no disks to search */

if (strscn(fname,":"))		/* check if ":" in file name */
	{
	if (*fname == '*')	/* check if "all disks" */
		{
		adisks = 0xFFFF;	/* set all bits */
		}
	else			/* set specific disks */
		{
		while(*fname != ':')	/* until ":" reached */
			{
			/* build the bit map by getting the next disk
			   id. (A - P), converting it to a number
			   in the range 0 - 15, and shifting a 1-bit
			   left that many places and OR-ing it into
			   the current active disks. */
			adisks |= 1 << (toupper(*fname) - 'A');
			++fname;	/* move to next character */
			}
		++fname;		/* bypass colon */
		}
	}
else	/* use only current default disk */
	{
		/* set just the bit corresponding to the current disk */
	adisks = 1 << bdos(GETDISK);
	}

setfcb(scb,fname);	/* set search control block as though it
			   is a file control block. */

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
scb -> scb_userno = user;	/* set user number */
scb -> scb_extent = extent;	/* set extent number */
scb -> scb_length = length;	/* set number of bytes to compare */
} /* end setscb */


/*==============================================================*/
dm_clr(disk_map)		/* disk map clear (to zeroes) */
/*==============================================================*/
/* This function clears all elements of the disk map to 0. */
/* Entry Parameters */
unsigned disk_map[16][18];	/* pointer to array of unsigned integers */

/* Exit Parameters
   None.
*/
{
	/* WARNING - The 576 in the setmem call below is based on the
	   fact that the disk map array is [16][18] - i.e. 288 unsigned
	   integers, hence 576 bytes. */
setmem(disk_map,576,'\0');	/* fill array with 0's */

} /* end of dm_clr */



/*==============================================================*/
dm_disp(disk_map,adisks)		/* disk map display */
/*==============================================================*/
/* This function displays the elements of the disk map, showing
   the count in each element. A zero value element is shown as
   blanks. For example:

     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  Used Free
A: 123      20  98         202     199 101 211                       954   70

   Lines will only be printed for active disks (as indicated by
   the bit map). */
/* Entry Parameters */
unsigned disk_map[16][18];	/* pointer to disk map array */
unsigned adisks;		/* bit map of active disks */
{
#define USED_COUNT 16		/* "user" number for used entities */
#define FREE_COUNT 17		/* "user" number for free entities */

int disk;		/* current disk number */
int userno;		/* current user number */
unsigned dsum;		/* sum of entries for given disk */

printf("\n      0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  Used Free");

for (disk = 0;		/* start with disk A: */
     disk < 16;		/* until disk P: */
     disk++)		/* next disk */
	{
	if (!(adisks & (1 << disk)))	/* check if disk is active */
		continue;	/* no - so bypass this one */
		
	printf("\n%c: ",disk + 'A');	/* display disk number */

	dsum = 0;		/* reset sum for this disk */
	for (userno = 0;	/* start with user 0 */
	     userno < 16;	/* until user 15 */
	     userno++)		/* next user number */
		{
		dsum += disk_map[disk][userno]; /* build sum */
		}

	if (dsum)	/* check if any output for this disk,
			   and if not, display d: None */
		{
		/* print either number or blanks */
		for (userno = 0;	/* start with user 0 */
		     userno < 16;	/* until user 15 */
		     userno++)		/* next user number */
			{
			if (disk_map[disk][userno])
				printf("%4d",disk_map[disk][userno]);
			else
				printf("    ");
			}
		}
	else		/* no output for this disk */
		{
		printf(  " -- None --                                                     ");
		}
	printf("  %4d %4d",disk_map[disk][USED_COUNT],disk_map[disk][FREE_COUNT]);
	}
} /* end dm_disp */


/*==============================================================*/
get_dpb(disk)		/* get disk parameter block address */
/*==============================================================*/
/* This function returns the address of the disk parameter
   block (located in the BIOS). */

/* Entry Parameters */
char disk;		/* logical disk for which DPB address is needed */

/* Exit Parameters
	0 = Invalid logical disk
	NZ = Pointer to disk parameter block
*/
{

if (biosh(SELDSK,disk) == 0)		/* make BIOS SELDSK call */
	return 0;			/* invalid disk */

bdos(SETDISK,disk);			/* use BDOS SETDISK function */
return bdos(GETDPARM);		/*  get the disk parameter block  */
} /*  end of get_dpb  */


/*	code table functions	*/

/* Most programs that interact with a user need to be able
   to accept parameters from the user by name and translate
   the name into some internal code value.
   They also need to be able to work in reverse, examining
   the setting of a variable, and determing what (in terms
   of its ASCII name) it has been set to.

   An example, is setting Baud Rates. The user may want to
   enter "19200", and have this translated into some magic
   number to be output to a chip. Alternatively, a previously
   set baud rate variable may need to be examined, and the
   string "19200" generated in order display its current
   setting to the user.

   A code table is used to make this task easier.
   Each element in the table logically consists of:

   A code value (unsigned integer)
   An ASCII character string (actually a pointer to it) */

/*==============================================================*/
ct_init(entry,code,string)	/* initialize code table */
/*==============================================================*/
/* This function initializes a specific entry in a code table
   with a code value and string pointer.

   IMPORTANT NOTE: By convention, the LAST entry in a given
   code table will have a code value of CT_SNF (String Not Found). */
/* Entry Parameters */
struct _ct *entry;		/* pointer to code table entry */
int code;			/* code value to store in entry */
char *string;			/* pointer to string for entry */

/* Exit Parameters 
   None.
*/
{
entry -> _ct_code = code;		/* set _ct_code */
entry -> _ct_sp = string;		/* set string pointer */
}

/*==============================================================*/
unsigned
ct_parc(table,string)		/* parameter - return code */
/*==============================================================*/
/* This function searches the specified table for a 
   matching string, and returns the code value that corresponds to it.
   If only one match is found in the table, then this function returns
   that code value. If no match, or more than one match is found,
   then it returns the error value, CT_SNF (string not found).
   This function is specifically designed for processing
   parameters on a command tail.
   Note that the comparison is done after folding to UPPER CASE
   (i.e. "STRING" matches "string"), and a SUBstring compare is used
   so that only the minimum number of characters for an unambiguous
   response need be entered. For example, if the table contained:

		Code	Value
		1	"APPLES"
		2	"ORANGES"
		3	"APRICOTS"

   A response of "O" would return code = 2, but "A" or "AP" would
   be ambiguous. "APR" or "APP" would be required. */
struct _ct *table;		/* pointer to table */
char *string;			/* pointer to key string */
{
int mcode;			/* matched code to return */
int mcount;			/* count of number of matches found */


mcode = CT_SNF;			/* assume error */
mcount = 0;		/* reset match count */

while(table -> _ct_code != CT_SNF) /* not at end of table */
	{
	/* Compare keyboard response to table entry using
	   UPPER CASE substring compare. */
	if (usstrcmp(table -> _ct_sp,string) == 0)
		{
		mcount++;	/* update match count */
		mcode = table -> _ct_code;	/* save code */
		}
	table++;		/* move to next entry */
	}

if (mcount == 1)		/* only one match found */
	return mcode;		/* return matched code */
else				/* illegal or ambiguous */
	return CT_SNF;

} /* end ct_parc */

/*==============================================================*/
unsigned
ct_code(table,string)	/* return code for string */
/*==============================================================*/
/* This function searches the specified table, searching for
   the specified string. If a match occurs, it returns the
   corresponding code value, otherwise it returns CT_SNF
   (String Not Found).
   Unlike ct_parc, this function compares every character in the
   key string, and will return the code on the first match found. */
/* Entry Parameters */
struct _ct *table;	/* pointer to table */
char *string;		/* pointer to string */

/* Exit Parameters
   Code value - if string found
   CT_SNF - if string not found
*/

{
while(table -> _ct_code != CT_SNF)	/* for all entries in table */
	{
	if (ustrcmp(table -> _ct_sp,string) == 0) /* compare strings */
		return table -> _ct_code;	/* return code */
	table++;			/* move to next entry */
	}
return CT_SNF;				/* String Not Found */
} /* end ct_code */


/*==============================================================*/
ct_disps(table)	/* displays all strings in specified table */
/*==============================================================*/
/* This function displays all of the strings in a given table.
   It is used to indicate valid responses for operator input. */

/* Entry Parameters */
struct _ct *table;		/* pointer to table */

/* Exit Parameters
	None.
*/

{

while(table -> _ct_code != CT_SNF)	/* not end of table */
	{
	printf("\n\t\t%s",table -> _ct_sp);	/* print string */
	table++;			/* move to next entry */
	}
putchar('\n');				/* add final return */
}


/*==============================================================*/
ct_index(table,string) /* returns index for a given string */
/*==============================================================*/
/* This function searches the specified table, and returns
   the INDEX of the entry containing a matching string.
   All characters of the string are used for the comparison
   after they have been folded to UPPER CASE. */

/* Entry Parameters */
struct _ct *table;		/* pointer to table */
char *string;			/* pointer to string */

/* Exit Parameters
   Index of entry matching string, or
   CT_SNF if String Not Found.
*/
	
{
int index;			/* current value of index */

index = 0;			/* initialise index */

while(table -> _ct_code != CT_SNF)	/* not at end of table */
	{

	if (ustrcmp(table -> _ct_sp,string) == 0)
		return index;	/* return index */
	table++;		/* move to next table entry */
	index++;		/* update index */
	}
return CT_SNF;		/* String Not Found */
}


/*==============================================================*/
char *ct_stri(table,index)	/* Get string according to index */
/*==============================================================*/
/* This function returns a pointer to the string in the
   table entry specified by the index. */
/* Entry Parameters */
struct _ct *table;		/* pointer to table */
int index;			/* index into table */
{
struct _ct *entry;			/* entry pointer */
	entry = table[index];		/* point to entry */
	return entry -> _ct_sp;		/* return pointer to string */
} /* end of ct_stri */


/*==============================================================*/
char *ct_strc(table,code)	/* Get string according to code value */
/*==============================================================*/
/* This function searches the specified table and returns a
   pointer to the character string in the entry with the
   matching code value or a pointer to a string of "Unknown"
   if the code value is not found. */

/* Entry Parameters */
struct _ct *table;		/* pointer to table */
unsigned  code;			/* code value */
{
while(table -> _ct_code != CT_SNF)	/* until end of table */
	{
	if (table -> _ct_code == code)	/* check code matches */
		return table -> _ct_sp;	/* yes, return ptr to str */
	table++;			/* no, move to next entry */
	}
return "Unknown";
}



/*	Bit Vector Functions	*/

/* These functions manipulate bit vectors. A bit vector is a group
   of adjacent bits, packed 8 per byte. Each bit vector has the
   structure defined in the LIBRARY.H file.
	
   Bit vectors are used primarily to manipulate such things as
   the Operating System's allocation vectors, and any other
   values that can best be represented as a series of bits. */

/*==============================================================*/
bv_make(bv,bytes)	/* make a bit vector and clear to zeros */
/*==============================================================*/
/* This function uses C's built-in memory allocation, alloc,
   to allocate the necessary amount of memory, and then
   sets the vector to 0-bits. */

/* Entry Parameters */
struct _bv *bv;		/* pointer to a bit vector */
unsigned bytes;		/* number of BYTES in bit vector */

/* Exit Parameter
   NZ = Vector created
   0 = Insufficient memory to create vector 
*/
{
if(!(bv -> bv_bits = alloc(bytes)))	/* request memory */
	return 0;			/* request failed */

bv -> bv_bytes = bytes;			/* set length */
bv -> bv_end = bv -> bv_bits + bytes;	/* set pointer to end */

bv_fill(bv,0);				/* fill with 0's */
return 1;
}

/*==============================================================*/
bv_fill(bv,value)	/* fill bit vector with value */
/*==============================================================*/
/* This function fills the specified bit vector with the
   specified value.
   This function exist only for consistency's sake and
   to isolate the main body of code from standard
   functions like setmem. */

/* Entry Parameters */
struct _bv *bv;		/* pointer to bit vector */
char value;		/* value to fill vector with */

/* Exit Parameters
   None.
*/
{
/*     address      length         value */
setmem(bv -> bv_bits,bv -> bv_bytes,value);
}


/*==============================================================*/
bv_set(bv,bitnum)		/* set the specified bit number */
/*==============================================================*/
/* This function sets the specified bit number in the bit vector
   to a 1-bit. */

/* Entry Parameters */
struct _bv *bv;			/* pointer to bit vector */
unsigned bitnum;		/* bit number to be set */

/* Exit Parameters
   None.
*/
{
unsigned byte_offset;		/* byte offset into the bit vector */

if ((byte_offset = bitnum >> 3) > bv -> bv_bytes)
	return 0;	/* bitnum is 'off the end' of the vector */

/* set the appropriate bit in the vector. The byte offset
   has already been calculated. The bit number in the byte
   is calculated by AND-ing the bit number with 0x07.
   The specified bit is then OR-red into the vector */

bv -> bv_bits[byte_offset] |= (1 << (bitnum & 0x7));

return 1;		/* indicate completion */
}



/*==============================================================*/
bv_test(bv,bitnum)		/* test the specified bit number */
/*==============================================================*/
/* This function returns a value that reflects the current
   setting of the specified bit. */

/* Entry Parameters */
struct _bv *bv;			/* pointer to bit vector */
unsigned bitnum;		/* bit number to be set */

/* Exit Parameters
   None.
*/
{
unsigned byte_offset;		/* byte offset into the bit vector */

if ((byte_offset = bitnum >> 3) > bv -> bv_bytes)
	return 0;	/* bitnum is 'off the end' of the vector */

/* set the appropriate bit in the vector. The byte offset
   has already been calculated. The bit number in the byte
   is calculated by AND-ing the bit number with 0x07.
   The specified bit is then OR-red into the vector */

return bv -> bv_bits[byte_offset] & (1 << (bitnum & 0x7));
}


/*==============================================================*/
bv_nz(bv)		/* test bit vector non-zero */
/*==============================================================*/
/* This function tests each byte in the specified vector,
   and returns indicating whether there are any bits set
   in the vector. */

/* Entry Parameters */
struct _bv *bv;		/* pointer to bit vector */

/* Exit Parameters
   NZ = One or more bits are set in the vector
   0 = all bits are off.
*/

{
char *bits;		/* pointer to bits in bit vector */

bits = bv -> bv_bits;		/* set working pointer */

while (bits != bv -> bv_end)	/* for entire bit vector */
	{
	if (*bits++)		/* if non-zero */
		return bits--;	/* return pointer to NZ byte */
	}
return 0;			/* indicate vector is 0 */
}


/*==============================================================*/
bv_and(bv3,bv1,bv2)		/* bv3 = bv1 & bv2 */
/*==============================================================*/
/* This function performs a boolean AND between the bytes
   of bit vector 1 and 2, storing the result in bit vector 3. */

/* Entry Parameters */
struct _bv *bv1;		/* pointer to input bit vector */
struct _bv *bv2;		/* pointer to input bit vector */

/* Exit Parameters */
struct _bv *bv3;		/* pointer to output bit vector */

{
char *bits1, *bits2, *bits3;	/* working pointers to bit vectors */

bits1 = bv1 -> bv_bits;	/* initialize working pointers */
bits2 = bv2 -> bv_bits;
bits3 = bv3 -> bv_bits;

	/* The AND-ing will proceed until the end of any one of the bit
	   vectors is reached. */
while (bits1 != bv1 -> bv_end && 
       bits2 != bv2 -> bv_end &&
       bits3 != bv3 -> bv_end)
	{
		*bits3++ = *bits1++ & *bits2++;	/* bv3 = bv1 & bv2 */
	}
}


/*==============================================================*/
bv_or(bv3,bv1,bv2)		/* bv3 = bv1 or bv2 */
/*==============================================================*/
/* This function performs a boolean inclusive-OR between the bytes
   of bit vector 1 and 2, storing the result in bit vector 3. */

/* Entry Parameters */
struct _bv *bv1;		/* pointer to input bit vector */
struct _bv *bv2;		/* pointer to input bit vector */

/* Exit Parameters */
struct _bv *bv3;		/* pointer to output bit vector */

{
char *bits1, *bits2, *bits3;	/* working pointers to bit vectors */

bits1 = bv1 -> bv_bits;	/* initialize working pointers */
bits2 = bv2 -> bv_bits;
bits3 = bv3 -> bv_bits;

	/* The OR-ing will proceed until the end of any one of the bit
	   vectors is reached. */
while (bits1 != bv1 -> bv_end && 
       bits2 != bv2 -> bv_end &&
       bits3 != bv3 -> bv_end)
	{
		*bits3++ = *bits1++ | *bits2++;	/* bv3 = bv1 or bv2 */
	}
}


/*==============================================================*/
bv_disp(title,bv)		/* bit vector display */
/*==============================================================*/
/* This function displays the contents of the specified bit vector
   in hexadecimal. It is normally only used for debugging purposes. */

/* Entry Parameters */
char *title;			/* Title for the display */
struct _bv *bv;			/* pointer to the bit vector */

/* Exit Parameters
   None.
*/

{
char *bits;			/* working pointer */
unsigned byte_count;		/* count used for formatting display */
unsigned bit_count;		/* count for processing bits in a byte */
char byte_value;		/* value to be displayed */

printf("\nBit Vector : %s",title);	/* display title */

bits = bv -> bv_bits;			/* set working pointer */
byte_count = 0;				/* initialize count */

while (bits != bv -> bv_end)	/* for the entire vector */
	{
	if (byte_count % 5 == 0)	/* chk if new line */
		printf("\n%4d : ",byte_count << 3);	/* display bit number */

	byte_value = *bits++;	/* get the next byte from the vector */

	for (bit_count = 0; bit_count < 8; bit_count++)
		{
		/* display the left-most bit, then shift the value
		   left one bit */
		if (bit_count == 4) putchar(' ');	/* separator */
		putchar((byte_value & 0x80) ? '1' : '0');
		byte_value <<= 1;	/* shift value left */
		}
	printf("  ");					/* separator */

	byte_count++;	/* update byte count */
	}
}

/* End of LIBRARY.C */
