/****************************************************************************/
/*									    */
/*			S i g n a l   F u n c t i o n			    */
/*			-----------------------------			    */
/*									    */
/*	The "signal" function allows a routine in C to catch a 68000 	    */
/*	interrupt, and service it with a C function.			    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = signal(sig,func);					    */
/*									    */
/*	Where:								    */
/*		ret	Is always 0 under CP/M.				    */
/*		sig	Is the UNIX signal number			    */
/*		func	-> the function to service the exception	    */
/*									    */
/****************************************************************************/
#include <stdio.h>				/* Include standard stuff   */
#include <signal.h>				/* Include definitions	    */
#include <cpm.h>				/* Include BDOS Functions   */
						/****************************/
EXTERN	WORD	*_illinst();			/* -> Illegal instruction ep*/
EXTERN	WORD	*_trace();			/* -> Trace trap epa	    */
EXTERN	WORD	*_trap();			/* -> TRAP instruction epa  */
EXTERN	WORD	*_buserr();			/* -> BUSERR epa	    */
EXTERN	WORD	*_arith();			/* -> Arithmetic traps epa  */
EXTERN	BYTE	*__signal[NSIG];		/* Holds user func addresses*/
						/*			    */
LONG signal(sig,func)				/****************************/
REG	DEFAULT	sig;				/* Signal number	    */
REG	DEFAULT	(*func)();			/* Function address	    */
{						/*			    */
REG	WORD	i;				/* Temp			    */
						/*			    */
	if(sig >= NSIG || sig < 0)		/* Too big?		    */
		return(BADSIG);			/* Yes, return UNIX NFG code*/
						/****************************/
	__signal[sig] = func;			/* Save the function addr   */
						/*			    */
	switch(sig)				/* Do signal processing	    */
	{					/*			    */
	  case SIGILL:				/* Illegal instruction	    */
		_setvec( 4,&_illinst);		/* Set vector #4	    */
		_setvec( 8,&_illinst);		/* Privilege violation	    */
		_setvec(10,&_illinst);		/* Set vector #10 (line A)  */
		_setvec(11,&_illinst);		/* Set vector #11 (line F)  */
		break;				/****************************/
						/*			    */
	  case	SIGTRAP:			/* Trace trap		    */
		_setvec(9,&_trace);		/* Set vector #9	    */
		break;				/****************************/
						/*			    */
	  case	SIGIOT:				/* Bad TRAP instruction	    */
		_setvec(32,&_trap);		/* Trap 0		    */
		for(i=35; i<48; i++)		/* Traps 4 - 15		    */
			_setvec(i,&_trap);	/*			    */
		break;				/****************************/
						/*			    */
	  case	SIGBUS:				/* BUSERR PDP-11 trap 4 & 10*/
		_setvec(2,&_buserr);		/* Set nxm vector	    */
		_setvec(3,&_buserr);		/* Set addressing vector    */
		break;				/****************************/
						/*			    */
	  case	SIGFPE:				/* Arithmetic section	    */
		for(i=5; i<8; i++)		/* Set all misc vectors	    */
			_setvec(i,&_arith);	/* 			    */
		break;				/****************************/
						/*			    */
	}					/*			    */
	return(0L);				/* Return OK		    */
}						/****************************/

VOID	_setvec(vector,func)			/****************************/
	WORD	vector;				/* Vector #		    */
	BYTE	*func;				/* Function address	    */
{						/****************************/
	struct	{				/* A CP/M EPB structure	    */
	WORD	vec;				/* Vector number	    */
	BYTE	*userepa;			/* User's epa		    */
	BYTE	*bdosepa;			/* BDOS's epa		    */
	}	epb;				/****************************/
	epb.vec = vector;			/* Set up vector	    */
	epb.userepa = func;			/* and function		    */
	epb.bdosepa = 0L;			/* Clear return word	    */
	__BDOS(SETVEC,&epb);			/* Do it.		    */
}						/****************************/
