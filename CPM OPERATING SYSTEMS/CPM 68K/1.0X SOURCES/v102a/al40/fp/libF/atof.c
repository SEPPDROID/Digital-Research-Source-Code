/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#) atof - dec 29, 1982"; */

/*
 *	Ascii String to FFP Floating Point Routine :
 *		FFP Standard Single Precision Representation Floating Point
 *
 *	float
 *	atof(buf)
 *	char *buf;
 *
 *	No more than 9 significant digits are allowed in single precision.
 *	Largest positive number is 3.4 * 10^18 and the smallest positive
 *	number is 1.2 * 10^-20.
 *	Rely's on the fact that a long and a float are both 32 bits.
 */

#define EXPSIZ	4
#define FRACSIZ	20

long fptoffp();
float strbin();
float power10();

long
atof(buf)
char *buf;
{
	char ibuf[FRACSIZ], ebuf[EXPSIZ];
	register char *ip, *ep;
	long ffp;
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
	ffp = fptoffp(fp);
	if (isign)	/* negative float */
		ffp =| 0x80;
	return( ffp );
}

float
power10(pwr)			/* 10^pwr */
int pwr;
{
	float f;

	if (pwr < 0)	/* negative power */
		for (f = 1.0; pwr < 0; pwr++)
			f = f / 10.0;
	else			/* positive power */
		for (f = 1.0; pwr > 0; pwr--)
			f = f * 10.0;
	return(f);
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
