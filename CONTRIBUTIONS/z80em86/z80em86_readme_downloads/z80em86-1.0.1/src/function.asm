;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*           General function interface support for Z80 system code            *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 25 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Changed 'lea' instructions to  'mov'.
; - Changed all uses of 'Z80' map segment to 'es:Z80'
; - Added 'align=16' to all SEGMENT declarations. (nasm def is align=1)
; - Added code to display 4 opcodes at the PC when a trap occurs. 
;
; v1.00 - 28 April 1995 S.J.Kay
; - Last time code was worked on before releasing under the GPL.
;
; v0.00 - 1992 S.J.Kay
; - Started to code the Z80 emulator.

;===============================================================================
; z80em86 - A Z80 CPU emulator coded in Intel 86 assembly language.
; Copyright (C) 1992-2009 Stewart Kay
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
;===============================================================================

SEGMENT	Z80map	public align=16
Z80:

SEGMENT	.code	public align=16

GLOBAL	criter
GLOBAL	fboot,  SysErr
GLOBAL	BufCnt, BootDr
GLOBAL	fnc___, trp0__, trp1__

GLOBAL	Nocde0, Nocde1, ReqAct, GtZseg
GLOBAL	GetByt, SetByt
GLOBAL	GtBoot
GLOBAL	intfnc
GLOBAL	blkcnt, blkget, blkput, blkfil

GLOBAL	prmsta, prmget, vidsta, vidset
GLOBAL	usrbyt, failed, rstz80, extemu

; declared in MAIN.ASM
EXTERN  Z80res, Z80opc, Z80ext
EXTERN  prmflg, vidoff, prmbuf
EXTERN  BufSeg

; declared in WINDOW.ASM
EXTERN  OpnWnd, ClsWnd, PrnStr
EXTERN  PrnByt, PrnHex, GetAct

; declared in TABLE.ASM
EXTERN  T0,     Tx

; declared in HARDWARE.ASM
EXTERN  Z80seg, Z80dma, cpycom
EXTERN  sveoff, sveseg, sveamt, reqcpy

%include "macros.asm"

;===============================================================================
; DOS critical error handler
;===============================================================================
criter: sti                             ;turn interrupts back on
        push    ds
        push    es
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    bp
        push    cs
        pop     ds                      ;address local data
        mov     dh,8                    ;window Y start
        mov     dl,17                   ;window X start
        mov     ch,16                   ;window Y finish
        mov     cl,62                   ;window X finish
        call    NEAR OpnWnd
        add     al,'A'                  ;drive letter if disk error
        mov     [ErrDrv],al             ;place in error message
        test    ah,080h
        mov     dx,ErrMsB               ;Type: disk error on drive X message
        jz      FndTyp
        mov     es,bp
        test    word [es:si+4],8000h
        mov     dx,ErrMsC               ;Type: charater device message
        jnz     FndTyp
        mov     dx,ErrMsD               ;Type: block device message
FndTyp: mov     si,ErrMsA               ;dos critical error message
        call    NEAR PrnStr
        mov     si,dx
        call    NEAR PrnStr             ;display the error type
        mov     si,ErrMsE
        call    NEAR PrnStr             ;Error: message
        mov     si,ErrMXX
        mov     bx,di
        cmp     bl,010h                 ;is error code in range ?
        jnc     shwerr
        xor     bh,bh
        add     bx,bx
        mov     si,[ErrTbl+bx]
shwerr: call    NEAR PrnStr
        mov     si,ErrMsF               ;Code: message
        call    NEAR PrnStr
        mov     bx,di
        mov     al,bl
        call    NEAR PrnHex             ;print error number in hex
        mov     si,ErrMsG               ;cr, lf, lf
        call    NEAR PrnStr
        mov     bx,ErrKey
        test    ah,010h                 ;retry response allowed ?
        jz      noretr
        mov     byte [bx],'R'
        inc     bx
        mov     si,ErrMsH               ;retry message
        call    NEAR PrnStr
noretr: test    ah,020h                 ;Ignore response allowed ?
        jz      noignr
        mov     byte [bx],'I'
        inc     bx
        mov     si,ErrMsI               ;ignore message
        call    NEAR PrnStr
noignr: test    ah,008h                 ;fail response allowed ?
        jz      nofail
        mov     byte [bx],'F'
        inc     bx
        mov     si,ErrMsJ               ;fail message
        call    NEAR PrnStr
nofail: mov     byte [bx],'$'
        mov     si,ErrMsK               ;Action ? message
        call    NEAR PrnStr
        mov     si,ErrKey
        call    NEAR GetAct
        cmp     al,'R'                  ;retry action requested ?
        mov     ah,1                    ;retry code
        jz      critex
        cmp     al,'I'                  ;ignore action requested ?
        mov     ah,0
        jz      critex
        mov     ah,3                    ;fail code
critex: mov     al,ah
        call    NEAR ClsWnd
        pop     bp
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     es
        pop     ds
        iret

ErrKey  db      '$$$$'

ErrTbl  dw      ErrM00, ErrM01, ErrM02, ErrM03, ErrM04, ErrM05, ErrM06, ErrM07
        dw      ErrM08, ErrM09, ErrM0A, ErrM0B, ErrM0C, ErrM0D, ErrM0E, ErrM0F

ErrMsA  db      '            DOS CRITICAL ERROR', 0dh, 0ah, '$'
ErrMsB  db      ' Type: Disk error on drive '
ErrDrv  db      ' ', 0dh, 0ah, '$'
ErrMsC  db      ' Type: Character device', 0dh, 0ah, '$'
ErrMsD  db      ' Type: Block device', 0dh, 0ah, '$'
ErrMsE  db      'Error: $'
ErrMsF  db      ' Code: $'
ErrMsG  db      0dh, 0ah, 0ah, '$'
ErrMsH  db      '[R]etry  $'
ErrMsI  db      '[I]gnore  $'
ErrMsJ  db      '[F]ail$'
ErrMsK  db      0dh, 0ah, 'Action ?: $'

ErrM00  db      'Write-protect error', 0dh, 0ah, '$'
ErrM01  db      'Invalid drive number', 0dh, 0ah, '$'
ErrM02  db      'Drive not ready', 0dh, 0ah, '$'
ErrM03  db      'Unknown command requested', 0dh, 0ah, '$'
ErrM04  db      'Data error (CRC)', 0dh, 0ah, '$'
ErrM05  db      'Bad request structure length', 0dh, 0ah, '$'
ErrM06  db      'Seek error', 0dh, 0ah, '$'
ErrM07  db      'Unknown disk format', 0dh, 0ah, '$'
ErrM08  db      'Sector not found', 0dh, 0ah, '$'
ErrM09  db      'Printer out of paper', 0dh, 0ah, '$'
ErrM0A  db      'Write fault', 0dh, 0ah, '$'
ErrM0B  db      'Read fault', 0dh, 0ah, '$'
ErrM0C  db      'General, non specific error', 0dh, 0ah, '$'
ErrM0D  db      'UNDOCUMENTED ERROR 0D', 0dh, 0ah, '$'
ErrM0E  db      'UNDOCUMENTED ERROR 0E', 0dh, 0ah, '$'
ErrM0F  db      'Invalid disk change', 0dh, 0ah, '$'
ErrMXX  db      'UNKNOWN ERROR', 0dh, 0ah, '$'

;===============================================================================
; Reset Z80 emulator
;===============================================================================
rstz80: pop     ax                      ;discard near return address
        pop     ax                      ;discard Z80 reg PC
        pop     ax                      ;discard Z80 reg HL
        pop     ax                      ;discard Z80 reg DE
        pop     ax                      ;discard Z80 reg BC
        jmp     NEAR Z80res

;===============================================================================
; Exit from Z80 emulator to DOS
;===============================================================================
extemu: pop     ax                      ;discard near return address
        pop     ax                      ;discard Z80 reg PC
        pop     ax                      ;discard Z80 reg HL
        pop     ax                      ;discard Z80 reg DE
        pop     ax                      ;discard Z80 reg BC
        jmp     NEAR Z80ext

;===============================================================================
; Failure to boot Z80 system disk
;===============================================================================
failed: pop     ax                      ;discard near return address
        pop     ax                      ;discard Z80 reg PC
        pop     ax                      ;discard Z80 reg HL
        pop     ax                      ;discard Z80 reg DE
        pop     ax                      ;discard Z80 reg BC
fboot:  mov     dh,9                    ;window Y start
        mov     dl,23                   ;window X start
        mov     ch,14                   ;window Y finish
        mov     cl,57                   ;window X finish
        call    NEAR OpnWnd
        mov     si,BtfMsg
        call    NEAR PrnStr
        jmp     ActRE

;===============================================================================
; 1st byte of boot sector not $C3 (Z80 jump)
;===============================================================================
SysErr: mov     dh,9                    ;window Y start
        mov     dl,18                   ;window X start
        mov     ch,15                   ;window Y finish
        mov     cl,61                   ;window X finish
        call    NEAR OpnWnd
        mov     si,SysMsg
        call    NEAR PrnStr
        jmp     ActRE

;===============================================================================
; Unsupported OUT (Fn),A code function
;===============================================================================
fnc___: pop     ax                      ;discard near return address
        pop     ax                      ;discard Z80 reg PC
        pop     ax                      ;discard Z80 reg HL
        pop     ax                      ;discard Z80 reg DE
        pop     ax                      ;discard Z80 reg BC
        dec     bp
        dec     bp                      ;address of OUT (Fn),A code
        mov     dh,9                    ;window Y start
        mov     dl,21                   ;window X start
        mov     ch,15                   ;window Y finish
        mov     cl,58                   ;window X finish
        call    NEAR OpnWnd
        mov     si,FncMsg
        jmp     ShwPC

;===============================================================================
; Illegal OUT (Fn),A / 8086 INT function call
;===============================================================================
IntErr: pop     ax                      ;discard near return address
        pop     ax                      ;discard Z80 reg PC
        pop     ax                      ;discard Z80 reg HL
        pop     ax                      ;discard Z80 reg DE
        pop     ax                      ;discard Z80 reg BC
        dec     bp
        dec     bp                      ;address of OUT (Fn),A code
        mov     dh,9                    ;window Y start
        mov     dl,16                   ;window X start
        mov     ch,15                   ;window Y finish
        mov     cl,62                   ;window X finish
        call    NEAR OpnWnd
        mov     si,IntMsg
        jmp     ShwPC

;===============================================================================
; Trap undocumented Z80 opcodes
;===============================================================================
trp0__: dec     bp
        dec     bp
trp1__: dec     bp
        dec     bp
        mov     dh,9                    ;window Y start
        mov     dl,23                   ;window X start
        mov     ch,15                   ;window Y finish
        mov     cl,56                   ;window X finish
        call    NEAR OpnWnd
        mov     si,OpcMsg
        jmp     ShwPC

;===============================================================================
; Trap non emulated Z80 opcodes
;===============================================================================
Nocde0: dec     bp
Nocde1: dec     bp
        mov     dh,9                    ;window Y start
        mov     dl,19                   ;window X start
        mov     ch,16                   ;window Y finish
        mov     cl,59                   ;window X finish
        call    NEAR OpnWnd
        push    si
        mov     si,CdeMsg
        call    NEAR PrnStr
        pop     si
;
ShwPC:  call    NEAR PrnStr
        mov     si,PCaddr
        call    NEAR PrnStr
        mov     ax,bp
        xchg    ah,al
        call    NEAR PrnHex
        xchg    ah,al
        call    NEAR PrnHex

; display the 4 opcodes at the PC location
        mov     si,OpcMs0
        call    NEAR PrnStr
        mov     al,[es:Z80+bp+0]
        call    NEAR PrnHex
        mov     si,SpcMsg
        call    NEAR PrnStr
        mov     al,[es:Z80+bp+1]
        call    NEAR PrnHex
        mov     si,SpcMsg
        call    NEAR PrnStr
        mov     al,[es:Z80+bp+2]
        call    NEAR PrnHex
        mov     si,SpcMsg
        call    NEAR PrnStr
        mov     al,[es:Z80+bp+3]
        call    NEAR PrnHex
        mov     si,OpcMs1
        call    NEAR PrnStr

ActRE:  mov     si,REsels
        call    NEAR PrnStr
        mov     si,KeyRE
        call    NEAR GetAct
        call    NEAR ClsWnd
        cmp     al,'R'
        jz      z80rs0
        jmp     NEAR Z80ext
z80rs0: jmp     NEAR Z80res

BtfMsg: db      'Failure loading Z80 system code$'
SysMsg: db      'Disk in drive is not a Z80 system disk', 0dh, 0ah
        db      'Requires C3 hex (Z80 Jump) as 1st byte$'
OpcMsg: db      'Undocumented Z80 opcode$'
FncMsg: db      'OUT (Fn),A  Function Not Supported$'
IntMsg: db      'Illegal OUT (Fn),A / 8086 INT function call$'
CdeMsg: db      'No code emulation for this Z80 opcode', 0dh, 0ah
        db      'Mnemonic: $'
PCaddr: db      0dh, 0ah
        db      '   at PC: $'
OpcMs0: db      ' ($'
OpcMs1: db      ')$'
SpcMsg: db      ' $'
REsels: db      0dh, 0ah, 0ah
        db      '[R]eset Z80  [E]xit to DOS', 0dh, 0ah
        db      'Select ?: $'
KeyRE:  db      'RE$'

;===============================================================================
; User Interrupt (CTRL+ALT+I)
;===============================================================================
ReqAct: pushf
        push    ax
        push    bx
        push    cx
        push    dx
        push    es
        push    ds
        pop     es
        mov     si,Tx
        mov     di,T0
        mov     cx,512
        cld
rep     movsb
        pop     es
        dec     bp                      ;point to Z80 current PC
        mov     dh,9                    ;window Y start
        mov     dl,19                   ;window X start
        mov     ch,15                   ;window Y finish
        mov     cl,60                   ;window X finish
        call    NEAR OpnWnd
        mov     si,ExeMsg
        call    NEAR PrnStr
        mov     ax,bp
        xchg    ah,al
        call    NEAR PrnHex
        xchg    ah,al
        call    NEAR PrnHex
        mov     si,RECmsg
        call    NEAR PrnStr
        mov     si,KeyREC
        call    NEAR GetAct
        call    NEAR ClsWnd
        cmp     al,'R'
        jz      z80rs1
        cmp     al,'C'
        jz      conact
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        popf
        jmp     NEAR Z80ext
z80rs1: pop     dx
        pop     cx
        pop     bx
        pop     ax
        popf
        jmp     NEAR Z80res
conact: pop     dx
        pop     cx
        pop     bx
        pop     ax
        popf
        jmp     NEAR Z80opc

ExeMsg: db      'Execution Interrupted (Ctrl + Alt + I)', 0dh, 0ah
        db      'Current PC: $'
RECmsg: db      0dh, 0ah, 0ah
        db      '[R]eset Z80  [E]xit to DOS  [C]ontinue', 0dh, 0ah
        db      'Select ?: $'
KeyREC: db      'REC$'

;===============================================================================
; Various other functions
;===============================================================================
prmsta: mov     al,[prmflg]
        retn
prmget: mov     word [es:Z80+bx],ds
        mov     ax,prmbuf
        mov     word [es:Z80+bx+2],ax
        retn
vidsta: mov     al,[vidoff]
        retn
vidset: mov     [vidoff],al
        retn
;
usrdta  times	32	db      0
usrbyt: cmp     bx,020h                 ;is user byte in range
        jnc     usrext
        or      cl,cl
        jz      usrget
        mov     BYTE [usrdta+bx],al     ;set user byte
        retn
usrget: mov     al,[usrdta+bx]          ;get user byte
usrext: retn
;
BootDr  db      0
GtBoot: mov     al,[BootDr]
        retn
;
;===============================================================================
; Block memory routines:
;
; blkcnt:-  Get count of blocks available (1 block=16384 bytes)
;           Return: A(AL)=number of blocks
; blkget:-  Get 128 bytes from block (1 record=128 bytes) from
;           the current DMA bank. (see bnkdma function)
;           Pass:  C(CL)=block number
;                  D(DH)=starting record number
;                  E(DL)=number of records
;                 HL(BX)=Z80 destination address
; blkput:-  Put 128 bytes in block (1 record=128 bytes) for
;           the current DMA bank. (see bnkdma function)
;           Pass:  C(CL)=block number
;                  D(DH)=starting record number
;                  E(DL)=number of records
;                 HL(BX)=Z80 source address
; blkfil:-  Fill 128 bytes in block (1 record=128 bytes) for
;           the current DMA bank. (see bnkdma function)
;           Pass:  A(AL)=fill value
;                  C(CL)=block number
;                  D(DH)=starting record number
;                  E(DL)=number of records
;===============================================================================
; Get count of blocks available (1 block=16384 bytes)
BufCnt  db      0
blkcnt: mov     al,[BufCnt]
        retn

; Get 128 bytes from block (1 record=128 bytes) from the current DMA bank.
blkget: mov     di,bx                   ;destination address in Z80 reg HL
        call    blkprm
        jz      blkg
        retn
blkg:   mov     si,cx
        cld                             ;move upwards
nxtget: mov     BYTE [reqcpy],0ffh      ;copy common required
        mov     cx,128
        mov     [sveamt],cx
        mov     [sveoff],di
        push    ds
        push    es
        mov     es,[Z80dma]             ;bank #0, #1 DMA segment address
        mov     [sveseg],es
        mov     ds,[BufSeg+bx]
rep     movsb
        pop     es
        pop     ds
        call    NEAR cpycom             ;copy common if needed
        dec     dl
        jnz     nxtget
        xor     al,al
        retn

; Put 128 bytes in block (1 record=128 bytes) for the current DMA bank.
blkput: mov     si,bx                   ;source address in Z80 reg HL
        call    blkprm
        jz      blkp
        retn
blkp:   mov     di,cx
        push    ds
        push    es
        mov     es,[BufSeg+bx]
        mov     ds,[Z80dma]             ;bank #0, #1 DMA segment address
        cld                             ;move upwards
nxtput: mov     cx,128
rep     movsb
        dec     dl
        jnz     nxtput
        pop     es
        pop     ds
        xor     al,al
        retn

; Fill 128 bytes in block (1 record=128 bytes) for the current DMA bank.
blkfil: mov     si,ax
        call    blkprm
        jz      blkf
        retn
blkf:   mov     ax,si
        mov     di,cx
        cld                             ;fill upwards
        push    es
        mov     es,[BufSeg+bx]
nxtfil: mov     cx,128
rep     stosb
        dec     dl
        jnz     nxtfil
        pop     es
        xor     al,al
        retn

blkprm: cmp     cl,[BufCnt]             ;block # in Z80 reg C
        jnc     blkerr
        cmp     dh,128
        jnc     blkerr
        xor     bh,bh
        mov     bl,cl
        shl     bl,1
        mov     ch,dh
        xor     cl,cl
        shr     cx,1
        xor     al,al
        retn
blkerr: xor     al,al
        inc     al
        retn

;===============================================================================
; Interrupt services:
;
; GtZseg:-  Return Z80 map segment address
;           Pass: HL(BX)=Z80 address to place result
; intfnc:-  Call a ROM BIOS/DOS Interrupt service
;           Pass: BC(CX)=0AA55h
;                 DE(DX)=055AAh
;                 HL(BX)=base address of register table
;                  A(AL)=Interrupt
; GetByt:-  Get byte from memory
;           Pass: DE(DX)=segment
;                 HL(BX)=offset
;           Return: A(AL)=byte
; SetByt:-  Set byte in memory
;           Pass: DE(DX)=segment
;                 HL(BX)=offset
;                 A(AL)=byte
;===============================================================================
; Call a ROM BIOS/DOS Interrupt service
intfnc: cmp     cx,0aa55h               ;safety gaurd check
        jz      Pass1
        jmp     IntErr
Pass1:  cmp     dx,055aah               ;safety gaurd check
        jz      Pass2
        jmp     IntErr
Pass2:  mov     byte [cs:setint+1],al
        mov     bp,bx                   ;base address of register table
        mov     [SaveBP],bp
        push    word [es:Z80+bp+000h]   ;AX
        push    word [es:Z80+bp+002h]   ;BX
        push    word [es:Z80+bp+004h]   ;CX
        push    word [es:Z80+bp+006h]   ;DX
        push    word [es:Z80+bp+008h]   ;BP
        push    word [es:Z80+bp+00ah]   ;SI
        push    word [es:Z80+bp+00ch]   ;DI
        push    word [es:Z80+bp+00eh]   ;DS
        push    word [es:Z80+bp+010h]   ;ES
        pop     es
        pop     ds
        pop     di
        pop     si
        pop     bp
        pop     dx
        pop     cx
        pop     bx
        pop     ax
setint: int     0                       ;interrupt placed here
        push    es
        push    ds
        push    di
        push    si
        push    bp
        push    dx
        push    cx
        push    bx
        push    ax
        mov     ax,cs
        mov     ds,ax
        mov     ax,[Z80seg]
        mov     es,ax
        mov     bp,[SaveBP]
        pop     word [es:Z80+bp+000h]   ;AX
        pop     word [es:Z80+bp+002h]   ;BX
        pop     word [es:Z80+bp+004h]   ;CX
        pop     word [es:Z80+bp+006h]   ;DX
        pop     word [es:Z80+bp+008h]   ;BP
        pop     word [es:Z80+bp+00ah]   ;SI
        pop     word [es:Z80+bp+00ch]   ;DI
        pop     word [es:Z80+bp+00eh]   ;DS
        pop     word [es:Z80+bp+010h]   ;ES
        pushf
        pop     word [es:Z80+bp+012h]   ;FLAGS
        retn

SaveBP  resw	1

; Return Z80 map segment address
GtZseg: mov     word [es:Z80+bx],es     ;bank #0 or Bank #1 segment
        retn

; Get byte from memory
GetByt: push    ds
        mov     ds,dx
        mov     al,[bx]
        pop     ds
        retn

; Set byte in memory
SetByt: push    ds
        mov     ds,dx
        mov     [bx],al
        pop     ds
        retn

        end
