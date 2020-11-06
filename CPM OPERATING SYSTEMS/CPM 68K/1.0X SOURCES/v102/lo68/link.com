$ linker
$ set noon
$ !
$ !	lo68 and utilities build file for VMS
$ !
$ clink lo68,lo68init,lib:klib/lib lo68
$ clink dump dump
$ clink nm68,lib:klib/lib nm68
$ clink prtobj,lo68init,lib:klib/lib prtobj
$ clink reloc reloc
$ clink sendc68,lib:klib/lib sendc68
$ clink size68,lib:klib/lib size68
