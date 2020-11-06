/*****************************************************************************
*
*		     F P R I N T F   F U N C T I O N
*		     -------------------------------
*
*	The "fprintf" function is used to write data to a file using "putc".
*
*	calling sequence:
*
*		FILE *buff;
*		     .
*		     .
*		     .
*		fprintf(buff,format,arg1,arg2, ... argn);
*
*	Where:
*
*		buff	-> 	a UNIX structured buffer
*		format		is a text string as described in K & R 
*		Arg1-argn	are optional arguments to be converted and
*				placed in the output
*
*****************************************************************************/
fprintf		(fd,fmt,a1)			/* Declare args		    */
		int	fd;			/* POS channel number	    */
		char	*fmt;			/* -> Format string	    */
		int	a1;			/* Whatever the right size  */
{						/****************************/
	extern	char putc();			/* Putc routine from lib.   */
	_printf (fd,putc,fmt,&a1);		/* Invoke secret routine    */
}						/****************************/
