/* chown - change owner: like access() under CP/M */
#include <portab.h>
WORD chown(name,owner,group)
	BYTE *name;
	WORD owner;
	WORD group;
{
	return(access(name,0));
}
