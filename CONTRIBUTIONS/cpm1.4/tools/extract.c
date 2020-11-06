/* #include <conio.h> */
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fcb              // also directory entry
{
    unsigned char user;                     // not used in CP/M 1.4
    unsigned char name[8];                  // file name
    unsigned char type[3];                  // file extension
    unsigned char ext;                      // extent number
    unsigned char notused[2];
    unsigned char rc;                       // record count in this extent
    unsigned char dm[16];                   // single byte extent numbers
};

#define DIR_ENT_SZ          32              // size of one directory entry
#define SECT_SZ             128             // disk sector size
#define SPB                 8               // sectors per block
#define DIR_BLOCKS          2               // 2 blocks reserved for directory
#define SPT                 26              // sectors per track
#define RESERVED_TRACKS     2               // for cold boot, ccp and cpm

#define DIR_SECTORS         (SPB * DIR_BLOCKS)      // sectors for directory
#define DIR_LOCN            (SPT * RESERVED_TRACKS) // offset of directory 
#define DIR_ENT_PER_SEC     (SECT_SZ / DIR_ENT_SZ)  // directory entries per sector

unsigned char buff1[SPT * SECT_SZ];         // for de-interleaving
unsigned char buff2[SPT * SECT_SZ];         //  "

unsigned char fsec  [SECT_SZ];
         char fname [20];                   // file name for current extent
         char fname2[20];                   // currently open output file name, if any

FILE    *in,                                // raw image file
        *out;                               // extracted CP/M file

unsigned char exm[1024];                    // map of processed blocks

// sector interleave table
unsigned char sectran[] = {
    "\x01\x07\x0d\x13\x19\x05\x0b\x11"
    "\x17\x03\x09\x0f\x15\x02\x08\x0e"
    "\x14\x1a\x06\x0c\x12\x18\x04\x0a"
    "\x10\x16\x00\x00\x00\x00\x00\x00"};

char temp[] = "junkimg.jnk";                // name of de-interleaved raw image file

/**************************************************

    Read the input image file in gulps of 26 sectors, reorder using the 
    interleace table, and save the to the de-interleaved file

*/
void RemoveInterleave
(
    char *fn1,                  // ram image file name
    char * fn2,                 // reordered image file name
    int flag                    // not used
)
{
    int x;

    in  = fopen(fn1, "rb");
    out = fopen(fn2, "wb");

    while (fread(buff1, 1, sizeof(buff1), in))  // read a 'track'
    {
        for (x = 0; x < SPT; x++)               // shuffle
        {
            memcpy(&buff2[x * SECT_SZ], &buff1[(sectran[x] - 1) * SECT_SZ], SECT_SZ);
        }
        fwrite(buff2, 1, sizeof(buff2), out);
    }
    fcloseall();
}

/**************************************************

    Copy a file name or extension until a space character is found 
    or limit is reached.

*/
void copyuntil
(
    char *dst,                  // destination for copy
    char *src,                  // source for copy
    int count                   // max copy length
)
{
    while (*src != ' ' && count--)
    {
        *dst++ = *src++;
    }
    *dst = 0;
}

/**************************************************

    Convert an 8,3 filename into name.ext format

*/
void MakeFilename
(
    char *fname,                // destination for file name 
    struct fcb *tfcb            // pointer to fcb (directory entry)
)   
{
    copyuntil(fname, tfcb->name, 8);    // copy name part

    if (tfcb->type[0] != ' ')           // if extension is non-blank
    {
        strcat(fname, ".");             // add a dot
        copyuntil(fname + strlen(fname), tfcb->type, 3);
    }
}
/**************************************************

    Read a logical CP/M sector from the image file.  Sector is a logical 
    number based at the directory root.  The reserved sectors(tracks)
    are added.

*/
void ReadSector
(
    unsigned char *where,           // destination for read
    int sectorno                    // logical sector number
)
{
    fseek(in, (long)(DIR_LOCN + sectorno) * (long)SECT_SZ, SEEK_SET);
    fread(where, 1, SECT_SZ, in);
}

/**************************************************

    Construct the file name and copy all records in the specified extent into the
    output file.  If the file name differs from the previously processed file name 
    close the previous file and open a new one with the current name.
    Since extents are psorecces in sorted order, including the extent number, this 
    results in recovered files. 


*/
void CopyExtentToFile
(
    struct fcb  *tfcb,                  // pointer to fcb (directory entry)
    int         deleted                 // flag indicating a deleted extent
)
{
    int         x,                      // general counter 
                thisSect,               // computed logical sector number
                said;                   // flag for printing
    static int  extent;                 // logical extent number

    MakeFilename(fname, tfcb);          // extract filename
    if (strcmp(fname2, fname))          // is it different from before?
    {
        if (out != NULL)
        {
            fclose(out);
        }
        out = fopen(fname, "wb");       // create new output file
        strcpy(fname2, fname);
        printf("Extracting %sfile %s\n", deleted ? "deleted " : "", fname);
        extent = -1;
    }
    extent++;

    //printf("%15s", fname);
    for (x = 0; x < tfcb->rc; x++)      // fetch record count sectors and save
    {
        if (!(x % SPB))
        {
            //printf(" %02x", tfcb->dm[x / SPB]);
        }
        if ((exm[tfcb->dm[x / SPB]] != 0) && !said)
        {
            said = 1;
            printf("  extent %d has blocks used elsewhere\n");
        }
        exm[tfcb->dm[x / SPB]] = 1;     // mark in block map

        // convert logical sector to physical 
        thisSect = (tfcb->dm[x / SPB] * SPB) + (x % SPB);
        
        ReadSector(fsec, thisSect);             // load the file sector
        fwrite(fsec, 1, SECT_SZ, out);          // store in new file
    }
    //printf("\n");
}

/**************************************************

    Compare routine for qsort of directory

*/
int compare
(
    void *a,                    // pointer to key field 'a'
    void *b                     // pointer to key field 'b'
)
{
    return memcmp(a, b, 16);    // compare keys
}

/**************************************************
*/
main
(
    int argc,                   // count of command line arguments
    char **argv                 // pointers to arguments
)
{  
    int         x;
    struct fcb  *tfcb = (struct fcb *)buff1;

    //printf("Dir locn %x\n", DIR_LOCN * SECT_SZ);

	if (argc < 2)
	{  
	    printf("File name required for compare\n\n");
		printf("Correct usage is:\n");
		printf("   extract <filename>\n\n");
   		printf("   where <filename> is a diskette image file\n");
		exit(1);
	}

    RemoveInterleave(argv[1], temp, 0);

    in = fopen(temp, "rb");

    // load the directory into memory
    for (x = 0; x < DIR_SECTORS; x++)
    {
        ReadSector(buff1 + (x * SECT_SZ), x);
    }

    // sort the directory in [user], name and extent order
    qsort(buff1, DIR_SECTORS, DIR_ENT_SZ, compare);

    // process the sorted list  
    for (x = 0; x < (DIR_SECTORS * DIR_ENT_PER_SEC); x++)
    {
        if ((tfcb->name[0] != 0xe5) && (tfcb->name != 0))
        {
            CopyExtentToFile(tfcb, tfcb->user == 0xe5);
        }
        tfcb++;
    }
    fcloseall();
    remove(temp);
}
