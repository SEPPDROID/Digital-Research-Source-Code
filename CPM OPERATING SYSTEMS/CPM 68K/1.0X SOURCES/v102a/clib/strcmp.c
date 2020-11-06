/*********************************************************************
*    STRCMP  -  compares strings
*	
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


WORD	strcmp(s1,s2)				/* CLEAR FUNCTION ***********/
REG	BYTE *s1, *s2;
{
REG	BYTE	a,b;
	while (*s1 && *s2)
	{
	   a = (*s1++);
	   b = (*s2++);
	   if (a > b) return (1);
	   if (a < b) return (-1);
	}
	return(*s1 - *s2);
}
