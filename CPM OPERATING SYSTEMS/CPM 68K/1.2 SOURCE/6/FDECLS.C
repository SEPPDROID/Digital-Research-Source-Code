/****************************************************************************
*
*		D a t a   D e c l a r a t i o n s   M o d u l e
*		-----------------------------------------------
*	Copyright 1982 by Digital Research Inc.  All rights reserved.
*
*	Herein lie the data definitions for the stdio.h functions.
*	Note: function __fdecls() is here so _main can force this module to
*	be loaded.
*
*****************************************************************************/

#define FILE				/* avoid duplicate decls	    */
#include "stdio.h"

#if MAXFILES != 16
	compile snafu: fix following table
#endif

struct _iobuf _iob[16] ={
    { 0, _IOREAD, NULLPTR, NULLPTR, 0},
    { 1, _IOWRT+_IONBUF,  NULLPTR, NULLPTR, 0},
    { 2, _IOWRT+_IONBUF, NULLPTR, NULLPTR, 0},
    { 3, 0, NULLPTR, NULLPTR, 0 },
    { 4, 0, NULLPTR, NULLPTR, 0 },
    { 5, 0, NULLPTR, NULLPTR, 0 },
    { 6, 0, NULLPTR, NULLPTR, 0 },
    { 7, 0, NULLPTR, NULLPTR, 0 },
    { 8, 0, NULLPTR, NULLPTR, 0 },
    { 9, 0, NULLPTR, NULLPTR, 0 },
    { 10, 0, NULLPTR, NULLPTR, 0 },
    { 11, 0, NULLPTR, NULLPTR, 0 },
    { 12, 0, NULLPTR, NULLPTR, 0 },
    { 13, 0, NULLPTR, NULLPTR, 0 },
    { 14, 0, NULLPTR, NULLPTR, 0 },
    { 15, 0, NULLPTR, NULLPTR, 0 }
};

__fdecls()
{
}


}


}

