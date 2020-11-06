$! setup environment for working on PL/M-86 utilities for 
$!  Concurrent CP/M-86 v2.0	10/15/82 whf
$ v='f$verify(0)
$ set noverify
$ comon :== _DRB1:[CCPM86.COMMON]
$ @com:oldplm

$ !	$ lstring :== _drb1:[CCPM86.ccpm
$ !	$ assign 'lstring'.com] coms:
$ !	$ common :== set def comon
$ !	$ phome :== set def 'lstring']
$ !	$ pclean :== @coms:pclean	!cleans up PLM compile 

$ if v then $ set verify
