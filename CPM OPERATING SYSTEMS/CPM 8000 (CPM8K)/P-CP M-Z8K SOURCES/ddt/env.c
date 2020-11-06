#include	"env.h"
#include	"u.h"

int	memode;
int	mfile;		/* memory file */
BYTE	memory[MEMSIZE];

struct	symbol	regtbl[]	={
	0,	0,	"?",
	RINT,	0x00,	"r0",
	RINT,	0x02,	"r1",
	RINT,	0x04,	"r2",
	RINT,	0x06,	"r3",
	RINT,	0x08,	"r4",
	RINT,	0x0A,	"r5",
	RINT,	0x0C,	"r6",
	RINT+RCR,	0x0E,	"r7",

	RINT,	0x10,	"r8",
	RINT,	0x12,	"r9",
	RINT,	0x14,	"ra",
	RINT,	0x16,	"rb",
	RINT,	0x18,	"rc",
	RINT,	0x1A,	"rd",
	RINT,	0x1C,	"re",
	RINT+RCR,	0x1E,	"rf",

	RCHR,	0x00,	"rh0",	RCHR,	0x01,	"rl0",
	RCHR,	0x02,	"rh1",	RCHR,	0x03,	"rl1",
	RCHR,	0x04,	"rh2",	RCHR,	0x05,	"rl2",
	RCHR,	0x06,	"rh3",	RCHR,	0x07,	"rl3",
	RCHR,	0x08,	"rh4",	RCHR,	0x09,	"rl4",
	RCHR,	0x0a,	"rh5",	RCHR,	0x0b,	"rl5",
	RCHR,	0x0c,	"rh6",	RCHR,	0x0d,	"rl6",
	RCHR,	0x0e,	"rh7",	RCHR,	0x0f,	"rl7",

	RCHR,	0x10,	"rh8",	RCHR,	0x11,	"rl8",
	RCHR,	0x12,	"rh9",	RCHR,	0x13,	"rl9",
	RCHR,	0x14,	"rha",	RCHR,	0x15,	"rla",
	RCHR,	0x16,	"rhb",	RCHR,	0x17,	"rlb",
	RCHR,	0x18,	"rhc",	RCHR,	0x19,	"rlc",
	RCHR,	0x1a,	"rhd",	RCHR,	0x1b,	"rld",
	RCHR,	0x1c,	"rhe",	RCHR,	0x1d,	"rle",
	RCHR,	0x1e,	"rhf",	RCHR,	0x1f,	"rlf",

	RINT,	0x20,	"id",
	RINT,	0x22,	"$$",
#ifdef	Z8001
	RINT,	0x24,	"pcs",
	RINT,	0x26,	"pc",
#else
	RINT,	0x24,	"pc",
#endif
	0,	0,	"",
};

/*
********	the FCW bits
*/

struct	bitable	pflags[]	={
	'f',	0x8000,
	'x',	0x4000,
	'e',	0x2000,
	'v',	0x1000,
	'n',	0x0800,
	'a',	0x0400,
	'9',	0x0200,
	'8',	0x0100,
	'c',	0x0080,
	'z',	0x0040,
	's',	0x0020,
	'p',	0x0010,
	'd',	0x0008,
	'h',	0x0004,
	'1',	0x0002,
	'0',	0x0001,
	'\0',	0x0000,
};

/*
**

**	*************************************************
**	*						*
**	*	S U B R O U T I N E S   F O R   Z 8 K	*
**	*						*
**	*************************************************
**
*/

/*
********	initializer
*/

dbginit (state)
union	pstate	*state;
{
	return;
}

/*
********	Single stepper
*/

sstep (state)
union	pstate	*state;
{
	int	diopn;
	diopn= deasm((POINTER)state->lregs.pc,oline);
	printf ("%s\n",oline);
	state->lregs.pc += getnext(diopn);
	return(diopn);
}

/*
********	breakpoint routines
*/

tstrap (addr)
POINTER	addr;
{
	if (ldint(addr) == BKPT)	return (1);
	return (0);
}

setrap (addr,bkpt)
POINTER	addr;
WORD	bkpt;
{ 
	WORD	oldop;
	oldop= ldint(addr);
	stint (addr,bkpt);
	return (oldop & 0xFFFF);
}

/*
********	Memory access routines
*/

WORD ldint (addr)
POINTER	addr;
{ 
	return (((ldchr(addr) << 8) & 0xFF00) | (ldchr(addr+1) & 0xFF));
}

stint (addr,data)
POINTER	addr;
WORD	data;
{ 
	stchr (addr,(data >> 8) & 0xFF);
	stchr (addr+1,data & 0xFF);
}

BYTE	ldstemp;	/* load & store temporary varible */

#ifdef	CPM

BYTE	ldchr (addr)
POINTER	addr;
{ 
	mem_cpy (addr,map_adr((LONG)&ldstemp & 0xFFFFL,0),1L);
	return (ldstemp & 0xFF); 
}

stchr (addr,data)
POINTER	addr;
BYTE	data;
{ 
	ldstemp= data;
	mem_cpy (map_adr((LONG)&ldstemp & 0xFFFFL,0),addr,1L);
}

#else

BYTE	ldchr (addr)
POINTER	addr;
{ 
	return (*(BYTE *)addr); 
}

stchr (addr,data)
POINTER	addr;
BYTE	data;
{ 
	*(BYTE *)addr= data; 
}

#endif

/*
********	IO access routines
*/

pichr (addr)
POINTER	addr;
{ 
	return (*(BYTE *)addr); 
}

pochr (addr,data)
POINTER	addr;
int	data;
{ 
	*(BYTE *)addr= data; 
}

