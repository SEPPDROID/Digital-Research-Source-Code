/* simulate sbrk system call until it works */
#define SBLEN 60000
long _sb_fmem[SBLEN]=0;		/*free mem pool*/
char *_sb_fpt= _sb_fmem;	/*free mem ptr */

char *sbrk(incr)
int incr;
{

	register char *p;

	p = _sb_fpt;	/*save old ptr*/
	if((_sb_fpt =+ incr) >= &_sb_fmem[SBLEN])
		return(-1);
	return(p);
}

int brk(addr)
	char *addr;
{
	if (addr <= &_sb_fmem[0] || addr > &_sb_fmem[SBLEN])
	{
		return(-1);
	}
	_sb_fpt = addr;
	return(0);
} 
