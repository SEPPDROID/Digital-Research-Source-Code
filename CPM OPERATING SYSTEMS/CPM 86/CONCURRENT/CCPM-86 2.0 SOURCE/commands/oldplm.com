$ ! set up environs for PLM work: use old compiler
$ assign drb1:[CCPM86.VAXTOOLS] sys$mds

$ PLM86  :== $SYS$MDS:OPLM86
$ ASM86  :== $SYS$MDS:OASM86
$ LINK86 :== $SYS$MDS:OLINK86
$ LOC86  :== $SYS$MDS:OLOC86
$ !	$ LIB86  :== $SYS$MDS:LIB86
$ !	$ I2E    :== $SYS$MDS:I2E
$ !	$ E2I    :== $SYS$MDS:E2I
$ H86    :== $SYS$MDS:OH86
$ ASS SYS$INPUT: CI:
$ ASS SYS$OUTPUT: CO:
$ ASS 'COMON' F1:
$ ASS 'COMON' F2:
$ ASS 'COMON' F3:
$ ASS 'COMON' F4:
$ ASS 'COMON' F5:
$ ASS SYS$ERROR: FOR007
