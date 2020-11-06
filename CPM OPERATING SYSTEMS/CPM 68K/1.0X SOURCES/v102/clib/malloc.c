/********************************************************************
*	malloc, free - memory allocator mechanism
*
*	BYTE *malloc(numbytes) 
*		WORD numbytes
*	Returns a pointer to an area of memory at least 'numbytes'
*	in length.
*	Warning: the size of the area is right below the region:
*	do not overwrite!
*	Returns NULL if not enough memory.
*
*	free(mptr)
*		BYTE *mptr
*	Frees the region pointed to by 'mptr'.
*	Warning: if mptr does not contain a pointer previously
*	obtained from malloc, the memory structure will probably
*	get corrupted!
*
*	BYTE *realloc(mptr,numbytes)
*		BYTE *mptr
*		WORD numbytes
*	Returns a pointer to a (probably different) region of memory
*	(numbytes long) containing the same data as the old region 
*	pointed to by mptr.
*	Returns NULL if not enough memory.
*
********************************************************************/

#include <portab.h>



/* malloc - general purpose memory allocator
* This function allocates the requested number of bytes (nbytes) and returns
*	a pointer to this space to the calling function.
*	The memory is requested from the O.S. in larger chunks, and
*	free space is managed as a ring of free blocks.  Each block
*	contains a pointer to the next block (s.ptr), a block size (s.size),
*	and the space itself.
*	Alignment is handled by assuming that sizeof(FB_HDR) is an aligned
*	quantity, and allocating in multiples of that size.
*/

struct hdr {
	struct hdr *ptr;
	UWORD size;
};
typedef struct hdr FB_HDR;			   /* free block header type */


MLOCAL FB_HDR _afreebase = { &_afreebase, 0 };      /* initial (empty) block */
MLOCAL FB_HDR *_aflistptr = NULL; 		             /* ring pointer */

#define AOFFS 1                  /* alignment offset: 0=byte, 1=word, 3=quad */
#define AMASK(c) ((char *)((long)(c) & (long)~AOFFS)) /* mask alignment bits */
#define AFUDGE 4	  /* number multiples leeway for passing block as is */


	BYTE *
malloc(nbytes)
	UWORD nbytes;				   /* number bytes requested */
{
	FB_HDR *getmemory();			 /* local fn, gets from O.S. */
	REG FB_HDR *cp,				    /* current freeblock ptr */
		   *pp,				   /* previous freeblock ptr */
		   *np;					/* new freeblock ptr */
	REG UWORD nmults;			 /* multiples of FB_HDR size */

	nmults = 1+(nbytes+sizeof(FB_HDR)-1)/sizeof(FB_HDR);
#ifdef DEBUG4
	TRACE("malloc(%ui) = %ui units\n",nbytes,nmults);
	if( sizeof(FB_HDR) != AMASK(sizeof(FB_HDR)) )
		TRACE("bad alignment\n");
#endif
	if( _aflistptr == NULL ) {		/* initialize list if needed */
		_afreebase.ptr = _aflistptr = &_afreebase;
		_afreebase.size = 0;
	}
	pp = _aflistptr;			  /* init prev freeblock ptr */
	for( cp=pp->ptr; ; cp=(pp=cp)->ptr ) {
		if( cp->size >= nmults ) {
			if( cp->size <= nmults + AFUDGE) /* if size is close */
				pp->ptr = cp->ptr;        /* use block as is */
			else {			    /* cut up this freeblock */
				np = cp + nmults;
				np->size = cp->size - nmults;
				np->ptr = cp->ptr;
				pp->ptr = np;
				cp->size = nmults;
			}
			_aflistptr = pp;      /* start search here next time */
			return( (BYTE *)(cp+1) );
		}
		if( cp==_aflistptr )	  /* wrapped around ring: need space */
			if( (cp=getmemory(nmults)) == NULL )
				return( (BYTE *)NULL );
	}
}


/* getmemory - gets memory from O.S. 
* This function requests memory from the O.S. in multiples (ACHUNKS)
*	of the requested number of units (numu), in order to minimize
*	the number of calls to the O.S.
*	Function cleans up pointer alignment and adds to free list.
*/

#define ACHUNKS 64 				 /* chunks to alloc from O.S.*/

	MLOCAL FB_HDR *
getmemory(numu)
	UWORD numu;				          /* number of units */
{
	BYTE *sbrk();				  /* obtain memory from O.S. */
	REG BYTE *mmp;				      /* more memory pointer */
	REG FB_HDR *fbp;			       /* free block pointer */
	REG WORD utg;					     /* units to get */

#ifdef DEBUG4
	TRACE(" getmemory ");
#endif
	utg = ACHUNKS * ((numu+ACHUNKS-1) / ACHUNKS);

	mmp = sbrk(utg * sizeof(FB_HDR));      /* sbrk wants number of bytes */

	if( mmp == (char *)-1 )       /* sbrk = -1 means no memory available */
		return( (BYTE *)NULL );
	mmp = AMASK(mmp + AOFFS);    /* alignment handling: nop if good sbrk */
	fbp = (FB_HDR *)mmp;
	fbp->size = utg;
	free( (BYTE *)(fbp+1) );                         /* add to free list */
	return(_aflistptr);				    /* set in 'free' */
}


/* free - adds memory back to free block list.
* This function assumes freed memory pointer (fmp) points to free space
*	preceeded by a FB_HDR structure.
*	Note that free block is inserted in memory address order,
*	to facilitate compaction.
*	Disaster if random pointer is linked into this list.
*/

	VOID
free(fmp)
	BYTE *fmp;
{
	REG FB_HDR *cp,				      /* current freeblk ptr */
		   *pp;				     /* previous freeblk ptr */

	cp = (FB_HDR *)fmp - 1;	          /* backup pointer to before struct */
#ifdef DEBUG4
	TRACE("free, size=%ui ",cp->size);
#endif
	for( pp=_aflistptr;		   /* start where we left off before */
			!( cp>pp  &&  cp < pp->ptr ); /*break if right place */
			pp=pp->ptr ) {
		if( pp >= pp->ptr  &&  (cp < pp->ptr  ||  cp>pp) )
			break;		 /* break also if either end of list */
		if( cp==pp ) {		     /* block has already been freed */
			_aflistptr = pp; 		      /* that's okay */
			return;
		}
	}
#ifdef DEBUG4
	if( cp>pp  &&  cp < pp->ptr ) { 	 /* if cp is within the list */
		if( cp+cp->size > pp->ptr ) 	 		   /* ASSERT */
			TRACE("\t*** OVERLAPPING HI BLK ***\n");
		if( pp+pp->size > cp ) 			   /* ASSERT */
			TRACE("\t*** OVERLAPPING LO BLK\n");
	}
#endif

	if( cp+cp->size == pp->ptr ) {	/* check if we can merge w. next blk */
		cp->size += pp->ptr->size;
		cp->ptr = pp->ptr->ptr;
#ifdef DEBUG4
		TRACE("++free merge hi(%ui)++",cp->size);
#endif
	} else
		cp->ptr = pp->ptr;

	if( pp+pp->size == cp ){        /* check if we can merge w. prev blk */
		pp->size += cp->size;
		pp->ptr = cp->ptr;
#ifdef DEBUG4
		TRACE("--free merge lo(%ui)-- ",pp->size);
#endif
	} else
		pp->ptr = cp;

#ifdef DEBUG4
	TRACE("\n");
#endif
	_aflistptr = pp;		      /* search from here next time */
}



	BYTE *
realloc(ptr,siz)
	BYTE *ptr;
	UWORD siz;
{
	BYTE *np, *np2;
	FB_HDR *fp;
	UWORD ss;

#ifdef DEBUG4
	TRACE("realloc(,%ui)",siz);
#endif
	fp = ((FB_HDR *)ptr) - 1;
	ss = sizeof(FB_HDR) * fp->size;	       /* get size of old (in bytes) */
	if( (np=malloc(siz)) == NULL)
		return( (BYTE *)NULL );
	free(ptr);		 /* this would be nicer before the malloc... */
#ifdef DEBUG4
	TRACE(" moved %i bytes\n",np-ptr);
#endif
	if( ptr != np ) {		     /* don't copy if hasn't changed */
		if( siz < ss )
			ss = siz;			 /* ss = min(ss,siz) */
		if( np < ptr )
			for( np2=np; ss; ss-- )
				*np2++ = *ptr++;		  /* copy up */
		else
			for( np2=np+ss, ptr+=ss; ss; ss-- )
				*--np2 = *--ptr;		/* copy down */
	}
	return( np );
}

#ifdef DEBUG
	VOID
printstats()
{
	FB_HDR *fp;
	UWORD nblk=0, nbyt=0;
	double abyt;

	if( (fp=_aflistptr) != NULL ) {
		nblk++;
		nbyt += fp->size;
		for( fp=fp->ptr; fp != _aflistptr; fp = fp->ptr ){
			nblk++;
			nbyt += fp->size;
		}
	}
	nbyt *= sizeof(FB_HDR);
	abyt = 0;
	if( nblk>1 )
		abyt = nbyt / (nblk-1);		/* avg blk size */
	TRACE("\n\tmalloc free blocks=%ui bytes=%ui Avg size=%4.2f\n",
			nblk,nbyt,abyt);
}
#endif

