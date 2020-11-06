$ set noon
$ delete lib6.a;*
$ ar68 r lib6.a main.o printf.o prtint.o cputc.o putchar.o putc.o getchar.o -
getc.o exit.o close.o open.o creat.o read.o write.o seek.o lseek.o -
xopen.o unlink.o blkio.o chinit.o chkc.o igetc.o sbrk.o xstrcmp.o 
$ ar68 r lib6.a length.o clenf.o lrem.o ldiv.o lmul.o lrem.o -
dup.o strlen.o strcat.o
$ copy lib6.a lib:
$ purge lib:
