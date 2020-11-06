#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include "parser.h"
int inittype;
int begseq;
int bol 1;
int onepass;

/*
	This interfaces the Parser and the Code Generator, note that these
	allow you to link together the Parser and the Code Generator.
*/

/* outbdata - set up for byte data*/
outbdata()									/* returns - none*/
{
	inittype = CHAR;
	printf("\t.dc.b ");
}

/* outc - output a constant*/
outc(type,value)							/* returns - none*/
int type;
int value;
{
	if( type == CHAR )
		outbdata();
	else
		outwdata();
	printf("%d\n",value);
}

/* outwdata - set up for word data*/
outwdata()									/* returns - none*/
{
	inittype = INT;
	printf("\t.dc.w ");
}

/* outtext - change to text segment*/
outtext()								/* returns - none*/
{
	printf("\t.text\n");
}

/* outdata - set up for data output*/
outdata()								/* returns - none*/
{
	inittype = INT;
	printf("\t.data\n");
}

/* outldata - set up for long data output*/
outldata()								/* returns - none*/
{
	inittype = LONG;
	printf("\t.data\n");
}

/* outbss - change segment to bss*/
outbss()								/* returns - none*/
{
	printf("\t.bss\n");
}

/* outextdef - output global symbol reference*/
outextdef(sym)							/* returns - none*/
char *sym;								/* symbol name*/
{
	printf("\t.globl _%.8s\n",sym);
}

/* outcommon - output common reference*/
outcommon(sym,size)					/* returns - none*/
char *sym;							/* common symbol name*/
int size;							/* common size in bytes*/
{
	printf("\t.comm _%.8s,%ld\n",sym,((long)size & 0xffffL));
}

/* outresmem - outputs reserved memory*/
outresmem(size)						/* returns - none*/
int size;							/* size in bytes*/
{
	printf("\t.ds.b %d\n",size);
}

/* outpad - output padding for word allignment*/
outpad()							/* returns - none*/
{
	printf("\t.even\n");
}

/* outbentry - outputs block/function entry code*/
outbentry(nlocs,nds,nas)			/* returns - none*/
int nlocs;							/* local size*/
int nds;							/* number of D registers*/
int nas;							/* number of A registers*/
{
	if( nds == 0 && nas == 0 )		/* adjust for 1 arg*/
		nlocs =+ 4;
	printf("\tlink R14,#%d\n",-nlocs);
	if( nds || nas ) {
		printf("\tmovem.l R%d-R7",7-nds);	/*7 for one arg*/
		if( nas ) {
			putchar('/');
			printf("R%d-R13",14-nas);
		}
		printf(",-(sp)\n");
	}
}

/* outbexit - output function exit code*/
outbexit(nds,nas)						/* returns - none*/
int nds;								/* number of D registers*/
int nas;								/* number of A registers*/
{
	if( nds || nas ) {
		printf("\ttst.l (sp)+\n\tmovem.l (sp)+,");	/*1 arg stuff*/
		if( nds ) {
			printf("R%d-R7",8-nds);
			if( nas )
				putchar('/');
		}
		if( nas )
			printf("R%d-R13",14-nas);
		putchar('\n');
	}
	printf("\tunlk R14\n\trts\n");
}

/* outlocal - output local symbol for debugger*/
outlocal(type,sc,sym,val)
int type;								/* local name type*/
int sc;									/* storage type*/
char *sym;								/* symbol name*/
int val;
{
	switch( sc ) {

	case STATIC:
		if( notfunction(type) )
			printf("\t~%.8s=L%d\n",sym,val);
		break;

	case REGISTER:
		printf("\t~%.8s=R%d\n",sym,val);
		break;

	case AUTO:
		printf("\t~%.8s=%d\n",sym,val);
		break;
	}
}

/* outswitch - output switch table info*/
outswitch(ncases,deflab,sp)		/* returns - none*/
int ncases;							/* number of cases in switch*/
int deflab;								/* default label*/
struct swtch *sp;						/* switch table pointer*/
{
	register int vdif, val, hval, i, tlab;
	register struct swtch *s;

	val = sp->sw_value;
	hval = sp[ncases-1].sw_value;
	vdif = hval - val;
	if( ncases <= 4 ) {

	/*simple switch, do compares and brances, followed by branch to default*/

		for( s = sp; --ncases >= 0; s++ ) {
			if( s->sw_value == 0 )
				printf("\ttst R0\n");
			else
				printf("\tcmp #%d,R0\n",s->sw_value);
			printf("\tbeq L%d\n",s->sw_label);
		}
		outgoto(deflab);
	}
	else if( vdif > 0 && vdif <= ncases*3 ) {

	/*jump switch, uses value in R0 to index into table of labels*/

		if( val )
			printf("\tsub #%d,R0\n",val);
		tlab = nextlabel++;
		printf("\tcmp #%d,R0\n\tbhi L%d\n",vdif,deflab);	/*check for max*/
		printf("\tasl #2,R0\n\tmove R0,R8\n\tadd.l #L%d,R8\n",tlab);
		printf("\tmove.l (R8),R8\n\tjmp (R8)\n");
		outdata();
		outlab(tlab);
		for( s = sp; val <= hval; val++ ) {
			if( val == s->sw_value ) {
				outclab(s->sw_label);
				s++;
			}
			else
				outclab(deflab);
		}
		outtext();
	}
	else {

	/*direct switch, searches down table of values for match, if match*/
	/*found, branches to corresponding label in label table.*/

		tlab = nextlabel++;
		printf("\text.l R0\n\tmove.l #L%d,R8\n\tmove #%d,R1\n",tlab,ncases);
		i = nextlabel++;
		outlab(i);					/*loop label*/
		printf("\tcmp.l (R8)+,R0\n\tdbeq R1,L%d\n",i);
		printf("\tmove.l %d(R8),R8\n\tjmp (R8)\n",ncases*4);
		outdata();
		outlab(tlab);
		for( s = sp, i = ncases; --i >= 0; s++ )
			outlcon(s->sw_value);
		outlcon(0);						/* mark for default label*/
		for( s = sp, i = ncases; --i >= 0; s++ )
			outclab(s->sw_label);
		outclab(deflab);
		outtext();
	}
}

/* outlcon - output long constant to assembler*/
outlcon(val)							/* returns - none*/
int val;								/* word value*/
{
	printf("\t.dc.l %d\n",val);
}

/* outclab - output label consant*/
outclab(lab)							/* returns - none*/
int lab;								/* label*/
{
	printf("\t.dc.l L%d\n",lab);
}

/* outlab - output a label*/
outlab(lab)								/* returns - none*/
int lab;								/* label*/
{
	printf("\tL%d:",lab);
}

/* outflab - output function label*/
outflab(sym)							/* returns - none*/
char *sym;								/* function name*/
{
	printf("\t_%.8s:\n\t~~%.8s:\n",sym,sym);
}

/* outdlab - output data label*/
outdlab(sym)							/* returns - none*/
char *sym;								/* data symbol name*/
{
	printf("\t_%.8s:\n",sym);
}

outeof()
{
	register int c;

	fflush(&sbuf);
	fflush(&obuf);
}

/* copysfile - copy string file to end of output file*/
copysfile(fname)
char *fname;
{
	register int c;

	close(sbuf.io_fd);
	if( fopen(fname,&sbuf,0) < 0 )
		ferror("can't copy %s",fname);
	while( (c=getc(&sbuf)) > 0 )
		putc(c,&obuf);
	fflush(&obuf);
}

/* outword - output a word of data*/
outword(w)								/* word expression*/
int w;
{
	if( begseq )
		putchar(',');
	begseq++;
	printf("%d",w);
}

/* outlong - output a long data*/
outlong(l)								/* returns - none*/
long l;									/* long data to output*/
{
	outwdata();
	outword(l.hiword);
	outword(l.loword);
	outendseq();
}

outendseq()								/* returns - none*/
{
	begseq = 0;
	putchar('\n');
}

outgoto(lab)
int lab;
{
	if( lab > 0 )
		printf("\tbra L%d\n",lab);
}

/* outtstr - output text string*/
/*		This outputs a string to the string file, this is used wherever*/
/*		you cannot output the string directly to data space, such as in*/
/*		the middle of expressions.*/
outtstr(lab)
int lab;
{
	char *savep;
	int sbol;

	savep = obp;					/*save to restore later...*/
	obp = &sbuf;
	sbol = bol;
	bol = 1;
	printf("\tL%d:",lab);
	outstr();
	obp = savep;
	bol = sbol;
}

/* outstr - output a string as a sequence of bytes*/
/*		Outputs ".dc.b <byte1>,<byte2>,...,<0>*/
outstr()
{
	register char *s;
	register int i;

	outbdata();
	for( s = cstr, i = cstrsize; i > 0; i-- )
		outword(*s++ & 0xff);
	outendseq();
}

/* putchar - handle outputting to intermediate or error files*/
/*		This catches tabs to allow for the integration of the parser*/
/*		and code generator into one pass.  By merely throwing away the*/
/*		tabs here, the output will be OK for the assembler.*/
putchar(c)
char c;
{
	if( obp == 0 )
		write(1,&c,1);
	else if( c == '\t' ) {
		if( bol && onepass == 0 )
			putc('(',obp);					/*for code generator*/
	}
	else {
		bol = (c == '\n');
		putc(c,obp);
	}
}
