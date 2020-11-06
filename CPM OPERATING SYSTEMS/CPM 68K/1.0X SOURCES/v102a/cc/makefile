CC = cc
C68 = c68
CFLAGS = -O -w -DVAX11
C68FLAGS = -L -r -DMC68000
LIB = -lV6
C68LIB = -l6
.IGNORE:

vax:
	mkver -e "c68 Main Driver 4.3 -"
	${CC} ${CFLAGS} -n version.c main.c -o c68.vax ${LIB}

4k:
	mkver -e "c68 Main Driver 4.3 -"
	${C68} ${C68FLAGS} -n version.c main.c -o c68.4k ${C68LIB}
	setstack c68.4k 8000 8000

all: vax 4k
