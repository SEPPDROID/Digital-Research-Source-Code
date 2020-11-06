atoi(s)
char *s;
{
	register int n, sign;

	while( *s==' ' || *s=='\n' || *s == '\t')
		s++;
	sign = 1;
	if( *s == '+' || *s == '-' )
		sign = (*s++=='+') ? 1 : -1;
	for( n = 0; *s >= '0' && *s <= '9'; s++ )
		n = (n * 10) + (*s - '0');
	return( sign * n );
}
