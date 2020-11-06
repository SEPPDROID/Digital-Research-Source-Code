$ set noon
$ num
SID03.c
SID03.lis
$ num
SID04.c
SID04.lis
$ num
SID05.c
SID05.lis
$ num
SID68K.c
SID68K.lis
$ num
SIDFUN.c
SIDFUN.lis
$ as68 -l -u -p BDOS.S >bdos.lis
$ del bdos.o;
$ as68 -l -u -p SIDLOAD.S >sidload.lis
$ del sidload.o;
$ num
DDTINC.h
DDTINC.lst
$ num
DISAS.h
DISAS.lst
$ num
LGCDEF.h
LGCDEF.lst
$ num
OPTAB.h
OPTAB.lst
$ num
SIDDEF.h
SIDDEF.lst
$ num
STDIO.h
STDIO.lst
