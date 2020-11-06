/*****************************************************************************
*
*		    C P / M   C   H E A D E R   F I L E
*		    -----------------------------------
*
*	This is the standard include file for the CP/M-68K assembler / linker
*	kludge package.  
*
*****************************************************************************/

/*
 *	Standard type definitions
 */
						/***************************/
#define	BYTE	char				/* Signed byte		   */
#define	UBYTE	unsigned char			/* Unsigned byte	   */
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
						/***************************/

/*	Open Mode Definitions						    */
					/************************************/
#define	READ	0			/*	Open for READ only	    */
#define	WRITE	1			/*	Open for WRITE only	    */
#define	IO	2			/*	Open for READ and WRITE	    */
#define	UPDATE	2			/*	Same as IO		    */
					/************************************/



/****************************************************************************/
/*	Miscellaneous Definitions:					    */
/****************************************************************************/
#define	FILE	struct ccb		/*	Define FILE pointer	    */
#define	FAILURE	(-1)			/*	Function failure return val */
#define SUCCESS	(0)			/*	Function success return val */
#define	YES	1			/*	"TRUE"			    */
#define	NO	0			/*	"FALSE"			    */
#define	FOREVER	for(;;)			/*	Infinite loop declaration   */
#define	NULL	0			/*	Null pointer value	    */
#define	EOF	(-1)			/*	EOF Value		    */
#define	TRUE	(1)			/*	Function TRUE  value	    */
#define	FALSE	(0)			/*	Function FALSE value	    */
#define	STDIN	0			/*	File 			    */
#define	STDOUT	1			/*		numbers		    */
#define	STDERR	2			/*			for std i/o */

/****************************************************************************/
/*									    */
/*				M A C R O S				    */
/*				-----------				    */
/*									    */
/*	Define some stuff as macros ....				    */
/*									    */
/****************************************************************************/

#define	stdin	(&_iob[0])		/*	Standard input  channel	    */
#define	stdout	(&_iob[1])		/*	Standard output channel     */
#define	stderr	(&_iob[2])		/*	Standard error  channel	    */
#define	fileno(p)   p->chan		/*	File # of a channel	    */
#define	abs(x)	((x) < 0 ? -(x) : (x))	/*	Absolute value function	    */

#define	isalpha(c) (islower(c)||isupper(c)) 	/* true if "c" a letter	    */
#define	isdigit(c) ('0' <= (c) && (c) <= '9')	/* Ascii only!!		    */
#define	islower(c) ('a' <= (c) && (c) <= 'z')	/* Ascii only!!		    */
#define	isupper(c) ('A' <= (c) && (c) <= 'Z')	/* Ascii only!!		    */
#define iswhite(c) ((c) <= ' ' || 0177<= (c))	/* Is control / funny char  */
#define	max(x,y)   (((x) > (y)) ? (x) :  (y))	/* Max function		    */
#define	min(x,y)   (((x) < (y)) ? (x) :  (y))	/* Min function		    */
#define	tolower(c) (isupper(c) ? ((c)+040):(c))	/* translate to lower case  */
#define	toupper(c) (islower(c) ? ((c)-040):(c))	/* translate to upper case  */
#define isatty(x)  (((x)->flags & ISTTY) == 0 ? FALSE : TRUE)
