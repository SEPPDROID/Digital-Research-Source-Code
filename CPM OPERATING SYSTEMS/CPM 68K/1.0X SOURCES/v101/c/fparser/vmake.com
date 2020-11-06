$ set noon
$ set def [steve.cpm68k.c.fparser]
$ copy machine.vax machine.h
$ cx DECL
$ cx EXPR
$ cx ICODE
$ cx INIT
$ cx INTERF
$ cx LEX
$ cx MAIN
$ cx STMT
$ cx TABL
$ cx VERSION
$ @vrelink
