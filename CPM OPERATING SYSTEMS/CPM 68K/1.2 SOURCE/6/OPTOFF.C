/* OPTOFF.C: prints a message in case someone tries to use a part of the    */
/*	RTL which was OPTIONally linked out.				    */

#include "portab.h"
#include "osif.h"

_optoff(msg)
	BYTE 	*msg;
{
	BYTE	buf[200];			/* ought to be big enough   */

	strcpy(buf,"C RTL - program not linked for ");
	strcat(buf,msg);
	strcat(buf,"\r\nProgram terminating\r\n$");
	__OSIF(C_WRITESTR,buf);
	_exit(FAILURE);
}
TESTR,buf);
	_exit(FAILURE);
}
TESTR,buf);
	_exit(FAILURE);
}
