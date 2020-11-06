$ set def [frank.mpm86.asm86]
$ plm86 mainp.plm debug optimize(2) 'p1' xref   
$ plm86 cmac1.plm debug optimize(2) 'p1'   
$ plm86 cmac2.plm debug optimize(2) 'p1'   
$ plm86 cmac3.plm debug optimize(2) 'p1'   
$ plm86 cmac4.plm debug optimize(2) 'p1'   
$ plm86 cmac5.plm debug optimize(2) 'p1'   
$ plm86 mnem1.plm debug optimize(2) 'p1'   
$ plm86 mnem2.plm debug optimize(2) 'p1'   
$ plm86 mnem3.plm debug optimize(2) 'p1'   
$ plm86 mnem4.plm debug optimize(2) 'p1'   
$ plm86 symb.plm debug optimize(2) 'p1' xref   
$ plm86 io.plm debug optimize(2) 'p1' xref   
$ plm86 subr1.plm debug optimize(2) 'p1' xref   
$ plm86 subr2.plm debug optimize(2) 'p1' xref   
$ plm86 files.plm debug optimize(2) 'p1' xref   
$ plm86 scan.plm debug optimize(2) 'p1' xref   
$ plm86 print.plm debug optimize(2) 'p1' xref   
$ plm86 predef.plm debug optimize(2) 'p1' xref  
$ plm86 ermod.plm debug optimize(2) 'p1' xref  
$ plm86 text.plm debug optimize(2) 'p1' xref  
$ plm86 outp.plm debug optimize(2) 'p1' xref  
$ plm86 expr.plm debug optimize(2) 'p1' xref  
$ plm86 brexpr.plm debug optimize(2) 'p1' xref  
$ plm86 pseud1.plm debug optimize(2) 'p1' xref  
$ plm86 pseud2.plm debug optimize(2) 'p1' xref  
$ plm86 cmsubr.plm debug optimize(2) 'p1' xref  
$ plm86 instr.plm debug optimize(2) 'p1' xref  
$ plm86 dline.plm debug optimize(2) 'p1' xref  
$ plm86 global.plm debug optimize(2) 'p1' xref  
$ plm86 cm.plm debug optimize(2) 'p1' xref  
$ plm86 cm2.plm debug optimize(2) 'p1' xref  
$ asm86 c86lnk.asm debug 
$ link86 cmac1.obj,cmac2.obj,cmac3.obj,cmac4.obj,cmac5.obj to f11.mod
$ link86 mnem1.obj,mnem2.obj,mnem3.obj,mnem4.obj,symb.obj to f12.mod
$ link86 io.obj,subr1.obj,subr2.obj,files.obj,scan.obj to f13.mod
$ link86 print.obj,predef.obj,ermod.obj,text.obj,outp.obj to f14.mod
$ link86 expr.obj,brexpr.obj,pseud1.obj,pseud2.obj,cmsubr.obj to f15.mod
$ link86 instr.obj,dline.obj,global.obj,cm.obj,cm2.obj to f16.mod
$ link86 f11.mod,f12.mod,f13.mod to f21.mod
$ link86 f14.mod,f15.mod,f16.mod to f22.mod
$ link86 c86lnk.obj,mainp.obj,f21.mod,f22.mod,plm86.lib to asm86.mod
$ loc86 asm86.mod nopublics ad(sm(code(0))) od(sm(code,const,stack))
$ h86 asm86
! on micro under cp/m or mp/m
!
! determine BBB and MMM from asm86.mp2 file
! BBB = start of const segment / 16
! MMM = (start of memory segment - start of const segment)/ 16 + 100h
!          (4K for sysmbol table)
!
! gencmd asm86 data[bBBB,mMMM,xfff]
!    which turns out to be:
! gencmd asm86 data[4ad, m44e, xfff]

