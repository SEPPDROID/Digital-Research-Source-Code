;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                Exchange, Block Transfer, Block Search Groups                *
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

GLOBAL	Z_EXafaf, Z_EXX, Z_EXsphl, Z_EXdehl, Z_EXspx, Z_EXspy

GLOBAL	Z_CPI, Z_CPD, Z_CPIR, Z_CPDR
GLOBAL	Z_LDI, Z_LDD, Z_LDIR, Z_LDDR

; declared in INSTHAND.ASM
EXTERN  Z80SP, Z80IX, Z80IY
EXTERN  Z80AF, Z80BC, Z80DE, Z80HL
EXTERN  FlagN, FlagNx

%include "macros.asm"

tempb1  resb	1

;***********************************************************
;*      EX      DE,HL            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_EXdehl:
        xchg    dx,bx
        XinstA

;***********************************************************
;*      EXX     AF,AF'           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_EXafaf:
        mov     ah,[FlagN]
        xchg    ah,[FlagNx]
        mov     [FlagN],ah
        lahf
        xchg    ax,[Z80AF]
        XinstB

;***********************************************************
;*      EXX                      Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_EXX:
        xchg    cx,[Z80BC]
        xchg    dx,[Z80DE]
        xchg    bx,[Z80HL]
        XinstA

;***********************************************************
;*      EX      (SP),HL          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_EXsphl:
        mov     di,[Z80SP]
        xchg    bx,word [es:Z80+di]
        XinstA

;***********************************************************
;*      EX      (SP),IX          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_EXspx:
        mov     di,[Z80SP]
        mov     si,[Z80IX]
        xchg    si,word [es:Z80+di]
        mov     [Z80IX],si
        XinstA

;***********************************************************
;*      EX      (SP),IY          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_EXspy:
        mov     di,[Z80SP]
        mov     si,[Z80IY]
        xchg    si,word [es:Z80+di]
        mov     [Z80IY],si
        XinstA

;***********************************************************
;*      LDI                      Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  ^  0 .  *
;***********************************************************
Z_LDI:
        mov     BYTE [FlagN],0
        mov     ah,[es:Z80+bx]          ;source in HL
        mov     di,dx                   ;destination in DE
        mov     BYTE [es:Z80+di],ah
        lahf
        inc     bx
        inc     dx
        dec     cx
        and     ah,11101011b            ;lose half carry and parity flag
        jcxz    ldi
        or      ah,00000100b            ;set parity flag (BC <> 0)
ldi:    XinstB

;***********************************************************
;*      LDIR                     Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  0  0 .  *
;***********************************************************
Z_LDIR:
        lahf
        mov     si,bx                   ;source in HL
        mov     di,dx                   ;destination in DE
        push    ds
        push    es
        pop     ds
        cld
rep     movsb                           ;move memory
        pop     ds
        mov     bx,si
        mov     dx,di
        and     ah,11101011b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      LDD                      Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  ^  0 .  *
;***********************************************************
Z_LDD:
        mov     BYTE [FlagN],0
        mov     ah,[es:Z80+bx]          ;source in HL
        mov     di,dx                   ;destination in DE
        mov     BYTE [es:Z80+di],ah
        lahf
        dec     bx
        dec     dx
        dec     cx
        and     ah,11101011b            ;lose half carry and parity flag
        jcxz    ldd
        or      ah,00000100b            ;set parity flag (BC <> 0)
ldd:    XinstB

;***********************************************************
;*      LDDR                     Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  0  0 .  *
;***********************************************************
Z_LDDR:
        lahf
        mov     si,bx                   ;source in HL
        mov     di,dx                   ;destination in DE
        push    ds
        push    es
        pop     ds
        std
rep     movsb                           ;move memory
        pop     ds
        mov     bx,si
        mov     dx,di
        and     ah,11101011b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      CPI                      Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  ^  1 .  *
;***********************************************************
Z_CPI:
        lahf
        and     ah,00000001b            ;save the carry flag
        mov     [tempb1],ah
        mov     BYTE [FlagN],2
        cmp     al,[es:Z80+bx]          ;compare A=(HL)
        lahf
        inc     bx
        dec     cx
        and     ah,11111010b            ;lose the carry and parity flag
        or      ah,[tempb1]             ;and replace with old carry flag
        jcxz    cpi
        or      ah,00000100b            ;set parity flag (BC <> 0)
cpi:    XinstB

;***********************************************************
;*      CPIR                     Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  ^  1 .  *
;***********************************************************
Z_CPIR:
        lahf
        and     ah,00000001b            ;save the carry flag
        mov     [tempb1],ah
        mov     BYTE [FlagN],2
        mov     di,bx                   ;Z80 HL to DI reg
        cld
repne   scasb                           ;repeat until CX=0 or A=(HL)
        mov     bx,di                   ;DI reg TO Z80 HL
        lahf
        and     ah,11111010b            ;lose the carry and parity flag
        or      ah,[tempb1]             ;and replace with old carry flag
        jcxz    cpir
        or      ah,00000100b            ;set parity flag (BC <> 0)
cpir:   XinstB

;***********************************************************
;*      CPD                      Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  ^  1 .  *
;***********************************************************
Z_CPD:
        lahf
        and     ah,00000001b            ;save the carry flag
        mov     [tempb1],ah
        mov     BYTE [FlagN],2
        cmp     al,[es:Z80+bx]          ;compare A=(HL)
        lahf
        dec     bx
        dec     cx
        and     ah,11111010b            ;lose the carry and parity flag
        or      ah,[tempb1]             ;and replace with old carry flag
        jcxz    cpd
        or      ah,00000100b            ;set parity flag (BC <> 0)
cpd:    XinstB

;***********************************************************
;*      CPDR                     Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  ^  1 .  *
;***********************************************************
Z_CPDR:
        lahf
        and     ah,00000001b            ;save the carry flag
        mov     [tempb1],ah
        mov     BYTE [FlagN],2
        mov     di,bx                   ;Z80 HL to DI reg
        std
repne   scasb                           ;repeat until CX=0 or A=(HL)
        mov     bx,di                   ;DI reg TO Z80 HL
        lahf
        and     ah,11111010b            ;lose the carry and parity flag
        or      ah,[tempb1]             ;and replace with old carry flag
        jcxz    cpdr
        or      ah,00000100b            ;set parity flag (BC <> 0)
cpdr:   XinstB

        end
