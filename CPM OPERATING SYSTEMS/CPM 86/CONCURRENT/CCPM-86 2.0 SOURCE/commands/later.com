$	set noverify
$	COMDIR := DRB1:[CCPM86.COMMANDS]
$	default := ""
$	if ('f$locate(":",p1)' .eq. 'f$length(p1)')  .and. -
	   ('f$locate("[",p1)' .eq. 'f$length(p1)')	then -
						default := 'f$directory()'
$	open/error=TRYCOM tmpfi: 'default''p1'.com
$	close tmpfi:
$	goto DOCOPY
$ TRYCOM:
$	open/error=NOFIND tmpfi: 'COMDIR''p1'.com
$	close tmpfi:
$	default := 'COMDIR'
$	goto DOCOPY
$ NOFIND:
$	write sys$output can not find 'p1'
$	goto FINI
$ DOCOPY:
$	copy 'default''p1'.com sys$login:'p1'.com
$	submit sys$login:'p1'/after=18:30:00/delete/parameters= -
		("''p2'","''p3'","''p4'","''p5'","''p6'","''p7'","''p8'", -
					"''f$directory()'")
$ FINI:
