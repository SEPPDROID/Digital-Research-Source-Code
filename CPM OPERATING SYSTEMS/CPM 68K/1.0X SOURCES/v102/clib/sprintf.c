/************************************************************************
*
*			s p r i n t f   F u n c t i o n
*			-------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"sprintf" prints args specified in format string to a string
*	pointed to by str.  No checks for str overflow are possible.
*	sprintf returns str.
*
*	Calling sequence:
*		s = sprintf(string,fmt,arg1,arg2,...argn);
*	Where:
*		s = string = place to put info
*		fmt -> a string specifying how arg1-n are to be printed.
*
**************************************************************************/
#include <stdio.h>
BYTE *sprintf(string,fmt,args)
	BYTE *string,
	     *fmt,
	     *args;
{
	FILE stream; 				/* pseudo stream tab	    */
REG	FILE *sp;				/* ptr thereto		    */
	WORD rv;				/* return val from _doprt   */
						/*			    */
	sp = &stream;				/* point to pseudo stream   */
	sp->_cnt = 32767;			/* assume 'infinite' buf    */
	sp->_ptr = sp->_base = string;		/* stream buf -> string	    */
	sp->_flag = _IOWRT | _IOSTRI;		/* writeable string	    */
	sp->_fd = -1;				/* insure no real i/o	    */
	rv = _doprt(sp,fmt,&args);		/* do the print		    */
	putc(NULL,sp);				/* NULL terminate string    */
	return(rv==FAILURE ? NULL : string);	/* tell user what happened  */
}
