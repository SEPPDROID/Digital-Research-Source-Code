$ libe
$ set noon
$ !
$ !	Libe.a build file for VMS cross tools
$ !
$ ce68  ATOF
$ ce68  CEIL
$ ce68  ETOA
$ ce68  FABS
$ ce68  FLOOR
$ ce68  FMOD
$ ce68  FTOA
$ ce68  FTOL
$ ce68  IEEETOF
$ ce68  LTOF
$ ce68  XDOPRTFP
$ ce68  ATOI
$ as68 -l FFPABS.S 
$ as68 -l FFPADD.S 
$ as68 -l FFPCMP.S 
$ as68 -l FFPCPYRT.S 
$ as68 -l FFPDIV.S 
$ as68 -l FFPEXP.S 
$ as68 -l FFPHTHET.S 
$ as68 -l FFPIEEE.S 
$ as68 -l FFPLOG.S 
$ as68 -l FFPMUL2.S 
$ as68 -l FFPPWR.S 
$ as68 -l FFPSIN.S 
$ as68 -l FFPSQRT.S 
$ as68 -l FFPTHETA.S 
$ as68 -l FFPTNORM.S 
$ as68 -l FPADD.S 
$ as68 -l FPCMP.S 
$ as68 -l FPCOS.S 
$ as68 -l FPDIV.S 
$ as68 -l FPEXP.S 
$ as68 -l FPFTOL.S 
$ as68 -l FPLOG.S 
$ as68 -l FPLTOF.S 
$ as68 -l FPMUL.S 
$ as68 -l FPNEG.S 
$ as68 -l FPPWR.S 
$ as68 -l FPSIN.S 
$ as68 -l FPSQRT.S 
$ as68 -l FPSUB.S 
$ as68 -l IEFABS.S 
$ as68 -l IEFADD.S 
$ as68 -l IEFBACK.S 
$ as68 -l IEFCMP.S 
$ as68 -l IEFDIV.S 
$ as68 -l IEFEXP.S 
$ as68 -l IEFFPI.S 
$ as68 -l IEFFRONT.S 
$ as68 -l IEFIFP.S 
$ as68 -l IEFLOG.S 
$ as68 -l IEFMUL.S 
$ as68 -l IEFPWR.S 
$ as68 -l IEFSIN.S 
$ as68 -l IEFSQRT.S 
$ del  libE.a;*
$ ar68 r libE.a xdoprtfp.o
$ ar68 r libE.a ftoa.o etoa.o atof.o ieeetof.o fpadd.o fpcmp.o fpdiv.o 
$ ar68 r libE.a ftol.o ltof.o fabs.o floor.o ceil.o fmod.o fpmul.o 
$ ar68 r libE.a fpneg.o fpsub.o fpsin.o fpcos.o fpsqrt.o fpexp.o fplog.o 
$ ar68 r libE.a fppwr.o iefabs.o iefadd.o iefcmp.o iefdiv.o iefmul.o 
$ ar68 r libE.a iefsin.o iefsqrt.o iefpwr.o iefexp.o ieflog.o ieffront.o 
$ ar68 r libE.a iefback.o ffpieee.o ffpexp.o ffplog.o ffppwr.o ffpsin.o 
$ ar68 r libE.a ffpsqrt.o ffpabs.o ffpadd.o ffpcmp.o ffpdiv.o ffpmul2.o 
$ ar68 r libE.a ffptheta.o ffphthet.o ffptnorm.o ffpcpyrt.o atoi.o
$ del *.o;*
