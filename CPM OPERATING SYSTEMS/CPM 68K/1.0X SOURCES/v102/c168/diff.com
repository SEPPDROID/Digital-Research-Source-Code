$ assign drb0:[steve.cpm68k.v103.cgen] new:
$ set noon
$ dif :== diff/ign=(comm,blan,spac,trail,form)/comm=slash
$ dif CANON.C new:CANON.C
$ dif CGEN.H new:CGEN.H
$ dif CODEGEN.C new:CODEGEN.C
$ dif CSKEL.H new:CSKEL.H
$ dif CSKELS.C new:CSKELS.C
$ dif ICODE.H new:ICODE.H
$ dif INIT.C new:INIT.C
$ dif INTERF.C new:INTERF.C
$ dif MACHINE.11 new:MACHINE.11
$ dif MACHINE.68K new:MACHINE.68K
$ dif MACHINE.H new:MACHINE.H
$ dif MACHINE.VAX new:MACHINE.VAX
$ dif MAIN.C new:MAIN.C
$ dif OPTAB.C new:OPTAB.C
$ dif PUTEXPR.C new:PUTEXPR.C
$ dif SMATCH.C new:SMATCH.C
$ dif SUCOMP.C new:SUCOMP.C
$ dif TABL.C new:TABL.C
$ dif UTIL.C new:UTIL.C
$ dif VERSION.C new:VERSION.C