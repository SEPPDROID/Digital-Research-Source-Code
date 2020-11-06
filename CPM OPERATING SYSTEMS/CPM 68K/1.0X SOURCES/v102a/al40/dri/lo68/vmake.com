$ linker
$ set noon
$ !
$ !	lo68 and utilities build file for VMS
$ !
$ copy machine.vax machine.h
$ purge machine.h
$ cx ar68 
$ cx lo68 
$ cx lo68init
$ cx dump 
$ cx nm68
$ cx prtobj 
$ cx reloc 
$ cx sendc68 
$ cx size68
$ clink ar68,lib:klib/lib ar68
$ clink lo68,lo68init,lib:klib/lib lo68
$ clink dump dump
$ clink nm68,lib:klib/lib nm68
$ clink prtobj,lo68init,lib:klib/lib prtobj
$ clink reloc reloc
$ clink sendc68,lib:klib/lib sendc68
$ clink size68,lib:klib/lib size68
