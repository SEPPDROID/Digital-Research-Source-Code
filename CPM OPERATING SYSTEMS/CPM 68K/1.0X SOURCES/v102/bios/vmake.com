$ bios
$ set noon
$ !
$ !	VMS build file for EXORmacs bios using cross tools
$ !
$ copy loadbios.h biostype.h
$ cc68 bios
$ rename bios.o ldbios.o
$ as68 -u -l ldbiosa.s
$ as68 -u -l booter.s
$ copy normbios.h biostype.h
$ cc68 bios
$ as68 -u -l biosa.s
$ pur biostype.h
