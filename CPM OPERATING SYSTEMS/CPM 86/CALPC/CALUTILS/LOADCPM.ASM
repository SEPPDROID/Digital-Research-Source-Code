;---------------------------------------------------------------------
; LOADCPM.ASM (FOR CAL-PC MSDOS 2.11 - ASSEMBLE USING A86.COM)
; 28-10-88
; STEPHEN HUNT
;---------------------------------------------------------------------
; 28-12-89
; Modified to attempt to open the CPM.SYS file in the root directory
; if it could not be found in the current directory
;---------------------------------------------------------------------
;
cr	equ	0dh	; carriage return
lf	equ	0ah	; line feed
eof	equ	01ah	; end of file marker
;
dos	equ	21h
;
;---------------------------------------------------------------------
; jump to start of code
;---------------------------------------------------------------------
;
loadcpm:
;
	jmp	start
;
	db	'LOADCPM version 1.1  Copyright (C) Stephen Hunt'
	db	eof
;
;---------------------------------------------------------------------
; tell 'em what were up to
;---------------------------------------------------------------------
;
start:
;
	mov	dx,offset loading
	call	print_string
;
;---------------------------------------------------------------------
; open the CPM.SYS file
;---------------------------------------------------------------------
;
	mov	dx,offset cpm_sys+1	; assume 'CPM.SYS'
	call	open_cpm		; open file
	jnc	read_cpm_sys		; ok - continue
 ;
	mov	dx,offset cpm_sys	; assume '\CPM.SYS'
	call	open_cpm		; open file
	jnc	read_cpm_sys		; ok - continue
;
	mov	dx,offset err_open	; 'cannot find CPM.SYS'
	jmp	error_exit		; exit with error
;---------------------------------------------------------------------
; read the CPM.SYS file (upto 32k bytes, usually only 16k bytes)
;---------------------------------------------------------------------
;
read_cpm_sys:
;
	mov	bx,word ptr cpm_hnd
	mov	dx,offset cpm_buf
	mov	cx,32768
	mov	ah,03fh
	int	dos			; read 32768 bytes of CPM.SYS
;
	mov	word ptr cpm_siz,ax	; store number of bytes read
;
	pushf				; save read error status
	call	closecpm		; close CPM.SYS file
	popf				; restore read error status
;
	jnc	check_header		; continue if read successful
;
	mov	dx,offset err_read	; 'error reading CPM.SYS'
	jmp	error_exit		; exit with error
;---------------------------------------------------------------------
; check that the CPM.SYS header record is valid
;---------------------------------------------------------------------
;
check_header:
;
	cmp	word ptr cpm_siz,0	; q. was anything read ?
	jz	check_header_err	; no - error
;
	mov	si,offset cpm_buf
	cmp	byte ptr 0[si],1	; q. code segment group type present ?
	jnz	check_header_err	; no - error
;
	mov	ax,word ptr 1[si]	; get group length in paragraphs
	mov	dx,010h
	mul	dx			; convert to length in bytes
	mov	word ptr cpm_len,ax	; and store it
;
	mov	bx,word ptr cpm_siz
	cmp	bx,ax			; q. file size greater than code length ?
	jb	check_header_err	; no - error
;
	sub	bx,080h			; subtract length of header record
	sub	bx,ax			; then the code length
	cmp	bx,080h			; q. (file size less header) more than
;					; 128 bytes larger than code length ?
	jae	check_header_err	; yes - error
;
	cmp	word ptr 3[si],0	; q. base segment defined ?
	jz	check_header_err	; no - error
;
	mov	ax,word ptr 1[si]
	cmp	ax,word ptr 5[si]	; q. code length equal minimum length ?
	jnz	check_header_err	; no - error
;
	cmp	word ptr 7[si],0	; q. maximum length defined ?
	jnz	check_header_err	; yes - error
;
	add	si,9			; point to next group descriptor
	mov	cx,128 -9
	cld
;
check_header_loop:
;
	cmp	byte ptr 0[si],0	; q. any more descriptors defined ?
	jnz	check_header_err	; yes - error
	loop	check_header_loop	; loop until finished
;
	jmp	reset_int3		; reset int 3 vector
;
check_header_err:
;
	mov	dx,offset err_head	; 'invalid CPM.SYS header'
	jmp	error_exit		; exit with error
;---------------------------------------------------------------------
; reset interrupt 3 to use the prom interrupt 3 routine (FC00:32BE).
;---------------------------------------------------------------------
;
reset_int3:
;
	push	ds
	mov	dx,0fc00h		; segment FC00h
	mov	ds,dx
	mov	dx,032beh		; offset  32BEh
	mov	al,3
	mov	ah,025h
	int	dos			; reset int 3
	pop	ds

;---------------------------------------------------------------------
; relocate cpm image, set segment registers and jump to it.
;---------------------------------------------------------------------
;
move_cpm:
;
	mov	ax,word ptr cpm_buf +3
	mov	es,ax			; es points to cpm segment
;
	mov	si,offset cpm_buf +080h
	xor	di,di			; set source + destination pointers
;
	mov	cx,word ptr cpm_len	; set length of code to move
;
	cld				; clear the direction flag
	cli				; plus the interrupt flag
;
	rep	movsb			; move it !!!!
;
	mov	ds,ax
	push	ax			; push cpm segment address on stack
	mov	ax,02500h
	push	ax			; push bios cold boot address on stack
;
	retf				; return far to CPM-86 !!!
;---------------------------------------------------------------------
; Subroutines
;---------------------------------------------------------------------
; Open CPM.SYS file
;---------------------------------------------------------------------
;
open_cpm:
;
	xor	al,al
	mov	ah,03dh
	int	dos			; open CPM.SYS file
	mov	word ptr cpm_hnd,ax	; save file handle
;
	ret
;---------------------------------------------------------------------
; Close CPM.SYS file
;---------------------------------------------------------------------
;
close_cpm:
;
	mov	bx,word ptr cpm_hnd
	mov	ah,03eh
	int	dos			; close CPM.SYS file
;
	ret
;---------------------------------------------------------------------
; print string at offset dx
;---------------------------------------------------------------------
;
print_string:
;
	mov	ah,09h
	int	dos			; print string
;
	ret
;---------------------------------------------------------------------
; print string and exit
;---------------------------------------------------------------------
;
error_exit:
;
	call	print_string
	mov	ah,04ch
	xor	al,al
	int	dos			; return to dos
;
;---------------------------------------------------------------------
; data
;---------------------------------------------------------------------
;
loading		db	cr,lf,'Loading CP/M-86 $'
;
err_open	db	cr,lf,lf,'Unable to open CPM.SYS',cr,lf,'$'
err_read	db	cr,lf,lf,'Error reading CPM.SYS',cr,lf,'$'
err_head	db	cr,lf,lf,'Invalid CPM.SYS header',cr,lf,'$'
;
cpm_sys		db	'\CPM.SYS',0	; asciiz file name
cpm_hnd		dw	0		; file handle
cpm_siz		dw	0		; file size
cpm_len		dw	0		; code length
;
cpm_buf		db	32768 dup (0)	; cpm.sys load buffer
;
		db	0		; spare
;
;---------------------------------------------------------------------
;
;---------------------------------------------------------------------
;
		end
