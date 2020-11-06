$ preproc
$ set noon
$ ! 
$ !	Build file for VMS version of CP68
$ !
$ copy machine.vax machine.h
$ pur machine.h
$ cx CEXPR
$ cx INIT
$ cx LEX
$ cx MACRO
$ cx MAIN
$ cx UTIL
$ clink cexpr,init,lex,macro,main,util,lib:klib/lib cp68
