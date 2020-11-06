#define HDSIZE	(sizeof couthd)	/**.o file header size*/
struct hdr {
	int  ch_magic;		/*c.out magic number 060016 = $600E*/
	long ch_tsize;		/*text size*/
	long ch_dsize;		/*data size*/
	long ch_bsize;		/*bss size*/
	long ch_ssize;		/*symbol table size*/
	long ch_stksize;	/*stack size*/
	long ch_entry;		/*entry point*/
	int  ch_rlbflg;		/*relocation bits suppressed flag*/
} couthd;

#define MAGIC	0x601a	/* bra .+26 instruction*/
