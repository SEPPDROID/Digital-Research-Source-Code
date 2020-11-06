*********************************
*				*
*  Relocating overlay loader    *
*        modified from		*
*  Function 59 -- Program Load	*
*				*
*        March 17, 1983		*
*				*
* Does not load program root.	*
* Leaves base page and stack as	*
* set up by actual BDOS 59	*
*				*
*********************************

.globl  _loadr		* this routine is public

secsize = 128		* CP/M sector size

* d0 always contains the return parameter from pgmld
* d1 is the return register from local subroutines
* a0 contains the pointer to the Load Parm Block passed to pgmld

* Return parameters in d0 are:
*	00 - function successful
*	01 - insufficient memory or bad header in file
*	02 - read error on file
*	03 - bad relocation information in file


* Entry point for Program Load routine
_loadr:
	clr.l	d0		* start with return parm cleared
	bsr	gethdr		* get header
	tst	d0
	bne	lddone		* if unsuccessful, return
	bsr	setaddr		* set up load addresses
	tst	d0
	bne	lddone		* if unsuccessful, return
	bsr	rdtxt		* read code and data text segments into mem
	tst	d0
	bne	lddone		* if unsuccessful, return
	move.l	tstart,d7
	cmp.l	cseg,d7
	beq	noreloc
	bsr	reloc		* do relocation if necessary
noreloc:
	tst	d0
	bne	lddone
lddone:
	rts

* Subroutines

readseq:
* CP/M read sequential function
	move.l	d0,-(sp)	* save return parm
	move.l	FCBPtr(a0),d1
	moveq	#20,d0		* read seq function
	trap	#2		* call bdos
	move.l	d0,d1		* return parm in d1
	move.l	(sp)+,d0
	rts


setdma:
* CP/M set dma function
	move.l	d0,-(sp)	* save return parm
	moveq	#26,d0		* set dma function
	trap	#2		* call bdos
	move.l	(sp)+,d0	* restore d0
	rts


gethdr:
* Get header into buffer in data segment
	move.l	BasPage(a0),d1
	move.l	d1,basepg
	bsr	setdma
	bsr	readseq
	tst	d1		* read ok?
	bne	badhdr		* if no, return bad
	moveq	#18,d7
	movea.l	BasPage(a0),a5
	movea.l	#hdr,a6
geth1:	move.w	(a5)+,(a6)+	* move header into hdr
	dbf	d7,geth1
	rts
badhdr:	moveq	#2,d0
	rts


setaddr:
* Set up load addresses for cseg, dseg, bss, basepg, and stack
	move.w	magic,d6
	cmpi.w	#$601a,d6
	bne	badadr		* if magic nmbr <> 601a, skip
	move.l	bpsize,symsize
	move.l	LoAdr(a0),cseg
	move.l	cseg,d7
	add.l	csize,d7
	addq.l	#1,d7
	bclr	#0,d7
	move.l	d7,dseg
	add.l	dsize,d7
	addq.l	#1,d7
	bclr	#0,d7
	move.l	d7,bseg
* cseg, dseg, bseg set up
	rts
badadr:	moveq.l	#1,d0
	rts


movebuf:
* move (d3) bytes from the base page buffer to (a2)
* uses d6
	movea.l	basepg,a1
	move.l	#secsize,d6
	sub.w	bufbyts,d6	* address to move from =
	adda.w	d6,a1		*	(basepg) + secsize - (bufbyts)
	sub.w	d3,bufbyts	* update # bytes buffered
	bra	moveb2
moveb1:	move.b	(a1)+,(a2)+	* do the move
moveb2:	dbf	d3,moveb1
	rts


rdtxt:
* Read code and data text into memory
* during this routine, a2 is always the load address,
*		       d2 is number of bytes left to load
	move.w	#secsize-28,d7
	move.w	d7,bufbyts	* indicate # bytes of text in buffer
	move.w	#2,loop		* do for code, data segments
	move.l	cseg,a2		* start at cseg
	move.l	csize,d2	* for csize bytes
rdtxt3:
	clr.l	d3
	move.w	bufbyts,d3
	cmp.l	d2,d3		* # bytes in buffer >= # bytes to load?
	blt	rdtxt4
	move.l	d2,d3
	bsr	movebuf		* if yes, move # bytes to load
	bra	finrd
rdtxt4:
	sub.l	d3,d2		* if no, update # bytes to load
	bsr	movebuf		* move remainder of buffer
	move.l	#secsize,d3	* d3 = secsize fo following loop
rdtxt5:
	cmp.l	d3,d2		* have at least one more full sector?
	blt	rdtxt6
	move.l	a2,d1
	bsr	setdma		* if yes, set up dma address
	bsr	readseq		* read next sector
	tst.w	d1
	bne	rdbad		* if no good, exit
	sub.l	d3,d2		* decrement # bytes to load
	adda.l	#secsize,a2	* increment dma address
	bra	rdtxt5
rdtxt6:
	tst.l	d2		* any more bytes to read?
	beq	finrd
	move.l	basepg,d1
	bsr	setdma
	bsr	readseq		* if yes, read into base page
	tst.w	d1
	bne	rdbad
	move.w	d3,bufbyts	* indicate that we've buffered a sector
	move.l	d2,d3
	bsr	movebuf		* move remainder of segment
finrd:
	move.l	dseg,a2		* set up to load data segment
	move.l	dsize,d2
	sub.w	#1,loop
	bne	rdtxt3
	move.l	bseg,a2		* clear the bss segment
	move.l	bsize,d2
	beq	rdtxt8
rdtxt7:	clr.b	(a2)+
	subq.l	#1,d2
	bne	rdtxt7
rdtxt8:	rts

rdbad:	moveq.l	#2,d0
	rts


relocword:
* relocate word at (a2) based on reloc bits at (a3)
* lsb of d2 indicates whether previous word was 1st half of long-word
	move.w	(a3)+,d7	* get relocation info
	andi.w	#7,d7		* strip off symbol table bits
	lsl	#1,d7		* multiply by 2
	jmp	2(pc,d7)

	bra	relabs
	bra	reldata
	bra	relcode
	bra	relbss
	bra	relbad
	bra	rellong
	bra	relbad
	bra	relop

relbad:	move.l	(sp)+,d0	* pop return address
	moveq	#3,d0		* return bad relocation to main routine
	rts

relabs:
relop:	bclr	#0,d2		* reset long word flag
	tst.w	(a2)+		* point to next word of segment
	rts

rellong:
	bset	#0,d2		* set long word flag
	tst.w	(a2)+		* point to next word of segment
	rts

reldata:
relbss:
relcode:
	bclr	#0,d2		* long word flag set?
	bne	relc1		* if yes, skip
	move.w	(a2),d6
	add.w	d5,d6
	move.w	d6,(a2)+
	rts

relc1:	tst.w	-(a2)		* point to first word of long
	move.l	(a2),d6
	add.l	d5,d6
	move.l	d6,(a2)+	* note that a2 points past long word
	rts


reloc:
* Modify address references of code and data segments based on relocation bits
* During this routine,
* a2 points to text file to relocate
* a3 points to relocation word in basepg
* lsb of d2 is long word flag (set on reloc type 5, reset on next word)
* d3 is # words in relocation buffer
* d4 is nmbr of words left to relocate
* d5 is relocation offset

	move.l	basepg,d1
	bsr	setdma		* we will always read into base page
* skip past the symbol table
	move.l	symsize,d7
	divu	#secsize,d7	* calculate how many sectors to skip
* note that max # symbols is 8k, which is 896 sectors of 128 bytes
	move.w	d7,d6		* d6 is nmbr sectors to skip
	swap	d7		* d7 is nmbr bytes to skip
	move.w	bufbyts,d3
	sub.w	d7,d3		* subtract bytes to skip from buffer
	bge	skip1
	addi	#secsize,d3	*if amt in buffer < # bytes to skip,
	addq	#1,d6		*  read in 1 extra sector
skip1:	move.l	basepg,a3
	adda	#secsize,a3
	suba.w	d3,a3		* set up a3 to point to buffer
	lsr	#1,d3		* d3 is nmbr words in buffer
	bra	skip3
skip2:
	bsr	readseq		* read next symbol table sector
	tst.w	d1
	bne	rdbad
skip3:	dbf	d6,skip2
* we got past symbol table
* a3, d3 are set up
	move.l	cseg,d5
	move.l	d5,a2		* relocate cseg first
	sub.l	tstart,d5	* d5 contains the relocation offset
	move.l	csize,d4	* nmbr of bytes to relocate
	move.w	#2,loop		* we're going to relocate 2 segments
reloc1:
* relocate one segment
	clr.l	d2		* clear long word flag
	lsr	#1,d4		* make d4 indicate # words
	bra	reloc4
reloc2:
	subq.w	#1,d3
	bpl	reloc3
	bsr	readseq		* if no more words in buffer, refill it
	tst.w	d1
	bne	rdbad
	move.l	basepg,a3
	move.w	#(secsize/2)-1,d3
reloc3:
	bsr	relocword	* relocate one word
	subq.l	#1,d4
reloc4:
	tst.l	d4		* any more to relocate in this segment?
	bne	reloc2		* if yes, do it
	move.l	dseg,a2		* else, set up for dseg
	move.l	dsize,d4
	sub.w	#1,loop
	bne	reloc1
	rts



	.bss

* offsets from start of parameter block
FCBPtr  = 0
LoAdr   = 4
BasPage = 12

hdr:
				* load file header is read into here
magic:		.ds.w	1
csize:		.ds.l	1
dsize:		.ds.l	1
bsize:		.ds.l	1
bpsize:		.ds.l	1	* symb tbl size is swapped with base page size
stksize:	.ds.l	1
tstart:		.ds.l	1
rlbflg:		.ds.w	1
dstart:		.ds.l	1
bstart:		.ds.l	1

cseg:		.ds.l	1
dseg:		.ds.l	1
bseg:		.ds.l	1
basepg:		.ds.l	1

symsize:	.ds.l	1
temp:		.ds.l	1
loop:		.ds.w	1
bufbyts:	.ds.w	1

	.end
