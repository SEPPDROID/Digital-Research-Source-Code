$ set noon
$ !
$ !	Build file for EXORmacs loader bios using VMS cross tools
$ !
$ copy loadbios.h biostype.h
$ c68 bios
$ as68 -u -l ldbiosa.s
$ as68 -u -l booter.s
$ rename bios.o ldbios.o
