$ set noon
$ cc68 test
$ @rear
$ lo68 -r startup.o test.o lib6.a
$ nm68 c.out >test.map
$ print test.map
$ conv
c.out
test.asc
$ ! end
