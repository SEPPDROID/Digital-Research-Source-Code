true	equ	-1
false	equ	not true
jmpop	equ	0c3h
getch	equ	1
print	equ	9
open	equ	15
readseq	equ	20
dma	equ	26
drives	equ	2
bios	equ	0ee00h
ldrbios	equ	false
	extrn	@civec, @covec, @aovec, @lovec, @bnkbf
	extrn	@crdma
	extrn	@crdsk,	@fx, @resel, @vinfo, @usrcd, @ermde
	extrn	@date, @hour, @min, @sec, @mxtpa
iobyte	equ	3
tpa	equ	100h
dffcb	equ	5ch
stack80	equ	100h
bootram	equ	0
bdos	equ	5
ccp	equ	tpa
boot80	equ	0
warm80	equ	3
cnst80	equ	6
cnin80	equ	9
cnot80	equ	12
list80	equ	15
punc80	equ	18
redr80	equ	21
home80	equ	24
slds80	equ	27
strk80	equ	30
ssec80	equ	33
sdma80	equ	36
read80	equ	39
writ80	equ	42
lsta80	equ	45
sctr80	equ	48
	cseg
	jmp	boot
warmpt:
	jmp	wboot
	jmp	const
	jmp	conin
	jmp	conout
	jmp	list
	jmp	auxout
	jmp	auxin
	jmp	home
	jmp	seldsk
	jmp	settrk
	jmp	setsec
	jmp	setdma
	jmp	read
	jmp	write
	jmp	listst
	jmp	sectrn
	jmp	conost
	jmp	auxist
	jmp	auxost
	jmp	devtbl
	jmp	devini
	jmp	drvtbl
	jmp	multio
	jmp	flush
	jmp	move
	jmp	xmove
	jmp	selmem
	jmp	setbnk
	jmp	userf
	jmp	wboot
	jmp	wboot
boot:
	if	ldrbios
	ret
	endif

wboot:
	lxi	sp,stack80
	lxi	h,warmpt
	shld	bootram+1
	lhld	@mxtpa
	shld	bdos+1
	mvi	a,jmpop
	sta	bootram
	sta	bdos
	mvi	b,36
	lxi	h,dffcb
	call	clear
	lxi	h,dffcb
	mvi	m,1
	inx	h
	lxi	d,ccpstg
	lxi	b,11
	call	move
	lxi	d,dffcb
	mvi	c,open
	call	bdos
	ora	a
	jnz	operr
	lxi	d,tpa
ldloop:
	push	d
	mvi	c,dma
	call	bdos
	lxi	d,dffcb
	mvi	c,readseq
	call	bdos
	ora	a
	pop	d
	jnz	gocpm
	lxi	h,128
	dad	d
	xchg
	jmp	ldloop
gocpm:
	cpi	1
	jnz	rderr
	jmp	ccp
operr:
	lxi	d,opnmsg
	jmp	errs
rderr:
	lxi	d,errmsg
errs:
	mvi	c,print
	call	bdos
	mvi	c,getch
	call	bdos
	jmp	wboot
conost:
auxist:
auxost:
	mvi	a,true
devini:
multio:
xmove:
selmem:
setbnk:
userf:
	ret
flush:
	xra	a
	ret
devtbl:
	lxi	h,-1
	ret
drvtbl:
	lxi	h,-2
	ret
move:
	ldax	d
	mov	m,a
	inx	h
	inx	d
	dcx	b
	mov	a,b
	ora	c
	jnz	move
	ret
const	equ	bios+cnst80
conin	equ	bios+cnin80
conout	equ	bios+cnot80
list	equ	bios+list80
auxout	equ	bios+punc80
auxin	equ	bios+redr80
home	equ	bios+home80
settrk	equ	bios+strk80
setsec	equ	bios+ssec80
setdma	equ	bios+sdma80
read	equ	bios+read80
write	equ	bios+writ80
listst	equ	bios+lsta80
sectrn	equ	bios+sctr80
seldsk:
	lxi	h,0
	mov	a,c
	cpi	drives
	rnc
	mov	b,e
	push	b
	call	bios+slds80
	pop	b
	mov	a,l
	ora	h
	rz
	shld	dph2
	mov	l,c
	mvi	h,0
	dad	h
	dad	h
	dad	h
	dad	h
	dad	h
	dad	h
	lxi	d,tables
	dad	d
	mov	a,b
	ani	1
	rnz
	shld	dph3
	mvi	b,64
	call	clear
	lhld	dph2
	xchg
	lhld	dph3
	lxi	b,2
	call	move
	lxi	b,12
	dad	b
	xchg
	lxi	b,10
	dad	b
	xchg
	lxi	b,4
	call	move
	xchg
	lxi	b,32+17
	lhld	dph3
	dad	b
	push	h
	xchg
	mvi	a,-1
	stax	d
	mov	m,e
	inx	h
	mov	m,d
	inx	h
	mvi	m,-1
	inx	h
	mvi	m,-1
	lhld	dph2
	lxi	b,8
	dad	b
	xchg
	pop	h
	lxi	b,10
	dad	b
	lxi	b,2
	call	move
	xchg
	mov	e,m
	inx	h
	mov	d,m
	lhld	dph3
	lxi	b,12
	dad	b
	push	h
	lhld	dph3
	lxi	b,32
	dad	b
	pop	b
	mov	a,l
	stax	b
	inx	b
	mov	a,h
	stax	b
	lxi	b,15
	call	move
	if	ldrbios
	lxi	h,tables+64
	mvi	b,15
	call	clear
	lxi	h,tables+64+15
	shld	tables+64+10
	lxi	h,tables+64
	shld	tables+20
	mvi	m,-1
	endif
	lhld	dph3
	ret

clear:
	mvi	m,0
	inx	h
	dcr	b
	jnz	clear
	ret
ccpstg:
	db	'CCP     COM'
opnmsg:
	db	0dh,0ah,'BIOS can''t open CCP.COM $'
errmsg:
	db	0dh,0ah,'BIOS has bad sector in CCP.COM $'
	dseg
dph2	ds	2
dph3	ds	2
tables	ds	64*drives
	end
