/*
 *      Kludge hex dump routine for 68K
 */
#include <stdio.h>
#include <osif.h>
#include "machine.h"
#include <ctype.h>
main(argc,argv)

        int     argc;
        char    **argv;

{
        char    buffer[128];                    /* Sector buffer        */
        int     sector;                         /* Sector number        */
        long    filaddr;                        /* File address         */
        int     offset;                         /* Sector Offset        */
        register i,j,k,l,m;                     /* Temps.               */
        int     fd;                             /* File descriptor      */

        if(argc < 2 || argc > 3)                /* Only 1 file, please! */
        {
                printf("Usage: dump [-shhhhhh] file\n");
                exit();
        }

        i = 1;
        filaddr= 0;                             /* Zero file address    */
        if(*argv[1] == '-')
        {
           decode(argv[1],&filaddr);            /* Get hex file addr    */
           i=2;
        }

        if((fd=openb(argv[i],READ)) < 0)        /* Try to open file     */
        {
                printf("Unable to open \"%s\"\n",argv[i]);
                exit();
        }

        sector = filaddr >> 7;                  /* Zero sector count    */
        filaddr &= ~15;                         /* Round request down   */
        lseek(fd,filaddr,0);                    /* Do the seek          */
        m = (128 - (filaddr & 127));            /* # bytes in sector    */
        while((l=read(fd,buffer,m)) > 0)        /* Until EOF            */
        {
                l = (l + 15) / 16;              /* Number of lines      */
                offset = filaddr & 127;         /* Starting Offset      */
                for(i=0; i<l; i++)              /* 8 lines / sector     */
                {
                   printf("%04x %02x (%06lx): ",sector,offset,filaddr);
                   for(j=0; j<16; j++)          /* 16 bytes / line      */
                   {
#if VAX
                       printf(" %02x%02x",(buffer[i*16+j+1])&0xff,
                                (buffer[i*16+j]&0xff));
#endif
#if PDP11
                       printf(" %02x%02x",(buffer[i*16+j+1])&0xff,
                                (buffer[i*16+j])&0xff);
#endif
#if MC68000
                       printf(" %02x%02x",(buffer[i*16+j])&0xff,
                                (buffer[i*16+j+1]&0xff));
#endif
                       j++;
                   }
                   printf(" *");
                   for(j=0; j<16; j++)          /* Now ASCII            */
                   {
                        k = buffer[i*16+j] & 0x7f;
                        if ((k < ' ') || (k == 0x7f)) k= '.';
                        printf("%c",k);
                   }
                   printf("*\n");
                   filaddr += 16;               /* Bump up file address*/
                   offset  += 16;               /* and offset          */
                }
                sector++;                       /* Increment sector count*/
                m = 128;                        /* # bytes to read next */
                for(j=0; j<128; j++)
                        buffer[j] = 0;
        }
        printf("\n*** E O F ***\n");
}


/****************************************************************************/
/*                                                                          */
/*                      D e c o d e    F u n c t i o n                      */
/*                      ------------------------------                      */
/*                                                                          */
/*      Routine "decode" is called to process the relocation base argument  */
/*      from the command line.                                              */
/*                                                                          */
/*      Calling sequence:                                                   */
/*                                                                          */
/*              ret = decode(string,&address);                              */
/*                                                                          */
/*      Where:                                                              */
/*              string   -> argument string (usually argv[1])               */
/*              &address -> long word to receive converted value            */
/*                                                                          */
/*              ret      =  0 if successful conversion                      */
/*                       = -1 if anything amiss                             */
/*                                                                          */
/****************************************************************************/
WORD decode(string,addr)                        /*                          */
                                                /*                          */
REG     BYTE    *string;                        /* -> Command argument      */
        LONG    *addr;                          /* =  return value          */
{                                               /****************************/
REG     LONG    a;                              /* Temp return value        */
REG     BYTE    c;                              /* Temp character           */
                                                /*                          */
        if(*string++ != '-' || *string++ != 's')/* Check for valid switch   */
                return(FAILURE);                /* quit if NFG              */
                                                /*                          */
        a = 0;                                  /* Zero out accumulator     */
                                                /*                          */
        while(*string)                          /* Until no more chars      */
        {                                       /*                          */
          c = toupper(*string) & 0177;          /* Pick up next char        */
          if (c >= '0' && c <= '9')             /* Digit                    */
                a = (a << 4) + c - '0';         /* Un-ASCIIfy               */
          else if (c >= 'A' && c <= 'F')        /* Hex                      */
                a = (a << 4) + c - 'A'+ 10;     /* Un-ASCIIfy               */
          else                                  /* Bad character            */
                return(FAILURE);                /* So long, sucker!!        */
          string++;                             /* Increment pointer        */
        }                                       /*                          */
        *addr = a;                              /* Store result             */
        return(SUCCESS);                        /* Return all ok            */
}                                               /****************************/


