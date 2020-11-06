#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

int status = 0;
int filep = 0;
char *stdincl;		/* Include prefix for macro processor	*/
/* cexit - exit from C compiler driver*/
/*		This deletes any existing temps and exits with the error status.*/
cexit()									/* returns - none*/
{

	exit(status);
}

/* main - main routine for C68 Compiler system*/
/*		Handles the C68 arguments.  For each C file given, the macro*/
/*		pre-processor is called, then the parser, code generator and*/
/*		assember are fexec'd.  The loader arguments are collected and*/
/*		the loader is fexec'd.*/
main(argc,argv)
int argc;
char **argv;
{

	if(argc < 3)
		usage();

	if (*argv[1] == '-')
	{
		if(argc != 5 || argv[1][1] != 'i')
			usage();

		stdincl = argv[2];
		domacro(argv[3],argv[4],0,0L);
		cexit();
	}
	if(argc != 3)
		usage();
	domacro(argv[1],argv[2],0,0L);
	cexit();
}
usage()
{
		error("usage: c68 [-i x:] inputfile outputfile\n");
		exit();
}

/* strcmp - string comparison*/
/*		Compares two strings for equality, less or greater.*/
strcmp(s,t)								/* returns 0 for equality,*/
										/* neg for < and pos for >.*/
char *s;								/* first string*/
char *t;								/* second string*/
{
	for( ; *s == *t; s++, t++ )
		if( *s == '\0' )
			return(0);
	return( *s - *t );
}

/* strlen - string length*/
/*		Computes number of bytes in string.*/
strlen(s)								/* returns string length*/
char *s;								/* string to compute length*/
{
	register int n;

	for( n = 0; *s++ != '\0'; )
		n++;
	return(n);
}

/* itoa - integer to ASCII conversion*/
/*		Converts integer to ASCII string, handles '-'.*/
itoa(n,s,w)								/* returns - none*/
int n;									/* number to convert*/
char *s;								/* resulting string*/
int w;									/* minimum width of string*/
{
	register int sign, i;
	char temp[6];

	if( (sign=n) < 0 )
		n = -n;
	i = 0;
	do {
		temp[i++] = n % 10 + '0';
	} while( (n =/ 10) > 0 );
	if( sign < 0 )
		temp[i++] = '-';
	while( --w >= i )					/*pad on left with blanks*/
		*s++ = ' ';
	while( --i >= 0 )					/*move chars reversed*/
		*s++ = temp[i];
	*s = '\0';
}

/* strend - set string end*/
/*		This is used to compare the endings of file names for ".c", etc.*/
strend(s,t)								/* returns 1 if match, 0 otherwise*/
char *s;								/* string to compare*/
char *t;								/* string ending*/
{
	int ls, lt;

	if( (ls=strlen(s)) < (lt=strlen(t)) )
		return(0);
	if( strcmp(&s[ls-lt],t) == 0 )
		return(1);
	return(0);
}
