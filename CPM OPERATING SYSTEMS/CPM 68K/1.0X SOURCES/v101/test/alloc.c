/*
 * C library -- alloc/free
 */

#define	logical	char *

struct fb {
	logical	size;
	char	*next;
};

int	_freelist[] {
	0,
	-1,
};
logical	_slop	4;

char *
malloc(asize)
logical asize;
{
	register logical rsize;
	register logical np;
	register logical cp;

	if ((rsize = asize) == 0)
		return(-1);
	rsize += 3;
	rsize = _andptr(rsize,~01);
	for (;;) 
	{
		for (cp=_freelist; (np= cp->next) != -1; cp=np) 
		{
			if (np->size >= rsize) 
			{
				if (_addptr(rsize,_slop) >= np->size) 
				{
					cp->next = np->next;
					return(&np->next);
				}
				cp = cp->next = _addptr(np , rsize);
				cp->size = np->size - rsize;
				cp->next = np->next;
				np->size = rsize;
				return(&np->next);
			}
		}
		asize = (rsize < 1024) ? 1024: rsize;
		if ((cp = sbrk(asize)) == -1) 
		{
			return (-1);
		}
		cp->size = asize;
		free(&cp->next);
	}
}

free(aptr)
char *aptr;
{
	register logical ptr;
	register logical cp;
	register logical np;

	ptr = aptr-4;
	cp = _freelist;
	while ((np = cp->next) < ptr)
		cp = np;
	if (_addptr(ptr,ptr->size) == np) 
	{
		ptr->size = _addptr(ptr->size,np->size);
		ptr->next = np->next;
		np = ptr;
	} 
	else
		ptr->next = np;

	if (_addptr(cp,cp->size) == ptr) 
	{
		cp->size = _addptr(cp->size,ptr->size);
		cp->next = ptr->next;
	} 
	else
		cp->next = ptr;
}

/* Fudge routines */
_addptr (a,b)
{
	return(a+b);
}
_andptr (a,b)
{
	return(a&b);
}
