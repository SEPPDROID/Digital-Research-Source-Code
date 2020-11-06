/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"
#ifdef	DEBUG

char invalid[] "INVALID";

char *opname[] {
	invalid,						/*0*/
	"+",							/*1*/
	"-",							/*2*/
	"*",							/*3*/
	"/",							/*4*/
	"%",							/*5*/
	">>",							/*6*/
	"<<",							/*7*/
	"&",							/*8*/
	"|",							/*9*/
	"^",							/*10*/
	"!",							/*11*/
	"U-",							/*12*/
	"~",							/*13*/
	"--p",							/*14*/
	"++p",							/*15*/
	"p--",							/*16*/
	"p++",							/*17*/
	"=",							/*18*/
	"+=",							/*19*/
	"-=",							/*20*/
	"*=",							/*21*/
	"/=",							/*22*/
	"%=",							/*23*/
	">>=",							/*24*/
	"<<=",							/*25*/
	"&=",							/*26*/
	"|=",							/*27*/
	"^=",							/*28*/
	"jsr",							/*29*/
	"==",							/*30*/
	"!=",							/*31*/
	">",							/*32*/
	">=",							/*33*/
	"<",							/*34*/
	"<=",							/*35*/
	"int->long",					/*36*/
	"long->int",					/*37*/
	"btst",							/*38*/
	"load",							/*39*/
	"long*",						/*40*/
	"long/",						/*41*/
	"long%",						/*42*/
	"long*=",						/*43*/
	"long/=",						/*44*/
	"long%=",						/*45*/
	"=addr",						/*46*/
	"=not",							/*47*/
	"=neg",							/*48*/
	"docast",						/*49*/
	"st=",							/*50*/
	"ltof",							/*51*/
	"ftol",							/*52*/
	"itof",							/*53*/
	"ftoi",							/*54*/
	"tochar",						/*55*/
	invalid,						/*56*/
	invalid,						/*57*/
	invalid,						/*58*/
	invalid,						/*59*/
	"U&",							/*60*/
	"U*",							/*61*/
	"&&",							/*62*/
	"||",							/*63*/
	"?",							/*64*/
	":",							/*65*/
	",",							/*66*/
	"cint",							/*67*/
	"clong",						/*68*/
	"symbol",						/*69*/
	"++a",							/*70*/
	"a--",							/*71*/
	"call",							/*72*/
	"call()",						/*73*/
	"bitfield",						/*74*/
	"if",							/*75*/
	"init",							/*76*/
	"loadR0",						/*77*/
	"divlong",						/*78*/
};

char *types[] {
	invalid,						/*0=TYPELESS*/
	"char",							/*1=CHAR*/
	invalid,						/*2=SHORT*/
	"int",							/*3=INT*/
	"long",							/*4=LONG*/
	invalid,						/*5=UCHAR*/
	invalid,						/*6=USHORT*/
	"uint",							/*7=UINT*/
	invalid,						/*8=ULONG*/
	"float",						/*9=FLOAT*/
	invalid,						/*10=DOUBLE*/
	"struct",						/*11=STRUCT*/
	invalid,						/*12=undefined*/
	invalid,						/*13=undefined*/
	invalid,						/*14=undefined*/
	invalid,						/*15=undefined*/
};

char *suvals[] {
	"zero",
	"one",
	"quick",
	"small",
	"constant",
	"Areg",
	"Dreg",
	"addressable",
	"loadable",
	"easy",
	"hard",
	"veryhard",
};

int level;

putexpr(name,tp)
char *name;
struct tnode *tp;
{
	printf("%s\n",name);
	putsexpr(tp);
}

putsexpr(tp)
struct tnode *tp;
{
	register struct tnode *ltp;

	level++;
	ltp = tp->t_left;
	outlevel();
	printf("%s ",opname[tp->t_op]);
	puttsu(tp);
	if (tp->t_op < 0 || tp->t_op > 78) {
		printf("INVALID op\n");
		return;
	}
	switch( tp->t_op ) {

	case DCLONG:
	case CLONG:
	case CFLOAT:	/*[vlh] 3.4 */
		printf(" %x.%x\n",tp->t_lvalue.hiword,tp->t_lvalue.loword);
		break;


	case CINT:
		printf(" %d\n",tp->t_value);
		break;

	case AUTODEC:
	case AUTOINC:
		printf("Autodec or Autoinc");
		break;

	case SYMBOL:
		switch( tp->t_sc ) {

		case REGISTER:
			printf(" Register");
			break;

		case CINDR:
			printf(" %d\n",tp->t_offset);
			break;

		case CLINDR:
		case CFINDR:	/* [vlh] 3.4 */
			printf(" %x.%x\n",tp->t_offset,tp->t_ssp);
			break;

		case REGOFF:
			printf(" Regoffset");
			break;

		case EXTERNAL:
		case EXTOFF:
			printf(" %s+%d",tp->t_symbol,tp->t_offset);
			if( tp->t_sc == EXTOFF )
				printf("Ext offset");
			break;

		case STATIC:
		case STATOFF:
			printf(" STATIC or STATOFF");
			if( tp->t_sc == STATOFF )
				printf("STATOFF");
			break;

		case INDEXED:
			printf(" %d indexed",tp->t_offset);
			break;
		}
		putchar('\n');
		break;

	case IFGOTO:
		putsexpr(tp->t_left);
		break;

	default:
		putchar('\n');
		putsexpr(tp->t_left);
		if( binop(tp->t_op) )
			putsexpr(tp->t_right);
		break;
	}
	level--;
}

outlevel()
{
	register int i;

	for( i = 0; i < level; i++ )
		printf("     ");
}

puttsu(tp)
struct tnode *tp;
{
	register int i;

	switch( suptype(tp->t_type) ) {

	case FUNCTION:
		printf("()");
		break;

	case ARRAY:
		printf("[]");
		break;

	case POINTER:
		printf("*");
		break;
	}
	printf("%s ",types[btype(tp->t_type)]);
}
#endif
