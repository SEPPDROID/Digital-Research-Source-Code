$ diff/out=CEXPR.DIF CEXPR.C [-.orgcp68]CEXPR.C
$ diff/out=LEX.DIF   LEX.C [-.orgcp68]LEX.C
$ diff/out=MACRO.DIF MACRO.C [-.orgcp68]MACRO.C
$ diff/out=MAIN.DIF  MAIN.C [-.orgcp68]MAIN.C
$ diff/out=UTIL.DIF  UTIL.C [-.orgcp68]UTIL.C
$ diff/out=PREPROC.DIF PREPROC.H [-.orgcp68]PREPROC.H
