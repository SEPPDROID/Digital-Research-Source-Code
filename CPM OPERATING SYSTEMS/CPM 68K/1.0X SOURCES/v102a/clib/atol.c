

/* atol - convert decimal number in ascii to long integer */

#include "portab.h"
#include "ctype.h"

LONG	atol(str)				/* CLEAR FUNCTION ***********/
	REG BYTE *str;
{
	REG LONG val;
	REG WORD isneg;

	val = 0L;
	isneg = FALSE;
	while( isspace(*str) )
		str++;
	if( *str == '+' )
		str++;
	else if( *str == '-' ) {
		str++;
		isneg++;
	}
	while( *str >= '0'  &&  *str <= '9' )
		val = 10 * val + ( *str++ - '0' );
	if( isneg )
		val = -val;
	return( val );
}
