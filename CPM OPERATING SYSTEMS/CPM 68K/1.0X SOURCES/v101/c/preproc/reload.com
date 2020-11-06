$ set noon
$ lo68 -r -s -o cp68.out lib:startup.o cexpr.o lex.o macro.o main.o -
preproc.o util.o lib:lib6.a
$ conv :== $bin:conv
$ conv
cp68.out
[--.asc]cp68.asc
