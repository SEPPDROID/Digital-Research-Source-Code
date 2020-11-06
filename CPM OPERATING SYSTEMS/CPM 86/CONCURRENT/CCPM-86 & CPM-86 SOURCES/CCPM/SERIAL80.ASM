
bdos		equ	00005h		; BDOS entry address

jump$table	equ	00001h		; address of address of CP/M
					;  jump table
TOM		equ	00006h		; address of address of top
					;  of memory
cr		equ	00Dh		; carriage return
lf		equ	00Ah		; line feed
vt		equ	00Bh		; vertical tab
ht		equ	009h		; horizontal tab
bs		equ	008h		; backspace
bell		equ	007h		; sound bell
blank		equ	0E5h		; if whole sector contains this
					;  for data then sector is
					;  considered empty or blank
; BDOS function codes

inputf		equ	00Ah		; input from console into 
					;  buffer until 'CR' is sensed
printf		equ	009h		; print to console from buffer
					;  until '$' is sensed
char$in		equ	001h		; input single character from
					;  console
char$out	equ	002h		; send single character to
					;  console
reset$dsk	equ	00Dh		; reset disk system
reboot		equ	000h		; system reboot (warm) function


; more program constants

ser$def$len	equ	6		; length of serial field

	org	0100h

	jmp	start

wboot		ds	3		; jump to warm boot routine
conin		ds	3		; read character from console
conout		ds	3		; send character to console
const		ds	3		; check for console char ready
list		ds	3		; send character to list device
punch		ds	3		; send character to punch 
reader		ds	3		; get character from reader
home		ds	3		; move to track 00 on selected
					;  drive
seldsk		ds	3		; select disk drive
					;  reg C=disk # (0=A,1=B,etc.)
settrkf		ds	3		; select track function
					;  reg BC=track #
setsecf		ds	3		; select sector function
	‰			;  reg C = sector #
setdma		ds	3		; set dma address function
					;  reg BC=address
readf		ds	3		» read disk at select track
					;  and sector, into memory
wòitef		ds	3		; write to disk from memory
					;  at dma address to selected
					;  track and sector
listst		ds	3		; return list status
sectran		ds	3		; sector translate routine

start:
	lxi	sp,stack		; init stack pointer

; fill program jump table with proper information
	lhld	jump$table		; get starting address
					;  of system jump table
	lxi	d,wboot			; get address of our jump
					;  table
	mvi	b,3*16			; init byte counter
	call	mov$blk			; fill our jump table

; initialize ser$num to zero
	lxi	h,ser$num
	mvi	b,ser$def$len
initlop07:
	mvi	m,'0'
	inx	h
	dcr	b
	jnz	initlop07

; give sign on 
	call	clear			; clear the monitor
	lxi	d,msg0			; get address of char string
	call	pmsg			; print string

; prompt operator for source and destination drives
	lxi	d,msg12			; send source message to
	call	pmsg			;  console
	call	get$char		; get disk code
	sui	'A'			; correct it
	sta	dskS			; store code of source disk
	lxi	d,msg13			; send destination message
	call	pmsg			;  to console
	call	get$char		; get disk code
	sui	'A'			; correct it
	sta	dskD			; store code of dest disk

; prompt operator for source disk
	lxi	d,msg14			; get message address
	call	pmsg$wait

; the reset function clears out any previous information pertaining
;  to format, sector size, density, etc. and the next time a disk is
;  selected, those values will be reinitialized.
	mvi	c,reset$dsk
	call	bdos

; when a disk is selected CP/M initializes the Disk Parameter Header
;  and the Disk Parameter Block to conform with the format of the
;  selected disk
	call	selectS			; select source disk

; when the "select disk" function is performed the address of the DPH
;  (Disk Parameter Header) is returned in the HL register pair.  the
;  1st word in the DPH is the address of the system's sector
;  translation table.
	mov	a,m			; get low byte of table address
	sta	xlt$offset		; save it
	inx	h			; get high byte of address
	mov	a,m
	sta	xlt$offset+1		; save it

; the DPH also contains the address of the DPB (Disk Parameter Block).
	lxi	d,9			; the DPB address is 10 bytes
	dad	d			;  10 bytes in DPH
	mov	a,m			; get lower half of address
	sta	DPB$addr		; store it
	inx	h			; get upper half of address
	mov	a,m
	sta	DPB$addr+1		; store it

; move DPB (Disk Parameter Block) into program data area
	lxi	d,DPB			; DE points to program DPB
	lhld	DPB$addr		; HL points to system DPB
	mvi	b,15			; B is a counter
	call	mov$blk			; do the move

; get system sector translation table into data segment of program
	lhld	xlt$offset		; get address of trans table
	mov	a,h			; if HL = 0 then system doesn't
	ora	l			;  have a translation table
	jnz	has$table		;  which means that I'll have
	lda	spt			;  to init "xlt$tab" myself
	mov	c,a
	mov	a,0
	lxi	h,xlt$tab
initlop0:
	mov	m,a
	inr	a
	inx	h
	dcr	c
	jnz	initlop0
	mov	a,1			; set skew factor = 1
	jmp	skew$prompt
has$table:
	lhld	xlt$offset		; get address of system's
	xchg				;  translation table
	lxi	h,xlt$tab		; DE -> system table
					; HL -> program table
	lda	spt			; init counter
	mov	c,a
initlop1:
	ldax	d			; get a byte
	mov	m,a			; store it
	inx	d			; move pointers
	inx	h
	dcr	c			; decrement counter
	jnz	initlop1		; loop if C <> 0

; depending on the system, each physical sector on a diskette can
;  be made up of a number of logical sectors (128 bytes long).  the
;  following code analyses the system sector translate table to
;  determine the number of logical sectors in a physical sector.
	lxi	h,xlt$tab
	mvi	c,0			; init counter
xltlop0:
	inr	c			; increment counter
	mov	a,m
	inr	a
	inx	h			; HL -> next byte in table
	cmp	m			; see if the two numbers are in
	jz	xltlop0			;  the right order
	mov	a,c			; save the sectors/block value
	sta	spb
; calculate skew of system sector translation table
	mov	a,m			; get byte at xlt$tab+spb
	lxi	h,xlt$tab
	sub	m
	call	divide			; divide A by C
; tell operator about skew
skew$prompt:
	push	b			; save system skew
	call	clear			; blank monitor screen
	lxi	d,msg21			; send message
	call	pmsg
	pop	psw			; recover skew
	ori	030h			; make it ASCII
	mov	e,a			; send it console
	mvi	c,char$out
	call	bdos
; ask operator if he/she would like to change the sector skew
	lxi	d,msg16
	call	pmsg
	call	get$char
	cpi	'N'
	jz	dont$change
; ask for new skew factor
	lxi	d,msg17
	call	pmsg
	call	get$1num
	sta	skew
; make new sector translation table using skew factor supplied by
;  operator
	lxi	h,xlt$tab		;  HL -> "xlt$tab"
	mov	b,m			; init beginning sector number
	mov	c,b
	lda	skew			; get skew factor to init
	push	psw			;  loop counter
	lda	spt			; get sectors/track + beginning
	add	c			;  sector # into reg D
	mov	d,a
xltlop1:
	push	b			; save loop counter and 1st
					;  sector #
	lda	spb			; init sectors/block counter
	mov	e,a
	mov	a,b			; get current starting sector
					;  of block into reg A
xltlop2:
	mov	m,a			; put sector number into table
	inx	h			; increment pointer
	inr	a			; increment sector number
	dcr	e			; decrement spb counter
	jnz	xltlop2
	lda	spb			; calcualte new starting sector
	mov	c,a
	lda	skew
	call	mult
	pop	b			; recover starting sector #
	push	b			;  and save it again
	add	b			; A = spb*skew+current sector
					;  number
	cmp	d			; see if new sector # > spt
	jz	xltlop3			; go to xltlop3 if 0
	jnc	xltlop3
	pop	b			; recover starting sector #
					;  and skew then move new
	mov	b,a			;  starting sector # into B
	jmp	xltlop1			; do it again
xltlop3:
	pop	b			; recover current and starting
					;  sector #s
	lda	spb			; get sectors/block
	add	c			; add it to starting sector #
	mov	b,a			; init both sector registers
	mov	c,a
	pop	psw			; recover block counter
	dcr	a
	jnz	xltlop1

dont$change:
; ok, now calculate how many bytes there are on a track.
;  bytes/track = sectors/track * logical sector size.
	lda	spt			; A=sectors/track
	mvi	d,0			; init DE=sectors/track
	mov	e,a
	lxi	h,0			; init HL=0
	mvi	b,128			; init addition counter
multlop0:
	dad	d			; HL=HL+DE
	dcr	b
	jnz	multlop0
	shld	bpt

; search for blank tracks starting with last$trk
; the portion of the source disk which is empty (blank) isn't copied
	lxi	d,msg1			; tell operator about search
	call	pmsg
	lxi	d,msg7			; send "Reading" message
	call	pmsg
	lda	last$trk		; get value of 'last$track'
	mvi	b,0			; B=0
	mov	c,a			; C=A init track counter
scan$loop:
	push	b			; save track counter
	call	settrkf			; set track number
	pop	b			; get track number into reg A
	push	b
	mov	a,c			; make track number ASCII
	call	make$ascii
	lxi	d,ascii$trk		; print it
	call	pmsg
	lxi	h,trk$buff		; init dma address
	shld	dma$base
	push	h			; temp save HL (dma address)
	call	read		; read a track into 'trk$buff'
; check to see if the track that was just read is blank
	pop	d			; recover dma address
	lhld	bpt			; get byte counter
	xchg				; HL -> "trk$buff" 
					; DE = byte counter
blank$scan:
	push	d			; save byte counter
	mov	a,m			; A=(HL)
	cpi	blank			; if A<>blank then track isn't
	jnz	data$found		;  empty
	inx	h			; HL points to next byte
	pop	d			; recover byte counter
	dcx	d			; decrement byte counter
	mov	a,d			; see DE = 0
	ora	e
	jnz	blank$scan		; do again if DE <> 0
	call	erase$trk		; erase old track number
	pop	b			; recover track counter
	dcr	c			; decrement counter
	jnz	scan$loop		; do again if C <> 0

; the diskette is blank if program flow reaches this point
	lxi	d,error$msg0		; inform operator that source
	call	pmsg			;  diskette is blank and
	call	abort			;  abort operation

; 'last$trk' will reflect which track was found to contain data
data$found:
	pop	b			; clear stack
	pop	b			; recover track counter
	mov	a,c			; move track count to reg A
	sta	last$trk		; save track count

; the information on the source disk will more than likely need to be
;  paged into memory.  the following code calculates how many pages
;  or blocks will be need.

; calculate amount of memory available to be used for a buffer area
;  to hold the track data
	lxi	d,trk$buff
	lhld	TOM			; get end of TPA address
	mov	a,l			; now find the difference
	sub	e
	mov	l,a
	mov	a,h
	sbb	d
	mov	h,a			; HL = HL-DE
; calculate number of tracks that will fit in that space
	xra	a			; clear carry flag
	lda	spt			; reg A = sectors/track
	rar				; divide SPT by 2
	mov	c,a			; reg C = SPT/2
	mov	a,h			; reg A = available mem/256
	call	divide			; B = A/C
	dcr	b			; leave room for extra track
; you probably realize that I'am going to need two page sizes
;  (tracks/page) to read in the diskette.
;  example:
;   let's say that after scaning the diskette for blank tracks and
;   taking into account reserved tracks which won't be copied, we
;   find that there are 33 tracks with data.  next we calculate that
;   there is enough spare memory in our system to hold 9 tracks of
;   data from the disk (page size=9).  that means that we can read
;   3 pages of 9 tracks each and then 1 page of 6 tracks (33=3*9+1*6).
;   that's is why there are two track/block variables, "B1$cnt" and
;   "B2$cnt".
	lda	OFF			; get number of reserved tracks
	mov	c,a			; C = A
	lda	last$trk		; get last track with data
	sub	c			; A = # of tracks with data
	cmp	b			; if last$trk<b1$cnt then
	jp	over0			;  b1$cnt=last$trk
	sta	b1$cnt
	mvi	b,1			; will only need 1 page
	sub	a			;  and no block2 count
	jmp	b2$too
over0:
	push	psw			; temp save last$trk
	mov	a,b			; save "B1$cnt"
	sta	B1$cnt
	pop	psw			; recover "last$trk"
	mov	c,b			; prepare for division
	call	divide			; calcualte how many "B1$cnt"
					;  pages there are
b2$too:
	sta	b2$cnt			; block2 track count=remainder
	mov	a,b			; save block count
	sta	block$cnt

; prompt operator for total number of serial numbers on disk
	call	get$cnt
	cpi	0			; if count = 0 then don't ask
	jz	over09			;  for starting serial number

; prompt operator for starting serial number
	call	get$serial
over09:
; ask operator if he/she wants to verify the copy against the source
;  disk and if so, how often
	call	clear
	lxi	d,msg18			; ask question
	call	pmsg
	sub	a			; init verification frequency
	sta	ver$freq		;  value
	call	get$char		; get Y/N answer
	cpi	'N'			; check for No
	jz	over1
; ask operator for frequency of verification
	lxi	d,msg19			; send message
	call	pmsg
	call	get$2num
	sta	ver$freq		; save value
over1:

; clear monitor screen
	call	clear

	sub	a			; init diskette counter
	sta	dsk$cnt

; copy and serialization portion of program
new$disk:
	lxi	h,ser$num		; move current serial number
	lxi	d,serial1		;  into 'Insert' message
	mvi	b,ser$def$len
	call	mov$blk

	lxi	d,msg3			; prompt operator for
	call	pmsg$wait		;  destination diskette

; home both disk drives
	call	selectD			; select destination drive
	call	home			; home it
	call	selectS			; select source drive
	call	home			; home it

	sub	a			; init serial field counter
	sta	ser$cnt

	sub	a			; decides whether to use
	sta	tog0			;  b1$cnt or b2$cnt

	lda	b1$cnt			; init track counter
	sta	trk$cnt

	lda	OFF			; get number of reserved tracks
	sta	trackS			; init source disk track
					;  register
	sta	trackD			; init destination disk
					;  track register

	lxi	d,msg7			; send "Reading" message
	call	pmsg
	call	seekS			; seek
	lxi	h,trk$buff		; init dma address and read in
	shld	dma$base		;  1st track so that there is
	call	read		;  always a extra one in memory
					;  memory
	call	erase$msg		; erase "Reading" message

	lda	block$cnt		; get block counter
	mov	b,a
create$dsk:
	push	b			; save block counter
	call	selectS			; select source drive
	lhld	bpt			; calculate dma address
	lxi	d,trk$buff
	dad	d
	shld	dma$base		; save dma address
	lxi	d,msg7			; send 'Reading' message
	call	pmsg
	lda	trk$cnt			; init track counter
	mov	b,a
lop0:
	push	b			; save track counters
	call	seekS			; set track to read
	call	read		; read track
	call	erase$trk		; erase old track number
	call	track$setup		; move dma base address
	pop	b			; recover counter
	dcr	b
	jnz	lop0
	call	erase$msg		; erase "Reading" message

	call	serialize		; insert serial number

	call	selectD			; select destination drive
	lxi	h,trk$buff		; init dma address
	shld	dma$base
	lxi	d,msg8			; send 'Writing message
	call	pmsg
	lda	trk$cnt			; init track counter
	mov	b,a
lop1:
	push	b			; save track counter
	call	seekD			; set track to write
	call	write		; write track
	call	erase$trk		; erase old track number
	call	track$setup		; move dma base address
	pop	b			; recover track counter
	dcr	b
	jnz	lop1
	call	erase$msg		; erase "Writing message

; move extra track (read and searched but not written) to beginning
;  of buffer
	lhld	dma$base		; get address of unwritten
					;  track
	xchg				; DE=HL, DE-> beginning of
					;  extra track
	lhld	bpt			; get length of track
	push	h			; BC=HL
	pop	b
	lxi	h,trk$buff		; HL-> beginning of buffer
movlop5:
	ldax	d			; get byte
	mov	m,a			; move it
	inx	h			; increment pointers
	inx	d
	dcx	b			; decrement byte counter
	mov	a,b			; am I done ?	
	ora	c
	jnz	movlop5

	pop	b			; recover block counter 
	dcr	b			; decrement counter
	jnz	create$dsk		; do again if B<>0

	lda	tog0			; check toggle
	cpi	1
	jz	done			; if tog0=1 then the copy
					;  process is done
	mvi	a,1			; if tog0<>1 then need to
	sta	tog0			;  finish copy with 
	lda	b2$cnt			;  trk$cnt=b2$cnt
	cpi	0			;  but if b2$cnt=0 then
	jz	done			;  we are done
	sta	trk$cnt
	mvi	b,1			; reint block counter
	jmp	create$dsk		;  and finish copying
done:
	lxi	d,msg8			; send 'Writing message
	call	pmsg
	call	seekD			; write the extra track that
	call	write		;  was read in beginning

	lda	ser$cnt			; check if all the serial
	mov	b,a			;  fields were found
	lda	ser$num$cnt
	cmp	b
	jz	serial$ok
	lxi	d,error$msg3		; inform operator of error
	call	pmsg
	call	abort			; abort operation
serial$ok:

	call	erase$msg		; erase "Writing" message

	lxi	h,dsk$cnt		; get and increment diskette
	inr	m			;  counter
	mov	b,m
	push	b			; save it on stack

	lda	ver$freq		; get verification frequency
	cpi	0			; see if "ver$freq" = 0
	jz	dont$verify		; don't verify if yes
	mov	c,a			; "ver$freq" is divisor
	pop	psw			; "dsk$cnt" is quotient
	call	divide			; do division
	cpi	0			; if A<>0 then don't verify
	jnz	dont$verify
; do the verification
	lxi	d,msg20			; send "Verifying" message
	call	pmsg
	lda	off			; calculate # of tracks to
	sta	trackS			;  verify
	sta	trackD
	mov	b,a
	lda	last$trk
	sub	b
	inr	a
	mov	c,a			; init counter
verlop0:
	push	b			; save track counter
	call	selectD			; select dest disk
	call	seekD			; seek to current track
	lxi	h,trk$buff		; init dma address
	shld	dma$base
	call	read		; read a track from dest drive
	call	erase$trk		; erase old track message
	call	selectS			; select source disk
	call	seekS			; seek to current track
	lxi	h,trk$buff		; init dma address
	push	h			; save pointer to 1st buffer
	xchg
	lhld	bpt
	push	h			; save bytes/track value
	dad	d
	push	h			; save pointer to 2nd buffer
	shld	dma$base
	call	read		; read track from source disk
	call	erase$trk		; erase old track message
	pop	d			; recover pointer to 2nd buffer
	pop	b			; recover bytes/track value
	pop	h			; recover pointer to 1st buffer
verlop1:
	ldax	d			; get byte from dest buffer
	cmp	m			; compare it against byte
					;  from source buffer
	jnz	verlop3			; jump if not equal
	inx	d			; increment pointers
	inx	h
	dcx	b			; decrement byte counter and
	mov	a,b			;  check for BC = 0
	ora	c
	jnz	verlop1
verlop2:
	pop	b			; recover track counter
	dcr	c			; decrement counter
	jnz	verlop0
	jmp	dont$verify
verlop3:
	mvi	a,ser$def$len		; get length of serial
					;  definition
	push	h			; save dest pointer
	lxi	h,ser$def		; HL -> serial definition
verlop4:
	sta	cnter			; save value of counter
	ldax	d			; get byte from source buffer
	cmp	m			; see it equals byte from
	jnz	ver$error		;  serial definition
	inx	d			; increment pointers
	inx	h
	dcx	b			; decrement buffer byte counter
	mov	a,b			; and check for BC = 0
	ora	c
	jz	verlop2
	lda	cnter			; get counter
	dcr	a			; decrement byte counter
	jnz	verlop4			; do again if C <> 0
	pop	h			; recover dest pointer
	push	b			; save buffer byte counter
	lxi	b,ser$def$len		; correct it
	dad	b
	pop	b			; recover byte counter
	jmp	verlop1
; found a verification error so tell operator and try again
ver$error:
	call	clear			; clear monitor screen
	lxi	d,error$msg7
	call	pmsg
	jmp	new$disk
dont$verify:
	lxi	h,ser$num		; insert serial number into
	lxi	d,serial2		;  'Remove' message
	mvi	b,ser$def$len
	call	mov$blk

	call	clear			; clear monitor screen
	lxi	d,msg4			; send 'Remove' message to
	call	pmsg			; send to console

	call	inc$serial		; increment serial number

	jmp	new$disk		; do it again

error:
	lxi	d,errormsg3		; inform operator of error
	call	pmsg$wait		; send message to console

	jmp	wboot			; abort program

track$setup:
	lhld	bpt
	xchg
	lhld	dma$base
	dad	d
	shld	dma$base
	ret

inc$serial:
	mvi	b,ser$def$len		; B = length of serial number
	lxi	h,ser$num+ser$def$len-1 ; HL points to end of serial
					;  number
addlop0:
	push	b			; save byte counter
	mov	a,m			; get byte of serial number
	inr	a			; increment byte
	daa				; decimal adjust result
	push	psw			; need to check AC (auxiliary
	pop	b			;  carry) flag to see if there
	mov	a,c			;  was a carry from the low
	ani	010h			;  nybble to the high nybble
	jnz	fix			; increment the next byte if
					;  there was a carry
	mov	m,b			; else save serial number byte
	pop	b			;  and stop
	ret
fix:
	mov	a,b			; recover serial number byte
	ani	00Fh			; mask off upper nybble
	ori	030h			; make high nybble '3' again
	mov	m,a			; store byte
	dcx	h			; point to next byte
	pop	b			; recover byte counter
	dcr	b			; decrement byte counter
	jnz	addlop0			; do again if B<>0
	ret

; search for all occurances of 'ser$def' and insert serial number
serialize:
	lhld	dma$base		; get address of end of buffer
	dcx	h			; HL->end of trk$buff
	mov	a,m			; save last byte in trk$buff
	push	psw
	push	h			; save address of byte
	mvi	m,0			; put 00h at end of buffer
	inx	h			; HL -> end of "trk$buff" + 1
	xchg				; DE = HL
	lxi	h,trk$buff		; HL points to trk$buff
serlop0:
	lda	ser$def			; get 1st digit of ser$def
					;  into the source register
	cmp	m			; compare source with (HL)
	jz	serlop2			; if equal then check further
	inx	h			; increment HL
	mov	a,h			; am I done ?
	cmp	d
	jnz	serlop0
	mov	a,l
	cmp	e
	jnz	serlop0
	pop	h			; restore last byte of trk$buff
	pop	psw
	mov	m,a
	ret
serlop2:
	push	d			; save pointer to end of buffer
	push	h			; save 'trk$buff' pointer
	inx	h			; HL points to next byte
	lxi	d,ser$def+1		; DE points to 'ser$def'+1
	mvi	b,ser$def$len-1		; B = length 'ser$def'-1
serlop3:
	ldax	d			; get next byte of 'ser$def'
	cmp	m			; compare with where HL points
	jnz	not$ser$field		; if not equal stop compare
	inx	h			; increment HL
	inx	d			; increment DE
	dcr	b			; decrement byte counter
	jnz	serlop3			; do again if B<>0
; found a serial field so increment field counter
	lxi	h,ser$cnt
	inr	m
; no move current serial number into track data
	pop	h			; recover 'trk$buff' pointer
	lxi	d,ser$num		; DE points to current serial
					;  number
	mvi	b,ser$def$len		; init byte counter
serlop4:
	ldax	d			; get serial number byte
	mov	m,a			; move to where HL points
	inx	d			; increment DE
	inx	h			; increment HL
	dcr	b			; decrement byte counter
	jnz	serlop4			; if B<>0 do again
	pop	d			; recover end of buffer pointer
	jmp	serlop0			; continue with serialization
not$ser$field:
	pop	h			; recover trk$buff pointer
	inx	h			; HL points to next byte
	pop	d			; recover end of buffer
					;  pointer
	jmp	serlop0			; continue with serialization

; read a track from disk (track and disk drive already selected)
read:
	lda	spt			; source = sectors/track
	mov	b,a			; B=A  loop counter
	mvi	c,0			; init sector counter to 0
rtrklop0:
	push	b			; save loop counter on stack
	call	setsec			; set sector to read
	call	setdma			; set dma address
	call	readf			; read at selected track
					;  and sector
	cpi	000h			; check for read error
	jnz	rd$error		; if yes then tell operator
	pop	b			; recover loop and sector
					;  counters
	inr	c			; increment sector counter
	dcr	b
	jnz	rtrklop0		; do again if B<>0
	ret
rd$error:
	lxi	d,errormsg4		; prompt operator about error
	call	pmsg
	call	abort			; abort operation

; write a track to disk (track and disk already selected)
write:
	lda	spt			; source = sectors/track
	mov	b,a			; B=A
	mvi	c,0			; init sector counter
wtrklop0:
	push	b			; save loop and sector counters
	call	setsec			; set sector thru BDOS
	call	setdma			; set dma address thru BDOS
	call	writef			; write sector to disk
	cpi	000h			; check for write error
	jnz	wr$error		; if yes then tell operator
	pop	b			; recover loop and sector
					;  counters
	inr	c			; increment sector counter
	dcr	b			; decrement loop counter
	jnz	wtrklop0		; do again if B<>0
	ret
wr$error:
	lxi	d,errormsg5		; prompt operator about error
	call	pmsg
	call	abort			; abort operation

; do sector translation and set sector
setsec:
	mvi	b,0			; B=0
	lxi	h,xlt$tab		; HL -> translation table
	dad	b			; HL points to translated
					;  sector number
	mov	c,m			; C = translated sector number
	push	b
	call	setsecf			; set sector thru BDOS
	pop	b
	mov	a,c
	lxi	h,xlt$tab
	sub	m
	add	a
	mov	c,a
	lxi	h,addr$tab
	dad	b
	mov	c,m
	inx	h
	mov	b,m
	lhld	dma$base
	dad	b
	push	h
	pop	b
	ret

; move block (HL) to block (DE) with length of B
mov$blk:
	mov	a,m
	stax	d
	inx	d
	inx	h
	dcr	b
	jnz	mov$blk
	ret

; select source disk
selectS:
	lda	dskS
	jmp	select

; select destination disk
selectD:
	lda	dskD
select:
	mov	c,a
	call	seldsk
	ret

; select track on source disk
seekS:
	lxi	h,trackS
	jmp	seek

; select track on destination disk
seekD:
	lxi	h,trackD
seek:
	mov	c,m
	inr	m
	mvi	b,0
	push	b
	mov	a,c
	call	make$ascii
	lxi	d,ascii$trk
	call	pmsg
	pop	b			; recover track number
	call	settrkf
	ret

; print string on console
pmsg:
	mvi	c,printf
	call	bdos
	ret

; print string on console and wait for carriage return
pmsg$wait:
	call	pmsg
waitlop:
	mvi	c,char$in
	call	bdos
	cpi	'C'-'@'			; check for control-C
	jz	abort			; abort if yes
	cpi	cr
	jnz	waitlop
	ret

; abort the program
abort:
	lxi	d,abort$msg
	call	pmsg$wait
	mvi	c,reboot		; BDOS system reboot function
	call	bdos

; reads in starting serial number from console
get$serial:
	lxi	d,msg2			; DE = operator prompt address
	call	pmsg			; send it to console
	mvi	c,inputf		; C = input string BDOS code
	lxi	d,con$buff0		; DE = address of console buff
	call	bdos			; BDOS call
	lda	buff0len		; check to see if got right
	cpi	ser$def$len		;  number of digits
	rz				; yes so can return
	mvi	a,ser$def$len		; give operator correct
	ori	030h			;  number of digits to enter
	sta	errormsg2
	lxi	d,errormsg1		; tell operator about error
	call	pmsg
	jmp	get$serial		; do again

; get from the operator how  many serial fields are on the source
;  diskette.  this is a safeguard to help prevent the wrong disk
;  from being used.
get$cnt:
	lxi	d,msg6			; send prompt to console
	call	pmsg
	call	get$2num		; get a number (0-99) from
					;  operator
	sta	ser$num$cnt		; save number
	ret

; input two ASCII numbers into buffer 1 and translate them into a
;  binary number which is returned in the A register.
get$2num:
	mvi	c,inputf		; tell bdos to get input
	lxi	d,con$buff1		; DE=address of input buffer
	call	bdos
	lda	buff1len		; how many characters were read
	cpi	3			; check for more than two
	jm	input$ok0		; if two or less then OK
input$err0:
	lxi	d,errormsg6		; tell operator about problem
	call	pmsg
	jmp	get$2num		; try again
input$ok0:
	cpi	0			; need at least one digit.
	jz	input$err0		;  tell operator if not
	mov	c,a			; going to make HL->end of
	mvi	b,0			;  input string
	lxi	h,count$num
	dad	b
	dcx	h
	mov	a,m			; get units digit
	ani	00Fh			; mask off upper nybble of
					;  units digit
	dcr	c			; decrement digit counter
	jz	units			; if C=0 then only had one
					;  digit so can exit
	push	psw			; save units digit
	dcx	h			; make HL -> next digit
	mov	a,m			; get tens digit
	ani	00Fh			; mask off upper nybble
	mvi	c,10			; init multiplier
	call	mult			; A = A*C
	pop	b			; get units digit back
	add	b			; add tens and units digits
units:
	ret

; get a single ASCII number from operator and check for control-C
get$1num:
	mvi	c,char$in
	call	bdos
	cpi	'C'-'@'
	jz	abort
	ani	00Fh
	ret

; this code erases the "Reading" and "Writing" messages from the
;  monitor screen
erase$msg:
	lxi	d,msg10
	call	pmsg
	ret

; this code erases the old track numbers from the monitor screen
erase$trk:
	lxi	d,msg11
	call	pmsg
	ret

; clear monitor screen
clear:
	lxi	d,clear$scn		; get address of char string
	call	pmsg			; call print routine
	ret

; translate the 8 bit number in A to ASCII into "ascii$trk"
make$ascii:
	lxi	h,'  '			; fill "ascii$trk" with blanks
	shld	ascii$trk
	shld	ascii$trk+2
	lxi	h,ascii$trk+3		; HL -> "ascii$trk"+3
makelop0:
	mvi	c,10			; init divisor
	call	divide			; B=A/C remainder in A
	adi	'0'			; make remainder ASCII
	mov	m,a			; store it in "ascii$trk"
	dcx	h			; move pointer to next location
	mov	a,b			; get qoutient into A
	cpi	0			; is qoutient zero yet ?
	jnz	makelop0		; no, have more digits
	ret

; get ASCII input from operator and translate it to upper case
get$char:
	mvi	c,char$in		; iniô BDOS call
	call	bdos
	cpi	'C'-'@'			; check for control-C
	jz	abort			; abort if yes
	ani	05Fh			; xlate lower to upper case
; echo choice back to operator
echo$choice:
	push	psw			; temp save
	mvi	e,bs			; backspace over last input
	mvi	c,char$out		; send backspace to console
	call	bdos
	pop	psw			; recover old value
	push	psw			; and save it again
	mov	e,a			; send character to console
	mvi	c,char$out
	call	bdos	
	pop	psw			; restore old value
	ret

; divide number in register A by number in register C and return
;  quotent in register B with remainder in register A.
divide:
	mvi	b,0			; set result result reg = 0
divlop0:
	cmp	c			; if C>A then quit
	rc
	sub	c			; A = A-C
	inr	b			; increment result register
	jmp	divlop0
	ret

; multiply register A by register C and return result in register A
;  assume that A*C will be <= 255
mult:
	mov	b,a
	sub	a			; zero out result register
	dcr	c			; check for C = 0
	jm	dont$mult
	inr	c	
multlop1:
	add	b
	dcr	c
	jnz	multlop1
dont$mult:
	ret


; **** Data Area **** ;


cnter		ds	1		; will contain a counter value

sector		ds	1		; sector number held here

tog0		ds	1

trackS		db	76		; source disk track register
trackD		db	76		; destination disk track reg

dskS		ds	1		; code for source disk
dskD		ds	1		; code for destination disk

trk$cnt		ds	1		; how many tracks in block to
					;  read or write

ver$freq	ds	1		; holds	verification frequency
dsk$cnt		ds	1		; number of copies nade so far

skew		ds	1		; storage for skew factor
spb		ds	1		; sectors/ block storage
xlt$tab		ds	128		; sector translation table
addr$tab	dw	0000h,0680h,0480h,0B00h,0280h,0900h,0080h
		dw	0700h,0500h,0B80h,0300h,0980h,0100h,0780h
		dw	0580h,0C00h,0380h,0A00h,0180h,0800h,0600h
		dw	0C80h,0400h,0A80h,0200h,0880h

bpt		ds	2		; number of bytes/track

b1$cnt		ds	1		; number of tracks in 1st block
b2$cnt		ds	1		; number of tracks in 2nd block
block$cnt	ds	1		; how blocks of b1$cnt size

ser$def		db	'654321'	; serial field on master disk

con$buff0	db	10		; console input buffer
buff0len	ds	1
ser$num		ds	10

con$buff1	db	5		; console input buffer
buff1len	ds	1
count$num	ds	5

ser$num$cnt	ds	1		; how many serial fields
					;  are there on the source disk
ser$cnt		ds	1		; counter for number of serial
					;  fields
last$trk	db	76		; be sure this is right for
					;  your system

dma$base	ds	2		; save area for current dma
					;  base address

dma$addr	ds	2		; save area for current dma
					;  address

clear$scn	db	cr,lf,lf,lf,lf,lf,lf,lf,lf,lf,lf,lf,lf,lf,lf
		db	lf,lf,lf,lf,lf,lf,lf,lf,lf,lf
		db	vt,vt,vt,vt,vt,vt,vt,vt,vt,vt,vt,vt,vt,vt,vt
		db	vt,vt,vt,vt,vt,vt,vt,vt,vt
		db	'$'

msg0		db	cr,lf,lf
		db	'-------------------------'
		db	'-------------------------'
		db	'-'
		db	cr,lf
		db	'SERIAL8           V1.0  Serial No. '
		db	' SER-0000-000002'
		db	cr,lf
		db	'Copyright (C) 1982'
		db	cr,lf
		db	'Digital Research, Inc.          '
		db	'All Rights Reserved'
		db	cr,lf
		db	'-------------------------'
		db	'-------------------------'
		db	'-'
		db	cr,lf,lf
		db	'Diskette Generation Utility',cr,lf
		db	'for CP/M-80 based systems'
		db	'$'

msg14		db	cr,lf,lf
		db	'Insert Source Disk '
		db	'and type "Return" to continue'
		db	cr,lf,lf
		db	'$'

msg1		db	cr,lf,lf
		db	'Checking for blank tracks'
		db	cr,lf,lf
		db	'$'

msg2		db	cr,lf,lf
		db	'Enter starting serial number  '
		db	'$'

msg3		db	cr,lf,lf
		db	'Insert '
serial1		ds	ser$def$len
		db	' into destination drive'
		db	cr,lf
		db	' and type "Return" to continue'
		db	cr,lf,lf
		db	'$'

msg4		db	bell,cr,lf,lf
		db	'Remove '
serial2		ds	ser$def$len
		db	' from destination drive'
		db	cr,lf,lf
		db	'$'

msg6		db	bell,cr,lf,lf
		db	'Enter number of serial fields on'
		db	' source disk  '
		db	'$'

msg7		db	'Reading Track '
		db	'$'

msg8		db	'Writing Track '
		db	'$'

msg9		ds	0
ascii$trk	db	'    '
		db	' '
		db	'$'

msg10		db	bs,bs,bs,bs,bs,bs,bs,bs,bs,bs
		db	bs,bs,bs,bs,bs,bs
msg11		db	bs,bs,bs,bs,bs
		db	'$'

msg12		db	cr,lf,lf,lf
		db	'Source Drive is (A,B,etc) ? '
		db	'$'

msg13		db	cr,lf,lf
		db	'Destination Drive is (A,B,etc) ? '
		db	'$'

msg16		db	cr,lf,lf
		db	'Would you like to change the skew factor'
		db	' (Y/N) ? '
		db	'$'

msg17		db	cr,lf,lf
		db	'Enter skew factor (1-9) ? '
		db	'$'

msg18		db	cr,lf,lf
		db	'Do you want to perform copy verification'
		db	' (Y/N) ? '
		db	'$'

msg19		db	cr,lf,lf
		db	'1 = every disk is verified'
		db	cr,lf
		db	'2 = every other disk is verified'
		db	cr,lf
		db	'3 = every 3rd disk is verified'
		db	cr,lf,'"',cr,lf,'"',cr,lf,'"',cr,lf
		db	'99 = every 99th disk is verified'
		db	cr,lf,lf
		db	'How often do you want the verification to'
		db	' occur (1-99) ? '
		db	'$'

msg20		db	'Verifying Track '
		db	'$'

msg21		db	cr,lf,lf
		db	'The current sector skew is '
		db	'$'


errormsg0	db	bell,cr,lf,lf
		db	'Source Diskette is blank'
		db	cr,lf,lf
		db	'$'

errormsg1	db	cr,lf,lf
		db	'Serial number must be '
errormsg2	ds	1
		db	' digits long. Try again.'
		db	cr,lf
		db	'$'

errormsg3	db	bell,cr,lf,lf
		db	'Did not find all of the required serial '
		db	'fields'
		db	cr,lf
		db	'$'


errormsg4	db	bell,cr,lf,lf,lf
		db	'****** A read error has occurred ******'
		db	cr,lf
		db	'$'

errormsg5	db	bell,cr,lf,lf,lf
		db	'****** A write error has occurred ******'
		db	cr,lf
		db	'$'

errormsg6	db	cr,lf,lf
		db	'Need one or two digits. Try again.'
		db	'$'

error$msg7	db	bell,cr,lf,lf
		db	'**** Verification failed ****'
		db	cr,lf
		db	'$'

abort$msg	db	cr,lf,lf,lf
		db	'*** Serialization terminated ***'
		db	cr,lf,lf
		db	'Replace system disk in Drive A:'
		db	cr,lf
		db	'then type "Return" to reboot'
		db	'$'

xlt$offset	ds	2		; address of sector translation
					;table
dpb$addr	ds	2		; address of dpb

; Disk Parameter Block
dpb		ds	0		; DPB will be moved here
spt		ds	2		; sectors/track
		ds	11		; not important
off		ds	1		; number of reserved tracks
		ds	1		; not important

		ds	64		; stack area
stack		ds	0


trk$buff	ds	0		; track buffer


	end
