;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                     8-bit Arithmetic and Logical Group                      *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 24 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Changed all uses of 'Z80' map segment to 'es:Z80'
; - Added 'align=16' to all SEGMENT declarations. (nasm def is align=1)
; - Instructions Z_ADDn, Z_ADCn, Z_SUBn, Z_SBCn and Z_CPn failed to preserve
;   and restore the 8086's (Z80's V) overflow flag when using an 'inc bp',
;   replaced 'lahf, inc bp, sahf' with 'pushf, inc bp, popf'.
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

GLOBAL	Z_INCw, Z_DECw
GLOBAL	Z_ADDw, Z_ADCw, Z_SUBw, Z_SBCw, Z_ANDw, Z_XORw
GLOBAL	Z_ORw,  Z_CPw

GLOBAL	Z_INCv, Z_DECv
GLOBAL	Z_ADDv, Z_ADCv, Z_SUBv, Z_SBCv, Z_ANDv, Z_XORv
GLOBAL	Z_ORv,  Z_CPv

GLOBAL	Z_INCb, Z_INCc, Z_INCd, Z_INCe, Z_INCh, Z_INCl, Z_INCz, Z_INCa
GLOBAL	Z_DECb, Z_DECc, Z_DECd, Z_DECe, Z_DECh, Z_DECl, Z_DECz, Z_DECa
GLOBAL	Z_ADDb, Z_ADDc, Z_ADDd, Z_ADDe, Z_ADDh, Z_ADDl, Z_ADDz, Z_ADDa
GLOBAL	Z_ADCb, Z_ADCc, Z_ADCd, Z_ADCe, Z_ADCh, Z_ADCl, Z_ADCz, Z_ADCa
GLOBAL	Z_SUBb, Z_SUBc, Z_SUBd, Z_SUBe, Z_SUBh, Z_SUBl, Z_SUBz, Z_SUBa
GLOBAL	Z_SBCb, Z_SBCc, Z_SBCd, Z_SBCe, Z_SBCh, Z_SBCl, Z_SBCz, Z_SBCa
GLOBAL	Z_ANDb, Z_ANDc, Z_ANDd, Z_ANDe, Z_ANDh, Z_ANDl, Z_ANDz, Z_ANDa
GLOBAL	Z_XORb, Z_XORc, Z_XORd, Z_XORe, Z_XORh, Z_XORl, Z_XORz, Z_XORa
GLOBAL	Z_ORb,  Z_ORc,  Z_ORd,  Z_ORe,  Z_ORh,  Z_ORl,  Z_ORz,  Z_ORa
GLOBAL	Z_CPb,  Z_CPc,  Z_CPd,  Z_CPe,  Z_CPh,  Z_CPl,  Z_CPz,  Z_CPa
GLOBAL	Z_ADDn, Z_ADCn, Z_SUBn, Z_SBCn, Z_ANDn, Z_XORn, Z_ORn,  Z_CPn

; declared in INSTHAND.ASM
EXTERN       Z80IX, Z80IY, FlagN

%include "macros.asm"

;***********************************************************
;*      ADD     A,B              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDb:
        mov     BYTE [FlagN],0
        add     al,ch
        jo      addb
        XinstC
addb:   XinstD

;***********************************************************
;*      ADD     A,C              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDc:
        mov     BYTE [FlagN],0
        add     al,cl
        jo      addc
        XinstC
addc:   XinstD

;***********************************************************
;*      ADD     A,D              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDd:
        mov     BYTE [FlagN],0
        add     al,dh
        jo      addd
        XinstC
addd:   XinstD

;***********************************************************
;*      ADD     A,E              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDe:
        mov     BYTE [FlagN],0
        add     al,dl
        jo      adde
        XinstC
adde:   XinstD

;***********************************************************
;*      ADD     A,H              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDh:
        mov     BYTE [FlagN],0
        add     al,bh
        jo      addh
        XinstC
addh:   XinstD

;***********************************************************
;*      ADD     A,L              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDl:
        mov     BYTE [FlagN],0
        add     al,bl
        jo      addl
        XinstC
addl:   XinstD

;***********************************************************
;*      ADD     A,A              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDa:
        mov     BYTE [FlagN],0
        add     al,al
        jo      adda
        XinstC
adda:   XinstD

;***********************************************************
;*      ADD     A,(HL)           Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDz:
        mov     BYTE [FlagN],0
        add     al,[es:Z80+bx]
        jo      addz
        XinstC
addz:   XinstD

;***********************************************************
;*      ADD     A,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],0
        add     al,[es:Z80+si]
        jo      addw
        XinstC
addw:   XinstD

;***********************************************************
;*      ADD     A,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],0
        add     al,[es:Z80+si]
        jo      addv
        XinstC
addv:   XinstD

;***********************************************************
;*      ADD     A,NN             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADDn:
        mov     BYTE [FlagN],0
        add     al,[es:Z80+bp]
	pushf                           ;preserve O flag
        inc     bp
	popf
        jo      addn
        XinstC
addn:   XinstD

;***********************************************************
;*      ADC     A,B              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCb:
        mov     BYTE [FlagN],0
        adc     al,ch
        jo      adcb
        XinstC
adcb:   XinstD

;***********************************************************
;*      ADC     A,C              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCc:
        mov     BYTE [FlagN],0
        adc     al,cl
        jo      adcc
        XinstC
adcc:   XinstD

;***********************************************************
;*      ADC     A,D              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCd:
        mov     BYTE [FlagN],0
        adc     al,dh
        jo      adcd
        XinstC
adcd:   XinstD

;***********************************************************
;*      ADC     A,E              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCe:
        mov     BYTE [FlagN],0
        adc     al,dl
        jo      adce
        XinstC
adce:   XinstD

;***********************************************************
;*      ADC     A,H              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCh:
        mov     BYTE [FlagN],0
        adc     al,bh
        jo      adch
        XinstC
adch:   XinstD

;***********************************************************
;*      ADC     A,L              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCl:
        mov     BYTE [FlagN],0
        adc     al,bl
        jo      adcl
        XinstC
adcl:   XinstD

;***********************************************************
;*      ADC     A,A              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCa:
        mov     BYTE [FlagN],0
        adc     al,al
        jo      adca
        XinstC
adca:   XinstD

;***********************************************************
;*      ADC     A,(HL)           Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCz:
        mov     BYTE [FlagN],0
        adc     al,[es:Z80+bx]
        jo      adcz
        XinstC
adcz:   XinstD

;***********************************************************
;*      ADC     A,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],0
        adc     al,[es:Z80+si]
        jo      adcw
        XinstC
adcw:   XinstD

;***********************************************************
;*      ADC     A,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],0
        adc     al,[es:Z80+si]
        jo      adcv
        XinstC
adcv:   XinstD

;***********************************************************
;*      ADC     A,NN             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 ^  *
;***********************************************************
Z_ADCn:
        mov     BYTE [FlagN],0
        adc     al,[es:Z80+bp]
	pushf                           ;preserve O flag
        inc     bp
        popf
        jo      adcn
        XinstC
adcn:   XinstD

;***********************************************************
;*      SUB     A,B              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBb:
        mov     BYTE [FlagN],2
        sub     al,ch
        jo      subb
        XinstC
subb:   XinstD

;***********************************************************
;*      SUB     A,C              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBc:
        mov     BYTE [FlagN],2
        sub     al,cl
        jo      subc
        XinstC
subc:   XinstD

;***********************************************************
;*      SUB     A,D              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBd:
        mov     BYTE [FlagN],2
        sub     al,dh
        jo      subd
        XinstC
subd:   XinstD

;***********************************************************
;*      SUB     A,E              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBe:
        mov     BYTE [FlagN],2
        sub     al,dl
        jo      sube
        XinstC
sube:   XinstD

;***********************************************************
;*      SUB     A,H              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBh:
        mov     BYTE [FlagN],2
        sub     al,bh
        jo      subh
        XinstC
subh:   XinstD

;***********************************************************
;*      SUB     A,L              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBl:
        mov     BYTE [FlagN],2
        sub     al,bl
        jo      subl
        XinstC
subl:   XinstD

;***********************************************************
;*      SUB     A,A              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBa:
        mov     BYTE [FlagN],2
        sub     al,al
        jo      suba
        XinstC
suba:   XinstD

;***********************************************************
;*      SUB     A,(HL)           Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBz:
        mov     BYTE [FlagN],2
        sub     al,[es:Z80+bx]
        jo      subz
        XinstC
subz:   XinstD

;***********************************************************
;*      SUB     A,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],2
        sub     al,[es:Z80+si]
        jo      subw
        XinstC
subw:   XinstD

;***********************************************************
;*      SUB     A,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],2
        sub     al,[es:Z80+si]
        jo      subv
        XinstC
subv:   XinstD

;***********************************************************
;*      SUB     A,NN             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SUBn:
        mov     BYTE [FlagN],2
        sub     al,[es:Z80+bp]
	pushf                           ;preserve O flag
        inc     bp
        popf
        jo      subn
        XinstC
subn:   XinstD

;***********************************************************
;*      SBC     A,B              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCb:
        mov     BYTE [FlagN],2
        sbb     al,ch
        jo      sbcb
        XinstC
sbcb:   XinstD

;***********************************************************
;*      SBC     A,C              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCc:
        mov     BYTE [FlagN],2
        sbb     al,cl
        jo      sbcc
        XinstC
sbcc:   XinstD

;***********************************************************
;*      SBC     A,D              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCd:
        mov     BYTE [FlagN],2
        sbb     al,dh
        jo      sbcd
        XinstC
sbcd:   XinstD

;***********************************************************
;*      SBC     A,E              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCe:
        mov     BYTE [FlagN],2
        sbb     al,dl
        jo      sbce
        XinstC
sbce:   XinstD

;***********************************************************
;*      SBC     A,H              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCh:
        mov     BYTE [FlagN],2
        sbb     al,bh
        jo      sbch
        XinstC
sbch:   XinstD

;***********************************************************
;*      SBC     A,L              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCl:
        mov     BYTE [FlagN],2
        sbb     al,bl
        jo      sbcl
        XinstC
sbcl:   XinstD

;***********************************************************
;*      SBC     A,A              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCa:
        mov     BYTE [FlagN],2
        sbb     al,al
        jo      sbca
        XinstC
sbca:   XinstD

;***********************************************************
;*      SBC     A,(HL)           Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCz:
        mov     BYTE [FlagN],2
        sbb     al,[es:Z80+bx]
        jo      sbcz
        XinstC
sbcz:   XinstD

;***********************************************************
;*      SBC     A,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],2
        sbb     al,[es:Z80+si]
        jo      sbcw
        XinstC
sbcw:   XinstD

;***********************************************************
;*      SBC     A,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        mov     BYTE [FlagN],2
        sbb     al,[es:Z80+si]
        jo      sbcv
        XinstC
sbcv:   XinstD

;***********************************************************
;*      SBC     A,NN             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_SBCn:
        mov     BYTE [FlagN],2
        sbb     al,[es:Z80+bp]
	pushf                           ;preserve O flag
        inc     bp
        popf
        jo      sbcn
        XinstC
sbcn:   XinstD

;***********************************************************
;*      AND     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDb:
        and     al,ch
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDc:
        and     al,cl
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDd:
        and     al,dh
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDe:
        and     al,dl
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDh:
        and     al,bh
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDl:
        and     al,bl
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDa:
        and     al,al
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDz:
        and     al,[es:Z80+bx]
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        and     al,[es:Z80+si]
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        and     al,[es:Z80+si]
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        XinstB

;***********************************************************
;*      AND     NN               Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 1 x  V  0 0  *
;***********************************************************
Z_ANDn:
        and     al,[es:Z80+bp]
        mov     BYTE [FlagN],0
        lahf
        or      ah,00010000b
        inc     bp
        XinstB

;***********************************************************
;*      OR      B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORb:
        or      al,ch
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORc:
        or      al,cl
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORd:
        or      al,dh
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORe:
        or      al,dl
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORh:
        or      al,bh
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORl:
        or      al,bl
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORa:
        or      al,al
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORz:
        or      al,[es:Z80+bx]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        or      al,[es:Z80+si]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        or      al,[es:Z80+si]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      OR      NN               Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_ORn:
        or      al,[es:Z80+bp]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        inc     bp
        XinstB

;***********************************************************
;*      XOR     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORb:
        xor     al,ch
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORc:
        xor     al,cl
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORd:
        xor     al,dh
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORe:
        xor     al,dl
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORh:
        xor     al,bh
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORl:
        xor     al,bl
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORa:
        xor     al,al
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORz:
        xor     al,[es:Z80+bx]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        xor     al,[es:Z80+si]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        xor     al,[es:Z80+si]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      XOR     NN               Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  V  0 0  *
;***********************************************************
Z_XORn:
        xor     al,[es:Z80+bp]
        mov     BYTE [FlagN],0
        lahf
        and     ah,11101111b
        inc     bp
        XinstB

;***********************************************************
;*      CP      B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPb:
        cmp     al,ch
        mov     BYTE [FlagN],2
        jo      cpb
        XinstC
cpb:    XinstD

;***********************************************************
;*      CP      C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPc:
        cmp     al,cl
        mov     BYTE [FlagN],2
        jo      cpc
        XinstC
cpc:    XinstD

;***********************************************************
;*      CP      D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPd:
        cmp     al,dh
        mov     BYTE [FlagN],2
        jo      cpd
        XinstC
cpd:    XinstD

;***********************************************************
;*      CP      E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPe:
        cmp     al,dl
        mov     BYTE [FlagN],2
        jo      cpe
        XinstC
cpe:    XinstD

;***********************************************************
;*      CP      H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPh:
        cmp     al,bh
        mov     BYTE [FlagN],2
        jo      cph
        XinstC
cph:    XinstD

;***********************************************************
;*      CP      L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPl:
        cmp     al,bl
        mov     BYTE [FlagN],2
        jo      cpl
        XinstC
cpl:    XinstD

;***********************************************************
;*      CP      A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPa:
        cmp     al,al
        mov     BYTE [FlagN],2
        jo      cpa
        XinstC
cpa:    XinstD

;***********************************************************
;*      CP      (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPz:
        cmp     al,[es:Z80+bx]
        mov     BYTE [FlagN],2
        jo      cpz
        XinstC
cpz:    XinstD

;***********************************************************
;*      CP      (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        cmp     al,[es:Z80+si]
        mov     BYTE [FlagN],2
        jo      cpw
        XinstC
cpw:    XinstD

;***********************************************************
;*      CP      (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        cmp     al,[es:Z80+si]
        mov     BYTE [FlagN],2
        jo      cpv
        XinstC
cpv:    XinstD

;***********************************************************
;*      CP      NN               Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 ^  *
;***********************************************************
Z_CPn:
        cmp     al,[es:Z80+bp]
        mov     BYTE [FlagN],2
	pushf                           ;preserve O flag
        inc     bp
        popf
        jo      cpn
        XinstC
cpn:    XinstD

;***********************************************************
;*      INC     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCb:
        mov     BYTE [FlagN],0
        inc     ch
        jo      incb
        XinstC
incb:   XinstD

;***********************************************************
;*      INC     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCc:
        mov     BYTE [FlagN],0
        inc     cl
        jo      incc
        XinstC
incc:   XinstD

;***********************************************************
;*      INC     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCd:
        mov     BYTE [FlagN],0
        inc     dh
        jo      incd
        XinstC
incd:   XinstD

;***********************************************************
;*      INC     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCe:
        mov     BYTE [FlagN],0
        inc     dl
        jo      ince
        XinstC
ince:   XinstD

;***********************************************************
;*      INC     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCh:
        mov     BYTE [FlagN],0
        inc     bh
        jo      inch
        XinstC
inch:   XinstD

;***********************************************************
;*      INC     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCl:
        mov     BYTE [FlagN],0
        inc     bl
        jo      incl
        XinstC
incl:   XinstD

;***********************************************************
;*      INC     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCa:
        mov     BYTE [FlagN],0
        inc     al
        jo      inca
        XinstC
inca:   XinstD

;***********************************************************
;*      INC     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCz:
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+bx]
        jo      incz
        XinstC
incz:   XinstD

;***********************************************************
;*      INC     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        inc     BYTE [es:Z80+si]
        mov     BYTE [FlagN],0
        jo      incw
        XinstC
incw:   XinstD

;***********************************************************
;*      INC     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  0 .  *
;***********************************************************
Z_INCv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        inc     BYTE [es:Z80+si]
        mov     BYTE [FlagN],0
        jo      incv
        XinstC
incv:   XinstD

;***********************************************************
;*      DEC     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECb:
        mov     BYTE [FlagN],2
        dec     ch
        jo      decb
        XinstC
decb:   XinstD

;***********************************************************
;*      DEC     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECc:
        mov     BYTE [FlagN],2
        dec     cl
        jo      decc
        XinstC
decc:   XinstD

;***********************************************************
;*      DEC     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECd:
        mov     BYTE [FlagN],2
        dec     dh
        jo      decd
        XinstC
decd:   XinstD

;***********************************************************
;*      DEC     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECe:
        mov     BYTE [FlagN],2
        dec     dl
        jo      dece
        XinstC
dece:   XinstD

;***********************************************************
;*      DEC     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECh:
        mov     BYTE [FlagN],2
        dec     bh
        jo      dech
        XinstC
dech:   XinstD

;***********************************************************
;*      DEC     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECl:
        mov     BYTE [FlagN],2
        dec     bl
        jo      decl
        XinstC
decl:   XinstD

;***********************************************************
;*      DEC     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECa:
        mov     BYTE [FlagN],2
        dec     al
        jo      deca
        XinstC
deca:   XinstD

;***********************************************************
;*      DEC     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECz:
        mov     BYTE [FlagN],2
        dec     BYTE [es:Z80+bx]
        jo      decz
        XinstC
decz:   XinstD

;***********************************************************
;*      DEC     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        sahf
        dec     BYTE [es:Z80+si]
        mov     BYTE [FlagN],2
        jo      decw
        XinstC
decw:   XinstD

;***********************************************************
;*      DEC     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  V  1 .  *
;***********************************************************
Z_DECv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        sahf
        dec     BYTE [es:Z80+si]
        mov     BYTE [FlagN],2
        jo      decv
        XinstC
decv:   XinstD

        end
