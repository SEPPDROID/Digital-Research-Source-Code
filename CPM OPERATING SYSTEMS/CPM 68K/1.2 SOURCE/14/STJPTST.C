/*
	This program tests DRC/rtl setjmp & longjmp functions.
	program written by	Bill Fitler
	modified by		Ken Chai	20-Dec-83
*/

#include	<stdio.h>
#include	<setjmp.h>
#include	"bugdefs.h"

	jmp_buf	env;
	GLOBAL	WORD	i;
	GLOBAL	BYTE 	*model, *bugnum="stjptst: \ttest of setjmp & longjmp\
\n\t\t\t" ;

	main()
	{
	EXTERN	WORD	i;
	WORD	xret,ret;

		printf("\n\tThis routine tests SETJMP & LONGJMP\n");
		i = 0;
		xret= setjmp(env);
		printf("\n\tXRET= %d",xret);
		if (xret != 0) 
		{
			ASSERT(TRUE) ;
			printf("\n\tExiting test routine");
			exit();
		} /* if (xret != 0) */

		recur();

	} /* stjptst main function */
	
	recur()
	{
	EXTERN	WORD	i;
	WORD	ret;
	
		while(i <= 100)
		{
			i +=1;
			recur();
		} /* while(i <= 100) */

		longjmp(env,ret);

	} /* recur */
                                                                             00) */

		longjmp(env,ret);

	} /* recur */
                                                                             00) */

		longjmp(env,ret);

	} /* recur */
                                                                             