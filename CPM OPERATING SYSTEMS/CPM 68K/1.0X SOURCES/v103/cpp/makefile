CC = cc
C68 = nc68
VAXOBJS = vaxobj/cexpr.o \
	vaxobj/lex.o \
	vaxobj/macro.o \
	vaxobj/main.o
C68OBJS = 68obj/cexpr.o \
	68obj/lex.o \
	68obj/macro.o \
	68obj/main.o
CFLAGS = -O -DVAX11
C68FLAGS = -L -r -DMC68000 -t0 -t1
LIB = -lV6
C68LIB = -l6

vax:	${VAXOBJS}
	mkver -e "Preprocessor -"
	${CC} ${CFLAGS} -n version.c ${VAXOBJS} -o cpp68.vax ${LIB}

cpp:	${C68OBJS}
	mkver -e "Preprocessor -"
	${C68} ${C68FLAGS} version.c ${C68OBJS} -o cpp68.68 ${C68LIB}
	setstack cpp68.68 8000 8000

2k:	${C68OBJS}
	mkver -e "Preprocessor -"
	${C68} ${C68FLAGS} -n2 version.c ${C68OBJS} -o cpp68.2k ${C68LIB}
	setstack cpp68.2k 8000 8000

4k:	${C68OBJS}
	mkver -e "Preprocessor -"
	${C68} ${C68FLAGS} -n version.c ${C68OBJS} -o cpp68.4k ${C68LIB}
	setstack cpp68.4k 8000 8000

all: vax 4k

vaxobj/cexpr.o: cexpr.c
	${CC} ${CFLAGS} -c cexpr.c;mv -f cexpr.o vaxobj/cexpr.o

vaxobj/lex.o: lex.c
	${CC} ${CFLAGS} -c lex.c;mv -f lex.o vaxobj/lex.o

vaxobj/macro.o: macro.c
	${CC} ${CFLAGS} -c macro.c;mv -f macro.o vaxobj/macro.o

vaxobj/main.o: main.c
	${CC} ${CFLAGS} -c main.c;mv -f main.o vaxobj/main.o

68obj/cexpr.o: cexpr.c
	${C68} ${C68FLAGS} -c cexpr.c;mv -f cexpr.o 68obj/cexpr.o

68obj/lex.o: lex.c
	${C68} ${C68FLAGS} -c lex.c;mv -f lex.o 68obj/lex.o

68obj/macro.o: macro.c
	${C68} ${C68FLAGS} -c macro.c;mv -f macro.o 68obj/macro.o

68obj/main.o: main.c
	${C68} ${C68FLAGS} -c main.c;mv -f main.o 68obj/main.o


