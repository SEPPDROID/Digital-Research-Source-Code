$ set nover
$ set noon
$ set def c:[cpm68k.source.bios]
$ copy normbios.h biostype.h
$ num
bios.c
bios.num
$ cc68 bios
$ as68 -u -l -p biosa.s >biosa.lis
$ lo68 -r -t5000 -o bios.68k biosa.o bios.o
$ pr/nofeed bios.num,bios.lis,biosa.lis
