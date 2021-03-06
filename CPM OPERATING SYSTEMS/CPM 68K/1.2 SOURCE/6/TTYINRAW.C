/***************************************************************************
* 	Function: ttyinraw()
*
* 	Usage:	ret = ttyinraw(chktype)
*
*	Where:	WORD ret 	Value determined by chktype parameter.
*		WORD chktype 	What type of activity to check for, as in:
*		  0 - ret==input_char (do NOT echo), wait for a key to be 
*			pressed.  Appropriate for Raw I/O.
*		  1 - ret==pressed_key (do NOT echo) if any key pressed,
*		      ret==0 if no key has been pressed.  
*			This would be appropriate for editors and other 
*			interactive programs.
*		  2 - ret==input_char (NO echo) if user tries to interrupt, 
*		      ret==0 otherwise.  Always 0 for any O.S. 
*			with live CTRL C or equivalent.
*			this allows compute bound programs to check kb 
*			periodically to see if user wants to interrupt, 
*			and live keyboard type-ahead if appropriate for O.S.)
*		  3 - ret==1 if any key pressed, 
*		      ret==0 otherwise.  Appropriate for checking console 
*			status only, since the character is NOT input.
*
*****************************
*
* Function: 	_ttyinraw()
*
* Usage:	retchar = _ttyinraw(do_wait)
*
* Where:	WORD do_wait;	Wait for input if non-zero.
*		WORD retchar;	Character input from keyboard (no echo).
* Notes:
*	   Handles single character input independent of (CP/M) OS.
*	   Problem is that different OSs handle Raw Input in different ways.   
*	CPM-86 and CPM68K use only 0xFF, but CPM68K waits for a character.  
*	Hard loops in multi-tasking, however, waste CPU cycles, so MPM and  
*	Concurrent use 0xFD.						    
*
****************************
*
* Edits:								    
*	28-Feb-84 whf	Add ttyinraw().
*	26-Jan-84 whf	simplify for 68k compiler.
*	8-Dec-83 whf	handle PC-DOS like CPM-86			    
****************************************************************************/

#include "portab.h"
#include "osif.h"
#include "osiferr.h"
#include "osattr.h"
						/****************************/
MLOCAL WORD CharInReq = 0xFF;			/* Default CONIO Char In Rqs*/
						/****************************/
WORD _ttyinraw(DoWait)				/*			    */
    WORD DoWait;				/* Wait for key press?	    */
{						/****************************/
	REG WORD ic;				/* Input Char		    */
						/*			    */
	while((ic=__OSIF(CONIO,CharInReq))==0)	/* Wait in loop if needed   */
	{					/*			    */
	  if( !DoWait )				/* Don't wait for keypress? */
	    break;				/*   no, don't wait.	    */
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



/***************************************************************************
****************************************************************************/

WORD ttyinraw(chktype)				/* CLEAR FUNCTION ***********/
    WORD chktype;				/* type of input	    */
{
	WORD ch;				/* character input	    */

	switch(chktype) {

	case 2:					/* Check for interrupt?	    */
	    if( os_interrupts )			/* O.S. looks for CTRL C?   */
		return 0;			/*   yes, no interrupt check*/
						/*   no, drop thru...	    */
	case 0:					/* Return character or 0    */
	    return _ttyinraw(0);		/* 0 means don't wait	    */

	case 3:					/* Return console status    */
	    return __OSIF(C_STAT,0);		/* 1 if key pressed	    */

	default:
	case 1:					/* Return char with wait    */
	    return _ttyinraw(1);		/* 1 means wait		    */
	}
}
   return _ttyinraw(1);		/* 1 means wait		    */
	}
}
   return _ttyinraw(1);		/* 1 means wait		    */
	}
}
