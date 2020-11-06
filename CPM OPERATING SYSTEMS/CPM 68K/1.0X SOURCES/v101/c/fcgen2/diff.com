$ set noon
$ set def DRB0:[STEVE.CPM68K.C.FCGEN2]
$ diff CANON.C [-.fcgen]CANON.C
$ diff CODEGEN.C [-.fcgen]CODEGEN.C
$ diff CSKELS.C [-.fcgen]CSKELS.C
$ diff INTERF.C [-.fcgen]INTERF.C
$ diff MAIN.C [-.fcgen]MAIN.C
$ diff OPTAB.C [-.fcgen]OPTAB.C
$ diff PUTEXPR.C [-.fcgen]PUTEXPR.C
$ diff SMATCH.C [-.fcgen]SMATCH.C
$ diff SUCOMP.C [-.fcgen]SUCOMP.C
$ diff TABL.C [-.fcgen]TABL.C
$ diff UTIL.C [-.fcgen]UTIL.C
$ diff VERSION.C [-.fcgen]VERSION.C
$ diff CGEN.H [-.fcgen]CGEN.H
$ diff CSKEL.H [-.fcgen]CSKEL.H
$ diff ICODE.H [-.fcgen]ICODE.H
