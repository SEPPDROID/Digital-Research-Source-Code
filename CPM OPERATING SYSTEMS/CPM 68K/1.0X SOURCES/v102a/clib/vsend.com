$ set noon
$ vsend ABORT.S
$ vsend ACCESS.C
$ vsend ALDIV.S
$ vsend ALLOCC.C
$ vsend ALMUL.S
$ vsend ALREM.S
$ vsend ATOI.C
$ vsend ATOL.C
$ vsend BASEPAGE.H
$ vsend BDOS.H
$ vsend BLIVOT.S
$ vsend BLKIO.C
$ vsend C.SUB
$ vsend CALLOC.C
$ vsend CHINIT.C
$ vsend CHKC.C
$ vsend CHMOD.C
$ vsend CHOWN.C
$ vsend CLEANUP.C
$ vsend CLINK.SUB
$ vsend CLOSE.C
$ vsend CPM.H
$ vsend CREAT.C
$ vsend CREATVMS.C
$ vsend CTYPE.C
$ vsend CTYPE.H
$ vsend DOWN6.SUB
$ vsend ERRNO.C
$ vsend ERRNO.H
$ vsend EXEC.C
$ vsend EXIT.C
$ vsend FCLOSE.C
$ vsend FDOPEN.C
$ vsend FFLUSH.C
$ vsend FGETC.C
$ vsend FGETS.C
$ vsend FOPEN.C
$ vsend FPRINTF.C
$ vsend FPUTC.C
$ vsend FPUTS.C
$ vsend FREAD.C
$ vsend FREOPEN.C
$ vsend FSCANF.C
$ vsend FSEEK.C
$ vsend FTELL.C
$ vsend FTOAVMS.C
$ vsend FWRITE.C
$ vsend GETL.C
$ vsend GETPASS.C
$ vsend GETPID.C
$ vsend GETS.C
$ vsend GETW.C
$ vsend INDEX.C
$ vsend ISATTY.C
$ vsend ISATTYVMS.C
$ vsend LDIV.C
$ vsend LDIV.S
$ vsend LIST.COM
$ vsend LISTVMS.COM
$ vsend LMUL.S
$ vsend LREM.S
$ vsend LSEEK.C
$ vsend LSEEKVMS.C
$ vsend MACHINE.11
$ vsend MACHINE.68K
$ vsend MACHINE.H
$ vsend MACHINE.VAX
$ vsend MAKE.COM
$ vsend MAKE.SUB
$ vsend MALLOC.C
$ vsend MKTEMP.C
$ vsend MOPEN.MAR
$ vsend NEXT.SUB
$ vsend NOFLOAT.S
$ vsend OPEN.C
$ vsend OPENVMS.C
$ vsend PERROR.C
$ vsend PORTAB.H
$ vsend PRINTF.C
$ vsend PUTL.C
$ vsend PUTS.C
$ vsend PUTW.C
$ vsend QSORT.C
$ vsend RAND.C
$ vsend READ.C
$ vsend REAR.COM
$ vsend REAR.SUB
$ vsend REWIND.C
$ vsend RINDEX.C
$ vsend S.S
$ vsend SBREAK.MAR
$ vsend SBRK.C
$ vsend SBRKVMS.C
$ vsend SCANF.C
$ vsend SEND.SUB
$ vsend SEND6.SUB
$ vsend SETBUF.C
$ vsend SETJMP.H
$ vsend SETJMP.S
$ vsend SGTTY.H
$ vsend SIGNAL.C
$ vsend SIGNAL.H
$ vsend SPRINTF.C
$ vsend SSCANF.C
$ vsend STDIO.H
$ vsend STKCPY.S
$ vsend STRCAT.C
$ vsend STRCMP.C
$ vsend STRCPY.C
$ vsend STRINS.C
$ vsend STRLEN.C
$ vsend STRNCAT.C
$ vsend STRNCMP.C
$ vsend STRNCPY.C
$ vsend SWAB.C
$ vsend TTYNAME.C
$ vsend ULDIV.S
$ vsend ULDIVVMS.C
$ vsend UNGETC.C
$ vsend UNLINK.C
$ vsend UNLINKVMS.C
$ vsend UP6.SUB
$ vsend VMAKE.COM
$ vsend VMS.H
$ vsend W.S
$ vsend WRITE.C
$ vsend XDOPRT.C
$ vsend XDOPRTFP.C
$ vsend XDOSCAN.C
$ vsend XFDECLS.C
$ vsend XFILBUF.C
$ vsend XFILESZ.C
$ vsend XFLSBFVMS.C
$ vsend XFLSBUF.C
$ vsend XFPRINTF.C
$ vsend XHDR.MAR
$ vsend XMAIN.C
$ vsend XOPEN.C
$ vsend XPRTINT.C
$ vsend XPRTLD.C
$ vsend XPRTSHRT.C
$ vsend XSIGNAL.S
$ vsend XSTRCMP.C
$ vsend XTTYIN.C
$ vsend XWMAIN.C
$ vsend done
