$ set def [steve.cpm68k.c.parser]
$ set noon
$ conv :== $bin:conv
$ lo68 -r -s -o c068.out lib:startup.o decl.o expr.o icode.o interf.o lex.o -
main.o stmt.o tabl.o initx.o lib:lib6.a
$ conv
c068.out
[--.asc]c068.asc
