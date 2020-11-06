$ set noon
$ copy normbios.h biostype.h
$ c68 bios
$ as68 -u -l biosa.s
