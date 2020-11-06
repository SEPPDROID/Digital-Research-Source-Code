$ set noon
$ set def [steve.cpm68k.c.fpreproc]
$ copy machine.vax machine.h
$ cx CEXPR
$ cx INIT
$ cx LEX
$ cx MACRO
$ cx MAIN
$ cx UTIL
$ cx VERSION
$ @vrelink
