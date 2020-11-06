/***************************************************************************
*
*		b l k m o v e   F u n c t i o n
*		-------------------------------
*	Copyright 1983, by Digital Research Inc.
*
*	"blkmove()" moves a block of bytes from one address to another.
*
*	Calling sequence:
*		blkmove(to,from,nn)
*	Where:
*		'to' points to destination
*		'from' points to source, and
*		'nn' is the number of chars to move
*
***************************************************************************/
#include <portab.h>

VOID	blkmove(to,from,nn)			/* CLEAR FUNCTION ***********/
    BYTE *to, *from;
    WORD nn;
{
	if( to < from ) 
		while(nn--)
			*to++ = *from++;
	else 
		for( to += nn, from += nn; nn--; )
			*--to = *--from;
}
from ) 
		while(nn--)
			*to++ = *from++;
	else 
		for( to += nn, from += nn; nn--; )
			*--to = *--from;
}
from ) 
		while(nn--)
			*to++ = *from++;
	else 
		for( to += nn, from += nn; nn--; )
			*--to = *--from;
}
