$ !
$ ! this proc compiles, links and generates in hex the PIN process RSP
$ ! for Concurrent CP/M-86: vers IBM PC 1.0
$ ! It is assumed that the BATCH command is used for
$ ! setting the default directory
$ !
$ set verify
$ def sys$print nowhere		! log file goes to syslogin
$ newplm			! use NEW plm compiler
$ assign 'f$directory()' f1:
$ as86 rhpin.a86
$ as86 pxios.a86
$ pl86 pin.plm optimize(3) debug xref 'p1' 'p2' 'p3'
$ li86 rhpin.obj, pxios.obj, pin.obj to rhpin.lnk
$ ren rhpin.lnk pin.lnk
$ lo86 pin.lnk od(sm(code,dats,data,const,stack)) -
  ad(sm(code(0),dats(10000h))) ss(stack(0)) to pin.dat
$ newh86 pin.dat
$ pclean
