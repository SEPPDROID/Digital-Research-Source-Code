$ !
$ ! Assemble, compile, link, locate, and generate hex files containing
$ ! DIR.CMD to run with
$ ! Concurrent CP/M-86 vers 2.0
$ ! Note: separate code and data
$ !
$ ccpmsetup
$ plm86 dircmd.plm 'p1' 'p2' 'p3' 'p4' xref optimize(3) debug
$ link86 f1:scd.obj, dircmd.obj  to dircmd.lnk
$ loc86 dircmd.lnk od(sm(code,dats,data,stack,const))-
  ad(sm(code(0), dats(10000h)))  ss(stack(+32)) to dircmd.
$ h86 dircmd
$ !
$ !
$ ! Assemble, compile, link, locate, and generate hex files containing
$ ! DIR.RSP to run with
$ ! Concurrent CP/M-86 vers 2.0
$ ! Note: separate code and data
$ !
$ asm86 rhdir.a86		!Rsp Header DIR
$ plm86 dirrsp.plm 'p1' 'p2' 'p3' 'p4' xref optimize(3) debug
$ link86 rhdir.obj, dirrsp.obj  to dirrsp.lnk
$ loc86 dirrsp.lnk od(sm(code,dats,data,stack,const))-
  ad(sm(code(0), dats(10000h)))  ss(stack(0)) to dirrsp.
$ h86 dirrsp
$ !
