/*
 * load - convert a hex file to a com file
 */

#include <stdio.h>
#include <stdlib.h>
 
unsigned char   checksum;

FILE *fpout;

unsigned char   getbyte () {
    register int    c;
    unsigned char   x;

    c = getchar ();
    if ('0' <= c && c <= '9')
	x = c - '0';
    else
	if ('A' <= c && c <= 'F')
	    x = c - 'A' + 10;
	else
	    goto funny;

    x <<= 4;
    c = getchar ();
    if ('0' <= c && c <= '9')
	x |= c - '0';
    else
	if ('A' <= c && c <= 'F')
	    x |= c - 'A' + 10;
	else {
    funny:
	    fprintf (stderr, "Funny hex letter %c\n", c);
	    exit (2);
	}
    checksum += x;
    return x;
}

main (int argc, char **argv) {
    register unsigned   i, n;
    char    c, buf[64];
    unsigned    type;
    unsigned int al, ah, addr = 0, naddr;
	
	if (argc < 2) fpout = stdout;
	else fpout = fopen(argv[1],"wb");
    
    do {
	do {
	    c = getchar ();
	    if (c == EOF) {
		fprintf (stderr, "Premature EOF colon missing\n");
		exit (1);
	    }
	} while (c != ':');

	checksum = 0;
	n = getbyte ();		/* bytes / line */
	ah = getbyte ();
	al = getbyte ();

	
	switch (type = getbyte ()) 
	{
	    case 1:
		break;
		
	    case 0:
		naddr = (ah << 8) | al;
		if (!addr) addr = naddr;
		else while (addr < naddr)  
		{
			fwrite("", 1, 1, fpout);
			++addr;
		}
		if (addr > naddr) 
		{
			fprintf(stderr,"Records out of sequence at %x\n", naddr);
			exit(1);
		}

		for (i = 0; i < n; i++)
		    buf[i] = getbyte ();
		fwrite (buf, 1, n, fpout);
		break;
	    default:
		fprintf (stderr, "Funny record type %d\n", type);
		exit (1);
	}

	(void) getbyte ();
	if (checksum != 0) 
	{
	    fprintf (stderr, "Checksum error");
	    exit (2);
	}

	addr += n;
	
    } while (type != 1);
}
