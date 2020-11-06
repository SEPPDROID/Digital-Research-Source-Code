$ set def [cpm68k.release.boot]
$ set noon
$ del     ldrlib.;*
$ ar68 rv ldrlib ldrif.o cpmldr.o bdosif.o fileio.o 
$ ar68 rv ldrlib bdosmain.o bdosread.o iosys.o dskutil.o
$ lo68 -t0 -o exorboot.68k -uldr booter.o ldrlib ldbios.o ldbiosa.o
