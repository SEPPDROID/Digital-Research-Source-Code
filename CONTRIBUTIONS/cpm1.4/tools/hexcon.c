/*

    Intel hex to binary converter  Converts the specified file to binary

*/

#include <stdio.h>
#include <conio.h>
#include <assert.h>

typedef struct image_t
{
	unsigned lowaddress, highaddress;
} image_t;

//void LoadHexFile(char *fn, unsigned char *where, image_t *im);


#define LINE_SIZE       500             // max size of input hex line
#define MAX_IMG_SZ      0x4000

// Intel hex-file record types
#define RT_DATA         0
#define RT_EOF          1

static int hexsum;
static unsigned char *lyne;

/******************************************************************************

    Get a hexadecimal nybble from the input stream.  Increment the pointer.

*/
static int getnib(char **bt)
{
	int x = **bt - '0';

	if (x > 9)
    {
		x -= 7;
    }
	(*bt)++;
	return x;
}

/******************************************************************************

    Get a single unsigned byte (2 nibbles) from the input stream.  Increment the
    pointer and add the fetched byte to the line checksum

*/
static unsigned long gethex(char **bt)
{
	int x;
	
	x = getnib(bt) << 4;
	x |= getnib(bt);
	hexsum += x;
	return x;
}
/******************************************************************************

    Get an unsigned short from the input stream,  The pointer and the line checksum
    are maintained.

*/
static unsigned long getshort(char **bt)
{
    unsigned short ret;

	ret  = gethex(bt) << 8;
	ret |= gethex(bt);
	return ret;
}

/******************************************************************************

    Load the specified Intel format hex file into memory as a binary
    image.  Returns the file loaded in 'prog' with the base and limit filled in
    as im->lowaddress and im->highaddress respectively.

*/
void LoadHexFile(char *fn, unsigned char *prog, image_t *im, unsigned limit)
{
	char            *p1;                        // pointer to hex input text
	FILE            *ff = fopen(fn, "rb");
	int	            lth;                        // line length byte
	unsigned        address,                    // current line address
	                offset;                     // 0 offset of first byte 
	int				rt;                         // record type
	unsigned char	kh;                         // fetched byte from hex file

	printf("Converting %s\n", fn);
	
	lyne			= (unsigned char*)malloc(LINE_SIZE);
	
	im->lowaddress  = 0xffff;	
	im->highaddress = 0;	
	
	while (fgets(p1 = lyne, LINE_SIZE, ff))
	{
		if (*p1++ != ':')                           // all good Intel hex file have this
        {
			break;
        }
		hexsum  = 0;
		lth     = gethex(&p1);                      // fetch length of line
		address = getshort(&p1);                    // fetch target address 

		if ((rt = gethex(&p1)) == RT_EOF)           // fetch record type 
        {
			break;                                  // end of file 
        }
        switch(rt)
        {
            default:
			    printf("Invalid record type %02x\n", rt);
    			getch();
                break;

		    case RT_DATA:                               // data bytes
				if (address < im->lowaddress)
				{
					assert(im->highaddress == 0);
					im->lowaddress = address;
					offset         = address;
				}

        		while (lth--)
		        {
					kh = (unsigned char)gethex(&p1);
                    if ((address - offset) > limit)
                    {
		                printf("Hex file too large\n");
                        getch();
                        exit(0);
                    }
        			*(prog + address++ - offset) = kh;
		        }
				if (address > im->highaddress)
                {
					im->highaddress = address;
                }
                break;
        }
		kh = (unsigned char)gethex(&p1);
		if (hexsum & 0xff)
        {
		    printf("Hex conversion error\n");
            getch();
            exit(0);
        }
	}
	free(lyne);
    fclose(ff);
}

/******************************************************************************

    Convert the specified Intel format hex file <argv[1]> into memory as a binary
    image.  Saves the binary image as <argv[2]>.

*/
void main(int argc, char **argv)
{
    image_t img;
    FILE    *out;

    char *where = (char*)malloc(MAX_IMG_SZ);    

    memset(where, 0, MAX_IMG_SZ);
    LoadHexFile(argv[1], where, &img, MAX_IMG_SZ);
    out = fopen(argv[2], "wb");
    fwrite(where, 1, img.highaddress - img.lowaddress, out);
    fclose(out);
}