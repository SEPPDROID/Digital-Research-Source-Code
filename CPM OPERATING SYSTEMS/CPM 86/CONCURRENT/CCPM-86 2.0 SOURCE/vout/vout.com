$ !
$ ! this proc compiles, links and generates in hex the VOUT process RSP
$ ! for Concurrent CP/M-86: vers IBM PC 1.0
$ ! It is assumed that the BATCH command is used for
$ ! setting the default directory
$ !
$ set verify
$ def sys$print nowhere		! log file goes to syslogin
$ newplm			! use NEW plm compiler
$ as86 rhvout.a86
$ as86 pxios.a86
$ pl86 vout.plm optimize(3) debug xref 'p1' 'p2' 'p3'
$ li86 rhvout.obj, pxios.obj, vout.obj to rhvout.lnk
$ ren rhvout.lnk vout.lnk
$ lo86 vout.lnk od(sm(code,dats,data,const,stack)) -
  ad(sm(code(0),dats(10000h))) ss(stack(0)) to vout.dat
$ newh86 vout.dat
$ pclean
