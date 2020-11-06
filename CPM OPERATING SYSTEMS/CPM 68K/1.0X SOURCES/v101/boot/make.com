$ set def [cpm68k.release.boot]
$ set noon
$ cc68 bdosmain
$ cc68 bdosread
$ cc68 cpmldr
$ cc68 dskutil
$ cc68 fileio
$ cc68 iosys
$ as68 -l -u bdosif.s
$ as68 -l -u booter.s
$ as68 -l -u ldrif.s
$ del     ldrlib.;*
$ ar68 rv ldrlib ldrif.o cpmldr.o bdosif.o dskutil.o fileio.o bdosread.o 
$ ar68 rv ldrlib bdosmain.o iosys.o 
$ lo68 -t0 -o exorboot.68k -uldr booter.o ldrlib ldbios.o ldbiosa.o
