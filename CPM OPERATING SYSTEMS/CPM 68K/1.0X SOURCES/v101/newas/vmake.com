$ newas
$ !
$ ! as68 Version 2.2 make file for VMS
$ !
$ set noon
$ copy machine.vax machine.h
$ cx AS68INIT
$ cx DIR
$ cx EXPR
$ cx MAIN
$ cx MISC
$ cx PASS1A
$ cx PASS2
$ cx qsort
$ cx PRINTF
$ cx strncmp
$ cx SYMT
$ @vrelink
