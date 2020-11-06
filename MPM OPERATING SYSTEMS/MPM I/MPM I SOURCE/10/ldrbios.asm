	title	'Skeleton MP/M 1.1 Ldrbios'


;	Copyright (C) 1978, 1979, 1980
;	Digital Research
;	Box 579, Pacific Grove
;	California, 93950

false	equ	0
true	equ	not false

ram$top	equ	1d00h	 	; top address+1
bios	equ	ram$top-0600h	; basic input/output system
bdos	equ	bios-0e00h	; base of the bdos

	org	bios


buff	equ	0080h	;default buffer address

;	jump vector for indiviual routines

	jmp	boot
wboote:	jmp	wboot
	jmp	const
	jmp	conin
	jmp	conout
	jmp	list
	jmp	punch
	jmp	reader
	jmp	home
	jmp	seldsk
	jmp	settrk
	jmp	setsec
	jmp	setdma
	jmp	read
	jmp	write
	jmp	list$st		; list status poll
	jmp	sect$tran	; sector translation


boot:
wboot:
gocpm:
	ret

crtin:			; crt: input
	ret
crtout:			; crt: output
	ret
crtst:			; crt: status
	ret
ttyin:			; tty: input
	ret
ttyout:			; tty: output
	ret
lptout:			; lpt: output
	ret
lpt$st:
	ret

conin	equ	crtin
const	equ	crtst
conout	equ	crtout
reader	equ	ttyin
punch	equ	ttyout
list	equ	lptout
listst	equ	lptst

seldsk:	;select disk given by register c
	ret
;
home:	;move to home position
	ret
;
settrk:	;set track number given by c
	ret
;
setsec:	;set sector number given by c
	ret
;
setdma:	;set dma address given by regs b,c
	ret
;
sect$tran:		; translate the sector # in <c> if needed
	ret
;
read:	;read next disk record (assuming disk/trk/sec/dma set)
	ret
;
write:	;disk write function
	ret
;
	end

