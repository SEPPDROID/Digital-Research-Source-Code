$ set noon
$ num
BLKIO.C
BLKIO.lis
$ num
CHINIT.C
CHINIT.lis
$ num
CHKC.C
CHKC.lis
$ num
CLENF.C
CLENF.lis
$ num
CLOSE.C
CLOSE.lis
$ num
CPUTC.C
CPUTC.lis
$ num
CREAT.C
CREAT.lis
$ num
DUP.C
DUP.lis
$ num
EXIT.C
EXIT.lis
$ num
GETC.C
GETC.lis
$ num
GETCHAR.C
GETCHAR.lis
$ num
GETPID.C
GETPID.lis
$ num
IGETC.C
IGETC.lis
$ num
LENGTH.C
LENGTH.lis
$ num
LSEEK.C
LSEEK.lis
$ num
MAIN.C
MAIN.lis
$ num
OPEN.C
OPEN.lis
$ num
PRINTF.C
PRINTF.lis
$ num
PRTINT.C
PRTINT.lis
$ num
PUTC.C
PUTC.lis
$ num
PUTCHAR.C
PUTCHAR.lis
$ num
READ.C
READ.lis
$ num
SBRK.C
SBRK.lis
$ num
SEEK.C
SEEK.lis
$ num
STRCAT.C
STRCAT.lis
$ num
STRCMP.C
STRCMP.lis
$ num
STRLEN.C
STRLEN.lis
$ num
TTYIN.C
TTYIN.lis
$ num
UNLINK.C
UNLINK.lis
$ num
WRITE.C
WRITE.lis
$ num
XOPEN.C
XOPEN.lis
$ num
XSTRCMP.C
XSTRCMP.lis
$ num
XTTYIN.C
XTTYIN.lis
$ num
COUT.H
COUT.lst
$ num
CPM.H
CPM.lst
$ num
IODEC.H
IODEC.lst
$ num
PORTAB.H
PORTAB.lst
$ num
SENDC68.H
SENDC68.lst
$ num
STDIO.H
STDIO.lst
$ as68 -l -u -p COPYRT.S  >COPYRT.lis
$ as68 -l -u -p LDIV.S    >LDIV.lis
$ as68 -l -u -p LMUL.S    >LMUL.lis
$ as68 -l -u -p LREM.S    >LREM.lis
$ as68 -l -u -p STARTUP.S >STARTUP.lis
