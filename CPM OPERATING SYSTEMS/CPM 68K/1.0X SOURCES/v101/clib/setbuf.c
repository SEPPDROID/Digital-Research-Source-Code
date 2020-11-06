/* setbuf - assign a buffer to a stream, after open but
	before any i/o. 
	Returns SUCCESS if ok, FAILURE o.w. */
#include <stdio.h>

WORD setbuf(sp,buf)
REG	FILE *sp;
REG	BYTE *buf;
{
	if( sp->_base != NULL )
		return(FAILURE);
	sp->_base = sp->_ptr = buf;
	if( buf == NULL ) 
		sp->_flag |= _IONBUF;
	else	sp->_flag &= ~_IONBUF;
	return(SUCCESS);
}
