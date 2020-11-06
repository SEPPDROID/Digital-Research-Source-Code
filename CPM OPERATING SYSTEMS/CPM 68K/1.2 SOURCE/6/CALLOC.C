/********************************************************************
*
*	calloc.c - memory allocator for sets of elements
*	zalloc	 - memory allocator like malloc only zeros storage.
*
*	BYTE *calloc(nelem,sizelem)
*		UWORD nelem, sizelem;
*
*	Returns a pointer to a region of (zero filled) memory large
*	enough to hold 'nelem' items each of size 'sizelem'.
*	Returns NULL if not enough memory, or allocation too large
*	(on 8086).
*
*	BYTE	*zalloc(nbytes)
*		UWORD	nbytes;
*
*	Returns a pointer to a region of zero filled memory nbytes long.
*	Returns NULL if not enough memory.
*
*********************************************************************/

#include "portab.h"
#include "osif.h"

BYTE *	zalloc(nbytes)				/* CLEAR FUNCTION ***********/
	UWORD nbytes;		/* number of bytes */
{
REG	BYTE *rp;		/* pointer to region */
	BYTE *malloc();

	if( (rp = malloc(nbytes)) == NULLPTR) return(NULLPTR);
	blkfill( rp, NULL, nbytes );
	return(rp);
}

BYTE *	calloc(nelem,sizelem)			/* CLEAR FUNCTION ***********/
	UWORD nelem,		/* number of elements */
	     sizelem;		/* size of element */
{
REG	LONG size;

	size = sizelem*nelem;
#if I8086
	if( size > 65535L )
		return NULLPTR;
#endif
	return zalloc((UWORD)size);
}
e = sizelem*nelem;
#if I8086
	if( size > 65535L )
		return NULLPTR;
#endif
	return zalloc((UWORD)size);
}
e = sizelem*nelem;
#if I8086
	if( size > 65535L )
		return NULLPTR;
#endif
	return zalloc((UWORD)size);
}
