/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"
int bol;

outinit(tp,type)						/* returns - none*/
struct tnode *tp;
{
	outexpr(tnalloc(INIT,type,0,0,tp));
}

outcforreg(tp)
struct tnode *tp;
{
	outexpr(tnalloc(CFORREG,tp->t_type,0,0,tp));
}

outifgoto(tp,dir,lab)
struct tnode *tp;
int dir;
int lab;
{
	outexpr(tnalloc(IFGOTO,dir,lab,0,tp));
}

outexpr(tp)
struct tnode *tp;
{
	if( !bol )
		putchar('\n');
	printf(".%x\n",lineno);
	outtree(tp);
}

outtree(tp)
struct tnode *tp;
{
	if( !tp )
		return;
	printf("%x.%x",tp->t_op,tp->t_type);
	switch( tp->t_op ) {

	case CINT:
		printf(".%x\n",tp->t_value);
		break;

	case CLONG:
		printf(".%x.%x\n",tp->t_lvalue.hiword,tp->t_lvalue.loword);
		break;

	case CFLOAT:	/*[vlh] 3.4*/
		printf(".%x.%x\n",tp->t_lvalue.hiword,tp->t_lvalue.loword);
		break;

	case SYMBOL:
		printf(".%x",tp->t_sc);
		if( tp->t_sc == EXTERNAL )
			printf(".%.8s\n",tp->t_symbol);
		else
			printf(".%x\n",tp->t_offset);
		break;

	case 0:
		putchar('\n');
		break;

	case IFGOTO:
	case BFIELD:
		printf(".%x\n",tp->t_dp);
		outtree(tp->t_left);
		break;

	default:
		putchar('\n');
		outtree(tp->t_left);
		if( binop(tp->t_op) )
			outtree(tp->t_right);
		break;
	}
}

