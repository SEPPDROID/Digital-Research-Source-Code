$ set def [steve.cpm68k.asmlink]
$ set noon
$ assign [-.asc] asc:
$ conv :== $bin:conv
$ lo68 -r -s -o ar68.out lib:startup.o ar68.o lib:lib6.a
$ conv
ar68.out
asc:ar68.asc
$ lo68 -r -s -o lo68.out lib:startup.o lo68.o lib:lib6.a
$ conv
lo68.out
asc:lo68.asc
$ lo68 -r -s -o nm68.out lib:startup.o nm68.o lib:lib6.a
$ conv
nm68.out
asc:nm68.asc
$ lo68 -r -s -o prtobj.out lib:startup.o prtobj.o lib:lib6.a
$ conv
prtobj.out
asc:prtobj.asc
$ lo68 -r -s -o sendc68.out lib:startup.o sendc68.o lib:lib6.a
$ conv
sendc68.out
asc:sendc68.asc
$ lo68 -r -s -o reloc.out lib:startup.o reloc.o lib:clib68.a
$ conv
reloc.out
asc:reloc.asc
$ lo68 -r -s -o dump.out lib:startup.o dump.o lib:lib6.a
$ conv
dump.out
asc:dump.asc
$ ! end
