$ set noon
$ set def [steve.cpm68k.c.fpreproc]
$ clink CEXPR,INIT,LEX,MACRO,MAIN,UTIL,VERSION,lib:k3/lib cp68'p1'
