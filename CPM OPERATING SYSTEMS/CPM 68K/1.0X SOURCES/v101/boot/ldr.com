$ set noon
$ set def [bobh.cpm68k.boot]
$ define SYS$PRINT NOWHERE
$ ld68 :== $bin:lo68
$ ld68 -T900 -o cpmldr.68k ldrif.o cpmldr.o bdosif.o dskutil.o fileio.o - 
bdosread.o bdosmain.o iosys.o ldrbios.o
$ nm68 cpmldr.68k >cpmldr.sym
