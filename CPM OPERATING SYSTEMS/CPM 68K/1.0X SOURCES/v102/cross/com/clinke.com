$ lo68 -r -o 'p1'.rel lib:s.o 'p1'.o 'p2'.o 'p3'.o 'p4'.o 'p5'.o 'p6'.o 'p7'.o 'p8'.o 'p9'.o lib:clib lib:libe.a
