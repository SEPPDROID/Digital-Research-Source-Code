/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "cgen.h"
#include "cskel.h"

/*
 * canon - expression canonicalization
 *		Top level tree canonicalization.  This fixes any bit field
 *		accesses, then loops on commute and optim until no more
 *		optimizations are done.
 */
char *canon(tp)						/* returns pointer to tree*/
struct tnode *tp;					/* pointer to tree to canonicalize*/
{
#ifndef NODEBUG
	if ( oflag )
		putexpr("pre-canon",tp);
#endif
	tp = fixbfield(tp);
	do {
		sucomp(tp,0,1);				/*need Sethy-Ullman's for commute*/
		tp = commute(tp);			/*commute the tree*/
	} while( optim(&tp) );			/*keep trying for optimizations*/
#ifndef	NODEBUG
	if( oflag )
		putexpr("canon",tp);
#endif
	return(tp);
}

/* fixbfield - fix bit field operators*/
/*		Fixes bit field assignment and normal usage*/
char *fixbfield(tp)						/* returns pointer to fixed tree*/
struct tnode *tp;						/* pointer to tree*/
{
	register struct tnode *btp, *stp;
	register int foff, fmask, op, flen;

	if( leafop(op=tp->t_op) )
		return(tp);
	if( asgop(op) && tp->t_left->t_op == BFIELD ) {
		tp->t_right = fixbfield(tp->t_right);	/* [vlh] 4.0 */
		btp = tp->t_left;				/*pointer to BFIELD node*/
		stp = btp->t_left;				/*pointer to son of BFIELD node*/
		foff = (btp->t_su>>8) & 0377;
		flen = btp->t_su & 0377;
		fmask = (1<<flen)-1;
		if( tp->t_right->t_op == CINT && (op == ASSIGN || op == EQXOR) ) {
			if( op == EQXOR ) {
				tp->t_left = stp;
				tp->t_right->t_value =<< foff;
				return(tp);
			}
			if( !tp->t_right->t_value ) {
				tp->t_op = EQAND;
				tp->t_left = stp;
				tp->t_right->t_value = ~ (fmask << foff);
				return(tp);
			}
			if( (tp->t_right->t_value & fmask) == fmask ) {
				tp->t_op = EQOR;
				tp->t_left = stp;
				tp->t_right->t_value = fmask << foff;
				return(tp);
			}
		}
		if( fmask == -1 )
			tp->t_left = stp;
		else {
			stp = tcopy(stp);
			btp = fixbfield(btp);
			op = tp->t_op;
			tp = tp->t_right;				/*pointer to expression*/
			if( op != ASSIGN )				/*=op operator?*/
				tp = tnalloc(op-(EQADD-ADD),INT,0,0,btp,tp);
			tp = tnalloc(AND,INT,0,0,tp,cnalloc(INT,fmask));
			tp = tnalloc(LSH,INT,0,0,tp,cnalloc(INT,foff));
			btp = tnalloc(AND,INT,0,0,tcopy(stp),
					cnalloc(INT,~(fmask<<foff)));
			tp = tnalloc(OR,INT,0,0,btp,tp);
			tp = tnalloc(ASSIGN,INT,0,0,stp,tp);
		}
	}
	else if( op == BFIELD ) {
		foff = (tp->t_su>>8) & 0377;
		fmask = (1<<(tp->t_su&0377))-1;
		tp = tnalloc(RSH,INT,0,0,tp->t_left,cnalloc(INT,foff));
		tp = tnalloc(AND,INT,0,0,tp,cnalloc(INT,fmask));
	}
	else {
		tp->t_left = fixbfield(tp->t_left);
		if( binop(op) )
			tp->t_right = fixbfield(tp->t_right);
	}
	return(tp);
}

/*
 * optim - optimize expression tree
 *		This takes expression tree and performs the following
 *		translations:  folds auto names to accesses off the local
 *		environment pointer, performs mostly "machine-independent"
 *		optimizations, such as multiply by zero and one, etc.,
 *		turns field accesses into and's and or's, etc.
 *		[vlh] 4.0 added checks for divide/modulos long zero...
 *		[vlh] 4.0 optimize multiply/divide 1L, add/sub 0L...
 */
optim(tpp)							/* returns pointer to new tree*/
struct tnode **tpp;
{
	register struct tnode *ltp, *rtp, *tp;
	register char *p;
	register int i, cval, changes, op, lconst;
	long lcval;		/* [vlh] 4.0 simple long constant handling */

	tp = *tpp;
	if( tp->t_type & ~TYPE ) {
		tp->t_type =& TYPE;
		tp->t_type =| POINTER;
	}
	if( leafop(op=tp->t_op) )
		return(0);
	lconst = changes = 0;
	if( binop(op) )
		changes =+ optim(&tp->t_right);
	changes =+ optim(&tp->t_left);
	for( ; notleafop(op=tp->t_op); changes++ ) {
#ifndef NODEBUG
		if( oflag )
			putexpr("optim",tp);
#endif
		*tpp = tp;
		ltp = tp->t_left;
		if( binop(op) ) {
			rtp = tp->t_right;
/*			if( tp->t_type == CHAR )	[vlh] 4.0 */
/*				tp->t_type = INT;	[vlh] 4.0 */
			if (!(p = constant(rtp)))
				if (rtp->t_op == CLONG) {
					lconst = 1;
					lcval = rtp->t_lvalue;
				}
		}
		else
			if (!(p = constant(ltp)))
				if (ltp->t_op == CLONG) {
					lconst = 1;
					lcval = ltp->t_lvalue;
				}
		if( p )
			cval = p->t_value;
		switch( op ) {
	
		case ADD:
			if( p ) {
				if( !cval ) {
					tp = ltp;
					continue;
				}
				if( ltp->t_op == ADDR ) {
					ltp->t_left->t_offset =+ cval;
					tp = ltp;
					continue;
				}
			}
			else
				if (lconst && !lcval) {	/* [vlh] 4.0 plus long 0 */
					tp = ltp;
					continue;
				}
			break;
	
		case ASSIGN:
			if( rtp->t_op == ADD && indexreg(rtp->t_left) &&
					rtp->t_right->t_op == CINT ) {
				ltp = rtp->t_left;
				ltp->t_sc = REGOFF;
				ltp->t_offset =+ rtp->t_right->t_value;
				tp->t_right = ltp;
				tp->t_op = EQADDR;
				continue;
			}
			break;
	
		case SUB:
			if( p ) {
				if( !cval ) {
					tp = ltp;
					continue;
				}
				tp->t_op = ADD;
				p->t_value = -cval;
				continue;
			}
			else
				if (lconst && !lcval) {	/* [vlh] 4.0 minus long 0... */
					tp = ltp;
					continue;
				}
			if( (p=constant(ltp)) && !p->t_value ) {	/*0-X=-X*/
				tp->t_op = UMINUS;
				tp->t_left = ltp = rtp;
				continue;
			}
			break;
	
		case DIV:
		case EQDIV:
			if( p ) {
				if( !cval ) {		/*X/0->error*/
					error("divide by zero");
					tp = rtp;
					continue;
				}
				if( cval == 1 ) {
					tp = ltp;
					continue;
				}
				if( cval == -1 ) {
					tp->t_op = (tp->t_op == DIV ? UMINUS : EQNEG );
					continue;
				}
			}
			else
				if (lconst) {	/* [vlh] 4.0 simple long expr */
					if (!lcval) {
						error("divide by zero");
						tp = rtp;
						continue;
					}
					if (lcval == 1L) {
						tp = ltp;
						continue;
					}
				}
			if( (p=constant(ltp)) && !p->t_value ) {
				tp = ltp;
				continue;
			}
			if( multop(tpp) ) {
				tp = *tpp;
				continue;
			}
			break;
	
		case EQMOD:
		case MOD:
			if( p ) {
				if( !cval ) {		/*X%0->error*/
					error("modulus by zero");
					tp = rtp;
					continue;
				}
				if( cval == 1 ) {		/*X%1->0*/
					p->t_value = 0;
					if (op == EQMOD)	/*[vlh] 4.0*/
						tp->t_op = ASSIGN;
					else
						tp = rtp;
					continue;
				}
			}
			else	/* [vlh] 4.0 check for mod long 0 */
				if (lconst) {
					if(!lcval) {
						error("modulus by zero");
						tp = rtp;
						continue;
					}
					if (lcval == 1L) {
						rtp->t_lvalue = 0L;
						if (op == EQMOD)
							tp->t_op = ASSIGN;
						else
							tp = rtp;
						continue;
					}
				}
			if( (p=constant(ltp)) && !p->t_value ) {
				tp = ltp;
				continue;
			}
			if( multop(tpp) ) {
				tp = *tpp;
				continue;
			}
			break;
	
		case MULT:
		case EQMULT:
			if( p ) {
				if( !cval ) {
					if (op == EQMULT)
						tp->t_op = ASSIGN;
					else
						tp = rtp;
					continue;
				}
				if( cval == 1 ) {
					tp = ltp;
					continue;
				}
				if( cval == -1 ) {
					tp->t_op = (tp->t_op == MULT ? UMINUS : EQNEG);
					continue;
				}
			}
			else
				if (lconst) {	/* [vlh] 4.0 simple long expr */
					if (!lcval) {
						if (op == EQMULT)
							tp->t_op = ASSIGN;
						else
							tp = rtp;
						continue;
					}
					if (lcval == 1L) {
						tp = ltp;
						continue;
					}
				}
			if( multop(tpp) ) {
				tp = *tpp;
				continue;
			}
			break;

		case EQUALS:
		case NEQUALS:
			if( p && (i=onebit(cval)) >= 0 && ltp->t_op == AND &&
					(rtp=constant(ltp->t_right)) &&
					i == onebit(rtp->t_value) ) {
				tp->t_op = invrel[tp->t_op-EQUALS];
				p->t_value = 0;
				continue;
			}
			break;
	
		case GREATEQ:
			if( p && !cval && unsign(ltp->t_type) ) {
				p->t_value = 1;
				tp = p;
				continue;
			}
			break;
	
		case LESS:
			if( p && !cval && unsign(ltp->t_type) ) {
				tp = p;
				continue;
			}
			break;
	
		case AND:
			if( p ) {
				if( !cval ) {
					tp = rtp;
					continue;
				}
				if( cval == -1 ) {
					tp = ltp;
					continue;
				}
			}
			break;
	
		case OR:
			if( p ) {
				if( !cval ) {
					tp = ltp;
					continue;
				}
				if( cval == -1 ) {
					tp = rtp;
					continue;
				}
			}
			break;
	
		case EQXOR:
		case XOR:
			if( p ) {
				if( !cval ) {
					tp = ltp;
					continue;
				}
				if( cval == -1 ) {
					tp->t_op = (tp->t_op == XOR ? COMPL : EQNOT);
					continue;
				}
			}
			break;
	
		case LSH:
		case EQLSH:
			if( tp->t_type == LONG && rtp->t_op == INT2L &&
					unsign(rtp->t_left) ) {
				tp->t_right = rtp->t_left;
				continue;
			}
		case RSH:
		case EQRSH:
			if( p ) {
				if( !cval ) {
					tp = ltp;
					continue;
				}
				if( p != rtp ) {
					tp->t_right = p;
					continue;
				}
			}
			break;
	
		case ADDR:
			if( ltp->t_op == INDR ) {
				tp = ltp->t_left;
				continue;
			}
			if( ltp->t_op == SYMBOL && ltp->t_sc == REGOFF ) {
				tp->t_op = ADD;
				tp->t_right = cnalloc(INT,ltp->t_offset);
				ltp->t_type = tp->t_type;
				ltp->t_sc = REGISTER;
				ltp->t_offset = 0;
				continue;
			}
			break;
	
		case INDR:
			p = ltp->t_left;
			switch( ltp->t_op ) {

			case ADDR:
				tp = p;
				continue;

			case CINT:
				tp = snalloc(tp->t_type,CINDR,ltp->t_value,0,0);
				continue;

			case CLONG:
				tp = snalloc(tp->t_type,CLINDR,ltp->t_lvalue.hiword,0,
						ltp->t_lvalue.loword);
				continue;

			case CFLOAT:	/* [vlh] 3.4 */
				tp = snalloc(tp->t_type,CFINDR,ltp->t_lvalue.hiword,0,
						ltp->t_lvalue.loword);
				continue;

			case SYMBOL:
				if( indexreg(ltp) ) {
					ltp->t_sc = REGOFF;
					ltp->t_type = tp->t_type;
					ltp->t_offset = 0;
					tp = ltp;
					continue;
				}
				break;

			case ADD:
				if( p->t_op == SYMBOL && p->t_sc == REGOFF &&
						(rtp=constant(ltp->t_right)) &&
						notpointer(p->t_type) ) {
					p->t_offset =+ rtp->t_value;
					tp = p;
					continue;
				}
				if( indexreg(p) ) {
					if( rtp = constant(ltp->t_right) ) {
	/*
	 * This combines an address register and a constant into a register
	 * offset.  This relies on 68000 Addressing scheme somewhat.
	 */
						p->t_sc = REGOFF;
						p->t_type = tp->t_type;
						p->t_offset =+ rtp->t_value;
						tp = p;
						continue;
					}
					if( !lflag && ltp->t_right->t_op == ADDR ) {
	/*
	 * We can fold *(An+&expr) into *(&expr(An)), but note that &expr
	 * must be 16 bits for 68000, hence we can only do this if short
	 * addresses are enabled.  Note that the storage classes are mapped:
	 * EXTERNAL->EXTOFF, STATIC->STATOFF, REGISTER->REGOFF
	 */
						ltp = ltp->t_right->t_left;
						ltp->t_sc =+ (EXTOFF-EXTERNAL);
						ltp->t_type = tp->t_type;
						tp = ltp;
						continue;
					}
				}
				break;
	/*
	 * --X and X++ can be folded into -(An) and (An)+ 68000 instructions
	 */
			case PREDEC:
			case POSTINC:
				if( indexreg(p) && ltp->t_type == p->t_type ) {
					p->t_op = (ltp->t_op == PREDEC ? AUTODEC : AUTOINC);
					p->t_type = tp->t_type;
					tp = p;
					continue;
				}
			}
			break;
	
		case NOT:
			if( relop(ltp->t_op) ) {	/*!(X>Y)->X<=Y*/
				tp = ltp;
				tp->t_op = invrel[tp->t_op-EQUALS];
				continue;
			}
			break;
	
		case UMINUS:
		case COMPL:
			if( tp->t_type == CHAR )
				tp->t_type = INT;
			if( tp->t_op == ltp->t_op ) {
				tp = ltp->t_left;
				continue;
			}
			if( ltp->t_op == INT2L ) {	/*~(INT2L X)->INT2L (~X)*/
				ltp->t_op = tp->t_op;
				tp->t_op = INT2L;
				ltp->t_type = INT;
				continue;
			}
			break;
	
		case INT2L:
			if( ltp->t_op == MULT ) {	/*INT2L (X*Y)->X*Y*/
				ltp->t_type = (ltp->t_type&(~TYPE))|LONG;
				tp = ltp;
				continue;
			}
			break;
		}
		if(!ccexpr(tpp))
			break;
		tp = *tpp;
	}
	*tpp = tp;
	return(changes);
}

/*
 * commute - commutes expression tree to canonical form
 *		This sorts commutable expression trees so that the most
 *		difficult expression is the left-most operand.  Note that
 *		canon assumes that commute has placed constant operands in
 *		the right sub-tree.  This also swaps relationals so that the
 *		most difficult expression is on the left.
 */
char *commute(tp)					/* returns commuted expression tree*/
struct tnode *tp;
{
	struct tnode *clist[20], *plist[19];
	register struct tnode **p, **q, *s;
	struct tnode **plp, **clp;
	register int op;

	if( relop(op=tp->t_op) ) {
		s = tp->t_left;
		if( harder(tp->t_right,s) ) {
#ifndef	NODEBUG
			if(oflag)
				putexpr("swaprel",tp);
#endif
			tp->t_op = swaprel[op-EQUALS];
			tp->t_left = tp->t_right;
			tp->t_right = s;
		}
	}
	if( commop(op) ) {
#ifndef	NODEBUG
		if(oflag)
			putexpr("commute",tp);
#endif
		clp = clist;
		plp = plist;
		addtree(tp,&clp,&plp);	/*collect comm. expressions*/
	/*
	 * see if any sub-trees can also be commuted (with different operator)
	 */
		clp--;
		plp--;
		for( p = clist; p <= clp; p++ )
			*p = commute(*p);
	/*
	 * this sorts the expressions in decreasing order of Sethy-Ullman
	 * values.
	 */
		for( p = clist; p <= clp; p++ ) {
			for( q = p; q <= clp; q++ ) {
				if( harder(*q,*p) ) {
					s = *q;
					*q = *p;
					*p = s;
				}
			}
		}
	/*
	 * Now, we start at the end of the list and collect any constants
	 * if possible.
	 */
		for( q = clp, p = plp; p > plist; p-- ) {
			s = *p;
			s->t_right = *q;
			s->t_left = *--q;
			if( ccexpr(p) ) {
				clp--;
				plp--;
				*q = *p;	/*[vlh]*/
			}
		}
	/*
	 * this takes the sorted sub-expression pointers and the pointers
	 * to the commutable operator nodes (plist) and structures the
	 * tree so that the left sub-expression is the most complex.  The
	 * code generation scheme is very sensitive to this...
	 */
		q = clist;
		p = plist;
		s = *q++;
		while( p <= plp ) {
			(*p)->t_left = s;
			s = *p++;
			s->t_right = *q++;
			if(!longorptr(s->t_type)) {
				if( longorptr(s->t_right->t_type) )
					s->t_type = s->t_right->t_type;
				else if( longorptr(s->t_left->t_type) )
					s->t_type = s->t_left->t_type;
			}
		}
		tp = s;
#ifndef	NODEBUG
		if(oflag)
			putexpr("after commute",tp);
#endif
	}
	else if( binop(op) ) {
		tp->t_left = commute(tp->t_left);
		tp->t_right = commute(tp->t_right);
	}
	else if( unaryop(op) )
		tp->t_left = commute(tp->t_left);
	return(tp);
}

/*
 * harder - test one sub-expression for being "harder" than another
 *		This requires some special finagling for registers.  The reason
 *		for this is that the code skeletons produce better code if the
 *		register is on the left.  Also note that allowing an address
 *		register on the right can have disastrous effects for AND and OR.
 *		The basic point is: don't mess with this routine unless you're
 *		100% sure you understand the ramifications...
 */
harder(tp,ntp)							/* returns 1 if tp > ntp, else 0*/
struct tnode *tp;
struct tnode *ntp;
{
	if( ntp->t_su == SU_VHARD )
		return(0);
	if( tp->t_su == SU_VHARD )
		return(1);
	if( isreg(ntp) )
		return(0);
	if( isreg(tp) )
		return(1);
	if( constant(ntp) )
		return(1);
	if( constant(tp) )
		return(0);
	return( tp->t_su > ntp->t_su );
}

/*
 * addtree - collect commutable sub-trees for commute
 *		This recurses down the sub-trees looking for groups of
 *		commutable operators.  It collects the sub-trees and their
 *		parent nodes for commute.
 */
addtree(tp,clist,plist)				/* returns pointer to clist*/
struct tnode *tp;					/* pointer to tree*/
struct tnode ***clist;				/* commutable sub-trees*/
struct tnode ***plist;				/* parent nodes of sub-trees*/
{
	register struct tnode ***p, ***c;

	c = clist;
	p = plist;
	if( tp->t_op == tp->t_left->t_op )
		addtree(tp->t_left,c,p);
	else
		*(*c)++ = tp->t_left;
	if( tp->t_op == tp->t_right->t_op )
		addtree(tp->t_right,c,p);
	else
		*(*c)++ = tp->t_right;
	*(*p)++ = tp;
}

/* constant - test for tree being a constant node*/
char *constant(tp)					/* returns 0 or ptr to const node*/
struct tnode *tp;					/* pointer to tree*/
{
	if( tp->t_op == CINT )
		return(tp);
	if((tp->t_op==INT2L || tp->t_op==LONG2I) && tp->t_left->t_op==CINT )
		return(tp->t_left);
	return(0);
}

/* indexreg - returns whether node is an address register*/
/*		For 68000, must be an A register*/
indexreg(tp)						/* returns whether node is A reg*/
struct tnode *tp;					/* pointer to tree*/
{
	if( tp->t_op == SYMBOL && tp->t_sc == REGISTER &&
			!(isdreg(tp->t_reg)) )
		return(1);
	return(0);
}

/* ccexpr - compute constant expression*/
/*		Evaluates constant expressions, including ?: and relationals*/
ccexpr(tpp)							/* returns 1 if changes, 0 otherwise*/
struct tnode **tpp;					/* pointer to tree*/
{
	register struct tnode *ltp, *rtp, *tp;
	register int op, i, j, anylong, anyfloat;	/*[vlh] 3.4 anyfloat*/
	register long rval, lval;

	tp = *tpp;
	op = tp->t_op;
	if( leafop(op) )
		return(0);
	anylong = anyfloat = 0;
	ltp = tp->t_left;
	if( ltp->t_op == CLONG ) {
		lval = ltp->t_lvalue;
		anylong++;
	}
	else if ( ltp->t_op == CFLOAT) {	/* [vlh] 3.4 */
		lval = ltp->t_lvalue;
		anyfloat++;
	}
	else if( ltp = constant(ltp) )
		lval = ltp->t_value;
	else
		return(0);
	if( binop(op) ) {
		rtp = tp->t_right;
		if( op == QMARK ) {
			ltp = rtp->t_left;
			rtp = rtp->t_right;
			if( ltp->t_op != CINT || rtp->t_op != CINT )
				return(0);
			ltp->t_value = (lval?ltp->t_value:rtp->t_value);
			*tpp = ltp;
			return(1);
		}
		if( rtp->t_op == CLONG ) {
			anylong++;
			rval = rtp->t_lvalue;
		}
		else if (rtp->t_op == CFLOAT) {	/* [vlh] 3.4 */
			anyfloat++;
			rval = rtp->t_lvalue;
		}
		else if( rtp = constant(rtp) )
			rval = rtp->t_value;
		else
			return(0);
	}
	i = lval;
	j = rval;
	switch( op ) {

	case ADD:
		lval =+ rval;
		break;

	case SUB:
		lval =- rval;
		break;

	case MULT:
	case DIV:
	case MOD:
	case LSH:
	case RSH:
	case XOR:
		if( anylong || anyfloat )	/* [vlh] 3.4 float */
			return(0);
		switch( op ) {

		case MULT:
			lval = i * j;
			break;

		case DIV:
			lval = i / j;
			break;

		case MOD:
			lval = i % j;
			break;

		case RSH:
			lval = i >> j;
			break;

		case LSH:
			lval = i << j;
			break;

		case XOR:
			lval = i ^ j;
			break;
		}
		break;

	case GREAT:
		lval = (lval>rval);
		break;

	case GREATEQ:
		lval = (lval>=rval);
		break;

	case LESS:
		lval = (lval<rval);
		break;

	case LESSEQ:
		lval = (lval<=rval);
		break;

	case UMINUS:
		lval = -lval;
		break;

	case COMPL:
		lval = ~lval;
		break;

	case NOT:
		lval = !lval;
		break;

	case OR:
		lval =| rval;
		break;

	case AND:
		lval =& rval;
		break;

	default:
		return(0);

	}
	if( anylong )
		ltp = lcnalloc(LONG,lval);
	else if ( anyfloat )
		ltp = fpcnalloc(FLOAT,lval);
	else
		ltp->t_value = lval;
	*tpp = ltp;
	return(1);
}

/* power2 - do multiply and divide by powers of two*/
/*		This changes multiplies and divides by constant powers of two*/
/*		to shifts.*/
power2(tpp)							/* returns 0 if not power of two*/
struct tnode **tpp;					/* pointer to expression tree*/
{
	register char *p;
	register int i, j, op;
	register struct tnode *tp;

	tp = *tpp;
	if( p = constant(tp->t_right) ) {
		if( (i=onebit(p->t_value)) < 0 )
			return(0);
		switch( op = tp->t_op ) {

		case MULT:
			op = LSH;
			break;

		case EQMULT:
			op = EQLSH;
			break;

		case DIV:
			op = RSH;
			break;

		case EQDIV:
			op = EQRSH;
			break;

		case MOD:
			op = AND;
			i = p->t_value - 1;
			break;

		case EQMOD:
			op = EQAND;
			i = p->t_value - 1;
			break;

		default:
			return(0);
		}
		tp->t_op = op;
		p->t_value = i;
		return(1);
	}
	return(0);
}

/* chklong - check for tree being a long*/
chklong(tp)							/* returns 1 if long, 0 otherwise*/
struct tnode *tp;					/* pointer to expression tree*/
{
	if( tp->t_op == INT2L && !(unsign(tp->t_left->t_type)) )
		return(0);
	if( tp->t_op == CLONG )
		return(1);
	return( longorptr(tp->t_type) );
}

/* multop - handle multiplicative operators*/
/*		This checks for powers of two optimizations, then for a hard*/
/*		long operation.*/
multop(tpp)							/* returns ptr to expression tree*/
struct tnode **tpp;					/* pointer to expression tree*/
{
	register struct tnode *ltp, *tp, *rtp, *p;
	register int change;
	register long l;

	tp = *tpp;
	if( change = power2(tpp) )
		tp = *tpp;
	if( chklong(rtp=tp->t_right) || chklong(ltp=tp->t_left) ) {
		switch( tp->t_op ) {

		case MULT:
			tp->t_op = LMULT;
			break;

		case DIV:
		case MOD:
			if( chklong(rtp) ) {		/*only hard if divisor is long*/
				tp->t_op =+ (LMULT-MULT);
				if( rtp->t_op == CLONG )
					rtp->t_op = DCLONG;
			}
			break;

		case EQDIV:
		case EQMOD:
			if( !chklong(rtp) )
				break;
			if( rtp->t_op == CLONG )
				rtp->t_op = DCLONG;
		case EQMULT:
			tp->t_op =+ (LMULT-EQMULT);
			*tpp = tnalloc(ASSIGN,tp->t_type,0,0,tcopy(ltp),tp);
			change++;
			break;
		}
	}
	return(change);
}

/* onebit - returns whether constant is power of two (one bit on)*/
onebit(val)							/* returns bit number or -1*/
int val;							/* constant value to check*/
{
	register int i;

	for( i = 15; val != 0; val =<< 1, i-- )
		if( val & 0100000 )
			break;
	if (val != 0100000)
		return(-1);
	return(i);
}
