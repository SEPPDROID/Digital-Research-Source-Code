$ !
$ ! Compile, link, locate and generate hex for
$ ! 	'PIP'
$ ! Concurrent CP/M-86
$ !
$ util := pip
$ date := 2/9/83
$ ccpmsetup	! set up environment
$ assign 'f$directory()' f1:

$ asm86 scd1.a86 xref
$ asm86 inpout.a86 xref
$ plm86 'util'.plm xref 'p1' optimize(3) debug date('date')
$ link86 scd1.obj,inpout.obj,'util'.obj  to 'util'.lnk
$ loc86 'util'.lnk od(sm(code,dats,data,stack,const)) -
  ad(sm(code(0),dats(10000h))) ss(stack(+32)) to 'util'.
$ h86 'util'
$ !
$ !pclean
