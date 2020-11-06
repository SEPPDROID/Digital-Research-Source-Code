$ set nover
$ set noon
$ set def [bill.cpm68k.bios]
$ num
bios.c
bios.num
$ cc68 :== @cc68.com
$ as68 :== $bin:as68.exe
$ ld68 :== $bin:lo68.exe
$ cc68 bios
$ as68 -u -l -p biosa.s >biosa.lis
$ as68 -u -l -p ldbiosa.s >ldbiosa.lis
$ ld68 -r -t5000 -o bios.68k biosa.o bios.o
$ s68 - bios.68k >[bill.cpm68k.object]bios.sr
$ pr/nofeed/copies=7 bios.num,biosa.lis,ldbiosa.lis
