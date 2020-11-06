/*****************************************************************************
*
*		P R I N T F   I N T E R N A L   R O U T I N E
*		---------------------------------------------
*
*	Routine "_printf" is used to handle all "printf" functions, including
*	"sprintf", and "fprintf".
*
*	Calling Sequence:
*
*		_printf(fd,func,fmt,arg1);
*
*	Where:
*
*		fd		Is the file or string pointer.
*		func		Is the function to handle output.
*		fmt		Is the address of the format string.
*		arg1		Is the address of the first arg.
*
*	Bugs:
*
*	It is assumed that args are contiguous starting at "arg1", and that
*	all are the same size (int), except for floating point.
*
*	"putc" arguments are reversed from UNIX.
*
*****************************************************************************/

	char	*_ptrbf = 0;
	char	*_ptrst = 0;
	char	*__fmt  = 0;

_printf(fd,f,fmt,a1)				/****************************/
	int	fd;				/* Not really, but ...	    */
	char	(*f)();				/* Function pointer	    */
	char	*fmt;				/* -> Format string	    */
	int	*a1;				/* -> Arg list		    */
{						/****************************/
auto char  c, *s,  adj, *ptr,*p, buf[30];
auto int  *adx, x, n, m, width, prec,i, padchar;
double zz, *dblptr;
extern _putstr();
	_ptrbf = buf;
	adx = a1;
	 _ptrst = fd;
	__fmt = fmt;


while( c = *__fmt++ ){
   if(c != '%') (*f)(fd,c);
   else { x = *adx++;
      if( *__fmt == '-' ){ adj = 'l';  __fmt++; }
      else adj = 'r';
   padchar = (*__fmt=='0') ? '0' : ' ';
      width = __conv();
      if( *__fmt == '.'){++__fmt; prec = __conv();}
      else prec = 0;

   s = 0;
   if(*__fmt == 'l' || *__fmt == 'L') __fmt++;
   switch ( c = *__fmt++ ) {

     case 'D':
     case 'd':
	_prt1(x); break;
     case 'o':
     case 'O':
         _prnt8(x); break;
     case 'x':
     case 'X':
          _prntx(x); break;
      case 'S':
     case 's':    s=x;
        break;
     case 'C':
     case 'c':   *_ptrbf++ = x&0777;
         break;
     case 'E':
     case 'e':
     case 'F':
     case 'f':
      dblptr = adx-1;
      zz = *dblptr;
      adx =+ 1;
      ftoa (zz, buf, prec, c);
      prec = 0;
      s = buf;
     break;
     default:   (*f)(fd,c);
         adx--;
   }
   if (s == 0)
    {*_ptrbf = '\0'; s = buf;}
   n = aclenf (s);
   n = (prec<n && prec != 0) ? prec : n;
   m = width-n;
   if (adj == 'r') while (m-- > 0) (*f)(fd,padchar);
   while (n--) (*f)(fd,*s++);
   while (m-- > 0) (*f)(fd,padchar);
   _ptrbf = buf;
   }
}
if((*f) == _putstr) (*f)(fd,'\0');
}


_prnt8 (n)
{ /* print in octal */
	int p, k, sw;
	if (n==0) {*_ptrbf++ = '0'; return;}
	sw = 0;
	for (p=31; p >= 0; p =- 3)
  	if ((k = (n>>p)&07) || sw)
   	{
		if (p==31)
			k = k & 02;
    		*_ptrbf++ = '0' + k;
     		sw = 1;
   	}
}
_prntx (n)
{
	int d,a;
	if (a = n>>4)
		_prntx ( a & 0xfffffff);
	d = n&017;
	*_ptrbf++ =  d > 9 ? 'A'+d-10 : '0' + d;
}

__conv()
{
	auto c,n;
	n = 0;
	while( ((c = *__fmt++) >= '0') && (c<='9')) 
		n = n*10+c-'0';
	__fmt--;
	return(n);
}

_putstr(str,chr)
	char	chr;
	char	*str;
{
	*_ptrst++ = chr;
	return;
}
_prt1(n)
{
	int digs[15], *dpt;
	dpt = digs;
	if (n >= 0)
	   n = -n;
	else
	   *_ptrbf++ = '-';
	for (; n != 0; n = n/10)
	 *dpt++ = n%10;
	if (dpt == digs)
	   *dpt++ = 0;
	while (dpt != digs)
	{
	   --dpt;
	   *_ptrbf++ =  '0' - *dpt;
	}
}
