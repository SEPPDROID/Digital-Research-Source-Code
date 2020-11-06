
/* atoi - convert decimal number in ascii to integer */

#include "portab.h"
#include "ctype.h"

WORD	atoi(str)				/* CLEAR FUNCTION ***********/
	REG BYTE *str;
{
	REG WORD val;
	REG WORD isneg;

	val = 0;
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
 <= '9' )
		val = 10 * val + ( *str++ - '0' );
	if( isneg )
		val = -val;
	return( val );
}
 <= '9' )
		val = 10 * val + ( *str++ - '0' );
	if( isneg )
		val = -val;
	return( val );
}
