/*
 *	Seek.c -- Whitesmith's equivalent of V6 seek
 */
long	lseek();

seek(fildes,offset,ptrname)
	int	fildes;			/* UNIX / WS file descriptor	*/
	int	offset;			/* File offset, bytes		*/
	int	ptrname;		/* Sense variable		*/
{
	offset &= 0xffff;		/* Clear sign extension problems*/
	if(ptrname > 2)			/* Not byte sense seek		*/
	{				/*				*/
		offset = offset << 9;	/* Multiply by 512		*/
		ptrname -= 3;		/* reset to range 0 .. 2	*/
	}				/********************************/
	return(lseek(fildes,(long)offset,ptrname));
}
