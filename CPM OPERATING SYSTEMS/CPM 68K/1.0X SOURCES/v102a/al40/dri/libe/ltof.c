/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#) fpltof - Jan 7, 1983"; */

/* 
 *	Floating Point Long to Float Routine :
 *		Front End to IEEE Floating Point Package.
 *
 *		double
 *		fpltof(larg)
 *		long larg;
 *
 *	Return : Floating Point representation of Long Fixed point integer
 */

#define BIAS	127L

long
fpltof(l)
long l;
{
	register long exp;
	register int sign;

	if (l < 0L) {	/* signed ?? */
		sign = 1;
		l = -l;
	}
	else
		sign = 0;
	if (l == 0L)
		return(0L);
	exp = 23L;
	for( ; l & 0x7f000000; exp++)	/* something in upper 7 bits */
		l =>> 1;
	for( ; !(l & 0x00800000); exp--)	/* get mantissa : 1.F */
		l =<< 1;
	l =& 0x007fffff;	/* reduce to .F in 23 bits */
	if (sign)
		l =| 0x80000000;
	exp = (exp + BIAS)<<23;
	l =| exp;
	return(l);
}
