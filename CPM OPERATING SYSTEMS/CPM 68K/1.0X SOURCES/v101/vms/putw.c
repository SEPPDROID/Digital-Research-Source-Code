/* 
 * Putw.c -- Kludge version!
 */
putw(w,iobuf)
	int	w;		/* 32-bits of which 16 are important */
	int	*iobuf;		/* Not really, but ...		     */
{
	short	junk;		/* Take care of hi word / low word etc */
	junk = w & 0xffff;	/* Convert to 16 bits		       */
	write((*iobuf),&junk,2); /* Write out 2 bytes (Ugh!!)	       */
	return(w);
}

