$ set noon
$ set def [steve.cpm68k.c.fparser]
$ copy machine.68k machine.h
$ cc68f DECL
$ cc68f EXPR
$ cc68f ICODE
$ cc68f INTERF
$ cc68f LEX
$ cc68f MAIN
$ cc68f STMT
$ cc68f TABL
$ cc68f VERSION
$ @relink
