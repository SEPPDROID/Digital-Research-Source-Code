/*********************************************************************
*    STRCMP  -  compares strings
*	
*	Special version which is case - insensitive.
*
*	WORD strcmp(s1,s2)
*	BYTE *s1, *s2;
*
*	'strcmp' compares null terminated strings s1 and s2.
*	Returns:
*		strcmp < 0  if  s1<s2
*		strcmp = 0  if  s1=s2
*		strcmp > 0  if  s1>s2
*********************************************************************/

#include <portab.h>

WORD 	_strcmp(s1,s2)				/* CLEAR FUNCTION ***********/
REG	BYTE *s1, *s2;
{
REG	BYTE	a,b;
	while (*s1 || *s2)
	{
	   a = _toupper(*s1++);
	   b = _toupper(*s2++);
	   if (a > b) return (1);
	   if (a < b) return (-1);

	}
	return(0);
}
MLOCAL	_toupper(c)
REG	WORD	c;
{
	if(c >= 'a' && c <= 'z')
		c += 'A' - 'a';
	return(c);
}
' - 'a';
	return(c);
}
' - 'a';
	return(c);
}
