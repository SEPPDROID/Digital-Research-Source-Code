/********************************************************************
*
*	calloc.c - memory allocator for sets of elements
*
*	BYTE *calloc(nelem,sizelem)
*		WORD nelem, sizelem
*
*	Returns a pointer to a region of (zero filled) memory large
*	enough to hold 'nelem' items each of size 'sizelem'.
*	Returns NULL if not enough memory.
*
*********************************************************************/

#include <portab.h>
#include <cpm.h>

BYTE *calloc(ne,se)
REG	WORD ne,		/* number of elements */
	     se;		/* size of element */
{
REG	WORD rs;		/* region size to be allocated */
REG	BYTE *rp;		/* pointer to region */
	BYTE *malloc();

	if( (rp=malloc(rs =(se*ne))) == NULL)
		return(NULL);
	for( ; rs > 0; )
		rp[--rs]=ZERO;
	return(rp);
}
