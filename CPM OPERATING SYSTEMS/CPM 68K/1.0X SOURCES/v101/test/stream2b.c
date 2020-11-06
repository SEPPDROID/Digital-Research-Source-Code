/*****************************************************************************/
/*                                                                           */
/*       Test program 2 for C run-time library. Tests :                      */
/*       fopen,fclose,rewind,fwrite,fread,fseek,ftell,freopb.                */
/*       Program stream1.c should be run before stream2.c.                   */
/*       8/5/82 . FMB                                                        */
/*                                                                           */


/*****************************************************************************/

#include <stdio.h>

#define CREATF "w"                               /* Mode for create file.    */
#define READMODE "r"                             /* Mode for open-read .     */
#define APPMODE "a"                              /* Mode for append to file. */
#define G "George"                               /* Existing file. Make      */
                                                 /* sure you run Stream1 first.*/
#define S "Shirley"                              /* Gets created below.      */
#define ASCYES 1                                 /* Specifies binary file.   */
#define STDSIZ 100                               /* Standard block size.     */
#define BUFFMAX 100                              /* Maximum buffer length    */
#define SIZE 1                                   /* Number bytes in item     */
                                                 /* And for formatting...    */
#define STARS "***************************************************************"

main ()
{
FILE *fp,*fp1,*fp2,*temp;                         /* File pointers.           */
BYTE *strptr,*echobuff,*notherbuff;               /* Buffer pointers.         */
BOOLEAN greenlite ;                               /* To stop test before you  */
                                                  /* get into trouble .       */
WORD size = 1;                                    /* # bytes in an item       */
LONG cursor = 0;                                  /* # bytes in file stream.  */
WORD returnw;                                     /* Function ret value       */
BYTE i;
              
						/****************************/	
					        /*      Begin program.      */
						/****************************/

       printf("%-15s\n\n",STARS);
       printf("%-15s\n\n","Begin C library test # 2 : ");
       printf("%-15s\n",STARS);

       greenlite = TRUE;

						/****************************/
                                                /* This section tests memory*/ 
						/* allocation for buffers.  */
						/****************************/

       printf("\nTesting for memory allocation.......................\n");
                                 
       strptr = malloc(BUFFMAX); echobuff = malloc(BUFFMAX);
       notherbuff = malloc(BUFFMAX);
						/*****************************/
						/* Test for malloc o.k.      */
						/*****************************/
       if ((strptr && echobuff && notherbuff) == NULL) 
          {
          printf("Malloc didn't work. Stop here.\n");
          greenlite = FALSE;
          }
        else
          printf("Malloc succeeded on three successive calls,100 bytes each.\n");


/*****************************************************************************/

/*     This section tests fopen,fread,and ftell, all on one binary file. An   */      
/*     attempt is made to write one section (100 bytes) of the file (the     */
/*     results of fread) to the screen. File is opened as RO.                */

/*****************************************************************************/

						/*****************************/
                                                /* Open first file - George. */
						/*****************************/
          
       if (greenlite)
       {
       printf("Testing fopen..................................\n");
       fp = fopenb(G,READMODE,ASCYES) ;
       if (fp  == NULL )
                {
                  printf("Couldn\'t open first file.\n");
                  greenlite = FALSE;
                 }
                 else                           /*****************************/
						/* Ok to do more with George.*/
						/* Read from file to memory  */
						/* and to stream.            */
						/*****************************/
                 {
                  printf("Testing fread...................................\n");
                  if ((returnw = fread(strptr,SIZE,STDSIZ,fp)) == NULL )
                           {
			    if (feof(fp) != 0)
                            { 
                            printf("Fread unable to read from file........\n");
                            printf("Fread returned %D .\n",returnw);
                            greenlite = FALSE; 
                            }
                            else
                            printf("Fread read up to the EOF.");
                           } 
                  if (greenlite)
                             {
			     printf("Testing ftell on binary file..........\n");
                             cursor = ftell(fp);      
                             printf("\nCursor is %ld after ftell.\n",cursor);

                             				/*********************/
                                                        /* Now print it out. */
							/*********************/
                             printf("Testing fputs on fread ...............\n");
                             if (fputs(strptr,stdout) < 0 )
                                 printf("\nFputs should have printed contents of George.\n");
                             }
                   }
       }   						/*********************/
                                             		/* Try to write to it*/
       if (greenlite )					/*********************/
       {
       printf("\nTesting realloc and illegal write..........................\n");
       printf("Enter a string for first file. At this point,file is RO.\n");
       printf("Make your entry at least 500 bytes\' worth.\n");
        
       notherbuff = realloc(notherbuff,BUFFMAX * 5);
       fgets(notherbuff,BUFFMAX * 5,stdin);  /* Get something into buffer 1st*/
       if (fputs(notherbuff,fp) >= 0 ) 
                           printf("\nFputs should not have worked here.\n");
       }

						/*****************************/
                                                /* Now do a rewind on George.*/
						/*****************************/
       if (greenlite )
       {
       printf("Testing rewind on binary file..........................\n"); 
       if (rewind(fp) < 0 )
                    {
                      printf("Rewind failed on binary file.\n");
                      greenlite = FALSE;
                     }
        }
						/*****************************/
						/* Reopen George so we can   */
                                    		/* write to him.             */
						/*****************************/
       fp = freopb(G,APPMODE,fp,ASCYES);
       if (fp == NULL )
                {
                 printf("Reopening of first file didn\'t work.\n");
                 greenlite = FALSE;
                 }
                 else
                 {
                 printf("File is reopened in append mode.................\n");
                 greenlite = TRUE;              /*****************************/ 
						/* Since we're starting over */
						/* again anyway.             */
						/*****************************/ 	
                 }

       if ( greenlite )
          {
           printf("Please enter a string of 80 chars followed by a CR.\n");
           fgets(notherbuff,STDSIZ,stdin);
						/*****************************/
						/* Data's in buffer. Move    */
						/* cursor to end of current  */	                                                                             
						/* stream.                   */
           					/*****************************/
           printf("Testing fseek on binary file...........................\n");

           fseek(fp,0,2);
						/****************************/
						/* Now append buffer to the */
                  				/* end of George's stream.  */
						/****************************/
           printf("Testing fwrite with your 80 chars......................\n");
           if (fwrite(notherbuff,SIZE,STDSIZ,fp) < 0 )
                  {
                   printf("Attempt to append to first file failed.\n");
                   greenlite = FALSE;
                   }
           }

       if (greenlite )
          {
           printf("Testing fputc and ftell on binary file.................\n");
						/*****************************/
           fputc(NULL,fp);                      /* Tack on the null          */
           cursor = ftell(fp);                  /* Find out how much to read.*/
	   					/*****************************/
           printf("After ftell,cursor is %ld.\n",cursor);
              
						/*****************************/
						/* Reopen file for Reading   */
            fp = freopb(G,READMODE,fp);		/*****************************/              
           if ((returnw = fread(echobuff,SIZE,cursor,fp)) == NULL)
               printf("\nAttempt at Fread on file one failed or saw EOF.\n");
               printf("Number bytes read fro file: %ld.\n",returnw);

           /* This should have read file and null char into memory buffer.*/
           /* Now printf the whole thing out to the terminal.*/

           printf("Echoing appended file to the screen.\n");
           if (fputs(echobuff,stdout) < 0 )
                          printf("\nAs you can see, that wasn\'t right.\n");   
           }
                   
                                             	/* Do another file           */
       if (greenlite )
       {
       printf("Opening second file.....................................\n");
       fp2 = fopenb(S,CREATF,ASCYES);
       if (fp2 == 0 )
           printf("Couldn\'t open second file.\n");
                            			/* Open was successful, do a write.*/
       strptr = echobuff;                   	/* Save this                */
       printf("Filling file with test pattern..............\n");  
       for (i = 1; i < BUFFMAX; i++)
           *echobuff++ = 'a';
       echobuff = strptr;                   /* Restore buffer pointer */
                                            /* Now write to file      */
       if (fwrite(echobuff,1,BUFFMAX - 1,fp2) != (BUFFMAX -1))
           printf("\nFwrite of 100 chars returned bad count.\n");
           putc(NULL,fp2);                  /* Tack on the null        */
                                            /* Try to retrieve what's  */
                                            /* in there anyway.        */
        printf("\nReading from second file.........................\n");                                                
        fp2 = freopb(S,READMODE,fp2,ASCYES);/* So we can read from it  */
        fgets(strptr,BUFFMAX,fp2);         /* Put from stream to memory */
        fputs(strptr,stdout);                /* Send it to the screen   */
                 
       }
          					/*************************/
                                  		/* As a last test, close */
						/* all files.            */
						/*************************/  
          printf("\n\n\t\t\tEnd of Test # 2\n");
          _cleanup();
       }                                    /* End main.*/







