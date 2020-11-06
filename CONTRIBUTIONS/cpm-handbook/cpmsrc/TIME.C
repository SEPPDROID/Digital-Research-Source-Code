#define VN "\nTIME Vn 1.0 02/18/83"
/* This utility accepts the current time from the command tail
   validates it, and set the internal system time in the BIOS.
   Alternatively, it can be requested just to display the current
   system time. */

#include <LIBRARY.H>

char *time;		/* pointer to the time in the config. block */
char *time_set;		/* pointer to the time set flag */
int hh,mm,ss;		/* variables to hold hours, minutes, seconds */
int mcount;		/* match count of numeric values entered */
int count;		/* count used to add leading 0's to time */

main(argc,argv)
int argc;
char *argv[];
{
printf(VN);		/* display signon message */
time = get_cba(CB_TIMEA);	/* set pointer to time */
time_flag = get_cba(CB_DTFLAGS);	/* set pointer to the time set flag */
hh = mm = ss = 0;	/* initialize the time in case seconds or
			   minutes are not specified */

if (argc != 2)		/* check if help requested (or needed) */
	show_use();	/* display correct usage and exit */

if (usstrcmp("SHOW",argv[1]))	/* check if not SHOW option */
	{
		/* convert time into hours, minutes, seconds */
	mcount = sscanf(argv[1],"%d:%d:%d",&hh,&mm,&ss);
	if (!mcount)		/* input not numeric */
		show_use();	/* display correct usage and exit */

	if (hh > 12)		/* check valid hours, minutes, seconds */
		{
		printf("\n\007Hours = %d is illegal.",hh);
		show_use();	/* display correct usage and exit */
		}
	if (mm > 59)
		{
		printf("\n\007Minutes = %d is illegal.",mm);
		show_use();	/* display correct usage and exit */
		}
	if (ss > 59)
		{
		show_use();	/* display correct usage and exit */
		printf("\n\007Seconds = %d is illegal.",ss);
		}

		/* convert integers back into formatted string */
	sprintf(time,"%2d:%2d:%2d",hh,mm,ss);
	time[8] = 0x0A;		/* terminate with line feed */
	time[9] = '\0';		/* new string terminator */

		/* convert " 1: 2: 3" into "01:02:03" */
	for (count = 0; count < 7; count+=3)
		{
		if (time[count] == ' ')
			time[count] = '0';
		}
		/* turn bit on to indicate that the time has been set */
	*time_flag |= TIME_SET;
	}

printf("\n\tCurrent Time is %s",time);
}

show_use()		/* display correct usage and exit */
{
printf("\nTIME sets the system time. Usage is :");
printf("\n\tTIME hh{:mm{:ss}}");
printf("\n\tTIME SHOW (to display current time)\n");
exit();
}

