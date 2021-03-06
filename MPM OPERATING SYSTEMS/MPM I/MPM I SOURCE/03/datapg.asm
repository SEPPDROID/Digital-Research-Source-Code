$title	('MP/M 1.1 Data Page')
	name	datapg
	dseg
;datapg:
;do;
;

;/*
;  Copyright (C) 1979,1980
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950
;
;  Revised:
;    18 Jan 80 by Thomas Rolander
;*/

;$include (proces.lit)
;$include (memmgr.lit)
;
;  declare tod structure (
tod:
	public	tod
;    day address,
;    hr byte,
;    min byte,
;    sec byte ) public
;    initial (761,00H,00H,00H);
	dw	761	; day
	db	0	; hr
	db	0	; min
	db	0	; sec
;    /* 01/31/80 00:00:00 */
;
;  declare datapg (1) byte public at (.tod);
datapg	equ	tod
	public	datapg
;
;  declare initpd process$descriptor external;
	extrn	initpd

;  declare rlr address public initial(initpd);
rlr:	dw	initpd
	public	rlr
;
;  declare dlr address public initial(0);
dlr:	dw	0
	public	dlr
;
;  declare drl address public initial(0);
drl:	dw	0
	public drl
;
;  declare plr address public initial(0);
plr:	dw	0
	public	plr
;
;  declare slr address public initial(0);
slr:	dw	0
	public	slr
;
;  declare qlr address public initial(0);
qlr:	dw	0
	public	qlr
;
;  declare thrdrt address public initial(0);
thrdrt:	dw	initpd
	public	thrdrt
;
;  declare max$cns literally '16';
maxcns	equ	16
;
;  declare nmb$cns byte public;
nmbcns:	ds	1
	public	nmbcns
;
;  declare console$attached (max$cns) address public
;    initial (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
;  declare cnsatt (1) address public at (.console$attached);
cnsatt:
	public	cnsatt
	dw	0,0,0,0
	dw	0,0,0,0
	dw	0,0,0,0
	dw	0,0,0,0
;
;  declare console$queue (max$cns) address public
;    initial (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
;  declare cnsque (1) address public at (.console$queue);
cnsque:
	public	cnsque
	dw	0,0,0,0
	dw	0,0,0,0
	dw	0,0,0,0
	dw	0,0,0,0
;
;  declare max$flgs literally '32';
maxflgs	equ	32
nmbflags:
	public	nmbflags
	db	maxflgs
;
;  declare sys$flag (max$flgs) address public initial (
;    0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,
;    0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,
;    0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,
;    0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH,0FFFFH);
;  declare sysfla address public at (.sys$flag);
sysfla:
	public	sysfla
	dw	0ffffh,0ffffh,0ffffh,0ffffh
	dw	0ffffh,0ffffh,0ffffh,0ffffh
	dw	0ffffh,0ffffh,0ffffh,0ffffh
	dw	0ffffh,0ffffh,0ffffh,0ffffh
	dw	0ffffh,0ffffh,0ffffh,0ffffh
	dw	0ffffh,0ffffh,0ffffh,0ffffh
	dw	0ffffh,0ffffh,0ffffh,0ffffh
	dw	0ffffh,0ffffh,0ffffh,0ffffh
;
;  declare max$usr$pr literally '8';
maxusrpr	equ	8
;
;  declare nmb$segs byte public
;    initial (max$usr$pr);
nmbsegs:
	public	nmbsegs
	db	maxusrpr
;
;  declare mem$seg$tbl (max$usr$pr) memory$descriptor public;
msegtbl:
	public	msegtbl
	db	0,0,0,0
	db	0,0,0,0
	db	0,0,0,0
	db	0,0,0,0
	db	0,0,0,0
	db	0,0,0,0
	db	0,0,0,0
	db	0,0,0,0
;  declare memseg memory$descriptor public at (.mem$seg$tbl);
memseg	equ	msegtbl
	public	memseg
;
;  declare pdtbl (max$usr$pr) process$descriptor public;
pdtbl:
	public	pdtbl
	ds	52
	ds	52
	ds	52
	ds	52
	ds	52
	ds	52
	ds	52
	ds	52
;
;
; *** Note:
;	The user process stack table has been moved to the MPM
;  module where it overlays the initialization code.
;
;  declare stktbl (max$usr$pr)
;    structure (loc (20) address) public;
;stktbl:
;	public	stktbl
;
;table of offsets
;
ostod		equ	tod-datapg
osrlr		equ	rlr-datapg
osdlr		equ	dlr-datapg
osdrl		equ	drl-datapg
osplr		equ	plr-datapg
osslr		equ	slr-datapg
osqlr		equ	qlr-datapg
osthrdrt	equ	thrdrt-datapg
osnmbcns	equ	nmbcns-datapg
oscnsatt	equ	cnsatt-datapg
oscnsque	equ	cnsque-datapg
osnmbflags	equ	nmbflags-datapg
ossysfla	equ	sysfla-datapg
osnmbsegs	equ	nmbsegs-datapg
osmsegtbl	equ	msegtbl-datapg
ospdtbl		equ	pdtbl-datapg
;osstktbl	equ	stktbl-datapg

;end datapg;
	end
