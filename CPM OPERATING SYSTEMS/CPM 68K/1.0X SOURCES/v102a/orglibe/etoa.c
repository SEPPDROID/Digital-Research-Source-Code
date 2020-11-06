/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#) etoa - jan 24, 1982"; */

/*
 *	IEEE Floating Point to Ascii String Conversion Routine :
 *		IEEE Standard Single Precision Representation Floating Point
 *
 *	char *
 *	etoa(f,buf,prec)
 *	float f;
 *	char *buf;
 *	int prec;
 *
 *	No more than 9 decimal digits are allowed in single precision.
 *	Largest positive number is 3.4 * 10^33 and the smallest positive
 *	number is 1.2 * 10^-38.
 *	Rely's on the fact that a long and a float are both 32 bits.
 */

#define BIAS	127L
#ifndef ISIEEE
/* if this is compiled or executed with other than ieee fp */
	float _ieeetof();
#else
	float _efake();
#endif

char *
etoa(fl,buf,prec)
long fl;	/* ieee formatted float */
char *buf;
int prec;
{
	register char *bp;
	register int exp, digit;
	float f;

	prec = (prec <= 0) ? 1 : (prec <= 9) ? prec : 9;
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
	f = _efake(fl);
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
		*bp++ = '0';	*bp++ = '.';
		while (prec--)
			*bp++ = '0';
		*bp++ = 'e';	*bp++ = '0';	*bp++ = '0';	*bp = 0;
		return(buf);
	}
	for (exp=0; f < 1.0; f = f * 10.0)	/* get negative exp */
		exp--;
	for ( ; f >= 1.0; f = f / 10.0)		/* 0.XXXXXXE00 * 10^exp */
		exp++;

	exp--;	/* for one explicit digit */
	f = f * 10.0;
	digit = f;	/* get one digit */
	f = f - digit;
	*bp++ = digit + '0';
	*bp++ = '.';
	while(prec-- > 0) {	/* get prec. decimal places */
		f = f * 10.0;
		digit = f;
		f = f - digit;
		*bp++ = digit + '0';
	}
	*bp++ = 'e';
	if (exp < 0) {
		exp = -exp;
		*bp++ = '-';
	}
	*bp++ = (exp / 10) + '0';
	*bp++ = (exp % 10) + '0';
	*bp = 0;
	return(buf);
}

#ifdef ISIEEE
/* if these routines are executed on a 68000 using ieee then no need to
   do a translation !!! */
float
_efake(lf)
float lf;
{
	return(lf);
}
#endif
