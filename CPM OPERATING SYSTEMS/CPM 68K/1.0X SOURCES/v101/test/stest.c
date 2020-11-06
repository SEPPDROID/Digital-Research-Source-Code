#define	INCR	100
main()
{
	char	*blivot[100];
extern	char	*_break;
extern	char	*sbrk();
	int	i;

	printf("Original: %lx\n",_break);
	for(i=0; i<100; i++)
	{
		blivot[i] = sbrk(INCR);
		printf("i = %3d sbrk=%lx\n",i,blivot[i]);
	}
}
