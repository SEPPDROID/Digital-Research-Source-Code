$ set noon
$ clib
$ cc68 XMAIN
$ cc68 XWMAIN
$ cc68 PERROR
$ cc68 MKTEMP
$ cc68 CHMOD
$ cc68 CHOWN
$ cc68 FPRINTF
$ cc68 PRINTF
$ cc68 SPRINTF
$ cc68 XDOPRT
$ cc68 XPRTINT
$ cc68 XPRTLD
$ cc68 XPRTSHRT
$ cc68 FSCANF
$ cc68 SCANF
$ cc68 SSCANF
$ cc68 XDOSCAN
$ cc68 FDOPEN
$ cc68 FOPEN
$ cc68 FREOPEN
$ cc68 FTELL
$ cc68 REWIND
$ cc68 FSEEK
$ cc68 EXIT
$ cc68 CLEANUP
$ cc68 FCLOSE
$ cc68 FFLUSH
$ cc68 FREAD
$ cc68 FWRITE
$ cc68 FGETS
$ cc68 GETS
$ cc68 FPUTS
$ cc68 PUTS
$ cc68 GETL
$ cc68 GETW
$ cc68 PUTL
$ cc68 PUTW
$ cc68 UNGETC
$ cc68 SETBUF
$ cc68 FGETC
$ cc68 FPUTC
$ cc68 XFILBUF
$ cc68 XFLSBUF
$ cc68 XFDECLS
$ cc68 CLOSE
$ cc68 CREAT
$ cc68 XEXIT
$ cc68 LSEEK
$ cc68 XFILESZ
$ cc68 OPEN
$ cc68 READ
$ cc68 XTTYIN
$ cc68 UNLINK
$ cc68 WRITE
$ cc68 XOPEN
$ cc68 ACCESS
$ cc68 GETPASS
$ cc68 TTYNAME
$ cc68 ISATTY
$ cc68 CHINIT
$ cc68 CHKC
$ cc68 BLKIO
$ cc68 ALLOCC
$ cc68 ATOI
$ cc68 ATOL
$ cc68 SWAB
$ cc68 GETPID
$ cc68 RAND
$ cc68 QSORT
$ cc68 CALLOC
$ cc68 MALLOC
$ cc68 SBRK
$ cc68 ERRNO
$ cc68 CTYPE
$ cc68 INDEX
$ cc68 RINDEX
$ cc68 STRCAT
$ cc68 STRCMP
$ cc68 STRCPY
$ cc68 STRLEN
$ cc68 STRNCAT
$ cc68 STRNCMP
$ cc68 STRNCPY
$ cc68 signal
$ cc68 xstrcmp
$ as68 -l -u  ABORT.s
$ as68 -l -u  ALDIV.s
$ as68 -l -u  ALMUL.s
$ as68 -l -u  ALREM.s
$ as68 -l -u  LMUL.s
$ as68 -l -u  LDIV.s
$ as68 -l -u  LREM.s
$ as68 -l -u  ULDIV.s
$ as68 -l -u  setjmp.s
$ as68 -l -u  s.s
$ as68 -l -u  w.s
$ as68 -l -u  nofloat.s
$ as68 -l -u  blivot.s
$ as68 -l -u  xsignal.s
$ @rear
