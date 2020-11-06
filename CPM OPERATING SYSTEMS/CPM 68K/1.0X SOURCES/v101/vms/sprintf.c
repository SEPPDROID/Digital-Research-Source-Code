/*****************************************************************************
*
*		     S P R I N T F   F U N C T I O N
*		     -------------------------------
*
*	The "sprintf" function is used to format data into a string:
*
*	calling sequence:
*
*		char *buff;
*		     .
*		     .
*		     .
*		sprintf(buff,format,arg1,arg2, ... argn);
*
*	Where:
*
*		buff	-> 	a character string
*		format		is a text string as described in K & R 
*		Arg1-argn	are optional arguments to be converted and
*				placed in the output
*
*****************************************************************************/
sprintf		(buff,fmt,a1)			/* Declare args		    */
		char	*buff;			/* -> Output string	    */
		char	*fmt;			/* -> Format string	    */
		int	a1;			/* Whatever the right size  */
{						/****************************/
	extern	char	_putstr();		/* Use this function	    */
	_printf (buff,_putstr,fmt,&a1);		/* Invoke secret routine    */
}						/****************************/
