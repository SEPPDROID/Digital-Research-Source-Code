
/*----------------------------------------------------------------------*\
 |	NAME	:  tabinit						|
 |	CREATED	:  23-August-83		LAST MODIFIED:  19-September-83	|
 |	FUNCTION:  Tabinit initializes a tab position array used to	|
 |		   determine how many spaces to replace the tab with.	|
 |	INPUT	:  tabstop  -- ptr to tab stop array			|
 |		   max_len  -- length of tabstop array			|
 |	OUTPUT	:  Tabstop array initialized: 1 at tab stop colums and	|
 |		   zero elsewhere.					|
 |		   No return value.					|
\*----------------------------------------------------------------------*/

#include	<portab.h>

VOID	tabinit( tabstop,max_len,tabpos )
WORD	tabstop[];
WORD	max_len;
WORD	tabpos;
{
	WORD	col;

	tabstop[0] = 0;				/* using 1 index addr.	*/
	for( col=1; col < max_len; col++ )	/* tabstop[0] not used	*/
	   tabstop[col] = ( (col % tabpos) == 1 );
}
