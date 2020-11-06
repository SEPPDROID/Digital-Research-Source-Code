#include	<stdio.h>	
#include	<setjmp.h>
jmp_buf		env;
main()
{
	WORD	i;			/* Temp				*/
	if((i=setjmp(env)))		/* only for the longjump return	*/
	{
		printf("Longjmp returns %d\n",i);
		exit();
	}
	b();
}
b()
{
	longjmp(env,12);
}
