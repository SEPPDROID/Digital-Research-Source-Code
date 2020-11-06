/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*char *version "@(#) fmod - Feb 11, 1983";

/* fmod - returns the number f such that x = iy + f, and
		  0 <= f <= y. */

double
fmod(x,y)
double x;
double y;
{
	double z;
	double retval;
	register long i;
	double fabs();
	double absx;
	double absy;

	absx = fabs(x);
	absy = fabs(y);
	for(z = absx; z - absy >= 0. ; z -= absy)
			;
	i = z;
	if( x < 0.0 )
		i *= -1;
	retval = i;
	return( retval );
}
