#include	"cout.h"
#define BLEN 512
#include <klib.h>
struct iob {
	int fd;		/*file descriptor*/
	int cc;		/*char count*/
	char *cp;	/*ptr to next char*/
	char cbuf[BLEN];	/*char buffer*/
};

putchd(buf,header)
struct iobuf *buf;
struct hdr   *header;
{
	register short *p;
	register int	i;
	p = (short *)header;
	for(i=0; i<sizeof(*header);i += 2)
		putw(*p++,buf);
}

	                                                                                                                        }

	                                                                                                                        }

	                                                                                                                        