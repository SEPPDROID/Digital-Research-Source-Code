





/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                 Test 4 for C run-time library.                            */
/*                 Tests the get and put families for char and word          */
/*                 manipulations :                                           */
/*                                fgetc(getc),fputc(putc),ungetc,            */
/*                                putw,getw,getchar,putchar.                 */
/*                 May be run independently of other tests.                  */
/*                 Uses one ascii and one binary file.                       */
/*                 8-10-82. FMB                                              */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/



#include <stdio.h>

#define CREATF "w"                       /* Create and write mode */
#define RO "r"                           /* Read only mode        */
#define APPMODE "a"                      /* Append mode           */
#define BUFFMAX 100                      /* Size of memory buffer */
#define BSIZE 1                          /* For byte-size items   */
#define WSIZE 2                          /* For word-size items   */
#define ASCYES 1                         /* For ascii files       */
#define BINARY 0                         /* For nonascii files    */
#define T1 "test1"                       /* File one              */
#define T2 "test2"                       /* File two              */


/*****************************************************************************/

/*                       Begin Program                                       */

/*****************************************************************************/

main()
{

FILE *fp, *fp2;                           /* Stream pointers                 */
WORD *buff2,*tempbuff2;                   /* Buffer pointers                 */
BYTE *buff1, *tempbuff1;                  /* Buffer pointers                 */
WORD *chr1, *chr2, *chr3;                 /* Char var's for fgetc,fputc      */
WORD valword,retword;                     /* Word var's for getw,putw        */
BOOLEAN goahead = TRUE;                   /* Breakpoint variable             */
BYTE i;                                   /* Loop counter                    */
BYTE retbyte;                             /* Byte var for fgetc              */
BYTE chrc1,chrc2;                          /* Char's for putchar              */
WORD ch1,ch2;

 
printf("\n                    >>>>>>>>  <<<<<<<<     \n");
printf("\n                    BEGIN STREAM4 TEST     \n");

/*****************************************************************************/

/*                        Initialization                                     */

/*****************************************************************************/
 

fp = fopen(T1,CREATF,ASCYES);                /* Open the files          */
fp2 = fopen(T2,CREATF,BINARY);


/*****************************************************************************/                                         

/*   This section tests malloc and realloc while initializing buffers.       */

/*****************************************************************************/

printf("\nTesting memory allocation.................................\n");
buff1 = malloc(BUFFMAX/2); buff2 = malloc(BUFFMAX/4);
if ((buff1 || buff2 ) == FAILURE)
   {
   printf("\nMalloc didn't like something. Maybe the size argument.\n");
   goahead = FALSE;
   }
     
buff1 = realloc(buff1,BUFFMAX); buff2 = realloc(buff2,BUFFMAX);
if ((buff1 || buff2 )== FAILURE) 
   {
   printf("Realloc didn't succeed here.\n\n");
   goahead = FALSE;
   }

tempbuff1 = buff1;                         /* Save buffer start addresses   */
tempbuff2 = buff2;


/*****************************************************************************/

/*             This section tests getchar and getw.                          */

/*****************************************************************************/

printf("\nBegin testing getchar....................................\n");

printf("Please enter 100 characters to test getchar.Blanks are valid char's.\n"); 
for (i = 0; i < BUFFMAX; i++)             
    *buff1++  =  getchar();                 /* Fill buffer with chars   */
    *buff1 = NULL;                          /* Tack on the NULL         */
    buff1 = tempbuff1;                      /* Reset buffer pointer     */
                                            /* Now echo it back         */
printf("Here is an echo of your entry.\n%s\n",buff1);    

printf("Please enter 20 16-bit items to test getw.\n");
for (i = 0; i < BUFFMAX/5; i++)
    *buff2++ = getw(stdin);                  /* Fill buffer with words */
    *buff2 = NULL;                           /* Tack on a NULL         */ 
    buff2 = tempbuff2;                       /* Reset buffer pointer   */
                                             /* Now echo it back       */
printf("Here is an echo of your entry.\n%s\n",buff2);
    

/*****************************************************************************/

/*            This section tests Fputc and Putw.                             */

/*****************************************************************************/

printf("Testing fputc ........................................\n");

                                              /* Copy buffers to streams     */

while (*buff1 != NULL)
      fputc(*buff1++,fp);
      *buff1 = NULL;                          /* Tack on the null            8/ 
      buff1 = tempbuff1;                      /* Reset buffer pointer        */

printf("Testing putw .........................................\n");

while (*buff2 != NULL)
      putw(*buff2++,fp2);
      *buff2 = NULL;                          /* Tack on the null            */
      buff2 = tempbuff2;                      /* Reset buffer pointer        */


/*****************************************************************************/

/*             This section tests Putchar, Fgetc and Ungetc.                 */

/*****************************************************************************/

printf("Testing fgetc and putchar ..............................\n");

fp = freopen(T1,RO,fp,ASCYES);          /* So we can read from file     */

while ((retword = fgetc(fp)) != FAILURE)      /* Put char's on screen   */
        putchar(retword);

valword = ungetc(retword,fp);                 /* Push EOF back on stream    */


/*****************************************************************************/

/*              This section tests Getw and Putw.                            */

/*****************************************************************************/

fp2 = freopen(T2,RO,fp2,BINARY);        /* So we can read from the file*/   

while ((retword = getw(fp2)) != FAILURE)/* Get word from stream to screen */
        putw(retword,stdout);

valword = ungetc(retword,fp2);          /* Push EOF back on stream         */


/*****************************************************************************/  

/*              This section tests char-word exchanges.                      */

/*****************************************************************************/

printf("\nTesting char-word output.........................\n");

putchar('5');                                /* Put it on the screen        */

printf("\nThat was the character. Now let's show it as a word.\n");

putw('5',stdout);

printf("\nThat was putw with the same argument.\n");

chrc1 = 53;                                  /* Output another 5 */
putchar(chrc1);
printf("\n");

chrc1 = 33;                                  /* Output an ! */
putchar(chrc1);
printf("\n");

ch1 = 0x6948;                                /* Output Hi  */
putw(ch1,stdout);
printf("\n");

ch2 = 0x3231;                                /* Output 1234 */
putw(ch2,stdout);
ch1 = 0x3433;
putw(ch1,stdout);
printf("\n");


}
