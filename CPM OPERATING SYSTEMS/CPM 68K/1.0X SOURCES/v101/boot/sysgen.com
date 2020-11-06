$ set nover
$ set noon
$ set def [bobh.cpm68k.boot]
$ cc68 :== @[bobh]cc68.com
$ define SYS$PRINT NOWHERE
$ cc68 sysgen
