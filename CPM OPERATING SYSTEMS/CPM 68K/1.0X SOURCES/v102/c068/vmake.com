$ parser
$ set noon
$ !
$ !	C068 make file for VMS
$ !
$ copy machine.vax machine.h
$ pur machine.h
$ cx  DECL
$ cx  EXPR
$ cx  ICODE
$ cx  INIT
$ cx  INTERF
$ cx  LEX
$ cx  MAIN
$ cx  STMT
$ cx  TABL
$ cx  VERSION
$ clink DECL,EXPR,ICODE,INTERF,LEX,MAIN,STMT,TABL,init,version,lib:klib/lib c068
