$ set def [steve.cpm68k.c.fcgen]
$ diff CANON.C [-.cgen]CANON.C
$ diff CODEGEN.C [-.cgen]CODEGEN.C
$ diff INIT.C [-.cgen]INIT.C
$ diff INTERF.C [-.cgen]INTERF.C
$ diff MAIN.C [-.cgen]MAIN.C
$ diff OPTAB.C [-.cgen]OPTAB.C
$ diff PUTEXPR.C [-.cgen]PUTEXPR.C
$ diff SMATCH.C [-.cgen]SMATCH.C
$ diff SUCOMP.C [-.cgen]SUCOMP.C
$ diff TABL.C [-.cgen]TABL.C
$ diff UTIL.C [-.cgen]UTIL.C
$ diff VERSION.C [-.cgen]VERSION.C
$ diff CGEN.H [-.cgen]CGEN.H
$ diff CSKEL.H [-.cgen]CSKEL.H
$ diff ICODE.H [-.cgen]ICODE.H
