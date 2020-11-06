$ ! print volume 2 of CCP/M 2.0 source listings
$ control := /dev=lpa0/after=18:02
$ assign drb1:[ccpm86.commands] com:
$ assign drb1:[ccpm86.bdos] bdos:
$ assign drb1:[ccpm86.xios] xios:
$ assign drb1:[ccpm86.loader] loader:
$ assign drb1:[ccpm86.abort] abort:
$ assign drb1:[ccpm86.clock] clock:
$ assign drb1:[ccpm86.dir] dir:
$ assign drb1:[ccpm86.echo] echo:
$ assign drb1:[ccpm86.pin] pin:
$ assign drb1:[ccpm86.tmp] tmp:
$ assign drb1:[ccpm86.vout] vout:
$
$ home
$ print/nofeed'control'		com:vol2.toc, -
	bdos:cbdos.xrf, -
	xios:xios.xrf, -
	loader:boot.xrf,	lbdos.xrf,		load.xrf, -
		tcopy.xrf, -	
	abort:rabt.lst,		rabt.mp2/feed/header,	rhabt.lst, -
	clock:clock.xrf, -
	dir:dirrsp.lst,		dirrsp.mp2/feed/header,	rhdir.lst, -
	echo:echo.xrf, -
	pin:pin.lst,		pin.mp2/feed/header,	rhpin.lst, -
		pxios.lst, -
	tmp:tmp.xrf, -
	vout:vout.lst,		vout.mp2/feed/header,	rhvout.lst, -
		pxios.lst
