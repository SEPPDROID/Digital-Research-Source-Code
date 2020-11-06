$ delete ar68.exe;*,as68.exe;*,lo68.exe;*,nm68.exe;*,prtobj.exe;*
$ delete sendc68.exe;*
$ clink ar68,lib:klutz/lib ar68
$ clink lo68,lo68init,lib:klutz/lib lo68
$ clink nm68,lib:klutz/lib nm68
$ clink prtobj,lo68init,lib:klutz/lib prtobj
$ clink sendc68,lib:klutz/lib sendc68
