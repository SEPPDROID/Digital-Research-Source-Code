$ set noon
$ copy c:[cpm68k.release.bdos]*.c,*.s,*.h,*.com,*.doc [.cbdos]
$ pur  [.cbdos]
$ copy c:[cpm68k.release.boot]*.c,*.s,*.h,*.com [.boot]
$ pur  [.boot]
$ copy c:[cpm68k.release.ccp]*.c,*.s,*.h,*.com [.ccp]
$ pur  [.ccp]
$ copy c:[cpm68k.release.ddt]*.c,*.s,*.h,*.com [.ddt]
$ pur  [.ddt]
$ copy b:[cpm68k.release.bios]*.c,*.s,*.h,*.com [.bios]
$ pur  [.bios]
$ copy c:[cpm68k.release.pipedstat]*.* [.utils]
$ copy c:[cpm68k.release.tools]*.* [.utils]
$ copy c:[cpm68k.release.serial]*.* [.utils]
$ pur  [.utils]
$ copy c:[cpm68k.release.clib]*.* [.clib]
$ pur [.clib]
$ copy c:[cpm68k.release.relnotes]*.* [.doc]
$ copy c:[cpm68k.release.docs]*.* [.doc]
$ pur [.doc]
