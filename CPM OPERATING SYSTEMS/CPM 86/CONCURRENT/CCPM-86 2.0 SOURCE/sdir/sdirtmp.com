$ ccpmsetup
$ assign 'f$directory()' f1:
$
$ plm86 main86.plm debug object(main86) optimize(3) 'p1' 'p2' 'p3'
$ !plm86 scan.plm debug object(scan) optimize(3) 'p1' 'p2' 'p3'
$ !plm86 search.plm debug object(search) optimize(3) 'p1' 'p2' 'p3'
$ !plm86 sort.plm debug object(sort) optimize(3) 'p1' 'p2' 'p3'
$ plm86 disp.plm debug object(disp) optimize(3) 'p1' 'p2' 'p3'
$ !plm86 dpb86.plm debug object(dpb86) optimize(3) 'p1' 'p2' 'p3'
$ !plm86 util.plm debug object(util) optimize(3) 'p1' 'p2' 'p3'
$ !plm86 timest.plm debug object(timest) optimize(3) 'p1' 'p2' 'p3'
$ link86 f2:scd.obj,main86,scan,search,sort,disp,util,dpb86,timest -
  to sdir.lnk
$ loc86 sdir.lnk od(sm(code,dats,data,const,stack)) -
   ad(sm(code(0),dats(10000h))) ss(stack(+32)) to sdir.
$ h86 sdir

