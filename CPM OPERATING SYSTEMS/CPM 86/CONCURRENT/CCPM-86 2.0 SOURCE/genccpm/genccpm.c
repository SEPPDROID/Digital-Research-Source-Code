/**************************************************************************
 *	G E N C C P M -  generates an operating system
 *
 *	Written by Bill Fitler Nov. 1982
 *
 * GENCCPM is designed to edit the system image for the operating system
 * and construct an initialized, loadable image for execution.
 * GENCCPM was originally designed for Concurrent CP/M-86 v2.0, and was
 * modelled after the old GENSYS.
 *
 * Revision history:
 *      23 Feb 83 whf: add patch module
 *	25 Jan 83 whf: combine all modules for easier maintenance
 *	20 Jan 83 whf: handles SYSDAT overflow
 *	19 Nov 82 whf: added BDOS buffering generation.
 *
 ************************************************************************/


/*******************************/
/* for serialization purposes: */
char *copynote = "COPYRIGHT (C) 1983, DIGITAL RESEARCH 654321 $";
/****************************************************/
/* up front for patching: Note the internal EOS (0) */
char *clearx = "\033H\033E\0\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
/****************************************************/

/****** include files ********/
#define MAINMODULE
#define CCPM
#include <genccpm.h>
/*****************************/

#ifdef CCPM
BYTE *edsysver = "GENCCPM  v3.0  [3/7/83]\n";
#endif
#ifdef MPM
BYTE *edsysver = "GENMPM  vX.X  [??/??/??]\n";
#endif

BYTE mainmtitle[50] = "";		/* room for title of main menu */
MENU *mainmenu = NULLPTRI;		/* pointers to main menu items    */
MENU *xiosmenu = NULLPTRI;		/* pointers to XIOS menu items */
MENU *memmenu=NULLPTRI;			/* memory management menu */
MENU *syspmenu=NULLPTRI;		/* system parameters menu */
MENU *rspmenu=NULLPTRI;			/* rsp edit menu */

/**************************************************************************/
/* main module
/**************************************************************************/

main(ac,av)
WORD ac;
BYTE **av;
{
	BYTE cmds[CMDSLEN];		/* input area of commands */
	BOOLEAN sysvalid();		/* is the system data valid? */

	fprintf(stderr,edsysver);
	if( isatty(1) )			/* standard output to a tty? */
		clearit = clearx;	/* yes, do clear screens */
	else	clearit = "";		/* no, don't do clear screens */
	init_sysdat();			/* initialize system data */
	init_edsys();			/* initialize menus */
	if( ac>1 ) {			/* if any command line arguments */
	    copyargs(ac-1,av+1,cmds);	/* simplify cmd parser */
	    verbose = TRUE;		/* turn on wordiness by default */
	    domenu(cmds,mainmenu);	/* do what they've asked for */
	} 
	FOREVER {			/* work with user for a while */
		if(doit) {		/* does user want to quit? */
		    if( sysvalid() )	/* has evrything been defined? */
			break;		/* let them if everything okay */
		    press_return();	/* let them view the message(s) */
		}
		printf("%s",clearit);	/* clear the screen */
		prtmenu(mainmtitle,mainmenu);	/* display user options */
		printf(PROMPT);		/* prompt user */
		if( gets(cmds)!=cmds ){	/* on end of file */
		    close(0);		/* standard input */
		    open("CON:",AREAD);	/* reopen to the console */
		    close(1);		/* standard output: assume redirection */
		    open("CON:",AWRITE);/* reopen to console */
		    clearit = clearx;	/* do the clear screen routine */
		    continue;		/* force user to input blank line */
		}
		if( *cmds==NULL ) 	/* blank line of input */
		    continue;		/* don't bother with blank lines here */
		domenu(cmds,mainmenu);	/* do what they've asked for */
	    }				/*****************************/
					/* we've got user input */
	make_sysfile();			/* construct system image/use old one*/
	gentables();			/* generate the tables */
	fixups();			/* odds and ends on the system image */
	wrapup();			/* close files, etc. */
}

/* copyargs: copies args to cbuf for uniform parsing */
copyargs(ac,av,cbuf)
WORD ac;
BYTE **av;
BYTE *cbuf;
{
	for( *cbuf=NULL; ac>0; --ac, ++av ) {
		strcat(cbuf,*av);
		strcat(cbuf," ");
	}
}

/**************************************************************************/
/* init_sysdat: reads SYStem DATa info from:
/*	1. SYSFILE, if it exists
/*	2. SYSMOD, o.w. if this is the case, it also checks 
/*		for existence of all other required modules
/**************************************************************************/
VOID init_sysdat()
{
	REG WORD sf;			/* file descriptor for SysData read */
	LONG grpseek();			/* seek the data group */
	WORD goffs;			/* loc of group */
	EXTERN DLIST *rsps, *dirsearch(); /* directory search fn */
	BYTE *addmem();			/* for initializing memory */
	BYTE *chkmodules(), *xlist[80];	/* list of missing modules */

	printf("GENerate SYStem image for %s\n",VERLABEL);
	init_sds(sdat_area);		/* initialize program ptrs */
			/***************************************************/
	if(chkmodules(xlist)==NULLPTR  &&  (sf=BROPEN(SYSMOD)) > 0 ) {
	    printf("Constructing new %s file\n",SYSFILE);
					/*** procedureize this? for cmd use */
	    dogensys=TRUE;		/* no SYSFILE, read from modules */
	    if(grpseek(sf,GTYPDATA)<0)	/* seek start of data group */
		CRASH("no data group in SYSDAT module\n");
	    read(sf,sdat_area,SDLEN);	/* read in the data group */
	    /**** may have to init arbitrarily some MAX values here ****/
	    xtrapds = *sd_pul;		/* default number of extra process descriptors */
	    totqcbs = *sd_qul;		/* default number of qcbs */
	    totopen = 0x40;		/* arbitrary total open files & locked recs */
	    qbuflen = *sd_qmalen;	/* init'ed to bytes */
	    rsps = dirsearch("*.RSP");	/* init RSP list */
	    addmem("400,6000,400");	/* start out with this memory allocation */
	    close(sf);			/* don't need it for a while */
#ifdef CCPM
	    sf=BROPEN(XIOSMOD);		/* let's go read the XIOS info */
	    if( grpseek(sf,GTYPDATA) <= 0 ) /* if there is not a data group */
		grpseek(sf,GTYPCODE);	/* use the code group instead */
	    goffs = lseek( sf,0L,1 );	/* where did we end up? */
	    initxios_info(sf,goffs);	/* grab info from there */
	    close(sf);			/* don't need it for a while */
#endif
	} else if( FALSE  &&		/****** temporarily disable this option!!! ***/
		(sf=BROPEN(SYSFILE)) >= 0 ){ /* does SYSFILE exist? *******/
	    printf("Editting %s file\n",SYSFILE);
	    dogensys = FALSE;		/* tell them not to gen a new system */
	    grpseek(sf,GTYPDATA);	/* get data module in OS */
	    read(sf,sdat_area,SDLEN);	/* read in the info */
	    /**** may have to trace down some lists for MAX values ****/
	    close(sf);			/* don't need it for a while */
	} else {	/***************************************************/
	    if( xlist == NULLPTR ) {
		fprintf(stderr,"Can't find %s module\n",SYSMOD);
	    } else {
		fprintf(stderr,"Can't find these modules:\n%s\n",xlist);
	    }
	    fprintf(stderr,"Please find the correct modules\n");
	    exit(1);
	}				/**** end module check ****/
	if( VERSION != *sd_mpmvn ){	/* a little version number checking */
	    USERR("%s works on OS version %x\n",edsysver,VERSION);
	    USERR("Sys Data area found was from OS version %x\n",*sd_mpmvn);
	    USERR("Please find correct .SYS or %s files\n",MODEXT);
	    exit(1);
	}
}


/**************************************************************************/
#define RDAC 4				/* Read Access mode for 'access' */
    BYTE *
chkmodules(badbuf)			/* place to put missing module names */
BYTE *badbuf;
{

	*badbuf=0;	
	if( access(SYSMOD,RDAC) ) {strcat(badbuf,SYSMOD);strcat(badbuf," ");}
	if( access(SUPMOD,RDAC) ) {strcat(badbuf,SUPMOD);strcat(badbuf," ");}
	if( access(RTMMOD,RDAC) ) {strcat(badbuf,RTMMOD);strcat(badbuf," ");}
	if( access(MEMMOD,RDAC) ) {strcat(badbuf,MEMMOD);strcat(badbuf," ");}
	if( access(CIOMOD,RDAC) ) {strcat(badbuf,CIOMOD);strcat(badbuf," ");}
	if( access(BDOSMOD,RDAC) ) {strcat(badbuf,BDOSMOD);strcat(badbuf," ");}
#ifdef NET
	if( access(NETMOD,RDAC) ) {strcat(badbuf,NETMOD);strcat(badbuf," ");}
#endif
	if( access(XIOSMOD,RDAC) ) {strcat(badbuf,XIOSMOD);strcat(badbuf," ");}
	if( *badbuf ) 
		return badbuf;
	else	return NULLPTR;
}




/**************************************************************************/
/* init_edsys: initializes program variables and menus
/**************************************************************************/

VOID init_edsys()
{
	MENU *m;			/* mainmenu pointer */
	MENU *x;			/* xiosmenu pointer */
	MENU *me;			/* memory partitions menu */
	MENU *s;			/* sys params menu */
	MENU *r;			/* rsp menu */
	MENU *bldmenu();		/* func to build the menu list */
	BYTE *drvmsg;			/* destination drive message */
	BYTE *delmsg;			/* delete old SYSFILE msg */
	BYTE getdrive();		/* get default drive */
	BYTE *help();			/* func to help user */
	BYTE *doxios();			/* func to do xios menu */
	BYTE *dosysp();			/* func to do sys param menu */
	BYTE *dorsp();			/* func to display & handle RSPs */
	BYTE *dolbl();			/* func to modify version label */
	BYTE *domem();			/* handle memory partitions */
	BYTE *doxbufs();		/* func to construct bdos buffers */
	BYTE *addmem();			/* add mem part */
	BYTE *delmem();			/* delete mem part */
	BYTE *rspinclude();		/* include a bunch of RSPs */
	BYTE *rspexclude();		/* exclude a bunch of RSPs */
	BYTE *doexit();			/* exit the main menu */

	sprintf(mainmtitle,"*** %s %s Main Menu ***",VERLABEL,PROGNAME);
	destdrv = getdrive();		/* returns the default drive */
	drvmsg = malloc(50);
	sprintf(drvmsg,"%s Output To (Destination) Drive",SYSFILE);
	delmsg = malloc(55);
	sprintf(delmsg,"Delete (instead of rename) old %s file\n",SYSFILE);
	m=NULLPTR; x=NULLPTR; me=NULLPTR; s=NULLPTR; r=NULLPTR;	/* init menu pointers */

	/***** initialize menus *****/
#ifdef CCPM
	m=bldmenu(m,MPROC,help,"help","GENCCPM Help");
	m=bldmenu(m,MBOOL,&verbose,"verbose","More Verbose GENCCPM Messages");
#endif
#ifdef MPM
	m=bldmenu(m,MPROC,help,"help","GENMPM Help");
	m=bldmenu(m,MBOOL,&verbose,"verbose","More Verbose GENMPM Messages");
#endif
	m=bldmenu(m,MDRIV,&destdrv,"destdrive",drvmsg);
	m=bldmenu(m,MBOOL,&doclean,"deletesys",delmsg);


	m=bldmenu(m,MPROC,dosysp,"sysparams","Display/Change System Parameters");

	s=bldmenu(s,MDRIV,sd_srchdisk,"sysdrive","System Drive");
	s=bldmenu(s,MDRIV,sd_tempdisk,"tmpdrive","Temporary File Drive");
	s=bldmenu(s,MBOOL,sd_dayfile,"cmdlogging","Command Day/File Logging at Console");
	s=bldmenu(s,MBOOL,sd_cmode,"compatmode","CP/M FCB Compatibility Mode");
	s=bldmenu(s,MWORD,sd_mmp,"memmax","Maximum Memory per Process (paragraphs)");
	s=bldmenu(s,MBYTE,sd_popen_max,"openmax","Open Files per Process Maximum");
	s=bldmenu(s,MBYTE,sd_plock_max,"lockmax","Locked Records per Process Maximum\n");

	s=bldmenu(s,MWORD,sd_mpmseg,"osstart","Starting Paragraph of Operating System");
	s=bldmenu(s,MWORD,&totopen,"nopenfiles","Number of Open File and Locked Record Entries");
	s=bldmenu(s,MBYTE,&xtrapds,"npdescs","Number of Process Descriptors");
	s=bldmenu(s,MBYTE,&totqcbs,"nqcbs","Number of Queue Control Blocks");
	s=bldmenu(s,MWORD,&qbuflen,"qbufsize","Queue Buffer Total Size in bytes");


	m=bldmenu(m,MPROC,domem,"memory","Display/Change Memory Allocation Partitions");
	me=bldmenu(me,MPROC,addmem,"add","ADD Memory Partition(s)");
	me=bldmenu(me,MPROC,delmem,"delete","DELETE Memory Partition(s)");


#ifdef MPM
	m=bldmenu(m,MPROC,doxios,"xios","Display/Change XIOS Dependent Values");

	x=bldmenu(x,MBYTE,sd_ncns,"nconsoles","Number of System Consoles");
	x=bldmenu(x,MBYTE,sd_nlst,"nprinters","Number of System Printers");
	x=bldmenu(x,MBYTE,sd_nccb,"nccbs","Total Character Control Blocks");
	x=bldmenu(x,MBYTE,sd_nflags,"nflags","Number of Flags");
	x=bldmenu(x,MBYTE,sd_tickspsec,"nticks","Number of Ticks Per Second");
#endif
#ifdef CCPM
	m=bldmenu(m,MPROC,doxbufs,"diskbuffers","Display/Change Disk Buffer Allocation");
#endif

	if( dogensys )
	    m=bldmenu(m,MPROC,dolbl,"oslabel","Display/Change Operating System Label");

	m=bldmenu(m,MPROC,dorsp,"rsps","Display/Change RSP list\n");
	r=bldmenu(r,MPROC,rspinclude,"include","Include RSPs");
	r=bldmenu(r,MPROC,rspexclude,"exclude","Exclude RSPs");


	m=bldmenu(m,MPROC,doexit,"gensys","I'm finished changing things, go GEN a SYStem\n");


	mainmenu=m;			/* assign now to longer name */
	xiosmenu=x;			/* assign to longer name */
	memmenu=me;			/*  yah yah */
	syspmenu=s;			/*   "   "  */
	rspmenu=r;			/*   "   "  */
}


    BYTE *
doexit()				/* this is really a boolean... */
{
	doit = TRUE;
	return NULLPTR;
}

/**************************************************************************/
/* sysvalid: validates system data.
/*	also calculates certain SYSDAT variables, in the course of validation.
/**************************************************************************/
#define BYTE_ADJUST(xx) if( xx > 0xFF ) xx = 0xFF
#define ALIGN(xx) (((xx)+0xF) & 0xFFF0)

    BOOLEAN
sysvalid()
{
	REG BOOLEAN isv;		/* is valid? */
#ifdef CCPM
	BOOLEAN xiosvalid();		/* check xios info */
#endif
	LOCAL WORD nmparts;		/* number of memory partitions */

	isv = TRUE;			/* innocent until proven guilty */
	if( totopen < *sd_popen_max ) {
		USERR("'nopenfiles' is less than 'openmax'. Please adjust.\n");
		isv = FALSE;
	}
	if( totopen < *sd_plock_max ) {
		USERR("'nopenfiles' is less than 'lockmax'. Please adjust.\n");
		isv = FALSE;
	}
	if( (nmparts=cntmlist(memroot)) <= 0 ) {
		USERR("Memory Partitions need to be adjusted\n");
		isv = FALSE;
	}
#ifdef CCPM
	if( !xiosvalid() ) {		/* xios info needs work? */
		USERR("Disk Performance Buffers need to be adjusted\n");
		isv=FALSE;
	}
#endif
	if( !isv )
		USERR("Please correct the System Parameters\n");
	return isv;
}



/**************************************************************************/
/* make_sysfile: responsible for preparing SYSfile image for table generation
/* and 
/*	if( dousesys )
/*		doctors up SYSFILE for appropriate table generation and fixups
/*	else	constructs a SYSfile from assembled modules
/**************************************************************************/
VOID make_sysfile()
{
	BYTE fbuf[20];			/* file name buffer */

	PUTDRV(fbuf,NEWSYS);
	unlink(fbuf);			/* this is the file we'll be using */
	if( (fns=BWCREAT(fbuf))<0 ) {	/* try to open for output */
	    USERR("can't open new .SYS file (%s)\n",fbuf);
	    exit(1);
	}
	if( dogensys )
		gensys();		/* make a new SYS file from modules */
	else	edsys();		/* edit the old SYS file */
}


/**************************************************************************/
/* gensys: generates a new SYS file image from modules
/**************************************************************************/
    VOID
gensys()
{
	WORD fxm;			/* file descriptor for xios mod */
	BYTE fbuf[20];

	if( doclean ) {			/* clean up the SYSFILE first? */
	    PUTDRV(fbuf,SYSFILE);
	    unlink(fbuf);		/* delete if there... */
	}
	if(verbose) printf("Generating new SYS file\n");
	FCHECK(fns);			/* File error check */
	write(fns,cmdhdr,SECSIZ);	/* write out a (dummy) command hdr */
	genfix(); 			/* remember the sysdat & sup fixups */
	clsup = xfergrp(SUPMOD,GTYPCODE);
	clos_label = writelbl();	/* write the OS label (returns 0 if none exists) */
	if( clos_label != 0 )
		*sd_verptr = clsup<<4;	/* fill in offset of label rel sup code seg */
	genfix(); clrtm = xfergrp(RTMMOD,GTYPCODE);
	genfix(); clmem = xfergrp(MEMMOD,GTYPCODE);
	genfix(); clcio = xfergrp(CIOMOD,GTYPCODE);
	genfix(); clbdos= xfergrp(BDOSMOD,GTYPCODE);
#ifdef NET
	genfix(); clnet = xfergrp(NETMOD,GTYPCODE);
#else
	clnet = 0;
#endif
	fxm = BROPEN(XIOSMOD);		/* look at XIOS module */
	if( grpseek(fxm,GTYPDATA) >= 0){	/* is there a data group?*/
#ifdef CCPM
		USERR("XIOS has separate code and data (small model)");
		USERR("This is not supported in this O.S.: use 8080 model.");
		exit(1);
#else
		pure_xios = TRUE;	/* ifso, it's separate code & data */
		genfix(); clxios = xfergrp(XIOSMOD,GTYPCODE);
#endif
	} else {
		pure_xios = FALSE;
		clxios = 0;
	}
	cltotal = clsup+clos_label+clrtm+clmem+clcio+clbdos+clnet+clxios;
	/*** finished writing the code portion of NEWSYS ***/

	fldstart = lseek(fns,0L,1);	/* where in file SYSDAT gets written */
	dsstart = *sd_mpmseg + cltotal;	/* where in memory SYSDAT gets loaded */
	dlsysdat = xfergrp(SYSMOD,GTYPDATA);	/* system data area, fixed later */

	if( pure_xios )
		dlxios = xferpart_grp(fxm,GTYPDATA,dlsysdat);
	else { genfix(); dlxios = xferpart_grp(fxm,GTYPCODE,dlsysdat);	}
	close(fxm);			/* don't need this file descriptor */
	dltotal = dlsysdat+dlxios;	/* this will change later */
	/*** finished writing STATIC data portion of NEWSYS ***/

	/*** fill in sysdat here ***/
	sd_supmod->ep_eseg  = sd_supmod->ep_iseg  = *sd_mpmseg;
	sd_rtmmod->ep_eseg  = sd_rtmmos->ep_iseg  = sd_supmod->ep_eseg+clsup+clos_label;
	sd_memmod->ep_eseg  = sd_memmod->ep_iseg  = sd_rtmmod->ep_eseg+clrtm;
	sd_ciomod->ep_eseg  = sd_ciomod->ep_iseg  = sd_memmod->ep_eseg+clmem;
	sd_bdosmod->ep_eseg = sd_bdosmod->ep_iseg = sd_ciomod->ep_eseg+clcio;
	sd_xiosmod->ep_eseg = sd_xiosmod->ep_iseg = sd_bdosmod->ep_eseg+clbdos;
#ifdef NET
	sd_netmod->ep_eseg  = sd_netmod->ep_iseg  = sd_xiosmod->ep_eseg+clxios;
#endif
	if( pure_xios )			/* compute xios offsets */
		sd_xiosmod->ep_ioff = 0;	/* separate code */
	else	sd_xiosmod->ep_ioff = dlsysdat<<4;	/* mixed code & data, after sysdat */
	sd_xiosmod->ep_eoff = sd_xiosmod->ep_ioff + 3;
}

/****************************************************************************/
genfix()		/* subroutine to remember SYSDAT & Super fixups */
{
	LONG fa;

	fa = lseek(fns,0L,1);		/* where are we in this file? */
	fixlater(fa+6,F_PUT,&dsstart);	/* fix the SYSDAT address */
	fixlater(fa+8,F_PUT,&(sd_supmod->ep_eoff));	/* Supervisor entry offset */
	fixlater(fa+10,F_PUT,&(sd_supmod->ep_eseg));	/*     "        "   segment */
}


/**************************************************************************/
/* edsys: edit the old SYS file image.
/*	- copies old sys file into new
/*	- if( donew_xios ) will patch in a new XIOS from module
/**************************************************************************/
    VOID
edsys()
{
	if(verbose) printf("Editting old SYS file\n");
	write(fns,cmdhdr,SECSIZ);	/* leave room for command hdr */
	cltotal = xfergrp(SYSFILE,GTYPCODE);
	dltotal = xfergrp(SYSFILE,GTYPDATA);
	if( donew_xios ) {
	    ; /* need to read 'xt' from XIOS module */
	} else {
#ifdef CCPM
	    ; /* need to init 'xt' from sysdat */
#endif
	}
}



/**************************************************************************/
/* fixups: inserts updated SYSDAT page into NEWSYS file, and does some
/*	address fixups
/**************************************************************************/
VOID fixups()
{
	REG WORD ii;			/* counter */
	GROUP *cg, *dg;			/* group ptrs */
	UWORD genmfl();			/* (re)Gen Memory Free List */

	if(verbose) printf("Doing fixups\n");
	FCHECK(fns);			/* File error check */
					/*** fixup the memory list ***/
	if(verbose) {
	    printf("SYS image load map:\n");
	    printf("        Code starts at %4.4x\n",*sd_mpmseg);
	    printf("        Data starts at %4.4x\n",dsstart);
	    printf("       Tables start at %4.4x\n",(dsotables>>4)+dsstart);
	    printf("         RSPs start at %4.4x\n",(dsotables>>4)+dsstart+dltables);
	    printf(" XIOS buffers start at %4.4x\n",rsvd_seg);
	    printf("          End of OS at %4.4x\n",*sd_endseg);
	}

	if( trimlist(*sd_mpmseg,*sd_endseg) ) {	/* mem part list trimmed? */
	    printf("Trimming memory partitions. New list:");
	    dspmlist(memroot);
	    lseek( fns, locmfl, 0 );
	    ii = (dlsysdat+dlxios)<<4;
	    genmfl(ii);			/*****/
	}
#ifdef CCPM
					/*** fixup XIOS info ***/
	fixupxios_info(); 		/* fixes the buffer info (DPH info) */
#endif
					/*** fixup the command header ***/
	for( ii=0; ii<SECSIZ; ii++ )	/* zero out command header */
	    cmdhdr[ii] = '\0';
	cg = cmdhdr;			/* code group first */
	cg->gtype = GTYPCODE;		/* init all code vars */
	cg->glen = cltotal;		/* total length of code groups */
	cg->gmin = cltotal;		/* min length of code group */
	cg->gabase = *sd_mpmseg;	/* loads at absolute address */
	dg = cmdhdr + sizeof(*cg);	/* data group next */
	dg->gtype = GTYPDATA;		/* init all data vars */
	dg->glen = dltotal;		/* total length of data group+tables */
	dg->gmin = dltotal;		/* min length of data group */
	dg->gabase = cg->gabase+cltotal;/* abs addr of start of system data */
	lseek( fns, 0L, 0 );		/* seek the beginning of NEWSYS */
	write(fns,cmdhdr,SECSIZ);	/* write out the command header */

					/*** fixup the sysdat page ***/
	lseek(fns,fldstart,0);		/* seek there in file */
	write(fns,sdat_area,SDLEN);	/* put it into the file */

					/*** take care of the little fixes ***/
	fixfile();			/* fixes from the list */
}

/**************************************************************************/
/* wrapup: closes files
/**************************************************************************/
VOID wrapup()
{
	BYTE buf1[20], buf2[20];

	if(verbose) printf("Wrapping up\n");
	FCHECK(fns);			/* File error check */
	lseek(fns,0L,2);		/* flush NEWSYS buffers? */
	if(strlen(edsysver) != write(fns,edsysver,strlen(edsysver))) {
	    USERR("WRITE FAILURE - the disk may be too full\n");
	    exit(1);
	}
	close(fns);			/* all finished with NEWSYS */
	PUTDRV(buf1,OLDSYS); unlink(buf1);
	PUTDRV(buf2,SYSFILE); rename(buf2,buf1);
	PUTDRV(buf1,NEWSYS); rename(buf1,buf2);
}



/***************************************************************/
/***************************************************************/
/* edszzz.c: the rest of edsys.c, broken up for shorter compiles */
/***************************************************************/
/***************************************************************/
#ifndef MAINMODULE
#include <genccpm.h>
#endif


/**************************************************************************/
/* init_sds: points the system data pointers to the right place
/**************************************************************************/
    VOID
init_sds(sysdat)
    BYTE *sysdat;			/* ptr to system data buffer */
{
	sd_supmod     = sysdat + 0x00;	/* (4) */
	sd_rtmmod     = sysdat + 0x08;	/* (4) */
	sd_memmod     = sysdat + 0x10;	/* (4) */
	sd_ciomod     = sysdat + 0x18;	/* (4) */
	sd_bdosmod    = sysdat + 0x20;	/* (4) */
	sd_xiosmod    = sysdat + 0x28;	/* (4) */
	sd_netmod     = sysdat + 0x30;	/* (4) */
	sd_mpmseg     = sysdat + 0x40;
	sd_rspseg     = sysdat + 0x42;
	sd_endseg     = sysdat + 0x44;
	sd_module_map = sysdat + 0x46;
	sd_ncns       = sysdat + 0x47;
	sd_nlst       = sysdat + 0x48; 
	sd_nccb       = sysdat + 0x49;
	sd_nflags     = sysdat + 0x4A;
	sd_srchdisk   = sysdat + 0x4B;
	sd_mmp        = sysdat + 0x4C;
	sd_nslaves    = sysdat + 0x4E;
	sd_dayfile    = sysdat + 0x4F;
	sd_tempdisk   = sysdat + 0x50;
	sd_tickspsec  = sysdat + 0x51;
	sd_lul        = sysdat + 0x52;
	sd_ccb        = sysdat + 0x54;
	sd_flags      = sysdat + 0x56;
	sd_mdul       = sysdat + 0x58;
	sd_nxmds      = sysdat + 0x58;
	sd_mfl        = sysdat + 0x5A;
	sd_nmparts    = sysdat + 0x5A;
	sd_pul        = sysdat + 0x5C;
	sd_nxpd       = sysdat + 0x5C;
	sd_qul        = sysdat + 0x5E;
	sd_nqds       = sysdat + 0x5D;        
	sd_qmau       = sysdat + 0x60;	/* (4) */
	sd_qmastart   = sysdat + 0x62;
	sd_qmalen     = sysdat + 0x64;
	sd_verptr     = sysdat + 0x78;
	sd_vn         = sysdat + 0x7A;
	sd_mpmvn      = sysdat + 0x7C;
	sd_ncondev    = sysdat + 0x83;
	sd_nlstdev    = sysdat + 0x84;
	sd_nciodev    = sysdat + 0x85;
#ifdef MPM
	sd_plock_max  = sysdat + 0x8A;
	sd_popen_max  = sysdat + 0x8C;
#endif
#ifdef CCPM
	sd_lcb        = sysdat + 0x86;
	sd_popen_max  = sysdat + 0x8A;
	sd_plock_max  = sysdat + 0x8B;
#endif
	sd_cmode      = sysdat + 0x90;
}


/************************************************************************/
/* doxios: gets miscellaneous XIOS values from user */
/************************************************************************/
    BYTE *
doxios(bb,xmtitle)			/* submenu for XIOS value mods */
    BYTE *bb;				/* value passed in from menu */
    BYTE *xmtitle;
{
	BYTE cmds[CMDSLEN];		/* space for additional commands */
	EXTERN MENU *xiosmenu;		/* list of menu items for xios menu */
	EXTERN BYTE *clearit;

	if( bb != NULLPTR ) {		/* if any "command line" arguments */
	    domenu(bb,xiosmenu);	/* do what they've asked for */
	} else FOREVER {		/* work with user for a while */
		printf("%s",clearit);	/* clear screen */
		prtmenu(xmtitle,xiosmenu);	/* display user options */
		printf(PROMPT);		/* prompt user */
		if( gets(cmds)!=cmds ||	/* on end of file */
		    *cmds==NULL )	/* or blank line of input */
		    break;		/* exit interaction with user */
		domenu(cmds,xiosmenu);	/* do what they've asked for */
	}				/*****************************/
	return NULLPTR;			/* standard return... */
}



/************************************************************************/
/* dosysp: gets miscellaneous system paramaters from user */
/************************************************************************/
    BYTE *
dosysp(bb,smtitle)			/* submenu for SYSP value mods */
    BYTE *bb;				/* value passed in from menu */
    BYTE *smtitle;
{
	BYTE cmds[CMDSLEN];		/* space for additional commands */
	EXTERN MENU *syspmenu;		/* list of menu items for sysp menu */
	EXTERN BYTE *clearit;

	if( bb != NULLPTR ) {		/* if any "command line" arguments */
	    domenu(bb,syspmenu);	/* do what they've asked for */
	} else FOREVER {		/* work with user for a while */
		printf("%s",clearit);	/* clear screen */
		prtmenu(smtitle,syspmenu);	/* display user options */
		printf(PROMPT);		/* prompt user */
		if( gets(cmds)!=cmds ||	/* on end of file */
		    *cmds==NULL )	/* or blank line of input */
		    break;		/* exit interaction with user */
		domenu(cmds,syspmenu);	/* do what they've asked for */
	}				/*****************************/
	return NULLPTR;			/* standard return... */
}



/*************************************************************************/
/* dorsp: handles rsp editting */
/*************************************************************************/
    BYTE *
dorsp(parm,xmtitle)			/* handle RSP list editting */
    BYTE *parm;				/* command value parameter */
    BYTE *xmtitle;
{
	BYTE cmds[CMDSLEN];		/* space for additional commands */
	EXTERN MENU *rspmenu;		/* list of menu items for rsp menu */
	EXTERN BYTE *clearit;

	if( parm != NULLPTR ) {		/* if any "command line" arguments */
	    domenu(parm,rspmenu);	/* do what they've asked for */
	} else FOREVER {		/* work with user for a while */
		printf("%s",clearit);	/* clear screen */
		rsp_display(rsps);	/* display what they have to work with */
		prtmenu(xmtitle,rspmenu);	/* display user options */
		printf(PROMPT);		/* prompt user */
		if( gets(cmds)!=cmds ||	/* on end of file */
		    *cmds==NULL )	/* or blank line of input */
		    break;		/* exit interaction with user */
		domenu(cmds,rspmenu);	/* do what they've asked for */
	}				/*****************************/
	return NULLPTR;			/* standard return... */
}

/*************************************************************************/
    BYTE *
rspinclude(rs)
    BYTE *rs;
{
	BYTE *bp, *cp, *ep;		/* ptrs to take apart command str */
	DLIST *dd;

	if( rs==NULLPTR || *rs==NULL ) 
	    return "You need to say 'include=xx.rsp,yy.rsp,zz.rsp'";
	for( ep=rs; *ep; ++ep ) ;	/* end of string pointer */
	for( bp=rs; bp<ep; bp=cp+1 ){	/* process each rsp spec */
	    for( cp=bp; *cp && *cp!=','; ++cp ) ;	/* find eos or comma */
	    if( *cp == ',' ) *cp = '\0'; /* null terminate the spec */
	    if( (dd=dirsearch(bp)) == NULLPTR )
		return "Can't find RSP to include";
	    rspjoin(dd);
	}
	return NULLPTR;
}

rspjoin(dd)
    DLIST *dd;
{
	DLIST *rs;

	if( rsps == NULLPTR )
	    rsps = dd;
	else {
	    for( rs=rsps; rs->dlnext != NULLPTR; rs=rs->dlnext ) ;
	    rs->dlnext = dd;
	}
}



/*************************************************************************/
    BYTE *
rspexclude(rs)
    BYTE *rs;
{
	BYTE *bp, *cp, *ep;
	BOOLEAN rspzap();

	if( rs==NULLPTR || *rs==NULL )
	    return "You need to say 'exclude=aa.rsp,bb.rsp,cc.rsp'";
	for( ep=rs; *ep; ++ep ) ;	/* find eos */
	for( bp=rs; bp<ep; bp=cp+1 ){	/* process each rsp spec */
	    for( cp=bp; *cp && *cp!=','; ++cp ) ;	/* find eos or comma */
	    if( *cp == ',' ) *cp = '\0'; /* null terminate the spec */
	    if( !rspzap(bp) )
		return "Can't find RSP to exclude";
	}
	return NULLPTR;
}

    BOOLEAN
rspzap(nm)
    BYTE *nm;
{
	DLIST **qs, *ps, *rs;

	if( *nm == '*' ) {		/* zap entire list? */
	    for( ps=rsps; ps!=NULLPTR; ps=rs ) {
		rs = ps->dlnext;
		free(ps);
	    }
	    rsps = NULLPTR;
	    return TRUE;
	}
	for( qs= &rsps; *qs!=NULLPTR; qs = &rs->dlnext ) {	/* zap part of list */
	    rs = *qs;
	    if( namematch(nm,rs->dlname) ) {
		*qs = rs->dlnext;
		free(rs);
		return TRUE;
	    }
	}
	return FALSE;			/* thru loop, can't find it */
}

namematch(s1,s2)
    BYTE *s1, *s2;
{
	for( ; *s1 && *s2; ++s1, ++s2 ) 
	    if( toupper(*s1) != toupper(*s2) )
		return FALSE;
	if( *s1 || *s2 )
	     return FALSE;
	else return TRUE;
}


/**************************************************************************/
#define PAGEWIDTH 72
rsp_display(rp)
DLIST *rp;
{
	WORD ii;

	printf("\nRSPs to be included are:\n");
	for( ii=0; rp!=NULLPTR; rp = rp->dlnext ) {
	    if( (ii += 15) > PAGEWIDTH ) {
		printf("\n");
		ii=15;
	    }
	    printf("   %12.12s",rp->dlname);
	}
	printf("\n");
}


/**************************************************************************/
/* grpseek: seeks in MOD file 'fd' to the start of a code/data group, */
/*	returns length (in bytes) of that group */
/*************************************************************************/
    LONG
grpseek(fde,gt)
    WORD fde;				/* file descriptor */
    WORD gt;				/* group type to look for */
{
	WORD gc;			/* group counter */
	LONG dps;			/* pgphs to group base in file */
	GROUP grp;			/* structure for Cmd Hdr info */

	if( lseek(fde,0L,0) < 0 )	/* seek to beginning of file */
	    CRASH("grp cmd header seek failed\n");
	dps=8;				/* pgphs to start of load group */
	for( gc=0; gc<8; gc++ ) {	/* search up to 8 groups in cmd hdr */
	    read(fde,&grp,sizeof(grp));	/* read each group in header */
	    if( grp.gtype == gt )	/* the group we want */
		    break;
	    dps += grp.glen;		/* add in size of previous group */
	}
	if( grp.gtype != gt )		/* did we find the group? */
	    return ((LONG) FAILURE);	/* nope, so fail */
	dps = dps << 4;			/* pgphs xfm to bytes */
	if( lseek(fde,dps,0) < 0 )	/* seek to start of data group */
	    CRASH("grp group seek failed\n");
	return ((LONG)((grp.glen)<<4));	/* return group size in bytes */
}


/**************************************************************************/
/* xfer rtns: transfer a group from file 'module' to file NEWSYS (fns) 
/*	returns number of paragraphs in the group
/**************************************************************************/
    WORD
xfergrp(module,group_type)
    BYTE *module;			/* module name */
    WORD group_type;			/* code? or data? */
{
	REG WORD fm;			/* file descriptor for module */
	REG WORD rv;			/* return value */

	if( (fm=BROPEN(module)) < 0 ) 	/* open the module for binary reading */
		CRASH("can't find a system module\n");
	rv = xferpart_grp(fm,group_type,0);
	close(fm);
	return rv;
}

/**************************************************************************/
    WORD
xferpart_grp(fm,gt,skip)
    WORD fm;			/* file descriptor for module */
    WORD gt;				/* group type: code or data */
    UWORD skip;				/* num pgphs to skip in this group */
{
	REG WORD ii;			/* counter */
	REG LONG xx, yy;		/* long counters */
	BYTE xferbuf[BUFSIZ];		/* a transfer buffer */
	GROUP grp;			/* a group area */

	if( lseek(fm,0L,0) < 0 )	/* seek to beginning of file */
	    CRASH("grp cmd header seek failed\n");
	yy = skip+8;			/* num pgphs to skip + in cmd hdr */
	for( ii=0; ii<8; ++ii ) {	/* read group hdrs in cmd hdr */
	    read(fm,&grp,sizeof(grp));
	    if( grp.gtype == gt )
		break;
	    yy += grp.glen;		/* count #pgphs in this group to skip */
	}
	if( grp.gtype!=gt ) CRASH("module doesn't have group\n");
	xx = yy << 4;			/* cvt to bytes */
	if( lseek(fm,xx,0)<0 ) CRASH("couldn't seek in module\n");
	xx = grp.glen; xx -= skip; 	/* pgphs to xfer */
	for( yy=xx<<4; yy>0; yy-=ii ){	/* xfer bytes to NEWSYS */
	    ii = ( yy > BUFSIZ ? BUFSIZ : yy );	/* num bytes per xfer */
	    if( read(fm,xferbuf,ii) != ii ) CRASH("not enough bytes in module\n");
	    write(fns,xferbuf,ii);
	}
	return (WORD)(xx);		/* convert to pgphs */
}



/**************************************************************************/
/* help: prints out a help message */
/**************************************************************************/

BYTE *helpm1[] = {
	"\n\n",
	"\t\t*** GENCCPM Help Function ***",
	"\t\t============================",
	"     GENCCPM lets you edit and/or generate a system image from",
	"operating system modules on the default drive.  A detailed",
	"explanation of each parameter may be found in the Concurrent CP/M-86",
	"System Guide, Section 2.\n",
	"     GENCCPM assumes the default values shown within square",
	"brackets.  All numbers are Hexadecimal.  To change a parameter,",
	"enter the parameter name followed by '=' and the new value.  Type",
	"<CR> (a carriage return) to enter the assignment.  You can make",
	"multiple assignments if you separate them by a space.  No spaces",
	"are allowed within an assignment.  Example:\n",
	"CHANGES?  verbose=N sysdrive=A: openmax=1A <CR>\n",
	"Parameter names may be shortened to the minimum combination of",
	"letters unique to the currently displayed menu.  Example:\n",
	"CHANGES?  v=N sysd=A: op=1a <CR>\n",
	NULLPTRI
};
BYTE *helpm2[] = {
	"\n\n",
	"Sub-menus (the last few options without default values) are accessed",
	"by typing the sub-menu name followed by <CR>.  You may enter",
	"multiple sub-menus, in which case each sub-menu will be displayed",
	"in order.  Example:\n",
	"CHANGES?  help sysparams rsps <CR>\n",
	"Enter <CR> alone to exit a menu, or a parameter name, '=' and the",
	"new value to assign a parameter.  Multiple assignments may be",
	"entered, as in response to the Main Menu prompt.\n",
	NULLPTRI
};


    BYTE *
help(tx)				/* print a helpful message to user */
    BYTE *tx;
{
	REG BYTE **cpp;		/* pointer to help strings */

	for( cpp=helpm1; *cpp != NULLPTR; cpp++ )
		printf("%s\n",*cpp);
	press_return();
	for( cpp=helpm2; *cpp != NULLPTR; cpp++ )
		printf("%s\n",*cpp);
	press_return();
	return NULLPTR;
}

/************************************************************************/
/* memory list allocation */
/**************************************************************************/


    BYTE *
domem(cmdval,memtitle)
    BYTE *cmdval;
    BYTE *memtitle;
{
	WORD cntmlist();		/* does list validation */
	BOOLEAN okl;			/* okay list */
	BYTE cmds[CMDSLEN];		/* place to put input */
	EXTERN MLIST *memroot;		/* memory list root */
	EXTERN MENU *memmenu;		/* memory edit menu */
	EXTERN BYTE *clearit;

	okl = (cntmlist(memroot) > 0);	/* overlap chk & condense */
	if(cmdval!=NULLPTR) {
	    domenu(cmdval,memmenu);	/* do the commands */
	    okl = (cntmlist(memroot)>0);/* check the results */
	}
	if( !okl || cmdval==NULLPTR ) 	/* do we need to interact with user? */
	    FOREVER {			/* display, the edit */
		printf("%s",clearit);	/* clear the screen (maybe) */
		dspmlist(memroot);	/* here's what it looks like */
		prtmenu(memtitle,memmenu);	/* here's how to edit */
		printf(PROMPT);		/* what to do? */
		if( gets(cmds)!=cmds ||	*cmds==NULL ) {	/* see if the want to get out */
		    if(okl)
			 break;
		    else {
			printf("Please adjust memory partitions\n");
			press_return();
			continue;
		    }
		}
		domenu(cmds,memmenu);	/* edit the memory list */
		okl = (cntmlist(memroot)>0);/* validate the memory list */
	    }
	return NULLPTR;			/* standard return */
}


/**************************************************************************/
    BYTE *
addmem(ms)				/* add memory partitions */
    BYTE *ms;				/* memory partition spec */
{
	REG BYTE *pl;			/* pointer to last addr spec */
	REG BYTE *ps;			/* pointer to size spec */
	UWORD f, l, s;			/* values of spec */
	MLIST *intomlist(), *mnew;	/* where to put them */
	EXTERN MLIST *memroot;		/* memory partitions list */
	BYTE *badspec = "Add memory partition spec should look like:\n\tadd=first,last,size";
	BYTE *badval = "Spec: add=first,last,size\n\twhere last>first, size>80";
	BYTE *index();

	if( (pl=index(ms,','))==0)	/* look for ptr to last addr */
		return badspec;
	else	*pl++ = NULL;		/* terminate first addr spec */
	if( (ps=index(pl,','))==0)	/* look for ptr to size */
		return badspec;
	else	*ps++ = NULL;
	f=atoih(ms);			/* convert first addr spec */
	s=atoih(ps);			/* convert size spec */
	if( *pl=='+' )			/* last addr relative to first? */
		l=atoih(pl+1)+f;	/* yes, so calculate */
	else if( *pl==NULL  ||  *pl=='*' ) {
	    if( *pl=='*' )		/* last addr multiple of size? */
		 l=atoih(pl+1);		/* spec multiple */
	    else l=1;			/* default multiple=1 */
	    l = f+l*s;			/* s in paragraphs */
	} else	l=atoih(pl);		/* no, it's absolute address */
	if( l<=f  ||  s==0 )		/* 1st validation check */
		return badval;		
	if( s<MINKMEM )			/* 2nd validation check */
		return "Memory partition must be at least 80 paragraphs";
	mnew = (MLIST *)malloc(sizeof(*mnew));	/* make room */
	mnew->mlfirst = f;
	mnew->mllast = l;
	mnew->mlsize = s;
	memroot=intomlist(memroot,mnew);	/* sorted insertion into list */
	if( s > l-f )			/* check: size_partition > mem_region */
		return "Warning: partition size larger than memory region";
	return NULLPTR;			/* everything okay */
}


/**************************************************************************/
    BYTE *
delmem(ms)				/* delete memory partition */
    BYTE *ms;
{
	REG WORD df, dl;		/* delete first, last */
	MLIST **mm, **mn, *mo, *mp, *mq;	/* necessary list ptrs */
	EXTERN MLIST *memroot;		/* memory partitions list */
	BYTE *pl;
	WORD i;
	BYTE *badspec="To delete a memory partition, say\n\tdelete=1  or  delete=1-3\n";

	if( *ms == '*' ) {		/* delete all? */
	    for( mp=memroot; mp!=NULLPTR; mp=mq ) {
		mq = mp->mlnext;	/* save the pointer before we free it */
		free(mp);
	    }
	    memroot = NULLPTR;
	    return NULLPTR;		/* everything okay */
	}
	if( (pl=index(ms,'-')) == 0 )
	       	pl=ms;			/* point last to first */
	else 	*pl++ = NULL; 		/* else point to after 'dash' */
	df=atoi(ms);			/* decimal conversion */
	dl=atoi(pl);
	for( i=1, mm=&memroot, mn=NULLPTR, mo=memroot;
	    i<=dl && mo!=NULLPTR;
	    ++i ) {
		if( i==df ) mn=mm;	/* found first, pt to ptr to */
		mm = &mo->mlnext;	/* ptr to ptr */
		mo = mo->mlnext;
	}
	if( mn==NULLPTR || i<=dl )	/* check range of deletions */
	    return badspec;
	mp = *mn;			/* save ptr to deleted list */
	*mn = mo;			/* cut them suckers out of the list */
	for( ; mp!=mo; mp=mq ) {
	    mq = mp->mlnext;
	    free(mp);	
	}
	return NULLPTR;			/* everything ok */
}



/**************************************************************************/
dspmlist(mroot)
    MLIST *mroot;
{
	MLIST *mn, *mo;
	REG WORD i, nump;

	printf("\n\n");
	printf("          Addresses          Partitions    (in paragraphs)\n");
	printf(" #      Start    Last       Size      Qty \n");
	for( i=1, mn=mroot; mn!=NULLPTR; ++i, mn=mo ) {
	    nump = (mn->mllast-mn->mlfirst) / mn->mlsize;
	    printf("%2.2d.     %4.4xh    %4.4xh      %4.4xh   %4.4xh ",
		    i,mn->mlfirst,mn->mllast,mn->mlsize,nump);
	    if( (mo=mn->mlnext) != NULLPTR ) {	/* do some checking */
		if( mn->mllast > mo->mlfirst ) 
		    printf("**overlaps** ");
		if( mn->mlsize > mn->mllast-mn->mlfirst )
		    printf("**partition too big** ");
	    }
	    printf("\n");
	}
}


/**************************************************************************/
    WORD
cntmlist(mroot)			/* check for overlaps, condense if possible */
    MLIST *mroot;		/* returns number mem partitions, 0 if bad list */
{
	MLIST *mn, *mo;			/* ptr within list */
	REG BOOLEAN olap;		/* partitions overlap or bad size */
	REG WORD nump;			/* number of partitions this block */
	REG WORD totnump;		/* keep the count going */

	totnump = 0;			/* total number partitions */
	olap = FALSE;			/* innocent until proven guilty */
	for( mn=mroot; mn!=NULLPTR && (mo=mn->mlnext)!=NULLPTR; ) {
	    if( mn->mlsize==mo->mlsize ){ 	/* partitions same size? */
	      if( mn->mlfirst==mo->mlfirst && 	/* same partitions? */
		  mn->mllast==mo->mllast ) {
			mn->mlnext=mo->mlnext;
			free(mo);	/* mo is redundant, zap it */
			continue;
	      } else 
	      if( mn->mllast==mo->mlfirst  ||	/* adjacent partition? */
		 mn->mllast==(mo->mlfirst-1) ) {
			mn->mllast=mo->mllast;	/* make 1st partition bigger */
			mn->mlnext=mo->mlnext;	/* zap out mo from list */
			free(mo);	/* we don't need it any more */
			continue;
	      }
	    }
	    if( mn->mllast > mo->mlfirst ) 
		olap=TRUE;		/* partitions overlap */
	    nump = (mn->mllast-mn->mlfirst) / mn->mlsize;
	    if( nump<=0 ) 
		olap=TRUE;		/* size > partition */
	    else totnump += nump;	/* incr this count */
	    mn=mo;			/* continue through loop */
	}
	if( mn!=NULLPTR ) {		/* catch the last in the loop */
	    nump = (mn->mllast-mn->mlfirst) / mn->mlsize;
	    if( nump<=0 ) 
		olap=TRUE;		/* size > partition */
	    else totnump += nump;	/* incr this count */
	}
	if( olap ) 
	     return 0;			/* boo boo, return 0 */
	else return totnump;		/* return num partitions */
}



/**************************************************************************/
    MLIST *
intomlist(mroot,mi)		/* insert mi into list mroot sorted */
    MLIST *mroot;
    MLIST *mi;
{
	MLIST **mm;			/* ptr to ptr to list */
	MLIST *mn;			/* ptr to list */

	for( mm=&mroot, mn=mroot;
	    mn != NULLPTR  &&  mi->mlfirst > mn->mlfirst; ){
		mm = &mn->mlnext;
		mn = mn->mlnext;
	}
	mi->mlnext = mn;
	*mm = mi;
	return mroot;
}


/**************************************************************************/
/* trimlist: trims the memory list according to OS bounds */
/**************************************************************************/

    BOOLEAN				/* returns TRUE iff any adjustments */
trimlist(osstart,osend)
    WORD osstart;			/* starting pgph */
    WORD osend;				/* ending pgph */
{
	EXTERN MLIST *memroot;
	REG MLIST **mm, *mn, *mo;	/* ptrs into memlist */
	REG WORD ss, zz;		/* temp vars */
	LOCAL WORD adjusts;		/* number of collisions */

	adjusts = 0;			/* we haven't adjusted anything yet */
	for( mm=&memroot, mn=memroot; mn!=NULLPTR; mn=*mm ) {
	    if( mn->mlfirst < osstart ) { 
		if( mn->mllast >= osstart ) { /** collision **/
		    ++adjusts;		/* we're doing some adjusting */
		    if( mn->mllast > osend ) {	/* real big mem part? */
			mo = (MLIST *)malloc(sizeof *mo); /* set it up now, adjust it later */
			mo->mlfirst = onbounds(mn->mlfirst,mn->mlsize,osstart);
			mo->mllast = mn->mllast;
			mo->mlsize = mn->mlsize;
			mo->mlnext = mn->mlnext;
			mn->mlnext = mo;
		    }
		    if( (zz=trimit(mn->mlfirst,osstart-1,mn->mlsize)) > 0 ) {
			mn->mllast = osstart-1;
			mn->mlsize = zz;	/* in case it was trimmed */
		    } else {		/* partitions too small, delete it */
			*mm = mn->mlnext;
			free(mn);
			continue;	/* don't increment list ptr */
		    }
		}			/**** end collision ****/

	    } else if( mn->mlfirst < osend ) { /** collision **/
		++adjusts;		/* we're doing some adjusting */
		if( mn->mllast <= osend ) {	/* does part cross past os? */
		    *mm = mn->mlnext;	/* no, delete it */
		    free(mn);
		    continue;
		}
		ss = onbounds(mn->mlfirst,mn->mlsize,osend);
		if( (zz=trimit(osend,ss,mn->mlsize)) > 0 ){/* set up a smaller mem part */
		    mo = (MLIST *)malloc(sizeof *mo);
		    mo->mlfirst = osend+1;
		    mo->mllast = ss;
		    mo->mlsize = zz;
		    mo->mlnext = mn;
		    *mm = mo;
		    mm = &mo->mlnext;
		}
		if( (zz=trimit(ss,mn->mllast,mn->mlsize)) > 0 ){
		    mn->mlfirst = ss;
		    mn->mlsize = zz;
		} else {		/* partition too small, delete */
		    *mm = mn->mlnext;
		    free(mn);
		    continue;		/* don't bottom out on this loop */
		}
	    }				/**** end collision ****/

	    mm = &mn->mlnext;		/* set up for next partition check */
	}				/**** end FOR loop ***/
	return adjusts>0;		/* returns TRUE if we adjusted */
}

    WORD		/* returns new size of partition, 0 if not enough room */
trimit(start,last,olds)
    WORD start;				/* start address of partition */
    WORD last;				/* end address of partition */
    WORD olds;				/* old partition size */
{
	REG WORD ss;

	ss = last-start;		/* size is in paragraphs */
	if( ss< 0 ) return 0;		/* negative size */
	if( ss<MINKMEM ) return 0;	/* sorry, too small */
	else if( ss<olds ) return ss-1;	/* needs to be smaller */
	else return olds;		/* fine just the way it is */
}

    WORD
onbounds(start,size,nst)
    WORD start;				/* original starting boundary */
    WORD size;				/* partition size */
    WORD nst;				/* new starting address */
{					/* returns addr>nst multiple of size from start */
	REG WORD rr;

	for( rr=start; rr<nst; rr += size )
		;
	return rr;
}




/*********************************************************************/
/*********************************************************************/
/* edsgen.c
/* EXPORTS gentable() - the routine which builds all the tables
/*********************************************************************/
/*********************************************************************/
#ifndef MAINMODULE
#include <genccpm.h>
#endif


MLOCAL UWORD rsp_last;		/* link to previous RSP */
MLOCAL UWORD rsp_seg;		/* where next RSP seg gets written */

#define ALIGN(xx) (((xx)+0xF)&0xFFF0)
#define BYTE_ADJUST(yy) if(yy>0xFF) yy=0xFF


/*************************************************************************/
    UWORD				/* return new offset */
segwrite(nbs)				/* pad to paragraph boundary */
    UWORD nbs;				/* old offset */
{
	REG UWORD nps;

	if( nbs & 0xF ) {		/* does it need padding? */
		nps = 0x10 - (nbs & 0xF);	/* number pad bytes needed */
		write(fns,"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",nps);
		return (nbs+nps);
	} else	return nbs;
}

    UWORD				/* return num paragraphs */
padwrite(nbs)				/* pad to paragraph boundary */
    UWORD nbs;				/* num bytes needing padding */
{
	return segwrite(nbs) >> 4;		/* convert to pgphs */
}

    UWORD
datwrite(from,size)
    BYTE *from;
    WORD size;
{
	if( write(fns,from,size) != size ) {
	    fprintf(stderr,"ERROR: Write to new SYS file failed\n");
	    fprintf(stderr,"Out of room on disk?\n");
	    exit(1);
	}
	return size;
}

/**************************************************************************/
MLOCAL LONG oldladdr=0;		/* save previous address for comparison */
#define BIGWORD 0x0000ffffL
chkaddr(uaddr)			/* check for SYSDAT overflow */
    UWORD uaddr;
{
	LONG laddr;

	laddr = BIGWORD & uaddr;
	if( laddr < oldladdr ) {
	    USERR("ERROR - System Data Area has grown too large\n");
	    USERR("Try reducing the size of some of the tables\n");
	    exit(1);
	}
	oldladdr = laddr;	/* save for wrap around compare */
}
	/* this is a kluge: I did it because I couldn't get the LONG cast */
	/* to work without doing sign extension... whf 1/20/83 */


/**************************************************************************/
/* EXPORT gentables: generates various tables in system data area,
/*	and handles RSPs
/**************************************************************************/
VOID gentables()
{
	LOCAL WORD nmparts;		/* number of memory partitions */
	LOCAL WORD nsat_items;		/* number of SAT items in qmau */
	LOCAL UWORD damds;		/* offset of beginning of tables */
	LOCAL UWORD daqbuf;		/* offset of beginning of qbuf */
	LOCAL LONG daend;		/* sys data offset: end of data area */
	LOCAL UWORD doffset;		/* current offset into data area */
	LOCAL UWORD lastlink;		/* helps link lists together */
	LOCAL WORD count;		/* counts num list items being generated */
	LOCAL UWORD ucount;		/* unsigned counter */
	LOCAL MLIST *mn;		/* for trapsing down memroot */
	LOCAL WORD nump;		/* local number partitions */
	LOCAL UWORD saddr;		/* local starting addr */
	LOCAL WORD length;		/* local partition length */
	BYTE buf[SECSIZ];		/* an area for writing */
	LOCAL LONG dmagic;		/* fixup location for xt_alloc info */
	UWORD genmfl();			/* gen a memory free list */
	UWORD genxios();		/* go gen the xios disk buffers */
	DLIST *rs;			/* ptr to rsp list */
	MD md;				/* sample memory descriptor */
	PD pd;				/*   "    process descriptor */
	QCB qcb;			/*   "    queue control block */
	CCB ccb;			/*   "    channel control block */
	LOCK lock;			/*   "    lock list item */
	FLAG flg;			/*   "    flag item */
	SATITEM sat;			/*   "    sub allocation table item */

	if(verbose) printf("Generating tables\n");
	FCHECK(fns);			/* File error check */

	/* pre-calculate certain vars */
	bufs_seg = 0;			/* total buf segs allocated in rsvd_seg */
	if( (nmparts=cntmlist(memroot)) <= 0 ) 
		CRASH("invalid memory list, aborting\n");
	totpds = xtrapds + nmparts;	/* 1 pd per memory partition, plus xtras */
	BYTE_ADJUST(totpds);
	totmds = (3*totpds) + nmparts + (nmparts>>1); /* 3 mds per process, plus */
					/* 1 per partition, plus extra for */
					/* memory allocator worst case */
	BYTE_ADJUST(totmds);
	nsat_items = (1+ (1+2*totqcbs)); /* 1 for header entry plus worst case */

	dltotal = dlsysdat+dlxios;	/* keep track of total data length in pgphs */
	doffset = dltotal << 4;		/* calculate current data offset */
	dsotables = doffset;		/* mark the start of tables */
	chkaddr(doffset);		/* init the function */

					/* MEMORY FREE LIST */
	locmfl = lseek(fns,0L,1);	/* remember where we are writing this */
	doffset = genmfl(doffset);	/* gen the mem free list */

	lastlink = 0;			/* UNUSED MEMORY DESCRIPTOR LIST */
	zfill(&md,sizeof md);		/* init the mem descriptor */
	for( count=totmds-nmparts; count>0; --count ) {	/* generate rest of mds */
	    md.md_link = lastlink;
	    lastlink = doffset;
	    doffset += datwrite(&md,sizeof(md));
	}
	*sd_mdul = lastlink;		/* store ptr to mds unused */

	lastlink = 0;			/* UNUSED PROCESS DESCRIPTOR LIST */
	zfill(&pd,sizeof(pd));
	for( count=totpds; count>0; --count ) {
	    pd.pd_link = lastlink;
	    lastlink = doffset;
	    doffset += datwrite(&pd,sizeof(pd));
	}
	*sd_pul = lastlink;		/* store ptr to pds unused */

	lastlink = 0;			/* UNUSED QUEUE CONTROL BLOCKS */
	zfill(&qcb,sizeof(qcb));
	for( count=totqcbs; count>0; --count ) {
	    qcb.qc_link = lastlink;
	    lastlink = doffset;
	    doffset += datwrite(&qcb,sizeof(qcb));
	}
	*sd_qul = lastlink;		/* store ptr to unused qcb list */

	lastlink = 0;			/* UNUSED LOCKED ITEMS LIST */
	zfill(&lock,sizeof(lock));
	for( count=totopen; count>0; --count ) {
	    lock.lo_link = lastlink;
	    lastlink = doffset;
	    doffset += datwrite(&lock,sizeof(lock));
	}
	*sd_lul = lastlink;		/* store ptr to unused lock items list */

#ifdef MPM
	*sd_ccb = doffset;		/* CHANNEL CONTROL BLOCK TABLE */
	zfill(&ccb,sizeof(ccb));
	ccb.cc_mimic = ccb.cc_msource = 0xFF;
	for( count= *sd_nccb; count>0; --count ) {
	    doffset += datwrite(&ccb,sizeof(ccb));
	}
#endif
#ifdef CCPM
	*sd_ncns = xt.xt_nvcns;
	*sd_ncondev = xt.xt_nccbs;
	*sd_ccb = xt.xt_ccb;
	*sd_nlst = xt.xt_nlcbs;
	*sd_nlstdev = xt.xt_nlcbs;
	*sd_lcb = xt.xt_lcb;
	*sd_nciodev = *sd_ncondev + *sd_nlstdev;
	*sd_tickspsec = xt.xt_ticks_sec;
#endif

	*sd_flags = doffset;		/* FLAG TABLE */
	flg.fl_status = -1;
	flg.fl_pd = -1;
	for( count= *sd_nflags; count>0; --count ) {
	    doffset += datwrite(&flg,sizeof(flg));
	}


#ifdef CCPM
	doffset = genxios(doffset);	/* go gen the xios buffering info */
#endif
	chkaddr(doffset);		/* is table space too big? */

	doffset = segwrite(doffset);	/* Q Sub Alloc Tab HEADER */
	*sd_qmastart = dsstart + (doffset>>4);	/* fill in seg addr */
	daqbuf = doffset + sizeof(sat)*nsat_items;
	zfill(&sat,sizeof(sat));
	sat.sa_start = --nsat_items;	/* decr before write */
	doffset += datwrite(&sat,sizeof(sat));	/* write the header */
					/* Q Sub Alloc Tab ENTRIES */
	sat.sa_start = daqbuf;
	sat.sa_length = qbuflen;	/* byte value */
	*sd_qmalen = qbuflen>>4;	/* pgph value */
	doffset += datwrite(&sat,sizeof(sat));	/* write the first sat entry */

	zfill(&sat,sizeof(sat));
	for( count= --nsat_items; count>0; --count ) {
	    doffset += datwrite(&sat,sizeof(sat));/* write the rest of the sat entries */
	}

	zfill(buf,SECSIZ);		/* Q BUFFER AREA - may go past 64K */
	daend = doffset;		/* abandon doffset */
	for( ucount=qbuflen; ucount>0; ucount -= length ) {
	    length = (ucount>SECSIZ ? SECSIZ : ucount);
	    daend += datwrite(buf,length); 
	}
	doffset = daend & 0xF;		/* get low nibble */
	daend = (daend >> 4) + padwrite(doffset); /* cvt to pgphs */

	dltables = daend - dltotal;	/* compute length of tables in pgphs */
	dltotal = daend;		/* keep track of total data length */

	/*** now fill in the rsps ***/
	if(verbose) printf("Appending RSPs to system file\n");
	FCHECK(fns);			/* File error check */
	rsp_last = 0;
	rsp_seg = dsstart + daend;
	for( rs=rsps; rs!=NULLPTR; rs=rs->dlnext ) /* follow the rsp list */
	    xfer_rsp(rs->dlname);
	*sd_rspseg = rsp_last;
	dlrsps = rsp_seg - (dsstart + daend);
	dltotal += dlrsps;
	rsvd_seg = rsp_seg;
#ifdef CCPM
	length = xt.xt_alloc;		/* how much memory is XIOS requesting */
	xt.xt_alloc = rsvd_seg+bufs_seg; /* this is the place they can use */
	dmagic = fldstart;		/* location of SYSDAT in SYS file */
	dmagic += XTABALLOC;		/* add in where xt_alloc goes to */
	fixlater(dmagic,F_PUT,&(xt.xt_alloc));	/* fix this number later on */
	bufs_seg += length;		/* allocate requested space */
#endif
	*sd_endseg = rsvd_seg+bufs_seg;	/* this is where we reserve to */
}


/**************************************************************************/
/* genmfl: writes out a memory free list, assuming file 'fns' is seeking
/*	correctly into file.
/**************************************************************************/
    UWORD
genmfl(doffset)
    UWORD doffset;
{
	REG MLIST *mn;			/* ptr into EDSYS mem list */
	LOCAL WORD nump;
	LOCAL UWORD lastlink, length, saddr; /* counter vars */
	MD md;				/* mem desc struct */

	*sd_mfl = doffset;		/* gen this list forward */
	zfill(&md,sizeof(md));		/* init the data structure */
	for( mn=memroot; mn!=NULLPTR; mn=mn->mlnext ) {
	    nump = (mn->mllast - mn->mlfirst) / mn->mlsize;
	    saddr = mn->mlfirst;	/* use this starting address */
	    length = md.md_length = mn->mlsize; /* size in pgphs */
	    for( ; nump>0; --nump ) {	/* for each partition */
		doffset += sizeof(md);	/* incr by sizeof object */
		lastlink = doffset;	/* record address of where this will go */
		if( nump == 1 )	{	/* on this group's last partition */
		    length = md.md_length = mn->mllast-saddr; /* stick in rmdr */
		    if( mn->mlnext == NULLPTR ) /* on the very last partition */
			lastlink=0;	/* set end of list */
		}
		md.md_link = lastlink;	/* thread the list */
		md.md_start = saddr;	/* set the starting addr */
		write(fns,&md,sizeof(md));	/* put it into the NEWSYS file */
		saddr += length;	/* next md starts here */
	    }
	}				/* finished with memory partitions */
	return doffset;
}


/**************************************************************************/
    VOID
zfill(addr,num)
    BYTE *addr;
    WORD num;
{
	for( ; num>0; --num )
		*addr++ = '\0';
}


/**************************************************************************/
/**************************************************************************/
#define RSPHDR struct rsp_header
RSPHDR {
	UWORD rs_link;			/* link to other RSPs */
	WORD rs_sdatvar;		/* SYSDAT offset: num copies */
	BYTE rs_ncopies;		/* local: num copies */
	BYTE rs_space[11];		/* fill up rest of header */
};

RSPHDR *rhdr;				/* RSP's header */
PD *rpd;				/* RSP's process descriptor area */
UDA *ruda;				/* RSP's uda area */
#define RHDRSIZE (sizeof(RSPHDR)+sizeof(PD)+sizeof(UDA))
#define RHDRPGPHS (RHDRSIZE>>4)
MLOCAL BYTE rh[RHDRSIZE];		/* where to read the RSP header */
MLOCAL BYTE rspnbuf[9];			/* room for copy of pd name */
MLOCAL BYTE *rspname;			/* name of RSP */


/**************************************************************************/
xfer_rsp(rspnm)
    BYTE *rspnm;			/* name of rsp */
{
	LOCAL WORD fr;			/* rsp file descriptor */
	LOCAL WORD ncs;			/* number of copies */

	rhdr = rh;			/* init these vars */
	rpd = rh+sizeof(*rhdr);		/* points to process descriptor */
	ruda = rh+sizeof(*rhdr)+sizeof(*rpd);	/* points to user data area */
	if( (fr=BROPEN(rspnm)) < 0 ) {
	    printf("can't open RSP %s\n",rspnm);
	    exit(1);
	}
	if( grpseek(fr,GTYPDATA) > 0L){	/* is there a data group? */
	    ncs = gwab_hdr(fr);
	    if( rpd->pd_mem == 0 )
		 xfer_separate_rsp(fr,ncs);
	    else xfer_pure_rsp(fr,ncs);
	} else {
	    grpseek(fr,GTYPCODE);
	    ncs = gwab_hdr(fr);
	    if( rpd->pd_mem == 0 )
		 xfer_mixed_rsp(fr,ncs);
	    else {
		USERR("RSP %s has non zero MEM field and no data group\n",
			rspnm);
	    }
	}
	close(fr);
}


/**************************************************************************/
    WORD
gwab_hdr(fr)
    WORD fr;
{
	REG WORD rv;			/* return val */
	EXTERN BYTE sdat_area[];

	rspname = NULLPTR;		/* blank out this name */
	read(fr,rh,RHDRSIZE);
	strncpy(rspnbuf,rpd->pd_name,8);/* store this in case we need it */
	if( rhdr->rs_sdatvar != 0 ) {
		rspname = rspnbuf;
		rv = sdat_area[rhdr->rs_sdatvar] - 1;
	} else {
	    if( rhdr->rs_ncopies > 0 )
		rspname = rspnbuf;
	    rv = rhdr->rs_ncopies;
	}
	return rv;
}


/**************************************************************************/
xfer_separate_rsp(fr,ncs)		/* (many) data and (many) code segs */
    WORD fr;				/* file desc of RSP file */
    WORD ncs;				/* number of copies */
{
	WORD ii;			/* counter */

	for( ii=0; ii<=ncs; ++ii) {
	    ruda->ud_dsinit = ruda->ud_esinit = ruda->ud_ssinit = rsp_seg;
	    ruda->ud_csinit = rsp_seg + (grpseek(fr,GTYPDATA)>>4);
	    rhdr->rs_ncopies = ii;
	    if( rspname ) 		/* do we need to insert a modfied name? */
		putname(rpd->pd_name,rspname,ii);
	    rhdr->rs_link = rsp_last;	/* link together the list */
	    rsp_last = rsp_seg;		/* remember this for next time */
	    write(fns,rh,RHDRSIZE);	/* write out the RSP header */
	    rsp_seg += RHDRPGPHS + xferpart_grp(fr,GTYPDATA,RHDRPGPHS);
	    rsp_seg += xferpart_grp(fr,GTYPCODE,0);
	}
}


/**************************************************************************/
xfer_pure_rsp(fr,ncs)			/* (many) data segs and one code segs */
    WORD fr;				/* file desc of RSP file */
    WORD ncs;				/* number of copies */
{
	WORD ii;			/* counter */

	ruda->ud_csinit = rsp_seg + (1+ncs) * (grpseek(fr,GTYPDATA) >> 4);
	for( ii=0; ii<=ncs; ++ii) {
	    ruda->ud_dsinit = ruda->ud_esinit = ruda->ud_ssinit = rsp_seg;
	    rhdr->rs_ncopies = ii;
	    if( rspname ) 		/* do we need to insert a modfied name? */
		putname(rpd->pd_name,rspname,ii);
	    rpd->pd_mem = 8; 		/* magic ptr to md in rsphdr */
	    rhdr->rs_link = rsp_last;	/* link together the list */
	    rsp_last = rsp_seg;		/* remember this for next time */
	    write(fns,rh,RHDRSIZE);	/* write out the RSP header */
	    rsp_seg += RHDRPGPHS + xferpart_grp(fr,GTYPDATA,RHDRPGPHS);
	}
	rsp_seg += xferpart_grp(fr,GTYPCODE,0);
}


/**************************************************************************/
xfer_mixed_rsp(fr,ncs)			/* (many) mixed code/data segs */
    WORD fr;				/* file desc of RSP file */
    WORD ncs;				/* number of copies */
{
	WORD ii;			/* counter */

	for( ii=0; ii<=ncs; ++ii) {
	    ruda->ud_csinit = ruda->ud_dsinit = ruda->ud_esinit = 
		ruda->ud_ssinit = rsp_seg;
	    rhdr->rs_ncopies = ii;
	    if( rspname ) 		/* do we need to insert a modfied name? */
		putname(rpd->pd_name,rspname,ii);
	    rhdr->rs_link = rsp_last;	/* link together the list */
	    rsp_last = rsp_seg;		/* remember this for next time */
	    write(fns,rh,RHDRSIZE);	/* write out the RSP header */
	    rsp_seg += RHDRPGPHS + xferpart_grp(fr,GTYPCODE,RHDRPGPHS);
	}
}


/**************************************************************************/
putname(to,from,num)
    BYTE *to;
    BYTE *from;
    WORD num;
{
	BYTE buf[15];
	BYTE *bp;
	WORD jj;

	for( bp=buf, jj=0; *from  &&  *from != ' '  &&  jj<8; ++jj )
		*bp++ = *from++;
	if( jj>6 )
		bp = buf+7;
	sprintf(bp,"%x      ",num);
	for( bp=buf, jj=0; jj<8; jj++ )
		*to++ = *bp++;
}


/************************************************************************/
/* dolbl: modify operating system version label */
/************************************************************************/
#define MAXLBL 80

    BYTE *
dolbl(txt,lbtitle)
    BYTE *txt;				/* cmd val */
    BYTE *lbtitle;
{
	DLIST *sl, *psl;		/* ptrs to system labels */
	BYTE mb[MAXLBL];		/* max size of each line */
	EXTERN BYTE *clearit;

	sl = psl = syslbls;		/* init ptrs */
	printf("%s\n%s\n",clearit,lbtitle);
	printf("Current message is:\n");
	if( syslbls==NULLPTR )
		printf("<null>\n");
	else for( ; sl != NULLPTR; sl = (psl=sl)->dlnext ) 
		printf("%s\n",sl->dlname);
	printf("\nAdd lines to message.  Terminate by entering only RETURN:\n");
	FOREVER {
	    if( gets(mb) != mb  ||  *mb==NULL )
		break;
	    sl = (DLIST *)malloc(sizeof(*sl));	/* alloc a list item */
	    sl->dlname = malloc(sizeof(mb));
	    strcpy(sl->dlname,mb);	/* copy message to new space */
	    sl->dlnext = NULLPTR;
	    if( psl==NULLPTR )		/* new list? */
		syslbls = psl = sl;	/* this will be the first line */
	    else {
		psl->dlnext = sl; 
		psl = psl->dlnext; 
	    }
	}
	return NULLPTR;
}

/************************************************************************/
/* writelbl: writes the OS label to the NEWSYS file, returning the length
/*	of the label (in pgphs)
/************************************************************************/
    WORD
_wrlbl(str)				/* subroutine to writelbl */
    BYTE *str;
{
	REG WORD slen;

	slen=strlen(str);		/* length of string to be written */
	write(fns,str,slen);		/* write string to file NEWSYS */
	write(fns,"\015\012",2);	/* cr,lf */
	return slen+2;
}

    WORD
writelbl()				/* returns num pgphs written */
{
	REG WORD cc;			/* count of chars output */
	DLIST *sl;			/* list ptr */

	cc = 0;

	cc += _wrlbl("");		/* crlf */
	cc += _wrlbl(version);		/* write out the version label */
	cc += _wrlbl(copyright);	/* followed by copyright notice */
	for( sl=syslbls; sl!=NULLPTR; sl=sl->dlnext )
	    cc += _wrlbl(sl->dlname);	/* followed by all lines in OS label */
	cc += _wrlbl("$");		/* terminate message for printstr fn */
	return padwrite(cc);		/* writes enough zeroes to pad to pgph bdry */
}


/***************************************************************************/
/* fixup routines: remembers and posthumously patches the SYS file
/***************************************************************************/

FIX *fixroot = NULLPTRI;		/* list of fixes */

fixlater(fa,ft,fp)			/* records the fix in the list */
    LONG fa;				/* addr within NEWSYS to fix */
    WORD ft;				/* type of fix: F_ADD or F_PUT */
    WORD *fp;				/* ptr to val used in fixing */
{
	FIX *fx;

	fx = (FIX *)malloc(sizeof *fx);	/* alloc a list item */
	fx->f_addr = fa;
	fx->f_type = ft;
	fx->f_ptr = fp;
	fx->f_next = fixroot;		/* insert at head of list */
	fixroot = fx;
}

fixfile()				/* does the fixes in the file */
{
	FIX *fx;
	WORD fval;			/* the value to stuff */
	LONG lastval;

	lastval=lseek(fns,0L,2);

	for( fx=fixroot; fx!=NULLPTR; fx = fx->f_next ) {
	    if(fx->f_addr > lastval) {
		printf("fixfile: %X %x %x\n",fx->f_addr,fx->f_type,fx->f_ptr);
		continue;
	    }
	    if( lseek(fns,fx->f_addr,0) < 0L )
		CRASH("fixup seek failure\n");
	    if( fx->f_type == F_ADD ) {	/* F_ADD the value to what's there */
		read(fns,&fval,2);
		fval += *(fx->f_ptr);
		lseek(fns,-2L,1);	/* backup to write */
	    } else			/* must be a F_PUT */
		fval = *(fx->f_ptr);
	    write(fns,&fval,2);
	}
}




/************************************************************************/
/************************************************************************/
/* edsdisk - This module exports the following routines:
/*
/* initxios_info(xf,goff)	'xf' is XIOSMOD file descriptor, 'goff'
/*	is offset of data group in XIOSMOD (including header);  
/*	this module inits the buffering info questions.
/* doxbufs(txt,title)	'txt' is a command, 'title' is the prompt for this fn;
/*	this routine is called within the MENU module, and prompts user
/*	for xios info.
/* xiosvalid()  returns a BOOLEAN value indicating whether the xios info
/*	is valid.
/* genxios(doffset)  generates the buffers for the xios, starting at 'doffset'
/*	in the system data area.
/* fixupxios_info()	writes DPH fixups required by genxios
/**************************************************************************/
/**************************************************************************/
#ifndef MAINMODULE
#include <genccpm.h>
#endif


/**************************************************************************/
/* edsdisk.h : defines the data structures used in this module
/**************************************************************************/

#define ADDR UWORD			/* Address (offset) within SysDat */
#define SEG  UWORD			/* Segment address (may be outside SysDat) */

#define DPH struct dph_item
DPH {					/* Disk Parameter Header structure */
	ADDR dph_xlt;			/* Translation Table Address */
	BYTE dph_space[6];		/* reserved & Media Flag */
	ADDR dph_dpb;			/* Disk Parameter Block Address */
	ADDR dph_csv;			/* CheckSum Vector Address */
	ADDR dph_alv;			/* Allocation Vector Address */
	ADDR dph_dirbcb;		/* Dir Buffer Control Block Header Address */
	ADDR dph_datbcb;		/* Data Buffer Control Block Header Address */
	SEG  dph_hstbl;			/* Hash Table Segment */
};

#define DPB struct dpb_item
DPB {					/* Disk Parameter Block structure */
	WORD dpb_spt;			/* Sectors Per Track */
	BYTE dpb_bsh;			/* Allocation Block Shift Factor */
	BYTE dpb_blm;			/* Allocation Block Mask */
	BYTE dpb_exm;			/* Extent Mask */
	WORD dpb_dsm;			/* Disk Storage Maximum */
	WORD dpb_drm;			/* Directory Maximum */
	WORD dpb_dav;			/* Directory Allocation Vector */
	WORD dpb_cks;			/* Checksum Vector Size */
	WORD dpb_off;			/* Track Offset */
	WORD dpb_psh;			/* Physical Record Shift Factor */
	WORD dpb_phm;			/* Physical Record Mask */
};

#define BCBH struct bcbh_item
BCBH {					/* Buffer Control Block Header structure */
	ADDR bcbh_lr;			/* BCB List Root */
	BYTE bcbh_pm;			/* BCB Process Max */
};

#define BCB struct bcb_item
BCB {					/* Buffer Control Block structure */
	BYTE bcb_drv;			/* Drive */
	BYTE bcb_record[3];		/* Record Number */
	BYTE bcb_wflg;			/* Write Pending Flag */
	BYTE bcb_seq;			/* Sequential Access Counter */
	WORD bcb_track;			/* Logical Track Number */
	WORD bcb_sector;		/* Logical Translated Sector Number */
	UWORD bcb_bufptr;		/* Buffer Offset (for DIRBCB) / Segment (for DATBCB) */
	ADDR bcb_link;			/* Link to next BCB */
	WORD bcb_pdadr;			/* Process Descriptor Address */
};


#define DINFO struct dirbuf_information
DINFO {
	BOOLEAN d_modified;		/* DPH was modified? */
	BOOLEAN d_hashing;		/* user wants hashing? */
	WORD d_ndirbs;			/* Num dir buffers */
	WORD d_sdirbs;			/* Size dir buffers */
	WORD d_pmdirbs;			/* Process Max Number Dir Buffs */
	WORD d_ndatbs;			/* Num data buffers */
	WORD d_sdatbs;			/* Size data buffers */
	WORD d_pmdatbs;			/* Process Max Number Dir Buffs */
	WORD d_shash;			/* Size of hash table */
	WORD d_alvsize;			/* size of Alloc Vector area (in bytes ) */
	WORD d_blksize;			/* size of disk Allocation Block */
	WORD d_csvsize;			/* size of Checksum Vector area */
};

/* end of edsdisk.h ********************************************************/



/***************************************************************************/
WORD csfd;				/* command file descriptor */
WORD csgoff;				/* group offset within command file */

cseek(offs)				/* seek offset within Command File */
    ADDR offs;
{
	lseek(csfd,(LONG) offs+csgoff, 0);	/* compensate for cmd header */
}

/***************************************************************************/
/* following is temporary!! change to zero after debugging */
#define ZBYTE (doffset&0xFF)		/* the byte usually passed in */
    WORD
bufwrite(size,byt)
    WORD size;
    BYTE byt;
{
	WORD ss;

	for( ss=size; ss>0; --ss )
		write(fns,&byt,1);
	return size;
}


/***************************************************************************/
/* Module local variables */
#define DPHNUM 16			/* number of possible disks */
ADDR *dphtab;				/* table of offsets of Disk Parm Hdrs */
DPH *dh[DPHNUM];			/* table of Disk Parm Hdrs */
DINFO *di[DPHNUM];			/* table of info on DPHs */


/* Module local predicates */
#define ASKFOR(aa) ((aa) == 0xFFFF)	/* Is this one we're sposed to ask for? */
#define SHRMSK 0x8000			/* Is this shared with another drive? */
#define SHARED(bb) ((bb)!=0xFFFF  &&  (bb) & SHRMSK)	/* Is this nd??bs field shared with another disk? */
#define GETSHARED(cc) ((cc) & ~SHRMSK)	/* Get the drive we share with */



/***************************************************************************/
/* EXPORT
/* initxios_info: gets whatever information GENSYS needs from xios
/***************************************************************************/
initxios_info(xf,goff)			/* get info from xios file */
    WORD xf;				/* xios file descriptor */
    WORD goff;				/* group offset within xios file */
{
	REG WORD ii;			/* counter */
	DPB dd;				/* a place to keep this info */
	DPH *dph;
	DINFO *dinf;

	csfd = xf;			/* assign to global: set up cseek */
	csgoff = goff;			/* assign to global */
	cseek(XTABLOC);			/* go to where the XIOSTAB info is */
	read(xf,&xt,sizeof xt);		/* read in the xios info */
	dphtab = xt.xt_dphtab;		/* point to disk parm header table */

	for( ii=0; ii<DPHNUM; ++ii ) {	/* for each dph pointer */
	    if( dphtab[ii] == 0 ) {	/* is there a dph? */
		dh[ii]=NULLPTR;
		di[ii]=NULLPTR;		/* no tickee, no washee */
	    } else {			/* aha! let's set up this info */
		dh[ii] = (DPH *)malloc(sizeof(DPH));
		di[ii] = (DINFO *)malloc(sizeof(DINFO));	
		zfill(di[ii],sizeof(DINFO));
		cseek(dphtab[ii]);
		read(xf,dh[ii],sizeof(DPH));
	    }
	}				/* end of reading dph */
	for( ii=0; ii<DPHNUM; ++ii ) 	/* let's go thru it again */
	    if( dh[ii] ) {		/* if there's something there */
		dph = dh[ii];
		dinf = di[ii];
		cseek(dph->dph_dpb);	/* point to the disk parm block */
		read(xf,&dd,sizeof(DPB));
		dinf->d_blksize = 128 << dd.dpb_psh;
		dinf->d_csvsize = 0x7FFF & dd.dpb_cks;
		dinf->d_alvsize = (dd.dpb_dsm/8 + 1) * 2;
		dinf->d_shash = 4 * (1 + dd.dpb_drm);
		dinf->d_ndirbs = -1;	/* ask for these by name! */
		dinf->d_ndatbs = -1;
		dinf->d_hashing = TRUE;	/* default to hashing */
	    }				/* end of drive info init */
}



/************************************************************************/
/* EXPORT
/* doxbufs: queries user for BDOS buffering info;
/*	exits only when all data valid
/************************************************************************/
    BYTE *
doxbufs(txt,bdtitle)
    BYTE *txt;
    BYTE *bdtitle;
{
	BYTE cmds[CMDSLEN];
	BOOLEAN xiosvalid();
	EXTERN BYTE *clearit;

	FOREVER {
	    printf("%s",clearit);	/* clear screen */
	    printf("%s\n",bdtitle);
	    dbufs_display();			/* display */
	    printf("Drive (<CR> to exit) ? ");
	    if( gets(cmds)!=cmds || *cmds==NULL ) {	/* they hit <CR>? */
		if( xiosvalid() ) break;
		USERR("Please correct drive buffers information\n");
		press_return();
		continue;
	    }
	    askabout(cmds);		/* go do what they want */
	}
	return NULLPTR;			/* error msg for menu handler */
}

/************************************************************************/
/* dbufs_display: display disk buffering info 
/************************************************************************/
dbufs_display()
{
	REG WORD ii, jj, tot;		/* counters */
	DPH *dph;
	DINFO *dinf;

	tot = 0;			/* init this */
	dcalcsizes();			/* check all the sizes */
	printf("\n\t*** Disk Buffering Information ***\n");
	printf("      Dir  Max/Proc   Data Max/Proc   Hash   Specified\n");
	printf("Drv   Bufs Dir Bufs   Bufs Dat Bufs   -ing   Buf Pgphs\n");
	printf("===   ==== ========   ==== ========   ====   =========\n");
	for( ii=0; ii<DPHNUM; ++ii ) {	/*** for each drive ***/
	    if( dphtab[ii]==0 ) continue; /* no DP Hdr for this drive */
	    dph = dh[ii]; dinf = di[ii];
	    printf(" %c:   ",'A'+ii);	/* print which drive */
	    dspdnums(dph->dph_dirbcb,dinf->d_ndirbs,dinf->d_pmdirbs);
	    dspdnums(dph->dph_datbcb,dinf->d_ndatbs,dinf->d_pmdatbs);
	    if( ! ASKFOR(dph->dph_hstbl) )
		 printf(" fixed  ");
	    else if( dinf->d_hashing )
		 printf(" yes    ");
	    else printf(" no     ");
	    jj = dspdsize(dph,dinf);
	    if( jj == -2 )
		 printf(" fixed   ");
	    else if( jj == -1 )
		 printf(" ??      ");
	    else {
		printf(" %4.4x   ",jj >> 4);
		tot += jj;
	    }
	    printf("\n");
	}				/*** end for each drive ***/
	printf("Total paragraphs allocated to buffers: %x\n",tot >> 4);
}


/***************************************************************************/
/* dspdsize: calculates a drive's user-requestable buffer size, in bytes
/***************************************************************************/
    WORD
dspdsize(dph,dinf)			/* calculate size requested */
    DPH *dph;				/* returns -1 if still needs info */
    DINFO *dinf;			/* returns -2 if pre-allocated */
{
	REG WORD siz;

	siz=0;
	if( !ASKFOR(dph->dph_dirbcb)  &&
	    !ASKFOR(dph->dph_datbcb)  &&
	    !ASKFOR(dph->dph_hstbl) )
		return -2;		/* nothing to say here */
	if( ASKFOR(dph->dph_dirbcb) ){
	    if( ASKFOR(dinf->d_ndirbs) )
		return -1;
	    if( !SHARED(dinf->d_ndirbs) )
		siz += dinf->d_sdirbs * dinf->d_ndirbs;
	}
	if( ASKFOR(dph->dph_datbcb) ){
	    if( ASKFOR(dinf->d_ndatbs) )
		return -1;
	    if( !SHARED(dinf->d_ndatbs) )
		siz += dinf->d_sdatbs * dinf->d_ndatbs;
	}
	if( ASKFOR(dph->dph_hstbl) ){
	    if( dinf->d_hashing )
		siz += dinf->d_shash;
	}
	return siz;
}


/***************************************************************************/
/* dspdnums: displays the data/dir buf numbers
/***************************************************************************/
dspdnums(nfix,ninput,npm)		/* display subroutine */
    WORD nfix;
    WORD ninput;
    WORD npm;
{
	if( !ASKFOR(nfix) )
	    printf("fixed           ");
	else if( SHARED(ninput) )
	    printf("shares %c:       ",'A'+GETSHARED(ninput));
	else {
	    if( ASKFOR(ninput) )
		 printf(" ??  ");
	    else printf(" %2.2x  ",ninput);
	    if( ASKFOR(npm) )
		 printf("  ??       ");
	    else printf("  %2.2x       ",npm);
	}
}


/***************************************************************************/
/* dcalcsizes: calculates buffer sizes across shared buffers
/***************************************************************************/
dcalcsizes()			/* make sure all the sizes are correct */
{
	REG WORD ii, jj;
	REG WORD *pkk;
	DPH *dph;
	DINFO *dinf;

	for( ii=0; ii<DPHNUM; ++ii )	/* init all sizes to zero */
	    if(dphtab[ii]) {
		dinf = di[ii];
		dinf->d_sdirbs = dinf->d_sdatbs = 0;
	    }
	for( ii=0; ii<DPHNUM; ++ii )		/* for each drive */
	    if(dphtab[ii]) {
		dph = dh[ii];
		dinf = di[ii];
		if( ASKFOR(dph->dph_dirbcb) ) 	/* max out the size of a dirbuf */
		    if( !ASKFOR(dinf->d_ndirbs) ) { /* if it's real data there */
			if( SHARED(dinf->d_ndirbs) ) {
			    jj = GETSHARED(dinf->d_ndirbs);
			    pkk = & ((di[jj])->d_sdirbs);
			} else 
			    pkk = & dinf->d_sdirbs;
			if( dinf->d_blksize > *pkk ) /* assign the maximum size found */
			    *pkk = dinf->d_blksize;
		    }
		if( ASKFOR(dph->dph_datbcb) ) 	/* max out the size of a data buf */
		    if( !ASKFOR(dinf->d_ndatbs) ) {
			if( SHARED(dinf->d_ndatbs) ) {
			    jj = GETSHARED(dinf->d_ndatbs);
			    pkk = & ((di[jj])->d_sdatbs);
			} else 
			    pkk = & dinf->d_sdatbs;
			if( dinf->d_blksize > *pkk )
			    *pkk = dinf->d_blksize;
		    }
	    }
}


/***************************************************************************/
/* ask user about a drive
/***************************************************************************/
askabout(drspec)			/* ask user about a drive */
    BYTE *drspec;
{
	REG WORD dr;
	DPH *dph;
	DINFO *dinf;
	BOOLEAN askhash();

	if( *drspec == '*' ) {
	    askstar();			/* get info, apply to all disks */
	    return;
	}
	dr = toupper(*drspec) - 'A';	/* which drive do they want to fix? */
	if( dr<0  ||  dr>=DPHNUM  ||  dphtab[dr]==0  ||  drspec[1] != ':') {
	    USERR("Problem with '%s':\n",drspec);
	    USERR("Please specify an existing drive between 'A:' and 'P:'\n");
	    press_return();
	    return;
	}
	dph = dh[dr];
	dinf = di[dr];
	if( dspdsize(dph,dinf) == -2 ) { /* is there anything to modify? */
	    USERR("All buffers for %c: are fixed within the XIOS module.\n",
		dr+'A');
	    USERR("You can't modify this fixed information in GENSYS.\n");
	    press_return();
	    return;
	}
	if( ASKFOR(dph->dph_dirbcb) ){
	    dinf->d_ndirbs = asknd(1,FALSE);	/* get directory info */
	    dinf->d_pmdirbs = askpm(1,dinf->d_ndirbs);
	}
	if( ASKFOR(dph->dph_datbcb) ){
	    dinf->d_ndatbs = asknd(2,(128==dinf->d_blksize)); /* get data info */
	    dinf->d_pmdatbs = askpm(2,dinf->d_ndatbs);
	}
	if( ASKFOR(dph->dph_hstbl) ){
	    dinf->d_hashing = askhash();
	}
}


/****************************************************************************/
/* askstar: asks buffering questions & applies to all ASKFOR/unanswered drives
/****************************************************************************/
askstar()
{
	REG WORD dr;
	DPH *dph;
	DINFO *dinf;
	WORD andirbs, andirpms, andatbs, andatpms;
	BOOLEAN ahash, askhash();

	andirbs = asknd(1,FALSE);	/* Number of directory buffers? */
	andirpms = askpm(1,andirbs);	/* Process Max Dir Bufs? */
	andatbs = asknd(2,FALSE);	/* Number of data buffers? */
	andatpms = askpm(2,andatbs);	/* Process Max Data Bufs? */
	ahash = askhash();		/* Hashing? */
	for( dr=0; dr<DPHNUM; ++dr )
	    if( dphtab[dr] ) {		/* for all existing drives */
		dph = dh[dr]; dinf = di[dr];
		if( ASKFOR(dph->dph_dirbcb) )
		    if( ASKFOR(dinf->d_ndirbs) ) {
			dinf->d_ndirbs = andirbs;
			dinf->d_pmdirbs = andirpms;
		    }
		if( ASKFOR(dph->dph_datbcb) )
		    if( ASKFOR(dinf->d_ndatbs) ) {
			dinf->d_ndatbs = andatbs;
			dinf->d_pmdatbs = andatpms;
		    }
		if( ASKFOR(dph->dph_hstbl) )
		    dinf->d_hashing = ahash;
	    }
}



/***************************************************************************/
/* asknd: ask the user for the number of dir/data buffers
/***************************************************************************/
    WORD
asknd(type,zero_ok)
    WORD type;				/* 1=dir, 2=data */
    BOOLEAN zero_ok;
{
	REG WORD val, jj;
	BYTE *name;
	BYTE cmds[CMDSLEN];
	DPH *dph;

	if( type==1 ) name="directory"; else name="data";
	FOREVER {
	    printf("Number of %s buffers, or drive to share with ? ",name);
	    gets( cmds );
	    if( cmds[1] != ':' ) {
		val = atoih(cmds);
		if( val > 0  &&  val <= 127 ) break;
		    if(*cmds == '0') {
			if( zero_ok ) break;
			USERR("Number of buffers must be greater than 0\n");
			press_return();
			continue;
		    }
	    } else {
		val = toupper(*cmds) - 'A';	/* which drive do they want? */
		if( val>=0 && val<DPHNUM && dphtab[val]!=0) { /* a drive maybe? */
		    dph = dh[val];
		    if( type==1 ) jj = dph->dph_dirbcb; 
		    else          jj = dph->dph_datbcb;
		    if( ASKFOR(jj) ) {
		    	if( type==1 ) jj = di[val]->d_ndirbs; 
		    	else	      jj = di[val]->d_ndatbs;
		    	if( !SHARED(jj) ) { 
			    val |= SHRMSK;	/* turn on sharing */
			    break;
		        }
		    }
		    USERR("Drive %c: is not available for sharing\n",'A'+val);
		    press_return();
		    continue;
		}
	    }
	    USERR("Please input a number, or an existing drive from 'A:' to 'P:'\n");
	    press_return();
	}
	return val;
}

/************************************/
/* askpm: ask about process maximum
/************************************/
    WORD
askpm(type,pmmax)
    WORD type;
    WORD pmmax;
{
	REG WORD val;
	BYTE *cmds[CMDSLEN];
	BYTE *name;

	switch(type) { case 1: name="directory"; break; case 2: name="data"; }
	if( SHARED(pmmax) || pmmax<=0 )
	    return 0;
	FOREVER {			/* get input from user */
	    printf("Maximum %s buffers per process [%x]? ",name,pmmax);
	    gets( cmds );
	    if( *cmds==NULL )
		 val = pmmax;		/* default value */
	    else val = atoih(cmds);
	    if( val > 0  &&  val <= pmmax ) break;
	    USERR("Maximum must be > zero and <= %x\n",pmmax);
	    press_return();
	}
	return val;
}

/*****************************************/
/* askhash: ask user if they'd like to hash
/*****************************************/
    BOOLEAN
askhash()
{
	REG WORD val;
	BYTE *cmds[CMDSLEN], *cp;

	FOREVER {
	    printf("Hashing [yes] ? ");
	    gets( cmds );
	    if( *cmds == NULL )
		return TRUE;
	    for( cp=cmds; *cp; ++cp ) {
		if( *cp==' '  ||  *cp=='\t' ) {
		    *cp = NULL;
		    break;
		}
		if( isupper(*cp) )
		    *cp = tolower(*cp);
	    }
	    if( (val=whichof(cmds,",yes,true,on,hashing,no,false,off")) > 0 )
		break;
	    USERR("Please answer 'hashing', 'yes', or 'no'.\n");
	    press_return();
	}
	return val<=4;			/* TRUE iff answer was one of 1st four */
}



/************************************************************************/
/* EXPORT
/* xiosvalid: validates that all the user-requestable info has been filled in
/************************************************************************/
    BOOLEAN
xiosvalid()
{
	REG WORD ii;
	REG WORD ndph;

	ndph = 0;
	for( ii=0; ii<DPHNUM; ++ii )
	    if( dphtab[ii]!=0 ) {
		if( dspdsize(dh[ii],di[ii]) == -1 )
		    return FALSE;
		++ndph;
	    }
	if( ndph==0 ) {
	    USERR("no dph information in xios header...");
	    exit(1);
	}
	return TRUE;
}



/***************************************************************************/
/* EXPORT
/* genxios: generates disk buffers (to be placed after the rest of the
/*	system tables.
/*************************************************************************/
#define OFF_BCBPTR 0xA
#define OFF_DPHHTAB 0x12
ADDR locrsvd_off;			/* local reserved offset, for DIRBUF fix */

    ADDR				/* returns final offset */
genxios(doffset)			/* generate the disk buffers */
    ADDR doffset;			/* offset in data file */
{
	REG WORD ii, jj;		/* counter */
	REG BOOLEAN dm;			/* modified flag */
	REG ADDR lastlink;		/* used for threading lists */
	ADDR locbufs_off;		/* local buffer offset for DIRBUF grouping */
	LONG dmagic;			/* where word needing fix goes into file */
	BCB bcb;			/* place for organizing, calc for fixups */
	BCBH bcbh;
	DPH *dph;
	DINFO *dinf;
	BOOLEAN xiosvalid();

	if( !xiosvalid() ) CRASH("invalid disk buffer information");

	for( ii=0; ii<DPHNUM; ++ii ) 	/* allocate CHECK SUM VECTORS */
	    if( dh[ii] ) {		/* if there's something to look at */
		dph = dh[ii];		/* convenient ptr */
		dinf = di[ii];
		if( ASKFOR(dph->dph_csv) ){
		    dinf->d_modified = TRUE;
		    if( dinf->d_csvsize == 0 )
			dph->dph_csv=0;
		    else {
			dph->dph_csv = doffset;
			doffset += bufwrite(dinf->d_csvsize,ZBYTE);
		    }
		}
	    }
	for( ii=0; ii<DPHNUM; ++ii ) 	/* allocate ALLOCATION TABLE VECTORS */
	    if( dh[ii] ) {		/* if there's something to look at */
		dph = dh[ii];		/* convenient ptr */
		dinf = di[ii];
		if( ASKFOR(dph->dph_alv) ){
		    dinf->d_modified = TRUE;
		    dph->dph_alv = doffset;
		    doffset += bufwrite(dinf->d_alvsize,ZBYTE);
		}
	    }
	locbufs_off = 0;		/* amount (bytes) of local dir buffer*/
	for( ii=0; ii<DPHNUM; ++ii ) 	/* allocate DIR BCBs & BUFFERS */
	    if( dh[ii] ) {		/* if there's something to look at */
		dph = dh[ii];		/* convenient ptr */
		dinf = di[ii];
		if( ASKFOR(dph->dph_dirbcb) ){	/* DIR BCBs */
		    dinf->d_modified = TRUE;
		    if( ! SHARED(dinf->d_ndirbs) ) { /* take care of sharing later */
			lastlink=0;		     /* linker */
			zfill(&bcb,sizeof(BCB));     /* init to zeroes */
			bcb.bcb_drv = 0xFF;
			for( jj=dinf->d_ndirbs; jj>0; --jj ){
			    bcb.bcb_link = lastlink;
			    /* pts to loc of buffer relative to locrsvd_off */
			    bcb.bcb_bufptr = locbufs_off;
			    locbufs_off += dinf->d_sdirbs; /* increase buf size */
			    /* calc where bcb will be written to, for fixlater */
			    dmagic = lseek(fns,0L,1) + OFF_BCBPTR;
			    fixlater(dmagic,F_ADD,&locrsvd_off);
			    /* finally, write out the bcb info */
			    lastlink = doffset;	     /* point to the bcb */
			    doffset += datwrite(&bcb,sizeof bcb);
			}
			bcbh.bcbh_lr = lastlink; /* fill in the bcb hdr */
			bcbh.bcbh_pm = dinf->d_pmdirbs;
			dph->dph_dirbcb = doffset; /* link to head of list */
			/* write out a buffer control block hdr */
			doffset += datwrite(&bcbh,sizeof bcbh);
		    }
		}
	    }
	for( ii=0; ii<DPHNUM; ++ii ) 	/* allocate DATA BCBs & BUFFERS */
	    if( dh[ii] ) {		/* if there's something to look at */
		dph = dh[ii];		/* convenient ptr */
		dinf = di[ii];
		/* the bufs can go out of the system page, but the bcbs need to be in */
		if( ASKFOR(dph->dph_datbcb) ){	/* DATA BCBs: !!temporary!!  */
		    dinf->d_modified = TRUE;
		    if( ! SHARED(dinf->d_ndatbs) ) { /* do sharing later */
			lastlink=0;		     /* linker */
			zfill(&bcb,sizeof(BCB));     /* init to zeroes */
			bcb.bcb_drv = 0xFF;
			for( jj=dinf->d_ndatbs; jj>0; --jj ){
			    bcb.bcb_link = lastlink;
			    bcb.bcb_bufptr = bufs_seg; /* point into reserved area */
			    bufs_seg += (dinf->d_sdatbs) >>4; /* cvt to pgphs */
			    /* calculate where the bcb.bcb_bufptr will be written in file */
			    dmagic = lseek(fns,0L,1) + OFF_BCBPTR;
			    fixlater(dmagic,F_ADD,&rsvd_seg); /* fix the bufptr */
			    lastlink = doffset;
			    /* write out a buffer control block */
			    doffset += datwrite(&bcb,sizeof bcb);
			}
			bcbh.bcbh_lr = lastlink; /* fill in the bcb hdr */
			bcbh.bcbh_pm = dinf->d_pmdatbs;
			dph->dph_datbcb = doffset; /* link to head of list */
			/* write out a buffer control block hdr */
			doffset += datwrite(&bcbh,sizeof bcbh);
		    }
		}
	    }
	/* now write the local directory buffers */
	locrsvd_off = doffset;	/* address of directory buffer area */
	doffset += bufwrite(locbufs_off,ZBYTE); /* write out the buffers */

	/* dmagic: magic number to add to dphtab[ii] */
	dmagic = OFF_DPHHTAB + fldstart;/* where in file does SYSDAT start */

	for( ii=0; ii<DPHNUM; ++ii ) 	/* allocate HASH TABLES */
	    if( dh[ii] ) {		/* if there's something to look at */
		dph = dh[ii];		/* convenient ptr */
		dinf = di[ii];
		if( ASKFOR(dph->dph_hstbl) ) {	/* HASH TABLE ALLOC */
		    dinf->d_modified = TRUE;
		    if( dinf->d_hashing ) {
			dph->dph_hstbl = bufs_seg; /* point into reserved area */
			bufs_seg += (dinf->d_shash) >>4; /* cvt bytes to pgphs */
			/* remember where the dph_hstbl word will be stored */
			/* so we can adjust bufs_seg by starting loc of rsvd_seg */
			fixlater(dmagic+dphtab[ii],F_ADD,&rsvd_seg);
		    } else dph->dph_hstbl = 0;
		}
	    }				/**** end of processing ****/

	for( ii=0; ii<DPHNUM; ++ii )	/* resolve shared buffers */
	    if( dh[ii] ) {
		if( ASKFOR(dh[ii]->dph_dirbcb) ) {
		    jj = GETSHARED(di[ii]->d_ndirbs);
		    dh[ii]->dph_dirbcb = dh[jj]->dph_dirbcb;
		}
		if( ASKFOR(dh[ii]->dph_datbcb) ) {
		    jj = GETSHARED(di[ii]->d_ndatbs);
		    dh[ii]->dph_datbcb = dh[jj]->dph_datbcb;
		}
	    }				/* end resolve shared buffers */

	return doffset;
}


/***************************************************************************/
/* EXPORT
/* fixupxios_info: does the fixup for the dph info in the xios
/*************************************************************************/
fixupxios_info()		/* call from fixups */
{
	REG WORD ii;			/* counter */
	REG LONG dmagic;		/* addr var */

	for( ii=0; ii<DPHNUM; ++ii )
	    if( di[ii]->d_modified ) {
		dmagic = fldstart + dphtab[ii];
		lseek(fns,dmagic,0); /* where to write to */
		write(fns,dh[ii],sizeof(DPH));	/* what to write */
	    }
}




/**************************************************************************/
/**************************************************************************/
/*	M E N U  handling routines
/*
/*	Written by Bill Fitler
/**************************************************************************/
/**************************************************************************/
#ifndef MAINMODULE
#include <portab.h>
#include <stdio.h>
#include <menu.h>
EXTERN BOOLEAN verbose;			/* flag: be wordy */
#endif

MLOCAL BOOLEAN eflag = FALSE;		/* user error */

/**************************************************************************/
/* bldmenu: builds a menu list from successive calls
/**************************************************************************/
    MENU *
bldmenu(muroot,typ,ptr,name,description)
    MENU *muroot;			/* root of menu list (or NULLPTR) */
    WORD typ;				/* type of menu item */
    BYTE *ptr;				/* pointer to something... */
    BYTE *name;				/* command name for this menu item */
    BYTE *description;			/* long label for this item */
{
	REG MENU *mu;			/* ptr to created item */
	REG MENU *tmu;			/* temp ptr for linked list scan */

	mu = (MENU *)malloc(sizeof(*mu));	/* allocate memory for item */
	mu->mutype = typ;
	mu->muiptr = ptr;
	mu->muname = name;
	mu->mudesc = description;
	mu->munext = NULLPTR;
	if( muroot == NULLPTR )		/* if NULL list */
		return mu;		/* return something to start with */
	for( tmu=muroot; tmu->munext != NULLPTR; tmu = tmu->munext )
		;			/* append to end of list */
	tmu->munext = mu;	
	return muroot;			/* keep list in order */
}

/**************************************************************************/
/* domenu: executes the specified commands (in cbuf) from the menu (mnu)
/**************************************************************************/
#define CMDLEN 80
#define USERR ++eflag;printf
#define ECHK(fn) _errchk(fn,cmd,cmdval)

    VOID
domenu(cbuf,mnu)
    BYTE *cbuf;				/* command buffer */
    MENU *mnu;				/* table of menu specs */
{
	REG MENU *mi;			/* ptr to menu item */
	LOCAL BYTE cmd[CMDLEN];		/* place for command & value */
	LOCAL BYTE *cmdval;		/* ptr within cmd */
	LOCAL BYTE *cp;			/* temp char ptr */
	BYTE *msetbool();		/* set a boolean value */
	BYTE *msetbyte();		/* set a numeric byte value */
	BYTE *msetword();		/* set an integer pointer */
	BYTE *msettxt();		/* set a text value */
	BYTE *(*proc)();		/* procedure pointer */
	BYTE *msetdrv();		/* set a drive value */
	BYTE *_nxtcmd();		/* command parser */
	MENU *_fndcmd();		/* command lookup */

	eflag = FALSE;			/* no errors yet */
	while((cbuf=_nxtcmd(cbuf,cmd))){ /* while there are commands in cbuf */
	    cmdval = NULLPTR;		/* init value ptr for command */
	    for( cp=cmd; *cp; cp++ ) {	/* scan command for assign op */
		if( isupper(*cp) )	/* convert all commands to LOWER CASE */
		    *cp = tolower(*cp);
		if( *cp == '=' ) {	/* found? */
		    *cp++ = NULL;	/* terminate command */
		    cmdval = cp;	/* point value to after assign op */
		    break;		/* and terminate loop */
		}
	    }
	    if( (mi=_fndcmd(cmd,mnu)) == NULLPTR ){ /* scan menu commands */
		USERR("'%s' is not a command for this menu\n",cmd); 
	    } else {			/******** cmd loop ********/
		switch(mi->mutype) {	/* handle cmd type */
		case MBOOL:		/** BOOLEAN command **/
		    ECHK(msetbool(mi->muiptr,cmdval));
		    break;
		case MBYTE:		/** BYTE command **/
		    ECHK(msetbyte(mi->muiptr,cmdval));
		    break;
		case MWORD:		/** INTEGER command **/
		    ECHK(msetword(mi->muiptr,cmdval));
		    break;
		case MTEXT:		/** TEXT command **/
		    ECHK(msettxt(mi->muiptr,cmdval));
		    break;
		case MPROC:		/** PROCEDURAL command **/
		    proc = mi->muiptr;	/* assign to function ptr */
		    ECHK(((*proc)(cmdval,mi->mudesc)));
		    break;
		case MDRIV:		/** DRIVE letter command **/
		    ECHK(msetdrv(mi->muiptr,cmdval));
		    break;
		}			/* end switch */
	    }				/********* end cmd loop *********/
	    if(eflag) {			/* user error? */
		press_return();
		*cbuf = NULL;
	    } else if(verbose)		/* are we being wordy? */
		prtmival(mi);		/* let the user know what happened */
	}				/**** end while loop ****/
}



/* _nxtcmd: parses cin into cout */
    BYTE *
_nxtcmd(cin,cout)			/* returns ptr to after next cmd */
    BYTE *cin;				/* command in buf */
    BYTE *cout;				/* command out buf */
{
	/***** handle quoted strings??? *****/
	while( isspace(*cin) )		/* skip leading spaces */
		++cin;
	if( *cin == NULL )		/* check for EOS */
		return NULLPTR;
	while( *cin && !isspace(*cin) )	/* scan to eos or whitespace */
		*cout++ = *cin++;	/* xfer to command out buf */
	*cout = NULL;			/* null terminate command out buf */
	return cin;			/* return ptr to after command */
}



/* _fndcmd: scans thru menu structure 'mu' looking for command name 'cn' */
/*		returns ptr to menu item or NULLPTR */
    MENU *
_fndcmd(cn,mus)
    BYTE *cn;				/* command name */
    MENU *mus;				/* ptr to menu list */
{
	REG WORD cnl;			/* cmd name len */
	MENU *musave;			/* extra ptr to menu item */

	cnl = strlen(cn);
	for( ; ; mus = mus->munext) {	/* scan menu list */
	    if( mus==NULLPTR )		/* if we hit the end then fail */
		return NULLPTR;
	    if( strncmp(cn,mus->muname,cnl) == 0 )	/* cn prefix of name?*/
		break;			/* yes, we have a candidate */
	}
	musave = mus;
	for( mus=mus->munext; ; mus=mus->munext ) { /* finish scanning list */
	    if( mus==NULLPTR )		/* if we hit the end then succeed */
		return musave;		/* we reached end of list: found it! */
	    if( strncmp(cn,mus->muname,cnl) == 0 )
		return NULLPTR;		/* another match: ambiguous command */
	}
}



/* _errchk: checks for error returns on functions */
    VOID
_errchk(emsg,cm,cmv)
    BYTE *emsg;				/* error message to check */
    BYTE *cm;				/* command generating the error */
    BYTE *cmv;				/* value generating the error */
{
	if( emsg ) {			/* non NULL error message return? */
		USERR("Error on command '%s",cm);
		if(cmv)
			USERR("=%s",cmv);
		USERR("': %s\n",emsg);
	}
}



/* msetbool: sets the BOOLEAN pointed to by 'bp' according to value in 'cvp' */
    BYTE *				/* return NULLPTR, or ptr to err msg */
msetbool(bp,cvp)
    BYTE *bp;				/* pointer to boolean to set */
    BYTE *cvp;				/* pointer to value to set to */
{
	REG WORD bv;
	REG BYTE *tp;

	if( cvp==NULLPTR )		/* no val: toggle boolean */
		bv = (*bp ? 4 : 1);	/* toggle by setting bv index */
	else {
	    for( tp=cvp; *tp; ++tp ) 
		if( isupper(*tp) ) *tp=tolower(*tp);
	    if( (bv=whichof(cvp,",on,yes,true,off,no,false"))==0 ) 
		return "value must be 'yes' or 'no'";
	}
	if( bv < 4 )
		*bp = -1;		/* zap to 0xFFFF */
	else 	*bp = 0;		/* zap to zero */
	return NULLPTR;
}


    BYTE *
msetbyte(bp,cvp)			/* set numeric BYTE value */
    BYTE *bp;				/* ptr to BYTE to set */
    BYTE *cvp;				/* value to set BYTE to */
{
	REG WORD val;			/* temp holding place */

	val=0;
	if(cvp != NULLPTR) val=atoih(cvp);	/* assume numbers are in HEX!! */
	if( cvp==NULLPTR || (val==0  &&  !isdigit(*cvp)) )
	    return "value must be a number";
	if( val > 255 )
	    return "value must be less than FF hex (255 decimal)";
	*bp = val;
	return NULLPTR;
}


    BYTE *
msetword(ip,cvp)
    WORD *ip;				/* ptr to WORD to set */
    BYTE *cvp;				/* value to set WORD to */
{
	REG WORD val;			/* temp holding place */

	val=0;
	if(cvp != NULLPTR) val=atoih(cvp);	/* assume numbers are in HEX!! */
	if( cvp==NULLPTR || (val==0  &&  !isdigit(*cvp)) )
	    return "value must be an unsigned hex number between 0 and FFFFh";
	*ip = val;
	return NULLPTR;
}


    WORD
atoih(cp)				/* conver ascii hex to word */
    BYTE *cp;				/* buffer number */
{
	REG WORD v;			/* resulting value */
	REG BYTE cv;			/* character value */
	BYTE *savcp;			/* save orig ptr */

	v = 0; savcp = cp;		/* init value */
	for( cv = *cp; ; cv = *++cp ){	/* for each char in cp */
	    if( isdigit(cv) )
		v = (v<<4) + (cv-'0');	/* convert digit and add */
	    else {			/* maybe it's a letter */
		if( isupper(cv) )	/* upper case letter? */
		    cv = tolower(cv);	/* convert it */
		if( cv<'a' || cv>'f')	/* range check */
		    break;		/* terminate loop if failed */
		v = (v<<4)+(10+cv-'a');	/* convert letter and add */
	    }
	}
	if( cp-savcp <= 4 )
	     return v;			/* return the value */
	else { 
	     *savcp |= 0x80; 
	     return 0; 		/* turn on hi bit, make isdigit test fail */
	}
}

    BYTE *
msettxt(tp,cvp)
    BYTE **tp;				/* place to store ptr to value */
    BYTE *cvp;				/* value to set BYTEs to */
{
	REG BYTE *tmp;			/* temp ptr for allocated storage */

	if( cvp==NULLPTR )	
	    cvp="";			/* point to null string, instead */
	tmp = malloc(1+strlen(cvp));	/* get space for string */
	strcpy(tmp,cvp);		/* copy to allocated space */
	*tp = tmp;			/* assign ptr */
	return NULLPTR;
}

    BYTE *
msetdrv(dp,cvp)
    BYTE *dp;				/* place to store ptr to value */
    BYTE *cvp;				/* value to set DRIVE to */
{
	REG WORD v;			/* work place */

	if( cvp!=NULLPTR ) {		/* check for drive letter */
	    if( isupper(*cvp) )
		*cvp = tolower(*cvp);
	    v = *cvp - 'a';		/* convert drive spec to nibble */
	}
	if( cvp==NULLPTR  ||  v<0  ||  v>15  || cvp[1] != ':' )
	     return "you must specify a drive 'A:' thru 'P:'";
	*dp = v;			/* looks okay, set it */
	return NULLPTR;			/* no errors */
}


/**************************************************************************/
/* prtmenu: displays the menu labels, values and descriptors
/**************************************************************************/
    VOID 
prtmenu(mtitle,mnu)
    BYTE *mtitle;			/* title of menu */
    MENU *mnu;				/* table specifying menu */
{
	printf("\n\n%s\n",mtitle);	/* give the menu a title */
	for( ; mnu!=NULLPTR; mnu = mnu->munext ) /* travel down list */
	    prtmival(mnu);	
}


prtmival(mnu)				/* return ptr to value of item */
    MENU *mnu;				/* item to get value of */
{
	REG BYTE *ival;
	BYTE ivbuf[10];			/* local buf for display */
	BYTE *vbp;			/* value byte ptr */
	WORD *vwp;			/* value word ptr */
	WORD v;				/* place to put value */

	printf("%12.12s ",mnu->muname); /* first the name */
	ival = ivbuf; *ival = NULL;	/* init value */
	switch(mnu->mutype) {	/* now the value */
	    case MBOOL:
		if( *(mnu->muiptr) )
		     ival="[Y]";
		else ival="[N]";
		break;
	    case MBYTE:
		vbp = mnu->muiptr;
		v = (*vbp & 0xFF);
		sprintf(ivbuf,"[%2.2x]",v);	/* display in hex */
		break;
	    case MWORD:
		vwp = mnu->muiptr;
		v = *vwp;
		sprintf(ivbuf,"[%4.4x]",v);	/* display in hex */
		break;
	    case MTEXT:
		vbp = *((BYTE **)mnu->muiptr);	/* go get the char ptr */
		if( strlen(vbp) > 4 )
		     sprintf(ivbuf,"[%-4.4s>",vbp);
		else sprintf(ivbuf,"[%s]",vbp);
		break;
	    case MPROC:
		/* leave blank */
		break;
	    case MDRIV:
		sprintf(ivbuf,"[%c:]",'A'+(*mnu->muiptr));
		break;
	}
	printf("%-6s ",ival);
	printf("%s\n",mnu->mudesc);	/* finally the description */
}


/************************************************************************
/* whichof: scans the string 'set' (delimited by 1st char in 'set')
/*	for 1st occurrence of string 'sample'.
/*	Returns 0 if 'sample' not a prefix of any item in set or if
/*		'sample' is a prefix of more than one item in set;
/*		delimiter# o.w.
/* E.G. if set = ",on,yes,true,off,no,false"  then
/*	whichof("yes",set) == 2;
/*	whichof("y",set) == 2;
/*	whichof("o",set) == 0;
/*	whichof("x",set) == 0;
/* NOTE: results not guaranteed if item delimiter (*set) is in string 'sample',
/*	or if null item is in set (adjacent delimiters).
/*************************************************************************/

/* first, a subroutine: */
/* _wget: returns NULLPTR if sa not prefix of item in st; */
/*	  returns end of item (ptr in st) otherwise */
BYTE *_wget(sa,st,de)
BYTE *sa;
BYTE *st;
BYTE de;
{
	BYTE *s2;

	for( ;; ) {					/* do forever */
		for( s2=sa; *s2 && *s2 == *st; ){
			++s2; ++st;			/* try match */
		}
		if( *s2	== NULL ){	/* success! we've matched all of sa */
			while( *st && *st != de ) ++st;	/* adv to nxt delim */
			return( --st );			/* backup one char */
		}
		while( *st  &&  *st != de ) /* no match, look for next item */
			++st;				/* adv to nxt delim */
		if( *st == NULL )
			return( NULLPTR );		/* end of list */
		++st;
	}
}

WORD whichof(sample,set)
BYTE *sample;
BYTE *set;
{
	WORD sx;
	BYTE *sp, *np;

	if( (sp=_wget(sample,set+1,*set)) == NULLPTR )
		return 0;				/* not found */
	if( _wget(sample,sp+1,*set) != NULLPTR )
		return 0;				/* ambiguous */
	for( sx=0, np=set; np<sp; np++ )
		if( *np == *set ) sx++;		/* count delimiters */
	return sx;
}




/*******************************************************************/
/*******************************************************************/
/* misc.c:
/* some miscellaneous functions which belong in the system library */
/*******************************************************************/
/*******************************************************************/
#ifndef MAINMODULE
#include <portab.h>
#include <stdio.h>
#include <cpm.h>			/* directory search support */
#endif

#define FCB struct fcbstruct
FCB {
	BYTE	fcbdr;			/* drive */
	BYTE	fcbname[8];		/* name */
	BYTE	fcbext[3];		/* extension */
	BYTE	fcbrest[24];		/* all the rest */
};


#define FSEARCH(faddr) (bdos(17,faddr)&0xFF)
#define NSEARCH(faddr) (bdos(18)&0xFF)	/* doesn't use FCB */
#define SETDMA(dma) bdos(26,dma)	/* where to put dir info */
#define SECSIZE 128
#define AMASK 0x7F

/* dirsearch: returns a DLIST of names matching 'fs' */
    DLIST *				/* returns list of names found in */
dirsearch(fs)				/* directory search for */
    BYTE *fs;				/* file spec */
{
	FCB f, *fp;			/* use this FCB */
	REG WORD ret;			/* ptr */
	DLIST *_newdli(), *ls;		/* list */
	BYTE dmabuf[SECSIZE];		/* place for directory info */

	fp = &f;			/* everything else uses address */
	parsefn(fs,fp);			/* fill fcb with filespec */
	SETDMA(dmabuf);			/* tell em where to put it */
	if( (ret=FSEARCH(fp)) > 3 )	/* look for first occurrence */
	    return NULLPTR;		/* no luck */
	ls=_newdli(dmabuf+(ret<<5),NULLPTR);	/* start off a list */
	while( (ret=NSEARCH(fp)) <= 3 ) { /* do all the search_nexts */
	    ls = _newdli(dmabuf+(ret<<5),ls); /* link them together */
	}
	return ls;
}


    DLIST *
_newdli(f,lnk)
    FCB *f;
    DLIST *lnk;
{
	REG WORD i;
	BYTE *np, nb[15];		/* name buffer */
	DLIST *newl;			/* new list item */

					/* make a pretty filename */
	np = nb;
	for( i=0; i<11; ++i )		/* zap out any high bits in filename */
	    f->fcbname[i] &= AMASK;	/* mask off attribute bits */
	for( i=0; i<8; ++i ) {		/* handle name spec */
	    if( f->fcbname[i] == ' ' )	/* end of name? */
		break;			/* okay */
	    *np++ = f->fcbname[i];	/* o.w. stuff into name buffer */
	}
	*np++ = '.';			/* tack this on */
	for( i=0; i<3; ++i ){		/* handle ext spec */
	    if( f->fcbext[i] == ' ' )	/* end of ext? */
		break;
	    *np++ = f->fcbext[i];
	}
	*np++ = NULL;			/* null terminate string */
	/*************************************************************/

	np = malloc(np-nb);		/* save it away */
	strcpy(np,nb);			/* do the copy */
	newl = (DLIST *)malloc(sizeof(*newl));	/* get ptr area set up */
	newl->dlname = np;
	newl->dlnext = 	lnk;
	return newl;
}



parsefn(fs,f)
    BYTE *fs;				/* file spec */
    FCB *f;				/* fcb */
{
	REG WORD i;
	REG BYTE *cp;

	for( cp=fs; *cp; cp++ )		/* make everything upper case */
	    if( islower(*cp) )
		*cp = toupper(*cp);
	for( i=0; i<24; ++i )		/* init fcb */
	    f->fcbrest[i] = 0;
	if( *(fs+1) == ':' ) {		/* drive spec? */
	    f->fcbdr = 1 + *fs - 'A';	/* 0=default, 1 = A:,... */
	    fs += 2;			/* skip dr spec */
	} else f->fcbdr = 0;		/* default drive */
	for( i=0; *fs && *fs != '.' && i<8; fs++, ++i ) {	/* NAME. */
	    if( *fs == '*' ) {		/* wildcard? */
		for( ; i<8; ++i )	/* fill up rest of name */
		    f->fcbname[i]='?';	/* with search spec */
		for( ; *fs && *fs!='.'; fs++ )	/* ignore to end or '.' */
		    ;
		break;			/* terminate loop */
	    }
	    f->fcbname[i] = *fs;
	}
	for( ; i<8; ++i )		/* fill up rest of name */
	    f->fcbname[i] = ' ';	/* with blanks */
	for( ; *fs; fs++ )		/* skip rest of name */
	    if( *fs == '.' ) {		/* end of name? */
	        fs++; break;		/* yes, so exit this loop */
	    }
  	for( i=0; *fs && i<3; fs++,++i ) {			/* .EXT */
	    if( *fs == '*' ){		/* wildcard */
		for( ; i<3; ++i )
		    f->fcbext[i] = '?';
		break;
	    }
	    f->fcbext[i] = *fs;
	}
	for( ; i<3; ++i )		/* fill up rest of ext */
	    f->fcbext[i] = ' ';		/* with blanks */
}



/* access: tests for existence: return NULL if exist, -1 o.w. */
    WORD
access(fn)
    BYTE *fn;
{
	FCB f;				/* use this FCB */
	BYTE dmabuf[SECSIZE];		/* place for directory info */

	parsefn(fn,&f);			/* fill fcb with filespec */
	SETDMA(dmabuf);			/* tell em where to put it */
	if( FSEARCH(&f) > 3 )		/* look for first occurrence */
	    return -1;			/* no luck */
	return 0;
}

    BYTE *
gets(s)
    BYTE *s;
{
	BYTE *sav;

	if( fgets(s,32767,stdin) == NULLPTR )
		return NULLPTR;
	if( !isatty(0) )		/* reading from CON:? */
		fputs(s,stdout);	/* echo if not */
	for( sav=s; *s && *s != '\n'; ++s )
		;
	*s = '\0';
	return sav;
}

/************************************************************************/
    BYTE
getdrive()
{
	return bdos(25,0);
}


/************************************************************************/

/* press_return: let user look at screen */
press_return()
{
	BYTE bitbucket[40];		/* leave room */

	if( !isatty(0) ) {		/* stdin from CON:? */
	    printf("Error in command file: terminating program\n");
	    exit(1);
	}
	printf("\07Press RETURN to continue ");
	if( gets(bitbucket)!=bitbucket )
		exit(1);		/* eof, quit now */
}

/* patch: provides a patch area */
patch()					/* should never get called */
{
	int i;
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	i=i+5; i=i+5; i=i+5; i=i+5; i=i+5; 
	return;
}


/*******************************************************************/
/*******************************************************************/
/*			T H E   E N D
/*******************************************************************/
/*******************************************************************/
