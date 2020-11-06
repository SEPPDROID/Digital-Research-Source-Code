/********************************************************************
*	malloc, free - memory allocator mechanism
*
*	9/83: fixed free bug, added check word   whf
*	10/83: added debugging features: DEBUGMEM and MALLOCND hooks   whf
*
* BYTE *malloc(numbytes) 
*		WORD numbytes
*	Returns a pointer to an area of memory at least 'numbytes'
*	in length.  Returns NULLPTR if out of memory or corrupted freelist.
*	Warning: the size of the area is right below the region: 
*	do not overwrite!
*
*
* WORD free(mptr)
*		BYTE *mptr
*	Frees the region pointed to by 'mptr'.  Returns 0 (SUCCESS) if ok,
*	FAILURE (-1) if corrupted freelist (or if mptr was not previously
*	allocated by malloc).
*
*
* BYTE *realloc(mptr,numbytes)
*		BYTE *mptr
*		WORD numbytes
*	Returns a pointer to a (probably different) region of memory
*	(numbytes long) containing the same data as the old region 
*	pointed to by mptr. Returns NULLPTR if not enough memory, or
*	if freelist was corrupted.
*
************************************************************************/

#include "portab.h"

/************************* #include "malloc.h"  *****************************/
#define FB_HDR struct hdr			/* free block header type   */
#define NULLFBH ((FB_HDR *)0)			/* Null of above struct     */
						/*			    */
FB_HDR {					/* mem_block header	    */
	struct hdr *ptr;			/* ptr next blk (when freed)*/
	UWORD size;				/* block size (always)	    */
	UWORD chksize;				/* 1's complement of size   */
};						/****************************/
						/*			    */
FB_HDR _afreebase = {&_afreebase,0,~0 };	/* initial (empty) block    */
FB_HDR *_aflistptr = &_afreebase;		/* ptr into ring of freeblks*/
						/*			    */
#define AOFFS 1L                /* alignment offset: 0=byte, 1=word, 3=quad */
#define AMASK(c) ((char *)((long)(c) & ~AOFFS))	/* mask alignment bits	    */
#define AFUDGE 4	 		  /* leeway for passing block as is */
#define ACHUNKS 64 				/* chunks to alloc from O.S.*/
						/*			    */
#define ERR_FINDBLOCK 	1			/* error in 'findblock()'   */
#define ERR_GETMEM	2			/* error in 'getmemory()'   */
#define ERR_FREE	3			/* error in 'free()'	    */
						/*			    */
						/*** end of "malloc.h" ******/
/****************************************************************************/
/* malloc - general purpose memory allocator
* This function allocates the requested number of bytes (nbytes) and returns
*	a pointer to this space to the calling function.
*	The memory is requested from the O.S. in larger chunks, and
*	free space is managed as a ring of free blocks.  Each block
*	contains a pointer to the next block (s.ptr), a block size (s.size),
*	a check word (1's complement of size), and the space itself.
*	Alignment is handled by assuming that sizeof(FB_HDR) is an aligned
*	quantity, and allocating in multiples of that size.
**************************************************			    */
BYTE *	malloc(nbytes)				/* CLEAR FUNCTION ***********/
    UWORD nbytes;				/* number bytes requested   */
{						/****************************/
    REG	UWORD nmults;				/* multiples of FB_HDR size */
    REG	FB_HDR *pp;				/* temporary ptr	    */
	FB_HDR *findblock();			/* find free block	    */
	BYTE *cutup();				/* cut free block to fit    */
						/*			    */
	nmults = (nbytes+sizeof(FB_HDR)-1)/sizeof(FB_HDR)+1; /*		    */
	if( (pp=findblock(nmults)) == NULLFBH )	/* find ptr to ptr to block */
		return NULLPTR;			/*	no luck...	    */
	return cutup(pp,nmults);		/* cut block to fit & return*/
}						/****************************/

/****************************************************************************/
/* findblock - local subr to find a free block that's big enough.	    */
/*	It returns a pointer to the freeblock BEFORE the desired freeblock, */
/*	in order to fix the pointer of the this freeblock.		    */
/*************************************************			    */
    MLOCAL FB_HDR *				/*			    */
findblock(units)				/* find a free block	    */
    UWORD units;				/* at least this big	    */
{						/****************************/
    REG	FB_HDR	*cp;				/* ptr to current block	    */
    REG	FB_HDR	*pp;				/* ptr to previous block    */
	FB_HDR	*getmemory();			/* get from OS, add to list */
						/*			    */
	pp = _aflistptr;			/* start search here	    */
	cp = pp->ptr;				/* init current ptr	    */
	while(1)				/* do forever		    */
	{					/*			    */
	    if(~(cp->size^cp->chksize))		/* does cp pt to valid blk? */
	    {					/*    no...		    */
		_errmalloc(ERR_FINDBLOCK);	/* handle this error	    */
		return NULLFBH;			/*			    */
	    }					/*			    */
	    if( cp->size >= units )		/* is this block big enough?*/
		return pp;			/* yes! NOTE: return prevptr*/
	    if( cp == _aflistptr )		/* end of list?		    */
		if( (cp=getmemory(units)) == NULLFBH ) /* is there more?    */
		{				/*			    */
		    _errmalloc(ERR_GETMEM);	/*			    */
		    return NULLFBH;		/*	no more memory...   */
		}				/*			    */
	    pp = cp;				/*			    */
	    cp = cp->ptr;			/* move on down the list    */ 
	}					/*			    */
}						/****************************/

/****************************************************************************/
/* cutup - a local fn to cut up the free block (if its much bigger than the */
/*	number of units requested), and to convert blk ptr to byte ptr.     */
/*************************************************			    */
    MLOCAL BYTE *				/*			    */
cutup(pp,units)					/* cut the block to fit	    */
    FB_HDR *pp;					/* ptr to ptr to block	    */
    UWORD units;				/* num units to cut to	    */
{						/****************************/
    REG	FB_HDR *cp;				/* cur ptr		    */
    REG	FB_HDR *np;				/* new ptr (if needed)	    */
						/*			    */
	cp = pp->ptr;				/* get ptr to big block	    */
	if( cp->size <= units+AFUDGE )		/* is size close enough?    */
	    pp->ptr = cp->ptr;			/*   yes: cut cp from list  */
	else {					/* o.w. cut block up	    */
	    np = cp + units;			/* where it starts	    */
	    np->size = cp->size - units;	/* how big it is	    */
	    np->chksize = ~np->size;		/* it's a valid blk	    */
	    np->ptr = cp->ptr;			/* it's linked into freelist*/
	    pp->ptr = np;			/* from both sides	    */
	    cp->size = units;			/* new size for cur block   */
	    cp->chksize = ~cp->size;		/* and make valid	    */
	}					/****************************/
	_aflistptr = pp;			/* search from here next tim*/
	return cp+1;				/* point to after header    */
}						/****************************/


/****************************************************************************/
/* getmemory - gets memory from O.S. 					    */
/* This function requests memory from the O.S. in multiples (ACHUNKS)	    */
/*	of the requested number of units (numu), in order to minimize	    */
/*	the number of calls to the O.S.					    */
/*	Function cleans up pointer alignment and adds to free list.	    */
/*************************************************			    */
	MLOCAL FB_HDR *
getmemory(numu)
	UWORD numu;				          /* number of units */
{
	BYTE *sbrk();				  /* obtain memory from O.S. */
	REG BYTE *mmp;				      /* more memory pointer */
	REG FB_HDR *fbp;			       /* free block pointer */
	REG WORD utg;					     /* units to get */

	utg = ((numu+(ACHUNKS-1)) / ACHUNKS) * ACHUNKS;

	mmp = sbrk(utg * sizeof(FB_HDR));      /* sbrk wants number of bytes */

	if( mmp == (char *)-1 )       /* sbrk = -1 means no memory available */
		return( NULLFBH );
	mmp = AMASK(mmp + AOFFS);    /* alignment handling: nop if good sbrk */
	fbp = (FB_HDR *)mmp;
	fbp->size = utg;
	fbp->chksize = ~fbp->size;
	free( (BYTE *)(fbp+1) );                         /* add to free list */
	return(_aflistptr);				    /* set in 'free' */
}


/****************************************************************************/
/* free - adds memory back to free block list.
* This function assumes freed memory pointer (fmp) points to free space
*	preceeded by a FB_HDR structure.
*	Note that free block is inserted in memory address order,
*	to facilitate compaction.
*	Fails (-1 return) if you link in a random ptr.
*************************************************			    */
WORD	free(fmp)				/* CLEAR FUNCTION ***********/
	BYTE *fmp;				/* freed memory ptr	    */
{						/****************************/
	REG FB_HDR *cp,				/* current freeblk ptr	    */
		   *pp;				/* previous freeblk ptr	    */
						/****************************/
	cp = (FB_HDR *)fmp - 1;			/* ptr to (hdr of) freed blk*/
	if(~(cp->size^cp->chksize))		/* is blk valid?	    */
	{					/*			    */
	    _errmalloc(ERR_FREE);		/*			    */
	    return FAILURE;			/*	no: fail...	    */
	}					/****************************/
	for( pp=_aflistptr; ; pp=pp->ptr )	/* start through the list   */
	{					/*			    */
	    if( cp >= pp  &&  cp < pp->ptr )	/* are we at correct place? */
		break;				/*	yes...		    */
	    if( pp >= pp->ptr  &&  		/* at end of list? and	    */
		(cp <= pp->ptr  ||  cp >= pp) )	/*   blk is off either end? */
		break;		 		/*	yes...		    */
	}					/****************************/
	if( cp>=pp  &&  cp+cp->size <= pp+pp->size )/* block already freed? */
	{					/*			    */
		_aflistptr = pp;		/* search from here next    */
		return SUCCESS;			/* and do no more	    */
	}					/****************************/
/*************************************************			    */
/* Insert freed block back into list.  Try to coalesce (merge) adjacent	    */
/*	regions.							    */
/*************************************************			    */
	if( cp+cp->size == pp->ptr )		/* end freed blk==start nxt?*/
	{					/*	then merge with nxt */
		cp->size += pp->ptr->size;	/*			    */
		cp->chksize = ~cp->size;	/*			    */
		cp->ptr = pp->ptr->ptr;		/*			    */
	} else					/*****			    */
		cp->ptr = pp->ptr;		/* else just point to nxt   */
						/****************************/
	if( pp+pp->size == cp )			/* end prev plk==start cur? */
	{					/*	then merge with prev*/
		pp->size += cp->size;		/*			    */
		pp->chksize = ~pp->size;	/*			    */
		pp->ptr = cp->ptr;		/*			    */
	} else					/*****			    */
		pp->ptr = cp;			/* else point prev to cur   */
						/****************************/
	_aflistptr = pp;			/* search from here next time*/
	return SUCCESS;				/* say its ok		    */
}						/****************************/


/****************************************************************************/
/* realloc - free memory, allocate again (with changed size maybe),	    */
/*		preserve contents.					    */
/*************************************************			    */
BYTE *	realloc(ptr,siz)			/* CLEAR FUNCTION ***********/
REG	BYTE *ptr;				/* ptr to (prev malloc'd)mem*/
	UWORD siz;				/* size of mem		    */
{						/****************************/
REG	BYTE *np;				/* ptr to new allocation    */
REG	UWORD nmults;				/* multiples if FB_HDR size */
	FB_HDR *pp, *findblock();		/* find free block	    */
	BYTE *cutup();				/* cut free block to fit    */
						/*			    */
	free(ptr);	/* stuff back into free list: any coalesce will not */
			/* affect original data region 			    */
	nmults = (siz+sizeof(FB_HDR)-1)/sizeof(FB_HDR)+1; /*		    */
	if( (pp=findblock(nmults)) == NULLFBH )	/* find ptr to ptr to block */
		return NULLPTR;			/*	no luck...	    */
	np = (pp->ptr)+1;			/* convert to BYTE ptr	    */
	if( ptr != np ) {			/* if ptr changed	    */
		if( np < ptr )			/* if new ptr in lower mem  */
		    for( ; siz; siz-- )		/* copy up		    */
			*np++ = *ptr++;		/*			    */
		else				/* if new ptr in higher mem */
		    for( np+=siz, ptr+=siz; siz; siz-- ) /* copy down	    */
			*--np = *--ptr;		/*			    */
	}					/****************************/
	return cutup(pp,nmults);		/* cut block to fit & return*/
}						/****************************/
