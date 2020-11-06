$ set noon
$ cx lo68
$ cx lo68init
$ clink lo68,lo68init,lib:klutz/lib lo68
$ cx nmc68
$ clink nmc68,lib:klutz/lib nmc68
$ cx prtobj
$ clink prtobj,lo68init,lib:klutz/lib prtobj
$ cx sendc68
$ clink sendc68,lib:klutz/lib sendc68
$ cx ar68
$ clink ar68,lib:klutz/lib ar68
