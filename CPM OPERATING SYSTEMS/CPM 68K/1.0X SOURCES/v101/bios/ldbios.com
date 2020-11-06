$ set nover
$ set noon
$ set def [bill.cpm68k.bios]
$ num
hdbios.c
hdbios.num
$ cc68 :== @cc68.com
$ as68 :== $bin:as68.exe
$ ld68 :== $bin:lo68.exe
$ cc68 hdbios
$ as68 -u -l -p ldbiosa.s >ldbiosa.lis
$ pr/nofeed hdbios.num,hdbios.lis,ldbiosa.lis
