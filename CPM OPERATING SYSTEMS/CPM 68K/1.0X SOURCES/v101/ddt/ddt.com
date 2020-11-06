$ set noon
$ set def [cpm68k.freeze.ddt]
$ assign drb0:[steve.bin] bin:
$ assign drb0:[steve.lib] lib:
$ as68 :== $bin:as68.exe
$ ld68 :== $bin:lo68.exe
$ cc68 ddt1
$ cc68 ddt2
$ cc68 ddtutil
$ cc68 disasm
$ as68 -u -l -p bdos.s >bdos.lis
$ ld68 -r -o ddt1.68k ddt1.o ddt2.o ddtutil.o disasm.o bdos.o
$ as68 -u -l -p ddtload.s >ddtload.lis
$ ld68 -r -o ddt.68k ddtload.o
$ pr/nofeed ddt1.c,ddt2.c,ddtutil.c,disasm.c,bdos.lis, -
	    ddtinc.h,disas.h,optab.h,bdosfunc.h,stdio.h, -
	    ddtload.lis
