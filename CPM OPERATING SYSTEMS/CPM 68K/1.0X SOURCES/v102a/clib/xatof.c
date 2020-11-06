/*
 *	Dummy routine for use with floating point stuff ...
 */
#include <portab.h>
DOUBLE	atof();
DOUBLE	_atof(buf)
BYTE	*buf;
{
	return(atof(buf));
}
