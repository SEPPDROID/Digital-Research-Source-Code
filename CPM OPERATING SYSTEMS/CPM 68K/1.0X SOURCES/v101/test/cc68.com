$ set noon
$ cp68   :==  $bin:c68.exe
$ c068   :==  $bin:c068.exe
$ c168   :==  $bin:c168.exe
$ as68   :==  $bin:as68.exe
$ lo68   :==  $bin:lo68.exe
$ 	cp68 -i lib: 'p1'.c 'p1'.i
$ 	c068 'p1'.i 'p1'.ic 'p1'.st
$ 	c168 'p1'.ic 'p1'.s -L
$ 	as68 -l -u 'p1'.s
$ 	delete 'p1'.s;*,'p1'.ic;*,'p1'.i;*,'p1'.st;*
