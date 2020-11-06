
/* rand.c - kluges a random number generator with addition & overflow */

#include "portab.h"


#define NSEEDS 7

MLOCAL WORD _seeds[NSEEDS] = 
	{ 0, 24213, 12345, 4622, 2143, 32010, 7942 }; 

MLOCAL WORD _seedptr=0;


WORD	rand()					/* CLEAR FUNCTION ***********/
{
REG	UWORD tot, ii;

	for( tot=0, ii=0; ii<NSEEDS; ii++)
		tot += _seeds[ii];	/* ignore overflow */
	if( ++_seedptr >= NSEEDS )
		_seedptr = 0;
	_seeds[_seedptr] = tot;
	return( tot >> 1 );		/* ignore lo bit because of addition */
}


WORD	srand(seed1)				/* CLEAR FUNCTION ***********/
	int seed1;
{
REG	WORD ncs;
	_seeds[0] = seed1;
	for( ncs = seed1&077; ncs; ncs-- )
		rand();
	return(rand());
}

