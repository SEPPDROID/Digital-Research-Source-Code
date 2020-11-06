/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

/*
	C68 Parser - include file
*/
#include "icode.h"



/*symbol attribute fields*/
#define	SRESWORD	001	/*is symbol a reserved word?*/
#define	SGLOBAL		002	/*is symbol global?*/
#define	STYPEDEF	004	/*typedef declaration?*/
#define	SDEFINED	010	/*symbol defined?*/


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


	/*mixed-mode conversions, entries in 2-d array indexed by:*/
	/*(int,unsn,long,doub,ptr)*/
#define	INT_UNSN	0	/*no conversion is generated*/
#define	INT_LONG	2
#define	INT_DOUB	3
#define	INT_PTR		4
#define	UNSN_INT	0	/*no conversion is generated*/
#define	UNSN_LONG	6
#define	UNSN_DOUB	7
#define	UNSN_PTR	8
#define	LONG_INT	9
#define	LONG_UNSN	10
#define	LONG_DOUB	11
#define	LONG_PTR	12
#define	DOUB_INT	13
#define	DOUB_UNSN	14
#define	DOUB_LONG	15
#define	PTR_INT		16
#define	PTR_UNSN	17
#define	PTR_LONG	18
#define	PTR_PTR		19
#define	BADCONV		20


#define	SSIZE	8	/*chars per symbol*/
#define	OPSSIZE	40	/*operator stack size*/
#define	OPDSIZE	80	/*operand stack size*/
#define	HSIZE	512	/*hash table size*/
#define	SYMSIZE	1024	/*size to alloc for symbol structures*/
#define	SWSIZE	256	/*max no. of cases in a switch*/
#define	DSIZE	1000	/*dimension table size*/
#define	BITSPWORD 16	/*bits per word*/
#define AREGLO	010	/*A reg flag*/
#define HICREG	2	/*highest reg # used for code generation*/
#define	BITSPCHAR 8	/*bits per char*/
#define	CHRSPWORD 2	/*chars per word*/
#define	STRSIZE	300	/*max string length*/
#define	NFARGS	40	/*max no. of args to function*/
#define	NFRSTR	20	/*max no. of forward ref struct proto*/


	/*symbol table node*/
struct symbol {
	char s_attrib;
	char s_sc;
	int s_type;
	int s_dp;
	int s_ssp;
	int s_offset;
	char s_symbol[SSIZE];
	struct symbol *s_next;
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
	int t_type;	/*data type of symbol*/
	int t_dp;	/*dimension pointer of symbol*/
	int t_ssp;	/*structure size index to dtab*/
	int t_sc;	/*storage class of symbol*/
	int t_offset;	/*offset of symbol*/
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
	int t_symbol[SSIZE];	/*symbol name*/
};

	/*expression tree node for integer constant*/
struct conode {
	int t_op;
	int t_type;
	int t_dp;
	int t_ssp;
	int t_value;	/*constant value*/
};

struct lconode {
	int t_op;
	int t_type;
	int t_dp;
	int t_ssp;
	long t_lvalue;	/*constant value*/
};

struct swtch {
	int sw_label;
	int sw_value;
} swtab[SWSIZE];


			/*operator and operand stack used by expr*/
struct ops {		/*operator stack*/
	int o_op;	/*operator*/
	int o_pri;	/*priority*/
} opstack[OPSSIZE], *opp;

char *opdstack[OPDSIZE];	/*operand stack*/
char **opdp;		/*operand stack pointer*/
char *opap;		/*pointer to next available loc in exprarea*/
char *exprp;		/*place to start building expression*/
int	opinfo[];	/*operator info table*/
struct tnode *frp;	/*pointer to function return info node*/
int swp;		/*current entry in switch table*/
int cswp;		/*current low switch table index*/
int	nextlabel;	/*generates unique label numbers*/
int clabel;		/*continue label*/
int blabel;		/*break label*/
int rlabel;		/*return label*/
int dlabel;		/*default label*/
int	lineno;		/*current line number of input*/
int	errcnt;		/*count of errors*/
int equalstop;		/*stop lex at '=', used for external init*/
int commastop;		/*stop parse at comma(used for const expr)*/
int colonstop;		/*stop parser at colon(used for case value)*/
int instruct;		/*set when in structure declaration*/
int infunc;		/*set when in function body*/
int smember;		/*set when seen . or ->*/
int tdflag;		/*declaration is a typedef proto*/
char *tdp;		/*points to typedef prototype*/
int localsize;		/*length of local variables*/
int naregs;		/*keeps track of pointer registers alloc'd*/
int	ndregs;		/*keep track of data registers alloc'd*/
int loadreg;		/*need to load registers...*/
int	boffset;	/*current bit offset in structure*/
int xflag;		/*translate int's to long's*/

	/*dimension table*/
int	dtab[DSIZE];
int cdp;		/*next entry in dtab to alloc*/

	/*lexical analyzer values*/
int cvalue;		/*current token value if keyword or CINT*/
int	cstrsize;	/*current string size*/
long clvalue;		/*current token value if long constant*/
struct symbol *csp;	/*current token symbol ptr if SYMBOL*/
char cstr[STRSIZE];	/*current token value if CSTRING*/
struct symbol *dsp;	/*declarator symbol pointer*/

	/*function argument table, used to collect function parameters*/
struct farg {
	struct symbol *f_sp;
	int f_offset;
} fargtab[NFARGS];

	/*forward referenced structure prototype names*/
struct symbol *frstab[NFRSTR];
int frstp;

	/*output buffers for intermediate code and strings*/
struct io_buf obuf, sbuf, ibuf, *obp;

#define	stypedef(sp)	(sp->s_attrib&STYPEDEF)
#define	wallign(add)	((add+1)&(~1))
#define	array(type)	((type&SUPTYP)==ARRAY)
#define	function(type)	((type&SUPTYP)==FUNCTION)
#define	pointer(type)	((type&SUPTYP)==POINTER)
#define	notarray(type)	((type&SUPTYP)!=ARRAY)
#define	notfunction(type)	((type&SUPTYP)!=FUNCTION)
#define	notpointer(type)	((type&SUPTYP)!=POINTER)
#define	btype(type)	(type&TYPE)
#define	suptype(type)	(type&SUPTYP)
#define	alltype(type)	(type&(SUPTYP|TYPE))
#define	asgop(op)	((opinfo[op]&OPASSIGN)!=0)
#define	relop(op)	((opinfo[op]&OPREL)!=0)
#define	lintegral(op)	((opinfo[op]&OPLWORD)!=0)
#define	rintegral(op)	((opinfo[op]&OPRWORD)!=0)
#define	rasop(op)	((opinfo[op]&OPRAS)!=0)
#define	binop(op)	((opinfo[op]&OPBIN)!=0)
#define	unaryop(op)	((opinfo[op]&OPBIN)==0)
#define	leaf(op)	((opinfo[op]&OPTERM)!=0)
#define	lvalop(op)	((opinfo[op]&OPLVAL)!=0)
#define	oppriority(op)	(opinfo[op]&OPPRI)
#define	makeiop(op)	(op|(0254<<8))

	/*functions returning pointers*/
char *expr();
char *talloc();
char *tnalloc();
char *enalloc();
char *snalloc();
char *cnalloc();
char *lcnalloc();
char *popopd();
char *cvopgen();
char *funcref();
char *arrayref();
char *install();
char *lookup();
char *sbrk();
char *balpar();
