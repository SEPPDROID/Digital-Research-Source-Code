$ set def [steve.cpm68k.asmlink]
$ set noon
$ copy machine.68k machine.h
$ purge machine.h
$ delete *.o;*
$ cc68 ar68
$ cc68 lo68
$ cc68 nm68
$ cc68 prtobj
$ cc68 sendc68
$ cc68 reloc
$ cc68 dump
$ @reload
