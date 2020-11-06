CC = c68
AS = as68
CFLAGS = -L -e
ASFLAGS = -u -L
OBJS = ftoa.o etoa.o atof.o ftol.o ltof.o ieeetof.o printf.o fprintf.o fabs.o
OBJ2 = floor.o ceil.o fmod.o
OBJ3 = atan.o sinh.o cosh.o tanh.o iefatan.o ffpatan.o iefsinh.o

init: 	${OBJS} ${OBJ2}
	${AS} ${ASFLAGS} fpadd.s
	${AS} ${ASFLAGS} fpcmp.s
	${AS} ${ASFLAGS} fpcos.s
	${AS} ${ASFLAGS} fpdiv.s
	${AS} ${ASFLAGS} fpmul.s
	${AS} ${ASFLAGS} fpneg.s
	${AS} ${ASFLAGS} fpsin.s
	${AS} ${ASFLAGS} fpsqrt.s
	${AS} ${ASFLAGS} fpsub.s
	${AS} ${ASFLAGS} fpexp.s
	${AS} ${ASFLAGS} fplog.s
	${AS} ${ASFLAGS} fppwr.s
	${AS} ${ASFLAGS} ffpabs.s
	${AS} ${ASFLAGS} ffpadd.s
	${AS} ${ASFLAGS} ffpcmp.s
	${AS} ${ASFLAGS} ffpcpyrt.s
	${AS} ${ASFLAGS} ffpdiv.s
	${AS} ${ASFLAGS} ffpmul2.s
	${AS} ${ASFLAGS} ffpsin.s
	${AS} ${ASFLAGS} ffpsqrt.s
	${AS} ${ASFLAGS} ffptheta.s
	${AS} ${ASFLAGS} ffphthet.s
	${AS} ${ASFLAGS} ffptnorm.s
	${AS} ${ASFLAGS} ffpieee.s
	${AS} ${ASFLAGS} ffpexp.s
	${AS} ${ASFLAGS} ffplog.s
	${AS} ${ASFLAGS} ffppwr.s
	${AS} ${ASFLAGS} iefabs.s
	${AS} ${ASFLAGS} iefadd.s
	${AS} ${ASFLAGS} iefcmp.s
	${AS} ${ASFLAGS} iefdiv.s
	${AS} ${ASFLAGS} iefmul.s
	${AS} ${ASFLAGS} iefsin.s
	${AS} ${ASFLAGS} iefsqrt.s
	${AS} ${ASFLAGS} iefexp.s
	${AS} ${ASFLAGS} ieflog.s
	${AS} ${ASFLAGS} iefpwr.s
	${AS} ${ASFLAGS} iefback.s
	${AS} ${ASFLAGS} ieffront.s
	${AS} ${ASFLAGS} atan.s
	${AS} ${ASFLAGS} sinh.s
	${AS} ${ASFLAGS} cosh.s
	${AS} ${ASFLAGS} tanh.s
	${AS} ${ASFLAGS} ffpatan.s
	${AS} ${ASFLAGS} iefatan.s
	${AS} ${ASFLAGS} iefsinh.s
