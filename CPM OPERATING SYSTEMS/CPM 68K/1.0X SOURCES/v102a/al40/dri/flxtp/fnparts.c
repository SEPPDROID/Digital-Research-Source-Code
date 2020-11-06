
#include <stdio.h>
#include <ctype.h>

#define PERIOD	'.'
#define SLASH	'/'

fnparts (origfn, base, maxbase, suffix, maxsuffix)
char *origfn, *base, *suffix;
int maxbase, maxsuffix;
{
	char *end, *bp, *sp, *endbase;
	int i;

	/* point past last char of orignal filename */
	end = origfn;
	while (*end++);

	/* find suffix by scanning backwards for '.'
	 * but don't backup before a pathname
	 */
	sp = end;
	while ( *sp != PERIOD & *sp != SLASH & sp != origfn) sp--;

	if ( *sp != PERIOD ) { /* no suffix found */
		endbase = end;
		*suffix = '\0';
	}
	else {
		endbase = sp++;		/* and skip the '.' */

		i=0;
		while ( i< (maxsuffix-1)) {
			if (islower(*sp) | isdigit(*sp) | isupper(*sp) )
				*suffix++ = *sp++;
			else sp++;	/* ignore other chars */
			if ( *sp == '\0' ) break;
		} /* while */
		*suffix = '\0';		/* terminate with null */
	} /* end of getting the suffix */

	/* find base by scanning backwards from suffix */
	bp=endbase;
	while ( *bp != SLASH & bp != origfn ) bp--;
	if ( *bp == SLASH ) bp++;	/* skip slash, if found */

	i=0;
	while ( i < (maxbase-1) ) {	/* save the base */
		if (islower(*bp) | isdigit(*bp) | isupper(*bp))
			*base++ = *bp++;
		else bp++;	/* ignore other chars */
		if ( bp == endbase ) break;
	} /* save the base */

	*base = '\0';	/* terminate with null */
} /* fnparts */


