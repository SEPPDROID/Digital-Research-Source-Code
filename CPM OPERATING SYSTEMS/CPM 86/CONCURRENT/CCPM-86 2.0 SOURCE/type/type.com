$ !
$ ! Compile, link, locate and generate hex for
$ ! 	'TYPE'
$ ! Concurrent CP/M-86
$ !
$ util := type
$ ccpmsetup	! set up environment
$ assign 'f$directory()' f1:

$ plm86 'util'.plm xref 'p1' optimize(3) debug
$ link86 f2:scd.obj, 'util'.obj  to 'util'.lnk
$ loc86 'util'.lnk od(sm(code,dats,data,stack,const)) -
  ad(sm(code(0),dats(10000h))) ss(stack(+32)) to 'util'.
$ h86 'util'
$ !
$ !pclean
