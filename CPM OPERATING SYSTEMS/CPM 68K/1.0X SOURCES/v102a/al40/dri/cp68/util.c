atoi(as)
char *as;
{
	register int n, sign;
	register char *s;

	s = as;
	while( *s==' ' || *s=='\n' || *s == '\t')
		s++;
	sign = 1;
	if( *s == '+' || *s == '-' )
		sign = (*s++=='+') ? 1 : -1;
	for( n = 0; *s >= '0' && *s <= '9'; s++ )
		n = (n * 10) + (*s - '0');
	return( sign * n );
}
