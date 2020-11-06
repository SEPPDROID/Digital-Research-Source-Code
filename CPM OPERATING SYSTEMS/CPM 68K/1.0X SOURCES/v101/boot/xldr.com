$ set noon
$ set def [bobh.cpm68k.boot]
$ define SYS$PRINT NOWHERE
$ ld68 :== $bin:lo68
$ ld68 -T0 -o xldr.68k booter.o ldrif.o cpmldr.o bdosif.o dskutil.o -
fileio.o bdosread.o bdosmain.o iosys.o ldrbiosa.o hdbios.o
