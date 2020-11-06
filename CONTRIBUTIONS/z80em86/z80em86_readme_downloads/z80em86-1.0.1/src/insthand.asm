;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                 Instruction Handler for Extended Opcodes                    *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 10 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Changed all uses of 'Z80' map segment to 'es:Z80'
; - Added 'align=16' to all SEGMENT declarations. (nasm def is align=1)
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

GLOBAL	Z_8086, CBxops, CByops, IXops, IYops, EDops, CBops

GLOBAL	FlagX,  FlagN,  FlagNx, IFF
GLOBAL	Z80I, Z80R, Z80SP, Z80IX, Z80IY, Z80AF, Z80BC, Z80DE, Z80HL

; declared in TABLE.ASM
EXTERN  T0, T1, T2, T3
EXTERN  T4, T5, T6, T7

%include "macros.asm"

svewrd  resw	1               ;temporary word storage
FlagX   resb	1               ;saves flag bits 5 and 3
FlagN   resb	1               ;Emulates the Z80 N flag (bit 1)
FlagNx  resb	1               ;
IFF     resb	1               ;Z80 Interrupt Flip Flop
Z80I    resb	1               ;Z80 interrupt register
Z80R    resb	1               ;Z80 refresh register
Z80SP   resw	1               ;Z80 stack pointer storage
Z80IX   resw	1               ;Z80 IX index register storage
Z80IY   resw	1               ;Z80 IY index register storage
Z80AF   resw	1               ;Z80 2nd AF register storage
Z80BC   resw	1               ;Z80 2nd BC register storage
Z80DE   resw	1               ;Z80 2nd DE register storage
Z80HL   resw	1               ;Z80 2nd HL register storage

; High level interface functions accessed with OUT (FncNmb),A
Z_8086: mov     si,ax                   ;save Z80 reg A
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;8086 code function number
        mov     di,ax
        shl     di,1                    ;offset into function table
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A
        push    cx                      ;save Z80 reg BC
        push    dx                      ;save Z80 reg DE
        push    bx                      ;save Z80 reg HL
        push    bp                      ;save Z80 reg PC
        call    WORD [T7+di]            ;call 8086 code function
        pop     bp                      ;restore Z80 reg PC
        pop     bx                      ;restore Z80 reg HL
        pop     dx                      ;restore Z80 reg DE
        pop     cx                      ;restore Z80 reg BC
        or      al,al                   ;set/res Z80 zero flag
        XinstA

; $CB IX opcode of Z80 instruction
CBxops: lahf                            ;get Z80 reg F
        mov     si,ax                   ;save Z80 regs A and F
        mov     al,[es:Z80+bp]          ;get IX offset
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     [svewrd],ax             ;save IX + signed offset
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        inc     bp                      ;next Z80 PC
        mov     di,ax
        shl     di,1
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set Z80 reg F
        mov     si,[svewrd]
        jmp     WORD [T6+di]

; $CB IY opcode of Z80 instruction
CByops: lahf                            ;get Z80 reg F
        mov     si,ax                   ;save Z80 regs A and F
        mov     al,[es:Z80+bp]          ;get IY offset
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     [svewrd],ax             ;save IY + signed offset
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        inc     bp                      ;next Z80 PC
        mov     di,ax
        shl     di,1
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set Z80 reg F
        mov     si,[svewrd]
        jmp     WORD [T5+di]

; IX opcode of Z80 instruction
IXops:  lahf                            ;get Z80 reg F
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set Z80 reg F
        jmp     WORD [T4+di]

; IY opcode of Z80 instruction
IYops:  lahf                            ;get Z80 reg F
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set Z80 reg F
        jmp     WORD [T3+di]

; $ED opcode of Z80 instruction
EDops:  lahf                            ;get Z80 reg F
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set Z80 reg F
        jmp     WORD [T2+di]

; $CB opcode of Z80 instruction
CBops:  lahf                            ;get Z80 reg F
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set Z80 reg F
        jmp     WORD [T1+di]

        end
