/* streamio.c: tests Unix Stream I/O functions */
/* Modifications: */
/* 1/84 verbose error msgs for 68K    whf */
/* 9/83 improved error msgs   whf */
/* Runs by itself: just compile and exec.  It will tell you which functions
	it is testing. */

#include <portab.h>
#include <stdio.h>


/************************************************/
/* Possible file size parameters for each pass: */
#define NUMPASS 4		/* Number of passes for this test */
#define MAXRECMAX 128		/* Maximum record size */
#define NRCHECKS 7		/* Number of random i/o checks */
WORD pass_types[NUMPASS][2] =	/* Num records, Len records for each pass */
{
	100,	MAXRECMAX,
	16,	32,
	10,	20,
	10000,	20
};


/*************************/
/* O.S. dependent values */
#define BYTE_LEVEL 128		/* Size of sectors: PCDOS = 1, CP/M = 128 */


/*********************************************/
/* Possible kinds of File I/O for each case: */

/* Ascii/Binary I/O indicator */
#define AB_ASCII	1
#define AB_BINARY	0

/* Record/Char/Line indicator */
#define RCL_RECORD	1
#define RCL_CHAR	2
#define RCL_LINE	3

#define NUMCASE 6		/* Number of cases per pass */
WORD case_types[NUMCASE][2] =	/* AB type, RCL type for each case */
{
	AB_BINARY,	RCL_RECORD,
	AB_BINARY,	RCL_LINE,
	AB_BINARY,	RCL_CHAR,
	AB_ASCII,	RCL_LINE,
	AB_ASCII,	RCL_RECORD,
	AB_ASCII,	RCL_CHAR
};

/*************************************************************************/
/*************************************************************************/

GLOBAL WORD nerrors = 0;		/* number of errors reported */

main(ac,av)
    int ac;
    char **av;
{
	int ii, jj, nfiles;
	BYTE **files, *calloc();
	BYTE *test_dir, *make_fn();

	av[0] = "STREAMIO";		/* kludge: DRC doesn't handle */
	test_dir = "g:";		/* where we want to do the testing */
	nfiles = 0;
	files = calloc(NUMPASS*NUMCASE,sizeof(BYTE *));

	printf("%s tests the following Stream I/O functions:\n",av[0]);
	printf("\tfopen, fclose, freopen, fseek, ftell, rewind\n");
	printf("as well as functions listed below.\n");
	printf("****************** %s test starting ****************\n",av[0]);

	for( ii=0; ii<NUMPASS; ++ii )
	{
		prt_pass(ii);
		for( jj=0; jj<NUMCASE; ++jj )
		{
			files[nfiles] = make_fn(ii,jj,test_dir);
			test(ii,jj,files[nfiles]);
			nfiles++;
		};
	};
	for( ii=0; ii<nfiles; ++ii )
	{
		/* unlink files here */
	}
	printf("****************** %s test completed *************\n",av[0]);
	printf("******************** number errors: %d *************\n",nerrors);
}

prt_pass(pass)
    WORD pass;
{
	LONG rmax;
	WORD rlen;

	rmax = pass_types[pass][0];
	rlen = pass_types[pass][1];
	printf("pass %d:\t\t",pass);
	printf("number records = %ld, record length = %d\n",rmax,rlen);
}

BYTE *make_fn(pass,cse,prefix)	/* make a filename from all the parameters */
    WORD pass,cse;
    BYTE *prefix;
{
	LONG rmax;
	WORD rlen, ab_f, rcl_f;
	BYTE ab_ch, rcl_ch, *fp, *calloc();

	rmax = pass_types[pass][0];
	rlen = pass_types[pass][1];
	ab_f = case_types[cse][0];
	ab_ch = (ab_f==AB_ASCII) ? 'A' : 'B';
	rcl_f = case_types[cse][1];
	switch(rcl_f)
	{
	  case RCL_RECORD:	rcl_ch = 'R'; break;
	  case RCL_CHAR:	rcl_ch = 'C'; break;
	  case RCL_LINE:	rcl_ch = 'L'; break;
	};
	fp = calloc(strlen(prefix)+12,1);
	sprintf(fp,"%sZ%c%c%.5D.%.3d",prefix,ab_ch,rcl_ch,rmax,rlen);
	return fp;
}


prt_case(cse)
    WORD cse;
{
	WORD ab_f, rcl_f;

	ab_f = case_types[cse][0];
	rcl_f = case_types[cse][1];
	printf("\tcase %d: %s file i/o on ",cse,
		(AB_ASCII==ab_f) ? "ASCII" : "BINARY" );
	switch(rcl_f)
	{
	  case RCL_RECORD:	printf("fread,fwrite"); break;
	  case RCL_CHAR:	printf("fgetc,fputc"); break;
	  case RCL_LINE:	printf("fgets,fputs"); break;
	};
	printf("   ");
}


/*************************************************************************/
/*************************************************************************/

GLOBAL LONG wrecs;			/* records actually written */
GLOBAL LONG rrecs;			/* records actually read */
GLOBAL LONG recmax;			/* max number of records to write */
GLOBAL WORD reclen;			/* length of each record */
GLOBAL WORD ab_flag;			/* Ascii/Binary i/o indicator */
GLOBAL WORD rcl_flag;			/* Record/Char/Line indicator */
GLOBAL BYTE *filename;			/* Name of test file */
GLOBAL WORD all_but_N;			/* num records to write in steps 2,3 */
GLOBAL FILE *tfsp;			/* Test File Stream Pointer */
GLOBAL BYTE ch_first, ch_last;		/* range of chars to write to file */
GLOBAL BYTE rec1[MAXRECMAX+2], rec2[MAXRECMAX+2];	/* record buffers */
GLOBAL WORD chk_cnt;			/* count of num rand read checks */
GLOBAL WORD chk_incr;			/* read random every chk_incr records*/
GLOBAL LONG chk_recn[NRCHECKS];		/* record numbers to read random */
GLOBAL LONG chk_rpos[NRCHECKS];		/* positions of above records */
LONG ftell();

test(pass,cse,fn)
    WORD pass, cse;
    BYTE *fn;
{
	wrecs = 0; rrecs = 0;
	recmax = pass_types[pass][0];
	reclen = pass_types[pass][1];
	ab_flag = case_types[cse][0];
	rcl_flag = case_types[cse][1];
	filename = fn;
	all_but_N = 5;
	ch_first = 0; ch_last = 0xFF;
	if( AB_ASCII == ab_flag  ||  RCL_LINE == rcl_flag ) 
	{				/* don't write extra \n in bin files */
		ch_first = ' '; ch_last = '~'; 
	}
	chk_incr = recmax / NRCHECKS;
	for( chk_cnt=0; chk_cnt<NRCHECKS; ++chk_cnt )
		chk_recn[chk_cnt] = chk_rpos[chk_cnt] = 0;
	chk_cnt = 0;

	prt_case(cse); printf("%s tested: ",fn);

#define CHK(xx) if(xx) break
	while(1)			/* loop once (allow break) */
	{
		CHK( 	build_it()	);
		CHK( 	grow_it()	);
		CHK(	rand_write()	);
		CHK( 	check_it()	);
		CHK( 	rcheck_it()	);

		printf("OK\n");
		return;
	}
	printf("\n\tRecords written/read before failure: %ld/%ld\n\n",
		wrecs,rrecs);
	++nerrors;			/* incr error count */
}
/***************************************************************************/

build_it()			/*** build the file ***/
{
	LONG rn;
	FILE *fopen(), *fopenb();

	/* step 1: create the file */
	if( ab_flag == AB_ASCII )
		tfsp = fopen(filename,"w");
	else	tfsp = fopenb(filename,"w");
	if( NULLPTR == tfsp ) return eprintf("fopen(%s,w) in build_it",
					filename);

	/* step 2: write & fflush the first record */
	rn = 0;
	if( capture_pos(rn) ) return FAILURE;
	if( write_it(rn) ) return FAILURE;
	if( FAILURE == fflush(tfsp) ) return eprintf("fflush in build_it");

	/* step 2: write all but all_but_N of the records */
	for( rn=1; rn<recmax-all_but_N; ++rn )
	{
		if( capture_pos(rn) ) return FAILURE;
		if( write_it(rn) ) return FAILURE;
	}

	/* step 3: close file once */
	if( FAILURE == fclose(tfsp) ) return eprintf("fclose in build_it");

	return SUCCESS;
}
/***************************************************************************/

capture_pos(rn)
    LONG rn;
{
	if( rn % chk_incr == 0  &&  chk_cnt < NRCHECKS)
	{
		chk_recn[chk_cnt] = rn;
		if( -1L == (chk_rpos[chk_cnt]=ftell(tfsp)) )
			return eprintf("ftell in capture_pos");
		++chk_cnt;
	}
	return SUCCESS;
}

/***************************************************************************/

write_it(rnum)
    LONG rnum;
{
	WORD rlen, ii;

	/* step 1: make a record */
	rlen = reclen;
	if( rcl_flag == RCL_LINE ) --rlen;	/* leave room for newline */
	makerec(rec1,rlen,rnum,filename,ch_first,ch_last);
	if( rcl_flag == RCL_LINE ) rec1[reclen-1]='\n';

	/* step 2: write the record */
	switch( rcl_flag ) 
	{
	  case RCL_RECORD:
		if( reclen != fwrite(rec1,1,reclen,tfsp) )
			return eprintf("fwrite in write_it");
		break;
	  case RCL_CHAR:
		for( ii=0; ii<reclen; ++ii )
			if( FAILURE == fputc(rec1[ii],tfsp) )
				return eprintf("fputc in write_it");
		break;
	  case RCL_LINE:
		if( FAILURE == fputs(rec1,tfsp) )
			return eprintf("fputs in write_it");
		break;
	}
	++wrecs;		/* incr num records actually written */
	return SUCCESS;
}
/***************************************************************************/

grow_it()			/*** append to the file ***/
{
	LONG rn;
	LONG rpos;
	FILE *fopen(), *fopenb(), *freopen(), *freopb();

	/* step 1: open for append */
	if( AB_ASCII == ab_flag )
		tfsp = fopen(filename,"a");
	else	tfsp = fopenb(filename,"a");	/* watch: reclen % 128 != 0 */
	if( NULLPTR == tfsp )
		return eprintf("fopen(%s,a) in grow_it",filename);

	/* step 2: write all_but_N-1 records */
	rn = recmax - all_but_N;
	if( rn < 0 )
		rn = 0;
	if( AB_BINARY == ab_flag )	/* make sure we handle reclen */
	{
		rpos = rn * reclen;
		if( fseek(tfsp,rpos,0) )
			return eprintf("fseek in grow_it");
	}
	for( ; rn < recmax - 1; ++rn )
	{
		if( capture_pos(rn) ) return FAILURE;
		if( write_it(rn) ) return FAILURE;
	}

	/* step 3: reopen file */
	if( AB_ASCII == ab_flag )
		tfsp = freopen(filename,"a",tfsp);
	else	tfsp = freopb(filename,"a",tfsp);
	if( NULLPTR == tfsp )
		return eprintf("freopen(%s,a) in grow_it",filename);

	/* step 4: write last record */
	if( AB_BINARY == ab_flag )	/* make sure we handle reclen */
	{
		rpos = rn * reclen;
		if( fseek(tfsp,rpos,0) )
			return eprintf("fseek in grow_it");
	}
	for( ; rn < recmax; ++rn )
	{
		if( capture_pos(rn) ) return FAILURE;
		if( write_it(rn) ) return FAILURE;
	}

	/* step 5: close file again */
	if( FAILURE == fclose(tfsp) )
		return eprintf("fclose in grow_it");

	return SUCCESS;
}
/***************************************************************************/

rand_write()				/* write some randomly */
{
	WORD ii;
	LONG tt, eof_slack;
	FILE *fopen(), *fopenb();

	/* step 1: open for append */
	if( AB_ASCII == ab_flag )
		tfsp = fopen(filename,"a");
	else	tfsp = fopenb(filename,"a");	/* watch: reclen % 128 != 0 */
	if( NULLPTR == tfsp )
		return eprintf("fopen(%s,a) in rand_write",filename);

	/* Step 2: write randomly absolute */
	for( ii=0; ii<chk_cnt; ++ii )
	{
		if( FAILURE == fseek(tfsp,chk_rpos[ii],0) ) {
			eprintf("fseek(%ld,0) in rand_write",chk_rpos[ii]);
			for( ii=0; ii<chk_cnt; ++ii )
			    printf("%2.2d %5.5ld %5.5ld\n",ii,chk_recn[ii],
				chk_rpos[ii]);
			return FAILURE;
		}
		if( write_it(chk_recn[ii]) )
			return FAILURE;
	}

	/* step 3: write randomly relative */
	if( AB_BINARY == ab_flag ) {
		/*** compute 'slack' at eof due to inexact nature of
		     eof for binary files with 128 byte sectors
		     (goes away on byte level i/o O.S.s )        ********/
		if( BYTE_LEVEL > 1 ) {
		     eof_slack = BYTE_LEVEL - ((recmax*reclen) % BYTE_LEVEL);
		     if( eof_slack == BYTE_LEVEL )
			eof_slack = 0;
		} else eof_slack = 0;
		tt = -reclen - eof_slack;
		if( FAILURE == fseek(tfsp,tt,2) ) /* addr of last record*/
			eprintf("fseek(%ld,2) in rand_write",tt);
		if( write_it(recmax-1) )
			return FAILURE;
		tt = 1 - recmax; tt *= reclen; tt -= eof_slack;
		if( FAILURE == fseek(tfsp,tt,2) )
			eprintf("fseek(%ld,2) in rand_write",tt);
		if( write_it(1L) )
			return FAILURE;
	}

	/* step 4: close file again */
	if( FAILURE == fclose(tfsp) )
		return eprintf("fclose in rand_write");

	return SUCCESS;
}

/***************************************************************************/


check_it()		/*** check that everything reads ok sequential ***/
{
	LONG rn;
	FILE *fopen(), *fopenb();

	/* step 1: open for read */
	if( AB_ASCII == ab_flag )
		tfsp = fopen(filename,"r");
	else	tfsp = fopenb(filename,"r");	/* watch: reclen % 128 != 0 */
	if( NULLPTR == tfsp )
		return eprintf("fopen(%s,r) in check_it",filename);

	/* step2: read sequential */
	for( rn=0; rn < recmax; ++rn )
	{
		if( read_it(rn) )
			return FAILURE;
	}

	/* step 3: close file */
	if( FAILURE == fclose(tfsp) )
		return eprintf("fclose in check_it");

	return SUCCESS;
}
/***************************************************************************/

read_it(rnum)
    LONG rnum;
{
	WORD rlen, ii;

	/* step 1: make a record */
	rlen = reclen;
	if( rcl_flag == RCL_LINE ) --rlen;	/* leave room for newline */
	makerec(rec1,rlen,rnum,filename,ch_first,ch_last);
	if( rcl_flag == RCL_LINE ) rec1[reclen-1]='\n';

	/* step 2: read the record */
	switch( rcl_flag ) 
	{
	  case RCL_RECORD:
		if( reclen != fread(rec2,1,reclen,tfsp) )
			return eprintf("fread in read_it");
		break;
	  case RCL_CHAR:
		for( ii=0; ii<reclen; ++ii )
			if( FAILURE == (rec2[ii]=fgetc(tfsp)) )
				return eprintf("fgetc in read_it");
		break;
	  case RCL_LINE:
		if( FAILURE == fgets(rec2,reclen+1,tfsp) )
			return eprintf("fgets in read_it");
		break;
	}
	if( strncmp(rec1,rec2,reclen) != 0 )
	 return eprintf("ripple mismatch:\n\texpected: '%s'\n\tgot:      '%s'",
			rec1,rec2);
	++rrecs;			/* incr num recs actually read */
	return SUCCESS;
}
/***************************************************************************/

rcheck_it()		/*** check that some of it reads ok randomly ***/
{
	WORD ii;
	LONG rn;
	LONG rpos;
	FILE *fopen(), *fopenb();

	/* Step 1: open the file */
	if( AB_ASCII == ab_flag )
		tfsp = fopen(filename,"r");
	else	tfsp = fopenb(filename,"r");
	if( NULLPTR == tfsp )
		return eprintf("fopen in rcheck_it");

	/* Step 2: read randomly absolute */
	for( ii=0; ii<chk_cnt; ++ii )
	{
		if( FAILURE == fseek(tfsp,chk_rpos[ii],0) )
			return eprintf("fseek(%ld,0) in rcheck_it",
				chk_rpos[ii]);
		if( ftell(tfsp) != chk_rpos[ii] )
			return eprintf("ftell in rcheck_it");
		if( read_it(chk_recn[ii]) )
			return FAILURE;
	}

	/* Step 3: read randomly relative */
	if( FAILURE == rewind(tfsp) )
		return eprintf("rewind in rcheck_it");
	if( AB_BINARY == ab_flag )		/* do this for BINARY files: */
	    for( rn=chk_incr,rpos=chk_incr*reclen; rn<recmax; rn+=(1+chk_incr) )
	    {
		if( FAILURE == fseek(tfsp,rpos,1) )
			return eprintf("fseek(%ld,1) in rcheck_it",rpos);
		if( read_it(rn) )
			return FAILURE;
	    }

	/* Step 4: close */
	if( FAILURE == fclose(tfsp) )
		return eprintf("fclose in rcheck_it");

	return SUCCESS;
}
/***************************************************************************/
/***************************************************************************/


/****************************************************************/
/* makerec: builds a rippled record for a given record number 	*/
/*	Record looks like:					*/
/*		<record_label><record_number><ripple_pattern> 	*/
/*	or	<ripple_pattern>     if reclabel==NULL		*/
/****************************************************************/

makerec(record,rlen,recnum,reclabel,clo,chi)
    BYTE *record;		/* place to make the record */
    UWORD rlen;		/* size of the record to make */
    LONG recnum;		/* record number to make */
    BYTE *reclabel;		/* label to prepend to each record */
    WORD clo, chi;		/* range of char values for ripple pattern */
{
	BYTE lbuf[10];		/* place to hold chars */
	BYTE *cp;		/* char ptr */
	WORD cc;		/* char temp */

	clo &= 0xFF; chi &= 0xFF;		/* defeat sign extension */

	if( reclabel != NULLPTR )
	{
	   for( ; *reclabel  &&  rlen>0; --rlen )/* copy label to record */
		*record++ = *reclabel++;
	   sprintf(lbuf,"#%5.5D#",recnum);      /* cvt recnum to ascii */
	   for( cp=lbuf; *cp  &&  rlen>0; --rlen)/* copy recnum into rec */
		*record++ = *cp++;
	}
	cc = recnum % (1 + chi - clo);		/* choose new place to start */
	cc += clo;				/* DRC bug... */
	for( ; rlen>0; --rlen )			/* generate ripple pattern */
	{
	   if( cc > chi ) cc = clo;		/* ripple range check */
	   *record++ = cc++;
	}
	*record++ = NULL;			/* insure NULL terminated */
	*record++ = NULL;			/* insure NULL terminated */
}

/***************************************************************************/
/***************************************************************************/
/************************************************************************
*
*	Calling sequence:
*		nchrs = eprintf(fmt,arg1,arg2,...argn);
*	Where:
*		nchrs = # chars output by printf
*		fmt -> a string specifying how arg1-n are to be printed.
*
**************************************************************************/


WORD eprintf(fmt,args)
	BYTE *fmt,
	     *args;
{
	_doprt(stdout,"\nERROR:\t");
	return(_doprt(stdout,fmt,&args));
}

WORD ep_init()		/* initiate eprintf for new set of error messages */
{
}

WORD ep_fini()		/* wrap up a set of error messages */
{
}
                                                                                      ap up a set of error messages */
{
}
                                                                                      ap up a set of error messages */
{
}
                                                                                      