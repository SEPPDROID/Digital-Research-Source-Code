cc -c getc.c
cc -c length.c
cc -c printf.c
cc -c prtint.c
cc -c putchar.c
cc -c xstrcmp.c
cc -c putc.c
rm libx.a
ar r libx.a printf.o prtint.o putc.o putchar.o length.o getc.o xstrcmp.o
