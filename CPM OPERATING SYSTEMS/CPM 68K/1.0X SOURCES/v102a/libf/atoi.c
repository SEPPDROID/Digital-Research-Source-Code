/* atoi - convert decimal number in ascii to integer */
#include <portab.h>
#include <ctype.h>

WORD atoi(s)
	REG BYTE *s;
{
	REG WORD val;
	REG WORD isneg;

	val = 0;
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

