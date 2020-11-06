$ set noon
$ del  lib:libE.a
$ ar68 rv lib:libE.a ftoa.o etoa.o atof.o ieeetof.o fpadd.o fpcmp.o fpdiv.o 
$ ar68 rv lib:libE.a ftol.o ltof.o fabs.o floor.o ceil.o fmod.o fpmul.o 
$ ar68 rv lib:libE.a fpneg.o fpsub.o fpsin.o fpcos.o fpsqrt.o fpexp.o fplog.o 
$ ar68 rv lib:libE.a fppwr.o iefabs.o iefadd.o iefcmp.o iefdiv.o iefmul.o 
$ ar68 rv lib:libE.a iefsin.o iefsqrt.o iefpwr.o iefexp.o ieflog.o ieffront.o 
$ ar68 rv lib:libE.a iefback.o ffpieee.o ffpexp.o ffplog.o ffppwr.o ffpsin.o 
$ ar68 rv lib:libE.a ffpsqrt.o ffpabs.o ffpadd.o ffpcmp.o ffpdiv.o ffpmul2.o 
$ ar68 rv lib:libE.a ffptheta.o ffphthet.o ffptnorm.o ffpcpyrt.o
