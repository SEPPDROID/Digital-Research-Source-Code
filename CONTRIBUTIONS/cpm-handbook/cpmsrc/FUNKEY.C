#define VN "\nFUNKEY Vn 1.0 02/18/83"

#include <LIBRARY.H>


int fnum;			/* function key number to be programmed */
char fstring[20];		/* string for function key */
struct _fkt *pfk;		/* pointer to function key table */

main(argc,argv)
int argc;
char *argv[];
{

if (argc == 1 || argc > 3)
	show_use();

pfk = get_cba(CB_FKT);	/* set pointer to function key table */

if (usstrcmp("SHOW",argv[1]))
	{
	if (!isdigit(argv[1][0]))
		{
		printf("\n\007'%s' is an illegal function key.",
			argv[1]);
		show_use();
		}

	fnum = atoi(argv[1]);	/* convert function key number */

	if (fnum > FK_ENTRIES)
		{
		printf("\n\007Function key number %d too large.",fnum);
		show_use();
		}

	if (get_fs(fstring) > FK_LENGTH)
		{
		printf("\n\007Function key string is too long.");
		show_use();
		}


	pfk += fnum;	/* update pointer to string */
		/* copy string into function key table */

		/* check if function key input present */
	if (!(pfk -> fk_input[0]))
		{
		printf("\n\007Error : Function Key #%d is not set up to be programmed.",fnum);
		show_use();
		}
	strcpy(pfk -> fk_output,fstring);
	}
else		/* SHOW function specified */
	{
	printf(VN);		/* display signon message */
	show_fun();
	}
}

get_fs(string)		/* get function string from command tail */
char string[];		/* pointer to character string */
{
char *tail;		/* pointer to command tail */
short tcount;		/* count of TOTAL characters in command tail */
int slen;		/* string length */

tail = 0x80;		/* command line is in memory at 0080H */
tcount = *tail++;	/* set TOTAL count of characters in command tail */
slen = 0;		/* initialize string length */

while(tcount--)		/* for all characters in the command tail */
	{
	if (*tail++ == '"')	/* scan for first quotes */
		break;		
	}
if (!tcount)		/* no quotes found */
	{
	printf("\n\007No leading quotes found.");
	show_use();
	}

++tcount;		/* adjust tail count */
while(tcount--)		/* for all remaining characters in tail */
	{
	if (*tail == '"')
		{
		string[slen] = '\0';	/* add terminator */
		break;		/* exit from loop */
		}
	string[slen] = *tail++;	/* move char. from tail into string */

	if (string[slen] == '<')
		string[slen] = 0x0A;
	++slen;
	}
if (!tcount)		/* no terminating quotes found */
	{
	printf("\n\007No trailing quotes found.");
	show_use();
	}		
return slen;		/* return string length */
}


show_fun()		/* display settings for all function keys */
{
struct _fkt *pfkt;	/* local pointer to function keys */
int count;		/* count to access function keys */
char *lf;		/* pointer to '<' character (line feed) */

pfkt = get_cba(CB_FKT);	/* set pointer to function key table */

for (count = 0; count <= FK_ENTRIES; count++)
	{
	if (pfkt -> fk_input[0])	/* key is programmed */
		{
			/* check if at physical end of table */
		if (pfkt -> fk_input == 0xFF)
			break;	/* yes - break out of for-loop */
		strcpy(fstring,pfkt -> fk_output);
			/* convert all 0x0A chars to '<' */
		while (lf = strscn(fstring,"\012"))
			{
			*lf = '<';
			}

		printf("\n\tKey #%d = '%s'",count,fstring);
		}
	++pfkt;		/* move to next entry */
	}
}


show_use()
{
printf("\nFUNKEY sets a specific function key string.");
printf("\n\tFUNKEY key-number \042string to be programmed<\042 ");
printf("\n\t           (Note : '<' is changed to line feed.)");
printf("\n\t           (        key-number is from 0 to %d.)",
	FK_ENTRIES-1);
printf("\n\t           (        string can be up to %d chars.)",
	FK_LENGTH);
printf("\n\tFUNKEY SHOW        (displays settings for all keys)");
exit();
}

