/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#) atof - dec 29, 1982"; */

/*
 *	Ascii String to IEEE Floating Point Routine :
 *		IEEE Standard Single Precision Representation Floating Point
 *
 *	float
 *	atof(buf)
 *	char *buf;
 *
 *	No more than 9 significant digits are allowed in single precision.
 *	Largest positive number is 3.4 * 10^33 and the smallest positive
 *	number is 1.2 * 10^-38.
 *	Rely's on the fact that a long and a float are both 32 bits.
 */

#define BIAS	127L
#define EXPSIZ	4
#define FRACSIZ	20

long fptoieee();
float strbin();
float power10();

long
atof(buf)
char *buf;
{
	char ibuf[FRACSIZ], ebuf[EXPSIZ];
	register char *ip, *ep;
	long ieee;		/* return value */
	int dp, esign, isign, ebin, places;
	float ibin, fp;

	ip = &ibuf; ep = &ebuf; dp = 0; places = 0L;
	while (*buf == ' ' || *buf == '\t')	/* ignore white spaces */
		buf++;
	isign = (*buf == '-');
	if (*buf == '-' || *buf == '+')
		buf++;
	while (*buf && *buf != 'e' && *buf != 'E') {
		if (*buf == '.')
			dp++;
		else {	/* digit seen */
			*ip++ = *buf;
			if (dp)
				places++;
		}
		buf++;
	}
	*ip = 0;
	if (*buf == 'e' || *buf == 'E') {	/* exponent string */
		buf++;
		esign = (*buf == '-');
		if (*buf == '-' || *buf == '+')
			buf++;
		while (*buf)	/* get exponent string */
			*ep++ = *buf++;
	}
	*ep = 0;
	ibin = strbin(ibuf);
	ebin = atoi(ebuf);
	places = (esign) ? -ebin - places : ebin - places;
	fp = ibin * power10(places);
	ieee = fptoieee(fp);
	if (isign)	/* negative float */
		ieee =| 0x80000000;
	return( ieee );
}

float
power10(pwr)			/* 10^pwr */
int pwr;
{
	float f;

	if (pwr < 0L)	/* negative power */
		for (f = 1.0; pwr < 0; pwr++)
			f = f / 10.0;
	else			/* positive power */
		for (f = 1.0; pwr > 0; pwr--)
			f = f * 10.0;
	return(f);
}

long
fptoieee(f)			/* convert current machine float to ieee rep */
float f;			/* unsigned float... */
{
	register long exp, l;

	if (f == 0.0)
		return(0L);
	exp = 0L;
	for( ; f >= 2.0; f = f / 2.0)
		exp++;
	for( ; f < 1.0; f = f * 2.0)
		exp--;
	f = f - 1.0;		/* implicit 1, eg. 1.F */
	if (f != 0.0)
		f = f * 8388608.0;	/* 2 ^ 23 */
	l = f;
	exp =+ BIAS;
	l =| ((exp<<23) & 0x7f800000);
	return(l);
}

float
strbin(p)			/* decimal string => binary long */
char *p;
{
	float f;

	for (f = 0.0; *p >= '0' && *p <= '9'; p++) {
		f = f * 10.0;
		f = f + (*p - '0');
	}
	return(f);
}
