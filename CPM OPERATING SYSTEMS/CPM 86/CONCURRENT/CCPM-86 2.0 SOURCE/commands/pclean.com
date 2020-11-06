$ !
$ ! clean up directory, keep last .LST and .MP2 files
$ !
$ delete *.jou;*, *.lin;*, *.sym;*, *.obj;*, *.lnk;*, *.dat;*
$ purge
$ purge sys$login:*.log
