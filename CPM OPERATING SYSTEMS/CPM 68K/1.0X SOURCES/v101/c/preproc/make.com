$ set noon
$ set def [steve.cpm68k.c.preproc]
$ copy machine.68k machine.h
$ cc68 cexpr
$ cc68 lex
$ cc68 macro
$ cc68 main
$ cc68 preproc
$ cc68 util
$ @reload
