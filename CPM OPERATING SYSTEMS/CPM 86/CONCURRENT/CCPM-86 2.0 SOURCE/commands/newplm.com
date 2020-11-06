$ ! set up environs for PLM work: use NEW compiler
$!  Concurrent CP/M-86 v2.0	10/15/82 whf
$ v='f$verify(0)
$ set noverify
$ comon :== _DRB1:[CCPM86.COMMON]
$ assign drb1:[CCPM86.VAXTOOLS] SYS$BETA

$ NEWPLM86	:==	"$ SYS$BETA:nplm86 PLM86 "
$ NEWLINK86	:==	"$ SYS$BETA:nlink86 LINK86 "
$ NEWLOC86	:==	"$ SYS$BETA:nloc86 LOC86 "
$ NEWASM86	:==	"$ SYS$BETA:nasm86 ASM86 "
$ NHEX86	:==	"$ SYS$BETA:nhex86"
$ AS86		:==	@ sys$beta:NAS86
$ PL86		:==	@ sys$beta:NPL86
$ LO86 		:==	@ sys$beta:NLO86 
$ LI86 		:==	@ sys$beta:NLI86
$ NEWH86	:==	@ sys$beta:NEWH86
$ REFMT 	:==	@ sys$beta:NREFMT
$ PCLEAN	:==	@ com:PCLEAN
$ ! NEWLIB86:=="$SYS$BETA:LIB86 LIB86 "
$ ! NEWREL2:=="$SYS$BETA:REL2 REL2 "
$ ! ASS SYS$BETA:PLM86.LIB NEWLIB
$ ! ASS SYS$BETA: PLM$UDI
$ ASS SYS$INPUT: CI:
$ ASS SYS$OUTPUT: CO:
$ ASS 'COMON' F1:
$ ASS 'COMON' F2:
$ ASS 'COMON' F3:
$ ASS 'COMON' F4:
$ ASS 'COMON' F5:
$ ASS SYS$ERROR: FOR007

$ if v then $ set verify
