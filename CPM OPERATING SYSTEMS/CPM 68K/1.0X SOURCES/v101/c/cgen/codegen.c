#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include "cgen.h"
#include "cskel.h"
#define	SK_TYPE(x)	(x&017)


/* scodegen - over-all code generation for expression*/
/*		Picks off post-fix ++, --.*/
scodegen(tp,cookie,reg)				/* returns register result is in*/
struct tnode *tp;
int cookie;
int reg;
{
	struct tnode *clist[20];
	struct tnode **clp;
	register struct tnode **cp;
	register int r;
	register int ccflag;
	register struct tnode *rtp;

	ccflag = 0;
	clp = clist;
	tp = addptree(tp,&clp);
	if( clp > clist ) {

	/*post ++, -- in tree.  We need to compile the tree sans post operators*/
	/*then generate code to do the post operators, then do any fix up of*/
	/*condition codes since the Stupid 68000 architect was a nimnul.*/

		if( cookie == FORCC ) {

	/*here we make the observation that if we are comparing something with*/
	/*zero OR the top operator of the tree is not a comparison operator,*/
	/*we can compile the tree to a register, and then set the condition*/
	/*codes OK with a tst instruction at the end.*/

			if( relop(tp->t_op) ) {
				if( (rtp=constant(tp->t_right)) && rtp->t_value == 0 ) {
					ccflag = 1;
					tp = tp->t_left;
					cookie = FORREG;
				}
				else
					ccflag = 2;
			}
			else {
				ccflag = 1;
				cookie = FORREG;
			}
		}
	}
	r = codegen(tp,cookie,reg);
	if( clp > clist ) {
		if( ccflag == 2 )
			outccsave(r);
		for( cp = clist; cp < clp; cp++ )
			codegen(*cp,FOREFF,r+1);
		if( ccflag == 1 )
			outcmp0(r,tp);
		else if( ccflag == 2 )
			outccrestore(r);
	}
	return(r);
}

/* addptree - prune off postfix ++, -- from expression tree*/
/*		This prunes off ++, -- and collects those expressions for*/
/*		scodegen.*/
char *addptree(tp,clp)				/* returns pointer to pruned tree*/
struct tnode *tp;
struct tnode ***clp;
{
	register int op;

	op = tp->t_op;
	if( leafop(op) )
		return(tp);
	if( op == POSTINC || op == POSTDEC ) {
		*(*clp)++ = tp;
		return( tcopy(tp->t_left) );
	}
	if( binop(op) )
		tp->t_right = addptree(tp->t_right,clp);
	tp->t_left = addptree(tp->t_left,clp);
	return(tp);
}

/* codegen - generate code for expression*/
/*		This calls up rcodegen, which prunes off any special register*/
/*		optimization code, then calls ucodegen (unrecursive) code*/
/*		generation.*/
codegen(tp,cookie,reg)				/* returns reg result is in*/
struct tnode *tp;					/* tree pointer*/
int cookie;							/* code generation goals*/
int reg;							/* first available register*/
{
	register int size, savestk, ssize, r, i, scookie;
	register struct tnode *rtp;

#ifndef NODEBUG
	if( cflag )
		printf("codegen op=%d cookie=%d reg=%d\n",tp->t_op,cookie,reg);
#endif
	switch( tp->t_op ) {

	case CALL:
	case NACALL:
		ssize = 0;
		savestk = stacksize;
		if( tp->t_left->t_op != SYMBOL )
			stacksize++;
		if( tp->t_op == CALL ) {
			rtp = tp->t_right;
			while( rtp->t_op == COMMA ) {
				ssize =+ dofarg(rtp->t_right);
				rtp = rtp->t_left;
			}
			ssize =+ dofarg(rtp);
		}
		tp->t_op = FJSR;				/*generate JSR (unary op)*/
		codegen(tp,FORREG,reg);
		popstack(ssize);
		stacksize = savestk;
		fixresult(tp,cookie,0);
		return(0);						/*result in R0*/

	case COMMA:
		codegen(tp->t_left,FOREFF,reg);
		return(codegen(tp->t_right,cookie,reg));

	case AND:
		if( cookie == FORCC && (i=isonebit(tp->t_right)) >= 0 &&
				(i=dobitadd(tp->t_left,i)) >= 0 ) {
			if( convop(tp->t_right->t_op) )
				tp->t_right = tp->t_right->t_left;
			tp->t_right->t_value = i;
			tp->t_op = BTST;
			tp = canon(tp);
			sucomp(tp,reg,1);
		}
		break;
	}
	if( rcodegen(&tp,cookie,reg) ) {
		if( cookie == FORCC && tp->t_op == SYMBOL ) {
			if( indexreg(tp) )
				outcmp0(tp->t_reg,tp);
			return(reg);
		}
	}
	r = ucodegen(tp,cookie,reg);
	return(r);
}

/* fixresult - fix result of code generation*/
fixresult(tp,cookie,reg)	/* returns - none*/
struct tnode *tp;
int cookie;							/* wanted this cookie*/
int reg;
{
	switch( cookie ) {

	case FORCC:
		outcmp0(reg,tp);
		break;

	case FORSP:
	case FORSTACK:
		stacksize++;
		outrpush(reg,tp,cookie==FORSTACK);
		break;

	}
	return(reg);
}

/* ucodegen - generate code for tree given cookie and starting register*/
/*		Handles the matching of the expression tree node with the*/
/*		corresponding code generation table.  When a match is found,*/
/*		expand is called to expand the code skeleton macro.*/
ucodegen(tp,cookie,reg)				/* returns reg result is in*/
struct tnode *tp;					/* pointer to expression tree*/
int cookie;							/* (FORCC,FOREFF,FORREG,FORSTACK)*/
int reg;							/* first available register*/
{
	register int r;
	register char *p;

#ifndef NODEBUG
	if(cflag)
		putexpr("ucodegen",tp);
#endif
	switch( tp->t_op ) {

	case SYMBOL:
		if( cookie == FOREFF )
			return(reg);
		break;

	case LSH:
		if( isareg(reg) && (p=constant(tp->t_right)) &&
				unsign(tp->t_left->t_type) == 0 &&
				(p->t_value == 1 || p->t_value == 2) ) {
			r = codegen(tp->t_left,FORREG,reg);
			outmovr(r,reg,tp->t_left);
			if( p->t_value == 2 )
				outaddr(reg,reg,tp);
			outaddr(reg,reg,tp);
			return(reg);
		}
		break;

	case EQMULT:
	case EQDIV:
	case LEQMULT:
	case LEQDIV:
	case EQMOD:
	case LEQMOD:
	case EQRSH:
	case EQLSH:
	case EQAND:
	case EQOR:
	case EQXOR:
		if( indexreg(tp->t_left) ) {
			reg = dreg(reg);
			outmovr(r=tp->t_left->t_reg,reg,tp);
			tp->t_left->t_reg = reg;
			codegen(tp,cookie,reg+1);
			outmovr(reg,r,tp);
			return(reg);
		}
		break;

	case ADD:
	case EQADD:
		if( (p=constant(tp->t_right)) && p->t_value < 0 &&
				p->t_value >= -QUICKVAL ) {
			p->t_value = - p->t_value;
			tp->t_op =+ (SUB-ADD);
		}
		break;
	}
	sucomp(tp,reg,1);
	if( (r=loadexpr(tp,cookie,reg)) >= 0 )
		return(r);
	if( (r=cqmark(tp,cookie,reg)) >= 0 )
		return(r);
	if( (r=hardrel(tp,cookie,reg)) >= 0 )
		return(r);
	if( cookie == FORCC && (p=match(tp,FOREFF,reg)) != 0 ) {
		r = expand(tp,FOREFF,reg,p);
		if( asgop(tp->t_op) && indexreg(tp->t_left) )
			outcmp0(tp->t_left->t_reg,tp->t_left);
	}
	else if( p = match(tp,cookie,reg) )
		r = expand(tp,cookie,reg,p);
	else if( cookie != FORREG )
		r = fixresult(tp,cookie,ucodegen(tp,FORREG,reg));
	else
		error("no code table for %d",tp->t_op);
	return(r);
}

/* expand - code skeleton expansion*/
/*		Handles the expansion of code skeleton macros.*/
expand(tp,cookie,freg,skp)			/* returns register result is in*/
struct tnode *tp;					/* pointer to expression tree*/
int cookie;							/* goal of expression tree*/
int freg;							/* register to leave results in*/
struct skeleton *skp;				/* pointer to code skeleton*/
{
	register int op, nreg, reg;
	register int c;
	register int extf, i2f;
	register struct tnode *ltp, *rtp;
	register char *p;
	register int i, inaddreg, sreg, flag, subtrees, scookie;
	register char *macro;

#ifndef NODEBUG
	if( eflag )
		printf("expand op=%d left=%x right=%x skp=%o\n",tp->t_op,
				skp->sk_left,skp->sk_right,skp);
#endif
	if( ((op=tp->t_op) >= MULT && op <= XOR) || tp->t_type == CHAR )
		freg = dreg(freg);
	macro = skp->sk_def;
	extf = 0;
	i2f = 0;
	rtp = ltp = tp->t_left;
	subtrees = 1;
	if( binop(op) ) {
		subtrees++;
		rtp = tp->t_right;
		if( longorptr(tp->t_type) && (op == DIV || op == MOD ||
				(op != MULT && isdreg(freg) &&
				longorptr(ltp->t_type)==0 && longorptr(rtp->t_type)==0)) )
			extf++;
		switch( op ) {

		case RSH:
		case LSH:
		case EQLSH:
		case EQRSH:
			if( unsign(ltp->t_type) )
				i2f++;
			break;

		case MULT:
		case EQMULT:
		case DIV:
		case MOD:
		case EQDIV:
		case EQMOD:
			if( unsign(ltp->t_type) || unsign(rtp->t_type) )
				i2f++;
			break;
		}
	}
	nreg = freg + 1;
	while( c = *macro++ ) {
		c =& 0xff;
		switch( c ) {

		default:
			putchar(c);
			break;

		case POP:
			stacksize--;
			printf("(sp)+");
			break;

		case POP8:
			stacksize =- 2;
			popstack(8);
			break;

		case PSH:
			if( cookie == FORSP )				/*don't affect sp*/
				printf("(sp)");
			else
				printf("-(sp)");
			stacksize++;
			break;

		case MOV:
		case MOVL:
		case JSR:
		case CLR:
		case CLRL:
		case EXTW:
		case EXTL:
		case LEA:
		case STK:
			printf("%s",strtab[c-128]);
			break;

		case TLEFT:
			outtype( leafop(op) ? tp->t_type : ltp->t_type );
			break;

		case TLEFTL:
			outatype( leafop(op) ? tp->t_type : ltp->t_type );
			break;

		case TEITHER:
			if( longorptr(rtp->t_type) || longorptr(ltp->t_type) )
				outtype(LONG);
			break;

		case TRIGHT:
			outtype(rtp->t_type);
			break;

		case OP:
		case AOP:
			if( c == AOP || i2f )
				i = optab[op][1];
			else
				i = optab[op][0];
			printf(mnemonics[i]);
			break;

		case LADDR:
		case RADDR:
			p = (c==RADDR?rtp:ltp);
			outaexpr(p,IMMED);
			break;

		case CR:
			outcreg(freg);
			break;

		case NR:
			outcreg(nreg);
			break;

		case CAR:
			outcreg(areg(freg));
			break;

		case NAR:
			outcreg(areg(nreg));
			break;

		case EXL:
			outextend(ltp,LONG,freg);
			break;

		case EXRL:
		case EXRLN:
			outextend(rtp,ltp->t_type,c==EXRL?freg:nreg);
			break;

		case EXLR:
		case EXLRN:
			outextend(ltp,rtp->t_type,c==EXLR?freg:nreg);
			break;

		case LEFT:
		case RIGHT:
			subtrees--;
		case TREE:
			p = (c==LEFT?ltp:c==RIGHT?rtp:tp);
			flag = *macro++;
			scookie = FORREG;
			if( flag & S_STACK ) {
				if( cookie == FORSP )
					scookie = FORSP;
				else
					scookie = FORSTACK;
			}
			else if( flag & S_FORCC )
				scookie = FORCC;
			if( flag & S_NEXT )
				reg = nreg;
			else
				reg = freg;
			if( flag & S_INDR ) {
				if( p->t_op != INDR )
					error("code skeleton error: %d\n",op);
				p = p->t_left;		/*skip INDR*/
				i = 0;
				if( coffset(p) ) {
					p = p->t_left;
					i++;		/*need in An*/
				}
				if( i || p->t_op == SYMBOL || p->t_op == CINT ) {

	/*we need address of "* expr" in an address register.  If this is a*/
	/*leaf node or the type of expression is not character, it is ok to*/
	/*try to compile directly to the A register.  Otherwise, we may as*/
	/*well compile to D register anyway.  This is fixed up immediately*/
	/*after codegen below...*/

					if( leafop(p->t_op) || p->t_type != CHAR )
						reg = areg(reg);
				}
			}
			sreg = codegen(p,scookie,reg);	/*code for subtree*/
			if( scookie == FORREG ) {
				if( flag & S_INDR ) {
					if( isdreg(sreg) )
						outmovr(sreg,areg(reg),p);
				}
				else if( flag & S_NEXT )
					nreg = sreg;
				else if( sreg != reg ) {

	/*result was not in expected register, if remaining sub-tree can be*/
	/*compiled using the remaining registers, update current and next*/
	/*registers, saving us the trouble of moving the register.*/

					if( c == TREE || (isdreg(sreg) && subtrees > 0 &&
						((c == LEFT &&
							sucomp(rtp,sreg,0) <= skp->sk_right &&
							sucomp(rtp,sreg,1) <= SU_ANY) ||
						( c == RIGHT &&
							sucomp(ltp,sreg,0) <= skp->sk_left &&
							sucomp(ltp,sreg,1) <= SU_ANY))) ) {
						freg = dreg(sreg);
						nreg = freg + 1;
					}
					else
						outmovr(sreg,dreg(freg),p);
				}
			}
			break;

		case LOFFSET:
		case ROFFSET:
			p = (c==LOFFSET) ? ltp->t_left : rtp->t_left;
			if((p=coffset(p)) != 0 && (p->t_op != CINT || p->t_value != 0))
				outaexpr(p,NOTIMMED);
			break;

		case MODSWAP:
			switch( op ) {

			case MOD:
			case EQMOD:
			case LMOD:
			case LEQMOD:
				outswap(freg);
			}
			break;

		}
	}
	if( extf && cookie == FORREG && isdreg(freg) ) {
		if( unsign(ltp->t_type) || unsign(rtp->t_type) )
			outuext(freg);
		else
			outext(freg);
	}
#ifndef NODEBUG
	if( eflag )
		printf("ending expand skp=%o\n",skp);
#endif
	return(freg);
}

/* match - try to match expression tree with code skeleton*/
/*		Given the expression tree, tries to match the given tree with*/
/*		the appropriate code skeleton.  The code skeleton list is*/
/*		gotten from the root operator and the cookie value.  The code*/
/*		skeleton list is then searched, checking the Sethy-Ullman numbers*/
/*		of the sub-trees against the Sethy-Ullman numbers in the code*/
/*		skeleton list.  If the Sethy-Ullman numbers are OK, then the*/
/*		left and right sub-trees are checked for compatability, e.g.*/
/*		integer pointers, etc.  If a match is found, the code skeleton*/
/*		list pointer is returned.*/
char *match(tp,cookie,reg)				/* returns pointer to code skeleton*/
										/* or 0 if no skeleton*/
struct tnode *tp;						/* pointer to tree*/
int cookie;								/* goal for code expansion*/
int reg;								/* register to use*/
{
	register struct skeleton *skp;
	register int op, bop, i, opndx;
	register struct tnode *ltp, *rtp;

#ifndef NODEBUG
	if( mflag )
		printf("match op=%d cookie=%d reg=%d\n",tp->t_op,cookie,reg);
#endif
	if( (op=tp->t_op) >= LCGENOP )
		return(0);
	if( leafop(op) )
		ltp = tp;
	else
		ltp = tp->t_left;
	if( (bop=binop(op)) ) {
		rtp = tp->t_right;
		if( convop(ltp->t_op) ) {
			if( op != LSH && notconvop(rtp->t_op) ) {
				tp->t_left = ltp->t_left;
				if( (unsign(ltp->t_left->t_type) == 0 || op == ASSIGN) &&
						(skp=match(tp,cookie,reg)) != 0 )
					return(skp);
				tp->t_left = ltp;
			}
		}
		else if( convop(rtp->t_op) ) {
			tp->t_right = rtp->t_left;
			if( (unsign(rtp->t_left->t_type) == 0 || op == ASSIGN) &&
					(skp=match(tp,cookie,reg)) != 0 )
				return(skp);
			tp->t_right = rtp;
		}
	}
	switch( cookie ) {

	case FORCC:
		i = 3;
		break;

	case FOREFF:
		i = 2;
		break;

	case FORSTACK:
	case FORSP:
		i = 4;
		break;

	case FORREG:
		i = 5;
		break;
	
	default:
		error("match cookie=%d\n",cookie);
		return(0);
	}
	if( (i=optab[op][i]) == 0 )
		return(0);
	skp = codeskels[i];
#ifndef NODEBUG
	if(mflag) {
		printf("match LEFT ");
		puttsu(ltp);
		if(bop) {
			printf(" RIGHT ");
			puttsu(rtp);
		}
		putchar('\n');
	}
#endif
	for( ; skp->sk_left != 0; skp++ ) {
#ifndef NODEBUG
		if( mflag > 1 )
			printf("sk_left=%x sk_right=%x\n",skp->sk_left,skp->sk_right);
#endif
		if( skelmatch(ltp,skp->sk_left) == 0 )
			continue;
		if( bop && skelmatch(rtp,skp->sk_right) == 0 )
			continue;
#ifndef NODEBUG
		if( mflag )
			printf("match found skp=%o left=%x right=%x\n",skp,
					skp->sk_left,skp->sk_right);
#endif
		return(skp);
	}
	return(0);
}

/* unsign - check for UNSIGNED type*/
unsign(type)						/* returns 1 for unsigned, 0 otherwise*/
int type;							/* type to check*/
{
	return(type == UNSIGNED);
}

/* longorptr - check for being long or ptr*/
longorptr(type)						/* returns 1 for yes 0 for no*/
int type;
{
	return( type == LONG || suptype(type) );
}

unorptr(type)
int type;
{
	if( type == UNSIGNED )
		return(1);
	if( suptype(type) )
		return(1);
	return( 0 );
}

/* basetype - get the btype info sans unsigned*/
basetype(type)						/* returns signed base type*/
int type;							/* type to check*/
{
	return( type == UNSIGNED ? INT : type);
}

/* dreg - get corresponding data register*/
dreg(reg)							/* returns data register index*/
int reg;							/* register index*/
{
	return( reg & (~AREGLO) );
}

/* areg - return corresponding address register*/
areg(reg)							/* returns address register index*/
int reg;							/* register index*/
{
	return( reg | AREGLO );
}

/* isareg - check register for being an address register*/
isareg(reg)
int reg;
{
	return( reg >= AREGLO );
}

/* isdreg - check register for being data register*/
isdreg(reg)							/* returns 1 if data reg, 0 otherwise*/
int reg;							/* register index*/
{
	return( reg < AREGLO );
}

/* isreg - tests tree for being a register*/
isreg(tp)							/* returns 1 if is reg, else 0*/
struct tnode *tp;					/* pointer to tree*/
{
	return( tp->t_op == SYMBOL && tp->t_sc == REGISTER );
}

/* skelmatch - sub-tree type matching for match*/
/*		This checks a subtree for type compatability in match.*/
skelmatch(tp,skinfo)					/* returns 1 if matched, else 0*/
struct tnode *tp;						/* pointer to expression tree*/
int skinfo;
{
	register int type, unsignf, const, stype;

	if( tp->t_su > skinfo || ((skinfo&T_INDR) && tp->t_op != INDR) )
		return(0);
	stype = SK_TYPE(skinfo);
	type = tp->t_type;
	if( function(type) )
		type = btype(type);
	if( unsignf = unsign(type) )
		type = basetype(type);
	const = 0;
	switch( tp->t_op ) {

	case CLONG:
		if( tp->t_su > SU_CONST )
			break;
	case CINT:
		const++;
		break;
	}
	switch( stype ) {

	case T_CHAR:
		return( type == CHAR );

	case T_ANY:							/*either int or char*/
		if( type == CHAR )
			return(1);
	case T_INT:
		return( type == INT || const );

	case T_UNSN:
		return( unsignf );

	case T_LONG:
		return( longorptr(type) );

	default:
		error("skelmatch type: %x",stype);
		return(0);
	}
}

/* loadexpr - load an addressable expression into a register*/
/*		This checks for any possible usage of the register indexed*/
/*		addressing mode.  Note that this relies on the good graces of the*/
/*		load code skeletons not to muck up the compiler registers before*/
/*		loading an addressable expression...*/
loadexpr(tp,cookie,reg)					/* returns register loaded or -1*/
struct tnode *tp;						/* pointer to expression tree*/
int reg;								/* register to load*/
{
	register struct tnode *rtp, *ltp, *xtp, *atp;
	register int off, r, type, nr, ar, xr, xt;

	if( tp->t_op == INDR || LOADABLE(tp) ) {
		type = tp->t_type;
		if( tp->t_op == INDR && (ltp=tp->t_left)->t_op == ADD ) {
			rtp = ltp->t_right;
			ltp = ltp->t_left;
			off = 0;
			if( rtp->t_op == CINT && ((off=rtp->t_value) < -128 ||
					off > 127 || ltp->t_op != ADD ) ) {
				tp = snalloc(type,AUTO,off,0,0);
				if( indexreg(ltp) )
					tp->t_reg = ltp->t_reg;
				else {
					r = codegen(ltp,FORREG,areg(reg));
					if( isdreg(r) )
						outmovr(r,areg(r),ltp);
					tp->t_reg = areg(r);
				}
			}
			else {
				if( rtp->t_op == CINT ) {
					rtp = ltp->t_right;
					ltp = ltp->t_left;
				}
				if( indexreg(rtp) || (indexreg(ltp) == 0 && isreg(rtp)) ) {
					xtp = ltp;
					ltp = rtp;
					rtp = xtp;
				}
				xtp = 0;
				atp = 0;
				if( indexreg(ltp) ) {
					ar = ltp->t_reg;
					if( isreg(rtp) && rtp->t_type != CHAR ) {
						xr = rtp->t_reg;
						xt = rtp->t_type;
					}
					else
						xtp = rtp;
				}
				else if( isreg(ltp) && ltp->t_type != CHAR &&
						(lflag || rtp->t_op != ADDR) ) {
					xr = ltp->t_reg;
					xt = ltp->t_type;
					atp = rtp;
				}
				else if( rtp->t_op == ADDR ) {
					atp = ltp;
					xtp = rtp;
				}
				else {
					atp = rtp;
					xtp = ltp;
				}
				nr = 0;
				if( atp )
					nr++;
				if( xtp && (xtp->t_op != ADDR || lflag ) )
					nr++;
				if( dreg(nr+reg) <= HICREG ) {
					r = reg;
					if( atp ) {
						ar = codegen(atp,FORREG,areg(r));
						if( isdreg(ar) ) {
							outmovr(ar,areg(ar),atp);
							ar = areg(ar);
						}
						r++;
					}
					if( xtp && xtp->t_op == ADDR && lflag == 0 ) {
						tp = xtp->t_left;
						tp->t_sc =+ (EXTOFF-EXTERNAL);
						tp->t_offset =+ off;
						tp->t_reg = ar;
					}
					else {
						if( xtp ) {
							xr = codegen(xtp,FORREG,areg(r));
							xt = xtp->t_type;
						}
						tp = xnalloc(type,ar,off,xr,xt);
					}
				}
			}
		}
		if( isareg(reg) && tp->t_type == CHAR )
			reg = dreg(reg);
		tp = tnalloc(LOAD,tp->t_type,SU_EASY,0,tp,null);
		return( codegen(tp,cookie,reg) );
	}
	return(-1);

}

/* coffset - check offset for addressable node*/
char *coffset(tp)						/* returns ptr to const off node*/
struct tnode *tp;						/* pointer to node*/
{
	register struct tnode *rtp;

	if( tp->t_op == ADD ) {
		rtp = tp->t_right;
		if( rtp->t_op == CINT )
			return(rtp);
		if( lflag == 0 ) {
			if( rtp->t_op == ADDR )
				return(rtp->t_left);
			rtp = tp->t_left;
			if( rtp->t_op == ADDR ) {
				tp->t_left = tp->t_right;
				tp->t_right = rtp;
				return(rtp->t_left);
			}
		}
	}
	return(0);
}

/* hardrel - do relationals returning a value*/
hardrel(tp,cookie,reg)				/* returns reg or -1*/
struct tnode *tp;					/* pointer to tree*/
int cookie;							/* cookie for code generation*/
int reg;							/* low register*/
{
	char *p;
	int op, lab1, lab2;

	if( cookie != FORCC && (relop(op=tp->t_op) || op == LOR ||
			op == LAND || op == NOT) ) {
		lab1 = nextlabel++;
		condbr(tp,TRUE,lab1,reg);
		p = canon(cnalloc(INT,0));
		codegen(p,cookie,reg);
		lab2 = nextlabel++;
		outgoto(lab2);
		outlab(lab1);
		p = canon(cnalloc(INT,1));
		codegen(p,cookie,reg);
		outlab(lab2);
		return(reg);
	}
	return(-1);
}

/* cqmark - compile question mark operator*/
/*		This does the compilation of the question mark operator.*/
cqmark(tp,cookie,reg)				/* returns reg or -1*/
struct tnode *tp;
int cookie;
int reg;
{
	register int lab1, lab2, savestk, r;

	if( tp->t_op == QMARK && cookie != FORCC ) {
		lab1 = nextlabel++;
		condbr(tp->t_left,FALSE,lab1,reg);
		savestk = stacksize;
		r = codegen(tp->t_right->t_left,cookie,reg);
		outmovr(r,reg,tp);
		stacksize = savestk;
		outgoto(lab2=nextlabel++);
		outlab(lab1);
		r = codegen(tp->t_right->t_right,cookie,reg);
		outmovr(r,reg,tp);
		outlab(lab2);
		return(reg);
	}
	return(-1);
}

/* condbr - handle conditional branch code generation*/
/*		This handles the conditional branch code generation, handling*/
/*		the special cases for constants, ||, &&, ! and generating the*/
/*		correct conditional branch instruction.*/
condbr(tp,dir,lab,reg)
struct tnode *tp;
int dir;
int lab;
int reg;
{
	register struct tnode *ltp, *rtp;
	register int lab1, optype, op, subdir;

	ltp = tp->t_left;
	if( binop(op=tp->t_op) )
		rtp = tp->t_right;
	subdir = dir;					/*set up for LOR*/
	switch( op ) {

	case CINT:
		if( tp->t_value == 0 ) {
			if( dir == FALSE )
				outgoto(lab);
		}
		else if( dir != FALSE )
			outgoto(lab);
		break;

	case NOT:
		condbr(ltp,!dir,lab,reg);
		break;

	case LAND:
		dir = !dir;
	case LOR:
		if( dir == FALSE ) {
			lab1 = nextlabel++;
			condbr(ltp,!subdir,lab1,reg);
			condbr(rtp,subdir,lab,reg);
			outlab(lab1);
		}
		else {
			condbr(ltp,subdir,lab,reg);
			condbr(rtp,subdir,lab,reg);
		}
		break;

	case COMMA:
		scodegen(tp->t_left,FOREFF,reg);
		condbr(tp->t_right,dir,lab,reg);
		break;

	default:
		if( relop(op) && ltp->t_op == AUTOINC && rtp->t_op == AUTOINC &&
				ltp->t_type == rtp->t_type )
			outcmpm(tp);
		else
			scodegen(tp,FORCC,reg);
		optype = 0;
		if( relop(op) ) {
			if( unorptr(ltp->t_type) || unorptr(rtp->t_type) )
				optype =+ 1;
		}
		else
			op = NEQUALS;
		if( dir == 0 )
			op = invrel[op-EQUALS];
		optype = brtab[op-EQUALS][optype];
		printf("%s L%d\n",mnemonics[optype],lab);
		break;
	}
}

rcodegen(tpp,cookie,reg)			/* returns changed flag*/
struct tnode **tpp;					/* pointer to tree*/
int cookie;							/* code generation cookie*/
int reg;							/* register to use for code*/
{
	register int change, op;
	register struct tnode *tp;

	tp = *tpp;
	op = tp->t_op;
	change = 0;
	if( notleafop(op) && op != COMMA ) {
		change =+ rcodegen(&tp->t_left,cookie,reg);
		if( binop(op) )
			change =+ rcodegen(&tp->t_right,cookie,reg);
		change =+ rcgen(tpp,cookie,reg);
	}
	if( change )
		*tpp = canon(*tpp);
	return(change);
}

rcgen(tpp,cookie,reg)				/* returns changed flag*/
struct tnode **tpp;					/* pointer to tree*/
int cookie;							/* code generation goals*/
int reg;							/* register to use*/
{
	register struct tnode *tp, *p, *ltp, *rtp;
	register int op, change;

	change = 0;
	for( tp = *tpp ; binop(op=tp->t_op); *tpp=tp=canon(tp), change++ ) {
		ltp = tp->t_left;
		if( ltp->t_op != SYMBOL )
			break;
		rtp = tp->t_right;
		switch( op ) {

		case ASSIGN:
			if( ltp->t_sc != REGISTER )
				return(change);
			switch( rtp->t_op ) {

			case MULT:
			case DIV:
			case AND:
			case OR:
			case XOR:
			case LSH:
			case RSH:
				if( isareg(ltp->t_reg) )
					return(change);
			case ADD:
			case SUB:
				p = rtp->t_right;
				if( NOTADDRESSABLE(p) || (p->t_op == SYMBOL &&
						p->t_sc == REGISTER && p->t_reg == ltp->t_reg) )
					return(change);
				p = rtp->t_left;
				if( p->t_op != SYMBOL || p->t_sc != REGISTER ||
						p->t_reg != ltp->t_reg ) {
					tp->t_right = p;
#ifndef NODEBUG
					if( cflag > 1 )
						putexpr("rcgen",tp);
#endif
					codegen(tp,FOREFF,reg);
				}
				tp->t_right = rtp->t_right;
				tp->t_op = rtp->t_op + (EQADD-ADD);
				continue;
			}
		case EQLSH:
		case EQRSH:
			if( ltp->t_sc != REGISTER )
				return(change);
		case EQADD:
		case EQSUB:
		case EQAND:
		case EQOR:
		case EQXOR:
#ifndef NODEBUG
			if( cflag > 1 )
				putexpr("rcgen",tp);
#endif
			ucodegen(tp,FOREFF,reg);
			tp = ltp;
			continue;

		case PREDEC:
		case PREINC:
			if( cookie == FORCC )
				return(change);
			ucodegen(tp,FOREFF,reg);
			tp = ltp;
			continue;
		}
		break;
	}
	return(change);
}

/* cdsize - compute size of data item*/
cdsize(tp)						/* returns data size in bytes*/
struct tnode *tp;
{
	register int type;

	type = tp->t_type;
	if( suptype(type) )
		return(PTRSIZE);
	switch( type ) {

	case CHAR:
	case INT:
	case UNSIGNED:
		return(INTSIZE);

	case LONG:
		return(LONGSIZE);
	}
	error("cdsize: invalid type");
	return(0);
}

dofarg(tp)						/* returns number of bytes pushed*/
struct tnode *tp;				/* pointer to expression tree*/
{
	register int nb;

	nb = 0;
	if( tp->t_op == SYMBOL && tp->t_sc == STRUCT )
		error("structure operation not implemented");
	else if( stacksize ) {
		codegen(tp,FORSTACK,0);
		nb = cdsize(tp);
	}
	else
		codegen(tp,FORSP,0);
	return( nb );
}

/* dobitadd - do bit operation address checking and fixup*/
dobitadd(tp,bitno)						/* returns -1 if can't or bitno*/
struct tnode *tp;
int bitno;
{
	register int offset;

	if( tp->t_type == CHAR )
		offset = 0;
	else
		offset = cdsize(tp) - (bitno/BITSPBYTE) - 1;
	if( tp->t_op == SYMBOL ) {
		switch( tp->t_sc ) {

		case REGISTER:
			if( isdreg(tp->t_reg) )
				return(bitno);
		default:
			return(-1);

		case EXTERNAL:
		case STATIC:
		case REGOFF:
		case STATOFF:
		case EXTOFF:
			tp->t_offset =+ offset;
			return( bitno % BITSPBYTE );
		}
	}
	else if( tp->t_op == INDR ) {
		tp->t_left = tnalloc(ADD,tp->t_left->t_type,0,0,tp->t_left,
							cnalloc(INT,offset));
		return( bitno % BITSPBYTE );
	}
	return(-1);
}

isonebit(tp)					/* returns -1 if not 1 bit, else bitno*/
struct tnode *tp;					/* pointer to tree*/
{
	if( tp = constant(tp) )
		return( onebit(tp->t_value) );
	return(-1);
}
