$ !module to compile everything for 
$ !	Concurrent CP/M-86

$ ! Note: those utilities marked 'doit' are compiled here, while those
$ !	marked 'micro' are handled elsewhere

$ set verify
$ doit := @com:doit.com
$ micro := @com:micro.com

$ doit  ABORT
$ !doit ASM86			!save time, don't compile this one
$ doit  DATE
$ micro DDT86
$ doit  DIR
$ doit  ED	
$ doit  ERA	
$ doit  ERAQ	
$ micro GENCCPM	
$ doit  GENCMD
$ doit  HELP	
$ micro INITDIR	
$ doit  PIN
$ doit  PIP	
$ micro PRINTER
$ doit  REN	
$ doit  SDIR	
$ doit  SET	
$ doit  SHOW	
$ micro SUBMIT	
$ doit  SYSTAT	
$ micro TMP	
$ doit  TYPE	
$ micro USER
$ doit  VCMODE	
$ doit  VOUT
$ !
$ micro SERIAL
$ !
