/*
	Copyright 1981
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

char *sccs "@(#)as68 - Nov 12, 1982";

/*
 * a two pass relocatable assembler for the Motorola 68000 microprocessor

 *  Bill Allen
 *	Modified by Vicki Hutchison

 *  after any of this assembler is recompiled, it must be initialized
 *  before it will execute properly.  To initialize, become super user
 *  and execute the command:

 *		as68 -i as68init

 *  where as68 is the newly compiled version of the assembler.  With-
 *  out this initialization, the assembler will not run (probably bus
 *  error).
 */

/*
 *  Revisions :
 *
 *		Added fix to make location relative equates
 *		correct even if branch optimization occurs.		vlh 15-sep-82
 *  	Recognizes commands in uppercase.				vlh 17-sep-82
 *		Added recognization of upper case directives
 *		which do not have the preceding '.'.			vlh 17-sep-82
 *		Added recognizing of ignored directives
 *		IDNT, ORG and MASK2.							vlh 17-sep-82
 *		Added new directives XDEF, XREF.				vlh 17-sep-82
 *		Added COMLINE and SECTION.						vlh 20-sep-82
 *		Added new directive REG.						vlh 21-sep-82
 *		Added new directives DCB and OFFSET.			vlh 22-sep-82
 *		Recognizes branch extensions and takes
 *		the appropriate action.							vlh 22-sep-82
 *		Added assembler directives ifeq, ifne,
 *		ifle, iflt, ifge, ifgt.							vlh 23-sep-82
 *		Added character '$' to set of legitimate
 *		characters in a symbol.							vlh 23-sep-82
 *		Added new ignored directive OPT.				vlh 23-sep-82
 *		Recognizes non-opcodes starting in column 1
 *		as labels.										vlh 23-sep-82
 *		Added conditional assembler directives
 *		ifc, ifnc.										vlh 27-sep-82
 *		Major reworking of expression handler			vlh  1-oct-82
 *		More revisions to expression stacks				vlh  4-oct-82
 *		Added the recognition and handling of			vlh 26-oct-82
 *		the 'illegal' instruction.
 *		Added handling of the jmp suffixes				vlh 26-oct-82
 *		Fixed problem with -p flag and external			vlh 27-oct-82
 *		variable listings.
 *		Added checking for legitimate command 			vlh 28-oct-82
 *		suffixes.
 *		Added comma as a legitimate register			vlh 28-oct-82
 *		divisor for the REG directive.
 *		Fixed second pass error/instr len problem		vlh 28-oct-82
 *		Added checking for invalid bit ranges.			vlh  1-nov-82
 *		Added proper 2nd pass handling of the			vlh  2-nov-82
 *		'*' symbol (avoid turning jsr to bra).
 *		Fixed first pass guess if operand is			vlh  4-nov-82
 *		(rx,rx) instead of d(rx,rx).
 *		Check for appropriate range of 16-bit			vlh  5-nov-82
 *		immediate date.
 *		Operand type check for shift/bit manip.			vlh  5-nov-82
 *		Turn and/or/eor #xxx,sr/ccr into the			vlh  5-nov-82
 *		corresponding immediate instruction.
 *		Generate an error when an illegal pc			vlh 10-nov-82
 *		relative ea is specified.
 */

/****************************************************************************/
/*									    */
/*			    D R I   H a c k s				    */
/*			    -----------------				    */
/*									    */
/*	The following changes were made to facilitate running as68 under    */
/*	VMS and CP/M:							    */
/*									    */
/*	o	Added "-f" flag to redirect temp files to another directory */
/*		or CP/M floppy drive.					    */
/*									    */
/*	o	Added "-s" flag to redirect symbol table file.		    */
/*									    */
/*	o	Added separate source file with initialization of all 	    */
/*		external variables to be compatible with Whitesmith's C.    */
/*									    */
/*	o	Added "VMS", "UNIX", and "CPM" conditional compilation 	    */
/*		switches for these operating systems.			    */
/*									    */
/*	o	Added various listing enhancements so the printed file is   */
/*		usable on non-UNIX environments.			    */
/*									    */
/*	o	Changed I/O calls to be useable with filter library on V7   */
/*									    */
/****************************************************************************/



# include "as68.h"
char  tfilname[80];			/* Temp filename area 		*/
char  initfnam[80];			/* Initialization filename area */
#ifdef	UNIX
char  tfilbase[] {"a6AXXXXXX"};
#else
char  tfilbase[] {"a6xxxxA"};		/* Temp file base name		*/
#endif
char  initbase[] {"as68symb.dat"};	/* Init file base name		*/
#ifdef	VMS				/* On VMS, use			*/
char *tdname = "";			/* Temps in same directory	*/
char *idname = "bin:";			/* Init  in the "bin:" directory*/
#endif					/********************************/
#ifdef	UNIX				/*				*/
char *tdname = "/tmp/";			/* UNIX Temp directory		*/
char *idname = "/lib/";			/* UNIX Init Directory		*/
#endif					/********************************/
#ifdef	CPM				/* On CP/M, defaults for all.	*/
char *tdname = "";			/* No temp prefix		*/
char *idname = "";			/* No init prefix		*/
#endif					/********************************/

char ldfn[40];			/*name of the relocatable object file*/

int brkln1 077777;		/*pass 1 break line number for debugging*/
int opcval;				/*opcode*/
int chmvq;

int hopd(), hend(), send(), horg(), sorg(), hequ(), hreg();
int hds(), sds();
int sdcb();
int hdsect(), hpsect(), sdsect(), spsect();
int hsection(), ssection(), hoffset();
int hent(), hext();
int igrst();
int hbss(), sbss();
int heven(), seven();
int hdc(), sdc(), hdcb();
int hmask2(), hcomline(), hidnt();
int hifeq(), hifne(), hiflt(), hifle(), hifgt(), hifge(), hendc();
int hifnc(), hifc(), hopt();
int hpage(),spage();
int (*p1direct[])() {
	hopd,		/*0*/
	hend,		/*1*/
	hdsect,		/*2*/
	hpsect,		/*3*/
	hequ,		/*4*/
	hequ,		/*5 .set same as .equ*/
	0,			/*6*/
	0,			/*7*/
	hdc,		/*8*/
	hent,		/*9*/
	hext,		/*10*/
	hbss,		/*11*/
	hds,		/*12*/
	heven,		/*13*/
	horg,		/*14*/
	hmask2,		/*15*/
	hreg,		/*16*/
	hdcb,		/*17*/
	hcomline,	/*18*/
	hidnt,		/*19*/
	hoffset,	/*20*/
	hsection,	/*21*/
	hifeq,		/*22*/
	hifne,		/*23*/
	hiflt,		/*24*/
	hifle,		/*25*/
	hifgt,		/*26*/
	hifge,		/*27*/
	hendc,		/*28*/
	hifc,		/*29*/
	hifnc,		/*30*/
	hopt,		/*31*/
	hpage,		/*32*/
	0};

int (*p2direct[])() {
	0,			/*0*/
	send,		/*1*/
	sdsect,		/*2*/
	spsect,		/*3*/
	0,			/*4*/
	0,			/*5*/
	0,			/*6*/
	0,			/*7*/
	sdc,		/*8*/
	0,			/*9*/
	0,			/*10*/
	sbss,		/*11*/
	sds,		/*12*/
	seven,		/*13*/
	sorg,		/*14*/
	0,			/*15*/
	0,			/*16*/
	sdcb,		/*17*/
	sds,		/*18 comline same as .ds.b*/
	0,			/*19*/
	0,			/*20*/
	ssection,	/*21*/
	0,			/*22*/
	0,			/*23*/
	0,			/*24*/
	0,			/*25*/
	0,			/*26*/
	0,			/*27*/
	0,			/*28*/
	0,			/*29*/
	0,			/*30*/
	0,			/*31*/
	spage,			/*32*/
	0};

char *ermsg[] {
	"label redefined",					/*1*/
	"invalid label",					/*2*/
	"invalid opcode",					/*3*/
	"no label for operand",					/*4*/
	"opcode redefined",					/*5*/
	"illegal expr",						/*6*/
	"undefined symbol in equate",				/*7*/
	0,									/*8*/
	"invalid first operand",			/*9*/
	"invalid second operand",			/*10*/
	"absolute value required",			/*11*/
	"no code or data allowed in offset",	/*12*/
	"undefined symbol",					/*13*/
	"illegal index register",			/*14*/
	"illegal constant",					/*15*/
	"illegal extension",				/*16*/	/*[vlh]*/
	"constant required",				/*17*/
	"illegal format",					/*18*/
	"illegal string",					/*19*/
	"illegal addressing mode",			/*20*/
	0,									/*21*/
	"illegal relative address",			/*22*/
	"invalid bit range",				/*23*/
	"illegal text delimiter",			/*24*/
	"unexpected endc",					/*25*/
	"endc expected",					/*26*/
	"relocation error",					/*27*/
	"symbol required",					/*28*/
	"bad use of symbol",				/*29*/
	"invalid data list",				/*30*/
	0,									/*31*/
	"missing )",						/*32*/
	"register required",				/*33*/
	"illegal size",						/*34*/
	"illegal 8-bit displacement",		/*35*/
	"illegal external",					/*36*/
	"illegal shift count",				/*37*/
	"invalid instruction length",		/*38*/
	"code or data not allowed in bss",	/*39*/
	"backward assignment to *",			/*40*/
	"illegal 16-bit displacement",		/*41*/
	"illegal 16-bit immediate",			/*42*/
	"illegal 8-bit immediate",			/*43*/
	0
};

int endit();
int rubout();
int symcon;
char endstr[] "end";
char equstr[] "equ";
char evnstr[] "even";
char orgstr[] "~.yxzorg";

int poslab;
char tlab1[NAMELEN];
int explmode;	/*explicit mode length given*/

main(argc,argv)
char **argv;
{
	register i, ttmp;

	register xargc;		/* Temp copy of argc		*/
	xargc = argc;

	initflg = 0;
	prtchidx = prtchars;
#ifdef	UNIX			/* Only available with UNIX	*/
	signal(2,rubout);
#endif
	pitix = itbuf;
	pexti = extbl;
	ttmp = (STESIZE*SZMT) + 2;
	bmte = sbrk(ttmp);
	if(((long)bmte) == -1L)
	{
		rpterr("Symbol Table Overflow\n");
		abort();
	}
	if((long)bmte&1)
		bmte++;		/*make it even*/
	emte = bmte + ttmp - 2;		/*end of main table*/
	if(argc<=1) usage();
	i = 1;
	shortadr = -1;	/*short addresses ok*/
	while(argv[i][0] == '-') {		/*may be print or initialize*/
		switch(argv[i++][1]) {

		case 'i':		/*initialize the assembler*/
		case 'I':
			initflg++;
			break;

		case 'p':		/*produce a listing*/
		case 'P':
			prtflg++;
			break;

		case 'u':		/*make undefined symbols external*/
		case 'U':
			undflg++;
			break;

		case 'N':		/*no branch optimization*/
		case 'n':
			didorg++;
			break;

		case 'L':		/*long addresses only*/
		case 'l':
			shortadr = 0;
			break;

		case 'f':		/* Change temp files	*/
		case 'F':
			tdname = argv[i++];
			break;

		case 's':		/* Change symbol table name */
		case 'S':
			idname = argv[i++];
			break;

		default:
			usage();
		}
	}
	strcat (tfilname,tdname);	/* Build temp filenames */
	strcat (tfilname,tfilbase);	/*			*/
					/**************************/
#ifdef	UNIX				/* For UNIX, Need to make */
	mktemp (tfilname);		/* Unique file name	  */
#endif					/**************************/
	tfilptr = &tfilname[strlen(tfilname)-1];	/* -> changed char */
#ifdef	UNIX
	tfilptr -= 6;			/* Back off PID		*/
#endif
	
	strcat (initfnam,idname);	/* Build Symbol table filename	    */
	strcat (initfnam,initbase);

	if(i>=argc) usage();
	ifn=open(argv[i],0,0);	/*open source file*/
	sfname = argv[i];	/* remember source filename */
	setldfn(argv[i]);	/*create relocatable object file name*/
	lfn=openfi(ldfn,1);	/*open loader file*/
	itfn = gettempf();	/*get a temp file for it*/
	itfnc = LASTCHTFN;	/*remember last char for unlink*/
	trbfn = gettempf();	/*temp for text relocation bits*/
	trbfnc = LASTCHTFN;
	dafn = gettempf();	/*temp for data binary*/
	dafnc = LASTCHTFN;
	drbfn = gettempf();	/*temp for data relocation bits*/
	drbfnc = LASTCHTFN;
	if(initflg) {		/*initializing te main table*/
		lmte=bmte;		/*beginning main table*/
		cszmt = SZMT;	/*current size of main table*/
		for(i=0; i<=SZIRT-2; i=+2) {
			sirt[i] = &sirt[i];		/*initialize the initial ref tables*/
			sirt[i+1] = 0;
			oirt[i] = &oirt[i];
			oirt[i+1] = 0;
		}

/*make entries in main table for directives*/
		mdemt("opd",0);			/*opcode definition*/
		mdemt(endstr,1);		/*end statement*/
		mdemt("data",2);		/*dsect directive(code DATA based)*/
		mdemt("text",3);		/*psect directive(code TEXT based)*/
		mdemt(equstr,4);		/*equate*/
		mdemt("set",5);			/*.set - same as .equ*/
		mdemt("dc",8);			/*define byte*/
		mdemt("globl",9);		/*define global (public) symbols*/
		mdemt("xdef",9);		/*[vl]define global (public symbols*/
		mdemt("xref",9);		/*[vl]define global (public) symbols*/
		mdemt("comm",10);		/*define external symbols*/
		mdemt("bss",11);		/*block storage based*/
		mdemt("ds",12);			/*block storage based*/
		mdemt(evnstr,13);		/*round pc*/
		mdemt(orgstr,14);		/*internal org for *=n*/
		mdemt("org",14);		/*[vlh]*/
		mdemt("mask2",15);		/*[vl] assemble for mask2, ignore*/
		mdemt("reg",16);		/*[vlh] register equate*/
		mdemt("dcb",17);		/*[vlh] define block*/
		mdemt("comline",18);	/*[vlh] command line*/
		mdemt("idnt",19);		/*[vlh] relocateable id record, ignore*/
		mdemt("offset",20);		/*[vlh] define offsets*/
		mdemt("section",21);	/*[vlh] define sections*/
		mdemt("ifeq",22);		/*[vlh] ca if expr = 0*/
		mdemt("ifne",23);		/*[vlh] ca if expr != 0*/
		mdemt("iflt",24);		/*[vlh] ca if expr < 0*/
		mdemt("ifle",25);		/*[vlh] ca if expr <= 0*/
		mdemt("ifgt",26);		/*[vlh] ca if expr > 0*/
		mdemt("ifge",27);		/*[vlh] ca if expr >= 0*/
		mdemt("endc",28);		/*[vlh] end ca*/
		mdemt("ifc",29);		/*[vlh] ca if string compare*/
		mdemt("ifnc",30);		/*[vlh] ca if not string compare*/
		mdemt("opt",31);		/*[vlh] ignored, assemb options*/
		mdemt("ttl",31);		/* Ignore title op also */
		mdemt("page",32);

	}
	else {	/*not initializing*/
		getsymtab();		/*read initialized main table*/
	}

	rlflg = TEXT;			/*code initially TEXT based*/
	inoffset = 0;			/*[vlh]not in offset mode*/
	loctr = 0;				/*no generated code*/
	ca = 0;					/*[vlh]depth of conditional assembly*/
	extindx = 0;			/*no external symbols yet*/
	p2flg = 0;				/*pass 1*/
	ca_true = 1;			/*[vlh]true unless in side false case*/
	absln = 1;
	sbuflen = -1;			/*no source yet*/
	fchr = gchr();			/*get first char*/
	if(!initflg) {		/*not initializing*/
		pack(orgstr,lmte);
		orgptr = lemt(oirt,TRUE);
		pack(endstr,lmte);
		endptr = lemt(oirt,TRUE);
		pack(equstr,lmte);
		equptr = lemt(oirt,TRUE);
		pack("add",lmte);
		addptr = lemt(oirt,TRUE);
		pack("addi",lmte);
		addiptr = lemt(oirt,TRUE);
		pack("addq",lmte);
		addqptr = lemt(oirt,TRUE);
		pack("sub",lmte);
		subptr = lemt(oirt,TRUE);
		pack("subi",lmte);
		subiptr = lemt(oirt,TRUE);
		pack("subq",lmte);
		subqptr = lemt(oirt,TRUE);
		pack("cmp",lmte);
		cmpptr = lemt(oirt,TRUE);
		pack("adda",lmte);
		addaptr = lemt(oirt,TRUE);
		pack("cmpa",lmte);
		cmpaptr = lemt(oirt,TRUE);
		pack("suba",lmte);
		subaptr = lemt(oirt,TRUE);
		pack("cmpm",lmte);
		cmpmptr = lemt(oirt,TRUE);
		pack("and",lmte);
		andptr = lemt(oirt,TRUE);
		pack("andi",lmte);
		andiptr = lemt(oirt,TRUE);
		pack("or",lmte);
		orptr = lemt(oirt,TRUE);
		pack("ori",lmte);
		oriptr = lemt(oirt,TRUE);
		pack("cmpi",lmte);
		cmpiptr = lemt(oirt,TRUE);
		pack("eor",lmte);
		eorptr = lemt(oirt,TRUE);
		pack("eori",lmte);
		eoriptr = lemt(oirt,TRUE);
		pack("move",lmte);
		moveptr = lemt(oirt,TRUE);
		pack("moveq",lmte);
		moveqptr = lemt(oirt,TRUE);
		pack("exg",lmte);
		exgptr = lemt(oirt,TRUE);
		pack("jsr",lmte);
		jsrptr = lemt(oirt,TRUE);
		pack("bsr",lmte);
		bsrptr = lemt(oirt,TRUE);
		pack("nop",lmte);
		nopptr = lemt(oirt,TRUE);
		pack(evnstr,lmte);
		evenptr = lemt(oirt,TRUE);
	}
	mloop();
}

usage()
{
     rpterr("Usage: as68 [-p] [-u] [-l] [-n] [-f d:] [-s d:] sourcefile\n");
     endit();
}


/*main loop*/
mloop()
{
	register i;

	while(fchr!=EOF) {
		if(absln>=brkln1)	/*break for debugging the assembler*/
			i=0;
		fcflg = 0;			/*first time thru expr pass one*/
		cisit();			/*create it for one statement*/
	}
	opcpt = endptr;
	hend();
}

#define NOCODE ((i>=0&&i<6)||i==9||i==11||i==16||(i>=LOW_CA&&i<=HI_CA))

/*create intermediate text (it) for one statement*/
/*	call with first character of statement in fchr*/
cisit()
{

	register int *p1,*p2;
	register int (*dirop)();
	register int i, col1;	/*[vlh] col1 labels in col 1...*/
	char str[NAMELEN], *l;

ciss1:
	immed[0] = immed[1] = indir[0] = indir[1] = numcon[0] = 0;
	numcon[1] = numsym[0] = numsym[1] = numreg[0] = numreg[1]=0;
	plevel = numops = opdix = explmode = 0;
cistop:
	col1 = 1;
	if(fchr==EOLC) {
		fchr = gchr();
		goto cistop;
	}
	if(fchr==' ') {
		col1 = 0;
		igblk();
		if(fchr==EOLC)		/*blank line*/
			goto cistop;
		peekc = fchr;
		if (fchr != EOF) fchr = ' ';	/* [vlh] catch eof... */
	}
	if(fchr==EOF) return;

	if(fchr=='*') {			/*ignore comments*/
		fchr = gchr();
		if(fchr=='=') {		/*relocation counter assignment*/
			fchr = gchr();	/*pass the =*/
			horg();			/*output constants if not bss*/
		}
		igrst();
		fcflg = 0;		/*clear expr first time flag for next stmt*/
		goto ciss1;
	}

/* get the opcode and label*/

	mode = 'w';			/*word mode*/
	igblk();			/*ignore blanks*/
	poslab = 1;
	gterm(TRUE);
	poslab = 0;
	if(fchr==':' || fchr=='=') {			/*there is a label*/
label:
		col1 = 0;
		if(itype!=ITSY) {			/*not a symbol*/
			uerr(2);
			lbt[0] = 0;		/*no label*/
		}
		else {
			p2 = &lmte->name[0];
			for(p1= &lbt[0]; p1 < &lbt[NAMELEN]; ) {
				*p1++ = *p2++;
			}
			if(fchr==':') fchr=gchr();		/*ignore the colons*/
		}
labl1:
		ligblk();
		if(fchr == EOF) return;
		if(fchr == '*') {
			igrst();		/*comment*/
			goto labl1;
		}
		gterm(TRUE);
		if(fchr==':' || fchr=='=') {	/*another label*/
			if(lbt[0]) {
				savelab();	/*save current label*/
				dlabl();	/*define the last one*/
				pack(tlab1,lmte);	/*restor the old lable*/
			}
			goto label;
		}
	}
	else {
		lbt[0] = 0;			/*no label*/
	}
	igblk();
	if(fchr == '=')
		goto label;
	if(itype==ITSP) {
		if(ival.wd2 == '=') {
			hequ();
			return;
		}
	}
	if(itype!=ITSY) {	/*not valid opcode*/
		goto cisi3;
	}
	if (col1) {	/* [vlh] could be a label save as is... */
		l = &str;
		strcpy(l,lmte->name,NAMELEN);
	}
	if((opcpt=lemt(oirt,TRUE))==lmte) {	/*not in opcode table*/
		if (col1) {	/* [vlh] it's a label... */
			strcpy(lmte->name,l,NAMELEN);
			goto label;
		}
cisi3:
		if (ca_true)	/* [vlh] report error if not in CA false */
			xerr(3);
		igrst();
		return;
	}
	getmode();		/*look for .b .w or .l mode flag*/
	if(opcpt->flags&OPDR) {	/* its a directive*/
		i = opcpt->vl1;
		if (!ca_true && (i < LOW_CA || i > HI_CA)) { igrst(); return; }
		if (inoffset) 	/* [vlh] */
			if (!(NOCODE)) {	/* can't generate code in offset */
				xerr(12);
				return;
			}
		dirop = p1direct[i];	/*call routine to handle directive*/
		(*dirop)();
		return;
	}
	else if (!ca_true) {		/* [vlh] */
		igrst();
		return;
	}
	else if (inoffset) {		/* [vlh] */
		xerr(12);
		return;
	}

	opcval = (opcpt->vl1);			/*opcode*/
	format = (opcpt->flags&OPFF);	/*format of this instr*/
	if (explmode)
		if (!modeok()) { xerr(16);  return; }
	dlabl();			/*define label*/
	opitb();			/*beginning of statement*/
	if(format)
		opito();		/*may have operands*/
	else
		igrst();		/*only comments*/
	format = (opcpt->flags&OPFF);	/* may have changed*/


/*end of statement*/

	i = calcilen();
	stbuf[1].itrl = i;		/*assumed instruction length*/

	stbuf[0].itrl = itwc;	/*number of it entries*/
	wostb();			/*write out statement buffer*/
	loctr =+ i;
}

getmode()
{
	if (fchr=='.') {
		fchr = gchr();
		switch (fchr) {
			case 'b':
			case 'B':
			case 's':
			case 'S':
				modelen = 1;
				mode = BYTE;
				break;
			case 'w':
			case 'W':
				modelen = 2;
				mode = WORD;
				break;
			case 'l':
			case 'L':
				modelen = 4;
				mode = LONG;
				break;
			default:
				peekc = fchr;
				fchr = '.';
				goto getm1;
		}
		explmode++;
		fchr = gchr();
		igblk();
		return;
	}
getm1:
	if(opcpt == exgptr) {	/*length is long*/
		modelen = 4;
		mode = LONG;
	}
	else {
		mode = WORD;		/*default is word*/
		modelen = 2;
	}
}

/* check to be sure specified mode is legal */
modeok()	/* [vlh] */
{
	switch(format) {
		case 0  :
		case 14 :
		case 18 :
			return(FALSE);
		case 13 :
		case 15 :
		case 20 :
		case 21 :
			return(modelen==1?FALSE:TRUE);
		case 4  :
		case 25 :
			return(modelen==1?TRUE:FALSE);
		case 7  :
		case 9  :
			return(modelen==2?FALSE:TRUE);
		case 5  :
		case 11 :
		case 28 :
			return(modelen==2?TRUE:FALSE);
		case 6  :
			return(modelen==4?FALSE:TRUE);
		case 12 :
		case 30 :
		case 22 :
		case 29 :
			return(modelen==4?TRUE:FALSE);
		default :
			return(TRUE);
	}
}

/* calculate the instruction length in bytes*/
calcilen()
{

	register i,j;
	register long l;
	register char *p;

	i = 2;		/*all instrs at least 2 bytes*/

	switch(format) {

	case 20:
		i =+ 2;	/*for reg mask*/
	case 1:		/*two ea's -- one of which may be a reg*/
	case 15:
	case 30:
	case 26:
	case 5:
	case 3:
	case 21:
		i =+ lenea(1);
	case 16:
	case 24:
	case 25:
	case 29:
		i =+ lenea(0);
		break;

	case 9:		/* [vlh] explicit jmp length... */
		if (!explmode)
			i =+ lenea(0);
		else
			return(mode==LONG?6:4);	/*[vlh] explicit jmp.? */
		break;

	case 7:
		i =+ (immed[0]) ? 2+lenea(1) : lenea(1);
		break;

	case 14:
	case 11:
	case 19:
		i =+ 2;		/*always 4 bytes*/
		break;

	case 6:		/*relative branches*/
		if(itwc == ITOP1+1) {
			if(stbuf[ITOP1].itty == ITCN)
				l = stbuf[ITOP1].itop;
			else if(stbuf[ITOP1].itty == ITSY) {
				p = stbuf[ITOP1].itop.ptrw2;
				if(p->flags&SYDF)
					l = p->vl1;	/*symbol value*/
				else
					goto loffst;
			}
			else {
				goto loffst;
			}
			l =- (loctr+2);
			if(l<=127 && l>=-128)	/*8 bit offset*/
				break;
		}
loffst:
		if (!explmode || modelen > 1) /*[vlh] recognize br extensions*/
			i =+ 2;		/*long offset for branches*/
		break;

	case 2:
		i =+ (mode==LONG?4:2) + lenea(1);
		break;

	case 23:
		if(immed[0])
			i =+ (mode==LONG?4:2);
	case 17:
	case 22:
		i =+ lenea(1);
		break;

	case 8:
		if(numops==1)		/*memory shift instruction*/
			i =+ shiftea(0);
		break;
	}

	return(i);
}

/* calc the length of an effective address*/
lenea(lidx)
{
	register i;

	if(immed[lidx])
		return(mode==LONG?4:2);
	return(shiftea(lidx));
}

shiftea(lidx)
{
	if(indir[lidx])
		return((numcon[lidx] || numsym[lidx]) ? 2 : 0);
	if(numsym[lidx] || numcon[lidx])
		return((!shortadr || numcon[lidx]==2) ? 4 : 2);
	return(0);
}

/*
 *define a label if there is one to define
 *	call with:
 *		label name in lbt if it exists
 *		else lbt[0] == 0
 */
dlabl()
{
	register i;

	if(lbt[0]) {	/*got a label*/
		pack(lbt,lmte);		/*put label in main table*/
		lblpt=lemt(sirt,FALSE);	/*look up label*/
		if(lblpt != lmte) {		/*symbol entered previously*/
			if(lbt[0] == '~') {	/*local symbol -- may be duplicate*/
				lblpt = lmte;
				mmte();
			}
			else {
				if(lblpt->flags&SYXR) {
					uerr(29);
					lblpt = 0;
					return;
				}
				if((lblpt->flags)&SYDF) {
					uerr(1);
					lblpt = 0;
					return;
				}
			}
		}
		else {
			mmte();		/*make label entry in main table*/
		}
		lblpt->flags =| SYDF;	/*label is now defined*/
		lblpt->flags =| (rlflg==DATA)?SYRA:(rlflg==BSS)?SYBS:SYRO;
		lblpt->vl1 = loctr;		/*label value*/
	}
	else
		lblpt = 0;
}

/*
 * output it for operands
 *		gets intput from gterm
 *		puts output in stbuf using itwc as an index
 *		itwc should point at the next entry to be made in stbuf
 */
opito()
{
	register lopcomma;

	lopcomma = symcon = chmvq = 0;
	numops++;				/*count first operand*/
	while(1) {
		starmul = symcon;	/*star is multiply op if flag is set*/
		if(fchr=='\'' || fchr=='"')
			lopcomma = 0;
		gterm(FALSE);	/*get a term*/
		if(itwc==ITOP1 && format==CLRFOR && opcval==CLRVAL)
			chgclr();
		opitoo();	/*output it for one operand*/
		if(itype==ITSP && ival.wd2==',') {
			if (plevel==1 && !numcon[opdix])	/* [vlh] */
				numcon[opdix] = 1;
			if(lopcomma)
				uerr(30);
			lopcomma++;
			igblk();	/*ignore blanks for 68000 C compiler*/
		}
		else
			lopcomma=0;
		if(ival==EOLC && itype==ITSP)	/*end of operands*/
			break;
		if(fchr==EOLC) {
			fchr=gchr();
			break;
		}
	}
	if(chmvq)		/*changed move to moveq*/
		if(numops!=2 || immed[1] || indir[1] || numcon[1] || numsym[1] ||
		  numreg[1]>=AREGLO) {
			stbuf[2].itop.ptrw2 = moveptr;	/*change it back*/
			opcpt = moveptr;
		}

	if (stbuf[2].itop.ptrw2==cmpptr) 	/* [vlh] cmp -> cmpm ?? */
		if (numreg[0] && numreg[1] && indir[0] && indir[1]) {
			stbuf[2].itop.ptrw2 = cmpmptr;
			opcpt = cmpmptr;
		}

	if(lopcomma)
		uerr(30);
}

/* change clr.l An to suba.l An,An*/
chgclr()
{
	register char *p;

	if(itype==ITSY) {	/*first op is symbol*/
		p = lemt(sirt,FALSE);
		if(p==lmte)
			return;
		if(!(p->flags&SYER) || p->vl1<AREGLO)		/*not A reg*/
			return;
		opcpt = subaptr;	/*make it a suba instr*/
		opitb();
		opitoo();		/*output first operand -- An*/
		itype = ITSP;
		ival = ',';
		opitoo();		/*output a comma*/
		itype = ITSY;	/*now the A reg again*/
	}
}

/*output it for one operand*/
opitoo()
{
	register i;
	register char *sp;

	symcon = 0;
	if(itype==ITSP) {				/*special symbol*/
		if(ival.wd2==',' && !plevel) {		/* another operand */
			numops++;
			if(!opdix)
				opdix++;
		}
		if(ival.wd2==' ') {		/*end of operands*/
			while(fchr!=EOLC)		/*ignore rest of statement*/
				fchr=gchr();
			return;
		}
		if(ival.wd2==EOLC)
			return;
	}
	else		/*symbol or constant*/
		symcon = 1;

	if(itwc >= STMAX) {			/*it overflow*/
		rpterr("i.t. overflow\n");
		abort();
	}
	pitw->itty = itype;		/*type of it entry*/

/*put symbol in it buffer*/
	if(itype==ITSY) {
		sp=lemt(sirt,FALSE);		/*look up it main table*/
		pitw->itop.ptrw2 = sp;	/*ptr to symbol entry*/
		if(sp==lmte)			/*first occurrance*/
			mmte();
		itwc++;				/*count entries in it buffer*/
		pitw++;
		if(!(sp->flags&SYER))	/*is it a register?*/
			numsym[opdix]++;
		else		/*yes, a register*/
			numreg[opdix] = sp->vl1;
		return;
	}
	else if(itype == ITCN ) {
		if(ival.wd1 && ival.wd1 != -1)
			numcon[opdix] = 2;
		else if(!numcon[opdix])
			numcon[opdix] = 1;
		if(numops == 1)
			tryquick();
	}

/* special characters and constants*/
	pitw->itop = ival;
	pitw->itrl = reloc;
	itwc++;
	pitw++;
}

/* change add into addq and sub into subq if possible*/
tryquick()
{
	register char *p;
	register long l;

	if(fchr!=',' || !immed[0])
		return;
	l = ival;
	if(itwc != ITOP1+1) {
		if(itwc!=ITOP1+2 || stbuf[ITOP1+1].itty!=ITSP ||
		  stbuf[ITOP1+1].itop.wd2 != '-')
			return;
		l = -l;
	}
	p = stbuf[2].itop.ptrw2;
	if(p==moveptr) {
		if(explmode && modelen != 4)	/*dont change .w or .b*/
			return;
		if(l>=-128 && l<=127) {
			stbuf[2].itop.ptrw2 = moveqptr;
			opcpt = moveqptr;
			chmvq++;
		}
		return;
	}
	if(l<=0 || l>8) {
		return;
	}
	if(p==addptr || p==addiptr) {
		stbuf[2].itop.ptrw2 = opcpt = addqptr;
	}
	else if(p==subptr || p==subiptr) {
		stbuf[2].itop.ptrw2 = opcpt = subqptr;
	}
}

strcpy(str1, str2, len)
register char *str1, *str2;
register int len;
{
	while (len--)
		*str1++ = *str2++;
}

/* index - find the index of a character in a string*/
/*		This is identical to Software Tools index.*/
index(str,chr)				/* returns index of c in str or -1*/
char *str;				/* pointer to string to search*/
char chr;				/* character to search for*/
{
	register char *s;
	register int i;

	for( s = str, i = 0; *s != '\0'; i++ )
		if( *s++ == chr )
			return(i);
	return(-1);
}

