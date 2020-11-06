/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121

	@(#)util.c	1.7	   12/15/83
*/

#include "cgen.h"
#include "cskel.h"

char null[];
char *opname[];

/* xnalloc - allocate address-indexed node*/
char *xnalloc(type,ar,off,xr,xt)	/* returns ptr to node alloced*/
int type;						/* data type*/
int ar;							/* address register*/
long off;						/* 8-bit offset*/
int xr;							/* index register*/
int xt;							/* index register type*/
{
	register struct indexnode *xp;

	xp = talloc(sizeof(*xp));
	xp->t_op = SYMBOL;
	xp->t_type = type;
	xp->t_sc = INDEXED;
	xp->t_reg = ar;
	xp->t_su = SU_ADDR;
	xp->t_offset = off;
	xp->t_xreg = xr;
	xp->t_xtype = xt;
	return(xp);
}

/* tcopy - expression tree copy*/
char *tcopy(tp,autof)					/* returns ptr to copied tree*/
struct tnode *tp;
int autof;								/* {A_DOPRE,A_DOPOST} */
{
	register char *p;
	register short op;

	op = tp->t_op;
	switch( op ) {

	case SYMBOL:
		if( tp->t_sc == EXTERNAL || tp->t_sc == EXTOFF )
			p = cenalloc(tp->t_type,tp->t_sc,tp->t_symbol);
		else {
			p = snalloc(tp->t_type,tp->t_sc,tp->t_offset,0,0);
			p->t_label = tp->t_label;
		}
		p->t_offset = tp->t_offset;
		p->t_reg = tp->t_reg;
		return(p);

	case CINT:
		return(cnalloc(tp->t_type,tp->t_value));

	case CLONG:
		return(lcnalloc(tp->t_type,tp->t_lvalue));

	case CFLOAT:		/*[vlh] 3.4 */
		return(fpcnalloc(tp->t_type,tp->t_lvalue));

	case DCLONG:
		p = lcnalloc(tp->t_type,tp->t_lvalue);
		p->t_op = DCLONG;
		return(p);

	case AUTOINC:		/*[mac] 4.2*/
	case AUTODEC:
		p = snalloc(tp->t_type,AUTO,0L,0,0);
		if( op == AUTODEC ) {
			if( (autof&A_DOPRE) != 0 )
				p->t_op = AUTODEC;
		}
		else if( op == AUTOINC ) {
			if( (autof&A_DOPOST) != 0 )
				p->t_op = AUTOINC;
		}
		p->t_reg = tp->t_reg;
		return(p);

	case POSTINC:
	case POSTDEC:
		if( (autof&A_DOPOST) == 0 )
			return(tcopy(tp->t_left,autof));
		goto copyop;
	case PREINC:
	case PREDEC:
		if( (autof&A_DOPRE) == 0 )
			return(tcopy(tp->t_left,autof));
copyop:
	default:
		if( ISASGOP(op) ) {
			if( (autof&A_DOPRE) == 0 )
				return(tcopy(tp->t_left,autof));
			autof |= A_DOPOST;		/*We'll come this way only once*/
		}
		p = tnalloc(op,tp->t_type,0,0,null,null);
		if( BINOP(op) )
			p->t_right = tcopy(tp->t_right,autof);
		p->t_left = tcopy(tp->t_left,autof);
		return(p);
	}
}

/* outaexpr - output address expression*/
outaexpr(tp,flags)				/* returns - none*/
struct tnode *tp;				/* pointer to tree*/
int flags;						/* flags (IMMED,LOFFSET,...)*/
{
	register short reg, lab;
	register long off;
	long l;

	if( tp->t_op == ADDR ) {
		tp = tp->t_left;
		putchar('#');
	}
	off = tp->t_offset;
	reg = tp->t_reg;
	lab = tp->t_label;
	switch( tp->t_op ) {
	
		case AUTOINC:
			if( (flags&A_DOPOST) != 0 )
				printf("(R%d)+",reg);
			else
				printf("(R%d)",reg);
			break;
		
		case AUTODEC:
			if( (flags&A_DOPRE) != 0 )
				printf("-(R%d)",reg);
			else
				printf("(R%d)",reg);
			break;
	
		case CINT:
			if( (flags&A_DOIMMED) != 0 )
				putchar('#');
			printf("%d",tp->t_value);
			break;
	
		case DCLONG:
		case CLONG:
		case CFLOAT:		/*[vlh] 3.4 */
			if( (flags&A_DOIMMED) != 0 )
				putchar('#');
			outlval(tp->t_lvalue);
			break;
	
		case SYMBOL:
			if( off ) {
				switch( tp->t_sc ) {
		
				default:
					printf("%ld+",off);
					break;

				case REGOFF:
					printf("%ld",off);
				case CINDR:
				case CLINDR:
				case CFINDR:	/* [vlh] 3.4 */
				case INDEXED:
					break;

				case REGISTER:
					error("invalid register expression");
					break;
				}
			}
			switch( tp->t_sc ) {
	
				case REGISTER:
					printf("R%d",reg);
					break;
		
				case REGOFF:
					printf("(R%d)",reg);
					break;
		
				case EXTERNAL:
					printf("_%.8s",tp->t_symbol);
					break;
		
				case EXTOFF:
					printf("_%.8s(R%d)",tp->t_symbol,reg);
					break;
		
				case STATIC:
					printf("L%d",lab);
					break;
		
				case STATOFF:
					printf("L%d(R%d)",lab,reg);
					break;

				case INDEXED:
					printf("%ld(R",off);	/* [vlh] 4.3, long offset */
					printf("%d,R%d",reg,tp->t_xreg);
					outatype(tp->t_xtype);
					putchar(')');
					break;

				case CINDR:
					printf("%ld",off);
					break;
		/*
		 * the following will work on: PDP-11, 68000, IBM-360, VAX, etc.
		 * it will not work on word machines or on machines where either
		 * longs two ints or two shorts.
		 */
				case CLINDR:
				case CFINDR:	/* [vlh] 3.4 */
					/*l.hiword = (short)tp->t_offset; l.loword = tp->t_ssp;*/
					l = tp->t_offset;	/* [vlh] 4.3, changed to long !!! */
					outlval(l);
					break;
		
				default:
					error("invalid storage class %d\n",tp->t_sc);
					break;
			}
			break;

		default:
			error("invalid operator %s\n",opname[tp->t_op]);
			break;
	}
}

/* outlval - output long value*/
/*		This is a big pain because the PDP-11 doesn't do long divides*/
/*		in hardware.*/
outlval(lval)
long lval;
{
	char digs[8];
	register short i, c;

	i = 0;
	do {
		digs[i++] = lval & 0xf;
		lval >>= 4;
		lval &= 0xfffffff;
	} while ( lval );
	putchar('$');
	while( --i >= 0 ) {
		c = digs[i];
		putchar(c>=10?c+('a'-10):c+'0');
	}
}

/* outtype - output 68000 type (null, .b, .l) depending on data type*/
outtype(type)
int type;
{
	if( ISFLOAT(type) || LONGORPTR(type) )
		printf(".l");
	else if( type == CHAR )
		printf(".b");
}

/* outatype - output address type (.l or null) depending on data type*/
outatype(type)
int type;
{
	if( LONGORPTR(type) || ISFLOAT(type) )
		printf(".l");
}

/* outextend - output register extension to long depending on type*/
outextend(tp,type,reg)						/* returns - none*/
struct tnode *tp;							/* tree to convert from*/
int type;									/* type to convert to*/
int reg;									/* register to convert*/
{
	if( (ISDREG(reg)) && !(LONGORPTR(tp->t_type)) && (LONGORPTR(type)) ) {
		if( UNSIGN(tp->t_type) )
			OUTUEXT(reg);
		else
			OUTEXT(reg);
	}
}

/* outrr - output register to register instruction*/
outrr(ins,r1,r2,tp)
char *ins;
int r1;
int r2;
struct tnode *tp;
{
	printf("%s",ins);
	if( ISAREG(r1) || ISAREG(r2) )
		outatype(tp->t_type);
	else
		outtype(tp->t_type);
	printf(" R%d,R%d\n",r1,r2);
}

/* outmovr - output "move[type] R1,R2" instruction*/
outmovr(r1,r2,tp)
int r1;
int r2;
struct tnode *tp;
{
	if( r1 != r2 )
		outrr("move",r1,r2,tp);
}

/* outcmpm - output "cmpm[type] (R1)+,(R2)+"*/
outcmpm(tp)
struct tnode *tp;
{
	register int lreg, rreg;

	if (!m68010) {	/* 68000 version cmpm ok !!! */
		printf("cmpm");
		outtype(tp->t_left->t_type);
		printf(" (R%d)+,(R%d)+\n",tp->t_left->t_reg,tp->t_right->t_reg);
	}
	else	/* generating code for 68010, micro code is bad for cmpm */
		fakecmpm(tp->t_left->t_type,tp->t_left->t_reg,tp->t_right->t_reg);
}

/* fakecmpm - output 68010 fake cmpm for bad microcode [vlh] 4.2 */
fakecmpm(type,lreg,rreg)
int type,lreg,rreg;
{
	printf("move");
	outtype(type);
	printf(" (R%d),R0\n",rreg);
	printf("cmp");
	outtype(type);
	printf(" (R%d),R0\n",lreg);
	if( ISFLOAT(type) || LONGORPTR(type) )
		printf("addq #4,R%d\naddq #4,R%d\n",lreg,rreg);
	else if( type == CHAR )
		printf("addq #1,R%d\naddq #1,R%d\n",lreg,rreg);
	else
		printf("addq #2,R%d\naddq #2,R%d\n",lreg,rreg);
}

/* outcreg - output reference to compiler temp register*/
outcreg(reg)
int reg;
{
	if( (DREG(reg)) > HICREG )
		error("expression too complex");
	printf("R%d",reg);
}

/* outcmp0 - output a compare with 0, special for address register*/
outcmp0(reg,tp)
int reg;
struct tnode *tp;
{
	if( ISAREG(reg) ) {
		printf("cmp");
		outatype(tp->t_type);
		printf(" #0,R%d\n",reg);
	}
	else {
		printf("tst");
		outtype(tp->t_type);
		printf(" R%d\n",reg);
	}
}

/* outrpush - output "move[type] R1,[-](sp)"*/
outrpush(reg,tp,pflag)
int reg;
struct tnode *tp;
int pflag;
{
	printf("move");
	outatype(tp->t_type);
	printf(" R%d,%s\n",reg,pflag?"-(sp)":"(sp)");
}

outdbra(dir,op,ltp,rtp,lab)
int dir, op, lab;
struct tnode *ltp, *rtp;
{
	if (!( (dir!=0 && op==NEQUALS) || (dir==0 && op==EQUALS)))
		return(0);
	if(!(ltp->t_op==PREDEC && rtp->t_op==CINT && rtp->t_value == -1 &&
			ISDREG(ltp->t_left->t_reg) && ltp->t_left->t_type==INT &&
			ltp->t_left->t_sc == REGISTER))
		return(0);
	
	printf("dbra R%d,L%d\n",ltp->t_left->t_reg,lab);
	return(1);
}

/* cenalloc - code generator external node allocation*/
/*		This may be coalesced into enalloc in parser.*/
char *cenalloc(type,sc,sym)		/* returns ptr to node alloced*/
int type;						/* type of symbol*/
int sc;							/* storage class*/
char *sym;						/* symbol name*/
{
	register struct extnode *ep;

	ep = talloc(sizeof(*ep));
	ep->t_op = SYMBOL;
	ep->t_type = type;
	ep->t_sc = sc;
	ep->t_su = 0;
	ep->t_offset = 0;
	symcopy(sym,ep->t_symbol);
	return(ep);
}

/*popstack - clear off the stack after a call if necessary */
popstack(nb)
{
	if (nb > 0 && nb <= 8)
		printf("addq.l #%d,sp\n",nb);
	else if (nb > 0)
		printf("adda.l #%d,sp\n",nb);
}
