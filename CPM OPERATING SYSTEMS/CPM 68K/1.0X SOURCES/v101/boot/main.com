$ set nover
$ set noon
$ set def [bobh.cpm68k.boot]
$ cc68 :== @[bobh]cc68.com
$ as68 :== $bin:as68.exe
$ ld68 :== $bin:lo68.exe
$ define SYS$PRINT NOWHERE
$ cc68 bdosmain
