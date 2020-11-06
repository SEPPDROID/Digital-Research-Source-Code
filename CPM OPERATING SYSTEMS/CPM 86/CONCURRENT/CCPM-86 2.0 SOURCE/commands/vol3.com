$ ! print volume 3 of CCP/M 2.0 source listings
$ control := /dev=lpa0/after=18:03
$ assign drb1:[ccpm86.commands] com:
$ assign drb1:[ccpm86.asm86] ASM:
$ !
$ print/nofeed'control'		COM:vol3.toc, -
	ASM:asm86all.com/header, c86lnk.lst, - 
	*.ext/feed/header, -
	*.lit/feed/header, -
	*.x86/feed/header, -
	mainp.lst,	cmac1.lst,	cmac2.lst, -
	cmac3.lst,	cmac4.lst,	cmac5.lst, -
	mnem1.lst,	mnem2.lst,	mnem3.lst, -
	mnem4.lst,	symb.lst,	io.lst, -
	subr1.lst,	subr2.lst,	files.lst, -
	scan.lst,	print.lst,	predef.lst, -
	ermod.lst,	text.lst,	outp.lst, -
	expr.lst,	brexpr.lst,	pseud1.lst, -
	pseud2.lst,	cmsubr.lst,	instr.lst, -
	dline.lst,	global.lst,	cm.lst, -
	cm2.lst, -
	asm86.mp2/feed
$ cd [-]
