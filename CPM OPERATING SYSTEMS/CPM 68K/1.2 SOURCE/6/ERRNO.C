/*
 * errno.c : declares storage for errno, a variable containing the last
 *		system error.
 */
#include "portab.h"
#include "osif.h"

WORD errno = 0;		/* set by low level i/o, unset by user (if at all) */
WORD _errcpm = 0;	/* assign BDOS return val when assigning errno */

l when assigning errno */

l when assigning errno */

