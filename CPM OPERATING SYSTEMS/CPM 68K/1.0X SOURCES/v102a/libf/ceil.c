/*
        Copyright 1983
        Alcyon Corporation
        8716 Production Ave.
        San Diego, Ca.  92121
*/

/*char *version "@(#)ceil.c     1.2    10/19/83";*/

/* ceil - returns the smallest integer (as a double precision
                  number) not greater than x. */

double
ceil(x)
double x;
{
        register long i;
        double retval;

        if( x > 0 )
                x += 0.999999999999;
        i = x;
        retval = i;
        return( retval );
}

