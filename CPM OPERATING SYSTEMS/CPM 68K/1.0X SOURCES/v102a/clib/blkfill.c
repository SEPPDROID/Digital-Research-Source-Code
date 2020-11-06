/***************************************************************************
*									    
*			b l k f i l l   F u n c t i o n			    
*			-------------------------------			    
*	Copyright 1983 by Digital Research Inc.  All rights reserved.	    
*									    
*	The blkfill function sets a region of memory to a given value.	    
*									    
*	Calling Sequence:						    
*									    
*		blkfill(addr,fc,num);					    
*									    
*	Where:								    
*		addr	Is a pointer to region of memory to blkfill	    
*		fc	Is a character to blkfill with			    
*		num	Is the number of chars to blkfill with		    
*									    
****************************************************************************/
#include <portab.h>

VOID	blkfill(addr,fc,num)			/* CLEAR FUNCTION ***********/
    BYTE *addr;
    BYTE fc;
    WORD num;
{
	while( num-- > 0 )
		*addr++ = fc;
}
