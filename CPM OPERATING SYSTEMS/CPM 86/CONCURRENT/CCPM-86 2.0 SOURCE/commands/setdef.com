$ if p2 .nes. "" then goto l2
$ if p1 .eqs. "" then goto l3
$ if 'f$locate(":",p1)' .ne. 'f$length(p1)' then goto l1
$ if 'f$locate("[",p1)' .ne. 'f$length(p1)' then goto l1
$ if 'f$locate("<",p1)' .ne. 'f$length(p1)' then goto l1
$ set def ['p1']
$ goto l3
$l1:
$ set def 'p1
$ goto l3
$l2:
$ set def 'p1':['p2']
$l3:
$ show default
