$ set def [steve.cpm68k.c.fcgen]
$ set noon
$ copy machine.vax machine.h
$ cx CANON
$ cx CODEGEN
$ cx CSKELS
$ cx INTERF
$ cx MAIN
$ cx OPTAB
$ cx PUTEXPR
$ cx SMATCH
$ cx SUCOMP
$ cx TABL
$ cx UTIL
$ cx VERSION
$ cx init
$ @vrelink
