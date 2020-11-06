/**
 *  Copyright 1983
 *  Alcyon Corporation
 *  8716 Production Ave.
 *  San Diego, Ca.  92121
**/

char *version = "@(#)main.c	1.12	12/28/83";

/* C68 -- Cross Compiler Main Driver */

#include "mdriver.h"

#ifndef DECC
#	include <signal.h>					/*[mac] 4.2a*/
#else
#	include "Isignal"
#endif
#ifdef WHITE
#	include <stdio.h>
#	include <klib.h>
#endif

int cexit();						/*[mac] 4.2a*/
int catchterm();					/*[mac] 4.2a*/
int child;							/*[mac] 4.2a*/

/**
 * main - main routine for c68 Compiler system
 *      Handles the C68 arguments.  For each C file given, the macro
 *      pre-processor is called, then the parser, code generator and
 *      assembler are fexec'd.  The loader arguments are collected and
 *      the loader is fexec'd.
**/
main(argc,argv)
int argc;
char **argv;
{
    register char *arg, **p, **cfilep, **loadp, *sp;
    register int nloadfiles, c, i, j, index;
    
    cfilep = &cfiles[0];
    loadp = &loadargs[0];
    nloadfiles = 0;
#ifdef MC68000
    sysinfo(0,&mmuflag);        /*[vlh] mmu system ??*/
#else
	mmuflag = 1;
#endif
    if (!mmuflag)               /*[vlh] default args*/
        *loadp = ldflg;

	if( signal(SIGINT,SIG_IGN) != SIG_IGN ) /*[mac] 4.2a signal processing*/
		signal(SIGINT,cexit);
	if( signal(SIGHUP,SIG_IGN) != SIG_IGN )
		signal(SIGHUP,cexit);
	if( signal(SIGQUIT,SIG_IGN) != SIG_IGN )
		signal(SIGQUIT,cexit);
	signal(SIGTERM,catchterm);

    for( p = argv, j = argc; --j > 0; ) {       /*process arguments*/
        if( *(arg= *++p) == '-' ) {
            arg++;
            for( i = 0; c = *arg++; i++ ) {
        
            switch( c ) {

            case 'c':
                cflag++;
                continue;

            case '1':
                oneflag++;
                parser = onepass;
                continue;

            case 'D':
                if (ndefs == NDEFS)
					ferror("To many command line defines");
				defs[ndefs++] = arg-2;  /* need -D... */
                i++;
                break;

            case 'I':
                if (ndefs == NDEFS)
					ferror("To many command line include directories");
                incl[nincl++] = arg-2;  /* need -I... */
                i++;
                break;

            case 'N':
                nflag++;
                continue;

            case 'e':   /* [vlh] 3.4 */
                ieeeflag++;
                if (ffpflag)
                    ferror("incompatible flags : '-f' and '-e'\n");
                continue;

            case 'f':   /* [vlh] 3.4 */
                ffpflag++;
                if (ieeeflag)
                    ferror("incompatible flags : '-f' and '-e'\n");
                continue;

            case 'a':	/* make all addresses 16 bits (short) */
                lflag = 0;
                continue;

            case 'L':	/* make all addresses 32 bits (long) */
                lflag = 1;
                continue;

            case 'o':
                if( --j <= 0 )
                    ferror("bad -o option\n");
                *loadp++ = *p++;
                if( strend(*p,".c") || strend(*p,".s") || strend(*p,".o") ) {
                    sp = makecopy(*p);
                    sp[strlen(sp)-2] = '\0';
                    *loadp++ = sp;
                    *loadp++ = setend(*p,'o');
                    nloadfiles++;
                    if( !strend(*p,".o") )
                        *cfilep++ = *p;
                }
                else
                    *loadp++ = *p;
                continue;

            case 'C':	/* [vlh] 4.2+, leave in comments */
				comment++;
			case 'E':   /* [vlh] 4.0 Preprocessor to stdout */
                eflag++;
            case 'P':
                pflag++;
                cflag++;
                continue;

            case 'S':
                sflag++;
                cflag++;
                nflag++;
                continue;

            case 't':
                if( (c= *arg++) == 'p' )        /* [vlh] 4.2 */
                    preproc = "/usr/c68/cpp68";
                else if( c == '0' )
                    parser = "/usr/c68/c068";
                else if( c == '1' )
                    cgen = "/usr/c68/c168";
                else if (c == '2')
                    optim = "/usr/c68/c268";
                else if (c == '3')
                    asmprog = "/usr/c68/as68";
                else if (c == '4')
                    loader = "/usr/c68/lo68";
                else if( c == '\0' )
                    tflag++;
                continue;

            case 'T':       /* [vlh] 4.2, 68010 target */
                tenflag++;
                continue;

            case 'g':       /* [vlh] 4.2 symbolic debugger label generation */
                gflag++;
                continue;
#ifndef NOPROFILE
            case 'p':       /* [vlh] 4.3, profiler flag */
                profile++;
                continue;
#endif
            case 'w':       /* [vlh] */
                wflag++;
                continue;

            case 'O':       /* [vlh] 3.4 */
                optimize++;
                continue;

            case 'v':       /* [vlh] 3.4 print file name */
                verbose++;
                continue;

            case '6':       /* [vlh] 3.4 v6 compatibility */
                v6flag++;
                continue;

            case '7':       /* [vlh] 3.4 v7 compatibility */
                v7flag++;
                continue;

            case '4':       /* [vlh] 4.3 ucb compatiblity */
                ucb++;
                continue;

            case '3':       /* [vlh] 3.4 s3 compatibility */
                s3flag++;
                continue;

            case '5':       /* [vlh] 3.4 s5 compatiblity */
                s5flag++;
                continue;

            default:
                if( loadp >= &loadargs[NARGS] )
                    ferror("too many loader args\n");
                *loadp++ = *p;
                i++;
                break;
            
            }       /* end of case statement */
            break;
            }   /* end of for statement */
            if( i )
                continue;
        }   /* end of if statement */
    
/*C or Assembler files to process*/
        if( strend(arg,".c") || strend(arg,".s") ) {

            if( cfilep >= &cfiles[NARGS] )
                ferror("too many files\n");
            *cfilep++ = arg;
            nloadfiles++;
            if( !chkdup(arg) ) {
                if( loadp >= &loadargs[NARGS] )
                    ferror("too many loader args\n");
                *loadp++ = setend(arg,'o');
            }
        }
        else if( !chkdup(arg) ) {   /*check for loader args*/
            if( loadp >= &loadargs[NARGS] )
                ferror("too many loader args\n");
            *loadp++ = arg;
            if( strend(arg,".o") )
                nloadfiles++;
        }
    }   /* end of for statement which processes arguments */

    if( cfilep != &cfiles[0] ) {                /*had a C file?*/
        for( i = 0; i < NTEMPS; i++ )           /*allocate temps*/
            fns[i] = maketemp(i);
        for( p = &cfiles[0]; arg = *p++; ) {    /*handle each C file*/
            for( i = 0; i < NTEMPS; i++ )
				tfns[i] = fns[i];
            if( cfilep != &cfiles[1] || verbose )
                printf("%s:\n",arg);

/* call the Preprocessor */
            if(verbose)
				printf("\tPreprocessor\n");
            asflag = strend(arg,".s");
			if (pflag||asflag) 
				tfns[MTEMP] = setend(arg,'i');
            INITFARGS();
            addfarg(preproc);
			if(comment)
				addfarg("-C");
			if(eflag)
                addfarg("-E");
			else if (pflag)
				addfarg("-P");
			if ( v6flag )		/* v6 compatiblity */
				addfarg("-6");
			else if ( v7flag )	/* v7 compatiblity */
				addfarg("-7");
			else if ( s3flag )	/* sys3 compatiblity */
				addfarg("-3");
			else if ( ucb )	/* ucb compatiblity */
				addfarg("-4");
			else if ( s5flag )	/* sys5 compatiblity */
				addfarg("-5");
            if (nincl > 0)
                for (index = 0; index != nincl; index++)
                    addfarg(incl[index]);
            if (ndefs > 0)
                for (index = 0; index != ndefs; index++)
                    addfarg(defs[index]);
            addfarg(arg);	/* source file name */
            addfarg(tfns[MTEMP]);
			ENDFARGS();
            if( fexec(preproc,fargs) ) {
                status++;       /* preprocessor failure */
                cflag++;
                continue;
            }
			if (pflag)	{ /* no further processing on this file... */
				tfns[MTEMP] = "";
				continue;
			}

/* call the Parser */
            if( !asflag ) {
                tfns[ASTEMP] = setend(arg,'s');
                INITFARGS();
                addfarg(parser);
                addfarg(tfns[MTEMP]);
                if( oneflag ) {
                    addfarg(tfns[ASTEMP]);
                    if( lflag )
                        addfarg("-L");	/* default for 4.2 and beyond */
					else
                        addfarg("-a");	/* [vlh] 4.2 16 bit addresses */
                    if( sflag || nflag )
                        addfarg("-D");
                    addfarg("-1");
                }
                else {
                    addfarg(tfns[ICODE]);
                    addfarg(tfns[LINK]);
                    if ( ffpflag )    	/* [vlh] 3.4 */
                        addfarg("-f");
                    else if ( ieeeflag )   /* [vlh] 3.4 */
                        addfarg("-e");
#ifndef NOPROFILE
                    if( profile )
                        addfarg("-p");
#endif
                    if ( wflag )
                        addfarg("-w");
                    if( tflag )
                        addfarg("-t");	/* strings --> text segment */
                    if( gflag )
                        addfarg("-g");	/* cdb line numbers */
                }
                ENDFARGS();
                if ( verbose )
                    printf("\tParser\n");
                if( fexec(parser,fargs) ) {
                    status++;
                    cflag++;
                    continue;
                }
				unlink(tfns[MTEMP]);

        
/* Call the Code Generator */
                if( !oneflag ) {
                    INITFARGS();
                    addfarg(cgen);
                    addfarg(tfns[ICODE]);
                    addfarg(tfns[LINK]);
                    if (!optimize)
						addfarg(tfns[ASTEMP]);
					else
						addfarg(tfns[OTEMP]);
                    if( !sflag )
                        fns[ASTEMP] = tfns[ASTEMP];
                    if( lflag )
                        addfarg("-L");	/* default for 4.2 and beyond */
					else
                        addfarg("-a");	/* [vlh] 4.2 16 bit addresses */
					if (tenflag)
						addfarg("-T");	/* [vlh] 4.2 68010 target... */
                    if( nflag || sflag )
                        addfarg("-D");
                    if( gflag )
                        addfarg("-g");	/* cdb line numbers */
                    ENDFARGS();
                    if ( verbose )
                        printf("\tCode generator\n");
                    if( fexec(cgen,fargs) ) {
                        status++;
                        cflag++;
                        continue;
                    }
                }
				if(optimize) {
                    INITFARGS();
					addfarg(optim);
					addfarg(tfns[OTEMP]);
					addfarg(tfns[ASTEMP]);
                    ENDFARGS();
                    if ( verbose )
                        printf("\tOptimizer\n");
                    if( fexec(optim,fargs) ) {
                        status++;
                        cflag++;
                        continue;
                    }
				}
                if( sflag ) {
					tfns[ASTEMP] = tfns[MTEMP];	/* don't rm this file.... */
                    continue;
				}
            }   /* if not processing an assembler file */
            else
                tfns[ASTEMP] = tfns[MTEMP];

/* Call the assembler */
            INITFARGS();
            addfarg(asmprog);
            if( !asflag )
                addfarg("-u");
			if( lflag )
				addfarg("-L");	/* default for 4.2 and beyond */
			else
				addfarg("-a");	/* [vlh] 4.2 16 bit addresses */
			if (tenflag)			/* [vlh] 4.2 68010 target */
				addfarg("-T");
            addfarg(tfns[ASTEMP]);
            ENDFARGS();
			if ( verbose )
				printf("\tAssembler\n");
            if( fexec(asmprog,fargs) ) {
                cflag++;
                status++;
            }
			unlink(tfns[ASTEMP]);
        }
    }   /* handle input source files */

    /**
     * set things up for the loader, this means that we need to add the
     * C preface at the beginning of the program which has the jsr to
     * main and then exits after return from main.
    **/

    if( !cflag && (loadp != &loadargs[0] || cfilep != &cfiles[0] )) {
		if (verbose)
			printf("\tLoader\n");
        INITFARGS();
        addfarg(loader);
        addfarg("-X");
        i = 1;
        for( p = loadargs; *p ; p++ ) { /* insert pref before 1st .o */
            if( i && strend(*p,".o") ) {        
                i = 0;
#ifndef NOPROFILE
				if( !profile )
					addfarg(pref);
				else		
					addfarg(ppref);
#else
				addfarg(pref);
#endif
            }
            addfarg(*p);
        }
        if (ffpflag)      /* [vlh] 3.4 */
            addfarg(deflibfp);
        if (ieeeflag)      /* [vlh] 3.4 */
            addfarg(deflibep);
        if (v6flag)     /* [vlh] 3.4 */
            addfarg(v6lib);
        if (v7flag)     /* [vlh] 3.4 */
            addfarg(v7lib);
        if (s3flag)     /* [vlh] 3.4 */
            addfarg(s3lib);
        if (ucb)     	/* [vlh] 4.3 */
            addfarg(ucblib);
        if (s5flag)     /* [vlh] 3.4 */
            addfarg(s5lib);
        addfarg(deflib);
        if (gflag)      /* [vlh] 4.2 */
            addfarg(post);
        ENDFARGS();
        status |= fexec(loader,fargs);
    /**
     * if we were given one C file and there is one ".o" file, we remove
     * the ".o" file.
    **/
        if( cfilep == &cfiles[1] && nloadfiles == 1 )
            unlink(setend(cfiles[0],'o'));
    }
    cexit();
}

/**
 * cexit - exit from C compiler driver
 *      This deletes any existing temps and exits with the error status.
**/
cexit()                                 /* returns - none*/
{
	register int i;
	int mfail;

	mfail = 0;
	signal(SIGINT,SIG_IGN);				/* [vlh] 4.2 */
	if( child > 0 )
		wait(&mfail);
	for(i = 0; i < NTEMPS; i++)
		if (fns[i])
			unlink(tfns[i]);
    
	exit(status|mfail);
}

/**
 * catchterm - catch terminate signal and pass on to any outstanding children
*/
catchterm()
{
	if( child > 0 )
		kill(child,SIGTERM);
	cexit();
}

/**
 * fexec - fork and exec
 *      This forks a new task, then does an execv to execute the given
 *      program with the given arguements.
**/
fexec(fname,args)                   /* returns 1 if error, 0 otherwise*/
char *fname;                        /* file to execute*/
char **args;                        /* arguments to pass*/
{
    register int i;
    int fstat;

	child = maketask(fname,0,0,args); /*do fork & exec*/
    if( child < 0 ) {
#ifdef PDP11
		printf("Couldn't maketask ");
		perror(fname);
#else
		printf("Couldn't maketask %s, errno %d\n",fname,errno);
#endif
        return(1);
    }
    while( child != wait(&fstat) )        /*wait for child*/
        ;
	child = 0;
    if( (i=fstat&0377) != 0 && i != 14 ) {
        status = i;
        if( status != 2 ) {	/* DEL key (interrupt) */
#ifdef MC68000
            if (i > 0 && i < 32)
#else
            if (i > 0 && i < 16)
#endif
                printf("%s error terminated, signal %d\n",fname,i);
            else if (i == 0x8b)
                printf("%s error terminated, segmentation fault\n",fname);
            else if (i != 0x8a)
                printf("%s error terminated, status $%x\n",fname,i);
			else
                printf("%s error terminated, bus error\n",fname);
        }
        cexit();
    }
#ifndef DECC
    return((fstat>>8)&0xff);
#else
    return(fstat);
#else
#endif
}

/**
 * setend - set the end character of a string
 *      This grabs a copy of the string and sets the last character to
 *      the given character.  This is used to generate ".o", ".i" and
 *      ".s" file names.
**/
char *
setend(s,c)                       /* returns pointer to string*/
char *s;                                /* pointer to old string*/
int c;                                  /* character to end string with*/
{
    register char *p;

    p = makecopy(s);
    p[strlen(p)-1] = c;
    return(p);
}

/*
 * chkdup - checks for duplicate ".o" files in file list
 *      Goes thru the loader argument list checking for the given
 *      ".o" file name.
**/
chkdup(s)                           /* returns 1 if found, 0 otherwise*/
char *s;                            /* pointer to argument*/
{
    register char **l;

    if( strend(s,".o") ) {
    for( l = &loadargs[0]; *l; )
        if( !strcmp(*l++,s) )
        return(1);
    }
    return(0);
}

/**
 * makecopy - makes a copy of a string
 *      This allows for manipulating the file name, while allowing the
 *      saving of the old file name.
**/
char *makecopy(s)                       /* returns pointer to string*/
char *s;                                /* string to copy*/
{
    register char *p;
    register int ndx;

#ifndef VMS
    if (strncmp("/tmp/",s,5) != 0) /* don't truncate tmp file names */
    while ((ndx = index(s,FILESEP)) != -1) 
#else
    if (strncmp("wk:",s,5) != 0)   /* don't truncate tmp file names */
    while ((ndx = index(s,':')) != -1) 
#endif
        s += ndx+1;  /* [vlh] */
    for( p = argp; *argp++ = *s++; )
    ;
    return(p);
}

/**
 * addfarg - add fexec argument
 *      This takes the given arguement and adds it to the argment block
**/
addfarg(s)
char *s;
{
    if( fargp >= &fargs[NARGS] )
        ferror("too many arguments\n");
    *fargp++ = s;
}

/**
 * ferror - fatal error
 *      Outputs error message and exits with error status.
**/
ferror(s)             					/* returns - none*/
char *s;                                /* printf string*/
{
    printf(s);
    status++;
    cexit();
}

/**
 * maketemp - make a temporary file name
 *      Generates unique file name with process id
**/
char *maketemp(arb)                     /* returns file name*/
int arb;                                /* arbitrary number*/
{
    char *p, tmp[6];
#ifdef VMS
	/**
	 *	temporary directory on vax -
	 *	file names on vax can be at most {9 chars}.{3 chars},
	 *	but, process id's can be longer than 9 characters.
	**/
	char tmp1[12];						/* string to hold whole process id */

	p = makecopy("[gil]ct6");
	argp--;
	itoa(getpid(),tmp1,1);
	strncpy(tmp,tmp1,6);				/* create a 9 char primary file name */	
	tmp[5] = '\0';
#else
    p = makecopy("/tmp/ct6");
    argp--;
    itoa(getpid(),tmp,1);
#endif
    makecopy(tmp);
    argp--;
    makecopy(".");
    argp--;
    itoa(arb,tmp,1);
    makecopy(tmp);
    return(p);
}

/**
 * strcmp - string comparison
 *      Compares two strings for equality, less or greater.
**/
strcmp(s,t)                             /* returns 0 for equality*/
char *s, *t;
{
    for( ; *s == *t; s++, t++ )
    if( *s == '\0' )
        return(0);
    return( *s - *t );
}

/**
 * strncmp - string comparison
 *      Compares two strings for equality, less or greater.
**/
strncmp(s,t,n)                          /* neg for < and pos for >.*/
char *s;                                /* first string*/
char *t;                                /* second string*/
{
    for( ; *s == *t && n; s++, t++, n--)
    if( *s == '\0' )
        return(0);
    return((n) ?  *s - *t : 0);
}

/**
 * strlen - string length
 *      Computes number of bytes in string.
**/
strlen(s)                               /* returns string length*/
char *s;                                /* string to compute length*/
{
    register int n;

    for( n = 0; *s++ != '\0'; )
    n++;
    return(n);
}

/**
 * itoa - integer to ASCII conversion
 *      Converts integer to ASCII string, handles '-'.
**/
itoa(n,s,w)                             /* returns - none*/
int n;                                  /* number to convert*/
char *s;                                /* resulting string*/
int w;                                  /* minimum width of string*/
{
    register char *tp;
    register int sign, i;
    char temp[6];

    if( (sign=n) < 0 )
        n = -n;
    i = 0;
    tp = &temp[0];
    do {
        i++;
        *tp++ = n % 10 + '0';
    } while( (n /= 10) > 0 );
    if( sign < 0 ) {
        i++;
        *tp++ = '-';
    }
    while( --w >= i )                   /*pad on left with blanks*/
        *s++ = ' ';
    while( --i >= 0 )                   /*move chars reversed*/
        *s++ = *--tp;
    *s = '\0';
}

/**
 * strend - set string end
 *      This is used to compare the endings of file names for ".c", etc.
**/
strend(s,t)                         /* returns 1 if match, 0 otherwise*/
char *s;                            /* string to compare*/
char *t;                            /* string ending*/
{
    int ls, lt;

    if( (ls=strlen(s)) < (lt=strlen(t)) )
        return(0);
    return((strcmp(&s[ls-lt],t) == 0));
}

/**
 * index - find the index of a character in a string
 *      This is identical to Software Tools index.
**/
index(str,chr)                      /* returns index of c in str or -1*/
char *str;                          /* pointer to string to search*/
char chr;                           /* character to search for*/
{
    register char *s;
    register int i;

    for( s = str, i = 0; *s != '\0'; i++ )
        if( *s++ == chr )
            return(i);
    return(-1);
}

