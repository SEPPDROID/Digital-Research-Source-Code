*****************************************************************
*								*
*	Usual filename:  SIDLOAD.S				*
*	Adapted by:  Timothy M. Benson				*
*	Control:  4 MAY 83  15:21  (TMB)			*
*								*
*	SID Loader -- This program loads DDT68000 at the top of	*
*	the TPA, hands it its command tail, and passes control	*
*	to it.							*
*								*
*****************************************************************
*
	.text
*
start:	move.l	(a7)+,ccprtn		* save stacked info
	move.l	(a7)+,bpaddr		
*
	lea	mystack+$80,a7		* switch to local stack
*
	move.l	bpaddr,a0		* copy base page to local area
	lea	localbp,a1
	move.l	#$100,d0		* count of bytes to move
	bsr	copy			* branch to subroutine
*
	move.w	#32,d0			* get the current user # and save it
	move.l	#$ff,d1			* d1 = 255
	trap	#2			* call the bdos
	move.l	d0,d3			* d3 holds current user #
*
	move.l	bpaddr,a0		* get drive for DDT68000.68K
	move.b	$24(a0),sidfcb		* and stuff into FCB
	move.w	#openf,d0		* open file DDT68000.68K
	move.l	#sidfcb,d1		* get address of fcb
	trap	#2			* call the bdos
	cmpi.b	#$ff,d0			* check for failure
	bne	openok			* if not then go ahead
*
	tst	d3			* check if we are on user 0
	beq	loadfail		* if so then we have failed
*
	move.w	#32,d0			* if not then switch to user 0
	clr.l	d1			* d1 = 0
	trap	#2			* call the bdos
*
	move.w	#openf,d0		* try the open again
	move.l	#sidfcb,d1		* address of fcb
	trap	#2			* call the bdos
	cmpi.b	#$ff,d0			* check for errors
	beq	loadfail		* error, load failed
*
openok:	move.w	#setdma,d0		* set up to read pgm header
	move.l	#dskbuf,d1		* dma address
	trap	#2			* call the bdos
*
	move.w	#readseq,d0		* read the header
	move.l	#sidfcb,d1		* address of fcb
	trap	#2			* call the bdos
	cmpi.w	#0,d0			* check for errors
	bne	loadfail		* error, load failed
*
	lea	dskbuf,a0
	move.l	tsize(a0),d0		* compute size of DDT68000.68K
	add.l	dsize(a0),d0		* by adding component sizes
	add.l	bsize(a0),d0
	add.l	#$1000,d0		* fake stack size since Belton doesnt
	add.l	#bplen+fudge,d0
*
	cmpi.w	#goodmgc,magic(a0)	* must be $601A
	bne	loadfail
*
	tst.w	rlbflg(a0)		* must be 0
	bne	loadfail
*
	lea	localbp,a0
	lea	lpb,a1
	lea	sidfcb,a2
	move.l	a2,lpbfcb(a1)
	move.l	bphitpa(a0),d1		* set up lpb and load pgm
	move.l	d1,lpbhitpa(a1)
	sub.l	d0,d1
	bclr.l	#0,d1
	move.l	d1,lpblotpa(a1)
	clr.w	lpbcf(a1)
	clr.b	sidfcb+fcbcr
	move.w	#pgmldf,d0		* now do load
	move.l	a1,d1
	trap	#2
	tst.w	d0
	bne	loadfail		* load has failed
*
	move.l	d3,d1			* restore the old user 0
	move.w	#32,d0			* get bdos function #
	trap	#2			* call the bdos
*
*	set up for SID execution
*
	move.l	lpbbpa(a1),a2			* addr of new bp
	move.l	bplotpa(a0),bplotpa(a2)		* copy tpa lower bound
	move.l	lpbdusp(a1),a7			* default user stack ptr
	move.l	a2,-(a7)			* stack new bp address
	move.l	ccprtn,-(a7)			* stack ccp rtn address
	move.l	bpcsstrt(a2),-(a7)		* stack entry to DDT68000.68K
	lea	bpfcb2(a0),a0			* copy parsed fcb's & 
	lea	bpfcb2(a2),a1			* command tail to new bp
	move.l	#bplen-bpfcb2,d0
	bsr	copy
	rts					* exit to DDT68000.68K
*
*	BDOS Function Code Equates
*
prstrng	=  9
openf	= 15
closef	= 16
readseq	= 20
setdma	= 26
bioscall= 50
pgmldf	= 59
*
*
*	Local Variables etc.
*
mystack: .ds.l	$20			* local stack
*
fudge	= $10
*
bplen	= $100
localbp: .ds.l	bplen			* local copy of base page
bplotpa	= 0
bphitpa = 4
bpcsstrt= 8
bpcslen = $C
bpdsstrt= $10
bpdslen = $14
bpbsstrt= $18
bpbslen = $1C
bpfcb2	= $38
bpfcb1	= $5C
*
ccprtn:	.ds.l	1			* return addr in CCP
bpaddr:	.ds.l	1			* base page address
*
sidfcb: .dc.b	0			* FCB for DDT68000.68K
	.dc.b	'DDT6800068K'		* File name and type.
	.dc.b	0			* extent
	.dc.b	0,0			* s1, s2
	.dc.b	0			* rc
	.ds.b	16			* d0 ... dn
	.dc.b	0			* cr
	.ds.b	0,0,0			* random record number
*
	.even
*
fcbcr	=	32
*
dskbuf:	.ds.w	64			* used to hold pgm header
magic	= 0				* offsets in pgm header
tsize	= 2
dsize	= 6
bsize	= 10
stksize	= 18
rlbflg	= 26
*
goodmgc	= $601A				* legal value for magic number
*
*
*
lpb:	.ds.w	11			* loader parameter block
lpbfcb	=  0
lpblotpa=  4
lpbhitpa=  8
lpbbpa	= 12
lpbdusp	= 16
lpbcf	= 20
*
*
*	Copy Bytes from (a0)+ to (a1)+ for d0
*
copy:	subi.l	#1,d0
	bmi	copyxit
copylp:	move.b	(a0)+,(a1)+
	dbf	d0,copylp
copyxit: rts
*
*
*	Can't Load DDT68000.68K, Print Error Message
*
loadfail: move.w #prstrng,d0
	move.l	#loaderr,d1
	trap	#2
	move.l	ccprtn,-(a7)
	rts
loaderr: .dc.b	13,10,'Cannot load DDT68000.68K.',13,10,'$'
*
*
	.end

	move.l	ccprtn,-(a7)
	rts
loaderr: .dc.b	13,10,'Cannot load DDT68000.68K.',13,10,'$'
*
*
	.end
