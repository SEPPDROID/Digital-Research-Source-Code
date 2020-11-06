$ set nover
$ set noon
$ set def [bill.cpm68k.bios]
$ num
relbios.c
relbios.num
$ cc68 :== @cc68.com
$ as68 :== $bin:as68.exe
$ ld68 :== $bin:lo68.exe
$ cc68 relbios
$ as68 -u -l -p relbiosa.s >relbiosa.lis
$ ld68 -r -t1e000 -o relbios.68k relbiosa.o relbios.o
$ s68  relbios.68k >[bill.cpm68k.object]relbios.sr
