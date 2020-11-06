/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "../icode.h"
//#define	NODEBUG	0
//#define VERSADOS

char brtab[][2];
int invrel[];
int swaprel[];
char *strtab[];
char null[];

	/*operator tree node for unary and binary operators*/
struct tnode {
	int t_op;						/*operator*/
	int t_type;						/*data type of result*/
	int t_su;						/*Sethy-Ullman number*/
	int t_ssp;
	struct tnode *t_left;			/*left sub-tree*/
	struct tnode *t_right;			/*right sub-tree (undefined if unary)*/
};

	/*constant terminal node*/
struct conode {
	int t_op;						/*operator*/
	int t_type;						/*type*/
	int t_su;						/*Sethy-Ullman number*/
	int t_ssp;
	int t_value;					/*value or label number*/
};

struct lconode {
	int t_op;						/*operator*/
	int t_type;						/*type*/
	int t_su;						/*Sethy-Ullman number*/
	int t_ssp;
	long t_lvalue;					/*value or label number*/
};

	/*local symbol terminal node*/
struct symnode {
	int t_op;						/*operator*/
	int t_type;						/*symbol data type*/
	int t_su;						/*Sethy-Ullman number*/
	int t_ssp;
	int t_sc;						/*storage class*/
	int t_offset;					/*register offset*/
	int t_reg;						/*register number*/
	int t_label;					/*label number if static*/
};

	/*external symbol reference node*/
struct extnode {
	int t_op;						/*operator*/
	int t_type;						/*symbol data type*/
	int t_su;						/*Sethy-Ullman number*/
	int t_ssp;
	int t_sc;						/*storage class*/
	int t_offset;					/*register offset*/
	int t_reg;						/*register number*/
	char t_symbol[SSIZE];			/*symbol name*/
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

int lflag;
int dflag;
int mflag;
int cflag;
int eflag;
int fflag;
int oflag;
struct io_buf ibuf, obuf;
int lineno;
int naregs;
int ndregs;
int errcnt;
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
char *fpcnalloc();
char *canon();
char *commute();
char *constant();
char *match();
char *addptree();
char *fixbfield();
char *coffset();
char *tcopy();

#define	wallign(add)	((add+1)&(~1))
#define	array(type)		((type&SUPTYP)==ARRAY)
#define	function(type)	((type&SUPTYP)==FUNCTION)
#define	pointer(type)	((type&SUPTYP)==POINTER)
#define	notarray(type)		((type&SUPTYP)!=ARRAY)
#define	notfunction(type)	((type&SUPTYP)!=FUNCTION)
#define	notpointer(type)	((type&SUPTYP)!=POINTER)
#define isfloat(type)	(type==FLOAT)
#define	btype(type)		(type&TYPE)
#define	suptype(type)	(type&SUPTYP)
#define	alltype(type)	(type&(SUPTYP|TYPE))
#define	asgop(op)		((opinfo[op]&OPASSIGN)!=0)
#define	relop(op)		((opinfo[op]&OPREL)!=0)
#define	lintegral(op)	((opinfo[op]&OPLWORD)!=0)
#define	rintegral(op)	((opinfo[op]&OPRWORD)!=0)
#define	rasop(op)		((opinfo[op]&OPRAS)!=0)
#define	binop(op)		((opinfo[op]&OPBIN)!=0)
#define	unaryop(op)		((opinfo[op]&(OPBIN|OPTERM))==0)
#define	leafop(op)		((opinfo[op]&OPTERM)!=0)
#define	notleafop(op)	((opinfo[op]&OPTERM)==0)
#define	lvalop(op)		((opinfo[op]&OPLVAL)!=0)
#define	oppriority(op)	(opinfo[op]&OPPRI)
#define	commop(op)		((opinfo[op]&OPCOM)!=0)
#define	convop(op)		((opinfo[op]&OPCONVS)!=0)
#define	notconvop(op)	((opinfo[op]&OPCONVS)==0)
#define	max(a,b)		(a>b?a:b)
#define min(a,b)		(a<b?a:b)

#define	QUICKVAL		8
#define	LEP				14
#define	FORCC			1
#define	FOREFF			2
#define	FORSTACK		3
#define	FORCREG			4
#define	FORSP			5
#define	FORREG			4
#define	HICREG			2
#define	NCREGS			3
#define	AREGLO			8
#define	IMMED			1
#define	NOTIMMED		0
#define	NOTLOFFSET		0

/* one line routines turned into defines [vlh] for speed */

/*outgoto - output "bra L[labno]"*/
#define outgoto(lab)	if (lab>0) printf("bra L%d\n",lab)
/*outlab - output "L[labno]:"*/
#define outlab(lab)		if (lab>0) printf("L%d:",lab)

/*outext - output register sign extension*/
#define outext(reg)		printf("ext.l R%d\n",reg)
/*outuext - output unsigned to long register extension*/
#define outuext(reg)	printf("swap R%d\nclr R%d\nswap R%d\n",reg,reg,reg)
/*outswap - output swap register instruction*/
#define outswap(reg)	printf("swap R%d\n",reg)
/*outaddr - output "add [type] R1 R2" instruction*/
#define outaddr(r1,r2,tp)	outrr("add",r1,r2,(tp))
/*outccsave - ouput instruction to move cc's to register*/
#define outccsave(reg)	printf("move sr,R%d\n",reg)
/*outccrestore - output instruction to restore cc's from register*/
#define outccrestore(reg)	printf("move R%d,ccr\n",reg)
/*basetype - get the btype info sans unsigned*/
#define basetype(type)	((type==UNSIGNED) ? INT : type)
#define unsign(type)	((type) == UNSIGNED)
#define longorptr(type)	(type==LONG || (type&SUPTYP))
#define unorptr(type)	(type==UNSIGNED || (type&SUPTYP))
#define dreg(reg)		((reg) & (~AREGLO))
#define areg(reg)		((reg) | AREGLO)
#define isareg(reg)		((reg) >= AREGLO)
#define isdreg(reg)		((reg) < AREGLO)
#define isreg(tp)		((tp)->t_op == SYMBOL && (tp)->t_sc == REGISTER)
