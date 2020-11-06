/* atol - convert decimal number in ascii to long integer */
#include <portab.h>
#include <ctype.h>

LONG atol(s)
	REG BYTE *s;
{
	REG LONG val;
	REG WORD isneg;

	val = 0L;
	isneg = FALSE;
	while( isspace(*s) )
		s++;
	if( *s == '+' )
		s++;
	else if( *s == '-' ) {
		s++;
		isneg++;
	}
	while( *s >= '0'  &&  *s <= '9' )
		val = 10 * val + ( *s++ - '0' );
	if( isneg )
		val = -val;
	return( val );
}
