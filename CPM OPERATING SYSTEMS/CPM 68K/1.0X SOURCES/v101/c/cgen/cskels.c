#include "cskel.h"
char ctnull[] {0};
char cteqa01[] {
	LEA,' ',RADDR,',',LADDR,'\n',
	MOVL,' ',LADDR,',',CR,'\n',
	0 };
char cteqa02[] {
	LEA,' ',RADDR,',',CAR,'\n',
	MOVL,' ',CAR,',',LADDR,'\n',
	MOVL,' ',CAR,',',CR,'\n',
	0 };
char cteqa03[] {
	LEFT,S_NEXT|S_INDR,
	LEA,' ',RADDR,',',CAR,'\n',
	MOVL,' ',CAR,',',LOFFSET,'(',NAR,')','\n',
	MOVL,' ',CAR,',',CR,'\n',
	0 };
struct skeleton fr_eqaddr[] {
	SU_AREG|T_LONG,	SU_ANY|T_ANY,	cteqa01,
	SU_AREG|T_LONG,	SU_ANY|T_LONG,	cteqa01,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	cteqa02,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	cteqa02,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	cteqa03,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	cteqa03,
	0,
};
char cteqa04[] {
	LEA,' ',RADDR,',',LADDR,'\n',
	0 };
char cteqa05[] {
	LEA,' ',RADDR,',',CAR,'\n',
	MOVL,' ',CAR,',',LADDR,'\n',
	0 };
char cteqa06[] {
	LEFT,S_NEXT|S_INDR,
	LEA,' ',RADDR,',',CAR,'\n',
	MOVL,' ',CAR,',',LOFFSET,'(',NAR,')','\n',
	0 };
struct skeleton fe_eqaddr[] {
	SU_AREG|T_LONG,	SU_ANY|T_ANY,	cteqa04,
	SU_AREG|T_LONG,	SU_ANY|T_LONG,	cteqa04,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	cteqa05,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	cteqa05,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	cteqa06,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	cteqa06,
	0,
};
char ctcal01[] {
	JSR,' ',LADDR,'\n',
	0 };
char ctcal02[] {
	LEFT,S_INDR,
	JSR,' ',LOFFSET,'(',CAR,')','\n',
	0 };
char ctcal03[] {
	LEFT,0,
	MOV,' ',CR,',',CAR,'\n',
	JSR,' ','(',CAR,')','\n',
	0 };
struct skeleton fr_call[] {
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctcal01,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctcal01,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctcal02,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctcal02,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctcal03,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctcal03,
	0,
};
char ctlod01[] {
	CLR,' ',CR,'\n',
	0 };
char ctlod02[] {
	MOV,TLEFT,' ',LADDR,',',CR,'\n',
	EXTW,' ',CR,'\n',
	0 };
char ctlod03[] {
	MOV,TLEFT,' ',LADDR,',',CR,'\n',
	0 };
char ctlod04[] {
	LEFT,S_INDR,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	EXTW,' ',CR,'\n',
	0 };
char ctlod05[] {
	LEFT,S_INDR,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	0 };
struct skeleton fr_ld[] {
	SU_ZERO|T_ANY,	SU_ANY|T_ANY,	ctlod01,
	SU_ADDR|T_CHAR,	SU_ANY|T_ANY,	ctlod02,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctlod03,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctlod03,
	SU_ANY|T_CHAR|T_INDR,	SU_ANY|T_ANY,	ctlod04,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctlod05,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctlod05,
	0,
};
char ctlod06[] {
	CLR,TLEFT,' ',PSH,'\n',
	0 };
char ctlod07[] {
	LEFT,0,
	MOV,' ',CR,',',PSH,'\n',
	0 };
char ctlod08[] {
	MOV,TLEFT,' ',LADDR,',',PSH,'\n',
	0 };
char ctlod09[] {
	LEFT,0,
	MOV,TLEFTL,' ',CR,',',PSH,'\n',
	0 };
struct skeleton fs_ld[] {
	SU_ZERO|T_ANY,	SU_ANY|T_ANY,	ctlod06,
	SU_ADDR|T_CHAR,	SU_ANY|T_ANY,	ctlod07,
	SU_ADDR|T_INT,	SU_ANY|T_ANY,	ctlod08,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctlod08,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctlod09,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctlod09,
	0,
};
char ctpst01[] {
	MOV,TLEFT,' ',LADDR,',',CR,'\n',
	OP,TLEFT,' ',RADDR,',',LADDR,'\n',
	0 };
char ctpst02[] {
	LEFT,S_INDR,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	OP,TLEFT,' ',RADDR,',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fr_postop[] {
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctpst01,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctpst01,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctpst02,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctpst02,
	0,
};
char ctneg01[] {
	LEFT,0,
	OP,TLEFT,' ',CR,'\n',
	0 };
struct skeleton fr_neg[] {
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctneg01,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctneg01,
	0,
};
char ctasg01[] {
	CLRL,' ',CR,'\n',
	MOV,' ',RADDR,',',CR,'\n',
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctasg02[] {
	RIGHT,0,
	EXRL,
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctasg03[] {
	LEFT,S_INDR,
	CLRL,' ',CR,'\n',
	MOV,' ',RADDR,',',CR,'\n',
	OP,TLEFT,' ',LOFFSET,'(',NAR,')',',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',NAR,')','\n',
	0 };
char ctasg04[] {
	LEFT,S_INDR,
	MOV,TRIGHT,' ',RADDR,',',CR,'\n',
	EXRL,
	OP,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char ctasg05[] {
	RIGHT,0,
	EXRL,
	LEFT,S_NEXT|S_INDR,
	OP,TLEFT,' ',CR,',',LOFFSET,'(',NAR,')','\n',
	0 };
char ctasg06[] {
	LEFT,S_STACK|S_INDR,
	RIGHT,0,
	EXRL,
	MOVL,' ',POP,',',CAR,'\n',
	OP,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fr_assign[] {
	SU_ADDR|T_LONG,	SU_ADDR|T_UNSN,	ctasg01,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctasg02,
	SU_ADDR|T_ANY,	SU_ANY|T_LONG,	ctasg02,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctasg02,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	ctasg02,
	SU_ANY|T_LONG|T_INDR,	SU_ADDR|T_UNSN,	ctasg03,
	SU_ANY|T_ANY|T_INDR,	SU_ADDR|T_INT,	ctasg04,
	SU_ANY|T_ANY|T_INDR,	SU_ADDR|T_LONG,	ctasg04,
	SU_ANY|T_LONG|T_INDR,	SU_ADDR|T_INT,	ctasg04,
	SU_ANY|T_LONG|T_INDR,	SU_ADDR|T_LONG,	ctasg04,
	SU_EASY|T_ANY|T_INDR,	SU_EASY|T_ANY,	ctasg05,
	SU_EASY|T_ANY|T_INDR,	SU_EASY|T_LONG,	ctasg05,
	SU_EASY|T_LONG|T_INDR,	SU_EASY|T_LONG,	ctasg05,
	SU_EASY|T_LONG|T_INDR,	SU_EASY|T_ANY,	ctasg05,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctasg06,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_LONG,	ctasg06,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctasg06,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	ctasg06,
	0,
};
char ctasg07[] {
	AOP,TLEFT,' ',LADDR,'\n',
	0 };
char ctasg08[] {
	LEFT,S_INDR,
	AOP,TLEFT,' ',LOFFSET,'(',CAR,')','\n',
	0 };
char ctasg09[] {
	OP,TLEFT,' ',RADDR,',',LADDR,'\n',
	0 };
char ctasg10[] {
	RIGHT,0,
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctasg11[] {
	OP,TRIGHT,' ',RADDR,',',LADDR,'\n',
	EXTW,' ',LADDR,'\n',
	0 };
char ctasg12[] {
	OP,TRIGHT,' ',RADDR,',',LADDR,'\n',
	EXTW,' ',LADDR,'\n',
	EXTL,' ',LADDR,'\n',
	0 };
char ctasg13[] {
	CLRL,' ',LADDR,'\n',
	MOV,' ',RADDR,',',LADDR,'\n',
	0 };
char ctasg22[] {
	OP,TRIGHT,' ',RADDR,',',LADDR,'\n',
	0 };
char ctasg14[] {
	OP,TRIGHT,' ',RADDR,',',LADDR,'\n',
	EXTL,' ',LADDR,'\n',
	0 };
char ctasg15[] {
	MOV,' ',RADDR,',',CAR,'\n',
	OP,TLEFT,' ',CAR,',',LADDR,'\n',
	0 };
char ctasg16[] {
	LEFT,S_INDR,
	CLRL,' ',CR,'\n',
	OP,' ',RADDR,',',CR,'\n',
	OP,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char ctasg17[] {
	RIGHT,S_INDR,
	OP,TLEFT,' ',ROFFSET,'(',CAR,')',',',LADDR,'\n',
	0 };
char ctasg18[] {
	RIGHT,0,
	EXRL,
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctasg19[] {
	LEFT,S_INDR,
	OP,TLEFT,' ',RADDR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char ctasg20[] {
	LEFT,S_INDR,
	RIGHT,S_NEXT|S_INDR,
	OP,TLEFT,' ',ROFFSET,'(',NAR,')',',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fe_assign[] {
	SU_ADDR|T_ANY,	SU_ZERO|T_ANY,	ctasg07,
	SU_ADDR|T_LONG,	SU_ZERO|T_ANY,	ctasg07,
	SU_ANY|T_ANY|T_INDR,	SU_ZERO|T_ANY,	ctasg08,
	SU_ADDR|T_CHAR,	SU_ADDR|T_CHAR,	ctasg09,
	SU_ADDR|T_INT,	SU_ADDR|T_INT,	ctasg09,
	SU_ADDR|T_LONG,	SU_ADDR|T_LONG,	ctasg09,
	SU_ADDR|T_ANY,	SU_CONST|T_INT,	ctasg09,
	SU_ADDR|T_LONG,	SU_CONST|T_INT,	ctasg09,
	SU_ADDR|T_CHAR,	SU_REG|T_INT,	ctasg09,
	SU_ADDR|T_CHAR,	SU_REG|T_LONG,	ctasg09,
	SU_AREG|T_LONG,	SU_ADDR|T_CHAR,	ctasg10,
	SU_AREG|T_ANY,	SU_ADDR|T_CHAR,	ctasg10,
	SU_REG|T_INT,	SU_ADDR|T_CHAR,	ctasg11,
	SU_REG|T_LONG,	SU_ADDR|T_CHAR,	ctasg12,
	SU_REG|T_LONG,	SU_ADDR|T_UNSN,	ctasg13,
	SU_AREG|T_LONG,	SU_ADDR|T_INT,	ctasg22,
	SU_REG|T_LONG,	SU_ADDR|T_INT,	ctasg14,
	SU_ADDR|T_LONG,	SU_ADDR|T_UNSN,	ctasg01,
	SU_ADDR|T_LONG,	SU_ADDR|T_INT,	ctasg15,
	SU_ANY|T_LONG|T_INDR,	SU_ADDR|T_UNSN,	ctasg16,
	SU_ADDR|T_CHAR,	SU_ANY|T_CHAR|T_INDR,	ctasg17,
	SU_ADDR|T_INT,	SU_ANY|T_INT|T_INDR,	ctasg17,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG|T_INDR,	ctasg17,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctasg18,
	SU_ADDR|T_ANY,	SU_ANY|T_LONG,	ctasg18,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctasg18,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	ctasg18,
	SU_ANY|T_INT|T_INDR,	SU_ADDR|T_INT,	ctasg19,
	SU_ANY|T_CHAR|T_INDR,	SU_ADDR|T_CHAR,	ctasg19,
	SU_ANY|T_LONG|T_INDR,	SU_ADDR|T_LONG,	ctasg19,
	SU_ANY|T_ANY|T_INDR,	SU_CONST|T_INT,	ctasg19,
	SU_ANY|T_LONG|T_INDR,	SU_CONST|T_INT,	ctasg19,
	SU_EASY|T_CHAR|T_INDR,	SU_EASY|T_CHAR|T_INDR,	ctasg20,
	SU_EASY|T_INT|T_INDR,	SU_EASY|T_INT|T_INDR,	ctasg20,
	SU_EASY|T_LONG|T_INDR,	SU_EASY|T_LONG|T_INDR,	ctasg20,
	SU_EASY|T_ANY|T_INDR,	SU_EASY|T_ANY,	ctasg05,
	SU_EASY|T_ANY|T_INDR,	SU_EASY|T_LONG,	ctasg05,
	SU_EASY|T_LONG|T_INDR,	SU_EASY|T_ANY,	ctasg05,
	SU_EASY|T_LONG|T_INDR,	SU_EASY|T_LONG,	ctasg05,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctasg06,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_LONG,	ctasg06,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctasg06,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	ctasg06,
	0,
};
char ctbop01[] {
	LEFT,0,
	0 };
char ctbop02[] {
	LEFT,0,
	EXLR,
	OP,TRIGHT,' ',RADDR,',',CR,'\n',
	0 };
char ctbop03[] {
	LEFT,0,
	CLRL,' ',NR,'\n',
	MOV,' ',RADDR,',',NR,'\n',
	OP,TLEFT,' ',NR,',',CR,'\n',
	0 };
char ctbop04[] {
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	RIGHT,S_NEXT,
	OP,TRIGHT,' ',NR,',',CR,'\n',
	0 };
char ctbop05[] {
	LEFT,0,
	OP,TLEFT,' ',RADDR,',',CR,'\n',
	0 };
char ctbop06[] {
	LEFT,0,
	EXLR,
	RIGHT,S_NEXT,
	EXRLN,
	OP,TEITHER,' ',NR,',',CR,'\n',
	0 };
char ctbop07[] {
	RIGHT,0,
	EXRL,
	MOVL,' ',CR,',',PSH,'\n',
	LEFT,0,
	OP,TLEFT,' ',POP,',',CR,'\n',
	0 };
char ctbop08[] {
	RIGHT,S_STACK,
	LEFT,0,
	EXLR,
	OP,TRIGHT,' ',POP,',',CR,'\n',
	0 };
char ctbop09[] {
	RIGHT,S_STACK,
	LEFT,0,
	OP,' ',POP,',',CR,'\n',
	0 };
struct skeleton fr_op[] {
	SU_ANY|T_ANY,	SU_ZERO|T_ANY,	ctbop01,
	SU_ANY|T_ANY,	SU_ADDR|T_INT,	ctbop02,
	SU_ANY|T_ANY,	SU_ADDR|T_LONG,	ctbop02,
	SU_ANY|T_LONG,	SU_ADDR|T_LONG,	ctbop02,
	SU_ANY|T_LONG,	SU_ADDR|T_UNSN,	ctbop03,
	SU_ADDR|T_UNSN,	SU_ANY|T_LONG,	ctbop04,
	SU_ANY|T_LONG,	SU_CONST|T_INT,	ctbop05,
	SU_ANY|T_LONG,	SU_ADDR|T_LONG,	ctbop05,
	SU_ANY|T_ANY,	SU_EASY|T_ANY,	ctbop06,
	SU_ANY|T_ANY,	SU_EASY|T_LONG,	ctbop06,
	SU_ANY|T_LONG,	SU_EASY|T_ANY,	ctbop06,
	SU_ANY|T_LONG,	SU_EASY|T_LONG,	ctbop06,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctbop07,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctbop07,
	SU_ANY|T_ANY,	SU_ANY|T_LONG,	ctbop08,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctbop08,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctbop09,
	0,
};
char ctbop10[] {
	LEFT,S_STACK,
	OP,TLEFTL,' ',RADDR,',',STK,'\n',
	0 };
char ctbop11[] {
	LEFT,S_STACK,
	RIGHT,0,
	OP,TLEFTL,' ',CR,',',STK,'\n',
	0 };
struct skeleton fs_op[] {
	SU_ANY|T_ANY,	SU_CONST|T_INT,	ctbop10,
	SU_ANY|T_LONG,	SU_CONST|T_INT,	ctbop10,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctbop11,
	0,
};
char ctxor01[] {
	LEFT,S_STACK,
	RIGHT,0,
	OP,' ',CR,',',STK,'\n',
	MOV,' ',POP,',',CR,'\n',
	0 };
struct skeleton fr_xor[] {
	SU_ANY|T_ANY,	SU_EASY|T_ANY,	ctbop06,
	SU_ANY|T_ANY,	SU_EASY|T_LONG,	ctbop06,
	SU_ANY|T_LONG,	SU_EASY|T_ANY,	ctbop06,
	SU_ANY|T_LONG,	SU_EASY|T_LONG,	ctbop06,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctxor01,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctbop07,
	0,
};
char ctshf01[] {
	LEFT,0,
	OP,TLEFTL,' ',RADDR,',',CR,'\n',
	0 };
char ctshf02[] {
	LEFT,0,
	RIGHT,S_NEXT,
	OP,TLEFTL,' ',NR,',',CR,'\n',
	0 };
char ctshf03[] {
	RIGHT,S_STACK,
	LEFT,0,
	MOV,TRIGHT,' ',POP,',',NR,'\n',
	OP,TLEFTL,' ',NR,',',CR,'\n',
	0 };
struct skeleton fr_shft[] {
	SU_ANY|T_ANY,	SU_ZERO|T_ANY,	ctbop01,
	SU_ANY|T_LONG,	SU_ZERO|T_ANY,	ctbop01,
	SU_ANY|T_ANY,	SU_SMALL|T_INT,	ctshf01,
	SU_ANY|T_LONG,	SU_SMALL|T_INT,	ctshf01,
	SU_ANY|T_ANY,	SU_EASY|T_ANY,	ctshf02,
	SU_ANY|T_ANY,	SU_EASY|T_LONG,	ctshf02,
	SU_ANY|T_LONG,	SU_EASY|T_ANY,	ctshf02,
	SU_ANY|T_LONG,	SU_EASY|T_LONG,	ctshf02,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctshf03,
	SU_ANY|T_ANY,	SU_ANY|T_LONG,	ctshf03,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctshf03,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctshf03,
	0,
};
char ctmul01[] {
	LEFT,0,
	OP,' ',RADDR,',',CR,'\n',
	0 };
char ctmul02[] {
	LEFT,0,
	RIGHT,S_NEXT|S_INDR,
	OP,' ',ROFFSET,'(',NAR,')',',',CR,'\n',
	0 };
char ctmul03[] {
	LEFT,0,
	RIGHT,S_NEXT,
	OP,' ',NR,',',CR,'\n',
	0 };
char ctmul04[] {
	RIGHT,S_STACK,
	LEFT,0,
	OP,' ',POP,',',CR,'\n',
	0 };
char ctmul05[] {
	MOVL,' ',RADDR,',',PSH,'\n',
	LEFT,S_STACK,
	JSR,' ',AOP,'\n',
	POP8,0 };
char ctmul06[] {
	RIGHT,0,
	EXRL,
	MOVL,' ',CR,',',PSH,'\n',
	LEFT,S_STACK,
	JSR,' ',AOP,'\n',
	POP8,0 };
char ctmul07[] {
	LEFT,0,
	EXLR,
	MOVL,' ',CR,',',PSH,'\n',
	RIGHT,S_STACK,
	JSR,' ',AOP,'\n',
	POP8,0 };
char ctmul08[] {
	RIGHT,S_STACK,
	LEFT,S_STACK,
	JSR,' ',AOP,'\n',
	POP8,0 };
struct skeleton fr_mult[] {
	SU_ANY|T_ANY,	SU_ADDR|T_ANY,	ctmul01,
	SU_ANY|T_ANY,	SU_EASY|T_ANY|T_INDR,	ctmul02,
	SU_ANY|T_ANY,	SU_EASY|T_ANY,	ctmul03,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctmul04,
	SU_ANY|T_LONG,	SU_CONST|T_INT,	ctmul05,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctmul06,
	SU_ANY|T_ANY,	SU_ANY|T_LONG,	ctmul07,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctmul08,
	0,
};
char ctdiv01[] {
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	OP,' ',RADDR,',',CR,'\n',
	MODSWAP,0 };
char ctdiv02[] {
	LEFT,0,
	EXL,
	OP,' ',RADDR,',',CR,'\n',
	MODSWAP,0 };
char ctdiv03[] {
	RIGHT,S_INDR,
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	OP,' ',ROFFSET,'(',CAR,')',',',CR,'\n',
	MODSWAP,0 };
char ctdiv04[] {
	LEFT,0,
	EXL,
	RIGHT,S_NEXT|S_INDR,
	OP,' ',ROFFSET,'(',NAR,')',',',CR,'\n',
	MODSWAP,0 };
char ctdiv05[] {
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	RIGHT,S_NEXT,
	OP,' ',NR,',',CR,'\n',
	MODSWAP,0 };
char ctdiv06[] {
	LEFT,0,
	EXL,
	RIGHT,S_NEXT,
	OP,' ',NR,',',CR,'\n',
	MODSWAP,0 };
char ctdiv07[] {
	RIGHT,S_STACK,
	LEFT,0,
	EXL,
	OP,' ',POP,',',CR,'\n',
	MODSWAP,0 };
struct skeleton fr_div[] {
	SU_ADDR|T_UNSN,	SU_ADDR|T_INT,	ctdiv01,
	SU_ANY|T_ANY,	SU_ADDR|T_INT,	ctdiv02,
	SU_ANY|T_LONG,	SU_ADDR|T_INT,	ctdiv02,
	SU_ADDR|T_UNSN,	SU_EASY|T_INT|T_INDR,	ctdiv03,
	SU_ANY|T_ANY,	SU_EASY|T_INT|T_INDR,	ctdiv04,
	SU_ANY|T_LONG,	SU_EASY|T_INT|T_INDR,	ctdiv04,
	SU_ADDR|T_UNSN,	SU_EASY|T_ANY,	ctdiv05,
	SU_ANY|T_ANY,	SU_EASY|T_ANY,	ctdiv06,
	SU_ANY|T_LONG,	SU_EASY|T_ANY,	ctdiv06,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctdiv07,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctdiv07,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctmul08,
	0,
};
char cteop01[] {
	CLRL,' ',CR,'\n',
	MOV,' ',RADDR,',',CR,'\n',
	OP,TLEFT,' ',LADDR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop02[] {
	LEFT,0,
	OP,TRIGHT,' ',RADDR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop03[] {
	RIGHT,0,
	OP,TLEFT,' ',LADDR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop04[] {
	RIGHT,S_INDR,
	LEFT,0,
	OP,TLEFT,' ',ROFFSET,'(',CAR,')',',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop05[] {
	RIGHT,S_NEXT,
	LEFT,0,
	OP,' ',NR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop06[] {
	RIGHT,S_INDR,
	LEFT,S_NEXT|S_INDR,
	MOV,' ',LOFFSET,'(',NAR,')',',',CR,'\n',
	OP,' ',ROFFSET,'(',CAR,')',',',CR,'\n',
	MOV,' ',CR,',',LOFFSET,'(',NAR,')','\n',
	0 };
char cteop07[] {
	RIGHT,S_STACK,
	MOV,TLEFT,' ',LADDR,',',CR,'\n',
	OP,' ',POP,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop08[] {
	RIGHT,0,
	LEFT,S_NEXT|S_INDR,
	OP,TLEFT,' ',CR,',',LOFFSET,'(',NAR,')','\n',
	MOV,TLEFT,' ',LOFFSET,'(',NAR,')',',',CR,'\n',
	0 };
char cteop09[] {
	LEFT,S_INDR,
	MOV,TRIGHT,' ',RADDR,',',CR,'\n',
	EXRL,
	OP,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char cteop10[] {
	RIGHT,S_STACK,
	LEFT,S_INDR,
	MOV,TRIGHT,' ',POP,',',CR,'\n',
	EXRL,
	OP,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char cteop11[] {
	RIGHT,S_STACK,
	LEFT,S_INDR,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	OP,TRIGHT,' ',POP,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char cteop12[] {
	RIGHT,S_STACK,
	LEFT,S_INDR,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	OP,' ',POP,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fr_eqop[] {
	SU_ADDR|T_LONG,	SU_ADDR|T_UNSN,	cteop01,
	SU_ADDR|T_ANY,	SU_ADDR|T_INT,	cteop02,
	SU_ADDR|T_CHAR,	SU_ADDR|T_CHAR,	cteop02,
	SU_ADDR|T_LONG,	SU_ADDR|T_LONG,	cteop02,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	cteop03,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	cteop03,
	SU_ADDR|T_INT,	SU_ANY|T_INT|T_INDR,	cteop04,
	SU_ADDR|T_INT,	SU_ANY|T_ANY,	cteop05,
	SU_EASY|T_INT|T_INDR,	SU_ANY|T_INT|T_INDR,	cteop06,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	cteop07,
	SU_EASY|T_INT|T_INDR,	SU_ANY|T_ANY,	cteop08,
	SU_EASY|T_LONG|T_INDR,	SU_ANY|T_ANY,	cteop08,
	SU_EASY|T_LONG|T_INDR,	SU_ANY|T_LONG,	cteop08,
	SU_ANY|T_LONG|T_INDR,	SU_ADDR|T_INT,	cteop09,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	cteop10,
	SU_ANY|T_INT|T_INDR,	SU_ANY|T_ANY,	cteop11,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	cteop11,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	cteop12,
	0,
};
char cteop14[] {
	RIGHT,S_INDR,
	MOV,TRIGHT,' ',ROFFSET,'(',CAR,')',',',CR,'\n',
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop15[] {
	RIGHT,S_INDR,
	LEFT,S_NEXT|S_INDR,
	MOV,TRIGHT,' ',ROFFSET,'(',CAR,')',',',CR,'\n',
	OP,TLEFT,' ',CR,',',LOFFSET,'(',NAR,')','\n',
	0 };
char cteop16[] {
	RIGHT,S_INDR,
	MOV,TLEFT,' ',LADDR,',',NR,'\n',
	OP,TRIGHT,' ',ROFFSET,'(',CAR,')',',',NR,'\n',
	MOV,TLEFT,' ',NR,',',LADDR,'\n',
	0 };
char cteop17[] {
	RIGHT,0,
	MOV,TLEFT,' ',LADDR,',',NR,'\n',
	OP,TLEFT,' ',CR,',',NR,'\n',
	MOV,TLEFT,' ',NR,',',LADDR,'\n',
	0 };
char cteop18[] {
	RIGHT,S_STACK,
	LEFT,S_INDR,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',NR,'\n',
	OP,TRIGHT,' ',POP,',',NR,'\n',
	MOV,TLEFT,' ',NR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char cteop19[] {
	CLRL,' ',CR,'\n',
	MOV,' ',RADDR,',',CR,'\n',
	OP,TLEFT,' ',RADDR,',',LADDR,'\n',
	0 };
char cteop20[] {
	MOV,TRIGHT,' ',RADDR,',',CR,'\n',
	EXRL,
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char cteop21[] {
	RIGHT,S_STACK,
	LEFT,S_INDR,
	MOV,TRIGHT,' ',POP,',',CR,'\n',
	OP,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char cteop22[] {
	LEFT,S_INDR,
	MOV,TRIGHT,' ',RADDR,',',CR,'\n',
	EXRL,
	OP,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char cteop23[] {
	RIGHT,S_STACK,
	LEFT,S_INDR,
	MOV,TRIGHT,' ',POP,',',CR,'\n',
	EXRL,
	OP,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fe_eqop[] {
	SU_ANY|T_ANY|T_INDR,	SU_ZERO|T_ANY,	ctnull,
	SU_ADDR|T_ANY,	SU_ZERO|T_ANY,	ctnull,
	SU_ANY|T_LONG,	SU_ZERO|T_ANY,	ctnull,
	SU_ANY|T_LONG|T_INDR,	SU_ZERO|T_ANY,	ctnull,
	SU_ADDR|T_ANY,	SU_CONST|T_INT,	ctasg09,
	SU_ADDR|T_LONG,	SU_CONST|T_INT,	ctasg09,
	SU_ANY|T_ANY|T_INDR,	SU_CONST|T_INT,	ctasg19,
	SU_ANY|T_LONG|T_INDR,	SU_CONST|T_INT,	ctasg19,
	SU_ADDR|T_LONG,	SU_AREG|T_LONG,	ctasg02,
	SU_ADDR|T_INT,	SU_REG|T_INT,	ctasg09,
	SU_REG|T_INT,	SU_ADDR|T_INT,	ctasg09,
	SU_ADDR|T_LONG,	SU_REG|T_LONG,	ctasg09,
	SU_REG|T_LONG,	SU_ADDR|T_LONG,	ctasg09,
	SU_AREG|T_LONG,	SU_REG|T_INT,	ctasg22,
	SU_ADDR|T_INT,	SU_ADDR|T_INT,	ctasg02,
	SU_ADDR|T_CHAR,	SU_ADDR|T_CHAR,	ctasg02,
	SU_ADDR|T_LONG,	SU_ADDR|T_LONG,	ctasg02,
	SU_ADDR|T_INT,	SU_ANY|T_INT|T_INDR,	cteop14,
	SU_ADDR|T_INT,	SU_ANY|T_ANY,	ctasg02,
	SU_EASY|T_INT|T_INDR,	SU_ANY|T_INT|T_INDR,	cteop15,
	SU_EASY|T_LONG|T_INDR,	SU_ANY|T_LONG|T_INDR,	cteop15,
	SU_ADDR|T_ANY,	SU_ANY|T_INT|T_INDR,	cteop16,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	cteop17,
	SU_EASY|T_INT|T_INDR,	SU_EASY|T_ANY,	ctasg05,
	SU_ANY|T_INT|T_INDR,	SU_ANY|T_ANY,	ctasg06,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	cteop18,
	SU_REG|T_LONG,	SU_ADDR|T_UNSN,	cteop19,
	SU_REG|T_LONG,	SU_ADDR|T_ANY,	cteop20,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctasg02,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG|T_INDR,	cteop14,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	ctasg02,
	SU_EASY|T_LONG|T_INDR,	SU_EASY|T_LONG,	ctasg05,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	cteop21,
	SU_ANY|T_LONG|T_INDR,	SU_ADDR|T_ANY,	cteop22,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	cteop23,
	0,
};
struct skeleton fr_eqmult[] {
	SU_ADDR|T_ANY,	SU_ADDR|T_INT,	cteop02,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	cteop07,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	cteop12,
	0,
};
char ctedv01[] {
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	OP,' ',RADDR,',',CR,'\n',
	MODSWAP,MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctedv02[] {
	MOV,TLEFT,' ',LADDR,',',CR,'\n',
	EXL,
	OP,' ',RADDR,',',CR,'\n',
	MODSWAP,MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctedv03[] {
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	RIGHT,S_NEXT,
	OP,' ',NR,',',CR,'\n',
	MODSWAP,MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctedv04[] {
	LEFT,0,
	OP,' ',RADDR,',',CR,'\n',
	MODSWAP,EXRL,
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctedv05[] {
	RIGHT,S_STACK,
	LEFT,0,
	EXL,
	OP,' ',POP,',',CR,'\n',
	MODSWAP,EXRL,
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctedv06[] {
	RIGHT,S_STACK,
	LEFT,S_INDR,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	EXL,
	OP,' ',POP,',',CR,'\n',
	MODSWAP,EXRL,
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fr_eqdiv[] {
	SU_ADDR|T_UNSN,	SU_ADDR|T_INT,	ctedv01,
	SU_ADDR|T_ANY,	SU_ADDR|T_INT,	ctedv02,
	SU_ADDR|T_UNSN,	SU_EASY|T_ANY,	ctedv03,
	SU_ADDR|T_LONG,	SU_ADDR|T_INT,	ctedv04,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctedv05,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctedv05,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctedv06,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctedv06,
	0,
};
char ctexo01[] {
	RIGHT,0,
	EXRL,
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	MOV,TLEFT,' ',LADDR,',',CR,'\n',
	0 };
char ctexo02[] {
	LEFT,S_STACK|S_INDR,
	RIGHT,0,
	EXRL,
	MOVL,' ',POP,',',CAR,'\n',
	OP,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	0 };
struct skeleton fr_eqxor[] {
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctexo01,
	SU_ADDR|T_ANY,	SU_ANY|T_LONG,	ctexo01,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctexo01,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	ctexo01,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctexo02,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_LONG,	ctexo02,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctexo02,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	ctexo02,
	0,
};
char ctexo05[] {
	MOV,TRIGHT,' ',RADDR,',',CR,'\n',
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
struct skeleton fe_eqxor[] {
	SU_ADDR|T_ANY,	SU_CONST|T_INT,	ctasg09,
	SU_ADDR|T_LONG,	SU_CONST|T_INT,	ctasg09,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctasg02,
	SU_ADDR|T_ANY,	SU_ANY|T_LONG,	ctasg02,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctasg02,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	ctasg02,
	SU_ANY|T_ANY|T_INDR,	SU_EASY|T_ANY,	ctasg05,
	SU_ANY|T_ANY|T_INDR,	SU_EASY|T_LONG,	ctasg05,
	SU_ANY|T_LONG|T_INDR,	SU_EASY|T_ANY,	ctasg05,
	SU_ANY|T_LONG|T_INDR,	SU_EASY|T_LONG,	ctasg05,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctasg06,
	SU_ADDR|T_LONG,	SU_CONST|T_INT,	ctasg09,
	SU_ADDR|T_LONG,	SU_ADDR|T_LONG,	ctexo05,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG|T_INDR,	cteop14,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	ctasg02,
	SU_ANY|T_LONG|T_INDR,	SU_CONST|T_INT,	ctasg19,
	SU_EASY|T_LONG|T_INDR,	SU_ANY|T_LONG,	ctasg05,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	cteop21,
	0,
};
char ctesh01[] {
	OP,TLEFT,' ',LADDR,'\n',
	MOV,TLEFT,' ',LADDR,',',CR,'\n',
	0 };
char ctesh02[] {
	LEFT,S_INDR,
	OP,TLEFT,' ',LOFFSET,'(',CAR,')','\n',
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	0 };
char ctesh03[] {
	LEFT,0,
	OP,TLEFT,' ',RADDR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctesh05[] {
	RIGHT,S_NEXT,
	MOV,TRIGHT,' ',LADDR,',',CR,'\n',
	OP,TLEFT,' ',NR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctesh06[] {
	LEFT,S_STACK|S_INDR,
	MOVL,' ',POP,',',CAR,'\n',
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	OP,TLEFT,' ',RADDR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
char ctesh07[] {
	LEFT,S_INDR,
	RIGHT,S_NEXT,
	MOV,TLEFT,' ',LOFFSET,'(',CAR,')',',',CR,'\n',
	OP,TLEFT,' ',NR,',',CR,'\n',
	MOV,TLEFT,' ',CR,',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fr_eqshft[] {
	SU_ADDR|T_INT,	SU_ONE|T_ANY,	ctesh01,
	SU_ANY|T_INT|T_INDR,	SU_ONE|T_ANY,	ctesh02,
	SU_ADDR|T_ANY,	SU_SMALL|T_INT,	ctesh03,
	SU_ADDR|T_LONG,	SU_SMALL|T_INT,	ctesh03,
	SU_ADDR|T_ANY,	SU_ANY|T_ANY,	ctesh05,
	SU_ADDR|T_ANY,	SU_ANY|T_LONG,	ctesh05,
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctesh05,
	SU_ADDR|T_LONG,	SU_ANY|T_LONG,	ctesh05,
	SU_ANY|T_ANY|T_INDR,	SU_SMALL|T_INT,	ctesh06,
	SU_ANY|T_LONG|T_INDR,	SU_SMALL|T_INT,	ctesh06,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_ANY,	ctesh07,
	SU_ANY|T_ANY|T_INDR,	SU_ANY|T_LONG,	ctesh07,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctesh07,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_LONG,	ctesh07,
	0,
};
char ctesh08[] {
	OP,TLEFT,' ',LADDR,'\n',
	0 };
char ctesh09[] {
	LEFT,S_INDR,
	OP,TLEFT,' ',LOFFSET,'(',CAR,')','\n',
	0 };
char ctesh10[] {
	CLRL,' ',CR,'\n',
	MOV,TRIGHT,' ',RADDR,',',CR,'\n',
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctesh11[] {
	RIGHT,S_INDR,
	CLRL,' ',CR,'\n',
	MOV,TRIGHT,' ',ROFFSET,'(',CAR,')',',',CR,'\n',
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctesh12[] {
	CLRL,' ',CR,'\n',
	RIGHT,0,
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
struct skeleton fe_eqshft[] {
	SU_ADDR|T_INT,	SU_ONE|T_ANY,	ctesh08,
	SU_ANY|T_INT|T_INDR,	SU_ONE|T_ANY,	ctesh09,
	SU_REG|T_ANY,	SU_SMALL|T_INT,	ctasg09,
	SU_REG|T_LONG,	SU_SMALL|T_INT,	ctasg09,
	SU_REG|T_ANY,	SU_ADDR|T_INT,	ctesh10,
	SU_REG|T_LONG,	SU_ADDR|T_INT,	ctesh10,
	SU_REG|T_ANY,	SU_ANY|T_INT|T_INDR,	ctesh11,
	SU_REG|T_LONG,	SU_ANY|T_INT|T_INDR,	ctesh11,
	SU_REG|T_ANY,	SU_ANY|T_ANY,	ctesh12,
	SU_REG|T_LONG,	SU_ANY|T_ANY,	ctesh12,
	0,
};
char ctitl01[] {
	CLRL,' ',CR,'\n',
	0 };
char ctitl02[] {
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	0 };
char ctitl03[] {
	MOVL,' ',LADDR,',',CR,'\n',
	0 };
char ctitl04[] {
	LEFT,0,
	EXL,
	0 };
struct skeleton fr_itl[] {
	SU_ZERO|T_ANY,	SU_ANY|T_ANY,	ctitl01,
	SU_ADDR|T_UNSN,	SU_ANY|T_ANY,	ctitl02,
	SU_CONST|T_INT,	SU_ANY|T_ANY,	ctitl03,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctitl04,
	0,
};
char ctitl05[] {
	CLRL,' ',CR,'\n',
	MOV,' ',LADDR,',',CR,'\n',
	MOVL,' ',CR,',',PSH,'\n',
	0 };
char ctitl06[] {
	LEFT,0,
	EXL,
	MOVL,' ',CR,',',PSH,'\n',
	0 };
char ctitl07[] {
	MOV,' ',LADDR,',',CAR,'\n',
	MOVL,' ',CAR,',',PSH,'\n',
	0 };
struct skeleton fs_itl[] {
	SU_ADDR|T_UNSN,	SU_ANY|T_ANY,	ctitl05,
	SU_ANY|T_UNSN,	SU_ANY|T_ANY,	ctitl06,
	SU_ADDR|T_INT,	SU_ANY|T_ANY,	ctitl07,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctitl06,
	0,
};
char ctlti01[] {
	LEFT,S_INDR,
	MOV,' ',LOFFSET,'+','2','(',CAR,')',',',CR,'\n',
	0 };
struct skeleton fr_lti[] {
	SU_ADDR|T_LONG,	SU_ANY|T_ANY,	ctitl03,
	SU_ANY|T_LONG|T_INDR,	SU_ANY|T_ANY,	ctlti01,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctbop01,
	0,
};
char ctlem01[] {
	RIGHT,0,
	EXRL,
	MOVL,' ',CR,',',PSH,'\n',
	LEFT,S_STACK,
	JSR,' ',OP,'\n',
	POP8,0 };
char ctlem02[] {
	RIGHT,S_STACK,
	LEFT,S_STACK,
	JSR,' ',OP,'\n',
	POP8,0 };
struct skeleton fr_lemul[] {
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctlem01,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctlem02,
	0,
};
char ctrel01[] {
	LEFT,S_FORCC,
	0 };
char ctrel02[] {
	RIGHT,S_INDR,
	OP,TRIGHT,' ',ROFFSET,'(',CAR,')',',',LADDR,'\n',
	0 };
char ctrel03[] {
	RIGHT,0,
	EXRL,
	OP,TLEFT,' ',CR,',',LADDR,'\n',
	0 };
char ctrel05[] {
	LEFT,0,
	MOV,' ',RADDR,',',CAR,'\n',
	OP,TLEFT,' ',CAR,',',CR,'\n',
	0 };
struct skeleton fc_rel[] {
	SU_CONST|T_INT,	SU_ZERO|T_ANY,	ctbop01,
	SU_CONST|T_INT,	SU_CONST|T_INT,	ctbop05,
	SU_AREG|T_LONG,	SU_ZERO|T_ANY,	ctlod03,
	SU_ADDR|T_ANY,	SU_ZERO|T_ANY,	ctasg07,
	SU_ADDR|T_LONG,	SU_ZERO|T_ANY,	ctasg07,
	SU_ANY|T_ANY|T_INDR,	SU_ZERO|T_ANY,	ctasg08,
	SU_ANY|T_LONG|T_INDR,	SU_ZERO|T_ANY,	ctasg08,
	SU_ANY|T_ANY,	SU_ZERO|T_ANY,	ctrel01,
	SU_ANY|T_LONG,	SU_ZERO|T_ANY,	ctrel01,
	SU_ADDR|T_ANY,	SU_CONST|T_INT,	ctasg09,
	SU_ADDR|T_LONG,	SU_CONST|T_INT,	ctasg09,
	SU_ANY|T_LONG|T_INDR,	SU_CONST|T_INT,	ctasg19,
	SU_ANY|T_ANY|T_INDR,	SU_CONST|T_INT,	ctasg19,
	SU_AREG|T_LONG,	SU_ADDR|T_INT,	ctasg22,
	SU_REG|T_CHAR,	SU_ADDR|T_CHAR,	ctasg09,
	SU_REG|T_INT,	SU_ADDR|T_INT,	ctasg09,
	SU_REG|T_LONG,	SU_ADDR|T_LONG,	ctasg09,
	SU_REG|T_INT,	SU_ANY|T_CHAR,	ctasg10,
	SU_REG|T_INT,	SU_ANY|T_INT|T_INDR,	ctrel02,
	SU_REG|T_LONG,	SU_ANY|T_LONG|T_INDR,	ctrel02,
	SU_AREG|T_LONG,	SU_ANY|T_INT|T_INDR,	ctrel02,
	SU_REG|T_INT,	SU_EASY|T_ANY,	ctrel03,
	SU_REG|T_LONG,	SU_EASY|T_LONG,	ctrel03,
	SU_REG|T_LONG,	SU_EASY|T_ANY,	ctrel03,
	SU_ANY|T_INT,	SU_ADDR|T_INT,	ctbop05,
	SU_ANY|T_CHAR,	SU_ADDR|T_CHAR,	ctbop05,
	SU_ANY|T_LONG,	SU_ADDR|T_LONG,	ctbop05,
	SU_ANY|T_LONG,	SU_CONST|T_INT,	ctbop05,
	SU_ANY|T_CHAR,	SU_ADDR|T_INT,	ctbop02,
	SU_ANY|T_LONG,	SU_ADDR|T_INT,	ctrel05,
	SU_ANY|T_ANY,	SU_EASY|T_ANY,	ctbop06,
	SU_ANY|T_ANY,	SU_EASY|T_LONG,	ctbop06,
	SU_ANY|T_LONG,	SU_EASY|T_ANY,	ctbop06,
	SU_ANY|T_LONG,	SU_EASY|T_LONG,	ctbop06,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctbop07,
	SU_ANY|T_ANY,	SU_ANY|T_LONG,	ctbop08,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctbop08,
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctbop09,
	0,
};
char ctbtt01[] {
	OP,' ',RADDR,',',LADDR,'\n',
	0 };
char ctbtt02[] {
	LEFT,S_INDR,
	OP,' ',RADDR,',',LOFFSET,'(',CAR,')','\n',
	0 };
struct skeleton fc_btst[] {
	SU_ADDR|T_ANY,	SU_CONST|T_INT,	ctbtt01,
	SU_ADDR|T_LONG,	SU_CONST|T_INT,	ctbtt01,
	SU_ANY|T_ANY|T_INDR,	SU_CONST|T_INT,	ctbtt02,
	SU_ANY|T_LONG|T_INDR,	SU_CONST|T_INT,	ctbtt02,
	0,
};
char ctfix01[] {
	TREE,0,
	0 };
struct skeleton fc_fix[] {
	SU_ANY|T_ANY,	SU_ANY|T_ANY,	ctfix01,
	SU_ANY|T_LONG,	SU_ANY|T_LONG,	ctfix01,
	SU_ANY|T_LONG,	SU_ANY|T_ANY,	ctfix01,
	SU_ANY|T_ANY,	SU_ANY|T_LONG,	ctfix01,
	0,
};
