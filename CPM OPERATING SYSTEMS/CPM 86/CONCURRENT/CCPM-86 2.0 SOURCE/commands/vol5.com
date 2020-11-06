$ ! print volume 5 of CCP/M 2.0 source listings
$ control := /dev=lpa0/after=18:05
$ assign drb1:[ccpm86.commands] com:
$ assign drb1:[ccpm86.pip] pip:
$ assign drb1:[ccpm86.ren] ren:
$ assign drb1:[ccpm86.sdir] sdir:
$ assign drb1:[ccpm86.set] set:
$ assign drb1:[ccpm86.show] show:
$ assign drb1:[ccpm86.submit] subm:
$ assign drb1:[ccpm86.systat] systat:
$ assign drb1:[ccpm86.type] type:
$ assign drb1:[ccpm86.vcmode] vcmode:
$ home
$
$ print/nofeed'control' com:vol5.toc, -
	pip:pip.lst,		scd1.lst,	inpout.lst,	pip.mp2/feed, -
	ren:ren.lst,		ren.mp2/feed, -
	sdir:main86.lst,	scan.lst,	search.lst,	sort.lst, -
		disp.lst,	dpb86.lst,	util.lst,	timest.lst, -
		sdir.mp2/feed, -
	set:set.lst,		set.mp2/feed, -
	show:show.lst,	show.mp2/feed, -
	subm:submit.xrf, -
	systat:systat.lst,	systat.mp2/feed, -
	type:type.lst,	type.mp2/feed, -
	vcmode:vcmode.lst,	vcmode.mp2/feed
