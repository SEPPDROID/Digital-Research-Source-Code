CC = c68
AS = as68
CFLAGS = -L -f 
AFLAGS = -u -L
OBJS = atof.o ceil.o etoa.o fprintf.o ftoa.o ftoffp.o ftol.o ltof.o printf.o
OBJ2 = ffptof.o floor.o fmod.o fabs.o
OBJ3 = atan.o sinh.o cosh.o tanh.o ffpatan.o ffpsinh.o

init:	${OBJS} ${OBJ2} ${OBJ3}
	${AS} ${AFLAGS} ffpabs.s
	${AS} ${AFLAGS} ffpadd.s
	${AS} ${AFLAGS} ffpcmp.s
	${AS} ${AFLAGS} ffpcpyrt.s
	${AS} ${AFLAGS} ffpdiv.s
	${AS} ${AFLAGS} ffpexp.s
	${AS} ${AFLAGS} ffphthet.s
	${AS} ${AFLAGS} ffplog.s
	${AS} ${AFLAGS} ffpmul2.s
	${AS} ${AFLAGS} ffppwr.s
	${AS} ${AFLAGS} ffpsin.s
	${AS} ${AFLAGS} ffpsqrt.s
	${AS} ${AFLAGS} ffptheta.s
	${AS} ${AFLAGS} ffptnorm.s
	${AS} ${AFLAGS} fpadd.s
	${AS} ${AFLAGS} fpcmp.s
	${AS} ${AFLAGS} fpcos.s
	${AS} ${AFLAGS} fpdiv.s
	${AS} ${AFLAGS} fpexp.s
	${AS} ${AFLAGS} fplog.s
	${AS} ${AFLAGS} fpmul.s
	${AS} ${AFLAGS} fpneg.s
	${AS} ${AFLAGS} fppwr.s
	${AS} ${AFLAGS} fpsin.s
	${AS} ${AFLAGS} fpsqrt.s
	${AS} ${AFLAGS} fpsub.s
	${AS} ${AFLAGS} atan.s
	${AS} ${AFLAGS} sinh.s
	${AS} ${AFLAGS} cosh.s
	${AS} ${AFLAGS} tanh.s
	${AS} ${AFLAGS} ffpatan.s
	${AS} ${AFLAGS} ffpsinh.s
