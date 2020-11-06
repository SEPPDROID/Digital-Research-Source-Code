#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include "icode.h"
#define	NODEBUG	1



char brtab[][2];
int invrel[];
int swaprel[];
char *strtab[];
char null[];

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

