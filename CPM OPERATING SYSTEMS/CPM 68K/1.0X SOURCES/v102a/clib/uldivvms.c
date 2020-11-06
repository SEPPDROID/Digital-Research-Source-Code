unsigned long	uldivr=0;

unsigned long	uldiv(a,b)

register	unsigned long	a;
register	unsigned long	b;
{
	uldivr = a % b;
	return (a/b);
}
