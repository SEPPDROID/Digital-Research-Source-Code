/*
	Copyright 1982
	Alcyon Corporation
	8716 Production Ave.
	San Diego, Ca.  92121
*/

#include "parser.h"
#define	DREG	0100				/*data loadable into D-register?*/

char dinfo[] {
	0,								/*TYPELESS=0*/
	1|DREG,							/*CHAR=1*/
	2|DREG,							/*SHORT=2*/
	2|DREG,							/*INT=3*/
	4|DREG,							/*LONG=4*/
	1|DREG,							/*UCHAR=5*/
	2|DREG,							/*USHORT=6*/
	2|DREG,							/*UNSIGNED=7*/
	4|DREG,							/*ULONG=8*/
	4|DREG,							/*FLOAT=9[vlh]*/
	4|DREG,							/*DOUBLE=10[vlh]*/
	0,								/*STRUCT=11*/
	0,								/*FRSTRUCT=12*/
	0,								/*LLABEL=13*/
	0,								/*INVALID=14*/
	0,								/*INVALID=15*/
};

char aregtab[] { AREG5, AREG4, AREG3, 0 };
char dregtab[] { DREG7, DREG6, DREG5, DREG4, DREG3, 0 };
int inittype;

/*
 * doextdef - external definition syntax
 *		This is fairly complicated since you do not know if you are
 *		parsing a external function declaration or a real function
 *		until after you've already scanned the argument list for the
 *		function.  Basically you start off scanning an external declaration
 *		or function in the same way by collecting attributes, scanning
 *		the declarator, then scanning the function arguments if a function.
 *		At that point you look at the next token and if its a '{', keyword
 *		proceed accordingly.
 *		The C Syntax handled by this routine is (roughly):
 *			external_definition:
 *					function_definition
 *					data_definition
 *			function_definition:
 *					type_specifier function_declarator function_body
 *			function_declarator:
 *					declarator ( parameter_list )
 *			data_definition:
 *					EXTERNAL type_specifier init_declarator_list ;
 *					STATIC type_specifier init_declarator_list ;
 */
doextdef()							/* returns 0 for EOF or 1*/
{
	register struct symbol *sp;
	register int dflag;
	int sc, type;
	long size;

	if(!next(SEMI)) {
		opap = exprp = exprarea;
		sc = EXTERNAL;
		type = (xflag?LONG:INT);
		dflag = gettype(&sc,&type,&size);
		if(type==STRUCT)	/* deal with forward ref structures */
			chksyms();
		while( dodecl(sc,type,0,size), (sp=dsp) != 0 ) {
			if( !dflag && notfunction(sp->s_type) ) {
				synerr("external definition syntax");
				return;
			}
			if( !stypedef(sp) && sc != STATIC )
				if (notfunction(sp->s_type))	/*[vlh] .globl ext. vars*/
					outextdef(sp->s_symbol);
			if( notfunction(sp->s_type) ) {	/*not function, check init*/
				if( !stypedef(sp) ) {
					doinit(sp);
					if (sc == STATIC)
						chksyms();
				}
			}
			else if( peek(RESWORD) || peek(LCURBR) ||
					(peek(SYMBOL) && stypedef(csp)) ) {
				if(!stypedef(sp) && sc!=STATIC)	/*[vlh] .globl local proc*/
					outextdef(sp->s_symbol);
				funcbody(sp);
				return;
			}
			dsp = 0;
			if( !next(COMMA) )
				break;
		}
		if( gettok() != SEMI )
			synerr("external definition syntax");
	}
}

/*
 * gettype - get attribute types in attribute list
 *		Handles single word keywords, such as int, char, etc. and also
 *		handles the declarations of structures and unions.
 */
gettype(defsc,deftype,size)			/* returns 0 for no type, 1 otherwise*/
int *defsc;							/* default storage class*/
int *deftype;						/* default data type*/
long *size;							/* size of data element 3.4 int=>long*/
{
	register int dtype, sc;
	register int sflag, uflag, lflag, decflag;
	register int token, stdflag, sbits;
	long tsize;
	register struct symbol *sp, *parent;

	tdp = 0; tdflag = uflag = decflag = lflag = sflag = 0; tsize = 0L;
	dtype = TYPELESS;
	sc = *defsc;
	for( ; ; decflag++ ) {
		if( (token=gettok()) == SYMBOL && stypedef(csp) ) {
			dtype = 0;
			tdp = csp;
			continue;
		}
		if( token != RESWORD )
			break;
		switch( cvalue ) {

		case R_TYPEDEF:
			if( tdflag )
				error("invalid typedef statement");
			tdflag++;
			continue;

		case R_STATIC:
			if( sc && sc != STATIC && sc != EXTERNAL )
				error("invalid storage class");
			sc = STATIC;
			continue;

		case R_AUTO:
			if( sc && sc != AUTO )
				error("invalid storage class");
			sc = AUTO;
			continue;

		case R_EXTERNAL:
			if( sc && sc != EXTERNAL )
				error("invalid storage class");
			sc = EXTERNAL;
			continue;
	
		case R_REGISTER:
			if( sc && sc != REGISTER && sc != PDECLIST )
				error("invalid register specification");
			sc = REGISTER;
			continue;
	
		case R_LONG:
			lflag++;
			continue;

		case R_SHORT:
			sflag++;
			continue;

		case R_UNSIGNED:
			uflag++;
			continue;

		case R_STRUCT:
			cvalue = STRUCT;
		case R_UNION:
			stdflag = tdflag;
			tdflag = 0; sp = 0;
			token = cvalue;
			smember++;
			if( next(SYMBOL) ) {	/*struct [symbol] { ... }*/
				sp = csp;
				parent = csp;
				strucptr[smember+instruct] = sp;
				if( !sp->s_sc ) {
					sp->s_attrib =| SDEFINED;
					if(!infunc)
						sp->s_attrib =| SGLOBAL;
					sp->s_sc = STRPROTO;
					sp->s_type = STRUCT;
					sp->s_ssp = dalloc(0L);
				}
				if( sp->s_sc != STRPROTO )
					error("redeclaration: %.8s",sp->s_symbol);
			}
			else parent = 0;
			smember = 0;
			if( next(LCURBR) ) {
				instruct++;
				strucptr[smember+instruct] = sp;
				sbits = boffset;
				boffset = 0;
				tsize = dlist(token==STRUCT?STELCL:UNELCL);
				boffset = sbits;
				if(!next(RCURBR))
					synerr("structure declaration syntax");
				else if( sp ) {
					if( dtab[sp->s_ssp] )
						error("redeclaration: %.8s",sp->s_symbol);
					dtab[sp->s_ssp] = tsize;
				}
				instruct--;
			}
			else if( !sp )
				error("no structure name");
			else if( sp->s_sc != STRPROTO )
				error("invalid structure prototype: %.8s",sp->s_symbol);
			else if( !dtab[sp->s_ssp] ) {	/* FRSTRUCT */
				token = FRSTRUCT;
				if( ++frstp >= NFRSTR )
					ferror("structure table overflow");
				frstab[frstp] = sp;
			}
			else
				tsize = dtab[sp->s_ssp];
			tdflag = stdflag;
			if( dtype != TYPELESS )
				error("invalid type declaration");
			dtype = (token == R_UNION) ? STRUCT : token;
			continue;

		case R_INT:
			if( dtype != TYPELESS )
				error("invalid type declaration");
			dtype = INT;
			continue;

		case R_CHAR:
			if( dtype != TYPELESS )
				error("invalid type declaration");
			dtype = CHAR;
			continue;

		case R_FLOAT:	/*[vlh] ver. 3.4*/
		case R_DOUBLE:
			if( dtype != TYPELESS )
				error("invalid type declaration");
			dtype = FLOAT;
			continue;
		}
		break;
	}
	pbtok(token);
	if( dtype == TYPELESS )
		dtype = INT;
	if(!sc)
		sc = AUTO;
	if( lflag ) {					/*allow: long float, long int*/
		if( dtype == INT )
			dtype = LONG;
/*		else if( dtype == FLOAT )	*/
/*			dtype = DOUBLE;			*/
		else
			error("invalid long declaration");
	}
	if( sflag ) {
		if( dtype != INT )
			error("invalid short declaration");
	}
	if( uflag ) {
		if( dtype != INT )			/*allow: unsigned int*/
			error("invalid unsigned declaration");
		else
			dtype = UNSIGNED;
	}
	if( !sflag && xflag && dtype == INT )
		dtype = LONG;
	*defsc = sc;
	*deftype = dtype;
	*size = tsize;
	if (dtype == STRUCT || dtype == FRSTRUCT)
		csp->s_struc = parent;
	return(decflag);
}

/*
 * dodecl - process a single declarator
 *		This does all the grubby handling of a single declarator given
 *		the attributes for the declarator.  Handles typedef attributes
 *		adjusts offsets for structure elements, allocates register
 *		variables, etc.
 */
long								/* [vlh] 3.4 int => long */
dodecl(sc,type,offset,size)			/* returns size of declarator*/
int sc;								/* storage class*/
int type;							/* data type*/
int offset;							/* offset if in structure or union*/
long size;							/* size of single data item 3.4 i=> l*/
{
	register struct symbol *sp;
	register int redecf, stype, dtype, i, j;

	if( peek(SEMI) || peek(RPAREN) )
		return(0);
	if( instruct && next(COLON) ) {	/*handle bit filler field*/
		if(!(i=cexpr()))
			size = salign(INT,offset);
		else
			size = falign(type,i);
	}
	else if( (type=|declarator(0)) >= 0 && (sp=dsp) != 0 ) {
		if( tdp )					/*typedef name in declaration*/
			type = addtdtype(tdp,type,sp->s_dp,&(sp->s_ssp));
		else if( btype(type) == STRUCT ) {
			if( size )
				sp->s_ssp = dalloc(size);
			else
				error("invalid structure declaration: %.8s",sp->s_symbol);
		}
		else if( btype(type) == FRSTRUCT )
			sp->s_ssp = frstp;
		redecf = 0;
		switch( sp->s_sc ) {		/*check for redeclarations.*/

		case 0:
			break;

		case PARMLIST:
			if( sc != PDECLIST && sc != REGISTER )
				goto redec;
			break;

		case BFIELDCL:
			if( sc != STELCL && sc != UNELCL )
				goto redec;
			break;

		case STELCL:
		case UNELCL:
			redecf++;
			break;	/* [vlh] get rid of redecf entirely ?? */

		case EXTERNAL:
			if (sp->s_type == type) {
				if (sc == sp->s_sc) break;
				if (sc == AUTO && suptype(type) == FUNCTION) {
					sc = EXTERNAL;
					break;
				}
			}
		default:
redec:
			error("redeclaration: %.8s",sp->s_symbol);
			return(size);
		}
		sp->s_type = type;
		dtype = suptype(type);
		type = btype(type);
		if( tdflag )				/*we are declaring typedef?*/
			sp->s_attrib =| STYPEDEF;
		if( instruct ) {
			if( next(COLON) ) {		/*handle bit field*/
				sc = BFIELDCL;
				i = cexpr();
				j = (boffset<<8)|i;
				if( redecf && sp->s_dp != j )	/* ??? */
					goto redec;
				sp->s_dp = j;
				size = j = falign(type,i);
			}
			else
				size = dsize(sp->s_type,sp->s_dp,sp->s_ssp) +
						(j=salign(type,offset));
			offset =+ j;
/*			if( redecf && sp->s_offset != offset )
				goto redec;	*/	/* [vlh] */
			sp->s_offset = offset;
		}
		if( dtype == FUNCTION ) {
			if( sc != AUTO && sc != EXTERNAL && sc != STATIC )
				error("illegal function declaration");
			if( sc != STATIC )
				sc = EXTERNAL;
		}
		else if( sc == REGISTER ) {
			if( !dtype ) {
				if( !(dinfo[type]&DREG) ) {	/* ignore reg specification */
/*					error("illegal register specification"); */
					sc = AUTO;
				}
				else if( !dregtab[ndregs] )
					sc = AUTO;
				else
					sp->s_offset = dregtab[ndregs++];
			}
			else if( dtype != POINTER ) {
				error("illegal register specification");
				sc = AUTO;			/*no more regs, make it auto*/
			}
			else if( !aregtab[naregs] )
				sc = AUTO;
			else
				sp->s_offset = aregtab[naregs++];
		}
		if( sc == AUTO ) {
			localsize =+ walign(dsize(sp->s_type,sp->s_dp,sp->s_ssp));
			sp->s_offset = -localsize;
		}
		else if( sc == STATIC )
			sp->s_offset = nextlabel++;
		sp->s_sc = sc;
		sp->s_attrib =| SDEFINED;
		if(!infunc)
			sp->s_attrib =| SGLOBAL;
	}
	return(size);
}

/*
 * doinit - do external definition initialization
 *		Handles following C syntax:
 *			initializer:
 *					= constant_expression
 *					= { initializer_list }
 *					= { initializer_list , }
 *			initializer_list:
 *					constant_expression
 *					initializer_list , initializer_list
 *					{ initializer_list }
 */
doinit(sp)							/* returns number of elements init'ed*/
struct symbol *sp;					/* pointer to symbol to init*/
{
	register int type;
	long isize, datasize, elsize;

	type = sp->s_type;
	datasize = dsize(type,sp->s_dp,sp->s_ssp);
	if( peek(COMMA) || peek(SEMI) ) {	/*no initialization*/
		isize = walign(datasize);
		if( sp->s_sc == EXTERNAL )
			outcommon(sp->s_symbol,isize);
		else {
			outbss();				/*bss segment*/
			outlab(sp->s_offset);
			outresmem(isize);
			outtext();				/*resume text segment*/
		}
	}
	else {
		next(ASSIGN);				/*ignore '=' if there*/
		if( type == LONG || pointer(type) )
			outldata();
		else if(type == DOUBLE || type == FLOAT)	/*[vlh] 3.4*/
			outfpdata();
		else
			outdata();
		if( sp->s_sc == EXTERNAL )
			outdlab(sp->s_symbol);
		else
			outlab(sp->s_offset);
		isize = initlist(sp->s_type,sp->s_dp,sp->s_ssp);
		if( isize < datasize ) {
			outresmem(datasize-isize);
			isize = datasize;
		}
		if( walign(isize) != isize )
			outpad();
		if( isize > datasize ) {
			if( array(sp->s_type) ) {
				for( type = sp->s_type; array(type); type = delsp(type) )
					;
				dtab[sp->s_dp] = isize / dsize(type,sp->s_dp,sp->s_ssp);
			}
			else
				error("too many initializers");
		}
		if( sp->s_sc == STATIC )
			outtext();
	}
}

/*
 * initlist - handles initializer lists
 *		This handles multi-level braces, and a character pointer pointing
 *		to a string.  Most of the work is in keeping track of how many
 *		bytes are left on the current "row", and outputting padding if
 *		necessary.
 */
long
initlist(type,dp,sp)				/* returns size of initializers in*/
									/* bytes*/
int type;							/* type of data*/
int dp;								/* dimension pointer*/
int sp;								/* structure pointer*/
{
	register int nrows;
	long datasize, i, elsize, nbleft, nbout;	/* [vlh] 3.4 int=>long */
	register int onetype;

	for( onetype = type; array(onetype); onetype = delsp(onetype) )
		;
	nbout = 0;
	datasize = dsize(type,dp,sp);
	nbleft = 0;						/*keep track of no. of bytes left*/
	if( type == (ARRAY|CHAR) && next(STRING) ) {
		nbout = cstrsize;
		outstr();
		if( datasize > nbout )
			nbleft = datasize - nbout;
	}
	else if( array(type) || (btype(type)==STRUCT && notpointer(type)) ) {
		elsize = datasize / dsize(delsp(type),dp+1,sp);
		elsize = datasize / elsize;
		if(!next(LCURBR))
			error("missing { in initialization");
		if( type == (ARRAY|CHAR) && next(STRING) ) {
			nbout = cstrsize;
			outstr();
			if( datasize > nbout )
				nbleft = datasize - nbout;
		}
		else {
			do {						/*in current row.*/
				if( peek(SEMI) || peek(EOF) )
					break;
				if( peek(LCURBR) ) {	/*recurse down one level?*/
					if( nbleft ) {
						outresmem(nbleft);	/*pad rest of current row*/
						nbout =+ nbleft;
						nbleft = 0;
					}
					i = initlist(delsp(type),dp+1,sp);
					if( i > elsize )
						error("initializer list too long");
					else if( i < elsize )
						outresmem(elsize-i);
					nbout =+ elsize;
				}
				else if( peek(RCURBR) )
					break;
				else {
					i = oneinit(onetype);
					nbout =+ i;
					if(!nbleft)
						nbleft = elsize;
					if( i > nbleft )
						error("initializer alignment");
					nbleft = (i >= nbleft) ? 0 : nbleft - i;
				}
			} while( next(COMMA) );
		}
		if(!next(RCURBR))
			synerr("missing }");
	}
	else {
		i = next(LCURBR);			/*pull of optional {*/
		nbout = oneinit(onetype);
		if( i && !next(RCURBR) )
			synerr("missing }");
	}
	if( nbleft ) {					/*pad current row*/
		outresmem(nbleft);
		nbout =+ nbleft;
	}
	return(nbout);
}

/*
 * oneinit - get one initializer expression or constant
 *		This checks the type of the data item expected against the
 *		type obtained from expr.  Note that there is no attempt to
 *		match structure initializations against the elements of the
 *		structure, hence, anything goes in a structure.
 */
oneinit(type)						/* returns size of initializer*/
int type;							/* type of initializer*/
{
	register int op, value;
	register struct tnode *tp;

	commastop++;
	tp = expr();
	commastop--;
	op = tp->t_op;
	value = tp->t_value;
	switch( alltype(type) ) {

		case CHAR:
		case ARRAY|CHAR:
			if( op != CINT )
				break;
			outc(CHAR,value);
			return(1);
	
		case INT:
		case ARRAY|INT:
			if( op == CLONG || op == ADDR )
				if (op == CLONG && clvalue == -32768) {	/*[vlh] 4.0*/
					op = CINT;
					value = -32768;
				}
				else
					break;
			if( op == CINT )
				outc(INT,value);
			else
				outinit(tp,inittype);
			return(2);
	
		case UNSIGNED:	/* [vlh] 4.0 split from INT... */
		case ARRAY|UNSIGNED:
			if (op == ADDR)
				break;
			if( op == CLONG ) {	/* get unsigned values > legit pos ints */
				if (clvalue < 0L || clvalue > 65535L)
					break;
				op = CINT;
				value = clvalue & 0x7fff;
				if (clvalue > 32767)
					value = -value;
			}
			else if (op == CINT && value < 0)
				break;
			if( op == CINT )
				outc(INT,value);
			else
				outinit(tp,inittype);
			return(2);
	
		case LONG:
		case ARRAY|LONG:
		case POINTER|CHAR:
		case POINTER|INT:
		case POINTER|LONG:
		case POINTER|STRUCT:
		case POINTER|UNSIGNED:
			if( op == CINT ) {
				clvalue = value;	/*[vlh] fix ??? MC ??? */
				outlong(clvalue);
			}
			else if( op == CLONG )
				outlong(clvalue);
			else
				outinit(tp,inittype);
			return(4);
	
		case DOUBLE:	/* [vlh] 3.4 */
		case FLOAT:
			outfp(clvalue);
			return(4);
	
		case STRUCT:
		case ARRAY|STRUCT:
			if( op == CINT ) {
				if( xflag ) {
					clvalue = value;
					outlong(clvalue);
					return(4);
				}
				outc(INT,value);
				return(2);
			}
			if( op == CLONG ) {
				outlong(clvalue);
				return(4);
			}
			outinit(tp,inittype);
			if (xflag || op==ADDR)
				return(4);
			if ((op=tp->t_right->t_op)==ADDR || op==CLONG) /*[vlh]*/
				return(4);
			if ((op=tp->t_left->t_op)==ADDR || op==CLONG) /*[vlh]*/
				return(4);
			return(2);
	}
	error("invalid initializer");
	return(0);
}

/*
 * funcbody - do function body declaration
 *		Basically handles function after we have scanned the parameter
 *		list, which is now set up in fargs array.  We now proceed to
 *		look for any declarations for the function arguments, then the
 *		function declaration list, followed by the function statement list.
 *		The C syntax is:
 *			function_body:
 *					type_decl_list function_statement
 *			function_statement:
 *					{ declaration_list statement_list }
 */
funcbody(fsp)
struct symbol *fsp;
{
	register int olddp;
	register struct symbol *sp;
	register struct farg *fp;
	register int offset, toff;

	infunc++;
	sp = fsp;
	opap = exprp;
	frp = snalloc(delsp(sp->s_type),sp->s_sc,sp->s_type,sp->s_dp,
			sp->s_ssp);
	exprp = opap;
	outtext();
	outflab(sp->s_symbol);
	olddp = cdp;
	dlist(PDECLIST);
	rlabel = nextlabel++;
	if(!next(LCURBR))
		synerr("function body syntax");
	else {
		localsize = 0;			/*end of first auto offset from l.e.p.*/
		offset = 8;				/*first arg offset from l.e.p.*/
		for( fp = &fargtab[0]; sp = fp->f_sp; fp++ ) {
			toff = offset;
			if( sp->s_type == CHAR )	/*char argument*/
				toff++;			/*offset of lower byte in word*/
			if( sp->s_sc == REGISTER )
				fp->f_offset = toff;
			else {
				fp->f_offset = 0;	/*really is auto arg*/
				sp->s_offset = toff;
				sp->s_sc = AUTO;
			}
			if( array(sp->s_type) ) {	/*change array ref to pointer*/
				sp->s_type = addsp(delsp(sp->s_type),POINTER);
				sp->s_dp++;
			}
			offset =+ walign(dsize(sp->s_type,sp->s_dp,sp->s_ssp));
		}
		dlist(0);
		chksyms();
		outbentry(localsize,ndregs,naregs);
		copyargs();				/*copy args to registers where required*/
		while(!next(RCURBR)) {
			if( next(EOF) ) {
				error("{ not matched by }");
				break;
			}
			stmt();
		}
	}
	outlab(rlabel);
	outbexit(ndregs,naregs);
	cdp = olddp;
	infunc--;
	freesyms();
}

/*
 * copyargs - copy args to register where required
 *		fargtab has been set so that args declared to be registers have a
 *		non-zero offset value and the register number is in the symbol
 *		table pointed to by symbol.
 */
copyargs()							/* returns - none*/
{
	register struct symbol *sp;
	register struct farg *fp;

	for( fp = &fargtab[0]; sp = fp->f_sp; fp++ ) {
		if( fp->f_offset )		/*was declared register*/
			outassign(snalloc(sp->s_type,sp->s_sc,sp->s_offset,0,0),
					snalloc(sp->s_type,AUTO,fp->f_offset,0,0));
	}
}

/*
 * dlist - declaration list
 *		Handles declaration lists in the following places:
 *		function parameter list declarations, structure or union member
 *		declarations and local declarations in functions.
 */
long
dlist(defsc)						/* returns length of declarators*/
int defsc;							/* default storage class*/
{
	register int offset;
	register struct symbol *sp;
	register long i, ddsize;
	long size;	/* [vlh] 3.4 int => long */
	int type, sc, isize;

	offset = 0; ddsize = 0L;
	do {
		sc = defsc;
		type = INT;
		if( !gettype(&sc,&type,&size) )
			break;
		do {
			i = dodecl(sc,type,offset,size);
			if( defsc != UNELCL ) {
				isize = i;
				offset =+ isize;
				ddsize =+ i;
			}
			else if( i > ddsize )
				ddsize = i;
			if( sc == STATIC && dsp && !stypedef(dsp) )
				doinit(dsp);		/*process any initializer*/
			dsp = 0;
		} while ( next(COMMA) );
		if (next(ASSIGN)) {
			synerr("auto initialization not implemented");
			break;
		}
		if(!next(SEMI)) {
			synerr("declaration syntax");
			break;
		}
	} while( 1 );
	isize = ddsize;
	ddsize =+ salign(INT,isize);
	return(ddsize);
}

/*
 * declarator - get one declarator
 *		Basically uses getdecl, which returns the declaration types
 *		reversed in the type word.
 */
declarator(castflg)							/* returns type or -1*/
int castflg;
{
	register int type, t;

	dsp = 0;
	if( (type=getdecl(castflg)) >= 0 )
		return( revsp(type) );
	return(type);
}

/*
 * getdecl - get one declarator, handling *, (), etc.
 *		The significance of the value returned by declarator is:  the
 *		least significant two bits represent the values (POINTER,FUNCTION,
 *		ARRAY), these values are repeated through the word.  For example,
 *		the declarations result in the following values for declarator:
 *				*x() => (POINTER,FUNCTION)
 *				(*x)() => (FUNCTION,POINTER)
 *				*(*x)() => (POINTER,FUNCTION,POINTER)
 *		The following C syntax is handled here:
 *			function_declarator:
 *					declarator ( parameter_list )
 *			declarator:
 *					identifier
 *					( declarator )
 *					* declarator
 *					declarator [ constant-expression ]
 */
getdecl(castflg)				/* returns special type of declarator*/
int castflg;					/* casting flag, 1=>allow no declarator*/
{
	register int type, i, value, sdp;
	register struct symbol *sp, *tsp;
	register struct farg *fp;
	register char *p;
	long lvalue;

	type = 0;
	if( next(LPAREN) ) {			/*( declarator ) ...*/
		type = getdecl(castflg);
		if(!next(RPAREN))
			goto baddec;
	}
	if( next(MULT) )
		return(addsp(getdecl(castflg),POINTER));
	sdp = cdp;
	if( next(SYMBOL) ) {
		sp = dsp = csp;
		type = 0;
		sp->s_dp = sdp;
	}
	while( 1 ) {
		if( next(LPAREN) ) {		/*declarator ( ... )*/
			if(!infunc) {
				ndregs = naregs = 0;
				for( fp = &fargtab[0]; next(SYMBOL); ) {
					p = csp;
					if( p->s_attrib & SDEFINED )
						error("redeclaration: %.8s",p->s_symbol);
					else if( fp >= &fargtab[NFARGS-1] ) {
						synerr("too many params");
						break;
					}
					else {
						p->s_attrib =| SDEFINED;
						p->s_sc = PARMLIST;
						p->s_type = INT;	/*default to int*/
						fp->f_sp = p;
						fp++;
					}
					if(!next(COMMA))
						break;
				}
				fp->f_sp = 0;
			}
			if(!next(RPAREN))
				break;
			type = addsp(type,FUNCTION);
			continue;
		}
		if( next(LBRACK) ) {	/*declarator [ cexpr ]*/
			if( next(RBRACK) )
				dalloc(1L);
			else {
				tsp = dsp;			/* [vlh] 3.4 save in case of reset */
				value = cexpr();	/* recurses on sizeof.... resets dsp */
				dsp = tsp;			/* [vlh] 3.4 */
				lvalue = value;		/* [vlh] 3.4 */
				for( i = sdp; i < cdp; )
					dtab[i++] =* lvalue;
				dalloc(lvalue);
				if( !next(RBRACK) )
					break;
			}
			type = addsp(type,ARRAY);
			continue;
		}
		if( castflg || dsp )
			return(type);
		break;
	}
baddec:
	synerr("invalid declarator");
	return(-1);
}

/*
 * addtdtype - add typedef info into declarator
 *		here we very carefully add in the dimensions for an array typedef
 *		declaration.  Note that declarator has already allocated the
 *		declarator-specific dimensions, now we allocate the typedef
 *		dimensions and adjust the size of the declarator's dimensions.
 *		Note that this must be done before the dalloc for the structure,
 *		otherwise we would mix up array and structure sizes.
 */
addtdtype(tddp,type,dp,ssp)			/* returns type*/
struct symbol *tddp;
int type;
int dp;
int *ssp;
{
	register int ntype, t, i, tdf;

	for( tdf = 0, t = tddp->s_type; suptype(t); t = delsp(t) )
		if( array(t) ) {
			tdf++;
			break;
		}
	ntype = 0;
	for( t = type, i = dp; suptype(t); t = delsp(t) ) {
		ntype = addsp(ntype,t);
		if( tdf && array(t) )
			dtab[i++] =* dtab[tddp->s_dp];
	}
	for( t = tddp->s_type, i = tddp->s_dp; suptype(t); t = delsp(t) )
		if( array(t) )
			dalloc(dtab[i++]);
	for( t = tddp->s_type; suptype(ntype); ntype = delsp(ntype) )
		t = addsp(t,ntype);
	if( (ntype=btype(t)) == STRUCT )
		*ssp = tddp->s_ssp;
	else if( ntype == FRSTRUCT )
		*ssp = frstp;
	return(t);
}
