$ bdos
$ !
$ !	VMS command file to build CPMLIB using cross tools
$ !
$ set   noon
$ cc68  bdosmain
$ cc68  bdosmisc
$ cc68  bdosrw
$ cc68  conbdos
$ cc68  dskutil
$ cc68  fileio
$ cc68  iosys
$ cc68  ccp
$ as68 -l -u  bdosif.s
$ as68 -l -u  -n exceptn.s
$ as68 -l -u  pgmld.s
$ as68 -l -u  ccpbdos.s
$ as68 -l -u  ccpif.s
$ as68 -l -u  ccpload.s
$ as68 -l -u  filetyps.s
$ as68 -l -u  stack.s
$ ar68 r cpmlib ccpif.o ccpbdos.o ccpload.o ccp.o bdosif.o conbdos.o 
$ ar68 r cpmlib bdosmisc.o dskutil.o fileio.o bdosrw.o bdosmain.o 
$ ar68 r cpmlib iosys.o pgmld.o exceptn.o filetyps.o stack.o
$ del *.o;*
