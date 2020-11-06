$ set def [steve.cpm68k.c.cgen]
$ set noon
$ lo68 -r -s -o c168.out lib:startup.o canon.o codegen.o cskels.o interf.o -
main.o optab.o putexpr.o sucomp.o tabl.o util.o initx.o lib:lib6.a
$ conv :== $bin:conv
$ conv
c168.out
[--.asc]c168.asc
