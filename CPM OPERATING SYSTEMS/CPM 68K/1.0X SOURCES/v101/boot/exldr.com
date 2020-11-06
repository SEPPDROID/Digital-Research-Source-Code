$ set noon
$ set def [bobh.cpm68k.boot]
$ define SYS$PRINT NOWHERE
$ ld68 :== $bin:lo68
$ ld68 -Tf900 -o exldr.68k ldrif.o cpmldr.o bdosif.o dskutil.o fileio.o - 
bdosread.o bdosmain.o iosys.o ldrbiosa.o hdbios.o
