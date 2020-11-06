/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) sys/cout.h - Jul 26, 1983  REGULUS 4.1
*/


struct hdr {
#ifndef VAX11
	int  ch_magic;		/* c.out magic number 060016 = $600E */
#else
	unsigned short ch_magic;
#endif
	long ch_tsize;		/* text size */
	long ch_dsize;		/* data size */
	long ch_bsize;		/* bss size */
	long ch_ssize;		/* symbol table size */
	long ch_stksize;	/* stack size */
	long ch_entry;		/* location of entry point */
	short ch_rlbflg;	/* relocation bits present flag, must be signed */
};

struct hdr_cout {
#ifndef VAX11
	int  ftype;		/* c.out magic number 060016 = $600E */
#else
	unsigned short ftype;
#endif
	long ftext;		/* text size */
	long fdata;		/* data size */
	long fbss;		/* bss size */
	long fsym;		/* symbol table size */
	long fssize;	/* stack size */
	long fentry;	/* location of entry point */
	short fflag;	/* relocation bits present flag, must be signed */
};

struct hdr2 {
#ifndef VAX11
	int ch_magic;         /* c.out magic number = 601B hex */
#else
	unsigned short ch_magic;
#endif
	long ch_tsize;		/* # bytes in program text segment */
	long ch_dsize;		/* # bytes in program data segment */
	long ch_bsize;		/* # bytes in program bss  segment */
	long ch_ssize;		/* # bytes in symbol table */
	long ch_stksize;	/* initial stack size */
	long ch_entry;		/* entry point--address of text segment */
	short ch_rlbflg;	/* relocation bits suppressed flag, must be signed */
	long ch_dstart;		/* address of data segment */
	long ch_bstart;		/* address of bss segment */
 };

/* HDSIZE should be 28 bytes, HDSIZ2 should be 36 bytes */
#ifndef VAX11
#	define HDSIZE		(sizeof (struct hdr))
#	define HDSIZ2		(sizeof (struct hdr2))
#else
#	define HDSIZE		28
#	define HDSIZ2		36
#endif

#ifdef VAX11
#	define MAGIC		(unsigned short) 0x601a	/*  bra .+26 instruction */
#	define MAGIC1		(unsigned short) 0x601b	/* data & bss base defined */
#	define MAGICST2 	(unsigned short) 0x601c	/* shared text 2K boundary */
#	define MAGICID 		(unsigned short) 0x601d	/* I & D split */
#	define MAGICST		(unsigned short) 0x601e	/* shared test 4k boundary (-n default) */

#	define EX_MAGIC		(unsigned short) 0x601a
#	define EX_ABMAGIC	(unsigned short) 0x601b
#	define EX_2KSTXT	(unsigned short) 0x601c
#	define EX_IDMAGIC	(unsigned short) 0x601d
#	define EX_4KSTXT	(unsigned short) 0x601e
#else
#	define MAGIC			0x601a	/*  bra .+26 instruction */
#	define MAGIC1			0x601b	/* data & bss base defined */
#	define MAGICST2 		0x601c	/* shared text 2K boundary */
#	define MAGICID	 		0x601d	/* I & D split */
#	define MAGICST			0x601e	/* shared test 4k boundary (-n default) */

#	define EX_MAGIC			0x601a
#	define EX_ABMAGIC		0x601b
#	define EX_2KSTXT		0x601c
#	define EX_IDMAGIC		0x601d
#	define EX_4KSTXT		0x601e
#endif

#define SHT2KBOUND		2048
#define SHT2KFIX(x)		(((x+SHT2KBOUND-1)/SHT2KBOUND)*SHT2KBOUND)
#define SHT4KBOUND		4096
#define SHT4KFIX(x)		(((x+SHT4KBOUND-1)/SHT4KBOUND)*SHT4KBOUND)

