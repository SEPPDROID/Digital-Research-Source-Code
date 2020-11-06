/* index - find the index of a character in a string*/
/*		This is identical to Software Tools index.*/
index(str,chr)						/* returns index of c in str or -1*/
char *str;							/* pointer to string to search*/
char chr;							/* character to search for*/
{
	register char *s;
	register int i;

	for( s = str, i = 0; *s != '\0'; i++ )
		if( *s++ == chr )
			return(i);
	return(-1);
}
