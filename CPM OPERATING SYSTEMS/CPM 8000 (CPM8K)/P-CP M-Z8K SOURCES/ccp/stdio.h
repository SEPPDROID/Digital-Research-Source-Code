/*****************************************************************************
*
*		    C P / M   C   H E A D E R   F I L E
*		    -----------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	This is an include file for assisting the user to write portable
*	programs for C.
*
*****************************************************************************/
#define ALCYON 1				/* NOT using Alcyon compiler*/
						/* but Zilog has similar bug*/

/*
 *	Standard type definitions
 */
						/***************************/
#define	BYTE	char				/* Signed byte		   */
#define BOOLEAN	char				/* 2 valued (true/false)   */
#define	WORD	short				/* Signed word (16 bits)   */
#define	UWORD	unsigned int			/* unsigned word	   */
#define	LONG	long				/* signed long (32 bits)   */
#define	ULONG	unsigned long			/* Unsigned long	   */
#define	REG	register			/* register variable	   */
#define	LOCAL	auto				/* Local var on 68000	   */
#define	EXTERN	extern				/* External variable	   */
#define	MLOCAL	static				/* Local to module	   */
#define	GLOBAL	/**/				/* Global variable	   */
#define	VOID	/**/				/* Void function return	   */
#define XADDR	long				/* Extended (SEG) address  */
						/***************************/
#ifdef ALCYON
#define UBYTE	char
#define UBWORD(a) ((UWORD)a & 0xff)
			/* Unsigned byte to word cast	*/
#else
#define	UBYTE	unsigned char			/* Unsigned byte	   */
#define UBWORD(a) (UWORD)a
#endif



/****************************************************************************/
/*	Miscellaneous Definitions:					    */
/****************************************************************************/

#define	FAILURE	(-1) 			/*	Function failure return val */
#define SUCCESS	(0) 			/*	Function success return val */
#define	YES	1 			/*	"TRUE"			    */
#define	NO	0 			/*	"FALSE"			    */
#define	FOREVER	for(;;) 		/*	Infinite loop declaration   */
#define	NULL	(BYTE *)0 		/*	Null pointer value	    */
#define XNULL	0L 			/*	Null XADDR		    */
#define	EOF	(-1)			/*	EOF Value		    */
#define	TRUE	(1)
					/*	Function TRUE  value	    */
#define	FALSE	(0)
					/*	Function FALSE value	    */


/****************************************************************************/
/*									    */
/*				M A C R O S				    */
/*				-----------				    */
/*									    */
/*	Define some stuff as macros ....				    */
/*									    */
/****************************************************************************/

#define	abs(x)	((x) < 0 ? -(xIqThis directory contains the source, object, and archived object ("libcpm.a")
of the Non-segmented version of the Z8000 C runtime library.  The segmented
version differs only in the assembler modules.
