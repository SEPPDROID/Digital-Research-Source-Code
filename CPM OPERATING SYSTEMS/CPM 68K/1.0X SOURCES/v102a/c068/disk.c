/**
 *	Copyright 1983
 *	Alcyon Corporation
 *	8716 Production Ave.
 *	San Diego, Ca.  92121
 *
 *	@(#)disk.c	1.2	11/3/83
**/

/**
 *	These routines replace the symbol handling routines in symt.c in
 *	the case that the machine which this compiler is running on does
 *	not have enough memory to store the symbol table.  These routines
 *	build and manage an on disk symbol table
 *
 *	Symbol Table entry allocation and lookup routines
**/

#ifdef SYM_TO_DSK
#include "parser.h"
#ifndef MC68000  /*sw Regulus?? */
#	include <c68/order.h>
#else
#	include <order.h>
#endif

#define SYM_SIZ 36 				/*sizeof(struct symbol)*/
#define DBUFSIZ 28 * SYM_SIZ	/* max # of symbols storable in under 1K */
#define	STEL	HSIZE/2

char dbuf[DBUFSIZ];
long data_addr = 0;			/*location dbuf[0] represents*/
long symtab[HSIZE];			/*hash table*/
short dirty_buf;			/*dirty buffer??, buffer altered??*/

/**
 * install - install a symbol in the symbol table
 *		Allocates a symbol entry, copies info into it and links it
 *		into the hash table chain.
**/
char *
install(sym,attrib,offset)			/* returns address of csp_entry */
char *sym;							/* symbol to install*/
int attrib;							/* attribues of symbol*/
int offset;							/* symbol offset (resword value)*/
{
	register struct symbol *sp;
	register short i;
	register long item_offset;	/* location for new item */
#ifdef DEBUG
	struct symbol par_entry;
#endif

	item_offset = dsk_offset;
	dsk_offset += SYM_SIZ;

	sp = &csp_entry;
	sp->s_sc = sp->s_type = sp->s_dp = sp->s_ssp = 0;
	sp->s_offset = offset;
	sp->s_attrib = attrib;

	if (in_struct) {
		sp->s_par = struc_parent[in_struct];
		hold_sib = struc_sib[in_struct];
		sp->s_scope = (infunc) ? FUNC_SCOPE : GLOB_SCOPE;	        /* [vlh] 4.2 */
		
#ifdef DEBUG
		read_st(&par_entry,sp->s_par);
		if (symdebug)
			printf("    struct element parent is %.8s\n", par_entry.s_symbol);
#endif
		if (struc_sib[in_struct]) {
			read_st(&volatile,struc_sib[in_struct]);
			volatile.s_sib = item_offset;
			write_st(&volatile,struc_sib[in_struct]);
#ifdef DEBUG
		if (symdebug) {
			printf("    sparent sib is %.8s\n",volatile.s_symbol);
			printf("    sparent addr %ld ",struc_sib[in_struct]);
			printf("child addr %ld\n",item_offset);
		}
#endif
		}
		else {
			read_st(&volatile,struc_parent[in_struct]);
			volatile.s_child = item_offset;
			write_st(&volatile,struc_parent[in_struct]);
#ifdef DEBUG
			if (symdebug)
				printf("    child of %.8s\n",volatile.s_symbol);
#endif
		}
		struc_sib[in_struct] = item_offset;
		sp->s_child = sp->s_sib = 0;
	}
	else {
		sp->s_sib = sp->s_child = sp->s_par = 0;
		sp->s_scope = scope_level;			/* [vlh] 4.2 */
	}
	symcopy(sym,sp->s_symbol);			/*copy symbol to symbol struct*/
	i = symhash(sym,in_struct|smember);	/*link into chain list*/
	sp->s_next = symtab[i];
	symtab[i] = item_offset;
	write_st(sp,item_offset);
	csp_addr = item_offset;
	return(sp);
}

/**
 * lookup - looks up a symbol in symbol table
 *		Hashes symbol, then goes thru chain, if not found, then
 *		installs the symbol.
**/
char *
lookup(sym,force)			/* returns pointer to symbol buffer*/
char *sym;					/* pointer to symbol*/
int force;					/* [vlh] 4.2 force entry in symbol table */
{
	register struct symbol *sp;
	register char *p;
	short exact, prev_level;		/* same name, diff type or offset */
	long next_sym, hold;

	p = sym; prev_level = 0; sp = &csp_entry;
	if ((next_sym = symtab[symhash(p,0)]) != 0) {
		 while (next_sym != 0) {
			read_st(&csp_entry,next_sym);
			if((sp->s_attrib&(SRESWORD|STYPEDEF)) && symequal(p,sp->s_symbol)) {
				csp_addr = next_sym;
				return(&csp_entry);
			}
			next_sym = sp->s_next;
		}
	}
	hold = 0;
	if (!(smember|in_struct)) {	/*[vlh]*/
		if ((next_sym = symtab[symhash(p,0)]) != 0) {
			while (next_sym != 0) {
				read_st(&csp_entry,next_sym);
				if(symequal(p,sp->s_symbol)) {
					if (scope_level == sp->s_scope) {
						csp_addr = next_sym;
						return(&csp_entry); /* perfect scope match */
					}
					else if (!force && prev_level <= sp->s_scope) {
						hold = next_sym;
						prev_level = sp->s_scope;
					}
				}
				next_sym = sp->s_next;
			}
			if ( hold ) {	/* [vlh] 4.2 added scope... */
				csp_addr = hold;
				read_st(&csp_entry,csp_addr);
				return(&csp_entry);
			}
		}
	}
	else {	/* doing a declaration or an expression */
		exact = 0;
		if ((next_sym = symtab[symhash(p,in_struct|smember)]) != 0) {;
			while (next_sym != 0) {
				read_st(&csp_entry,next_sym);
				if( symequal(p,sp->s_symbol) ) {
					if (symsame(sp,hold,&exact)) {
						csp_addr = next_sym;
						return(&csp_entry);
					}
					else if (!hold && !exact) 
						hold = next_sym;
				}
				next_sym = sp->s_next;
			}
			if (hold && (instmt || in_struct==0 || smember!=0)) { /*4.1*/
				csp_addr = hold;
				read_st(&csp_entry,csp_addr);
				return(&csp_entry);
			}
		}
	}
#ifdef DEBUG
	if (symdebug) printf("installing [%.8s]\n",p);
#endif
	return(install(p,0,0));
}

/**
 * freesyms - frees all local symbols at end of function declaration
 *		Searches thru symbol table, deleting all symbols marked as locals
**/
freesyms(level)							/* returns - none*/
int level;								/* [vlh] 4.2 scope levels... */
{
	register short i;
	register struct symbol *sp;
	register long *lp;
	register long hold, current, next_sym;
	struct symbol sp_entry;

	sp = &sp_entry;
	for(lp = &symtab[0], i = 0; i != HSIZE; lp++, i++) {
		current = *lp;
		for (hold = 0; current != 0; current = next_sym) {
			read_st(sp,current);
			next_sym = sp->s_next;
			if (level == FUNC_SCOPE)
				if( !(sp->s_attrib&SDEFINED) ) {
					error("undefined label: %.8s",sp->s_symbol);
					sp->s_attrib |= SDEFINED;
				}
			if ((sp->s_attrib & (SGLOBAL|SRESWORD)) || (sp->s_scope < level))
				hold = current;	/* should do a break !!! ??? */
			else {
#ifdef DEBUG
	if (symdebug) {
		printf("freeing %s, level %d, next %ld ",sp->s_symbol,level,sp->s_next);
		printf("location %ld\n",current);
	}
#endif
				if( hold ) {		/* should never get this case !!! ??? */
					read_st(&volatile,hold);
					volatile.s_next = sp->s_next;
					write_st(&volatile,hold);
				}
				else
					*lp = sp->s_next;
			}
		}
	}
}

/**
 * chksyms - checks symbol table for undefined symbols, etc.
 *		Goes thru the symbol table checking for undeclared forward
 *		referenced structures, and outputs local symbols for debugger.
**/
chksyms(ok)							/* returns - none*/
int ok;
{
	struct symbol item;
	register struct symbol *sp;
	register short sc;
	register long next_sym;

	next_sym = 0; sp = &item;
	while(next_sym < dsk_offset) {
		read_st(sp,next_sym);
		sc = sp->s_sc;
		if(sc!=0 && sp->s_ssp>=0 && (BTYPE(sp->s_type))==FRSTRUCT) {
			read_st(&volatile,frstab[sp->s_ssp]);
			sp->s_par = volatile.s_par;
			sp->s_ssp = volatile.s_ssp;	/* 3.4 ssp>0 */
			sp->s_type = (sp->s_type&~TYPE) | STRUCT;
		}
		if( sc == PDECLIST ) {
			error("not in parameter list: %.8s",sp->s_symbol);
			sp->s_sc = AUTO;
			if (ok) 
				outlocal(sp->s_type,sc,sp->s_symbol,sp->s_offset);
		}
		write_st(sp,next_sym);
		next_sym += SYM_SIZ;
	}
}

/* read symbol table item from disk into structure */
read_st(sp,d_offset)
register struct symbol *sp;
long d_offset;
{
	register char *p, *s;
	register short i;

	if ((d_offset >= (data_addr+DBUFSIZ)) || (d_offset < data_addr))
		reset_dbuf(d_offset);
	p = &dbuf[d_offset - data_addr];
	sp->s_attrib = *p++;
	sp->s_sc = *p++;
	sp->s_type.wb1 = *p++; sp->s_type.wb2 = *p++;
	sp->s_dp.wb1 = *p++; sp->s_dp.wb2 = *p++;
	sp->s_ssp.wb1 = *p++; sp->s_ssp.wb2 = *p++;
	sp->s_offset.wb1 = *p++; sp->s_offset.wb2 = *p++;
	sp->s_scope.wb1 = *p++; sp->s_scope.wb2 = *p++;
	for(i=0, s = sp->s_symbol; i < SSIZE; i++)
		*s++ = *p++;
	sp->s_par.b1 = *p++; sp->s_par.b2 = *p++; 
	sp->s_par.b3 = *p++; sp->s_par.b4 = *p++;
	sp->s_child.b1 = *p++; sp->s_child.b2 = *p++; 
	sp->s_child.b3 = *p++; sp->s_child.b4 = *p++;
	sp->s_sib.b1 = *p++; sp->s_sib.b2 = *p++; 
	sp->s_sib.b3 = *p++; sp->s_sib.b4 = *p++;
	sp->s_next.b1 = *p++; sp->s_next.b2 = *p++; 
	sp->s_next.b3 = *p++; sp->s_next.b4 = *p; 
}

/* write symbol table item to disk */
write_st(sp,d_offset)
register struct symbol *sp;
long d_offset;
{
	register char *p, *s;
	register short i;

	dirty_buf = 1;
	if ((d_offset >= (data_addr+DBUFSIZ)) || (d_offset < data_addr))
		reset_dbuf(d_offset);
	p = &dbuf[d_offset - data_addr];
	*p++ = sp->s_attrib;
	*p++ = sp->s_sc;
	*p++ = (sp->s_type).wb1; *p++ = (sp->s_type).wb2;
	*p++ = (sp->s_dp).wb1; *p++ = (sp->s_dp).wb2;
	*p++ = (sp->s_ssp).wb1; *p++ = (sp->s_ssp).wb2;
	*p++ = (sp->s_offset).wb1; *p++ = (sp->s_offset).wb2;
	*p++ = (sp->s_scope).wb1; *p++ = (sp->s_scope).wb2;
	for(i=0, s = sp->s_symbol; i < SSIZE; i++)
		*p++ = *s++;
	*p++ = (sp->s_par).b1; *p++ = (sp->s_par).b2; 
	*p++ = (sp->s_par).b3; *p++ = (sp->s_par).b4;
	*p++ = (sp->s_child).b1; *p++ = (sp->s_child).b2;
	*p++ = (sp->s_child).b3; *p++ = (sp->s_child).b4;
	*p++ = (sp->s_sib).b1; *p++ = (sp->s_sib).b2;
	*p++ = (sp->s_sib).b3; *p++ = (sp->s_sib).b4;
	*p++ = (sp->s_next).b1; *p++ = (sp->s_next).b2;
	*p++ = (sp->s_next).b3; *p   = (sp->s_next).b4;
}

reset_dbuf(dsk_addr)
long dsk_addr;			/* addr of next symbol to read or write */
{
	if(dirty_buf) {		/* dirty buffer...... */
		dirty_buf = 1;
		lseek(dsk_fd,data_addr,0);
		write(dsk_fd,&dbuf,DBUFSIZ);
		close(dsk_fd);
		dsk_fd = open(dskfile,2);
	}
	lseek(dsk_fd,dsk_addr,0);
	read(dsk_fd,&dbuf,DBUFSIZ);
	data_addr = dsk_addr;
}
#endif
