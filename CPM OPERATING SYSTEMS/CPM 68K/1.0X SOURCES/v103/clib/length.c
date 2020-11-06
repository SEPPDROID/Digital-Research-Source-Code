
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) __length.c - Sep 12, 1983  REGULUS 4.1
*/

__length(s)
char	*s;
{
	register int	l;
	register char	*p;

	p = s;
	l = 0;
	while (*p++)
		l++;
	return(l);
}
