$ set noon
$ set def [steve.cpm68k.c.preproc]
$ copy machine.vax machine.h
$ cx cexpr
$ cx lex
$ cx macro
$ cx main
$ cx preproc
$ cx util
$ @relink
$ copy c68.exe bin:
