/**********************************************************************
*
*			_ c l e a n u p   F u n c t i o n
*			---------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	"_cleanup" closes all buffered files
*
*	Calling sequence:
*		_cleanup()
*
***************************************************************************/

#include "stdio.h"

_cleanup()
{
REG	WORD ii;

	for( ii=0; ii<MAXFILES; ii++ )
		fclose(&_iob[ii]);
/*	_m_term();			/* CLEAR termination routine */
}
EAR termination routine */
}
EAR termination routine */
}
