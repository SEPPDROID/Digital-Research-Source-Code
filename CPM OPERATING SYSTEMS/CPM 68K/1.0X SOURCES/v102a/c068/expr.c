/*
	Copyright 1982, 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)expr.c	1.8	12/15/83
*/

#include "parser.h"

#define IS_ERROR 	0
#define IS_TERMINAL 1
#define IS_OTHER 	2

/**
 * expr - expression evaluator
 *		This handles all the expression syntax in C.  This is a 
 *		straigt forward operator-stack/oppri scheme for translating 
 *		infix into a binary expression tree.
**/
char *
expr(preset)						/* returns 0 or ptr to node*/
int preset;							/* [vlh] 4.2, stack setup ?? */
{
	register char *p;
	register short op, oppri;
	short token;

	if (!preset)
		expr_setup();

	while( (token=gettok(0)) != EOF ) {

		switch( is_terminal(&token) ) {
			case IS_ERROR:		/* error condition */
exprerr:
				if( token == SEMI || token == RCURBR )
					pbtok(token);
				error("invalid expression");
				opdp = (char **)opdsave;
				opp = (struct ops *)oprsave;
				opdontop = opdotsave;
				return(0);

			case IS_TERMINAL:	/* terminal node */
				continue;

			default:			/* IS_OTHER operator node... */
				break;
		}	/* switch is_terminal() */

	/**
	 * we have seen an operator, get its info and then check the operator
	 * stack.
	**/
		if( BINOP(token) ) {
	/**
	 * handle special binary operators, such as CAST and post-inc and
	 * post-dec.
	**/
			if(!opdontop)
				if( token != PREDEC && token != PREINC && token != CAST)
					goto exprerr;
			if( token != POSTDEC && token != POSTINC && token != MPARENS )
				opdontop = 0;				/*this fixes x++ op problem.*/
		}
		oppri = OPPRIORITY(token);
		if((commastop && token == COMMA) ||	/*need to stop at comma(init)*/
		   (colonstop && token == COLON))   /*need to stop at colon(case)*/
			oppri = COLPRI;
		while( 1 ) {
			if (oppri>opp->o_pri || (oppri==opp->o_pri && RASOP(token))) {
	/**
	 * we have encountered a higher priority (or right-associative)
	 * operator, hence we need to stack it.
	**/
				if( ++opp >= &opstack[OPSSIZE] )
					ferror("expression too complex");
				if( token == POSTINC || token == POSTDEC )
					oppri = PSTPRI;
				else if( token==LPAREN || token==LBRACK || token==CALL )
					oppri = CALPRI;
				opp->o_op = token;
				opp->o_pri = oppri;
				break;
			}
	/**
	 * we have encountered a lower priority operator, hence we need to
	 * do a reduction.
	**/
			op = opp->o_op;
			opp--;
			switch( op ) {
		
				case STACKEND:			/*we accept the expression...*/
					pbtok(token);
					if (!strassign && !maketree(0)) 
						goto exprerr;
					if (!(p=popopd()))
						goto exprerr;
					opdp = (char **)opdsave;
					opp = (struct ops *)oprsave;
					opdontop = opdotsave;
					return(p);
			
				case LPAREN:			/*assure these have matching )*/
				case CALL:
					if( token != RPAREN )
						goto exprerr;
					break;

				case MPARENS:
					if( !maketree(NACALL) ) {
						warning("Null expression encountered");
						return(0);
						/*goto exprerr;*/
					}
					continue;
			
				case LBRACK:
					if( token != RBRACK ) {
						goto exprerr;
					}
					if(!maketree(ADD))	/*array[index]->*(array+index)*/
						goto exprerr;
					op = INDR;
					break;
			
				case PREINC:			/*turn these into binary operators*/
				case POSTINC:			/*which in reality they are...*/
				case PREDEC:
				case POSTDEC:
					pushopd(cnalloc(INT,1));
				default:
					if(!maketree(op))
						goto exprerr;
					continue;			/*see if we can reduce some more...*/

			}	/* switch on op */
			if( op != LPAREN && !maketree(op) )
				goto exprerr;
			break;
		}	/* while (1) */
	}
	error("unexpected EOF");
	return(0);
}

/* expr_setup - setup op stack for expression evaluation */
expr_setup()
{
	opdsave = (char *)opdp;
	strassign = 0;
	oprsave = (char *)opp;
	opdotsave = opdontop;
	if( !opp || !opdp ) {
		opp = opstack;
		opdp = opdstack;
	}
	else
		opp++;
	opp->o_op = STACKEND;
	opp->o_pri = STKPRI;
	opap = exprp;
	opdontop = 0;
}

/**
 * is_terminal - token switch for expr()
 * the following are the terminal nodes of the expression tree,
 * note that when we see a terminal node, we push it and then go
 * and get the next token.  When we see an operator, we need to
 * check the operator stack to see if we can do a reduction.
**/
is_terminal(token)		/* returns 0 for error, 1 for terminal, 2 for other */
short *token;
{
	register struct tnode *p;
	short type, sc;
	long size;

	switch( *token ) {
		case CINT:
			if( doopd(cnalloc(INT,cvalue)) )
				return(IS_ERROR);
			return(IS_TERMINAL);
	
		case CFLOAT:	/*[vlh] 3.4*/
			if( doopd(fpcnalloc(FLOAT,clvalue)) )
				return(IS_ERROR);
			return(IS_TERMINAL);
	
		case CLONG:
			if( doopd(lcnalloc(LONG,clvalue)) )
				return(IS_ERROR);
			return(IS_TERMINAL);
	
		case SYMBOL:
			p = (struct tnode *)get_symbol();
			if( doopd(p) )
				return(IS_ERROR);
			return(IS_TERMINAL);
	
		case STRING:
			outtstr(cvalue);
			if( doopd(snalloc(ARRAY|CHAR,STATIC,cvalue,0,0)) )
				return(IS_ERROR);
			return(IS_TERMINAL);
	/**
	 * do special checking for unary ops and operators that can be
	 * either unary or binary operators, such as -, &, *, etc.
	**/
		case RESWORD:
			if(cvalue == R_SIZEOF)
				*token = SIZEOF;
			else if (cvalue == R_ASM)
				*token = ASM;
			else
				return(IS_ERROR);
		case COMPL:
		case NOT:
			if( opdontop )			/*can't have: operand unary-op*/
				return(IS_ERROR);
			break;

		case LBRACK:
			opdontop = 0;
			break;
	
		case RPAREN:	/* [vlh] 4.2.f, was with RBRACK */
			if (!opdontop) {
				*token = MPARENS;
				opdontop = 1;	/* fake out expr for null expression... */
			}
			break;

		case RBRACK:
			if( !opdontop )			/*can't be: operator )*/
				return(IS_ERROR);
			break;
	
		case PREINC:
			if( opdontop )			/*assume its lvalue++*/
				*token = POSTINC;
			break;

		case PREDEC:
			if( opdontop )			/*assume its lvalue--*/
				*token = POSTDEC;
			break;
	
		case SUB:
			if(!opdontop) {	/*if no operand, assume unary*/
				if(PEEK(CINT)) {
					cvalue = -cvalue;
					return(IS_TERMINAL);
				}
				if(PEEK(CLONG)) {
					clvalue = -clvalue;
					return(IS_TERMINAL);
				}
				if(PEEK(CFLOAT)) {	/*[vlh] 3.4*/
					if (!fflag) {		/* IEEE format */
						if (clvalue & 0x80000000)
							clvalue &= 0x7fffffff;
						else
							clvalue |= 0x80000000;
					}
					else			/* FFP format */
						if (clvalue & 0x80)
							clvalue &= 0xffffff7f;
						else
							clvalue |= 0x80;
					return(IS_TERMINAL);
				}
				*token = UMINUS;
			}
			break;
	
		case AND:
			if(!opdontop)
				*token = ADDR;
			break;

		case MULT:
			if( !opdontop )
				*token = INDR;
			break;

		case LPAREN:
			if( !opdontop ) {	/*see if casting or abstract declarator*/
				sc = type = 0;
				if( gettype(&sc,&type,&size,0) ) {
					sc = (type == STRUCT) ? dalloc(size) : cdp;
					p = (struct tnode *)snalloc(type,STATIC,0,sc,sc);
					p->t_type |= declarator(1);
					if( !next(RPAREN) )
						return(IS_ERROR);
					if( tdp )
						p->t_type=addtdtype(tdp,p->t_type,p->t_dp,&(p->t_ssp));
					pushopd(p);
					*token = CAST;
					if( opp->o_op == SIZEOF ) {
						opdontop++;
						return(IS_TERMINAL);
					}
				}
			}
			else  /*we've seen (), look for NACALL*/
				*token = (next(RPAREN)) ? MPARENS : CALL;
			break;
	
		case PERIOD:
		case APTR:
			smember++;			/*next token needs to be struct membr*/
			struc_parent[in_struct] = csp->s_par;
			break;
	
	}	/* switch on token */
	return(IS_OTHER);
}


/* get_symbol - handle symbol element for expr() */
char *
get_symbol()
{
	register struct symnode *p;
#ifdef SYM_TO_DSK
	long p_addr;
	struct symbol p_entry;

	p_entry = csp_entry;
	p_addr = csp_addr;
	p = &p_entry;
#else
	p = (struct symnode *)csp;
#endif
	if( !(p->s_attrib&SDEFINED) ) {
		if( PEEK(LPAREN) ) {	/*assume function call*/
			p->s_sc = EXTERNAL;
			p->s_type = FUNCTION|INT;
			p->s_scope = GLOB_SCOPE;	/* [vlh] 4.2 */
		}
		else if( commastop )	/*in initialization?*/
			p->s_sc = EXTERNAL;
		else {
		   error("undefined symbol: %.8s",p->s_symbol);
		}
		p->s_attrib |= SDEFINED;
		TO_DSK(p,p_addr);
	}
	if (p->s_sc==EXTERNAL || ISFUNCTION(p->t_type)) {
		if (!reducep || p->s_sc==EXTERNAL || PEEK(LPAREN)) {
			p = (struct symnode *)enalloc(p);
			p->t_sc = EXTERNAL;
		}
		else 	/* [vlh] 3.4 if (main).... */
			p = (struct symnode *)cnalloc(CINT,1);	/* eval function name */
	}
	else
		p = (struct symnode *)snalloc(p->s_type,p->s_sc,p->s_offset,
									p->s_dp,p->s_ssp);
	READ_ST(csp,csp_addr);
	return((char *)p);
}

/* binopeval - does binary operator constant expression evaluation*/
/*		Does the constant expression evaluation for binary operators.*/
binopeval(op,ltp,rtp)				/* returns 1 if done, 0 if not*/
int op;								/* operator to evaluate*/
struct lconode *ltp;				/* pointer to left subtree*/
struct lconode *rtp;				/* pointer to right subtree*/
{
	register long lvalue, rvalue;	/* [vlh] 4.1 short -> long */
	short islong, size;

	PUTEXPR(treedebug,"binopeval l",ltp);
	PUTEXPR(treedebug,"binopeval r",rtp);
	islong = 0;

	if( rtp->t_op == CINT )
		rvalue = ((struct conode *)rtp)->t_value;
	else if (rtp->t_op == CLONG ) {	/* [vlh] 4.1 added CLONG */
		rvalue = rtp->t_lvalue;
		islong++;
	}
	else
		return(0);

	if( ltp->t_op == CINT)
		lvalue = ((struct conode *)ltp)->t_value;
	else if (ltp->t_op == CLONG ) {	/* [vlh] 4.1 added CLONG */
		if ((ltp->t_type & POINTER) && op==ADD) { /* [vlh]4.3 array coercion */
			if(ltp->t_ssp) {	/* if not structure pointer coercion */
				size = dsize(delsp(ltp->t_type),ltp->t_dp,ltp->t_ssp);
				rvalue *= size;
			}
#ifdef DEBUG
			warning("constant coerced");
#endif
		}
		lvalue = ltp->t_lvalue;
		islong++;
	}
	else
		return(0);
	
	switch (op) {

		case ADD:
			lvalue += rvalue;
			break;

		case SUB:
			lvalue -= rvalue;
			break;

		case MULT:
			lvalue *= rvalue;
			break;

		case DIV:
			lvalue /= rvalue;
			break;

		case MOD:
			lvalue %= rvalue;
			break;

		case AND:
			lvalue &= rvalue;
			break;

		case OR:
			lvalue |= rvalue;
			break;

		case XOR:
			lvalue ^= rvalue;
			break;

		case LSH:
			lvalue <<= rvalue;
			break;

		case RSH:
			lvalue >>= rvalue;
			break;

		case EQUALS:
			lvalue = (lvalue == rvalue);
			break;

		case NEQUALS:
			lvalue = (lvalue != rvalue);
			break;

		case GREAT:
			lvalue = (lvalue > rvalue);
			break;

		case LESS:
			lvalue = (lvalue < rvalue);
			break;

		case GREATEQ:
			lvalue = (lvalue >= rvalue);
			break;

		case LESSEQ:
			lvalue = (lvalue <= rvalue);
			break;

		default:
			return(0);
	}
	if ( islong || ( (lvalue>0) && (rvalue>0) && (lvalue>0x7fff) ) ) {
		ltp->t_op = CLONG;
		ltp->t_lvalue = lvalue;
		if(ltp->t_type==INT)
			ltp->t_type = LONG;		/* 4.2+ */
	}	/* [vlh] 4.3, turn int into long if necessary */
	else
		((struct conode *)ltp)->t_value = lvalue;
	pushopd(ltp);
	PUTEXPR(treedebug,"result binop",ltp);
	return(1);
}

/* unopeval - unary operator constant expression evaluation*/
/*		Does constant expression evaluation for unary operators.*/
unopeval(op,tp)						/* returns 1 if done, 0 otherwise*/
int op;								/* operator to evaluate*/
struct lconode *tp;					/* pointer to subexpression*/
{
	register long value;

	if( tp->t_op == CINT )
		value = ((struct conode *)tp)->t_value;
	else if( tp->t_op == CLONG)	/* [vlh] 4.1 added clong.... */
		value = tp->t_lvalue;
	else
		return(0);
	switch( op ) {

		case COMPL:
			value = ~ value;
			break;

		case UMINUS:
			value = - value;
			break;

		case NOT:
			value = ! value;
			break;

		default:
			return(0);
	}
	if( tp->t_op == CINT )
		((struct conode *)tp)->t_value = value;
	else 						/* [vlh] 4.1 added clong.... */
		tp->t_lvalue = value;
	pushopd(tp);
	return(1);
}

/* cexpr - evaluate a constant integer expression*/
/*		Used in evaluating array bounds, bit field numbers, etc.*/
long
cexpr()						/* returns the constant value */
{
	register struct lconode *tp;
	register char *savep;
	register short op;

	savep = exprp;
	exprp = opap;
	commastop++;
	tp = (struct lconode *)expr(0);
	op = tp->t_op;
	if (op != CINT && op != CLONG)
		error("constant required"); /* [vlh] 4.1 added CLONG... */
	commastop--;
	exprp = savep;
	return((op==CLONG) ? tp->t_lvalue : (long)((struct conode *)tp)->t_value);
}

