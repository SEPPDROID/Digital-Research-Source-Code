$ set noon
$ klib
$ !
$ !	Klib creation for vms
$ !
$ del *.obj;*
$ cx cputc
$ cx creat
$ cx dup
$ cx getcvms
$ cx getchar
$ cx open
$ cx putcvms
$ cx putchar
$ cx seek
$ cx strcmp
$ del klib.olb;*
$ lib/creat klib *.obj
