$ set noon
$ fcgen
$ copy machine.vax machine.h
$ cc68 CANON
$ cc68 CODEGEN
$ cc68 INTERF
$ cc68 MAIN
$ cc68 OPTAB
$ cc68 PUTEXPR
$ cc68 SMATCH
$ cc68 SUCOMP
$ cc68 TABL
$ cc68 UTIL
$ cc68 VERSION
$ @relink
