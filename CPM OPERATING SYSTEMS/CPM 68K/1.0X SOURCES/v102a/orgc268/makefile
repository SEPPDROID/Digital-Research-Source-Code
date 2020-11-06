CC = cc
C68 = c68
CFLAGS = -O -w -DVAX11
C68FLAGS = -L -r -DMC68000
LIB = -lV6
C68LIB = -l6
.IGNORE:

vax:
	mkver -e "c68 Optimizer 4.3 -"
	${CC} ${CFLAGS} -n version.c optim.c -o c268.vax ${LIB}

4k:
	mkver -e "c68 Optimizer 4.3 -"
	${C68} ${C68FLAGS} -n version.c optim.c -o c268.4k ${C68LIB}
	setstack c268.4k 8000 8000

all: vax 4k

