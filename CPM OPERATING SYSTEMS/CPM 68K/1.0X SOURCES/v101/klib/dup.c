/*
 *	El-Kludg-o Dup routine.  Takes advantage of the fact that
 *	stdout is not closed by Bill Allen's stuff.
 */
dup(n)
	register int n;
{
	return(n);
}
