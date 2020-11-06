$ set noon
$ asm		! Assembler first
$ @vmake
$ copy as68.exe bin:
$ del *.obj;*,*.map;*,*.exe;*
$ pur bin:as68.exe
$ linker
$ @vmake
$ copy *.exe bin:
$ pur bin:
$ del *.obj;*,*.map;*,*.exe;*
$ preproc
$ @vmake
$ copy cp68.exe bin:
$ pur bin:cp68.exe
$ del *.obj;*,*.map;*,*.exe;*
$ parser
$ @vmake
$ copy c068.exe bin:
$ pur bin:c068.exe
$ del *.obj;*,*.map;*,*.exe;*
$ cgen
$ @vmake
$ copy c168.exe bin:
$ del *.obj;*,*.map;*,*.exe;*

