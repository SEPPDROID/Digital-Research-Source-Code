$title	('MP/M Version & Revision Date')
	name	ver
;
;  MP/M 1.1   Version
;

;/*
;  Copyright (C) 1979,1980
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950
;
;  Revised:
;    27 Jan 80 by Thomas Rolander
;*/

	cseg
;
	extrn	mpm
startmpm:
	jmp	mpm

	public	sysdat
sysdat:
	dw	$-$

copyright:
	db	'COPYRIGHT (C) 1980,'
	db	' DIGITAL RESEARCH '

serial:
	db	'654321'

	public	ver
ver:
	db	0dh,0ah,0ah
	db	'MP/M '
	db	'1.1'
;	db	'$'
	db	' '
;	db	'                '
	db	'[27 Jan 80 9:45]'
	db	'$       '

	end	startmpm
