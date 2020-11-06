#define VN "\nDATE Vn 1.0 02/18/83"
/* This utility accepts the current date from the command tail
   validates it, and set the internal system date in the BIOS.
   Alternatively, it can be requested just to display the current
   system date. */

#include <LIBRARY.H>

char *date;		/* pointer to the date in the config. block */
char *date_flag;	/* pointer to date set flag */
int mm,dd,yy;		/* variables to hold month, day, year */
int mcount;		/* match count of numeric values entered */
int count;		/* count used to add leading 0's to date */

main(argc,argv)
int argc;
char *argv[];
{
printf(VN);		/* display signon message */
date = get_cba(CB_DATE);	/* set pointer to date */
date_flag = get_cba(CB_DTFLAGS);	/* set pointer to "date set" flag */

if (argc != 2)		/* check if help requested (or needed) */
	show_use();	/* display correct usage and exit */

if (usstrcmp("SHOW",argv[1]))	/* check if not SHOW option */
	{
		/* convert specified time into month, day, year */
	mcount = sscanf(argv[1],"%d/%d/%d",&mm,&dd,&yy);
	if (mcount != 3)		/* input not numeric */
		show_use();	/* display correct usage and exit */

		/* NOTE : The following validity checking is VERY
		   simplistic - this could be expanded to accomodate
		   more context-sensitive checking, days in the month,
		   leap years, etc. */
	if (mm > 12 || mm < 1)	/* check valid month, day, year */
		{
		printf("\nMonth = %d is illegal.",mm);
		show_use();	/* display correct usage and exit */
		}
	if (dd > 31 || dd < 1)
		{
		printf("\nDay = %d is illegal.",dd);
		show_use();	/* display correct usage and exit */
		}
	if (yy > 90 || yy < 83)	/* <=== NOTE ! */
		{
		printf("\nYear = %d is illegal.",yy);
		show_use();	/* display correct usage and exit */
		}

		/* convert integers back into a formatted string */
	sprintf(date,"%2d/%2d/%2d",mm,dd,yy);
	date[8] = 0x0A;		/* terminate with line feed */
	date[9] = '\0';		/* new string terminator */

		/* change " 1/ 2/ 3" into "01/02/03" */
	for (count = 0; count < 7; count+=3)
		{
		if (date[count] == ' ')
			date[count] = '0';
		}

		/* turn flag on to indicate that the user has set the date */
	*date_flag |= DATE_SET;
	}
printf("\n\tCurrent Date is %s",date);
}

show_use()		/* display correct usage and exit */
{
printf("\nDATE sets the system date. Usage is :");
printf("\n\tDATE mm/dd/yy");
printf("\n\tDATE SHOW (to display current date)\n");
exit();
}

