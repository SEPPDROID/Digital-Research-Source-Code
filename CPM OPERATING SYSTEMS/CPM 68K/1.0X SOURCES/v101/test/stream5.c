 





/*****************************************************************************/
/*									     */
/*									     */
/*               This program tests the String Manipulation                  */
/*               functions in the C run-time library. Tests:                 */
/*               strlen,strcat,strncat,strcpy,strncpy,strcmp                 */
/*               and strncmp.                                                */
/*		 May be tested independently of the other        	     */
/*               stream tests.						     */
/*               8-16-82. FMB.                                               */
/*									     */
/*****************************************************************************/


#include <stdio.h>

#define u " you"
#define r " are"
#define h " Here"
#define BUFFMAX 100
#define a  "\nHello there, this is your captain speaking.\n"
#define b  "\nWing Commanders - please report to the flight deck.\n"
#define c  "\nJello there, this is your captain speaking.\n"


main()
{                                          /* String pointers */ 
BYTE *string1, *string2, *string3, *temp1, *temp2, *holding;
WORD retval;


/*****************************************************************************/
/*                           						     */
/*                          Begin Program                                    */
/*									     */
/*****************************************************************************/


/*    This section initializes variables and tests memory allocation.        */

printf("\n\n                    BEGIN STREAM5 TEST   \n");
printf("\nTesting memory allocation..............................\n");

temp1 = malloc(BUFFMAX);
string1 = malloc(BUFFMAX);
temp2 = malloc(BUFFMAX);
string2 = malloc(BUFFMAX);
string3 = malloc(BUFFMAX);                                 			      
holding = malloc(BUFFMAX);

if ((temp1 || string1 || temp2 || string2 || string3 || holding) == NULL)
   printf("\nMemory allocation unsuccessful. Stop here.\n");
else
   printf("Memory allocation successful. Continue tests.\n");

						/**************************/
						/* Set up primary strings.*/
						/**************************/
printf("Testing strcopy........................................\n");

string1 = strcpy(string1,a);
temp1 = strcpy(temp1,string1);
string2 = strcpy(string2,b);
temp2 = strcpy(temp2,string2); 
string3 = strcpy(string3,c);


/****************************************************************************/
/*             This section tests strcat and strncat.                       */
/****************************************************************************/


printf("Following are string1 and string 2.\n %s %s \n",string1,string2);
printf("Testing string concatenation...........................\n");

						/***********************/
                                                /* Cat the 2 strings   */
                                                /* Check lengths       */
						/***********************/

   if ((strlen(string1) + strlen(string2)) == strlen(strcat(string1,string2)))
   {
   printf("Let\'s see the results of strcat on the 2 strings.\n");
   puts(string1); printf("\n");
   }
else                                           /* Lengths don't match.*/
   printf("\nError in strcat. Length comparison was off.\n");

                                               /* Now try an illegal call */
printf("Testing illegal strcat.........................\n");
holding = strcat(string1,"go");                /* No null terminator here */
if (holding == FAILURE)
   printf("Strcat failed on cue.\n");
else
   printf("Strcat should not have worked here.\n");

                                               /* Now test strncat  */
printf("Testing strncat..........................\n");
string1 = strcpy(string1,temp1);              /* Restore original string */
holding = strncat(string1,h,strlen(string1) + strlen(h));
          printf("You should now see string1 with >here< added to it.\n");
          puts(holding); printf("\n");

holding = strncat(string1,u,strlen(string1) + strlen(u)); /* Add "you" */
holding = strncat(string1,r,strlen(string1) + strlen(r)); /* Add "are" */
holding = strncat(string1,"!\0",strlen(string1) + strlen("!\0"));/* Add "!" */
          printf("You should now see a 3 word sentence added on.\n");
          puts(holding);printf("\n");


/****************************************************************************/
/*               This section tests strcmp and strncmp.                     */
/****************************************************************************/


printf("Testing strcmp..........................................\n");

string1 = strcpy(string1,temp1);             /* Restore original string   */
if (strcmp(string1,string1) == NULL)         /* Try two identical strings.*/
   printf("Strcmp matched %s and %s\n successfully.\n",string1,string1);
else
   printf("Strcmp could\'nt match two identical strings.\n");

if (strcmp("goo \0","go o\0") == NULL)   /* Try 2 strings off by one letter*/
   printf("Strcmp could\'nt distinguish between goo and go o.\n");
else
   printf("Strcmp knew the difference between goo and go o.\n");

if (strcmp(string1,string2) != 0)        /* Different char's and lengths */
   printf("Strcmp distinguished between %s and %s\n",string1,string2);
else
   printf("Strcmp could\'t tell the difference between %s and %s\n",
             string1,string2);

if (strcmp(string1,string3) == NULL)   /* One-char difference,equal lengths*/
   printf("Strcmp didn\'t distinguish between %s and %s\n",string1,string3);
else
   printf("Strcmp distinguished between %s and %s\n",string1,string3);

                                      /* Now test strncmp */

printf("Testing strncmp....................................\n");

if (strncmp(string1,string3,5) == 0)   /* Compare first 5 letters */
    printf("Strcmp couldn\'t tell %s from %s.\n","Hello","Jello");
else
    printf("Strcmp knew %s from %s.\n","Hello","Jello");

if (strncmp("aaaaaaa","aaaaaab",6) != 0) /* Just compare the a's*/
   printf("Strncmp didn\'t work on two strings of a\'s.\n");
else
   printf("Strncmp picked off 6 a\'s from each of 2 strings and matched them.\n");



/****************************************************************************/
/*                This section tests strcpy and strncpy.                    */
/****************************************************************************/


printf("Now testing strcpy......................................\n");

string1 = strcpy(string1,temp1);  string2 = strcpy(string2,temp2);/* Restore original strings.*/
string2 = strcat(string2,temp2);
holding = strcpy(string1,string2);
          printf("You should now see 2 copies of string2.%s\n",string1);                  
                                      /* Now double the copy,add "and" */
holding = strcat(string2,"\nand\n");
holding = strcat(string2,temp2);
holding = strcpy(string1,string2);    /* Put whole thing back into string1 */
          printf("You should next see 3 copies of string1.%s\n",string1);

printf("Now testing strncpy.......................................\n");

string1 = strcpy(string1,temp1);                 /* Restore original string */

holding = strncpy(string1,string2,strlen(string2));
          printf("You should next see a copy of string2 in string1.%s\n",
                  string1);
holding = strncpy(string2,strcat(temp1,temp2),BUFFMAX);
          printf("You should then see a copy of both strings in string2.%s\n",
                  string2);


/****************************************************************************/

                                       /* Now free up memory and test FREE  */
printf("Testing memory free function.............................\n");

free(temp1);
free(string1);
free(temp2);
free(string2);
free(string3);
free(holding);

printf("\n\n                      Ending	 Stream5  Test  \n");

}

