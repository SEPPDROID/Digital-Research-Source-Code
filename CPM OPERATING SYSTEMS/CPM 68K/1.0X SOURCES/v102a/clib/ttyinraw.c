/***************************************************************************
 * TTYINRAW.C - handles raw input independent of (CP/M) OS.		    
 *	Problem is that different OSs handle Raw Input in different ways.   
 *	CPM-86 and CPM68K use only 0xFF, but CPM68K waits for a character.  
 *	Hard loops in multi-tasking, however, waste CPU cycles, so MPM and  
 *	Concurrent use 0xFD.						    
 *
 * Edits:								    
 *	26-Jan-84 whf	simplify for 68k compiler.
 *	8-Dec-83 whf	handle PC-DOS like CPM-86			    
 ****************************************************************************/

#include "portab.h"
#include "osif.h"
#include "osiferr.h"
						/****************************/
MLOCAL WORD CharInReq = 0xFF;			/* Default CONIO Char In Rqs*/
						/****************************/
WORD _ttyinraw()				/*			    */
{						/****************************/
	REG WORD ic;				/* Input Char		    */
						/*			    */
	while((ic=__OSIF(CONIO,CharInReq))==0)	/* Wait in loop if needed   */
	{					/*			    */
#if CPM   /*================================================================*/
	  /* This goes inside the loop; for Concurrent, it will be called   */
	  /* only once, while for other O.S.'s, it doesn't matter what we   */
	  /* do while we're waiting for a char.				    */
	    __OSIF(S_BDOSVER,0);		/* Puts AX in __cpmrv	    */
	    if( __cpmrv & 0x0500 )		/* Concurrent or MPM	    */
		CharInReq = 0xFD;		/* Change Char Input Parm   */
#endif    /*================================================================*/
	}					/*			    */
	return ic;				/*			    */
}						/****************************/
