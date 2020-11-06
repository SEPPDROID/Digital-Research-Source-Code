$ set noon
$ set def [bobh.cpm68k.boot]
$ define SYS$PRINT NOWHERE
$ ld68 :== $bin:lo68
$ ld68 -T800 -o test bdosif.o dskutil.o fileio.o bdosread.o bdosmain.o iosys.o 
