$ set def [bill.cpm68k.ddt]
$ cc68 :== @cc68.com
$ assign drb0:[steve.bin] bin:
$ assign drb0:[steve.lib] lib:
$ as68 :== $bin:as68.exe;17
$ ld68 :== $bin:lo68.exe
$ ld68 -r -o ddt.68k ddt1.o ddt2.o ddtutil.o disasm.o environ.o -
		examine.o bdos.o
