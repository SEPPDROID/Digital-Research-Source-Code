/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/* alignment, node type altering routines, dimension table allocating */
/* routine, and routines to determine elements actual size */

#include "parser.h"

#define DTSIZE	077					/*data size in bytes*/

char dinfo[];

/* dalloc - dimension table allocation*/
/*		Allocates an entry in the dimension table.*/
dalloc(dimsize)						/* returns ptr to dimension allocated*/
long dimsize;						/* dimension size [vlh] 3.4 i=>l*/
{
	register int i;

	if( (i=cdp++) >= DSIZE-1 )
		ferror("dimension table overflow");
	dtab[i] = dimsize;
	return(i);
}

/* addsp - add special type to special type info*/
/*		Takes given special type and adds it into the special type field.*/
addsp(type,nsptype)					/* returns resulting type*/
int type;							/* old type field*/
int nsptype;						/* special type to be added*/
{
	register int dtype;

	dtype = btype(type);
	type =& (~TYPE);
	return( (type<<SUTYPLEN) | suptype(nsptype) | dtype );
}

/* delsp - delete one special type info field from special type info*/
/*		Takes given special type field and deletes least sign.*/
delsp(type)							/* returns resulting type*/
int type;							/* old special type*/
{
	register int dtype;

	dtype = btype(type);
	type =& (~(ALLTYPE));
	return( (type>>SUTYPLEN) | dtype );
}

/*
 * revsp - reverse special type info
 *		This allows for the processing of the super-type info in
 *		the reverse order, which is necessary for initializations.
 */
revsp(type)							/* returns reversed type info*/
int type;							/* type to reverse*/
{
	register int t;

	for( t = btype(type); suptype(type) != 0; type = delsp(type) )
		t = addsp(t,type);
	return(t);
}

/* falign - handle bit field alignments*/
falign(type,flen)					/* returns number of bytes padded*/
int type;							/* data type*/
int flen;							/* field length*/
{
	register int off;

	off = 0;
	if( flen <= 0 ) {
		error("invalid field size");
		flen = 0;
	}
	switch( type ) {

	case INT:
	case UNSIGNED:
		if( flen > BITSPWORD )
			error("field overflows word");
		if( flen + boffset > BITSPWORD )
			off = CHRSPWORD;
		break;

	case CHAR:
		if( flen > BITSPCHAR )
			error("field overflows byte");
		if( flen + boffset > BITSPCHAR )
			off = 1;
		break;

	default:
		error("invalid field type description");
		return(0);

	}
	if( off )
		boffset = 0;
	boffset =+ flen;
	return(off);
}

/* salign - structure alignment*/
salign(type,offset)				/* returns bytes of padding*/
int type;							/* data type to align*/
int offset;							/* current structure offset*/
{
	register int off;

	off = offset;
	if( boffset ) {					/*remaining bit fields, flush 'em*/
		off =+ (boffset+(BITSPCHAR-1))/BITSPCHAR;
		boffset = 0;
	}
	while( array(type) )			/*get base type*/
		type = delsp(type);
	if( type != CHAR )				/*need word boundary*/
		off = walign(off);
	return( off - offset );
}

/* delspchk - delete one special reference and check if non-zero*/
delspchk(type)						/*returns new special type*/
int type;							/* type to modify*/
{
	if(!(suptype(type)))
		error("bad indirection");
	return( delsp(type) );
}

/* psize - return size of object ptd at by pointer*/
long								/* [vlh] 3.4 int => long */
psize(tp)							/* returns size of object in bytes*/
struct tnode *tp;					/* pointer to tree node*/
{
	if ( !(suptype(tp->t_type)) )	/* what case ??? */
		return(1);
	return( dsize(delsp(tp->t_type),tp->t_dp,tp->t_ssp) );
}

/* dsize - returns size of data object in bytes*/
long								/* [vlh] 3.4 */
dsize(type,dp,sp)					/* returns number of bytes*/
int type;							/* type of node*/
int dp;								/* dimension pointer*/
int sp;								/* size pointer if structure*/
{
	register long nel, size;

	nel = 1;
	for( ; array(type); type = delsp(type) )
		nel = dtab[dp];
	if( function(type) )
		return(0);
	size = (pointer(type)) ? PTRSIZE : (type == STRUCT) ? 
			dtab[sp] : dinfo[type]&DTSIZE;
	if(!size)
		error("invalid data type");
	return( size * nel );
}

