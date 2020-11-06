$ set def [steve.cpm68k.asmlink]
$ copy machine.vax machine.h
$ purge machine.h
$ cx ar68
$ cx lo68
$ cx lo68init
$ cx nm68
$ cx prtobj
$ cx sendc68
$ @vrelink
