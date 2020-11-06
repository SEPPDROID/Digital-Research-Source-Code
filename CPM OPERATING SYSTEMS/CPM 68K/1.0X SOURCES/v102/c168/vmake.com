$ cgen
$ set noon
$ !
$ !	C168 make file for VMS
$ !
$ copy machine.vax machine.h
$ pur  machine.h
$ cx  CANON
$ cx  CODEGEN
$ cx  CSKELS
$ cx  INIT
$ cx  INTERF
$ cx  MAIN
$ cx  OPTAB
$ cx  PUTEXPR
$ cx  SMATCH
$ cx  SUCOMP
$ cx  TABL
$ cx  UTIL
$ clink canon,codegen,cskels,interf,main,optab,putexpr,smatch,sucomp,tabl,util,init,lib:klib/lib c168
