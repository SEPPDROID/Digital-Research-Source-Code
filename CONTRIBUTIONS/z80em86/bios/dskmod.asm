;**************************************************************************
;*               CP/M 3 BIOS for Z80em86 Z80 CPU emulator                 *
;*                                                                        *
;*                      Emulator CP/M 3 Disk tables                       *
;*                                                                        *
;*                  Copyright (C) 1992-2009 Stewart Kay                   *
;**************************************************************************
;
; ChangeLog (most recent entries are at top)
; ------------------------------------------
; v1.0.0 - 16 February 2009, S.J.Kay
; - Prepare for public release.
;
; v1.00 - 17 April 1995, S.J.Kay
; - Undocumented changes.
;
; v0.00 - 1992, S.J.Kay
; - Initial creation date.

	public	@dtbl
	public  HDDdsk, ramdsk, dph2, dpb3
;
	extrn	ifdsk, lfdsk, rfdsk, wfdsk
	extrn	ihdsk, lhdsk, rhdsk, whdsk
	extrn	irdsk, lrdsk, rrdsk, wrdsk
;
	maclib	cpm3
	maclib	dphunit
	maclib	asmtype
;
unt0	equ	dsdsk + flp + hden + drv0
unt1	equ	dsdsk + flp + hden + drv1
unt2	equ	ssdsk + hdd
unt3	equ	ssdsk + ram
;
	cseg			;common memory

@dtbl:	dw	dph0		; A: floppy-disk
	dw	dph1		; B: floppy-disk
HDDdsk:	dw	dph2		; C: Hard Disk    (may change table pos)
	dw	0		; D:
	dw	0		; E:
	dw	0		; F:
	dw	0		; G:
	dw	0		; H:
	dw	0		; I:
	dw	0		; J:
	dw	0		; K:
	dw	0		; L:
ramdsk:	dw	dph3		; M: RAM-disk
	dw	0		; N:
	dw	0		; O:
	dw	0		; P:

; Drive A: - 80T DS HD  18 s/t  512 b/s
dpb0:	dpb	512,  18, 160, 2048,  512, 1

; Drive B: - 80T DS HD  18 s/t  512 b/s
dpb1:	dpb	512,  18, 160, 2048,  512, 1

; Drive C: - HDD-DISK (4 Megabytes)
dpb2:	dpb	512,  64, 128, 2048, 1024, 1, 8000h

; Drive M: - RAM-DISK (16k default)
dpb3:	dpb	128, 128,   1, 2048,  128, 0, 8000h

	dseg			;banked memory

; Disk Parameter Header drive A:
	dw	wfdsk		;write floppy disk routine
	dw	rfdsk		;read floppy disk routine
	dw	lfdsk		;logon floppy disk routine
	dw	ifdsk		;initialize floppy disk routine
	db	unt0		;UNIT
	db	0		;TYPE
dph0:	dw	xlt0		;XLT
	ds	9		;-0-    (BDOS scratch use)
	db	0		;Media flag
	dw	dpb0		;DPB
	dw	csv0		;CSV
	dw	alv0		;ALV
if banked
	dw	0fffeh		;DIRBCB
	dw	0fffeh		;DTABCB
else
	dw	dirbcb		;DIRBCB
	dw	dtabcb		;DTABCB
endif
	dw	0fffeh		;HASH   (gencpm option)
	db	0		;HBANK

; Disk Parameter Header drive B:
	dw	wfdsk		;write floppy disk routine
	dw	rfdsk		;read floppy disk routine
	dw	lfdsk		;logon floppy disk routine
	dw	ifdsk		;initialize floppy disk routine
	db	unt1		;UNIT
	db	0		;TYPE
dph1:	dw	xlt1		;XLT
	ds	9		;-0-    (BDOS scratch use)
	db	0		;Media flag
	dw	dpb1		;DPB
	dw	csv1		;CSV
	dw	alv1		;ALV
if banked
	dw	0fffeh		;DIRBCB
	dw	0fffeh		;DTABCB
else
	dw	dirbcb		;DIRBCB
	dw	dtabcb		;DTABCB
endif
	dw	0fffeh		;HASH   (gencpm option)
	db	0		;HBANK

; Disk Parameter Header HDD drive
	dw	whdsk		;write HDD routine
	dw	rhdsk		;read HDD routine
	dw	lhdsk		;logon HDD routine
	dw	ihdsk		;initialize HDD routine
	db	unt2		;UNIT
	db	0		;TYPE
dph2	dw	xlt2		;XLT
	ds	9		;-0-    (BDOS scratch use)
	db	0		;Media flag
	dw	dpb2		;DPB
	dw	0		;CSV
	dw	alv2		;ALV
if banked
	dw	0fffeh		;DIRBCB
	dw	0fffeh		;DTABCB
else
	dw	dirbcb		;DIRBCB
	dw	dtabcb		;DTABCB
endif
	dw	0fffeh		;HASH   (gencpm option)
	db	0		;HBANK  (n/a)

; Disk Parameter Header drive M:
	dw	wrdsk		;write RAM-DISK routine
	dw	rrdsk		;read RAM-DISK routine
	dw	lrdsk		;logon RAM-DISK routine
	dw	irdsk		;initialize RAM-DISK routine
	db	unt3		;UNIT
	db	0		;TYPE
dph3:	dw	0		;XLT    (no translate table)
	ds	9		;-0-    (BDOS scratch use)
	db	0		;Media flag
	dw	dpb3		;DPB    (Ram-disk)
	dw	0		;CSV    (no check sum vector)
	dw	alv3		;ALV
if banked
	dw	0fffeh		;DIRBCB (gencpm allocates)
else
	dw	dirbcb		;DIRBCB
endif
	dw	0ffffh		;DTABCB (no data buffers)
	dw	0fffeh		;HASH   (gencpm option)
	db	0		;HBANK

; skew tables
xlt0:	db	 1,  4,  7, 10, 13, 16
	db	 2,  5,  8, 11, 14, 17
	db	 3,  6,  9, 12, 15, 18
	ds	32
xlt1:	db	 1,  4,  7, 10, 13, 16
	db	 2,  5,  8, 11, 14, 17
	db	 3,  6,  9, 12, 15, 18
	ds	32
xlt2:	db	 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16
	db	17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
	db	33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48
	db	49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64

; directory checksum tables
csv0:	ds	128		;512 directory entries maximum
csv1:	ds	128		; "      "        "       "

if banked
; double bit allocation tables   BLOCKS  1K BLS   2K BLS  4K BLS
alv0:	ds	200		;   800    800k    1600k   3200k
alv1:	ds	200		;   800    800k    1600k   3200k
alv2:	ds	512		;  2048   2048k    4096k   8192k
alv3:	ds	64		;   256    n/a      512k   n/a

else
; single bit allocation tables   BLOCKS  1K BLS   2K BLS  4K BLS
alv0:	ds	100		;   800    800k    1600k   3200k
alv1:	ds	100		;   800    800k    1600k   3200k
alv2:	ds	256		;  2048   2048k    4096k   8192k
alv3:	ds	32		;   256    n/a      512k   n/a

; directory buffer control block
dirbcb:	db	0ffh,0,0,0,0,0,0,0,0,0
	dw	dirbuf

; data buffer control block
dtabcb:	db	0ffh,0,0,0,0,0,0,0,0,0
	dw	dtabuf

; directory and data buffers
dirbuf:	ds	1024		;1k directory buffer
dtabuf:	ds	1024		;1k data buffer
endif
	end
