/*
 *	Copies of header files used for initialization
 */

/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

	/*intermediate code operators*/
	/*0=>EOF, special operator*/
#define	EOF	0

	/*1-59=>operators that generate code (entries in code gen optab)*/
#define	ADD	1
#define	SUB	2
#define	MULT	3
#define	DIV	4
#define	MOD	5
#define	RSH	6
#define	LSH	7
#define	AND	8
#define	OR	9
#define	XOR	10
#define	NOT	11
#define	UMINUS	12
#define	COMPL	13
#define	PREDEC	14
#define	PREINC	15
#define	POSTDEC	16
#define	POSTINC	17
#define	ASSIGN	18
#define	EQADD	19
#define	EQSUB	20
#define	EQMULT	21
#define	EQDIV	22
#define	EQMOD	23
#define	EQRSH	24
#define	EQLSH	25
#define	EQAND	26
#define	EQOR	27
#define	EQXOR	28
#define	FJSR	29
#define	EQUALS	30
#define	NEQUALS	31
#define	GREAT	32
#define	GREATEQ	33
#define	LESS	34
#define	LESSEQ	35
#define	INT2L	36
#define	LONG2I	37

	/*machine dependent operators that generate code*/
#define	BTST	38
#define	LOAD	39
#define	LMULT	40
#define	LDIV	41
#define	LMOD	42
#define	LEQMULT	43
#define	LEQDIV	44
#define	LEQMOD	45
#define EQADDR	46
#define	LCGENOP	47	/*change if adding more operators...*/

	/*intermediate code operators that do not generate code*/
#define	ADDR	60
#define	INDR	61
#define	LAND	62
#define	LOR	63
#define	QMARK	64
#define	COLON	65
#define	COMMA	66
#define	CINT	67
#define CLONG	68
#define	SYMBOL	69
#define	AUTOINC	70
#define	AUTODEC	71
#define	CALL	72
#define	NACALL	73
#define BFIELD	74
#define IFGOTO	75
#define	INIT	76
#define	CFORREG	77
#define	DCLONG	78

	/*operators local to parser*/
#define CAST	80
#define	SEMI	81
#define	LCURBR	82
#define	RCURBR	83
#define	LBRACK	84
#define	RBRACK	85
#define	LPAREN	86
#define	RPAREN	87
#define	STRING	88
#define	RESWORD	89
#define	APTR	90
#define	PERIOD	91
#define SIZEOF	92
#define	MPARENS	93
#define	STACKEND	100

	/*data types*/
#define	TYPELESS	0
#define	CHAR	1
#define	SHORT	2
#define	INT	3
#define	LONG	4
#define	UCHAR	5
#define	USHORT	6
#define UNSIGNED 	7
#define	ULONG	8
#define	FLOAT	9
#define	DOUBLE	10

	/*data types local to parser*/
#define	STRUCT	11
#define	FRSTRUCT	12
#define	LLABEL	13

	/*type flags and definitions*/
#define	TYPE	017
#define	SUPTYP	060
#define	ALLTYPE	077
#define	POINTER	020
#define	FUNCTION	040
#define	ARRAY	060
#define	SUTYPLEN	2

/*	data registers*/
#define	DREG2	2
#define	DREG3	3
#define	DREG4	4
#define	DREG5	5
#define	DREG6	6
#define	DREG7	7
#define	AREG3	11
#define	AREG4	12
#define	AREG5	13

	/*storage classes*/
#define	AUTO	1
#define	REGISTER	2
#define	EXTERNAL	3
#define	STATIC	4
#define	REGOFF	5
#define	EXTOFF	6
#define	STATOFF	7
#define	INDEXED	8

	/*exclusively code generator storage classes*/
#define	CINDR	9
#define	CLINDR	10

	/*exclusively parser storage classes*/
#define	STRPROTO	9
#define PDECLIST	10
#define	PARMLIST	11
#define	BFIELDCL	12
#define UNELCL	13
#define	STELCL	14


	/*opinfo table bits*/
#define	OPPRI	077
#define	OPBIN	0100
#define	OPLVAL	0200
#define	OPREL	0400
#define	OPASSIGN	01000
#define	OPLWORD	02000
#define	OPRWORD	04000
#define	OPCOM	010000
#define	OPRAS	020000
#define	OPTERM	040000
#define	OPCONVS	0100000

	/*68000 definitions*/
#define	PTRSIZE	4
#define	INTSIZE	2
#define	LONGSIZE	4
#define	SSIZE	8
#define	TRUE	1
#define	FALSE	0
#define	BITSPBYTE	8

	/*operator class priorities*/
#define	TRMPRI	0	/*terminal nodes*/
#define	RPNPRI	1	/*) and ]*/
#define	CALPRI	2	/*in-stack call, ( or [*/
#define	COLPRI	3	/*init or case priority for : or ,*/
#define	STKPRI	4	/*priority of end of stack*/
#define	COMPRI	5	/*normal priority for ,*/
#define	ASGPRI	6	/*=, +=, -=, *=, /=, %=, ...*/
#define	QMKPRI	7	/*?:*/
#define	LORPRI	8	/*||*/
#define	LNDPRI	9	/*&&*/
#define	ORPRI	10	/*|, !*/
#define	ANDPRI	11	/*&*/
#define	EQLPRI	12	/*==, !=*/
#define	RELPRI	13	/*>, <, >=, <=*/
#define	SHFPRI	14	/*<<, >>*/
#define	ADDPRI	15	/*+, -*/
#define	MULPRI	16	/**, /, %*/
#define	UNOPRI	17	/*++, --, &, *, -, ~, sizeof*/
#define	LPNPRI	18	/*., ->, [, (, function call*/
#define	PSTPRI	19	/*in-stack post--, post++*/

struct io_buf {
	int io_fd;
	int io_nc;
	char *io_p;
	char io_b[512];
};
struct { int hiword; int loword; };
#define	EXPSIZE	1024
int exprarea[EXPSIZE]= {0}


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

	/*expression tree node for external symbol - need to keep name*/
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
} swtab[SWSIZE] = {0};


			/*operator and operand stack used by expr*/
struct ops {		/*operator stack*/
	int o_op;	/*operator*/
	int o_pri;	/*priority*/
} opstack[OPSSIZE] = {0}, 	
  *opp=0;

char *opdstack[OPDSIZE]={0};	/*operand stack*/
char **opdp=0;		/*operand stack pointer*/
char *opap=0;		/*pointer to next available loc in exprarea*/
char *exprp;		/*place to start building expression*/
int opinfo[];		/*operator info table*/
struct tnode *frp={0};	/*pointer to function return info node*/
int swp;		/*current entry in switch table*/
int cswp=0;		/*current low switch table index*/
int nextlabel;		/*generates unique label numbers*/
int clabel=0;		/*continue label*/
int blabel=0;		/*break label*/
int rlabel=0;		/*return label*/
int dlabel=0;		/*default label*/
int lineno=0;		/*current line number of input*/
int errcnt=0;		/*count of errors*/
int equalstop=0;	/*stop lex at '=', used for external init*/
int commastop=0;	/*stop parse at comma(used for const expr)*/
int colonstop=0;	/*stop parser at colon(used for case value)*/
int instruct=0;		/*set when in structure declaration*/
int infunc=0;		/*set when in function body*/
int smember=0;		/*set when seen . or ->*/
int tdflag=0;		/*declaration is a typedef proto*/
char *tdp=0;		/*points to typedef prototype*/
int localsize=0;	/*length of local variables*/
int naregs=0;		/*keeps track of pointer registers alloc'd*/
int ndregs=0;		/*keep track of data registers alloc'd*/
int loadreg=0;		/*need to load registers...*/
int boffset=0;		/*current bit offset in structure*/
int xflag=0;		/*translate int's to long's*/

	/*dimension table*/
int	dtab[DSIZE]={0};
int cdp=0;		/*next entry in dtab to alloc*/

	/*lexical analyzer values*/
int cvalue=0;		/*current token value if keyword or CINT*/
int cstrsize=0;		/*current string size*/
long clvalue=0;		/*current token value if long constant*/
struct symbol *csp=0;	/*current token symbol ptr if SYMBOL*/
char cstr[STRSIZE]={0};	/*current token value if CSTRING*/
struct symbol *dsp=0;	/*declarator symbol pointer*/

	/*function argument table, used to collect function parameters*/
struct farg {
	struct symbol *f_sp;
	int f_offset;
} fargtab[NFARGS]={0};

	/*forward referenced structure prototype names*/
struct symbol *frstab[NFRSTR]={0};
int frstp=0;

	/*output buffers for intermediate code and strings*/
struct io_buf obuf = {0}, 
	      sbuf = {0}, 	
	      ibuf = {0}, 
	      *obp =  0;

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
int  begseq=0;
int dflag=0;
int inclflag=0;
int inittype=0;
int onepass=0;
int opdontop=0;
int pbchar=0;
int peektok=0;
struct symbol *symbols=0;
struct symbol *symtab[HSIZE] = {0};
fflush(fp)
char	*fp;
{
	return(myfflush(fp));
}
