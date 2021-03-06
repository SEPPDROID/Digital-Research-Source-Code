/* prtftest.c performs tests on different formats except floating # formats.
	Everything works except f format for numbers whose absolute values
	are less than 0.05.

	program written by	Bill Fitler	
		modified by	Ken Chai	21-Dec-83

*/

#include "bugdefs.h"
#include <stdio.h>

BYTE *signon = {"testing printf\n"};

main()
{
	WORD w1,w2,w3;
	WORD j;
	BYTE *model, *bugnum="prtftest: \tdiverse tests of printf function\
\n\t\t\t" ;

	w1 = 100; w2 = -20; w3 = -3000;
	write(1,signon,strlen(signon));
	printf("test\t%%s: %s\n",signon);
	printf("test\t%%c: %c\n",'c');
	printf("test\t%%d: %d, %d, %d\n",w1,w2,w3);
	printf("test\t%%D: %D, %D, %D\n",(LONG)w1,(LONG)w2,(LONG)w3);
	printf("test\t%%u: %u, %u, %u\n",w1,w2,w3);
	printf("test\t%%U: %U, %U, %U\n",(LONG)w1,(LONG)w2,(LONG)w3);
	printf("test\t%%o: %o, %o, %o\n",w1,w2,w3);
	printf("test\t%%O: %O, %O, %O\n",(LONG)w1,(LONG)w2,(LONG)w3);
	printf("test\t%%x: %x, %x, %x\n",w1,w2,w3);
	printf("test\t%%X: %X, %X, %X\n",(LONG)w1,(LONG)w2,(LONG)w3);
	for( j=1; j<7; j++ ) {
		printf("test '*'\t%%%dd:\t%*d %*d %*d\n",j,j,w1,j,w2,j,w3);
		printf("\t\t%%-%d.%dd:\t%-*.*d %-*.*d %-*.*d\n",
				j,j,j,j,w1, j,j,w2, j,j,w3);
		printf("\t\t%%0%d.%dd:\t%0*.*d %0*.*d %0*.*d\n",
				j,j,j,j,w1, j,j,w2, j,j,w3);
		printf("\t\t%%-0%d.%dd: %-0*.*d %-0*.*d %-0*.*d\n",
				j,j,j,j,w1, j,j,w2, j,j,w3);
		printf("\n");
	} /* for( j=0; j<7; j++ ) */	

	ASSERT(TRUE) ;

} /* prtftest */
                                                                   =0; j<7; j++ ) */	

	ASSERT(TRUE) ;

} /* prtftest */
                                                                   =0; j<7; j++ ) */	

	ASSERT(TRUE) ;

} /* prtftest */
                                                                   