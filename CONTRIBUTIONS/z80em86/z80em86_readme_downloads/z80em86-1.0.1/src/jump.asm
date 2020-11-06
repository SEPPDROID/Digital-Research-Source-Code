;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                                Jump Group                                   *
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

GLOBAL	Z_JPx,  Z_JPy,  Z_JPhl

GLOBAL	Z_JP,   Z_JPNZ, Z_JPZ,  Z_JPNC,  Z_JPC,  Z_JPPO
GLOBAL	Z_JPPE, Z_JPP,  Z_JPM

GLOBAL	Z_JR,   Z_JRNZ, Z_JRZ,  Z_JRNC,  Z_JRC

GLOBAL	Z_DJNZ

; declared in INSTHAND.ASM
EXTERN  Z80IX, Z80IY

%include "macros.asm"

;***********************************************************
;*      JP      NNNN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JP:
        mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      NZ,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPNZ:
        jnz     jumpnz
        lahf
        inc     bp
        inc     bp
        XinstB
jumpnz: mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      Z,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPZ:
        jz     jumpz
        lahf
        inc     bp
        inc     bp
        XinstB
jumpz:  mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      NC,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPNC:
        jnc     jumpnc
        lahf
        inc     bp
        inc     bp
        XinstB
jumpnc: mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      C,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPC:
        jc      jumpc
        lahf
        inc     bp
        inc     bp
        XinstB
jumpc:  mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      PO,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPPO:
        jpo     jumppo
        lahf
        inc     bp
        inc     bp
        XinstB
jumppo: mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      PE,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPPE:
        jpe     jumppe
        lahf
        inc     bp
        inc     bp
        XinstB
jumppe: mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      P,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPP:
        jns     jumpp
        lahf
        inc     bp
        inc     bp
        XinstB
jumpp:  mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      M,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPM:
        js      jumpm
        lahf
        inc     bp
        inc     bp
        XinstB
jumpm:  mov     bp,word [es:Z80+bp]
        XinstA

;***********************************************************
;*      JP      (HL)             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPhl:
        mov     bp,bx
        XinstA

;***********************************************************
;*      JP      (IX)             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPx:
        mov     bp,[Z80IX]
        XinstA

;***********************************************************
;*      JP      (IY)             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JPy:
        mov     bp,[Z80IY]
        XinstA

;***********************************************************
;*      JR      RR               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JR:
        lahf
JRx:    mov     si,ax
        mov     al,[es:Z80+bp]
        inc     bp                      ;points to next instruction
        cbw                             ;label: jr   label = FE hex (-2)
        add     bp,ax
        mov     ax,si
        XinstB

;***********************************************************
;*      JR      C,RR             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JRC:
        jc      jrc
        lahf
        inc     bp
        XinstB
jrc:    jmp     Z_JR

;***********************************************************
;*      JR      NC,RR            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JRNC:
        jnc     jrnc
        lahf
        inc     bp
        XinstB
jrnc:   jmp     Z_JR

;***********************************************************
;*      JR      Z,RR             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JRZ:
        jz      jrz
        lahf
        inc     bp
        XinstB
jrz:    jmp     Z_JR

;***********************************************************
;*      JR      NZ,RR            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_JRNZ:
        jnz     jrnz
        lahf
        inc     bp
        XinstB
jrnz:   jmp     Z_JR

;***********************************************************
;*      DJNZ    RR               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_DJNZ:
        lahf
        dec     ch
        jnz     djnz
        inc     bp
        XinstB
djnz:   jmp     JRx

        end
