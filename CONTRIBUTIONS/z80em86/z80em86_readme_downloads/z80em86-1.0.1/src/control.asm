;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*              General Purpose Arithmetic and CPU Control Groups              *
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
; - Added 'align=16' to all SEGMENT declarations. (nasm def is align=1)
; - Fixed DAA instruction to give the correct half carry flag result.
; - The CCF instruction (Z_CCF) code required changes as the Zilog data book
;   was incorrect concerning the 'H' flag.
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

GLOBAL	Z_IM0, Z_IM1, Z_IM2, Z_DI,  Z_EI,  Z_HALT
GLOBAL	Z_DAA, Z_CPL, Z_NEG, Z_SCF, Z_CCF, Z_NOP

; declared in FUNCTION.ASM
EXTERN  Nocde0, Nocde1

; declared in INSTHAND.ASM
EXTERN  IFF, FlagN

%include "macros.asm"

;***********************************************************
;*      DAA                      Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  . ^  *
;***********************************************************
Z_DAA:
        mov     [temp1],al      ;keep track of bit 4
        lahf
        mov     si,ax           ;save ah (orig flags)
        and     ah,00000001b    ;save the carry flag
        cmp     al,99h
        jbe     daa0            ;if below or equal
        mov     ah,00000001b    ;set the carry flag
daa0:   mov     [temp2],ah
        mov     ax,si           ;get back ah (orig flags)
        cmp     BYTE [FlagN],2
        jz      daa1

        sahf
        daa
        lahf
        and     ah,11101110b    ;clear both carry flags
        xor     [temp1],al      ;if bit 4 changed then H=1
        and     BYTE [temp1],00010000b ;keep the half carry flag
        or      ah,BYTE [temp1] ;add in half carry flag
        or      ah,BYTE [temp2] ;add in carry flag
        XinstB

daa1:   sahf
        das
        lahf
        and     ah,11101110b    ;clear both carry flags
        xor     [temp1],al      ;if bit 4 changed then H=1
        and     BYTE [temp1],00010000b ;keep the half carry flag
        or      ah,BYTE [temp1] ;add in half carry flag
        or      ah,BYTE [temp2] ;add in carry flag
        XinstB

temp1   resb    1
temp2   resb    1

;***********************************************************
;*      CPL                      Flags: S Z - H - P/V N C  *
;*                                      . . x 1 x  .  1 .  *
;***********************************************************
Z_CPL:
        lahf
        not     al
        or      ah,00010000b    ;set the half carry flag
        mov     BYTE [FlagN],2
        XinstB

;***********************************************************
;*      NEG                      Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_NEG:
        neg     al
        mov     BYTE [FlagN],2
        jo      neg_
        XinstC
neg_:   XinstD

;***********************************************************
;*      CCF                      Flags: S Z - H - P/V N C  *
;*                                      . . x ^ x  .  0 ^  *
;***********************************************************
Z_CCF:
        cmc
        mov     BYTE [FlagN],0
        lahf
        jc      ccf_
        or      ah,00010000b    ;set the half carry flag
        XinstB
ccf_:   and     ah,11101111b    ;clear the half carry flag
        XinstB

;***********************************************************
;*      SCF                      Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  .  0 1  *
;***********************************************************
Z_SCF:
        stc
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b    ;clear the half carry flag
        XinstB

;***********************************************************
;*      NOP                      Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_NOP:
        XinstA

;***********************************************************
;*      HALT                     Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_HALT:
        mov     si,halt
        jmp     NEAR Nocde1
halt:   db      'HALT$'

;***********************************************************
;*      DI                       Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DI:
        mov     BYTE [IFF],0
        XinstA

;***********************************************************
;*      EI                       Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_EI:
        mov     BYTE [IFF],4
        XinstA

;***********************************************************
;*      IM      0                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_IM0:
        mov     si,im0
        jmp     NEAR Nocde0
im0:    db      'IM      0$'

;***********************************************************
;*      IM      1                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_IM1:
        mov     si,im1
        jmp     NEAR Nocde0
im1:    db      'IM      1$'

;***********************************************************
;*      IM      2                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_IM2:
        mov     si,im2
        jmp     NEAR Nocde0
im2:    db      'IM      2$'

        end
