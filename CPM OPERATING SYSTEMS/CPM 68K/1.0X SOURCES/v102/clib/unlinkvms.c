/*
 *	Unlink.c -- Whitesmith's version of UNIX unlink function
 */
unlink(name)
	char	*name;			/* -> file name			*/
{
	return(remove(name));
}
