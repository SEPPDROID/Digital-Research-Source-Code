

/****************************************************************************/
/*									    */
/*									    */
/*                Test Program #6 for C run-time library.		    */
/*		  Tests : putl,getl,setbuf,read,write,feof,		    */
/*		          calloc,fileno and rewind.			    */
/*		  Uses one ascii and one binary file.			    */
/*		  May be run independently of other tests.		    */
/*									    */
/*		  8-23-82. FMB					            */
/****************************************************************************/



#include <stdio.h>

#define NUMLS 5					/* number of longs	    */
#define BUFFMAX BUFSIZ				/* Buffer size	            */
#define LSIZE 4					/* Longs are 4 bytes        */
#define CREATF "w"				/* To create and write to   */
						/* a file.	            */
#define RO "r"					/* To read from a file      */
#define APPMODE "a"				/* To append to a file      */
#define poslong1 0x21214948			/* Sample positive long     */
#define neglong1 -0x8655555			/* Sample negative long     */



/***************************************************************************/
/*			     BEGIN MAIN PROGRAM				   */
/***************************************************************************/


main()
{
WORD retw;					/* Function return value   */
FILE *strm1,*strm2;				/* Stream pointers         */
LONG *shadowbuf,
     *shadowbuf2;                               /* Buffers for setbuf 	   */
BYTE *mybuf,*mybuf2;                            /*    "     "    "         */
LONG lval1,lval2,lval3,lval4;			/* For getl,putl	   */
BYTE i;


						/***************************/
						/* Memory allocation and   */
                                                /* initialization.         */
						/***************************/


printf("\n               ***************          ");
printf("\n               Stream Test # 6          ");
printf("\n               ***************          \n\n"); 
printf("\nTesting memory allocation with malloc.....................\n");
mybuf = malloc(BUFFMAX);
mybuf2 = malloc(BUFFMAX);

printf("Testing fopen.............................................\n");

if ((strm1 = fopena("ONE",CREATF)) == NULL)
     printf("\nAscii file open failed.\n");
if ((strm2 = fopenb("TWO",CREATF)) == NULL)
     printf("\nBinary file open failed.\n");


						/*****************************/
                                                /* This section tests Setbuf */
						/*****************************/	 

printf("Testing setbuf..........................................\n");
if ((retw = setbuf(strm1,mybuf))== FAILURE)
     printf("\nSetbuf of ascii file failed.\n");
if ((retw = setbuf(strm2,mybuf2)) == FAILURE)
     printf("\nSetbuf of binary file failed.\n");


						/*****************************/
						/* This section tests Putl   */
						/*****************************/

printf("Testing Putl .........................................\n");

						/***************************/
						/*  Fill one buffer with   */
						/*  long values.           */
						/***************************/
for ( i=0; i < NUMLS; i++)
     if((retw = putl((LONG)poslong1,strm1)) == FAILURE)
         printf("\nPutl failed on ascii file at the %dth slot.",i);

for ( i=0; i < NUMLS; i++)
     if ((retw = putl((LONG)neglong1,strm1)) == FAILURE)
         printf("\nPutl failed on ascii file at the %dth slot.",i);

fputc('\0',strm1);

						/**************************/
						/*  Echo buffer to screen */
						/**************************/
   
printf("Buffered results of putl are : \n");
for( i=0 ; i < 2*NUMLS; i++ )
	printf("  %x%x%x%x",*mybuf++,*mybuf++,*mybuf++,*mybuf++);
        printf("\n");

if ((retw = rewind(strm1)) != SUCCESS){
	perror("rewind failed");
	printf("\nRewind returned a %d .\n",retw);
}
			
strm1 = freopa("ONE",RO,strm1);
        setbuf(mybuf,strm1);
printf("Filed results of putl (getl) are : \n");
for( i=0 ; i < 2*NUMLS; i++ )
	printf("  %X",getl(strm1));
strm1 = freopa("ONE",APPMODE,strm1);
        setbuf(mybuf,strm1);

						/**************************/
						/*  Use stdin to fill the */
						/*  other buffer.         */
						/**************************/

printf("\nPlease enter 2 positive long values \n");
printf("         and 2 negative long values.\n");
  
lval1 = getl(stdin);
lval2 = getl(stdin);
lval3 = getl(stdin);
lval4 = getl(stdin);

for (i = 0; i < (NUMLS * 4) ; i++)
     {
     if ((retw = putl(lval1,strm2)) == FAILURE)
          printf("\nPutl failed on binary stream at %dth slot.\n",i);
     if ((retw = putl(lval2,strm2)) == FAILURE)
          printf("\nPutl failed on binary stream at %dth slot.\n",i);
     if ((retw = putl(lval3,strm2)) == FAILURE)
          printf("\nPutl failed on binary stream at %dth slot.\n",i);
     if ((retw = putl(lval4,strm2)) == FAILURE)
          printf("\nPutl failed on binary stream at %dth slot.\n",i);
     }
fputc('\0',strm2);				/* Tack on the null      */

						/*************************/
						/*  Now echo them back to*/
						/*  the screen.          */
						/*************************/
if ((retw = rewind(strm2)) == FAILURE)
     printf("\nRewind failed on binary file.\n");
else
     {
     printf("\nBuffered results of putl : \n");
     for (i=0 ; i < NUMLS*4; i++)
          printf("  %d%d%d%d",*mybuf2++,*mybuf2++,*mybuf2++,*mybuf2++);
     strm2 = freopb("TWO",RO,strm2);
             setbuf(mybuf2,strm2);
     printf("\nFile results of putl : \n");
     for (i=0; i < NUMLS*4; i++)
          printf("  %X",getl(strm2));
     }
     strm2 = freopb("TWO",APPMODE,strm2);
             setbuf(mybuf2,strm2);


						/**************************/
						/*  Try direct write to   */
						/*  mybuffs.              */
						/**************************/

for ( i = 0; i < NUMLS; i++)
     {
     /*(LONG)*mybuf = 123456;*/
     /*printf("\nResults of direct write to buffer :   %X",(LONG)*mybuf++);*/
     }



						/***************************/
						/* This section closes and */
						/* opens the files         */
						/***************************/

if (fclose(strm1) == FAILURE)
    printf("\nCouldn\'t close file 1.\n");
if (fclose(strm2) == FAILURE)
    printf("\nCould\'t close file 2.\n");

if ((strm1 = fopena("ONE",RO)) == NULL)
      printf("\nFile open on ascii file failed.\n");
if ((strm2 = fopenb("TWO",RO)) == NULL)
      printf("\nFile open on binary file failed.\n");


						/*****************************/
						/* This section tests setbuf */
						/* and reads from the files. */
						/*****************************/



if ((retw = setbuf(strm1,mybuf2)) == FAILURE)
     printf("\nSetbuf on reopened ascii file failed.\n");
if ((retw = setbuf(strm2,mybuf)) == FAILURE)
     printf("\nSetbuf on reopened binary file failed.\n");

printf("\nTesting Putl on file 1. Should be HI! .\n");
     putl(getl(strm1),stdout);
printf("\nTesting same data from buffer:  %x%x%x%x\n",*mybuf2++,*mybuf2++,
                                                      *mybuf2++,*mybuf2++);

printf("Testing Putl on file 2. Should be your 4 inputs.\n");
     putl(getl(strm2),stdout);
printf("\nTesting same data from buffer:  %d%d%d%d\n",*mybuf++,*mybuf++,
                                                      *mybuf++,*mybuf++);


						/***************************/
						/*   Finish up by freeing  */
						/*   memory,closing files. */
						/***************************/
free(mybuf);
free(mybuf2);
_cleanup();


}

