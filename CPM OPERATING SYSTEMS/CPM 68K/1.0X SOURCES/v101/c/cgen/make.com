$ set def [steve.cpm68k.c.cgen]
$ !
$ cc68 CANON
$ cc68 CODEGEN
$ cc68 CSKELS
$ cc68 INTERF
$ cc68 MAIN
$ cc68 OPTAB
$ cc68 PUTEXPR
$ cc68 SUCOMP
$ cc68 TABL
$ cc68 UTIL
$ cc68 initx
$ @relink
