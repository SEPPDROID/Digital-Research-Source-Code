$ set def [steve.cpm68k.c.parser]
$ set noon
$ cc68 DECL
$ cc68 EXPR
$ cc68 ICODE
$ cc68 INTERF
$ cc68 LEX
$ cc68 MAIN
$ cc68 STMT
$ cc68 TABL
$ cc68 initx
$ @relink
