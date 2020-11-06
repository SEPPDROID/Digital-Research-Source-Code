$ set noon
$ del lib:libF.a;*
$ ar68 rv lib:libF.a ftoa.o etoa.o atof.o ffptof.o ftoffp.o fabs.o floor.o 
$ ar68 rv lib:libF.a ceil.o fmod.o fpadd.o fpcmp.o fpdiv.o fpcos.o fppwr.o 
$ ar68 rv lib:libF.a fpsin.o fpsqrt.o fpexp.o fplog.o ltof.o ftol.o fpmul.o 
$ ar68 rv lib:libF.a fpneg.o fpsub.o ffppwr.o ffpsin.o ffpsqrt.o ffpabs.o 
$ ar68 rv lib:libF.a ffpadd.o ffpcmp.o ffpdiv.o ffpexp.o ffplog.o ffpmul2.o 
$ ar68 rv lib:libF.a ffptheta.o ffptnorm.o ffphthet.o ffpcpyrt.o
