/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"
int opdontop;
int strassign;

/*
 * expr - expression evaluator
 *		This handles all the expression syntax in C.  This is a straight-
 *		forward operator-stack/oppri scheme for translating infix into
 *		a binary expression tree.
 */
char *expr()						/* returns 0 or ptr to node*/
{
	register char *p;
	register char *opdsave, *oprsave;
	register int token, op, oppri, i, opdotsave;
	int type, sc;
	long size;

	opdsave = opdp;
	strassign = 0;
	oprsave = opp;
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

	while( (token=gettok()) != EOF ) {
		switch( token ) {
	/*
	 * the following are the terminal nodes of the expresion tree,
	 * note that when we see a terminal node, we push it and then go
	 * and get the next token.  When we see an operator, we need to
	 * check the operator stack to see if we can do a reduction.
	 */
		case CINT:
			if( doopd(cnalloc(INT,cvalue)) ) {
exprerr:
				if( token == SEMI || token == RCURBR )
					pbtok(token);
				error("invalid expression");
				opdp = opdsave;
				opp = oprsave;
				opdontop = opdotsave;
				return(0);
			}
			continue;
	
		case CFLOAT:	/*[vlh] 3.4*/
			if( doopd(fpcnalloc(FLOAT,clvalue)) )
				goto exprerr;
			continue;
	
		case CLONG:
			if( doopd(lcnalloc(LONG,clvalue)) )
				goto exprerr;
			continue;
	
		case SYMBOL:
			if( !((p=csp)->s_attrib&SDEFINED) ) {
				if( peek(LPAREN) ) {	/*assume function call*/
					p->s_sc = EXTERNAL;
					p->s_type = FUNCTION|INT;
				}
				else if( commastop )	/*in initialization?*/
					p->s_sc = EXTERNAL;
				else
					error("undefined symbol: %.8s",p->s_symbol);
				p->s_attrib =| SDEFINED;
			}
			if (p->s_sc==EXTERNAL || function(p->t_type)) {
				if (!reducep || p->s_sc==EXTERNAL || peek(LPAREN)) {
					p = enalloc(p);
					p->t_sc = EXTERNAL;
				}
				else {	/* [vlh] 3.4 if (main).... */
					p = cnalloc(CINT,1);	/* eval function name */
				}
			}
			else
				p=snalloc(p->s_type,p->s_sc,p->s_offset,p->s_dp,p->s_ssp);
			if( doopd(p) )
				goto exprerr;
			continue;
	
		case STRING:
			outtstr(cvalue);
			if( doopd(snalloc(ARRAY|CHAR,STATIC,cvalue,0,0)) )
				goto exprerr;
			continue;
	/*
	 * do special checking for unary ops and operators that can be
	 * either unary or binary operators, such as -, &, *, etc.
	 */
		case RESWORD:
			if( cvalue != R_SIZEOF ) {
				goto exprerr;
			}
			token = SIZEOF;
		case COMPL:
		case NOT:
			if( opdontop )			/*can't have: operand unary-op*/
				goto exprerr;
			break;

		case LBRACK:
			opdontop = 0;
			break;
	
		case RBRACK:
		case RPAREN:
			if( !opdontop )		/*can't be: operator )*/
				goto exprerr;
			break;
	
		case PREINC:
			if( opdontop )			/*assume its lvalue++*/
				token = POSTINC;
			break;

		case PREDEC:
			if( opdontop )			/*assume its lvalue--*/
				token = POSTDEC;
			break;
	
		case SUB:
			if(!opdontop) {	/*if no operand, assume unary*/
				if( peek(CINT) ) {
					cvalue = -cvalue;
					continue;
				}
				if( peek(CLONG) ) {
					clvalue = -clvalue;
					continue;
				}
				if( peek(CFLOAT) ) {	/*[vlh] 3.4*/
					if (!fflag) {		/* IEEE format */
						if (clvalue & 0x80000000)
							clvalue =& 0x7fffffff;
						else
							clvalue =| 0x80000000;
					}
					else			/* FFP format */
						if (clvalue & 0x80)
							clvalue =& 0xffffff7f;
						else
							clvalue =| 0x80;
					continue;
				}
				token = UMINUS;
			}
			break;
	
		case AND:
			if(!opdontop)
				token = ADDR;
			break;

		case MULT:
			if( !opdontop )
				token = INDR;
			break;
	/*
	 * for left parenthesis, we need to see if this is a casting operator.
	 */

		case LPAREN:
			if( !opdontop ) {	/*see if casting or abstract declarator*/
				sc = type = 0;
				if( gettype(&sc,&type,&size) ) {
					sc = (type == STRUCT) ? dalloc(size) : cdp;
					p = snalloc(type,STATIC,0,sc,sc);
					p->t_type =| declarator(1);
					if( !next(RPAREN) )
						goto exprerr;
					if( tdp )
						p->t_type=addtdtype(tdp,p->t_type,p->t_dp,
							&(p->t_ssp));
					pushopd(p);
					token = CAST;
					if( opp->o_op == SIZEOF ) {
						opdontop++;
						continue;
					}
				}
			}
			else  /*we've seen (), look for NACALL*/
				token = (next(RPAREN)) ? MPARENS : CALL;
			break;
	
		case PERIOD:
		case APTR:
			smember++;			/*next token needs to be struct membr*/
			strucptr[smember+instruct] = csp->s_struc;
			break;
	
		}
	/*
	 * we have seen an operator, get its info and then check the operator
	 * stack.
	 */
		if( binop(token) ) {
	/*
	 * handle special binary operators, such as CAST and post-inc and
	 * post-dec.
	 */
			if(!opdontop) {
				if( token != PREDEC && token != PREINC && token != CAST )
					goto exprerr;
			}
			if( token != POSTDEC && token != POSTINC && token != MPARENS )
				opdontop = 0;				/*this fixes x++ op problem.*/
		}
		oppri = oppriority(token);
		if( commastop && token == COMMA )	/*need to stop at comma(init)*/
			oppri = COLPRI;
		if( colonstop && token == COLON )	/*need to stop at colon(case)*/
			oppri = COLPRI;
		while( 1 ) {
			if( oppri > opp->o_pri ||
					(oppri == opp->o_pri && rasop(token)) ) {
	/*
	 * we have encountered a higher priority (or right-associative)
	 * operator, hence we need to stack it.
	 */
				if( ++opp >= &opstack[OPSSIZE] )
					ferror("expression too complex");
				if( token == POSTINC || token == POSTDEC )
					oppri = PSTPRI;
				else if( token == LPAREN || token == LBRACK ||
						token == CALL )
					oppri = CALPRI;
				opp->o_op = token;
				opp->o_pri = oppri;
				break;
			}
	/*
	 * we have encountered a lower priority operator, hence we need to
	 * do a reduction.
	 */
			op = opp->o_op;
			opp--;
			type = CINT;
			switch( op ) {
		
			case STACKEND:			/*we accept the expression...*/
				pbtok(token);
				if (!strassign) {
					if (!maketree(0))
						goto exprerr;
				}
				if (!(p=popopd()))
					goto exprerr;
				opdp = opdsave;
				opp = oprsave;
				opdontop = opdotsave;
				return(p);
		
			case LPAREN:			/*assure these have matching )*/
			case CALL:
				if( token != RPAREN )
					goto exprerr;
				break;

			case MPARENS:
				if( !maketree(NACALL) )
					goto exprerr;
				continue;
		
			case LBRACK:
				if( token != RBRACK )
					goto exprerr;
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
			}
			if( op != LPAREN && !maketree(op) )
				goto exprerr;
			break;
		}
	}
	error("unexpected EOF");
	return(0);
}

/* doopd - handle constant or symbol node operand*/
/*		Pushes node onto operand stack and handles opdontop flag.*/
doopd(tp)						/* returns 1 if syntax error, 0 for ok*/
struct tnode *tp;				/* pointer to tree node*/
{
	pushopd(tp);
	if( opdontop )
		return(1);
	opdontop++;
	return(0);
}

	/*
	 * This indicates if a conversion operator is needed given the types
	 * of the two operands.  The upper diagonal is applied ONLY if this
	 * is an assignment operator and the indexes are swapped so the lower
	 * diagonal is used otherwise.
	 *INT		UNSN		LONG		DOUB		PTR
	 */
char cvmap[5][5] {
	0,			UNSN_INT,	LONG_INT,	DOUB_INT,	PTR_INT,
	INT_UNSN,	0,			LONG_UNSN,	DOUB_UNSN,	PTR_UNSN,
	INT_LONG,	UNSN_LONG,	0,			DOUB_LONG,	PTR_LONG,
	INT_DOUB,	UNSN_DOUB,	LONG_DOUB,	0,			BADCONV,
	INT_PTR,	UNSN_PTR,	LONG_PTR,	BADCONV,	PTR_PTR,
};

/*
 * maketree - given operator, takes operands and builds up tree
 *		This takes the given operator, allocates a node for it
 *		and links up the subtrees on the operand stack.  A lot of
 *		work is in inserting the proper conversions.
 */
maketree(op)						/* returns success or failure */
int op;								/* new root operator*/
{
	register struct tnode *ltp, *rtp, *p;
	register int type, ltype, rtype, lconv, conv, pconv, ilen, docast;

#ifdef DEBUG
	printf("maketree op = %d\n",op);
#endif
	if( binop(op) ) {
		if(!(rtp=popopd()))
			return(0);
		rtype = (rtp=funcref(arrayref(rtp)))->t_type;
#ifdef DEBUG
	putexpr("maketree r",rtp);
#endif
	}
	if(!(ltp=popopd()))
		return(0);
#ifdef DEBUG
	putexpr("maketree l",ltp);
#endif
	if( op == SIZEOF ) {	/* [vlh] 3.4 ilen */
		ilen = dsize(ltp->t_type,ltp->t_dp,ltp->t_ssp);
		pushopd(cnalloc(INT,ilen));
		return(1);
	}
	if( op != ADDR ) {
		ltp = arrayref(ltp);
		if( op != CALL && op != NACALL )
			ltp = funcref(ltp);
	}
	else {	/* [vlh] 4.0 &p->array */
		if (ltp->t_op == ADD)	/* there must be a better way... */
			if (ltp->t_left->t_op==SYMBOL && ltp->t_right->t_op==CINT) {
				pushopd(ltp);
				return(1);
			}
	}
	if( specops(op,ltp,rtp) )
		return( 1 );
	ltype = ltp->t_type;
	if( lintegral(op) )
		integral(ltp,LONG);
	if( rintegral(op) )
		integral(rtp,LONG);
	if( lvalop(op) && ltp->t_op != SYMBOL && ltp->t_op != INDR &&
			ltp->t_op != BFIELD )
		error("assignable operand required");
	if( unaryop(op) ) {
		if(!unopeval(op,ltp))
			pushopd(tnalloc(op,ltype,ltp->t_dp,ltp->t_ssp,ltp,0L));
		return;
	}
	if( ltype == STRUCT || rtype == STRUCT ) {
		if (ltype==STRUCT && rtype==STRUCT && op==ASSIGN) { /*[vlh]*/
			pushopd(ltp);
			maketree(ADDR);
			pushopd(rtp);
			maketree(ADDR);
			maketree(STASSIGN);
			strassign = 1;
			return(1);
		}
		ltype = rtype = INT;
		error("illegal structure operation");
	}
	type = ltype;
	if( rtype == TYPELESS ) {
		rtp->t_type = rtype = INT;
		lconv = conv = 0;
	}
	else {
		lconv = ttoconv(ltype);
		conv = ttoconv(rtype);
		if (!asgop(op) && conv > lconv) {
			conv = cvmap[conv][lconv];
			lconv = 1;
			type = rtype;
		}
		else {
			conv = cvmap[lconv][conv];
			lconv = 0;
		}
	}
	if( asgop(op) ) {
		if( (op == ASSIGN || op == FRETURN) && (conv == INT_PTR ||
				conv == UNSN_PTR) && rtp->t_op != CINT )
			error("short assigned to pointer");
		if( op == ASSIGN || op == CAST ) {
			switch( conv ) {

			case INT_PTR:
			case UNSN_PTR:
			case PTR_PTR:
			case PTR_LONG:
			case LONG_PTR:
				conv = 0;
				break;
			}
		}
	}
	else if( op == COLON && suptype(ltype) != 0 && ltype == rtype )
		conv = 0;
	else if( relop(op) && conv == PTR_PTR )
		conv = 0;
	pconv = 0;
	if ((relop(op) || op==FRETURN) && alltype(ltype) == (STRUCT | POINTER)
				&& (conv == INT_PTR || conv == LONG_PTR))	/* [vlh] 3.4 */
		conv = 0;	/* int compare to struct pointer, no conversion */
	if( conv == PTR_PTR ) {
		conv = 0;
		if( op == SUB ) {
			type = LONG;
			pconv++;
		}
		else if(op != FRETURN && ( (alltype(ltype) != alltype(rtype) ||
					alltype(ltype) != (POINTER|CHAR)) ) )
			conv = BADCONV;
	}
	docast=(ltp->t_type==CHAR && rtp->t_sc!=REGISTER && rtp->t_sc<=STATIC);
	if( conv ) {
		if( conv == BADCONV )
			error("illegal type conversion");
		else if( lconv )
			ltp = cvopgen(ltp,type,conv,psize(rtp),op);
		else
			rtp = cvopgen(rtp,type,conv,psize(ltp),op);
	}
	if( op == CAST ) {	/* [vlh] 4.0 */
		if ( docast )	/* predefined to handle conv/cast ops */
			rtp = tnalloc(TOCHAR,CHAR,0,0,rtp,0L);
		else if ( rtp->t_type != CHAR && !conv ) {
			rtp->t_type = ltp->t_type;
			rtp->t_ssp = ltp->t_ssp;		/* [vlh] 3.4 */
			rtp->t_dp = ltp->t_dp;			/* [vlh] 3.4 */
		}
		pushopd(rtp);
	}
	if( relop(op) )
		type = INT;
	if(op != CAST && !binopeval(op,ltp,rtp)) {
		if( btype(ltype) == STRUCT || btype(rtype) != STRUCT )
			p = ltp;
		else
			p = rtp;
		pushopd(tnalloc(op,type,p->t_dp,p->t_ssp,ltp,rtp));
	}
	if( pconv && ltype != (POINTER|CHAR) ) {
		if(!(ltp=popopd()))
			return(0);
		pushopd(cvopgen(ltp,LONG,PTR_LONG,psize(ltp->t_left),op));
	}
	return(1);
}

/* specops - handle special operators in building tree*/
specops(op,ltp,rtp)				/* returns 1 if op special, 0 otherwise*/
int op;							/* operator*/
struct tnode *ltp;				/* left subtree pointer*/
struct tnode *rtp;				/* right subtree pointer*/
{
	register int type, sr_size, sl_size;

	type = ltp->t_type;
#ifdef DEBUG
	putexpr("specops l",ltp);
	putexpr("specops r",rtp);
#endif
	switch (op) {

	case 0:
		break;

	default:
		return(0);

	case APTR:							/*expr -> name*/
		integral(ltp,LONG);				/*we need to turn expr into a*/
		ltp->t_type = POINTER|STRUCT;	/*pointer to a struct, then use*/
		pushopd(ltp);					/*expr . name stuff*/
		maketree(INDR);
		ltp = popopd();					/*ltp cannot be 0*/
	case PERIOD:						/*expr . name*/
		if( !(isstel(rtp)) )
			error("invalid structure member name");
		type = rtp->t_type;
		if( array(type) ) {
			type = delspchk(type);
			rtp->t_dp++;
		}
		tadjust(ltp,type,rtp->t_dp,rtp->t_ssp);
		pushopd(ltp);
		maketree(ADDR);
		pushopd(cnalloc(TYPELESS,rtp->t_offset));
		maketree(ADD);
		if( notarray(rtp->t_type) )
			maketree(INDR);
		ltp = popopd();
		if( rtp->t_sc == BFIELDCL )		/*ltp cannot be 0*/
			ltp = tnalloc(BFIELD,type,rtp->t_dp,rtp->t_ssp,ltp,0L);
		break;

	case QMARK:
		if( rtp->t_op != COLON )
			error("invalid ?: operator syntax");
		if( ltp->t_op == CINT && rtp->t_left->t_op == CINT &&
				rtp->t_right->t_op == CINT )
			ltp->t_value = (ltp->t_value ? rtp->t_left->t_value :
					rtp->t_right->t_value);
		else
			ltp = tnalloc(op,rtp->t_type,0,0,ltp,rtp);
		break;

	case LAND:
	case LOR:
	case COMMA:							/*don't need conversions here*/
		ltp = tnalloc(op,INT,0,0,ltp,rtp);
		break;

	case INDR:
		if( ltp->t_op == ADDR  )		/**& is null op*/
			ltp = ltp->t_left;
		else {
			if( function(type) )
				error("indirection on function invalid");
			ltp = tnalloc(INDR,delspchk(type),ltp->t_dp,ltp->t_ssp,ltp,0L);
		}
		break;

	case STASSIGN:	/*[vlh]*/
		sr_size = psize(rtp);
		sl_size = psize(ltp);
		if( sl_size >= sr_size )
			ltp = tnalloc(STASSIGN,sr_size,0,0,ltp,rtp);
		else
			error("invalid structure assignment");
		break;

	case NACALL:
	case CALL:
		if( notfunction(type) )
			error("illegal call");
		ltp = tnalloc(op,delspchk(type),ltp->t_dp,ltp->t_ssp,ltp,rtp);
		break;

	case ADDR:
		if( ltp->t_op == INDR ) {		/*&* is null op*/
/*			ltp->t_left->t_type = addsp(type,POINTER);*/
/*			ltp->t_left->t_dp = ltp->t_dp;*/
/*			ltp->t_left->t_ssp = ltp->t_ssp;*/
			ltp = ltp->t_left;
		}
		else if( ltp->t_op == SYMBOL ) {
			if( ltp->t_sc == REGISTER )
				error("address of register");
			ltp = tnalloc(ADDR,addsp(type,POINTER),ltp->t_dp,ltp->t_ssp,
					ltp,0L);
		}
		else
			error("& operator illegal");
		break;
	}
	pushopd(ltp);
	return(1);
}

/* cvopgen - generate a conversion operator*/
/*		Generates conversions necessary for integers, pointers and longs.*/
char *cvopgen(tp,type,conv,len,op)	/* returns pointer to conv node*/
struct tnode *tp;					/* pointer to node to do conversion*/
int type;							/* type to convert to*/
int conv;							/* specified conversion*/
long len;							/* object length [vlh] 3.4 i=>l */
int op;								/* for cast operator*/
{
	register struct tnode *rtp;
	register int cop, ilen;

	switch(conv) {

	case INT_PTR:
	case UNSN_PTR:
		if (type == (POINTER|CHAR) && op == CAST ) {	/* [vlh] 4.0 */
			cop = INT2L;
			break;
		}
		else
			if( op == CAST || op == FRETURN ) {
				cop = INT2L;		/*of the ptd to objects length plus*/
				if( len != 1L ) {	/*an integer to long covnversion*/
					ilen = len;
					rtp = cnalloc(INT,ilen);
					tp = tnalloc(MULT,type,0,0,tp,rtp);
				}
				break;
			}
	case PTR_LONG:						/*need to generate mult or div*/
	case LONG_PTR:						/*of the ptd to objects length*/
		if( len == 1 )
			return(tp);
		ilen = len;
		cop = (conv == PTR_LONG ? DIV : MULT);
		rtp = cnalloc(INT,ilen);
		break;

	case INT_LONG:
	case UNSN_LONG:
		cop = INT2L;
		break;

	case INT_DOUB:	/*[vlh] 3.4*/
	case UNSN_DOUB:
		cop = INT2F;
		break;

	case LONG_DOUB:	/*[vlh] 3.4*/
		cop = LONG2F;
		break;

	case DOUB_LONG:	/*[vlh] 3.4*/
		cop = FLOAT2L;
		break;

	case DOUB_INT:	/*[vlh] 3.4*/
	case DOUB_UNSN:
		cop = FLOAT2I;
		break;

	case LONG_INT:
	case LONG_UNSN:
		cop = LONG2I;
		break;

	default:
		error("invalid conversion");
		return(tp);
	}
	return( tnalloc(cop,type,0,0,tp,rtp) );
}

/* tadjust - expression tree type adjustment*/
/*		Adjusts the types of subtrees to agree with the top of the tree.*/
tadjust(tp,type,dp,ssp)				/* returns - none*/
struct tnode *tp;					/* pointer to tree*/
int type;							/* type to adjust to*/
int dp;								/* dimension pointer or info*/
int ssp;							/* structure pointer*/
{
	register int op;

	tp->t_type = type;
	if( dp >= 0 ) {
		tp->t_dp = dp;
		tp->t_ssp = ssp;
	}
	if( (op=tp->t_op) == ADDR )
		type = delspchk(type);
	else if( op == INDR )
		type = addsp(type,POINTER);
	else if( op != ADD && op != SUB )
		return;
	tadjust(tp->t_left,type,dp,ssp);
}

/* funcref - handle tree function reference*/
/*		Turns a reference to a function into the address of the function.*/
char *funcref(tp)					/* returns pointer to node*/
struct tnode *tp;					/* pointer to old node*/
{
	if( function(tp->t_type) )
		tp = tnalloc(ADDR,addsp(tp->t_type,POINTER),tp->t_dp,
			tp->t_ssp,tp,0L);
	return(tp);
}

/* arrayref - handle tree array reference*/
/*		Turns a reference to an array into the address of the array.*/
char *arrayref(tp)					/* returns pointer to tree node*/
struct tnode *tp;					/* tree node pointer*/
{
	if( array(tp->t_type) && !(isstel(tp)) ) {
		tp->t_dp++;
		pushopd(tp);
		tadjust(tp,delspchk(tp->t_type),-1,0);
		maketree(ADDR);
		tp = popopd();				/*tp cannot be 0*/
	}
	return(tp);
}

/* integral - checks operand for integral type*/
/*		This checks for needing an integral operand.*/
integral(tp,atype)					/* returns - none*/
struct tnode *tp;					/* pointer to tree node*/
int atype;							/* alternate type allowable*/
{
	register int type;

	if( (type=tp->t_type) != INT && type != UNSIGNED && type != CHAR &&
			!suptype(type) && type != atype )
		error("invalid operand type");
}

/* ttoconv - maps normal type into conversion table type*/
ttoconv(type)						/* returns conversion type*/
int type;							/* type to convert*/
{
	switch(type) {

		case CHAR:
		case INT:
			return(0);

		case UNSIGNED:
			return(1);

		case LONG:
			return(2);

		case FLOAT:
		case DOUBLE:
			return(3);

		default:
			return(4);
	}
}

/* binopeval - does binary operator constant expression evaluation*/
/*		Does the constant expression evaluation for binary operators.*/
binopeval(op,ltp,rtp)				/* returns 1 if done, 0 if not*/
int op;								/* operator to evaluate*/
struct tnode *ltp;					/* pointer to left subtree*/
struct tnode *rtp;					/* pointer to right subtree*/
{
	register int lvalue, rvalue;

	if( ltp->t_op != CINT )
		return(0);
	lvalue = ltp->t_value;
	if( rtp->t_op != CINT )
		return(0);
	rvalue = rtp->t_value;
	switch (op) {

	case ADD:
		lvalue =+ rvalue;
		break;

	case SUB:
		lvalue =- rvalue;
		break;

	case MULT:
		lvalue =* rvalue;
		break;

	case DIV:
		lvalue =/ rvalue;
		break;

	case MOD:
		lvalue =% rvalue;
		break;

	case AND:
		lvalue =& rvalue;
		break;

	case OR:
		lvalue =| rvalue;
		break;

	case XOR:
		lvalue =^ rvalue;
		break;

	case LSH:
		lvalue =<< rvalue;
		break;

	case RSH:
		lvalue =>> rvalue;
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
	ltp->t_value = lvalue;
	pushopd(ltp);
	return(1);
}

/* unopeval - unary operator constant expression evaluation*/
/*		Does constant expression evaluation for unary operators.*/
unopeval(op,tp)						/* returns 1 if done, 0 otherwise*/
int op;								/* operator to evaluate*/
struct tnode *tp;					/* pointer to subexpression*/
{
	register int value;

	if( tp->t_op != CINT )
		return(0);
	value = tp->t_value;
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
	tp->t_value = value;
	pushopd(tp);
	return(1);
}

/* cexpr - evaluate a constant integer expression*/
/*		Used in evaluating array bounds, bit field numbers, etc.*/
cexpr()
{
	register struct tnode *tp;
	register char *savep;

	savep = exprp;
	exprp = opap;
	commastop++;
	if( (tp=expr()) && tp->t_op != CINT )
		error("constant required");
	commastop--;
	exprp = savep;
	return( tp->t_value );
}

