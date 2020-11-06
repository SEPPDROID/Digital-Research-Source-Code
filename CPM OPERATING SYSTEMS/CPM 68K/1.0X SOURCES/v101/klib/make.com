$ set noon
$ set def drb0:[steve.cpm68k.klib]
$ delete *.o;*,lib6.a;*,*.ic;*,*.st;*,*.i;*
$ cc68 BLKIO
$ cc68 CHINIT
$ cc68 CHKC
$ cc68 CLENF
$ cc68 CLOSE
$ cc68 cputc
$ cc68 CREAT
$ cc68 dup
$ cc68 EXIT
$ cc68 getc
$ cc68 getchar
$ cc68 getpid
$ cc68 IGETC
$ cc68 length
$ cc68 LSEEK
$ cc68 MAIN
$ cc68 OPEN
$ cc68 printf
$ cc68 prtint
$ cc68 putc
$ cc68 putchar
$ cc68 READ
$ cc68 SBRK
$ cc68 seek
$ cc68 xSTRCMP
$ cc68 strlen
$ cc68 strcat
$ cc68 UNLINK
$ cc68 WRITE
$ cc68 XOPEN
$ as68 -l startup.s
$ as68 -l lmul.s
$ as68 -l ldiv.s
$ as68 -l lrem.s
$ @rear
