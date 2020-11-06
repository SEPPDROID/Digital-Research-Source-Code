$ clib
$ set noon
$ num
ACCESS.C
ACCESS.lis
$ num
ALLOCC.C
ALLOCC.lis
$ num
ATOI.C
ATOI.lis
$ num
ATOL.C
ATOL.lis
$ num
BLKIO.C
BLKIO.lis
$ num
CALLOC.C
CALLOC.lis
$ num
CHINIT.C
CHINIT.lis
$ num
CHKC.C
CHKC.lis
$ num
CHMOD.C
CHMOD.lis
$ num
CHOWN.C
CHOWN.lis
$ num
CLEANUP.C
CLEANUP.lis
$ num
CLOSE.C
CLOSE.lis
$ num
CREAT.C
CREAT.lis
$ num
CTYPE.C
CTYPE.lis
$ num
ERRNO.C
ERRNO.lis
$ num
EXEC.C
EXEC.lis
$ num
EXIT.C
EXIT.lis
$ num
FCLOSE.C
FCLOSE.lis
$ num
FDOPEN.C
FDOPEN.lis
$ num
FFLUSH.C
FFLUSH.lis
$ num
FGETC.C
FGETC.lis
$ num
FGETS.C
FGETS.lis
$ num
FOPEN.C
FOPEN.lis
$ num
FPRINTF.C
FPRINTF.lis
$ num
FPUTC.C
FPUTC.lis
$ num
FPUTS.C
FPUTS.lis
$ num
FREAD.C
FREAD.lis
$ num
FREOPEN.C
FREOPEN.lis
$ num
FSCANF.C
FSCANF.lis
$ num
FSEEK.C
FSEEK.lis
$ num
FTELL.C
FTELL.lis
$ num
FWRITE.C
FWRITE.lis
$ num
GETL.C
GETL.lis
$ num
GETPASS.C
GETPASS.lis
$ num
GETPID.C
GETPID.lis
$ num
GETS.C
GETS.lis
$ num
GETW.C
GETW.lis
$ num
INDEX.C
INDEX.lis
$ num
ISATTY.C
ISATTY.lis
$ num
LDIV.C
LDIV.lis
$ num
LSEEK.C
LSEEK.lis
$ num
MALLOC.C
MALLOC.lis
$ num
MKTEMP.C
MKTEMP.lis
$ num
OPEN.C
OPEN.lis
$ num
PERROR.C
PERROR.lis
$ num
PRINTF.C
PRINTF.lis
$ num
PUTL.C
PUTL.lis
$ num
PUTS.C
PUTS.lis
$ num
PUTW.C
PUTW.lis
$ num
QSORT.C
QSORT.lis
$ num
RAND.C
RAND.lis
$ num
READ.C
READ.lis
$ num
REWIND.C
REWIND.lis
$ num
RINDEX.C
RINDEX.lis
$ num
SBRK.C
SBRK.lis
$ num
SCANF.C
SCANF.lis
$ num
SETBUF.C
SETBUF.lis
$ num
SIGNAL.C
SIGNAL.lis
$ num
SPRINTF.C
SPRINTF.lis
$ num
SSCANF.C
SSCANF.lis
$ num
STRCAT.C
STRCAT.lis
$ num
STRCMP.C
STRCMP.lis
$ num
STRCPY.C
STRCPY.lis
$ num
STRINS.C
STRINS.lis
$ num
STRLEN.C
STRLEN.lis
$ num
STRNCAT.C
STRNCAT.lis
$ num
STRNCMP.C
STRNCMP.lis
$ num
STRNCPY.C
STRNCPY.lis
$ num
SWAB.C
SWAB.lis
$ num
TTYNAME.C
TTYNAME.lis
$ num
UNGETC.C
UNGETC.lis
$ num
UNLINK.C
UNLINK.lis
$ num
WRITE.C
WRITE.lis
$ num
XDOPRT.C
XDOPRT.lis
$ num
XDOPRTFP.C
XDOPRTFP.lis
$ num
XDOSCAN.C
XDOSCAN.lis
$ num
XFDECLS.C
XFDECLS.lis
$ num
XFILBUF.C
XFILBUF.lis
$ num
XFILESZ.C
XFILESZ.lis
$ num
XFLSBUF.C
XFLSBUF.lis
$ num
XFPRINTF.C
XFPRINTF.lis
$ num
XMAIN.C
XMAIN.lis
$ num
XOPEN.C
XOPEN.lis
$ num
XPRTINT.C
XPRTINT.lis
$ num
XPRTLD.C
XPRTLD.lis
$ num
XPRTSHRT.C
XPRTSHRT.lis
$ num
XSTRCMP.C
XSTRCMP.lis
$ num
XTTYIN.C
XTTYIN.lis
$ num
XWMAIN.C
XWMAIN.lis
$ num
BASEPAGE.H
BASEPAGE.lst
$ num
BDOS.H
BDOS.lst
$ num
CPM.H
CPM.lst
$ num
CTYPE.H
CTYPE.lst
$ num
ERRNO.H
ERRNO.lst
$ num
PORTAB.H
PORTAB.lst
$ num
SETJMP.H
SETJMP.lst
$ num
SGTTY.H
SGTTY.lst
$ num
SIGNAL.H
SIGNAL.lst
$ num
STDIO.H
STDIO.lst
$ num
CREATVMS.C
CREATVMS.lsv
$ num
FTOAVMS.C
FTOAVMS.lsv
$ num
LSEEKVMS.C
LSEEKVMS.lsv
$ num
OPENVMS.C
OPENVMS.lsv
$ num
SBRKVMS.C
SBRKVMS.lsv
$ num
ULDIVVMS.C
ULDIVVMS.lsv
$ num
UNLINKVMS.C
UNLINKVMS.lsv
$ num
XFLSBFVMS.C
XFLSBFVMS.lsv
$ num
VMS.H
VMS.lsv
$ as68 -l -u -p ABORT.S >ABORT.lsa
$ as68 -l -u -p ALDIV.S >ALDIV.lsa
$ as68 -l -u -p ALMUL.S >ALMUL.lsa
$ as68 -l -u -p ALREM.S >ALREM.lsa
$ as68 -l -u -p BLIVOT.S >BLIVOT.lsa
$ as68 -l -u -p LDIV.S >LDIV.lsa
$ as68 -l -u -p LMUL.S >LMUL.lsa
$ as68 -l -u -p LREM.S >LREM.lsa
$ as68 -l -u -p NOFLOAT.S >NOFLOAT.lsa
$ as68 -l -u -p S.S >S.lsa
$ as68 -l -u -p SETJMP.S >SETJMP.lsa
$ as68 -l -u -p STKCPY.S >STKCPY.lsa
$ as68 -l -u -p ULDIV.S >ULDIV.lsa
$ as68 -l -u -p W.S >W.lsa
$ as68 -l -u -p XSIGNAL.S >XSIGNAL.lsa
$ del *.o;*
$ purge
$ pri/del/nofee *.lis,*.lsa,*.lst,*.lsv
