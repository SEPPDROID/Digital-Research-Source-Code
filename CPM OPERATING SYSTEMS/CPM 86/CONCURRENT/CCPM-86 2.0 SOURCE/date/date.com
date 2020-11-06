$ !
$ ! Compile, link, locate and generate hex for
$ ! 	'DATE'
$ ! Concurrent CP/M-86
$ !
$ util := DATE
$ ccpmsetup	! set up environment

$ plm86 'util'.plm xref 'p1' optimize(3) debug
$ link86 f1:scd.obj, 'util'.obj  to 'util'.lnk
$ loc86 'util'.lnk od(sm(code,dats,data,stack,const)) -
  ad(sm(code(0),dats(10000h))) ss(stack(+32)) to 'util'.
$ h86 'util'
$ !
$ !pclean
