
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121
*/

char *version = "@(#)optim.c	1.6	11/11/83";

/* Optimization pass of the C68 compiler */

#include "optim.h"

char *dest;

main(argc,argv)
int argc;
char *argv[];
{
	if (argc != 3)
		error(FATAL,"usage: %s source destination\n",argv[0]);
	
	in = &ibuf; out = &obuf; dest = argv[2];
	if (fopen(argv[1],in,0) < 0)
		error(FATAL,"Can't open source %s\n",argv[1]);
	if (fcreat(dest,out,0) < 0)
		error(FATAL,"Can't open destination %s\n",argv[2]);
	
	mloop();
	bra_optim();			/* fixup all saved material */
	do_exit(0);
}

/* mloop - main loop to parser file */
mloop()
{
	register int tokid, c;

	while(tokid=gettok()) {		/* while ATEOF not returned */
		if ( ISBRANCH(tokid) ) {
			save_b[braop].bra = 1;
			braop++;
			gettok();				/* branch label !!! */
			ignore();
			strcpy(save_b[braop-1].str,token);
		}
		else if (braop) {
			if(ISCOMMENT(tokid)) {	/* comments interspersed w/ bras, labels */
				save_comment();	
				continue;
			}
			if (islabel(tokid)) {	/* multiple branches to same point */
				strcpy(save_l[lab_ndx].str,token);
				save_l[lab_ndx].bra = braop;
				if((c=skip_white()) != NEWLINE)	/* skip over cr's */
					unget(c);
				lab_ndx++;
				bra_optim(TRUE);		/* remove unnecessary branches */
				continue;
			}
			bra_optim(FALSE);			/* remove unnecessary branches */
			puttok(token);
		}
		else if (tokid == SPECIAL)
			putc(token[0],out);
		else if (tokid != BRANCH) 
			puttok(token);
	}
}

/* do_exit - exit and cleanup */
do_exit(status)
int status;
{
	if (obuf.fd) {	/* File opened !!! */
		fflush(out);
		close(obuf.fd);
#ifndef VERSADOS
		if (status)
			unlink(dest);
#endif
	}
	exit(status);
}

/* bra_optim - remove all branches which are to the next instruction    */
/*		call routines to output remaining branches, comments and labels */
bra_optim(intermediary)
int intermediary;
{
	register int ndx, bndx;
	if(lab_ndx) {
		ndx = 0; bndx = braop;
		while (bndx) {
			if (!save_b[bndx-1].bra) 
				goto nobra;
			if( ndx < lab_ndx) {
				if(save_l[ndx].bra >= braop &&
						   strcmp(save_l[ndx].str,save_b[bndx-1].str)==0) {
					ndx = save_b[bndx-1].bra = 0;
nobra:
					bndx--;
					continue;
				}
				ndx++;
			}
			else	/* this bra instruction doesn't match any of the labels */
				break;
		}
	}
	if(!intermediary)
		out_bras();				/* output remaining branches */
}

/* save_comment - save a comment which is interspersed in labels and bra's */
save_comment()
{
	register int ndx, c;
	register char *p;

	save_c[com_ndx].bra = braop - 1;
	p = save_c[com_ndx].str;
	ndx = 0;
	while((c=ngetch()) != NEWLINE && ndx < MAXCOM)
		*p++ = c;
	*p = NULLC;
	com_ndx++;
}

/* out_bras - output unoptimized branches */
out_bras()
{
	register int ndx, cndx, lndx;

	if(braop)
		for(lndx = cndx = ndx = 0; ndx < braop; ndx++) {
			if (save_b[ndx].bra) {
				puttok("bra ");
				puttok(save_b[ndx].str);
				putc(NEWLINE,out);
			}
			cndx = out_comments(cndx,ndx);
			lndx = out_labs(lndx,ndx);
		}
	else {
		out_comments(0,0);
		out_labs(0,0);
	}
	lab_ndx = com_ndx = braop = 0;
}

/* out_comments - output comments which were interspersed with bra's & labels */
out_comments(cndx,bndx)
register int cndx, bndx;
{
	while (cndx < com_ndx && (bndx == braop || save_c[cndx].bra <= bndx)) {
		putc(ASTERISK,out);
		puttok(save_c[cndx].str);
		putc(NEWLINE,out);
		cndx++;
	}	/* retrieve saved comments */
	return(cndx);
}

/* out_labs - output labels */
out_labs(lndx,bndx)
int lndx, bndx;
{
	register int changes=0;

	while (lndx < lab_ndx && (bndx == braop || save_l[lndx].bra == (bndx+1))) {
		puttok(save_l[lndx].str);
		putc(COLON,out);
		lndx++;
		changes=1;
	}
	if (changes)
		putc(NEWLINE,out);
	return(lndx);
}

/**
 *	gettok - get a token from the input file.
 *		token <== token string
 *		return token identifier
**/
gettok()
{
	register char *p;
	register int fchr;

	fchr = skip_white();
	if(fchr == EOF)
		return(ATEOF);
	p = token;
	if(fchr=='$') {	/* hex constant */
		do {
			*p++ = fchr;
			fchr = ngetch();
		} while (ISHEX(fchr));
		unget(fchr);
		*p = 0;
		return(HEXCON);
	}
	if(fchr=='0') {	/* octal constant */
		do {
			*p++ = fchr;
			fchr = ngetch();
		} while ( ISOCTAL(fchr) );
		unget(fchr);
		*p = 0;
		return(OCTCON);
	}
	if(ISDIGIT(fchr)) {	/* decimal constant */
		do {
			*p++ = fchr;
			fchr = ngetch();
		} while( ISDIGIT(fchr) );
		unget(fchr);
		*p = 0;
		return(DECCON);
	}
	if(ISALPHA(fchr) || SYMCHAR(fchr)) {
		do {
			if(ISALPHA(fchr) || ISDIGIT(fchr) || SYMCHAR(fchr))
				*p++ = fchr;
			else 
				break;
			fchr = ngetch();
		} while (fchr != EOF);
		unget(fchr);
		*p = 0;
		if (strcmp(token,"bra")==0)
			return(BRANCH);
		else
			return(SYMBOL);
	}
	*p++ = fchr;
	*p = NULLC;
	if(fchr == ASTERISK)
		return(STAR);
	return(SPECIAL);
}

puttok(p,need_space)
register char *p;
int need_space;
{
	while (*p) {
		putc(*p,out);
		p++;
	};
}

skip_white()
{
	register int ch, white;

	white = -1;
	do {
		ch = ngetch();
		white++;
	} while(ISSPACE(ch));	/* skip white space tabs and spaces */
	if(!braop && white)
		putc(' ',out);
	return(ch);
}

ignore()
{
	register int ch;

	do {
		ch = ngetch();
	} while(ch != NEWLINE);
}

/* islabel - token is a label ?? */
islabel(tokid)
int tokid;
{
	register int ch;

	if(tokid == SYMBOL) {
		if((ch=ngetch()) == ':')
			return(TRUE);
		unget(ch);
		return(FALSE);
	}
	return(FALSE);
}

/* unget - save look-ahead character */
unget(ch)
int ch;
{
	if(pbchar)
		error(FATAL,"Too many pushed back characters");
	pbchar = ch;
}

/* ngetch - get new character or look-ahead character */
ngetch()
{
	register int c;

	if(pbchar) {
		c = pbchar;
		pbchar = 0;
	}
	else
		c = getc(in);
	if (c < 0)
		return(EOF);
	return(c);
}

/* error - print error strings, exit if error is fatal */
error(fatal,template,s)
int fatal;
char *template, *s;
{
#ifndef VERSADOS
	printf((char *)STDERR,template,s);
#else
	printf(template,s);
#endif
	if (fatal)
		do_exit(-1);
}

strcpy(s,t)
register char *s, *t;
{
	while(*s++ = *t++)
		;
}
