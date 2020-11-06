/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

/*
	C68 Parser - include file
*/
#include "icode.h"

/*symbol attribute fields*/
#define	SRESWORD	001			/*is symbol a reserved word?*/
#define	SGLOBAL		002			/*is symbol global?*/
#define	STYPEDEF	004			/*typedef declaration?*/
#define	SDEFINED	010			/*symbol defined?*/

	/*reserved words*/
#define	R_AUTO		1
#define	R_BREAK		2
#define	R_CASE		3
#define	R_CHAR		4
#define	R_CONTINUE	5
#define	R_DO		6
#define	R_DEFAULT	7
#define	R_DOUBLE	8
#define	R_GOTO		9
#define	R_ELSE		10
#define	R_EXTERNAL	11
#define	R_FLOAT		12
#define	R_FOR		13
#define	R_IF		14
#define	R_INT		15
#define	R_LONG		16
#define	R_REGISTER	17
#define	R_RETURN	18
#define	R_SHORT		19
#define	R_SIZEOF	20
#define	R_STATIC	21
#define	R_STRUCT	22
#define	R_SWITCH	23
#define	R_TYPEDEF	24
#define	R_UNION		25
#define	R_UNSIGNED	26
#define	R_WHILE		27

	/*
	 * mixed-mode conversions, entries in 2-d array indexed by:
	 * (int,unsn,long,doub,ptr)
	 */
#define	INT_CHAR		1
#define	UNSN_CHAR		1
#define	LONG_CHAR		1
#define DOUB_CHAR		1
#define PTR_CHAR		1
#define	INT_UNSN		0		/*no conversion is generated*/
#define	INT_LONG		2
#define	INT_DOUB		3
#define	INT_PTR			4
#define	UNSN_INT		0		/*no conversion is generated*/
#define	UNSN_LONG		6
#define	UNSN_DOUB		7
#define	UNSN_PTR		8
#define	LONG_INT		9
#define	LONG_UNSN		10
#define	LONG_DOUB		11
#define	LONG_PTR		12
#define	DOUB_INT		13
#define	DOUB_UNSN		14
#define	DOUB_LONG		15
#define	PTR_INT			16
#define	PTR_UNSN		17
#define	PTR_LONG		18
#define	PTR_PTR			19
#define	BADCONV			20

	/* miscellaneous constants */
#define	OPSSIZE		40			/*operator stack size*/
#define	OPDSIZE		80			/*operand stack size*/
#define	HSIZE		517			/*hash table size, 3.4 made prime */
#define	SYMSIZE		1024		/*size to alloc for symbol structures*/
#define	SWSIZE		256			/*max no. of cases in a switch*/
#define	DSIZE		1000		/*dimension table size*/
#define	BITSPWORD	16			/*bits per word*/
#define AREGLO		010			/*A reg flag*/
#define HICREG		2			/*highest reg # used for code generation*/
#define	BITSPCHAR	8			/*bits per char*/
#define	CHRSPWORD	2			/*chars per word*/
#define	STRSIZE		300			/*max string length*/
#define	NFARGS		40			/*max no. of args to function*/
#define	NFRSTR		20			/*max no. of forward ref struct proto*/

	/*symbol table node*/
struct symbol {
	char s_attrib;			/* defined, resword, global, typedef */
	char s_sc;				/* auto, static, external, register */
	int s_type;				/* 4bits specified, 2 bit fields for ptr... */
	int s_dp;				/* index into dimension table */
	int s_ssp;				/* dimension table/function arg table */
	int s_offset;			/* offset inside of structure */
	char s_symbol[SSIZE];	/* symbol identifier, to SSIZE chars */
	struct symbol *s_struc;	/* if struct, ptr to parent (sys III) */
	struct symbol *s_next;	/* next symbol table entry */
};

	/*expression tree operator node*/
struct tnode {
	int t_op;
	int t_type;
	int t_dp;
	int t_ssp;
	struct tnode *t_left;
	struct tnode *t_right;
};

	/*expression tree node for symbol - only keeps location*/
struct symnode {
	int t_op;
	int t_type;					/*data type of symbol*/
	int t_dp;					/*dimension pointer of symbol*/
	int t_ssp;					/*structure size index to dtab*/
	int t_sc;					/*storage class of symbol*/
	int t_offset;				/*offset of symbol*/
	int t_label;
};

	/*expressioon tree node for external symbol - need to keep name*/
struct extnode {
	int t_op;
	int t_type;
	int t_dp;
	int t_ssp;
	int t_sc;
	int t_offset;
	int t_reg;
	int t_symbol[SSIZE];		/*symbol name*/
};

	/*expression tree node for integer constant*/
struct conode {
	int t_op;
	int t_type;
	int t_dp;
	int t_ssp;
	int t_value;				/*constant value*/
};

struct lconode {
	int t_op;
	int t_type;
	int t_dp;
	int t_ssp;
	long t_lvalue;				/*constant value*/
};

struct swtch {
	int sw_label;
	int sw_value;
} swtab[SWSIZE]=0;

	/*operator and operand stack used by expr*/
struct ops {					/*operator stack*/
	int o_op;					/*operator*/
	int o_pri;					/*priority*/
} opstack[OPSSIZE]=0, *opp=0;

char *opdstack[OPDSIZE]=0;		/*operand stack*/
char **opdp=0;					/*operand stack pointer*/
char *opap=0;						/*ptr to next available loc in exprarea*/
struct tnode *frp=0;				/*pointer to function return info node*/
int cswp=0;						/*current low switch table index*/
int clabel=0;						/*continue label*/
int blabel=0;						/*break label*/
int rlabel=0;						/*return label*/
int dlabel=0;						/*default label*/
int	lineno=0;					/*current line number of input*/
int	errcnt=0;						/*count of errors*/
int inclflag=0;					/*in include file, don't incr line #'s*/
int inclline=0;					/*[vlh]line# in incl file for err rpting*/
char inclfile[13]=0;				/*[vlh]include filename for err rpting*/
int equalstop=0;				/*stop lex at '=', used for external init*/
int commastop=0;				/*stop parse @ comma(used for const expr)*/
int colonstop=0;				/*stop parse @ colon(used for case value)*/
int instruct=0;					/*set when in structure declaration*/
int smember=0;					/*set when seen . or ->*/
int infunc=0;						/*set when in function body*/
int tdflag=0;						/*declaration is a typedef proto*/
char *tdp=0;						/*points to typedef prototype*/
int localsize=0;					/*length of local variables*/
int naregs=0;						/*keeps track of ptr registers alloc'd*/
int	ndregs=0;						/*keep track of data registers alloc'd*/
int loadreg=0;					/*need to load registers...*/
int	boffset=0;					/*current bit offset in structure*/
int eflag=0;						/*[vlh] 3.4 IEEE floats */
int fflag=0;						/*[vlh] 3.4 FFP floats */
int xflag=0;						/*translate int's to long's*/
int wflag=0;						/*[vlh] don't generate warning mesgs*/
int reducep=0;					/*[vlh] if(procid); reduction*/
int peektok=0;					/*peeked at token*/

	/*dimension table*/
long dtab[DSIZE]=0;				/* [vlh] 3.4 int => long */
int cdp=0;						/*next entry in dtab to alloc*/

	/*lexical analyzer values*/
int cvalue=0;						/*current token value if keyword or CINT*/
int	cstrsize=0;					/*current string size*/
long clvalue=0;					/*current token value if long constant*/
struct symbol *csp=0;				/*current token symbol ptr if SYMBOL*/
char cstr[STRSIZE]=0;				/*current token value if CSTRING*/
struct symbol *dsp=0;				/*declarator symbol pointer*/

	/* -1 -> not instruct, 0 -> unnamed struct */
struct symbol *strucptr[10]=0;	/*[vlh] ptrs to struc symbols*/

	/*function argument table, used to collect function parameters*/
struct farg {
	struct symbol *f_sp;
	int f_offset;
} fargtab[NFARGS]=0;

	/*forward referenced structure prototype names*/
struct symbol *frstab[NFRSTR]=0;
int frstp=0;

	/*output buffers for intermediate code and strings*/
struct io_buf obuf=0, sbuf=0, ibuf=0, *obp=0;

#define	stypedef(sp)	(sp->s_attrib&STYPEDEF)
#define	walign(add)	((add+1)&(~1))
#define	array(type)		((type&SUPTYP)==ARRAY)
#define	function(type)	((type&SUPTYP)==FUNCTION)
#define	pointer(type)	((type&SUPTYP)==POINTER)
#define	notarray(type)		((type&SUPTYP)!=ARRAY)
#define	notfunction(type)	((type&SUPTYP)!=FUNCTION)
#define	notpointer(type)	((type&SUPTYP)!=POINTER)
#define	btype(type)		(type&TYPE)
#define	suptype(type)	(type&SUPTYP)
#define	alltype(type)	(type&(SUPTYP|TYPE))
#define	asgop(op)		((opinfo[op]&OPASSIGN)!=0)
#define	relop(op)		((opinfo[op]&OPREL)!=0)
#define	lintegral(op)	((opinfo[op]&OPLWORD)!=0)
#define	rintegral(op)	((opinfo[op]&OPRWORD)!=0)
#define	rasop(op)		((opinfo[op]&OPRAS)!=0)
#define	binop(op)		((opinfo[op]&OPBIN)!=0)
#define	unaryop(op)		((opinfo[op]&OPBIN)==0)
#define	leaf(op)		((opinfo[op]&OPTERM)!=0)
#define	lvalop(op)		((opinfo[op]&OPLVAL)!=0)
#define	oppriority(op)	(opinfo[op]&OPPRI)
#define	makeiop(op)		(op|(0254<<8))
	/* checks for symbol with structure element storage class */
#define isstel(tp)	(tp->t_op==SYMBOL && (sesc(tp)))
#define sesc(t)		(t->t_sc==STELCL||t->t_sc==UNELCL||t->t_sc==BFIELDCL)
	/* peek at next token, if not read token put back, else delete */
	/* 1 if matched, 0 otherwise */
#define peek(tok)	( (peektok=gettok()) == tok )

#define outcommon(sym,size)	printf("\t.comm _%.8s,%ld\n",sym,size)
#define outgoto(lab)		if( lab > 0 ) printf("\tbra L%d\n",lab)
	/* change to text segment */
#define outtext()		printf("\t.text\n")
	/* change segment to bss */
#define outbss()		printf("\t.bss\n")
	/* output global symbol references */
#define outextdef(sym)	printf("\t.globl _%.8s\n",sym)
	/* outputs reserved memory [vlh] 3.4 %d => %ld */
#define outresmem(size)	printf("\t.ds.b %ld\n",size)
	/* output padding for word alignments */
#define outpad()		printf("\t.even\n")
	/* output long constant to assembler */
#define outlcon(val)			printf("\t.dc.l %d\n",val)
	/* output label constant */
#define outclab(lab)			printf("\t.dc.l L%d\n",lab)
	/* output a label */
#define outlab(lab)				printf("\tL%d:",lab)
	/* output function label */
#define outflab(sym)			printf("\t_%.8s:\n\t~~%.8s:\n",sym,sym)
	/* output data label */
#define outdlab(sym)			printf("\t_%.8s:\n",sym)

	/*functions returning pointers*/
char *expr();
char *talloc();
char *tnalloc();
char *enalloc();
char *snalloc();
char *cnalloc();
char *lcnalloc();
char *fpcnalloc();
char *popopd();
char *cvopgen();
char *arrayref();
char *funcref();
char *install();
char *lookup();
char *balpar();
char *sbrk();

long initlist();
long dsize();
long psize();
long dodecl();
long dlist();
long getdec();
long gethex();
long getoct();
long getfp();
long toieee();
long toffp();

int inittype=0;
int strassign=0;
int begseq=0;

#define	EXPSIZE		1024
int exprarea[EXPSIZE]=0;

int opdontop=0;
int pbchar=0;

struct symbol *symbols=0;
struct symbol *symtab[HSIZE]=0;
