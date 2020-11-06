;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                          Input and Output Group                             *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 10 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Changed 'lea' instructions to  'mov'.
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

SEGMENT	.code	public align=16

GLOBAL	Z_INan
GLOBAL	Z_INbc,  Z_INcc,  Z_INdc,  Z_INec
GLOBAL	Z_INhc,  Z_INlc,  Z_INfc,  Z_INac
GLOBAL	Z_OUTcb, Z_OUTcc, Z_OUTcd, Z_OUTce
GLOBAL	Z_OUTch, Z_OUTcl, Z_OUTca
GLOBAL	Z_INI,   Z_INIR,  Z_IND,   Z_INDR
GLOBAL	Z_OUTI,  Z_OTIR,  Z_OUTD,  Z_OTDR

; declared in FUNCTION.ASM
EXTERN  Nocde0, Nocde1

%include "macros.asm"

;***********************************************************
;*      IN      B,(C)            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  0 .  *
;***********************************************************
Z_INbc:
        mov     si,inbc
        jmp     NEAR Nocde0
inbc:   db      'IN      B,(C)$'

;***********************************************************
;*      IN      C,(C)            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  0 .  *
;***********************************************************
Z_INcc:
        mov     si,incc_
        jmp     NEAR Nocde0
incc_:  db      'IN      C,(C)$'

;***********************************************************
;*      IN      D,(C)            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  0 .  *
;***********************************************************
Z_INdc:
        mov     si,indc
        jmp     NEAR Nocde0
indc:   db      'IN      D,(C)$'

;***********************************************************
;*      IN      E,(C)            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  0 .  *
;***********************************************************
Z_INec:
        mov     si,inec
        jmp     NEAR Nocde0
inec:   db      'IN      E,(C)$'

;***********************************************************
;*      IN      H,(C)            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  0 .  *
;***********************************************************
Z_INhc:
        mov     si,inhc
        jmp     NEAR Nocde0
inhc:   db      'IN      H,(C)$'

;***********************************************************
;*      IN      L,(C)            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  0 .  *
;***********************************************************
Z_INlc:
        mov     si,inlc
        jmp     NEAR Nocde0
inlc:   db      'IN      L,(C)$'

;***********************************************************
;*      IN      A,(C)            Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  P  0 .  *
;***********************************************************
Z_INac:
        mov     si,inac
        jmp     NEAR Nocde0
inac:   db      'IN      A,(C)$'

;***********************************************************
;*      IN      (HL),(C)         Flags: S Z - H - P/V N C  *
;*                                      ? ? x ? x  ?  ? ?  *
;***********************************************************
Z_INfc:
        mov     si,infc
        jmp     NEAR Nocde0
infc:   db      'IN      (HL),(C)$'

;***********************************************************
;*      IN      A,(NN)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_INan:
        mov     si,inan
        jmp     NEAR Nocde1
inan:   db      'IN      A,(NN)$'

;***********************************************************
;*      INI                      Flags: S Z - H - P/V N C  *
;*                                      x ^ x x x  x  1 x  *
;***********************************************************
Z_INI:
        mov     si,ini
        jmp     NEAR Nocde0
ini:    db      'INI$'

;***********************************************************
;*      IND                      Flags: S Z - H - P/V N C  *
;*                                      x ^ x x x  x  1 x  *
;***********************************************************
Z_IND:
        mov     si,ind
        jmp     NEAR Nocde0
ind:    db      'IND$'

;***********************************************************
;*      INIR                     Flags: S Z - H - P/V N C  *
;*                                      x 1 x x x  x  1 x  *
;***********************************************************
Z_INIR:
        mov     si,inir
        jmp     NEAR Nocde0
inir:   db      'INIR$'

;***********************************************************
;*      INDR                     Flags: S Z - H - P/V N C  *
;*                                      x 1 x x x  x  1 x  *
;***********************************************************
Z_INDR:
        mov     si,indr
        jmp     NEAR Nocde0
indr:   db      'INDR$'

;***********************************************************
;*      OUT     (C),B            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_OUTcb:
        mov     si,outcb
        jmp     NEAR Nocde0
outcb:  db      'OUT     (C),B$'

;***********************************************************
;*      OUT     (C),C            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_OUTcc:
        mov     si,outcc
        jmp     NEAR Nocde0
outcc:  db      'OUT     (C),C$'

;***********************************************************
;*      OUT     (C),D            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_OUTcd:
        mov     si,outcd
        jmp     NEAR Nocde0
outcd:  db      'OUT     (C),D$'

;***********************************************************
;*      OUT     (C),E            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_OUTce:
        mov     si,outce
        jmp     NEAR Nocde0
outce:  db      'OUT     (C),E$'

;***********************************************************
;*      OUT     (C),H            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_OUTch:
        mov     si,outch
        jmp     NEAR Nocde0
outch:  db      'OUT     (C),H$'

;***********************************************************
;*      OUT     (C),L            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_OUTcl:
        mov     si,outcl
        jmp     NEAR Nocde0
outcl:  db      'OUT     (C),L$'

;***********************************************************
;*      OUT     (C),A            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_OUTca:
        mov     si,outca
        jmp     NEAR Nocde0
outca:  db      'OUT     (C),A$'

;***********************************************************
;*      OUT     (NN),A           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************

; used for 8086 interfacing

;***********************************************************
;*      OUTI                     Flags: S Z - H - P/V N C  *
;*                                      x ^ x x x  x  1 x  *
;***********************************************************
Z_OUTI:
        mov     si,outi
        jmp     NEAR Nocde0
outi:   db      'OUTI$'

;***********************************************************
;*      OUTD                     Flags: S Z - H - P/V N C  *
;*                                      x ^ x x x  x  1 x  *
;***********************************************************
Z_OUTD:
        mov     si,outd
        jmp     NEAR Nocde0
outd:   db      'OUTD$'

;***********************************************************
;*      OTIR                     Flags: S Z - H - P/V N C  *
;*                                      x 1 x x x  x  1 x  *
;***********************************************************
Z_OTIR:
        mov     si,otir
        jmp     NEAR Nocde0
otir:   db      'OTIR$'

;***********************************************************
;*      OTDR                     Flags: S Z - H - P/V N C  *
;*                                      x 1 x x x  x  1 x  *
;***********************************************************
Z_OTDR:
        mov     si,otdr
        jmp     NEAR Nocde0
otdr:   db      'OTDR$'

        end
