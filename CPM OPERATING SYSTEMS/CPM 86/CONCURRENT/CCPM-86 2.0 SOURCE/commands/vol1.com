$ ! print volume 1 of CCP/M 2.0 source listings
$ control := /dev=lpa0/after=18:01
$ assign drb1:[ccpm86.kern] kern:
$
$ home
$ print/nofeed'control' com:vol1.toc, -
	kern:csup.xrf, crtm.xrf, cmem.xrf, ccio.xrf, csysdat.xrf
