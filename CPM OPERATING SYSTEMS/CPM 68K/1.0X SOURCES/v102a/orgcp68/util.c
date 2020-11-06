/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

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
