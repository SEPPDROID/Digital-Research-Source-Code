






/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/*           This program tests the printf-scanf family of routines          */
/*           contained in the C run-time library.                            */
/*           Tests:    fprintf,sprintf,printf,puts,scanf,sscanf,             */
/*                     fscanf.                                               */
/*                                                                           */
/*           8-12-82 . FMB.                                                  */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>

#define C "Clarence"                                 /* An empty file.    */
#define READMODE "r"                                 /* RO files          */
#define CREATF "w"                                   /* New files         */ 
#define APPMODE "a"                                  /* Append files      */
#define ASCYES 0                                     /* Ascii file        */
#define BINARY 1                                     /* Binary file       */
#define BUFFMAX 100                                  /* Maximum buffer length */

main()
{
FILE *filstream;                                      /* Point to file buff*/
BYTE *buff1,*buff2,*buff3,*buff4;                     /* Memory buffers    */
WORD returnw;                                         /* Return values     */
BOOLEAN goahead ;                                     /* Prevents traffic jams.*/
WORD nitems,savitems;                                 /* # items read      */
BYTE dum_d, a;                                        /* Vars for scanf family */
LONG *dum_b[2],*dum_c[6],*c[10];                      /*  "   */
BYTE *name,*dum_a,*sav_a;                             /*  "   */



/*****************************************************************************/

/*                           Begin Program                                   */

/*****************************************************************************/

                                                     /* Print program header */
    printf("\n\n\n                ********************************************\n");
    printf("                              Begin C Test # 3.\n\n");
    printf("%10s \n\n","First we will test malloc on the buffers.");

                                                    /* Init breaker */
    goahead = TRUE;

    /*************************************************************************/

    /*  This section tests malloc while initializing memory buffers.         */

    /*************************************************************************/

    printf("Testing memory allocation..........................\n");
                                                    /* Allocate buffer space */
    buff1 = malloc(BUFFMAX); buff2 = malloc(BUFFMAX);
    buff3 = malloc(BUFFMAX); buff4 = malloc(BUFFMAX);
    dum_a = malloc(10);      name  = malloc(50);    
    if ((buff1 && buff2 && buff3 && buff4 && dum_a && name) == NULL)   
       {
        printf("Malloc failed on buffer allocation.\n\n");
        goahead = FALSE;
       }
    

    /*************************************************************************/

    /*   This section tests fscanf and sscanf with an arbitrary delimiter.   */
 
    /*************************************************************************/

                                         /* Now open a file for testing.  */
                 filstream = fopen(C,CREATF,BINARY);
                 if (filstream == NULL )
                 {
                 printf("File open failed.Stop here.\n\n");
                 goahead = FALSE;
                 }
                 else                                /* Else, coast is clear */
                                                 /* Go ahead and test fscanf.*/
                 {
                 printf("Testing Fscanf.................................\n");
                 printf("Please input file material ending with a \'!\'.\n");
                 savitems = nitems = fscanf(stdin,"%[^!]%*c",buff3);        
                                            /* Where ! is the file delimiter */
					    /* And c gobbles up the !        */                        
                        if (nitems == NULL )
                           {
                           printf("\nfscanf failed on terminal input.\n\n");
                           goahead = FALSE;
                           }
                        else
                           {
                           printf("Testing Sscanf...................\n");
                                          /* Now let's try sscanf to move  */
                                          /* this stuff into a mem buffer. */

                           filstream = fopena("Ace",CREATF);
                           nitems = sscanf(buff3,"%[^.]%*c",filstream);
                           if (nitems == NULL  )
                                         printf("Sscanf didn\'t make it.\n\n");
                           }
                           }     
                                   
    /*************************************************************************/

    /*   This section tests scanf and sprintf with 3 kinds of variables.     */

    /*************************************************************************/

    if (goahead )
    {
                                           /* Test scanf with dum_ var's. */
    printf("Please enter data for scanf test. For the first item \n");
    printf("type in a word. \n");
    printf("Next,type in one 5-digit decimal and one 2-digit \n");
    printf("hex number,with optional spaces in between items.\n");
    
    nitems = scanf("%s%5D%2x",dum_a,dum_c,dum_b);
    
    /* At this point, dum_a should have a 1-word string ending with a null.*/
    /* Dum_b should have 2 hex digits,and dum_c should have a 5-place decimal #*/

    if (nitems != 3)
                 {
                 printf("Scanf returned bad item count.\n");
                 printf("Nitems was %d .\n",nitems);
                 }
                 else            /*Else, go ahead and test sprintf.*/
                 {
                 printf("Testing sprintf...................\n");
                 buff2 = sprintf(buff2,"%s  %5D  %2x  ",dum_a,*dum_c,*dum_b); 
                 if (buff2 == NULL)
                 printf("\nSprintf didn\'t work.");
                 else
                 printf("\nEcho = %s\n",buff2);
		}
     } 				
     if (goahead)                              /* Now test scanf with printf */         
     {
      /* The spacing between control char's below is an intentional test. */
      /* Printf should ignore the spaces and just count arg's with types.*/

      printf("\n%c%c  %s%c %c  \n",'*',' ',"Please enter :  ",'*',' ');   
      printf("%5c%s%5c\n", ' ',"One 2-digit decimal number ,",' ');
      printf("%5c%s%c\n",' ',"one 6-digit decmal number,",' ');
      printf("%5c%s%c\n",' ',"and one filename.",' ');
      printf("%5c%s%5c",' ',"Intervening spaces are ok.CR's not allowed.\0",' ');
      printf("\n\n");
                                               /* Get data from terminal */
      nitems = scanf("%2D%6D%s",dum_c,c,name);

      if (nitems != 3)                         /* See if it flopped */
         printf("Scanf didn't make it.\n");
         
      if (goahead)              /* Should we bother trying to read it back?*/
         {
         printf("Echoing  values ................\n");
         printf("The 6-digit decimal number is : %D .\n",*c);
         printf("The 2-digit decimal number is : %D .\n",*dum_c);
         printf("The name is : %s .\n",name);

         printf("Testing field widths ...............\n");
         printf("This one should be 2 out of 6 digits : %40.2D\n",*c);
         printf("This one should move over some : %20.2D\n",*c);
         printf("This one should move back some : %-20.2D\n",*c);

         }

      }       
      


    /*************************************************************************/

    /*    This section tests puts by displaying contents of memory buffer.   */

    /*************************************************************************/

    if (goahead )
    {                       /* Use puts to echo the buffer results to screen.*/
    if (puts(buff3) == FAILURE )
         printf("\nPuts didn't echo buff to screen.\n");
     
    }

     /************************************************************************/

     /*   This section tests fprintf (with an append) and printf.            */

     /************************************************************************/

     if (goahead )
        {
         filstream = fopen("Ace",CREATF);         
         if (fprintf(filstream,"%s",buff2) == FAILURE )
             printf("Couldn\'t append with fprintf.\n\n");
         else
            {  
             filstream = freopen("Ace",READMODE,filstream);   
             nitems = fscanf(filstream,"%s",buff1);
             printf("Echoing..... %s \n",buff1);
            }

        }

     } /* End program */

