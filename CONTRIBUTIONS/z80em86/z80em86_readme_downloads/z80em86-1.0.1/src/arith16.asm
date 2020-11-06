;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                         16-bit Arithmetic Group                             *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 10 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Added 'align=16' to all SEGMENT declarations. (nasm def is align=1)
;
; v1.00 - 29 April 1995 S.J.Kay
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

GLOBAL	Z_ADxbc,  Z_ADxde,  Z_INCx,   Z_ADxx,   Z_DECx
GLOBAL	Z_ADxsp

GLOBAL	Z_ADybc,  Z_ADyde,  Z_INCy,   Z_ADyy,   Z_DECy
GLOBAL	Z_ADysp

GLOBAL	Z_SChlbc, Z_AChlbc, Z_SChlde, Z_AChlde
GLOBAL	Z_SChlhl, Z_AChlhl, Z_SChlsp, Z_AChlsp

GLOBAL	Z_INCbc,  Z_INCde,  Z_INChl,  Z_DECbc,  Z_DECde,  Z_DEChl
GLOBAL	Z_INCsp,  Z_DECsp

GLOBAL	Z_ADhlbc, Z_ADhlde, Z_ADhlhl, Z_ADhlsp

; declared in INSTHAND.ASM
EXTERN  Z80IX, Z80IY, Z80SP, FlagN

%include "macros.asm"

;***********************************************************
;*      ADD     HL,BC            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADhlbc:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        add     bx,cx
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     HL,DE            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADhlde:
        mov     BYTE [FlagN],0
        mov     di,cx                   ;save value in reg CX
        lahf
        and     ah,11111110b
        mov     ch,ah
        add     bx,dx
        lahf
        and     ah,00000001b
        or      ah,ch
        mov     cx,di                   ;restore value to reg CX
        XinstB

;***********************************************************
;*      ADD     HL,HL            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADhlhl:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        add     bx,bx
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     HL,SP            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADhlsp:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        add     bx,[Z80SP]
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     IX,BC            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADxbc:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        add     [Z80IX],cx
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     IX,DE            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADxde:
        mov     BYTE [FlagN],0
        mov     di,cx                   ;save value in reg CX
        lahf
        and     ah,11111110b
        mov     ch,ah
        add     [Z80IX],dx
        lahf
        and     ah,00000001b
        or      ah,ch
        mov     cx,di                   ;restore value to reg CX
        XinstB

;***********************************************************
;*      ADD     IX,IX            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADxx:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        mov     si,[Z80IX]
        add     [Z80IX],si
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     IX,SP            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADxsp:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        mov     si,[Z80SP]
        add     [Z80IX],si
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     IY,BC            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADybc:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        add     [Z80IY],cx
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     IY,DE            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADyde:
        mov     BYTE [FlagN],0
        mov     di,cx                   ;save value in reg CX
        lahf
        and     ah,11111110b
        mov     ch,ah
        add     [Z80IY],dx
        lahf
        and     ah,00000001b
        or      ah,ch
        mov     cx,di                   ;restore value to reg CX
        XinstB

;***********************************************************
;*      ADD     IY,IY            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADyy:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        mov     si,[Z80IY]
        add     [Z80IY],si
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADD     IY,SP            Flags: S Z - H - P/V N C  *
;*                                      . . x x x  .  0 ^  *
;***********************************************************
Z_ADysp:
        mov     BYTE [FlagN],0
        mov     di,dx                   ;save value in reg DX
        lahf
        and     ah,11111110b
        mov     dh,ah
        mov     si,[Z80SP]
        add     [Z80IY],si
        lahf
        and     ah,00000001b
        or      ah,dh
        mov     dx,di                   ;restore value to reg DX
        XinstB

;***********************************************************
;*      ADC     HL,BC            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  0 ^  *
;***********************************************************
Z_AChlbc:
        mov     BYTE [FlagN],0
        adc     bx,cx
        jo      achlbc
        XinstC
achlbc: XinstD

;***********************************************************
;*      ADC     HL,DE            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  0 ^  *
;***********************************************************
Z_AChlde:
        mov     BYTE [FlagN],0
        adc     bx,dx
        jo      achlde
        XinstC
achlde: XinstD

;***********************************************************
;*      ADC     HL,HL            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  0 ^  *
;***********************************************************
Z_AChlhl:
        mov     BYTE [FlagN],0
        adc     bx,bx
        jo      achlhl
        XinstC
achlhl: XinstD

;***********************************************************
;*      ADC     HL,SP            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  0 ^  *
;***********************************************************
Z_AChlsp:
        mov     BYTE [FlagN],0
        adc     bx,[Z80SP]
        jo      achlsp
        XinstC
achlsp: XinstD

;***********************************************************
;*      SBC     HL,BC            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  1 ^  *
;***********************************************************
Z_SChlbc:
        mov     BYTE [FlagN],2
        sbb     bx,cx
        jo      schlbc
        XinstC
schlbc: XinstD

;***********************************************************
;*      SBC     HL,DE            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  1 ^  *
;***********************************************************
Z_SChlde:
        mov     BYTE [FlagN],2
        sbb     bx,dx
        jo      schlde
        XinstC
schlde: XinstD

;***********************************************************
;*      SBC     HL,HL            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  1 ^  *
;***********************************************************
Z_SChlhl:
        mov     BYTE [FlagN],2
        sbb     bx,bx
        jo      schlhl
        XinstC
schlhl: XinstD

;***********************************************************
;*      SBC     HL,SP            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x x x  V  1 ^  *
;***********************************************************
Z_SChlsp:
        mov     BYTE [FlagN],2
        sbb     bx,[Z80SP]
        jo      schlsp
        XinstC
schlsp: XinstD

;***********************************************************
;*      INC     BC               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_INCbc:
        lahf
        inc     cx
        XinstB

;***********************************************************
;*      INC     DE               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_INCde:
        lahf
        inc     dx
        XinstB

;***********************************************************
;*      INC     HL               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_INChl:
        lahf
        inc     bx
        XinstB

;***********************************************************
;*      INC     SP               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_INCsp:
        lahf
        inc     WORD [Z80SP]
        XinstB

;***********************************************************
;*      INC     IX               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_INCx:
        lahf
        inc     WORD [Z80IX]
        XinstB

;***********************************************************
;*      INC     IY               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_INCy:
        lahf
        inc     WORD [Z80IY]
        XinstB

;***********************************************************
;*      DEC     BC               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DECbc:
        lahf
        dec     cx
        XinstB

;***********************************************************
;*      DEC     DE               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DECde:
        lahf
        dec     dx
        XinstB

;***********************************************************
;*      DEC     HL               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DEChl:
        lahf
        dec     bx
        XinstB

;***********************************************************
;*      DEC     SP               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DECsp:
        lahf
        dec     WORD [Z80SP]
        XinstB

;***********************************************************
;*      DEC     IX               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DECx:
        lahf
        dec     WORD [Z80IX]
        XinstB

;***********************************************************
;*      DEC     IY               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DECy:
        lahf
        dec     WORD [Z80IY]
        XinstB

        end
