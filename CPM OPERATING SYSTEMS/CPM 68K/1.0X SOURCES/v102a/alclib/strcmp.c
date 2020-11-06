/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) strcmp.c - Sep 6, 1983
*/

strcmp(s,t)
register char *s, *t;
{
	while( *s ) {
		if( *s++ != *t++ ) {
			s--;
			t--;
			break;
		}
	}
	return( *s - *t );
}
