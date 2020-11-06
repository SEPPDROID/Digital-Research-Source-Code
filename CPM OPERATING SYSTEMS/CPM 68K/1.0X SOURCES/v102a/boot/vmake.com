$ boot
$ !
$ !	VMS procedure for building ldrlib with cross tools
$ !
$ set noon
$ cc68 bdosmain
$ cc68 bdosread
$ cc68 cpmldr
$ cc68 dskutil
$ cc68 fileio
$ cc68 iosys
$ as68 -l -u bdosif.s
$ as68 -l -u ldrif.s
$ del     ldrlib.;*
$ ar68 rv ldrlib ldrif.o cpmldr.o bdosif.o dskutil.o fileio.o bdosread.o 
$ ar68 rv ldrlib bdosmain.o iosys.o 
