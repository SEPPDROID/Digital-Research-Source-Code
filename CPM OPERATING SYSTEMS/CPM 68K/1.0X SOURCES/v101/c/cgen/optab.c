#
/*
	Copyright 1981
	Alcyon Corporation
	8474 Commerce Av.
	San Diego, Ca.  92121
*/

#include "cgen.h"

#define	I_NULL	0
#define	I_ADD	1
#define	I_INC	2
#define	I_SUB	3
#define	I_DEC	4
#define	I_MULS	5
#define	I_MULU	6
#define	I_DIVS	7
#define	I_DIVU	8
#define	I_ASR	9
#define	I_LSR	10
#define	I_ASL	11
#define	I_LSL	12
#define	I_AND	13
#define	I_OR	14
#define	I_EOR	15
#define	I_NEG	16
#define	I_NOT	17
#define	I_MOVE	18
#define	I_CLR	19
#define	I_CMP	20
#define	I_TST	21
#define	I_LMUL	22
#define	I_LDIV	23
#define	I_LREM	24
#define	I_LEML	25
#define	I_LEDV	26
#define	I_LERM	27
#define	I_BEQ	28
#define	I_BNE	29
#define	I_BGT	30
#define	I_BGE	31
#define	I_BLT	32
#define	I_BLE	33
#define	I_BLS	34
#define	I_BLO	35
#define	I_BCC	36
#define	I_BHI	37
#define	I_BRA	38
#define	I_NOP	39
#define	I_BTST	40

char *mnemonics[] {
	"",
	"add",
	"inc",
	"sub",
	"dec",
	"muls",
	"mulu",
	"divs",
	"divu",
	"asr",
	"lsr",
	"asl",
	"lsl",
	"and",
	"or",
	"eor",
	"neg",
	"not",
	"move",
	"clr",
	"cmp",
	"tst",
	"lmul",
	"_ldiv",
	"lrem",
	"almul",
	"aldiv",
	"alrem",
	"beq",
	"bne",
	"bgt",
	"bge",
	"blt",
	"ble",
	"bls",
	"blo",
	"bcc",
	"bhi",
	"jmp",
	"*nop",
	"btst",
};

#define	FE_EQOP		1
#define	FE_ASSIGN	2
#define	FE_EQSHFT	3
#define	FE_EQXOR	4
#define	FE_EQADDR	5
#define	FC_FIX		6
#define	FC_REL		7
#define	FC_BTST		8
#define	FS_OP		9
#define	FS_ITL		10
#define	FS_LD		11
#define	FR_ADD		12
#define	FR_MULT		13
#define	FR_DIV		14
#define	FR_SHFT		15
#define	FR_XOR		16
#define	FR_NEG		17
#define	FR_EQOP		18
#define	FR_POSTOP	19
#define	FR_ASSIGN	20
#define	FR_EQMULT	21
#define	FR_EQDIV	22
#define	FR_EQSHFT	23
#define	FR_EQXOR	24
#define	FR_CALL		25
#define	FR_ITL		26
#define	FR_LTI		27
#define	FR_LD		28
#define	FR_LEMUL	29
#define	FR_EQADDR	30
/*
 * You left these out, Bill.
 */
char fe_eqop[], fe_assign[], fe_eqshft[], fe_eqxor[], fe_eqaddr[];
char fc_fix[], fc_rel[], fc_btst[];
char fs_op[], fs_itl[], fs_ld[];
char fr_op[], fr_mult[], fr_div[], fr_shft[], fr_xor[], fr_neg[],
     fr_eqop[], fr_postop[], fr_assign[], fr_eqmult[], fr_eqdiv[],
     fr_eqshft[], fr_eqxor[], fr_call[], fr_itl[], fr_lti[],
     fr_ld[], fr_lemul[], fr_eqaddr[];

char *codeskels[] {
	0,
	fe_eqop,
	fe_assign,
	fe_eqshft,
	fe_eqxor,
	fe_eqaddr,
	fc_fix,
	fc_rel,
	fc_btst,
	fs_op,
	fs_itl,
	fs_ld,
	fr_op,
	fr_mult,
	fr_div,
	fr_shft,
	fr_xor,
	fr_neg,
	fr_eqop,
	fr_postop,
	fr_assign,
	fr_eqmult,
	fr_eqdiv,
	fr_eqshft,
	fr_eqxor,
	fr_call,
	fr_itl,
	fr_lti,
	fr_ld,
	fr_lemul,
	fr_eqaddr,
};

	/*This is the major table directing the code generation process.*/
	/*It is indexed by an O_op operator, which is obtained from the*/
	/*opinfo table for an intermediate code operator.  The actual*/
	/*code skeleton macros are in cskels.c, which are in a linked*/
	/*list in order of decreasing order of difficulty.*/
char optab[][6] {

/*	I		I2		effect		cc's	stack	register*/

	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	I_NULL,		/*NULL*/
	I_ADD,	I_INC,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*ADD*/
	I_SUB,	I_DEC,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*SUB*/
	I_MULS,	I_MULU,	I_NULL,		FC_FIX,	I_NULL,	FR_MULT,	/*MULT*/
	I_DIVS,	I_DIVU,	I_NULL,		FC_FIX,	I_NULL,	FR_DIV,		/*DIV*/
	I_DIVS,	I_DIVU,	I_NULL,		I_NULL,	I_NULL,	FR_DIV,		/*MOD*/
	I_ASR,	I_LSR,	I_NULL,		FC_FIX,	I_NULL,	FR_SHFT,	/*RSH*/
	I_ASL,	I_LSL,	I_NULL,		FC_FIX,	I_NULL,	FR_SHFT,	/*LSH*/
	I_AND,	I_AND,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*AND*/
	I_OR,	I_OR,	I_NULL,		FC_FIX,	FS_OP,	FR_ADD,		/*OR*/
	I_EOR,	I_EOR,	I_NULL,		FC_FIX,	I_NULL,	FR_XOR,		/*XOR*/
	I_NULL,	I_NULL,	I_NULL,		FC_FIX,	I_NULL,	I_NULL,		/*NOT*/
	I_NEG,	I_NEG,	I_NULL,		FC_FIX,	I_NULL,	FR_NEG,		/*NEG*/
	I_NOT,	I_NOT,	I_NULL,		I_NULL,	I_NULL,	FR_NEG,		/*COMPL*/
	I_SUB,	I_DEC,	FE_EQOP,	FC_FIX,	I_NULL,	FR_EQOP,	/*PREDEC*/
	I_ADD,	I_INC,	FE_EQOP,	FC_FIX,	I_NULL,	FR_EQOP,	/*PREINC*/
	I_SUB,	I_DEC,	FE_EQOP,	I_NULL,	I_NULL,	FR_POSTOP,	/*POSTDEC*/
	I_ADD,	I_INC,	FE_EQOP,	I_NULL,	I_NULL,	FR_POSTOP,	/*POSTINC*/
	I_MOVE,	I_CLR,	FE_ASSIGN,	I_NULL,	I_NULL,	FR_ASSIGN,	/*ASSIGN*/
	I_ADD,	I_INC,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*EQADD*/
	I_SUB,	I_DEC,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*EQSUB*/
	I_MULS,	I_MULU,	I_NULL,		FC_FIX,	I_NULL,	FR_EQMULT,	/*EQMULT*/
	I_DIVS,	I_DIVU,	I_NULL,		FC_FIX,	I_NULL,	FR_EQDIV,	/*EQDIV*/
	I_DIVS,	I_DIVU,	I_NULL,		I_NULL,	I_NULL,	FR_EQDIV,	/*EQMOD*/
	I_ASR,	I_LSR,	FE_EQSHFT,	I_NULL,	I_NULL,	FR_EQSHFT,	/*EQRSH*/
	I_ASL,	I_LSL,	FE_EQSHFT,	I_NULL,	I_NULL,	FR_EQSHFT,	/*EQLSH*/
	I_AND,	I_AND,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*EQAND*/
	I_OR,	I_OR,	FE_EQOP,	I_NULL,	I_NULL,	FR_EQOP,	/*EQOR*/
	I_EOR,	I_EOR,	FE_EQXOR,	FC_FIX,	I_NULL,	FR_EQXOR,	/*EQXOR*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_CALL,	/*FJSR*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*EQUALS*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*NEQUALS*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*GREAT*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*GREATEQ*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*LESS*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	I_NULL,	I_NULL,		/*LESSEQ*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	FS_ITL,	FR_ITL,		/*INT2L*/
	I_NULL,	I_NULL,	I_NULL,		I_NULL,	I_NULL,	FR_LTI,		/*LONG2I*/
	I_BTST,	I_BTST,	I_NULL,		FC_BTST,	I_NULL,	I_NULL,	/*BTST*/
	I_CMP,	I_TST,	I_NULL,		FC_REL,	FS_LD,	FR_LD,		/*LOAD*/
	I_MULS,	I_LMUL,	I_NULL,		I_NULL,	I_NULL,	FR_MULT,	/*LMULT*/
	I_DIVS,	I_LDIV,	I_NULL,		I_NULL,	I_NULL,	FR_DIV,		/*LDIV*/
	I_DIVS,	I_LREM,	I_NULL,		I_NULL,	I_NULL,	FR_DIV,		/*LMOD*/
	I_LEML,	I_LEML,	I_NULL,		I_NULL,	I_NULL,	FR_LEMUL,	/*LEQMULT*/
	I_LEDV,	I_LEDV,	I_NULL,		I_NULL,	I_NULL,	FR_LEMUL,	/*LEQDIV*/
	I_LERM,	I_LERM,	I_NULL,		I_NULL,	I_NULL,	FR_LEMUL,	/*LEQMOD*/
	I_NULL,	I_NULL,	FE_EQADDR,	I_NULL,	I_NULL,	FR_EQADDR,	/*EQADDR*/
};


	/*this maps comparison operators and comparison types into the*/
	/*actual branch opcode used.*/
char brtab[][2] {
	I_BEQ,	I_BEQ,					/*EQUALS*/
	I_BNE,	I_BNE,					/*NEQUALS*/
	I_BGT,	I_BHI,					/*GREAT*/
	I_BGE,	I_BCC,					/*GREATEQ*/
	I_BLT,	I_BLO,					/*LESS*/
	I_BLE,	I_BLS,					/*LESSEQ*/
};

	/*turns !x>y into x<=y*/
int invrel[] { NEQUALS, EQUALS, LESSEQ, LESS, GREATEQ, GREAT };

	/*turns x>y into y<=x*/
int swaprel[] { EQUALS, NEQUALS, LESS, LESSEQ, GREAT, GREATEQ };

	/*code skeleton built-in strings*/
char *strtab[] {
	"move",							/*MOV*/
	"move.l",						/*MOVL*/
	"jsr",							/*JSR*/
	"clr",							/*CLR*/
	"clr.l",						/*CLRL*/
	"ext.w",						/*EXTW*/
	"ext.l",						/*EXTL*/
	"lea",							/*LEA*/
	"(sp)",							/*STK*/
};
