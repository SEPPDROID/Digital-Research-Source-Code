$ lo68 -r -o as68.out lib:startup.o dir.o expr.o main.o misc.o pass1a.o -
pass2.o qsort.o strncmp.o symt.o lib:lib6.a
$ conv :== $bin:conv
$ conv
as68.out
[-.asc]as68.asc
