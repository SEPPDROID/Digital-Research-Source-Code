
/*****************************************************************************/
/*									     */
/*                Test Program for Sscanf and scanf.			     */
/*		  8-17-82.  FMB.					     */
/*                Modified by B.Fitler					     */	
/*									     */
/*****************************************************************************/





#include <stdio.h>

#define G "This is a string...with dots & spaces. \0!"
#define F "5691a24This is a buffer string.  z37"
#define H "$$$...........$$$$\0"
#define BUFFMAX 100
#define E "-1234567890"
#define E1 "%3hd%*2d%2hd%*2d%2hd"


					
main()
{

WORD wd1,wd2,wd3;                        /* For hex,octal,etc.  */
BYTE ch1,ch2,ch3,;                       /* For char's          */ 
LONG dec1,dec2,dec3,dec4;                /* For decimals        */
WORD ni1,ni2;                            /* Item counts         */
BYTE buff1[BUFFMAX],buff2[BUFFMAX];	 /* String buffers      */

printf("\n\t\tBegin Test # 7 for Sscanf and scanf.\n\n");

                                         /************************/
                                         /*   Begin Program      */
			 		 /*   Test one string    */
                                         /************************/

ni1 = sscanf(G,"%[^!]",buff1);           
if (ni1 != 1)
    printf("\nSscanf returned bad item count.\n");
if (strcmp(G,buff1) != NULL)
    printf("\nSscanf failed on whole string transfer.\n");

ni1 = sscanf(H,"%s",buff2);
if (ni1 != 1)
    {
    printf("\nSscanf returned bad item count.\n");
    printf("Nitems was %d .\n",ni1);
    }

					/************************/
                                        /*  Print out both.     */
					/*  They should match.  */
    					/************************/
    printf("Testing echo to screen.........................\n");
    puts(buff1);
    puts(buff2);

					/************************/
					/*   Try multiple arg's */
					/************************/

ni2 = sscanf(F,"%4D%3x%[^.]. %c%ho",&dec1,&wd1,buff1,&ch1,&wd2);
if (ni2 != 5)
    {
    printf("\nSscanf returned bad item count.\n");
    printf("Nitems was %d.\n",ni2);
    }
if (dec1 != 5691)
    printf("\n6-digit decimal not transferred correctly.\n");
if (wd1 != 0xa24)
    printf("\n3-digit hex not done correctly.\n");
if (ch1 != 'z')
    printf("\nSingle char not assigned correctly.\n");
if (wd2 != 037)
    printf("\n2-digit octal not assigned correctly.\n");
    

					/***********************/
                                        /*   Print string      */
					/***********************/
    puts(buff1);
    printf("\n");

					/***********************/
					/*   Unix examples.    */
					/***********************/

printf("Waiting for one decimal # (2 digits) and one string.\n");	
       ni2 = scanf("%d %s",&wd3,buff2);
       if (ni2 != 2)
           printf("\nScanf returned bad item count.\n");
       printf("%d %s",wd3,buff2);
     
printf("\nWaiting for a decimal # (3 digits) and a name.\n");
       ni2 = scanf("%2d%*d%s",&wd2,buff2);
       if (ni2 != 2)
	   {
           printf("\nScanf returned bad item count.\n");
           printf("Item count was %d.\n",ni2);
           }
           printf("Decimal should be 2 of 3 digits :%2d and the name %s.\n",
                    wd2,buff2);  


						/**************************/
						/*  More sscanf stuff.    */
						/**************************/

#define E2 "%3ld%*2d%2ld%*2d%2ld"
ni1 = sscanf(E,E2,&dec1,&dec2,&dec3);
printf("Sscanf:\tscanned\t///%s///\tusing\t%s\n\t\tfound  (%d)\t%ld %ld %ld\n",
                   E,E2,ni1,dec1,dec2,dec3);

#define E3 "%3D%*2D%2D%*2D%2D"
ni1 = sscanf(E,E3,&dec1,&dec2,&dec3);
printf("Sscanf:\tscanned\t///%s///\tusing\t%s\n\t\tfound (%d)\t%ld %ld %ld",
                E,E3,ni1,dec1,dec2,dec3);

#define E4 "%3d%*2d%2d%*2d%2d"
ni1 = sscanf(E,E4,&dec1,&dec2,&dec3);
printf("\nSscanf:\tscanned\t///%s///\tusing\t%s\n\t\tfound (%d)\t%d %d %d\n",
                E,E4,ni1,dec1,dec2,dec3);

printf("\n\n\t\t\tEnd Test # 7\n");
                                        	/***********************/
						/*   End for now.      */
						/***********************/

}
 

