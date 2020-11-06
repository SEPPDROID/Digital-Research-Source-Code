$ set noon
$ klib
$ !
$ !	Klib creation for vms
$ !
$ del *.o;*
$ cc68 cputc
$ cc68 creat
$ cc68 dup
$ cc68 getc
$ cc68 getchar
$ cc68 open
$ cc68 putc
$ cc68 putchar
$ cc68 seek
$ cc68 strcmp
$ del lib6.a;*
$ ar68 r lib6.a  cputc.o putchar.o putc.o getchar.o 
$ ar68 r lib6.a getc.o  open.o creat.o seek.o 
$ ar68 r lib6.a strcmp.o dup.o  
