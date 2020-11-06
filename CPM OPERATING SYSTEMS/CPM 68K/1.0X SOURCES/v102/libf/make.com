$ set noon
$ libf
$ !
$ !	Build file for libf.a for VMS cross tools
$ !
$ cc68 ATOF
$ cc68 CEIL
$ cc68 ETOA
$ cc68 FABS
$ cc68 FFPTOF
$ cc68 FLOOR
$ cc68 FMOD
$ cc68 FTOA
$ cc68 FTOFFP
$ cc68 FTOL
$ cc68 LTOF
$ cc68 XDOPRTFP
$ cc68 ATOI
$ as68 -l  FFPABS.S 
$ as68 -l  FFPADD.S 
$ as68 -l  FFPCMP.S 
$ as68 -l  FFPCPYRT.S 
$ as68 -l  FFPDIV.S 
$ as68 -l  FFPEXP.S 
$ as68 -l  FFPHTHET.S 
$ as68 -l  FFPLOG.S 
$ as68 -l  FFPMUL2.S 
$ as68 -l  FFPPWR.S 
$ as68 -l  FFPSIN.S 
$ as68 -l  FFPSQRT.S 
$ as68 -l  FFPTHETA.S 
$ as68 -l  FFPTNORM.S 
$ as68 -l  FPADD.S 
$ as68 -l  FPCMP.S 
$ as68 -l  FPCOS.S 
$ as68 -l  FPDIV.S 
$ as68 -l  FPEXP.S 
$ as68 -l  FPFTOL.S 
$ as68 -l  FPLOG.S 
$ as68 -l  FPLTOF.S 
$ as68 -l  FPMUL.S 
$ as68 -l  FPNEG.S 
$ as68 -l  FPPWR.S 
$ as68 -l  FPSIN.S 
$ as68 -l  FPSQRT.S 
$ as68 -l  FPSUB.S 
$ del libf.a;*
$ ar68 r  libF.a xdoprtfp.o
$ ar68 r  libF.a ftoa.o etoa.o atof.o ffptof.o ftoffp.o fabs.o floor.o 
$ ar68 r  libF.a ceil.o fmod.o fpadd.o fpcmp.o fpdiv.o fpcos.o fppwr.o 
$ ar68 r  libF.a fpsin.o fpsqrt.o fpexp.o fplog.o ltof.o ftol.o fpmul.o 
$ ar68 r  libF.a fpneg.o fpsub.o ffppwr.o ffpsin.o ffpsqrt.o ffpabs.o 
$ ar68 r  libF.a ffpadd.o ffpcmp.o ffpdiv.o ffpexp.o ffplog.o ffpmul2.o 
$ ar68 r  libF.a ffptheta.o ffptnorm.o ffphthet.o ffpcpyrt.o atoi.o
$ del *.o;*
