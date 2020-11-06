/* 
 *	Creat.c -- Whitesmith's simulation of V6 "creat" function
 */
#include <std.h>
creat(name,mode)

	char	*name;			/* -> Filename to create	*/
	int	mode;			/* Read / Write, etc.		*/
{
	return(create(name,1,1));	/*  Write + 1 byte records*/
}
