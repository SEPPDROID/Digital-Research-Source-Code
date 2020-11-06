 

/*************************************************************************/
/*                                                                       */

/*       This program tests : fopen,printf,fclose,gets,puts,             */
/*                            fgets,fputs,freopen .                      */
/*       Written 8/4/82 for C run-time library. FMB                      */
/*                                                                       */

/*************************************************************************/
       

#include <stdio.h>                          /* Has stdin,BYTE,etc.*/
#define CREATF "w"                          /* To create a file */
#define RO "r"                              /* To reopen a RO file */
#define APPMODE "a"                         /* To append to a file */
#define ASCYES  0                           /* For ascii files  */
#define BINARY 1                            /* For binary files */
#define BUFFMAX 100                         /* Maximum buffer length */
#define LINSIZ 256                          /* Input file name */
#define MAXFILES 16                         /* Max number of files open */


main()
{
FILE *filarray[MAXFILES + 4];
FILE *fp,*fp2,*temp;                         /* Pointers to open files */
BYTE *testbuff,*echobuff;                    /* Mem buffers  for io */
WORD returnw;                                /* Function value */
BYTE returnb;                                /* Function value */
WORD maxchars = 80;                          /* Default mem buffer size */
LONG cursor = 0;                             /* # bytes in stream of file.*/
BYTE i;                                      /* loop counter */
BYTE fname[LINSIZ] ;                         /* For user file name */




      printf("\n\n       *************************************************\n\n");
      printf("             Begin C run-time test #1.    \n\n");                   
      printf("       *************************************************\n");


      /***********************************************************************/

      /*            This section tests memory allocation.                    */

      /***********************************************************************/
    
                                             /* Allocate buffer space */
      printf("\nTesting memory allocation...........................\n");
      testbuff = malloc(BUFFMAX); echobuff = malloc(BUFFMAX); 
                                             /* Test for malloc o.k. */
      if ((testbuff || echobuff) == NULL)
           printf("Malloc didn't make it. Stop here.\n");


      /***********************************************************************/

      /*              This section tests Fopen.                              */       

      /***********************************************************************/

                                             
      printf("Testing maximum open and reopen....................\n");
            opnall(filarray);			      /* Open every file */
            printf("Opened all.\n");	
            reopnall(filarray);		      /* Reopen as binary*/
            printf("Reopened all.\n"); 	
            dotwo(filarray);                          /* Release two of them */
            printf("Closed two.\n"); 

                                                      /* Open file to start */
      printf("Opening first file...................................\n");    
                                      /* Test fopen with a string filename. */
      fp = fopen("George",CREATF,ASCYES) ;
      if (fp == NULL)  printf("Couldn't get fopen to work on #1.\n ");

                              /* Test for standard input filename with fopen.*/
      printf("Opening second file. Please enter a new filename.\n");
      gets(fname);                            /* Get name from stdin */
      fp2 = fopen(fname,CREATF,ASCYES);
      if (fp2 == NULL) printf("#2 fopen didn't work.\n");
      

      /***********************************************************************/

      /*                 This section tests Gets and Puts.                   */

      /***********************************************************************/

                              /* Test for gets and puts . Simple echo first  */      
      printf("\nNow please enter a string ending with a CR.\n");
      if (gets(testbuff) == NULL) printf("Gets didn't make it.\n");
         else                   {printf("Echoing ......\n");
                                 returnw = puts(testbuff);
                                }

      /***********************************************************************/

      /*        This section tests Fputs, Fgets, and Freopen.                */

      /***********************************************************************/
  
                                         /* Test for fputs and fgets.        */
                                         /* Put something in George's stream.*/
      printf("Testing Fputs........................\n");
      if (fputs(testbuff,fp) == NULL)            /* Check for error in Fputs */     
                    printf("\nFputs didn't work.\n");
                                        /* Else,o.k. to test the other stuff.*/
                    else
                    { 
                      fp = freopen("George",RO,fp,ASCYES); /* So we can read */
                      fgets(testbuff,maxchars,fp);   /* Move stream to buffer*/
                      fputs(testbuff,stdout);        /* Move buffer to screen*/   

                      printf("\nPlease enter 3 complete lines of text .Do not use ctr Z .\n");

                      /* After this loop,all 3 lines should be in the stream,*/
                      /* only last line typed should be in the memory buffer.*/
                      
                       for (i = 0; i < 3 ; i++)
                          {
                           fgets(echobuff,maxchars,stdin);/* From terminal to memory buffer.*/
                           fputs(echobuff,fp2);     /* From buffer to stream.*/
                          }

                      printf("\nTesting to see if all 3 lines are in file stream.\n");
                      fp2 = freopen(fname,RO,fp2,ASCYES);
                      for (i = 0; i < 3; i++)
                          {
                           fgets(testbuff,maxchars,fp2);/* Move stream to buffer*/
                           fputs(testbuff,stdout);   /* Move buffer to screen*/
                          } 

                      
                      returnw = rewind(fp);   /* So we can read from the top */
                      fgets(echobuff,maxchars,fp);/* Should give first line in George */ 

                      printf("\nTesting Fseek ,Freopen and Fputs............\n");

                      fp2 = freopen(fname,APPMODE,fp2,ASCYES);/* So we can add it */
                      returnw = fseek(fp2,cursor,2); /* Go to EOF for append*/
                      if (fputs(echobuff,fp2) == FAILURE )
                                    printf("\nLast line append didn't work.\n");
                                    
                      printf("\nTesting puts and fputs...................\n");
                      fp2 = freopen(fname,RO,fp2,ASCYES);/* So we can read from it */
                      fgets(echobuff,BUFFMAX,fp2);       /* Move some of file into buffer.*/
                      printf("\n");
                      puts(echobuff);                          /* First echo.*/
                      printf("\n");
                      fputs(echobuff,stdout);                 /* Second echo.*/
                      printf("\nThat should've been the first line of three.\n");
                    }       

        /*********************************************************************/

        /*                 This section tests Fclose.                        */

        /*********************************************************************/
 
        printf("Testing Fclose.................................\n");                                                                                      /* Test for fclose . */  
       	if (fclose(fp) < 0)    printf("Couldn't close first file.\n");
        if (fclose(fp2) < 0)   printf("Couldn't close the second file.\n");
        
        printf("Testing some illegal file closes......................\n");
                                          /* Now try some illegal fcloses.   */
       printf("We\'ll call fclose on a file that\'s already been closed.\n");
       returnw = fclose(fp);              /* This one's already been closed. */
                 printf("Return val should be -1. It is %d .\n",returnw);


       /*  The  following test results in a  compiler  error,  as            */
       /*  fclose does not check for declared-but-not-defined arg's.         */
       /*  It has been commented out for obvious reasons.                    */

       /*  returnw = fclose(temp);             No such file.                 */
       /*        printf("Retval should be -1. It is %d .\n",returnw);        */

     
       printf("\n And that's all for this test. Bye ....................\n");
  }
                                               
						/****************************/
						/* Function Openall opens   */
                                                /* maximum # of files       */
                                                /****************************/            
opnall(filist)
FILE *filist[ ];
{
  BYTE i, alpha[2];
  
  alpha[1] = '\0';
  for (i=3 ; i != MAXFILES ; i++)
      {
       alpha[0] = 'a' + i; 
       filist[i] = fopena(alpha,CREATF);
       if (filist[i] == NULL)
           printf("\nFile # %s was not opened.\n",alpha);
      }
}
						/***************************/
						/* Reopenall reopens max # */
						/***************************/

reopnall(filist)
FILE *filist[ ]; 
{
 BYTE i,alpha[2];

 alpha[1] = '\0';
 for (i = 3 ; i != MAXFILES ; i++)
      {  
         if (filist[i] != NULL)
         {
         alpha[0] = 'a' + i;
         filist[i] = freopen(alpha,RO,filist[i]);
         }
         else         
         printf("\nFile # %s not reopened correctly.\n",alpha);
      }
}
						/**************************/
						/* Dotwo closes 2 files   */
						/* so they can be used    */
						/* for further testing.   */
						/**************************/
dotwo(filist)				
FILE *filist[ ]; 
{
 if (filist[4] != NULL)
 { 
 if (fclose(filist[4]) == FAILURE)
     printf("\nCouldn\'t close file # d .\n");
 }
 else
 printf("\n File #3 was NULL.\n");
 if (filist[5] != NULL)
 { 
 if (fclose(filist[5]) == FAILURE)
     printf("\nCouldn\'t close file # e .\n");
 }
 else
 printf("\nFile #4 was NULL.\n");

}
