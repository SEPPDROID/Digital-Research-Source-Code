/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#)ftoa.c	1.3	12/29/83";*/

/*
 *	FFP Floating Point to Ascii String Conversion Routine :
 *		FFP Standard Single Precision Representation Floating Point
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
 *  29 dec 1983 - changed to allow zero decimal places.
 */

#define BIAS	127L

float ffptof();

char *
ftoa(fl,buf,prec)
long fl;	/* ffp formatted float */
char *buf;
int prec;
{
	register char *bp;
	register int exp, digit;
	float f;

	prec = (prec <= 0) ? 0 : (prec <= 9) ? prec : 9;
	bp = buf;
	f = ffptof(fl);	/* get floating point value */
	if (f < 0.0) {		/* negative float */
		*bp++ = '-';
		f = -f;		/* make it positive */
	}
	if (f == 0.0) {
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

