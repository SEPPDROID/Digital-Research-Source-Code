/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) strcpy.c - Sep 6, 1983
*/

char *strcpy(s,t)
register char *s, *t;
{
	register char *s1;

	s1 = s;
	while(*s++ = *t++)
		;
	return(s1);
}
