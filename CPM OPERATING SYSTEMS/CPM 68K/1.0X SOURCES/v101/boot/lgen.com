$ set noon
$ set def [bobh.cpm68k.boot]
$ define SYS$PRINT NOWHERE
$ ld68 :== $bin:lo68
$ ld68 -T900 -o sysgen.68k lib:startup.o sysgen.o lib:clib68.a
$ nm68 sysgen.68k >sysgen.sym
