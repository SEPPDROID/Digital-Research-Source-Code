
/*=======================================================================*/
/*+---------------------------------------------------------------------+*/
/*|									|*/
/*|     P-CP/M(tm)  Block Move  for the KONTRON BOARD (Z8002)		|*/
/*|									|*/
/*|     Copyright 1982, Zilog Incorporated.				|*/
/*|									|*/
/*+---------------------------------------------------------------------+*/
/*=======================================================================*/

char copyright[] = "Copyright 1982, Zilog Incorporated";

/* HISTORY
**
**	830111	F. Zlotnick (Zilog)
**	830121	D. Heintz (Me) Fixed bug in s/d window computation
*/
 
mem_bcp( sseg, source, dseg, dest, length)/* sseg and dseg are pseudo-segment */
unsigned short sseg, source, dseg, dest;/*  numbers from mem_copy.	    */
unsigned short length;			/*  source and dest are offsets in  */
					/*  the pseudo-segments.  length is */
					/*  the number of bytes to be copied*/
{
	register unsigned length1, length2;
	register unsigned middle;
	short swindow, dwindow;
	unsigned short blength;
	static char locbuf[256];	/* intermediate buffer */

/* deal recursively with the possibility of crossing the half-segment line */

	middle = 0x8000;

	if(source < middle && source + length > middle) {
		length1 = middle - source;	/* first half */
		length2 = length - length1;	/* second half */
		mem_bcp(sseg,source,dseg,dest,length1);	/* recurse */
		mem_bcp(sseg,middle,dseg,dest+length1,length2);/* recurse */
		return;
	}

	if(dest < middle && dest + length > middle) {
		length1 = middle - dest;	/* first half */
		length2 = length - length1;	/* second half */
		mem_bcp(sseg,source,dseg,dest,length1);	/* recurse */
		mem_bcp(sseg,source+length1,dseg,middle,length2);/* recurse */
		return;
	}

/*
 * This part is non-recursive.  If you get this far, you're guaranteed
 * not to cross a mid-segment boundary on either the source or dest
 * blocks.
 */

	swindow = ((sseg >> 8) << 1) | ( (source >= middle) ? 1 : 0);
	dwindow = ((dseg >> 8) << 1) | ( (dest >= middle) ? 1 : 0);
	if ( swindow == dwindow ) {	/* Both in same window */
		map_wdw(swindow);
		if(swindow != 0) {		/* If really mapped, then */
			source |= 0x8000;	/*  must be in upper half */
			dest |= 0x8000;		/*  of system address space */
		}
		blkmov(source,dest,length);
	}
	else if(swindow != 0 && dwindow != 0) {
		source |=  0x8000;
		dest |=  0x8000;
		while(length > 0) {
			blength = (length > 256)? 256: length ;
			map_wdw(swindow);
			blkmov(source,locbuf,blength);	/* Assumes pointers*/
			map_wdw(dwindow);		/*  are short!     */
			blkmov(locbuf,dest,blength);	/* Ditto	   */
			source += blength;
			dest += blength;
			length -= blength;
		}
	}
	else {
		if ( swindow == 0 ) {	/* System lower half: no window */
			map_wdw(dwindow);	/* Put dest block in upper */
			dest |= 0x8000;		/*  half of system space */
		}
		else if ( dwindow == 0 ) {
			map_wdw(swindow);	/* Put source block in upper */
			source |= 0x8000;	/*  half of system space */
		}
		while ( length > 0 ) {
			blength = (length > 256)? 256: length ;
			blkmov(source,dest,blength);
			source += blength;
			dest += blength;
			length -= blength;
		}
	}
}
