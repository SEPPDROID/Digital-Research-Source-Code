$!
$! Assemble, compile, link, locate, and generate hex
$! for ABORT.RSP to run in
$! Concurrent CP/M-86, vers 2.0
$! 
$ ccpmsetup
$
$ asm86 rhabt.a86		!Rsp Header ABorT
$ plm86 rabt.plm 'p1' 'p2' 'p3' xref optimize(3) debug	!Rsp ABorT
$ link86 rhabt.obj, rabt.obj to rabt.lnk
$ loc86 rabt.lnk od(sm(code,dats,data,stack,const))-
  ad(sm(code(0), dats(10000h)))  ss(stack(0)) to rabt.
$ h86 rabt
$!
