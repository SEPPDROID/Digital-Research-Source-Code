$ newas
$ copy machine.68k machine.h
$ set noon
$ cc68 DIR
$ cc68 EXPR
$ cc68 MAIN
$ cc68 MISC
$ cc68 PASS1A
$ cc68 PASS2
$ cc68 QSORT
$ cc68 STRNCMP
$ cc68 SYMT
$ @relink
