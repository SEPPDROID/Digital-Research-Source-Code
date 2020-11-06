/****************************************************************************/
/*									    */
/*	                   B A S E P A G E . H                              */
/*	                   -------------------                              */
/*	                                                                    */
/*	This file contains a definition of the CP/M basepage structure,     */
/*	b_page.								    */
/*									    */
/*	NOTE: In the portable CP/M environment, it is NOT guaranteed        */
/*	that the location of the base page is known at link-edit time       */
/*	(as it is, for example, in CP/M-80 and CP/M-86.)  Instead, a        */
/*	pointer to the current basepage is delivered by the BDOS	    */
/*	to each new program which is run.   This pointer, _base, is         */
/*	initialized by the C startup function (startup.s) and is            */
/*	available to C programs as an external.                             */
/*									    */
/*	"cpm.h" MUST be '#include'd before this file			    */
/*									    */
/****************************************************************************/

struct b_page
{
		BYTE		*ltpa;		/* Low TPA address	    */
		BYTE		*htpa;		/* High TPA address	    */
		BYTE		*lcode;		/* Start address of code seg*/
		LONG		 codelen;	/* Code segment length	    */
		BYTE		*ldata;		/* Start address of data seg*/
		LONG		 datalen;	/* Data segment length	    */
		BYTE		*lbss;		/* Start address of bss seg */
		LONG		 bsslen;	/* Bss segment length	    */
		LONG		 freelen;	/* Free segment length	    */
		BYTE		 resvd1[20];	/* Reserved area	    */
		struct fcb	 fcb2;		/* Second basepage FCB	    */
		struct fcb	 fcb1;		/* First basepage FCB	    */
		BYTE		 buff[128];	/* Default DMA buffer,	    */
						/*   command line tail	    */
};

extern struct b_page	*_base;
