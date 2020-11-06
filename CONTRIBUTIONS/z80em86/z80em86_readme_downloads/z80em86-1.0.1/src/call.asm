;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                           Call and Return Group                             *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 10 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Changed 'lea' instructions to 'mov'.
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

GLOBAL	Z_RETN,   Z_RETI
GLOBAL	Z_RETNZ,  Z_RETZ,   Z_RET,   Z_RETNC,  Z_RETC
GLOBAL	Z_RETPO,  Z_RETPE,  Z_RETP,  Z_RETM

GLOBAL	Z_CALLNZ, Z_CALLZ,  Z_CALL,  Z_CALLNC, Z_CALLC
GLOBAL	Z_CALLPO, Z_CALLPE, Z_CALLP, Z_CALLM

GLOBAL	Z_RST0,   Z_RST1,   Z_RST2,  Z_RST3
GLOBAL	Z_RST4,   Z_RST5,   Z_RST6,  Z_RST7

; declared in FUNCTION.ASM
EXTERN  Nocde0

; declared in INSTHAND.ASM
EXTERN  Z80SP

%include "macros.asm"

;***********************************************************
;*      CALL    NNNN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALL:
        mov     di,[Z80SP]
        mov     si,bp
        lahf
        dec     di
        dec     di
        inc     si
        inc     si
        mov     [Z80SP],di
        mov     word [es:Z80+di],si
        mov     bp,word [es:Z80+bp]
        XinstB

;***********************************************************
;*      CALL    NZ,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLNZ:
        jnz     callnz
        lahf
        inc     bp
        inc     bp
        XinstB
callnz: jmp     Z_CALL

;***********************************************************
;*      CALL    Z,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLZ:
        jz      callz
        lahf
        inc     bp
        inc     bp
        XinstB
callz:  jmp     Z_CALL

;***********************************************************
;*      CALL    NC,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLNC:
        jnc     callnc
        lahf
        inc     bp
        inc     bp
        XinstB
callnc: jmp     Z_CALL

;***********************************************************
;*      CALL    C,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLC:
        jc      callc
        lahf
        inc     bp
        inc     bp
        XinstB
callc:  jmp     Z_CALL

;***********************************************************
;*      CALL    PO,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLPO:
        jpo     callpo
        lahf
        inc     bp
        inc     bp
        XinstB
callpo: jmp     Z_CALL

;***********************************************************
;*      CALL    PE,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLPE:
        jpe     callpe
        lahf
        inc     bp
        inc     bp
        XinstB
callpe: jmp     Z_CALL

;***********************************************************
;*      CALL    P,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLP:
        jns     callp
        lahf
        inc     bp
        inc     bp
        XinstB
callp:  jmp     Z_CALL

;***********************************************************
;*      CALL    M,NNNN           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_CALLM:
        js      callm
        lahf
        inc     bp
        inc     bp
        XinstB
callm:  jmp     Z_CALL

;***********************************************************
;*      RET                      Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RET:
        mov     si,[Z80SP]
        mov     bp,word [es:Z80+si]
        lahf
        inc     si
        inc     si
        mov     [Z80SP],si
        XinstB

;***********************************************************
;*      RET     NZ               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETNZ:
        jnz     retnz
        XinstA
retnz:  jmp     Z_RET

;***********************************************************
;*      RET     Z                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETZ:
        jz      retz
        XinstA
retz:   jmp     Z_RET

;***********************************************************
;*      RET     NC               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETNC:
        jnc     retnc
        XinstA
retnc:  jmp     Z_RET

;***********************************************************
;*      RET     C                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETC:
        jc      retc
        XinstA
retc:   jmp     Z_RET

;***********************************************************
;*      RET     PO               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETPO:
        jpo     retpo
        XinstA
retpo:  jmp     Z_RET

;***********************************************************
;*      RET     PE               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETPE:
        jpe     retpe
        XinstA
retpe:  jmp     Z_RET

;***********************************************************
;*      RET     P                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETP:
        jns     retp
        XinstA
retp:   jmp     Z_RET

;***********************************************************
;*      RET     M                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETM:
        js      retm
        XinstA
retm:   jmp     Z_RET

;***********************************************************
;*      RETI                     Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETI:
        mov     si,reti_
        jmp     NEAR Nocde0
reti_:  db      'RETI$'

;***********************************************************
;*      RETN                     Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RETN:
        mov     si,retn_
        jmp     NEAR Nocde0
retn_:  db      'RETN$'

;***********************************************************
;*      RST     0                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST0:
        mov     si,0000h
restor: mov     di,[Z80SP]
        lahf
        dec     di
        dec     di
        mov     [Z80SP],di
        mov     word [es:Z80+di],bp
        mov     bp,si
        XinstB

;***********************************************************
;*      RST     1                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST1:
        mov     si,0008h
        jmp     restor

;***********************************************************
;*      RST     2                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST2:
        mov     si,0010h
        jmp     restor

;***********************************************************
;*      RST     3                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST3:
        mov     si,0018h
        jmp     restor

;***********************************************************
;*      RST     4                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST4:
        mov     si,0020h
        jmp     restor

;***********************************************************
;*      RST     5                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST5:
        mov     si,0028h
        jmp     restor

;***********************************************************
;*      RST     6                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST6:
        mov     si,0030h
        jmp     restor

;***********************************************************
;*      RST     7                Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_RST7:
        mov     si,0038h
        jmp     restor

        end
