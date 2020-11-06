$ ! print volume 4 of CCP/M 2.0 source listings
$ control := /dev=lpa0/after=18:04
$ assign drb1:[ccpm86.commands] com:
$ assign drb1:[ccpm86.common] common:
$ assign drb1:[ccpm86.date] date:
$ assign drb1:[ccpm86.ddt86] ddt86:
$ assign drb1:[ccpm86.dir] dir:
$ assign drb1:[ccpm86.ed] ed:
$ assign drb1:[ccpm86.era] era:
$ assign drb1:[ccpm86.eraq] eraq:
$ assign drb1:[ccpm86.genccpm] genccpm:
$ assign drb1:[ccpm86.gencmd] gencmd:
$ assign drb1:[ccpm86.help] help:
$ assign drb1:[ccpm86.initdir] initdir:
$
$ print/nofeed'control' com:vol4.toc, -
	common:scd.lst, -
	date:date.lst,		date.mp2/feed, -
	ddt86:ddt86.sub/header,	gentab.prn/feed/header,	ins86.lst, -
				dis86.lst,	dislnk86.lst, -
				dis86.mp2/feed/header, -
				ass86.lst, 	asmtab.lst, -
				asslnk86.lst, 	ass86.mp2/feed/header, -
				ddt86.xrf, -
	dir:dircmd.lst,		dircmd.mp2/feed/header, -
	ed:ed.lst,		ed.mp2/feed/header, -
	era:era.lst,		era.mp2/feed/header, -
	eraq:eraq.lst,		eraq.mp2/feed/header, -
	genccpm:genccpm.xrf,	genccpm.map, -
	gencmd:gencmd.lst,	gencmd.mp2/feed/header, -
	help:help.lst,		help.mp2/feed/header, -
	initdir:initdir.xrf,	initdira.xrf,	initdir.sym/feed/header

