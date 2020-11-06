/************************************************************************
* _errmalloc / MALLOCDB.C : prints error messages for malloc()
*	To be included in OPTION?.lib
*
*	10/83   whf
************************************************************************/

#include <portab.h>

/************************* #include "malloc.h"  *****************************/
#define FB_HDR struct hdr			/* free block header type   */
						/*			    */
FB_HDR {					/* mem_block header	    */
	struct hdr *ptr;			/* ptr next blk (when freed)*/
	UWORD size;				/* block size (always)	    */
	UWORD chksize;				/* 1's complement of size   */
};						/****************************/
						/*			    */
EXTERN FB_HDR _afreebase;			/* initial (empty) block    */
EXTERN FB_HDR *_aflistptr;			/* ptr into ring of freeblks*/
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

_errmalloc(etype)
    int etype;					/* type of error	    */
{
	switch(etype)
	{
	  case ERR_FINDBLOCK:
		printf("malloc() error: corrupt arena\n");
		break;
	  case ERR_GETMEM:
		printf("malloc() error: out of memory\n");
		break;
	  case ERR_FREE:
		printf("free() error: pointer was not from malloc()\n");
		break;
	}
	exit(1);
}


/****************************************************************************/
/* malloc_debug(): checks the free list to see if everything is ok.	    */
/*************************************************			    */
WORD malloc_debug()				/*			    */
{						/****************************/
REG	FB_HDR	*cp;				/* ptr to current block	    */
						/*			    */
	cp = _aflistptr;			/* start search here	    */
	while(1)				/* do forever		    */
	{					/*			    */
	    if(~(cp->size^cp->chksize))		/* does cp pt to valid blk? */
	    {					/*    no...		    */
		_errmalloc(ERR_FINDBLOCK);	/* handle this error	    */
		return FAILURE;			/*			    */
	    }					/*			    */
	    cp = cp->ptr;			/* move on down the list    */ 
	    if( cp == _aflistptr )		/* end of list?		    */
		return SUCCESS;			/*			    */
	}					/*			    */
}						/****************************/
f list?		    */
		return SUCCESS;			/*			    */
	}					/*			    */
}						/****************************/
f list?		    */
		return SUCCESS;			/*			    */
	}					/*			    */
}						/****************************/
