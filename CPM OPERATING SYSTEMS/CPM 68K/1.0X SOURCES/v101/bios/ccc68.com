$ set nover
$ set noon
$ c68    :==  $bin:c68.exe
$ c068   :==  $bin:c068.exe
$ c168   :==  $bin:c168.exe
$ as68   :==  $bin:as68.exe
$ lo68   :==  $bin:lo68.exe
$ c68 'p1'.c 'p1'.i
$ c068 'p1'.i 'p1'.ic 'p1'.st
$ c168 'p1'.ic 'p1'.s -LD
$ as68 -l -u -p 'p1'.s >'p1'.lis
$ delete 'p1'.s;*,'p1'.ic;*,'p1'.i;*,'p1'.st;*
