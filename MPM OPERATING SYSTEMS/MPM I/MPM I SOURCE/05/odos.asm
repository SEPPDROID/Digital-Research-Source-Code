	title	'Resident Portion of Banked BDOS'
;***************************************************************
;***************************************************************
;**                                                           **
;**   B a s i c    D i s k   O p e r a t i n g   S y s t e m  **
;**                                                           **
;**  R e s i d e n t  P o r t i o n  -  B a n k e d  B D O S  **
;**                                                           **
;***************************************************************
;***************************************************************

;/*
;  Copyright (C) 1978,1979,1980
;  Digital Research
;  P.O. Box 579
;  Pacific Grove, CA 93950
;
;  Revised:
;    11 Jan 80 by Thomas Rolander
;*/

on	equ	0ffffh
off	equ	00000h
;
	org	0000h
base	equ	$
;
;	XDOS jump table below BDOS origin
pdisp	equ	$-3
xdos	equ	pdisp-3

;	equates for non graphic characters
ctlc	equ	03h	;control c, abort
ctld	equ	04h	;control d, detach
ctle	equ	05h	;physical eol
ctlh	equ	08h	;backspace
ctlp	equ	10h	;prnt toggle
ctlq	equ	11h	;prnt owner toggle
ctlr	equ	12h	;repeat line
ctls	equ	13h	;stop/start screen
ctlu	equ	15h	;line delete
ctlx	equ	18h	;=ctl-u
ctlz	equ	1ah	;end of file
rubout	equ	7fh	;char delete
tab	equ	09h	;tab char
cr	equ	0dh	;carriage return
lf	equ	0ah	;line feed
ctl	equ	5eh	;up arrow
;
;	BDOS call equates
dskinit		equ	13
dskslct		equ	14
dsksetDMA	equ	26
setusercode	equ	32
;
;	XDOS call equates
poll		equ	131
makeque		equ	134
readque		equ	137
condreadque	equ	138
writeque	equ	139
condwriteque	equ	140
dispatch	equ	142
terminate	equ	143
attach		equ	146
detach		equ	147
sysdtadr	equ	154
;
;	Process Descriptor Offsets
pname		equ	6
console		equ	14
memseg		equ	15
thread		equ	18
disksetDMA	equ	20
diskselect	equ	22
diskparams	equ	23
searcha		equ	26
drvact		equ	28
;
;	System Data Page Offsets
userstackflag	equ	3
brkvctr		equ	48
usrstkregion	equ	80
;
;	MP/M Data Page Offsets
rlros		equ	5
thrdrt		equ	17
cnsatt		equ	20
msegtbl		equ	150
;
;*****
;
;	enter here from the user's program with function number
;	in c, and information address in d,e
	jmp	xbdos	;to xbdos handler
	jmp	bios	;to bios jump table base address
;
;	bdos/xdos initialization code
syinit:
	;HL = sysdat page address
	shld sysdat
	mvi l,7! lxi b,bnkbdos
	mov a,m! mov e,a! stax b! inx b! inx h
	mov a,m! mov d,a! stax b
	xchg! inx h! inx h! inx h
	shld bnkbdosinit
bnkbdosinit equ $+1
	call $-$	 ;initialize the banked bdos
	jmp syinitcontd

extjmptbl:
	jmp	extreboot
	jmp	rlr
	jmp	getrlradr
	jmp	dsplynm
	jmp	xprint
	jmp	xcrlf
	jmp	conoutx
	jmp	getmemseg
	jmp	coninf
	jmp	homef
	jmp	seldskf
	jmp	settrkf
	jmp	setsecf
	jmp	setdmaf
	jmp	extreadf
	jmp	extwritef
	jmp	sectran

syinitcontd:
	lhld sysdat! mov c,h
	mvi l,252! mov a,m! inx h! mov h,m
	mov l,a! lxi d,0005! dad d
	shld rlradr! mov h,c
	mvi l,userstackflag! mov a,m! sta usrstkflg
	dcr l! mov l,m! mvi h,0 ;HL = RST #
	mov c,l ;put RST # into C for use by SYSINITF
	dad h! dad h! dad h ;HL = breakpoint address
	mvi m,0C3h! inx h ;store jump to brkpt hndlr
	lxi d,brkpt ;DE = brkpt entry address
	mov m,e! inx h! mov m,d
	;initialize jmp at 0000H to fake BIOS jump table
	mvi a,0C3H! sta 0000H
	lxi h,drctjmptbl! shld 0001H
	;execute the XIOS SYSINIT function
	call syinitf
	ei ;in case it was not done in sysinit
	;create disk mutual exclusion queue
	mvi c,makeque! lxi d,diskcqcb! call xdos
	mvi c,writeque! lxi d,MXDisk! call xdos
	;create printer mutual exclusion queue
	mvi c,makeque! lxi d,listcqcb! call xdos
	mvi c,writeque! lxi d,MXList! call xdos
	;get the directory buffer address
	mvi c,0! call seldskf
	lxi d,8! dad d! mov e,m! inx h! mov d,m
	xchg! shld dirbufa
	;initialize disk system
	mvi c,dskinit! jmp bdos
	;ret ;initialization is complete
;
CoNm:
	db 'COPYRIGHT (C) 1980, '
	db ' DIGITAL RESEARCH '
Serial:
	db '654321'

lstack	equ	Serial+4

entsp	equ	lstack	;BDOS entry stack pointer

dtbl	equ	$
	org	((dtbl-base)+0ffh) and 0ff00h
; *** this table is page aligned
;	BIOS jump table for *.COM file support
	jmp bootf
drctjmptbl:
	jmp wbootf
	jmp bconst
	jmp bconin
	jmp bconout
	jmp blist
;
;  Support for direct BIOS console & list device I/O
;
bconst:	call setflg0
	lxi h,console! dad d! mov d,m
	jmp constf

bconin:	call setflg0! mvi c,3! jmp xbdos

bconout:call setflg0! mov e,c! mvi c,4! jmp xbdos

blist:	call setflg0! mov e,c! mvi c,5! jmp xbdos

setflg0:
	lhld rlradr! mov e,m! inx h! mov d,m
	lxi h,pname! dad d
	mov a,m! ori 80h! mov m,a
	ret

;	Disk and List data structures
diskcqcb:
	;disk mutual exclusion circular queue control block
	dw	$-$	;link
	db	'MXDisk  ' ;name
	dw	0	;message length
	dw	1	;number of messages
	dw	$-$	;dq process head
	dw	$-$	;nq process head
	dw	$-$	;msgin
	dw	$-$	;msgout
	dw	$-$	;msgcnt
	dw	$-$	;owner Process descriptor adr
MXDisk:
	;disk user queue control block
	dw	diskcqcb

listcqcb:
	;list mutual exclusion circular queue control block
	dw	$-$	;link
	db	'MXList  ' ;name
	dw	0	;message length
	dw	1	;number of messages
	dw	$-$	;dq process head
	dw	$-$	;nq process head
	dw	$-$	;msgin
	dw	$-$	;msgout
	dw	$-$	;msgcnt
	dw	$-$	;owner Process Descriptor adr
MXList:
	;list user queue control block
	dw	listcqcb

xbdos:	;arrive here from user programs
	mov a,c! ora c ; test function code
	jz reboot ;zero terminates calling process
	jp notXDOS ;jump if a BDOS call

	call XDOS ;func >= 128 is a XDOS call
	mov a,l ;XDOS returns address put low byte into A
	ret

badfunc:
	;invalid function code
	lxi h,0ffffh! mov a,l! ret

brkpt:	;debugger breakpoint entry
	di! shld svdhl! pop h! shld svdrt! push psw
	;set HL = RLR
	lhld rlradr! mov a,m! inx h! mov h,m! mov l,a
	mvi a,memseg! add l! mov l,a
	mvi a,0! adc h! mov h,a ;HL = .pd.memseg
	mov a,m! add a! adi brkvctr
	lhld sysdat! mov l,a ;HL = brkpt hndlr address
	mov a,m! inx h! mov h,m! mov l,a
	shld jmptobrk+1
	pop psw! lhld svdrt! push h! lhld svdhl
jmptobrk:
	jmp	$-$

restore:
	;restore user stack
	xchg! pop h! sphl! xchg ;get user stack pointer
	ret

notXDOS:
	lda usrstkflg! ora a
	jz BDOS ; if flag set then change stack
	; get memory segment index
	lhld rlradr! mov a,m! inx h
	mov h,m! mov l,a
	mvi a,memseg! add l! mov l,a
	mvi a,0! adc h! mov h,a
	mov a,m ; A = memory segment index
	inr a! jz BDOS ;no stack change if system process call
	dcr a! add a! adi usrstkregion
	lhld sysdat! mov l,a
	mov a,d! mov b,e ;save DE in AB
	mov e,m! inx h! mov d,m
	lxi h,0! dad sp
	xchg! sphl! push d ;set & save user SP
	lxi h,restore! push h ; setup return address to restore stack
	mov d,a! mov e,b ;restore DE

BDOS:
	mov a,c! cpi diskf! jc cnsfunc ;jump if not disk i/o
	cpi nfuncs! jnc badfunc ;skip if invalid #
	push d! push b ;save info & func

	;test for 'live console' suppression, indicated by
	; process name with high order bit of first char "on"
	lhld rlradr! mov a,m! inx h! mov h,m! mov l,a
	lxi b,pname! dad b! mov a,m! ani 80h
	cz func11 ;simulate 'live console' with kbd status chk

	;obtain entry by getting disk mutual exclusion message
	mvi c,readque! lxi d,MXDisk! call XDOS

	;setup & jump to banked bdos
	pop b! pop d ;restore info & func
	lxi h,0! dad sp! shld entsp
	lxi sp,lstack

	;perform the required buffer transfers from
	; the user in common memory
	mov a,c! cpi 17 ;search ?
	jnz skipsrch
	call rlr! push d! lxi d,searcha! dad d! pop d
	mov m,e! inx h! mov m,d
	jmp skipsrchnxt
    skipsrch:
	cpi 18 ;search next ?
	jnz skipsrchnxt
	call rlr! lxi d,searcha! dad d
	mov e,m! inx h! mov d,m
    skipsrchnxt:
	mvi b,0! lxi h,dfctbl-12! dad b
	mov a,m! push psw! push d
	rar! jc cpydmain
	rar! jc cpyfcbin
	jmp nocpyin
    cpydmain:
	call rlr! lxi d,disksetdma! dad d
	mov e,m! inx h! mov d,m
	xchg! shld dmabufa! xchg
	lhld dirbufa! mvi b,128
	call move
	pop d! push d
    cpyfcbin:
	lxi h,commonfcb! mvi b,36
	call move
	lxi d,commonfcb

    nocpyin:
	call getmemseg! sta usermemseg
	mvi m,0 ;set memseg to 0 (memseg #0 must be bank zero)
	push d! push b ;save info & func
	mvi c,dispatch! call xdos
	pop b! pop d

bnkbdos equ $+1
	call $-$

	shld aret
	call getmemseg! lda usermemseg! mov m,a
	mvi c,dispatch! call xdos

	pop d! pop psw ;resore dfctbl byte & fcb address
	ral! jc dmacpyout
	ral! jnc nocpyout
    fcbcpyout:
	lxi h,commonfcb! xchg! mvi b,36
	jmp cpyoutcmn
    dmacpyout:
	lhld dirbufa! xchg! lhld dmabufa! mvi b,128
    cpyoutcmn:
	call move

    nocpyout:
	lhld entsp! sphl ;user stack restored
	lhld aret! push h
	;release disk mutual exclusion message
	mvi c,writeque! lxi d,MXDisk! call XDOS
	;function done
	pop h! mov a,l! mov b,h ;BA = HL = aret
	ret

move:
	;move data length of B from source DE to
	;destination HL
	inr b ;in case of length=0
	move0:
		dcr b! rz
		ldax d! mov m,a
		inx d! inx h
		jmp move0

cnsfunc:
	mov a,c! mov c,e! mov b,d
	lxi h,functab! mov e,a! mvi d,0 ;DE=func, HL=.functab
	dad d! dad d! mov e,m! inx h! mov d,m ;DE=functab(func)
	xchg! pchl ;dispatched
;
;	dispatch table for functions
functab:
	dw	bootf,func1,func2,func3
	dw	func4,listf,func6,func7
	dw	cmnret,func9,func10,func11
diskf	equ	($-functab)/2

nfuncs	equ	40	;must match with banked bdos # fns

;
func1:
	;return console character with echo
	call attcns! call conech
	mov l,a! ret
;
func2:
	;write console character with tab expansion
	call attcns! jmp tabout
;
func3:
	;return raw unattached console character
	call testcnsatt
	jmp dirinp
;
func4:
	;write raw unattached console character
	call testcnsatt
	jmp conoutf
;
;func5:
	;write list character
	;write to list device
	;jmp listf
;
func6:
	;direct console i/o - read if 0ffh
	call attcns
	mov a,c! inr a! jz dirinp ;0ffh => 00h, means input mode
	         inr a! jz stsinp ;0feh => 00h, means status chk
		;direct output function
		jmp conoutf
	dirinp:
		;this differs from CP/M 2.0 which does const
		;and returns zero if not ready
		;character is ready, get it
		lxi h,kbchar! call ofsthl
		mov a,m! mvi m,0! ora a
		cz coninf ;to A
		mov l,a! ret
	stsinp:
		;direct console status input
		call constf ;to A
		mov l,a! ret
;
func7:
	;return io byte
	;*** Not implemented in MP/M 1.0 ***
	xra a
	mov l,a! ret
;
;func8:
	;set i/o byte
	;*** Not implemented in MP/M 1.0 ***
	;ret
;
func9:
	;write line until $ encountered
	;BC = string address
	call attcns! jmp print ;out to console
;
func10:
	;read a buffered console line
	call attcns! jmp read
;
func11:
	;check console status
	call testcnsatt
	jz doconbrk
		mvi c,dispatch! call xdos
		xra a! mov l,a! ret
	doconbrk:
		call conbrk
		mov l,a
;
cmnret:
		ret
;
getrlradr:
	lhld rlradr! ret

rlr:
	;set HL = contents of Ready List Root
	lhld rlradr! mov a,m! inx h! mov h,m! mov l,a
	ret
;
getmemseg:
	;set A = memory segment #
	call rlr ;HL = Ready List Root
	mvi a,memseg! add l! mov l,a
	mvi a,0! adc h! mov h,a ;HL = .pd.memseg
	mov a,m! ret
;
testcnsatt:
	;test to determine if console is attached
	;Zero = attached, D = Console #
	call rlr! xchg ;DE = Ready List Root
	lxi h,console! dad d! mov a,m! push psw! push b
		lhld rlradr! lxi b,cnsatt-rlros! dad b
		add a! mov c,a! mvi b,0! dad b
		mov a,m! cmp e! jnz testext
		inx h! mov a,m! cmp d
	testext:
	pop b! pop d
	mvi e,0 ;cns req'd flag false
	ret
;
attcns:
	;attach console if req'd
	call testcnsatt
	mvi e,0ffh! rz ;cns req'd flag true
	push b! push d!
		mvi c,attach! call xdos
	pop d! pop b!
	ret
;
ofsthl:
	;offset HL by console # (in D)
	push psw
		mov a,d! add l! mov l,a
		mvi a,0! adc h! mov h,a
	pop psw
	ret

;
;	console handlers

constx:
	push d! call constf! pop d! ret

coninx:
	;returns char & cond code = NZ if raw input
	push d! call rlr! lxi d,pname! dad d
	pop d! push d! ;restore console #
	mov a,m! ani 80h! push psw
	call coninf
	mov b,a! pop psw! mov a,b
	pop d! ret

conoutx:
	push d! call conoutf! pop d! ret

conin:
	;read console character to A
	lxi h,kbchar! call ofsthl
	mov a,m! mvi m,0! ora a! rnz
	;no previous keyboard character ready
	call getchr ;get character externally and test it
	jmp conin ;only exit is with kbchar <> 0
	;ret
;
conech:
	;read character with echo
	call conin! call echoc! rc ;echo character?
        ;character must be echoed before return
	push psw! mov c,a! call tabout! pop psw
	ret ;with character in A
;
echoc:
	;echo character if graphic
	;cr, lf, tab, or backspace
	cpi cr! rz ;carriage return?
	cpi lf! rz ;line feed?
	cpi tab! rz ;tab?
	cpi ctlh! rz ;backspace?
	cpi ' '! ret ;carry set if not graphic
;
conbrk:	;check for character ready
	call constx! ani 1
	lxi h,kbchar! call ofsthl
	jnz pgetchr ;jump if char to be read in
	mov a,m! ora a! rz ;return if no char in kbchar either
	jmp conb1 ;active kbchar
	pgetchr:
		mvi m,0 ;clear kbchar to prepare for new char
		;character ready, read it
	getchr: ;entry point used by conin
		call coninx ;to A
		jnz conb0; skip char testing if pd.pname.f0 is "on"
		cpi ctls! jnz notcts ;check stop screen function
		;found ctls, read next character
		call coninx ;to A
		cpi ctlc! jz controlc ;ctlc implies re-boot
		;not a reboot, act as if nothing has happened
		xra a! ret ;with zero in accumulator
	notcts:
		;not a control s, control d?
		cpi ctld! jnz notctd
		;found ctld, detach console
		push d! mvi c,detach! call XDOS
		;then attach console back
		; unless cns not req'd
		pop d! mov a,e! ora a
		rz! push d ;return if cns not req'd
		mvi c,attach! call XDOS! pop d
		;print console attach message
		lxi b,atchmsg! call xprint
		call rlr
	dsplynm:
		lxi b,pname! dad b! mvi e,8
		dsploop:
			mov c,m! push h! push d! call conoutx
			pop d! pop h! inx h! dcr e
			jnz dsploop
		inr e ;cns req'd set true
	xcrlf:
		;output crlf without CONBRK calls
		mvi c,cr! call conoutx
		mvi c,lf! call conoutx
		xra a! ret ;with zero in A

	notctd:
		;not a control d, control c?
		cpi ctlc! jnz conb0
	    controlc: ;entry point
		;test to see if this is a user process
		call getmemseg! inr a
		rz ;ignore ^C if system process running
		;print Abort (Y/N) ?
		lxi b,abtmsg! call query
			jz reboot
		xra a! ret ;with zero in A
	conb0:
		;character in accum, save it
		lxi h,kbchar! call ofsthl! mov m,a
	conb1:
		;return with true set in accumulator
		mvi a,1! ret
;
query:
	call xprint! call coninx! push psw
		mov c,a! call conoutx
		mvi c,cr! call conoutx
		mvi c,lf! call conoutx
	pop psw
	ani 5fh! cpi 'Y'
	ret
;
conout:
	;compute character position/write console char from C
	;compcol = true if computing column position
	lxi h,compcol! call ofsthl
	mov a,m! ora a! jnz compout
		;write the character, then compute the column
		;write console character from C
		push b!	call conoutx ;externally, to console
		call conbrk ;check for screen stop function
		pop b! push b! push d ;recall/save character & con #
		;may be copying to the list device
		lxi h,listcp! call ofsthl
		mov a,m! ani 01h! cnz listf ;to printer, if so
		pop d! pop b ;recall the character & con #
	compout:
		mov a,c ;recall the character
		;and compute column position
		lxi h,column! call ofsthl ;A = char, HL = .column
		cpi rubout! rz ;no column change if nulls
		inr m ;column = column + 1
		cpi ' '! rnc ;return if graphic
		;not graphic, reset column position
		dcr m ;column = column - 1
		mov a,m! ora a! rz ;return if at zero
		;not at zero, may be backspace or end line
		mov a,c ;character back to A
		cpi ctlh! jnz notbacksp
			;backspace character
			dcr m ;column = column - 1
			ret
		notbacksp:
			;not a backspace character, eol?
			cpi lf! rnz ;return if not
			;end of line, column = 0
			mvi m,0 ;column = 0
		ret
;
ctlout:
	;send C character with possible preceding up-arrow
	mov a,c! call echoc ;cy if not graphic (or special case)
	jnc tabout ;skip if graphic, tab, cr, lf, or ctlh
		;send preceding up arrow
		push psw! mvi c,ctl! call conout ;up arrow
		pop psw! ori 40h ;becomes graphic letter
		mov c,a ;ready to print
		;(drop through to tabout)
;
tabout:
	;expand tabs to console
	mov a,c! cpi tab! jnz conout ;direct to conout if not
		;tab encountered, move to next tab position
	tab0:
		mvi c,' '! call conout ;another blank
		call ldacolumn! ani 111b ;column mod 8 = 0 ?
		jnz tab0 ;back for another if not
	ret
;
backup:
	;back-up one screen position
	call pctlh! mvi c,' '! call conoutx ;jmp pctlh
;
pctlh:
	;send ctlh to console without affecting column count
	mvi c,ctlh! jmp conoutx
	;ret
;
crlfp:
	;print #, cr, lf for ctlx, ctlu, ctlr functions
	;then move to strtcol (starting column)
	mvi c,'#'! call conout
	call crlf
	;column = 0, move to position strtcol
	crlfp0:
		call ldacolumn! lxi h,strtcol! call ofsthl
		cmp m! rnc ;stop when column reaches strtcol
		mvi c,' '! call conout ;print blank
		jmp crlfp0
;;
;
crlf:
	;carriage return line feed sequence
	mvi c,cr! call conout! mvi c,lf! jmp conout
	;ret
;
xprint:
	;print routine which does not CONBRK
	;BC = string address, string terminated with a '$'
	ldax b! cpi '$'! rz
	push b! mov c,a! call conoutx! pop b
	inx b! jmp xprint
;
print:
	;print message until M(BC) = '$'
	ldax b! cpi '$'! rz ;stop on $
		;more to print
		inx b! push b! mov c,a ;char to C
		call tabout ;another character printed
		pop b! jmp print
;
pread:	;entry to read, restores buffer address
	pop b
read:	;BC = address (max length, current length, buffer)
	push b ;save buffer address for possible ^X or ^U
	call ldacolumn
	lxi h,strtcol! call ofsthl
	mov m,a ;save start for ctl-x, ctl-h
	mov h,b! mov l,c! mov c,m! inx h! push h! mvi b,0
	;B = current buffer length,
	;C = maximum buffer length,
	;HL= next to fill - 1
	readnx:
		;read next character, BC, HL active
		push b! push h ;blen, cmax, HL saved
		readn0:
			call conin ;next char in A
			ani 7fh ;mask parity bit
			pop h! pop b ;reactivate counters
			cpi cr! jz readen ;end of line?
			cpi lf! jz readen ;also end of line
			cpi ctlh! jnz noth ;backspace?
			;do we have any characters to back over?
			mov a,b! ora a! jz readnx
			;characters remain in buffer, backup one
			dcr b ;remove one character
			call ldacolumn
			lxi h,compcol! call ofsthl
			mov m,a ;col > 0
			;compcol > 0 marks repeat as length compute
			jmp linelen ;uses same code as repeat
		noth:
			;not a backspace
			cpi rubout! jnz notrub ;rubout char?
			;rubout encountered, rubout if possible
			mov a,b! ora a! jz readnx ;skip if len=0
			;buffer has characters, resend last char
			mov a,m! dcr b! dcx h ;A = last char
			;blen=blen-1, next to fill - 1 decremented
			jmp rdech1 ;act like this is an echo
;
		notrub:
			;not a rubout character, check end line
			cpi ctle! jnz note ;physical end line?
			;yes, save active counters and force eol
			push b! push h! call crlf
			lxi h,strtcol! call ofsthl
			xra a! mov m,a ;start position = 00
			jmp readn0 ;for another character
		note:
			;not end of line, list toggle?
			cpi ctlp! jnz notp ;skip if not ctlp
			mvi a,01h! jmp ctlpqcmn
		notp:
			cpi ctlq! jnz notq ;skip if not ctlq
			mvi a,80h
		    ctlpqcmn:
			;list toggle - change parity
			push h ;save next to fill - 1
			push b! push d
			lxi h,listcp! call ofsthl ;HL=.listcp flag
			xra m! mov m,a ;listcp=-listcp
			push h ;save address of listcp
			jnz prntron ; jump if printer to be turned on
			prntroff:
				;return list mutex queue message
				mvi m,0! ;zero listcp(console)
				mvi c,condwriteque! lxi d,MXList
				call XDOS
				pop h! jmp ctlpxit
			prntron:
				;test for ^p or ^q already "on"
				cpi 81h! jz prntroff
                                ;cond. read list mutex message
				mvi c,condreadque
				lxi d,MXList! call XDOS
				mov a,l! ora a
				pop h! jz ctlpxit
				;printer busy, could not ^p
				mvi m,0! lxi b,pbsymsg
				;D = console #
				pop d! push d
				call xprint
			ctlpxit:
				pop d! pop b
				pop h! jmp readnx ;for another char
		notq:
			;not a ctlp, line delete?
			cpi ctlx! jnz notx
			pop h ;discard start position
			;loop while column > strtcol
			backx:
				lxi h,strtcol! call ofsthl
				mov a,m! lxi h,column! call ofsthl
				cmp m! jnc pread ;start again
				dcr m ;column = column - 1
				call backup ;one position
				jmp backx
		notx:
			;not a control x, control u?
			cpi ctlu! jnz notu ;skip if not
			;delete line (ctlu)
			call crlfp ;physical eol
			pop h ;discard starting position
			jmp pread ;to start all over
		notu:
			;not line delete, repeat line?
			cpi ctlr! jnz notr
		linelen:
			;repeat line, or compute line len (ctlh)
			;if compcol > 0
			push b! call crlfp ;save line length
			pop b! pop h! push h! push b
			;bcur, cmax active, beginning buff at HL
		rep0:
			mov a,b! ora a! jz rep1 ;count len to 00
			inx h! mov c,m ;next to print
			dcr b! push b! push h ;count length down
			call ctlout ;character echoed
			pop h! pop b ;recall remaining count
			jmp rep0 ;for the next character
		rep1:
			;end of repeat, recall lengths
			;original BC still remains pushed
			push h ;save next to fill
			lxi h,compcol! call ofsthl
			mov a,m! ora a ;>0 if computing length
			jz readn0 ;for another char if so
			;column position computed for ctlh
			lxi h,column! call ofsthl! sub m ;diff > 0
			lxi h,compcol! call ofsthl
			mov m,a ;count down below
			;move back compcol-column spaces
		backsp:
			;move back one more space
			call backup ;one space
			lxi h,compcol! call ofsthl! dcr m
			jnz backsp
			jmp readn0 ;for next character
		notr:
			;not a ctlr, place into buffer
		rdecho:
			inx h! mov m,a ;character filled to mem
			inr b ;blen = blen + 1
		rdech1:
			;look for a random control character
			push b! push h ;active values saved
			mov c,a ;ready to print
			call ctlout ;may be up-arrow C
			pop h! pop b! mov a,b ;len to A
			;are we at end of buffer?
			cmp c! jc readnx ;go for another if not
		readen:
			;end of read operation, store blen
			pop h! mov m,b ;M(current len) = B
			pop h ;discard buffer address
			mvi c,cr! jmp conout ;return carriage
			;ret
;
ldacolumn:
	lxi h,column! call ofsthl! mov a,m! ret

pbsymsg:
	db	cr,lf,'Printer Busy.',cr,lf,'$'

abtmsg:
	db	cr,lf,'Abort (Y/N) ?','$'

atchmsg:
	db	cr,lf,'Attach:$'

;
;	data areas
;
nmbcns	equ	16

compcol:db	0	;true if computing column position
	rept	nmbcns-1
	db	0
	endm
strtcol:db	0	;starting column position after read
	rept	nmbcns-1
	db	0
	endm
column:	db	0	;column position
	rept	nmbcns-1
	db	0
	endm
listcp:	db	0	;listing toggle
	rept	nmbcns-1
	db	0
	endm
kbchar:	db	0	;initial key char = 00
	rept	nmbcns-1
	db	0
	endm

;*********
;
;	intercept bios boot, readf & writef to switch banks

extreboot:
	call getmemseg! lda usermemseg! mov m,a
	mvi c,dispatch! call xdos
	lxi h,0ffffh! shld aret
	lda usermemseg! inr a
	jz nocpyout ;system process - release MXDisk & return
;
reboot:
	;terminate the calling process
	mvi c,terminate! lxi d,0
	jmp xdos


extreadf:
	call getmemseg! lda usermemseg! mov m,a
	mvi c,dispatch! call xdos
	call readf
	jmp extcommon

extwritef:
	call getmemseg! lda usermemseg! mov m,a
	mvi c,dispatch! call xdos
	call writef
    extcommon:
	push psw
	call getmemseg! mvi m,0
	mvi c,dispatch! call xdos
	pop psw! ret

;
;	Local Data Segment
rlradr:	ds	2	;address of Ready List Root
sysdat:	ds	2	;address of system data page
usrstkflg: ds	1	;user stack flag, 0ffh=users stack
svdhl:	ds	2	;saved HL at breakpoint entry
svdrt:	ds	2	;saved return address at breakpoint entry
;
dirbufa: ds	2	;directory buffer address
dmabufa: ds	2	;dma buffer address
usermemseg: ds	1	;saved user mem seg index
aret:	ds	2	;address value to return

commonfcb:
	ds	36	;fcb copy in common memory

;
;	Disk Function Copy Table
;
dmain	equ	00000001b	;dma copy on entry
fcbin	equ	00000010b	;fcb copy on entry
dmaout	equ	10000000b	;dma copy on exit
fcbout	equ	01000000b	;fcb copy on exit

dfctbl:
	db 0			; 12=return version #
	db 0			; 13=reset disk system
	db 0			; 14=select disk
	db fcbin+fcbout		; 15=open file
	db fcbin		; 16=close file
	db fcbin+dmain+dmaout	; 17=search first
	db fcbin+dmain+dmaout	; 18=search next
	db fcbin		; 19=delete file
	db fcbin+fcbout		; 20=read sequential
	db fcbin+fcbout		; 21=write sequential
	db fcbin+fcbout		; 22=make file
	db fcbin		; 23=rename file
	db 0			; 24=return login vector
	db 0			; 25=return current disk
	db 0			; 26=set DMA address
	db 0			; 27=get alloc address
	db 0			; 28=write protect disk
	db 0			; 29=get R/O vector
	db fcbin		; 30=set file attributes
	db 0			; 31=get disk param addr
	db 0			; 32=get/set user code
	db fcbin+fcbout		; 33=read random
	db fcbin+fcbout		; 34=write random
	db fcbin+fcbout		; 35=compute file size
	db fcbin+fcbout		; 36=set random record
	db 0			; 37=drive reset
	db 0			; 38=access drive
	db 0			; 39=free drive


	ds	6	;reserve space for two jump instructions
;
;	bios external jump table
last:
	org	(((last-base)+255) AND 0ff00h) - 6
	jmp xbdos
	jmp pdisp
;
;	bios access table
bios	equ	$		;base of the bios jump table
bootf	equ	bios		;cold boot function
wbootf	equ	bootf+3		;warm boot function
constf	equ	wbootf+3	;console status function
coninf	equ	constf+3	;console input function
conoutf	equ	coninf+3	;console output function
listf	equ	conoutf+3	;list output function
punchf	equ	listf+3		;punch output function
readerf	equ	punchf+3	;reader input function
homef	equ	readerf+3	;disk home function
seldskf	equ	homef+3		;select disk function
settrkf	equ	seldskf+3	;set track function
setsecf	equ	settrkf+3	;set sector function
setdmaf	equ	setsecf+3	;set dma function
readf	equ	setdmaf+3	;read disk function
writef	equ	readf+3		;write disk function
liststf	equ	writef+3	;list status function
sectran	equ	liststf+3	;sector translate
;
;	xios access table
xiosms	equ	sectran+3	;memory select / protect
xiospl	equ	xiosms+3	;device poll
strclk	equ	xiospl+3	;start clock
stpclk	equ	strclk+3	;stop clock
exitr	equ	stpclk+3	;exit critical region
maxcns	equ	exitr+3		;max console #
syinitf	equ	maxcns+3	;MP/M system initialization
;
	end
