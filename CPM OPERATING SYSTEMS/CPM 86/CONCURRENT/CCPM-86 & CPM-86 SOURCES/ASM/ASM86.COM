$ !
$ ! Compile, link, locate and generate hex files containing
$ ! ASM86 for
$ ! Concurrent CP/M-86: vers IBM PC 1.0
$ ! uses old PL/M-86 compiler
$ !
$ set verify
$ assign 'f$directory()' f1		! force TMP files to local dir
$ oldplm86 mainp.plm debug optimize(2) 'p1' xref   
$ oldplm86 cmac1.plm debug optimize(2) 'p1'   
$ oldplm86 cmac2.plm debug optimize(2) 'p1'   
$ oldplm86 cmac3.plm debug optimize(2) 'p1'   
$ oldplm86 cmac4.plm debug optimize(2) 'p1'   
$ oldplm86 cmac5.plm debug optimize(2) 'p1'   
$ oldplm86 mnem1.plm debug optimize(2) 'p1'   
$ oldplm86 mnem2.plm debug optimize(2) 'p1'   
$ oldplm86 mnem3.plm debug optimize(2) 'p1'   
$ oldplm86 mnem4.plm debug optimize(2) 'p1'   
$ oldplm86 symb.plm debug optimize(2) 'p1' xref   
$ oldplm86 io.plm debug optimize(2) 'p1' xref   
$ oldplm86 subr1.plm debug optimize(2) 'p1' xref   
$ oldplm86 subr2.plm debug optimize(2) 'p1' xref   
$ oldplm86 files.plm debug optimize(2) 'p1' xref   
$ oldplm86 scan.plm debug optimize(2) 'p1' xref   
$ oldplm86 print.plm debug optimize(2) 'p1' xref   
$ oldplm86 predef.plm debug optimize(2) 'p1' xref  
$ oldplm86 ermod.plm debug optimize(2) 'p1' xref  
$ oldplm86 text.plm debug optimize(2) 'p1' xref  
$ oldplm86 outp.plm debug optimize(2) 'p1' xref  
$ oldplm86 expr.plm debug optimize(2) 'p1' xref  
$ oldplm86 brexpr.plm debug optimize(2) 'p1' xref  
$ oldplm86 pseud1.plm debug optimize(2) 'p1' xref  
$ oldplm86 pseud2.plm debug optimize(2) 'p1' xref  
$ oldplm86 cmsubr.plm debug optimize(2) 'p1' xref  
$ oldplm86 instr.plm debug optimize(2) 'p1' xref  
$ oldplm86 dline.plm debug optimize(2) 'p1' xref  
$ oldplm86 global.plm debug optimize(2) 'p1' xref  
$ oldplm86 cm.plm debug optimize(2) 'p1' xref  
$ oldplm86 cm2.plm debug optimize(2) 'p1' xref
$ oldasm86 c86lnk.asm debug 
$ oldlink86 cmac1.obj,cmac2.obj,cmac3.obj,cmac4.obj,cmac5.obj to f11.mod
$ oldlink86 mnem1.obj,mnem2.obj,mnem3.obj,mnem4.obj,symb.obj to f12.mod
$ oldlink86 io.obj,subr1.obj,subr2.obj,files.obj,scan.obj to f13.mod
$ oldlink86 print.obj,predef.obj,ermod.obj,text.obj,outp.obj to f14.mod
$ oldlink86 expr.obj,brexpr.obj,pseud1.obj,pseud2.obj,cmsubr.obj to f15.mod
$ oldlink86 instr.obj,dline.obj,global.obj,cm.obj,cm2.obj to f16.mod
$ oldlink86 f11.mod,f12.mod,f13.mod to f21.mod
$ oldlink86 f14.mod,f15.mod,f16.mod to f22.mod
$ oldlink86 c86lnk.obj,mainp.obj,f21.mod,f22.mod to asm86.mod
$ oldloc86 asm86.mod nopublics ad(sm(code(0))) od(sm(code,const,stack))
$ oldh86 asm86
$ assign [ccpmpc.vax.common] f1
$ ren asm86.mp1 asm86.111
$ del *.mp1;*
$ ren asm86.111 asm86.mp1
$ pclean
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
