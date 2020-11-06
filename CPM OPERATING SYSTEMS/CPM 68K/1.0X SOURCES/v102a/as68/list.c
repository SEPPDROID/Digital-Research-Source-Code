/*
 *	Put all the listing stuff in here
 */
#include "as68.h"
#define	LPP	58								/* # Lines per listing page	    */
int	xline = LPP;							/* Current line	on page		    */
int xpage = 0;								/* Current page #				*/
short pline;								/* Current listing line #		*/
/****************************************************************************/
/*																		    */
/*	.page directive handlers (from dir.c).								    */
/*																		    */
/****************************************************************************/
hpage()									/* Pass 1 .page routine			    */
{										/************************************/
	opitb();							/* Output statement beginning	    */
	wostb();							/* Remaining part of statement	    */
	igrst();							/* Ignore optional comment	        */
}										/************************************/
spage()									/* Pass 2 .page routine			    */
{										/************************************/
	print(0);							/* Print directive line			    */
	xline = LPP;						/* Force top of					    */
	page();								/*				Listing page	    */
}										/************************************/
/****************************************************************************/
/*																		    */
/*	Symbol table print routine.  "psyms" is called AFTER the symbol table   */
/*	has been output to the loader file.  We sort the symbol table, and print*/
/*	the sorted table in PDP-11ish (RT-11) fashion, with a reasonable format */
/*	for the symbols.													    */
/*																		    */
/****************************************************************************/
psyms()											/*							*/
{												/****************************/
	register long j;							/* Temporary				*/
	register char *p;							/* -> Symbol table entries  */
	int symcmp();								/* Symbol comparison func.  */
												/****************************/
	xline = LPP;								/* Force page				*/
	page();										/*				Eject	    */
	printf("S y m b o l   T a b l e\n\n");		/* Print Header				*/
	xline++;									/* Bump line counter		*/
	j = ((lmte-bmte)/STESIZE);					/* Compute # elements		*/
	qsort(bmte,(int)j,STESIZE,symcmp);			/* Sort'em 					*/
	j = 0;										/* Count symbols / line		*/
	for(p = bmte; p < lmte; p += STESIZE)		/* Loop through symbol table*/
	{											/*							*/
		if(j > 3)								/* 4 Symbols / line			*/
		{										/*							*/
			printf("\n");						/* Print newline			*/
			page();								/* Check for top of page	*/
			j = 0;								/* Reset counter			*/
		}										/****************************/
		j += psyme(p);							/* Print 1 table entry 		*/
	}											/*		  (maybe)			*/
	if(j <= 3)									/* Partial line?			*/
		printf("\n");							/*  Yes, finish it			*/
}												/****************************/
symcmp(a,b)										/* Qsort comparison function*/
register char *a,*b;							/* -> Elts to compare		*/
{												/****************************/
	return(strncmp(a,b,NAMELEN));				/* Return +1  for a > b		*/
												/*		   0  for a = b		*/
												/*		  -1  for a < b		*/
}												/****************************/
/****************************************************************************/
/*																			*/
/*	Psyme function.  This function prints a single symbol table entry on	*/
/*	the listing file, complete with TEXT, DATA, BSS, EXT, or UNDEF tag.		*/
/*																			*/
/****************************************************************************/
psyme(osypt)									/* Call with -> ST entry	*/
register struct symtab *osypt;					/****************************/
{												/*							*/
	register char *p;							/* -> Name field			*/
	register int   i;							/* Count register			*/
												/****************************/
	if((osypt->flags & SYER) != 0 ||			/* Do we need to print it?  */
		(osypt->flags & SYIN))					/*							*/
			return(0);							/* 		No.					*/
												/****************************/
	p = &(osypt->name[0]);						/* p -> Symbol name field	*/
	for(i=0;i<NAMELEN;i++)						/* Print name				*/
	{											/*							*/
		if(*p)									/* Non-null character?		*/
			putchar(*p);						/*		Yes, print			*/
		else									/* No						*/
			putchar(' ');						/*		print blank			*/
		p++;									/* Bump pointer				*/
	}											/****************************/
												/*							*/
	printf("  ");								/* Align descriptor			*/
	if(osypt->flags & SYXR)						/* External Reference?		*/
	{											/*							*/
		printf("******** EXT   ");				/* Macro-11 style.			*/
		return(1);								/*							*/
	}											/****************************/
	if(osypt->flags & SYDF)						/* Defined?					*/
	{											/*							*/
		puthex(osypt->vl1.hiword,4);			/* Print high word			*/
		puthex(osypt->vl1.loword,4);			/* And   low  word			*/
		if(osypt->flags & SYRA)					/* DATA?					*/
			 printf(" DATA  ");					/*	Yes.					*/
		else if(osypt->flags & SYRO)			/* TEXT?					*/
			 printf(" TEXT  ");					/*	Yes.					*/
		else if(osypt->flags & SYBS)			/* BSS?						*/
			 printf(" BSS   ");					/*  Yes.					*/
		else printf(" ABS   ");					/* Then it must be absolute	*/
	}											/****************************/
	else										/*							*/
	{											/*							*/
		nerror++;								/* Bump Error count			*/
		printf("*UNDEFINED*    "); 				/* Identify FUBAR			*/
	}											/*							*/
	return(1);									/*							*/
}												/****************************/

/*
 * output source and object listing
 *	call with
 *		2 => print address and binary code only
 *		1 => object in ins[] and instr type in format
 *		0 => print address only
 */
print(pflag)
int pflag;
{
	register short i, *pi;

	if( !prtflg || fchr == EOF) /*no printing desired, or end of source file*/
		return;

	i = instrlen; instrlen = 1;	/*to print preceeding lines*/
	while(pline<p2absln) {		/*need to print some lines*/
		page();
		printf("%4d ",pline);	/*put source line num on listing*/
		printf("                          ");	/*align the source*/
		prtline(1);
		putchar('\n');
		if ((fchr=gchr()) == EOF)
			return;
		pline++;
	}
	instrlen = i;

/* output current address, binary, and source*/
	page();
	printf("%4d ",p2absln);		/*put source line num on listing*/
	puthex((int)loctr.hiword,4);
	puthex((int)loctr.loword,4);
	putchar(' ');
	if(!pflag)			/*no binary*/
		printf("                 ");	/*blanks instead*/
	else {
		pi = ins;
		for(i=0; i< (instrlen/2); i++)	/* binary*/
			puthex(*pi++,4);
		if(instrlen&1)
		{	
			i++;
			puthex(*pi,2);
			printf("  ");		/*Word align*/
		}
		putchar(' ');
		for(;i<4;i++) 			/*four bytes max per line*/
			printf("    ");		/*align the source*/
	}
	if(pline>p2absln || pflag==2)
		putchar('\n');			/*end of line*/
	else {
		prtline(0);
		if(fchr==EOF) return;
		putchar('\n');
		fchr=gchr();
		pline++;
	}
}

prtline(flg)
int flg;
{
		while(fchr!=EOLC && fchr!=EOF)
		{
			putchar(fchr);
			fchr = gchr();
		}
}
/*
 *	Heading Print routine
 */
page()
{
	if((prtflg == 0) || (++xline < LPP)) return;
	printf("\014C P / M   6 8 0 0 0   A s s e m b l e r\t\t%s\t\tPage%4d\n",
		   "Revision 04.03",++xpage);
	printf("Source File: %s\n\n",sfname);
	xline = 3;
}
