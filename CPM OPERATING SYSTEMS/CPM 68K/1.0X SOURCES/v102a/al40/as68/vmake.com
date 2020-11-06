$ asm
$ !
$ ! as68 Version 2.3 make file for VMS
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
$ cx SYMT
$ @vrelink
