#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include "cgen.h"
int bol;
int onepass;

/* outexpr - output expression*/
outexpr(tp)						/* returns - none*/
struct tnode *tp;						/* pointer to tree node*/
{
	if( dflag )
		outline();
	if( exprok(tp) )
		scodegen(canon(tp),FOREFF,0);
}

outforreg(tp)
struct tnode *tp;
{
	register int reg;

	if( dflag )
		outline();
	if( exprok(tp) && (reg=scodegen(canon(tp),FORREG,0)) != 0 )
		outmovr(reg,0,tp);
}

outifgoto(tp,dir,lab)
struct tnode *tp;
int dir;
int lab;
{
	if( dflag )
		outline();
	if( exprok(tp) )
		condbr(canon(tp),dir,lab,0);
}

outinit(tp)							/* returns - none*/
struct tnode *tp;
{
	register int typeout;

	if( dflag )
		outline();
	if( exprok(tp) ) {
		typeout = tp->t_type;
		tp = canon(tp);
		if( tp->t_op == ADDR )
			tp = tp->t_left;
		if( tp->t_op == CINT || tp->t_op == SYMBOL ) {
			if( tp->t_op == SYMBOL )
				printf(".dc.l ");
			else {
				printf(".dc.");
				outtype(typeout);
				putchar(' ');
			}
			outaexpr(tp,IMMED);
		}
		else
			error("invalid initialization");
		putchar('\n');
	}
}

/* snalloc - code generator symbol node allocation*/
/*		This might be coalesced into parser snalloc.*/
char *snalloc(type,sc,offset,dp,ssp)	/* returns ptr to node alloced*/
int type;						/* type of symbol*/
int sc;							/* storage class*/
int offset;						/* offset from Local Environment Ptr*/
int dp;							/*for compatability with parser*/
int ssp;						/*for compatability with parser*/
{
	register struct symnode *sp;

	sp = talloc(sizeof(*sp));
	sp->t_op = SYMBOL;
	sp->t_type = type;
	sp->t_su = dp;
	sp->t_ssp = ssp;
	sp->t_sc = sc;
	switch( sc ) {

	case STATIC:
		sp->t_offset = 0;
		sp->t_reg = 0;
		sp->t_label = offset;
		break;

	case REGISTER:
		sp->t_offset = 0;
		sp->t_reg = offset;
		sp->t_label = 0;
		break;

	case AUTO:
		sp->t_sc = REGOFF;
		sp->t_offset = offset;
		sp->t_reg = LEP;
		sp->t_label = 0;
		break;

	default:
		sp->t_offset = offset;
		sp->t_reg = 0;
		sp->t_label = 0;
		break;
	}
	return(sp);
}

exprok(tp)
struct tnode *tp;
{
	if( tp < exprarea || tp > &exprarea[EXPSIZE] )
		return(0);
	if( leafop(tp->t_op) )
		return(1);
	if( binop(tp->t_op) && exprok(tp->t_right) == 0 )
		return(0);
	return( exprok(tp->t_left) );
}

outline()
{
	if( onepass && bol == 0 )
		putchar('\n');
	printf("*line %d\n",lineno);
}
