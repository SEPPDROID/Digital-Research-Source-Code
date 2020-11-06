$ set def [steve.cpm68k.c.cgen]
$ clink canon,codegen,cskels,init,interf,main,optab,putexpr,sucomp,tabl,util,-
lib:k3/lib c168'p1'
