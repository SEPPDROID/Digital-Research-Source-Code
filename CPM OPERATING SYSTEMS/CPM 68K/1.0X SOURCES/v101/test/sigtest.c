#include <stdio.h>
#include <signal.h>

main()
{
	int	illegal();
	int	buserr();

	signal(SIGIOT,&illegal);
	signal(SIGBUS,&buserr);
	xbarf();
	printf("Return OK\n");
}
illegal()
{
	printf("Trap instruction\n");
}
buserr()
{
	printf("Bus error exception\n");
}
