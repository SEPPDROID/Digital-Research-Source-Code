$ set def drb0:[steve.cpm68k.v102a.libf]
$ set noon
$ num
ATOF.c
ATOF.lis
$ num
CEIL.c
CEIL.lis
$ num
ETOA.c
ETOA.lis
$ num
FABS.c
FABS.lis
$ num
FFPTOF.c
FFPTOF.lis
$ num
FLOOR.c
FLOOR.lis
$ num
FMOD.c
FMOD.lis
$ num
FTOA.c
FTOA.lis
$ num
FTOFFP.c
FTOFFP.lis
$ num
FTOL.c
FTOL.lis
$ num
LTOF.c
LTOF.lis
$ num
XDOPRTFP.c
XDOPRTFP.lis
$ num
ATOI.c
ATOI.lis
$ as68 -l -p  FFPABS.s >FFPABS.lis
$ as68 -l -p  FFPADD.s >FFPADD.lis
$ as68 -l -p  FFPCMP.s >FFPCMP.lis
$ as68 -l -p  FFPCPYRT.s >FFPCPYRT.lis
$ as68 -l -p  FFPDIV.s >FFPDIV.lis
$ as68 -l -p  FFPEXP.s >FFPEXP.lis
$ as68 -l -p  FFPHTHET.s >FFPHTHET.lis
$ as68 -l -p  FFPLOG.s >FFPLOG.lis
$ as68 -l -p  FFPMUL2.s >FFPMUL2.lis
$ as68 -l -p  FFPPWR.s >FFPPWR.lis
$ as68 -l -p  FFPSIN.s >FFPSIN.lis
$ as68 -l -p  FFPSQRT.s >FFPSQRT.lis
$ as68 -l -p  FFPTHETA.s >FFPTHETA.lis
$ as68 -l -p  FFPTNORM.s >FFPTNORM.lis
$ as68 -l -p  FPADD.s >FPADD.lis
$ as68 -l -p  FPCMP.s >FPCMP.lis
$ as68 -l -p  FPCOS.s >FPCOS.lis
$ as68 -l -p  FPDIV.s >FPDIV.lis
$ as68 -l -p  FPEXP.s >FPEXP.lis
$ as68 -l -p  FPFTOL.s >FPFTOL.lis
$ as68 -l -p  FPLOG.s >FPLOG.lis
$ as68 -l -p  FPLTOF.s >FPLTOF.lis
$ as68 -l -p  FPMUL.s >FPMUL.lis
$ as68 -l -p  FPNEG.s >FPNEG.lis
$ as68 -l -p  FPPWR.s >FPPWR.lis
$ as68 -l -p  FPSIN.s >FPSIN.lis
$ as68 -l -p  FPSQRT.s >FPSQRT.lis
$ as68 -l -p  FPSUB.s >FPSUB.lis
$ pri/del/nofee *.lis
$ del *.o;*
