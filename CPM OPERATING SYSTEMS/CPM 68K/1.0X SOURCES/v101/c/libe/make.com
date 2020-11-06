$ set def [steve.cpm68k.c.libe]
$ set noon
$ cc68f ATOF
$ cc68f CEIL
$ cc68f ETOA
$ cc68f FABS
$ cc68f FLOOR
$ cc68f FMOD
$ cc68f FPRINTF
$ cc68f FTOA
$ cc68f FTOL
$ cc68f IEEETOF
$ cc68f LTOF
$ cc68f PRINTF
$ as68 -l -p FFPABS.S >FFPABS.LIS
$ as68 -l -p FFPADD.S >FFPADD.LIS
$ as68 -l -p FFPCMP.S >FFPCMP.LIS
$ as68 -l -p FFPCPYRT.S >FFPCPYRT.LIS
$ as68 -l -p FFPDIV.S >FFPDIV.LIS
$ as68 -l -p FFPEXP.S >FFPEXP.LIS
$ as68 -l -p FFPHTHET.S >FFPHTHET.LIS
$ as68 -l -p FFPIEEE.S >FFPIEEE.LIS
$ as68 -l -p FFPLOG.S >FFPLOG.LIS
$ as68 -l -p FFPMUL2.S >FFPMUL2.LIS
$ as68 -l -p FFPPWR.S >FFPPWR.LIS
$ as68 -l -p FFPSIN.S >FFPSIN.LIS
$ as68 -l -p FFPSQRT.S >FFPSQRT.LIS
$ as68 -l -p FFPTHETA.S >FFPTHETA.LIS
$ as68 -l -p FFPTNORM.S >FFPTNORM.LIS
$ as68 -l -p FPADD.S >FPADD.LIS
$ as68 -l -p FPCMP.S >FPCMP.LIS
$ as68 -l -p FPCOS.S >FPCOS.LIS
$ as68 -l -p FPDIV.S >FPDIV.LIS
$ as68 -l -p FPEXP.S >FPEXP.LIS
$ as68 -l -p FPFTOL.S >FPFTOL.LIS
$ as68 -l -p FPLOG.S >FPLOG.LIS
$ as68 -l -p FPLTOF.S >FPLTOF.LIS
$ as68 -l -p FPMUL.S >FPMUL.LIS
$ as68 -l -p FPNEG.S >FPNEG.LIS
$ as68 -l -p FPPWR.S >FPPWR.LIS
$ as68 -l -p FPSIN.S >FPSIN.LIS
$ as68 -l -p FPSQRT.S >FPSQRT.LIS
$ as68 -l -p FPSUB.S >FPSUB.LIS
$ as68 -l -p IEFABS.S >IEFABS.LIS
$ as68 -l -p IEFADD.S >IEFADD.LIS
$ as68 -l -p IEFBACK.S >IEFBACK.LIS
$ as68 -l -p IEFCMP.S >IEFCMP.LIS
$ as68 -l -p IEFDIV.S >IEFDIV.LIS
$ as68 -l -p IEFEXP.S >IEFEXP.LIS
$ as68 -l -p IEFFPI.S >IEFFPI.LIS
$ as68 -l -p IEFFRONT.S >IEFFRONT.LIS
$ as68 -l -p IEFIFP.S >IEFIFP.LIS
$ as68 -l -p IEFLOG.S >IEFLOG.LIS
$ as68 -l -p IEFMUL.S >IEFMUL.LIS
$ as68 -l -p IEFPWR.S >IEFPWR.LIS
$ as68 -l -p IEFSIN.S >IEFSIN.LIS
$ as68 -l -p IEFSQRT.S >IEFSQRT.LIS