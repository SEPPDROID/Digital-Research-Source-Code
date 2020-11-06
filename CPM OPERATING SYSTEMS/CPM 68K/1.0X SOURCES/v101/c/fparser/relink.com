$ set noon
$ set def [steve.cpm68k.c.fparser]
$ lo68 -r -o c068.rel lib:startup.o DECL.o EXPR.o ICODE.o INTERF.o -
LEX.o MAIN.o STMT.o TABL.o VERSION.o lib:lib6.a lib:libF.a
