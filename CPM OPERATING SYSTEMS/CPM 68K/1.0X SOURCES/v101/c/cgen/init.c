#
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
int exprarea[EXPSIZE] = {0};

#define	NODEBUG	1



char brtab[][2];
int invrel[];
int swaprel[];
char *strtab[];
char null[]={0};

	/*operator tree node for unary and binary operators*/
struct tnode {
	int t_op;	/*operator*/
	int t_type;	/*data type of result*/
	int t_su;	/*Sethy-Ullman number*/
	int t_ssp;
	struct tnode *t_left;	/*left sub-tree*/
	struct tnode *t_right;	/*right sub-tree (undefined if unary)*/
};

	/*constant terminal node*/
struct conode {
	int t_op;	/*operator*/
	int t_type;	/*type*/
	int t_su;	/*Sethy-Ullman number*/
	int t_ssp;
	int t_value;	/*value or label number*/
};

struct lconode {
	int t_op;	/*operator*/
	int t_type;	/*type*/
	int t_su;	/*Sethy-Ullman number*/
	int t_ssp;
	long t_lvalue;	/*value or label number*/
};

	/*local symbol terminal node*/
struct symnode {
	int t_op;	/*operator*/
	int t_type;	/*symbol data type*/
	int t_su;	/*Sethy-Ullman number*/
	int t_ssp;
	int t_sc;	/*storage class*/
	int t_offset;	/*register offset*/
	int t_reg;	/*register number*/
	int t_label;	/*label number if static*/
};

	/*external symbol reference node*/
struct extnode {
	int t_op;	/*operator*/
	int t_type;	/*symbol data type*/
	int t_su;	/*Sethy-Ullman number*/
	int t_ssp;
	int t_sc;	/*storage class*/
	int t_offset;	/*register offset*/
	int t_reg;	/*register number*/
	char t_symbol[SSIZE];	/*symbol name*/
};

	/*68000 special - indexed symbol node*/
	/*this is used to generate a An(off,Xn.type) address*/
struct indexnode {
	int t_op;
	int t_type;
	int t_su;
	int t_ssp;
	int t_sc;
	int t_offset;
	int t_reg;
	int t_xreg;
	int t_xtype;
};

int lflag=0;
int dflag=0;
int mflag=0;
int cflag=0;
int eflag=0;
int fflag=0;
int oflag=0;
struct io_buf ibuf = {0}, 
	      obuf = {0};
int lineno=0;
int naregs=0;
int ndregs=0;
int errcnt=0;
int opinfo[];
int nextlabel;
char null[];
char optab[][6];
char *mnemonics[];
char *codeskels[];
int stacksize;

char *tnalloc();
char *snalloc();
char *cenalloc();
char *xnalloc();
char *talloc();
char *cnalloc();
char *lcnalloc();
char *canon();
char *commute();
char *constant();
char *match();
char *addptree();
char *fixbfield();
char *coffset();
char *tcopy();
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
#define	unaryop(op)	((opinfo[op]&(OPBIN|OPTERM))==0)
#define	leafop(op)	((opinfo[op]&OPTERM)!=0)
#define	notleafop(op)	((opinfo[op]&OPTERM)==0)
#define	lvalop(op)	((opinfo[op]&OPLVAL)!=0)
#define	oppriority(op)	(opinfo[op]&OPPRI)
#define	commop(op)	((opinfo[op]&OPCOM)!=0)
#define	convop(op)	((opinfo[op]&OPCONVS)!=0)
#define	notconvop(op)	((opinfo[op]&OPCONVS)==0)
#define	max(a,b)	(a>b?a:b)
#define min(a,b)	(a<b?a:b)

#define	QUICKVAL	8
#define	LEP	14
#define	FORCC	1
#define	FOREFF	2
#define	FORSTACK	3
#define	FORCREG	4
#define	FORSP	5
#define	FORREG	4
#define	HICREG	2
#define	NCREGS	3
#define	AREGLO	8
#define	IMMED	1
#define	NOTIMMED	0
#define	LOFFSET	2
#define	NOTLOFFSET	0

int	bol=0;
int	errflg=0;
int	onepass=0;
char	*opap=0;
int	stacksize=0;

fflush(fp)
char	*fp;
{
	return(myfflush(fp));
}
