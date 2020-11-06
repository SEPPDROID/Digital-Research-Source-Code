$ cp68 :== $bin:cp68
$ c068 :== $bin:c068
$ c168 :== $bin:c168
$ as68 :== $bin:as68
$ lo68 :== $bin:lo68
$ set noon
$ cp68 'p1'.c 'p1'.i
$ c068 'p1'.i 'p1'.ic 'p1'.st -e
$ delete 'p1'.i;*,'p1'.st;*
$ c168 'p1'.ic 'p1'.s -l
$ delete 'p1'.ic;*
$ as68 -l -u 'p1'.s
$ delete 'p1'.s;*
