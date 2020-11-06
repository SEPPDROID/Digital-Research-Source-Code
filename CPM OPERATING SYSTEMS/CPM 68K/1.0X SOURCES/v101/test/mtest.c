#include <stdio.h>
#define MIN	1
#define	MAX	50
#define	MAXPART	175
main()
{
	int	*p;
	int	i;
	int	j;
	int	k;
	int	*q;
	int	*a[MAXPART];
	int	pass;
	pass = 0;
	k = 0;
while(1)
{
	for(i = MIN; i < MAX; i++)
	{
		p = malloc(i*sizeof(int));	/* Gettum buffer	*/
		if(p == NULL)
		{
		  printf("Allocation fails %d blocks\n",k);
		  exit();
		}
		for (j = 0; j < i; j++)		/* Fill buffer		*/
		{
		   p[j] = j;			/* Set up pattern	*/
		}

		q = realloc(p,i*sizeof(int) + 200);
		if(q==NULL)
		{
			printf("Realloc out of room size = %d k = %lx\n",i,k);
			exit();
		}

		for (j = 0; j < i; j++)
		{
		   if(q[j] != j)		/* OOPS!!		*/
		   {
			printf("Realloc fails addr=%lx size = %d\n",q,i);
			exit();
		   }
		}
		
		if(k >= MAXPART)
		{
			for(k=0; k < MAXPART; k++)
				free(a[k]);
			k = 0;
		}
		a[k++] = q;	
	}
	printf("Pass %d\n",++pass);
}
}
