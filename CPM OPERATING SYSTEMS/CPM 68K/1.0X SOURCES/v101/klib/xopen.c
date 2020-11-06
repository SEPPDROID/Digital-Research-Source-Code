/****************************************************************************/
/*									    */
/*			_ o p e n   F u n c t i o n			    */
/*			---------------------------			    */
/*									    */
/*	Function "_open" is used to parse the CP/M fcb and open or create   */
/*	the requested file.  Created files are deleted first, to avoid 	    */
/*	directory problems.						    */
/*									    */
/*	Calling Sequence:						    */
/*									    */
/*		ret = _open(ch,filnam,bdosfunc);			    */
/*									    */
/*	Where:								    */
/*		ch	Is a vacant channel number			    */
/*		filnam	Is a null-terminated CP/M filename		    */
/*		bdosfunc Is the desired BDOS function to perform	    */
/*									    */
/*	This routine may also be used to delete files as well.		    */
/*									    */
/****************************************************************************/
#include "stdio.h"
#include "cpm.h"
WORD	_open(ch,filnam,bdosfunc)		/****************************/
						/*			    */
	WORD	ch;				/* Channel number	    */
REG	BYTE	*filnam;			/* -> filename		    */
	WORD	bdosfunc;			/* BDOS Function	    */
{						/****************************/
EXTERN	FILE	*_getc();			/* fd -> fp xlate routine   */
REG	FILE	*fp;				/* -> ccb area		    */
REG	struct xfcb	*fcbp;			/* -> FCB area in ccb	    */
REG	BYTE	*p;				/* Temp character pointer   */
REG	WORD	i;				/* Character index	    */
						/****************************/
	fp = _getc(ch);				/* Fetch ccb pointer	    */
	fcbp = &(fp ->fcb);			/* Fetch fcb pointer	    */
						/****************************/
	if(*(filnam+1) == ':')			/* Drive ID present?	    */
	{					/* 	If here, yes	    */
		fcbp->drive = toupper(*filnam)	/* Load drive ID	    */
			      - 'A' + 1;	/* 1 for a:, 2 for b:, ...  */
		filnam += 2;			/* Bump past ':'	    */
	}					/****************************/
						/*			    */
	i = 0;					/* index		    */
						/****************************/
	while (*filnam > ' ' && i < 8 &&	/* 			    */
		*filnam != '.')			/* Terminate on ext.	    */
	{					/*			    */
		fcbp -> fname[i++] = 		/* Load next character	    */
			toupper(*filnam);	/* 			    */
		filnam++;			/* Increment		    */
	}					/*			    */
						/****************************/
	if (*filnam == '.')			/* Extension specified?	    */
	{					/* Yes			    */
	  filnam++;				/* Bump past '.'	    */
	  i = 0;				/* set index to 0	    */
	  while(*filnam > ' ' && i < 3)		/* copy			    */
	  {					/*			    */
		fcbp -> ftype[i++] = 		/*			    */
			toupper(*filnam);	/*	file type	    */
		filnam++;			/*			    */
	  }					/*			    */
	}					/****************************/
						/*			    */
	if(bdosfunc == CREATE)			/* Creating file?	    */
		__BDOS(DELETE,fcbp);		/* Yes, delete it first	    */
						/****************************/
	i = __BDOS(bdosfunc,fcbp);		/* Do requested operation   */
	return((i<=3) ? SUCCESS : FAILURE);	/* Binary return code	    */
}						/****************************/
