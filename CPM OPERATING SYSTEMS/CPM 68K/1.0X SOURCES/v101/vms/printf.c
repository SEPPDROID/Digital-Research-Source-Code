/*****************************************************************************
*
*			P R I N T F   F U N C T I O N
*			-------------------------------
*
*	The "printf" function is used to write data to the standard output.
*
*	calling sequence:
*
*		printf(format,arg1,arg2, ... argn);
*
*	Where:
*
*		format		is a text string as described in K & R 
*		Arg1-argn	are optional arguments to be converted and
*				placed in the output
*
*****************************************************************************/
__putc(fd,c)					/* WSL is backwards!!	    */
{						/****************************/
	putchar(c);				/* Output character	    */
}						/****************************/
printf		(fmt,a1)			/* Declare args		    */
		char	*fmt;			/* -> Format string	    */
		int	a1;			/* Whatever the right size  */
{						/****************************/
	_printf (0,__putc,fmt,&a1);		/* Invoke secret routine    */
}						/****************************/

