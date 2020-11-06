$ set noon
$ as68 -l -u -p BDOSIF.s >BDOSIF.lis
$ as68 -l -u -p CCPBDOS.s >CCPBDOS.lis
$ as68 -l -u -p CCPIF.s >CCPIF.lis
$ as68 -l -u -p CCPLOAD.s >CCPLOAD.lis
$ as68 -l -u -p EXCEPTN.s >EXCEPTN.lis
$ as68 -l -u -p FILETYPS.s >FILETYPS.lis
$ as68 -l -u -p PGMLD.s >PGMLD.lis
$ as68 -l -u -p STACK.s >STACK.lis
