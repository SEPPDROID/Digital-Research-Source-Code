/*********************************************************************
*
*			f c l o s e   F u n c t i o n
*			-----------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"fclose" flushes a stream (buffered file) and releases the
*	channel and any allocated buffers.
*
*	Calling sequence:
*		ret = fclose(stream)
*	Where:
*		stream -> file to be closed (FILE *)
*		ret = SUCCESS or FAILURE (if IO error)
*
*****************************************************************************/

#include "stdio.h"

WORD	fclose(sp)				/* CLEAR FUNCTION ***********/
REG	FILE *sp;				/* stream to close	    */
{						/*			    */
						/****************************/
	if( sp->_flag & (_IOREAD|_IOWRT) )	/* is it closeable?	    */
	{					/* yup...		    */
		fflush(sp);			/* do the flush		    */
		if( sp->_flag & _IOABUF )	/* was buf alloc'd?	    */
			free(sp->_base);	/* free it ifso		    */
		sp->_base = sp->_ptr = NULL;	/* reset these		    */
		sp->_cnt = 0;			/*			    */
	}					/* reset all flags	    */
	sp->_flag &= ~(_IOREAD|_IOWRT|_IOABUF|_IONBUF|_IOERR|_IOEOF|_IOLBUF);
	return(close(sp->_fd));			/* and return		    */
}						/****************************/
