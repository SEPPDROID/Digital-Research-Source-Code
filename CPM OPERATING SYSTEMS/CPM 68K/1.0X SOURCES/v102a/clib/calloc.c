/********************************************************************
*
*	calloc.c - memory allocator for sets of elements
*	zalloc	 - memory allocator like malloc only zeros storage.
*
*	BYTE *calloc(nelem,sizelem)
*		WORD nelem, sizelem;
*
*	Returns a pointer to a region of (zero filled) memory large
*	enough to hold 'nelem' items each of size 'sizelem'.
*	Returns NULL if not enough memory.
*
*	BYTE	*zalloc(nbytes)
*		UWORD	nbytes;
*
*	Returns a pointer to a region of zero filled memory nbytes long.
*	Returns NULL if not enough memory.
*
*********************************************************************/

#include "portab.h"

BYTE *	zalloc(nbytes)				/* CLEAR FUNCTION ***********/
	WORD nbytes;		/* number of bytes */
{
REG	BYTE *rp;		/* pointer to region */
	BYTE *malloc();

	if( (rp = malloc(nbytes)) == NULLPTR) return(NULLPTR);
	blkfill( rp, NULL, nbytes );
	return(rp);
}

BYTE *	calloc(nelem,sizelem)				/* CLEAR FUNCTION ***********/
	WORD nelem,		/* number of elements */
	     sizelem;		/* size of element */
{
	return(zalloc(sizelem*nelem));
}
