/*
	Copyright 1982, 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#)ftoa.c	1.4	12/29/83";*/

/*
 *	IEEE Floating Point to Ascii String Conversion Routine :
 *		IEEE Standard Single Precision Representation Floating Point
 *
 *	char *
 *	ftoa(f,buf,prec)
 *	float f;
 *	char *buf;
 *	int prec;
 *
 *	No more than 9 decimal digits are allowed in single precision.
 *	Largest positive number is 3.4 * 10^33 and the smallest positive
 *	number is 1.2 * 10^-38.
 *	Rely's on the fact that a long and a float are both 32 bits.
 *  29 dec 83 - allow 0 decimal places !!!
 */

#define BIAS	127L

#ifdef ISIEEE
	float _ffake();
#else
	float _ieeetof();
#endif

char *
ftoa(fl,buf,prec)
long fl;	/* ieee formatted float */
char *buf;
int prec;
{
	register char *bp;
	register int exp, digit;
	float f;

	prec = (prec <= 0) ? 0 : (prec <= 9) ? prec : 9;
	bp = buf;
	if(fl == 0) goto shortcut;
	if(fl == 0x7f800000) {	/* positive infinity */
		strcpy(bp,"+infinity");
		return(bp);
	}
	if(fl == 0xff800000) {	/* negative infinity */
		strcpy(bp,"-infinity");
		return(bp);
	}
#ifndef ISIEEE
	f = _ieeetof(fl);		/* get floating point value */
#else
	f = _ffake(fl);
#endif
#ifndef ISIEEE
	if (f < 0.0) {			/* negative float */
#else
	if (fl & 0x80000000) {	/* negative float */
#endif
		*bp++ = '-';
		f = -f;		/* make it positive */
	}
#ifndef ISIEEE
	if (f == 0.0) {
#else
	if (fl == 0) {
#endif
shortcut:
		*bp++ = '0';	
		if(prec) {
			*bp++ = '.';
			while (prec--)
				*bp++ = '0';
		}
		*bp = 0;
		return(buf);
	}
	for (exp=0; f < 1.0; f = f * 10.0)	/* get negative exp */
		exp--;
	for ( ; f >= 1.0; f = f / 10.0)		/* 0.XXXXXXE00 * 10^exp */
		exp++;

	if (exp<=0)	/* one significant digit */
		*bp++ = '0';
	for ( ; exp>0; exp--) {	/* get significant digits */
		f = f * 10.0;
		digit = f;	/* get one digit */
		f = f - digit;
		*bp++ = digit + '0';
	}
	if(prec) {
		*bp++ = '.';
		for( ; exp<0 && prec; prec--, exp++)	/* exp < 0 ? */
			*bp++ = '0';
		while(prec-- > 0) {
			f = f * 10.0;
			digit = f;	/* get one digit */
			f = f - digit;
			*bp++ = digit + '0';
		}
	}
	*bp = 0;
	return(buf);
}

#ifdef ISIEEE
/* if these routines are executed on a 68000 using ieee then no need to
   do a translation !!! */
float
_ffake(lf)
float lf;
{
	return(lf);
}
#endif
