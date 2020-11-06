;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                             16-bit Load Group                               *
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

GLOBAL	Z_LDxn,  Z_LDmx,   Z_LDxm,  Z_POPx,   Z_PUSHx, Z_LDspx
GLOBAL	Z_LDyn,  Z_LDmy,   Z_LDym,  Z_POPy,   Z_PUSHy, Z_LDspy

GLOBAL	Z_LDmbc, Z_LDbcm,  Z_LDmde, Z_LDdem,  Z_LDmsp, Z_LDspm

GLOBAL	Z_LDbcn, Z_LDden,  Z_LDhln, Z_LDmhl,  Z_LDhlm, Z_LDspn
GLOBAL	Z_POPbc, Z_PUSHbc, Z_POPde, Z_PUSHde, Z_POPhl, Z_PUSHhl
GLOBAL	Z_POPaf, Z_PUSHaf
GLOBAL	Z_LDsphl

; declared in INSTHAND.ASM
EXTERN  Z80SP, Z80IX, Z80IY
EXTERN  FlagN, FlagX

%include "macros.asm"

;***********************************************************
;*      LD      BC,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbcn:
        mov     cx,word [es:Z80+bp]
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      DE,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDden:
        mov     dx,word [es:Z80+bp]
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      HL,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhln:
        mov     bx,word [es:Z80+bp]
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      SP,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDspn:
        mov     di,word [es:Z80+bp]
        mov     [Z80SP],di
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      IX,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDxn:
        mov     di,word [es:Z80+bp]
        mov     [Z80IX],di
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      IY,NNNN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDyn:
        mov     di,word [es:Z80+bp]
        mov     [Z80IY],di
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      BC,(NNNN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbcm:
        mov     di,word [es:Z80+bp]
        mov     cx,word [es:Z80+di]
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      DE,(NNNN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdem:
        mov     di,word [es:Z80+bp]
        mov     dx,word [es:Z80+di]
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      HL,(NNNN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhlm:
        mov     di,word [es:Z80+bp]
        mov     bx,word [es:Z80+di]
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      SP,(NNNN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDspm:
        mov     di,word [es:Z80+bp]
        mov     si,word [es:Z80+di]
        mov     [Z80SP],si
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      IX,(NNNN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDxm:
        mov     di,word [es:Z80+bp]
        mov     si,word [es:Z80+di]
        mov     [Z80IX],si
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      IY,(NNNN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDym:
        mov     di,word [es:Z80+bp]
        mov     si,word [es:Z80+di]
        mov     [Z80IY],si
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      (NNNN),BC        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDmbc:
        mov     di,word [es:Z80+bp]
        mov     word [es:Z80+di],cx
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      (NNNN),DE        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDmde:
        mov     di,word [es:Z80+bp]
        mov     word [es:Z80+di],dx
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      (NNNN),HL        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDmhl:
        mov     di,word [es:Z80+bp]
        mov     word [es:Z80+di],bx
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      (NNNN),SP        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDmsp:
        mov     si,[Z80SP]
        mov     di,word [es:Z80+bp]
        mov     word [es:Z80+di],si
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      (NNNN),IX        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDmx:
        mov     si,[Z80IX]
        mov     di,word [es:Z80+bp]
        mov     word [es:Z80+di],si
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      (NNNN),IY        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDmy:
        mov     si,[Z80IY]
        mov     di,word [es:Z80+bp]
        mov     word [es:Z80+di],si
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      SP,HL            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDsphl:
        mov     [Z80SP],bx
        XinstA

;***********************************************************
;*      LD      SP,IX            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDspx:
        mov     di,[Z80IX]
        mov     [Z80SP],di
        XinstA

;***********************************************************
;*      LD      SP,IY            Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDspy:
        mov     di,[Z80IY]
        mov     [Z80SP],di
        XinstA

;***********************************************************
;*      PUSH    AF               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_PUSHaf:
        pushf
        lahf
        and     ah,11010101b            ;reset unused 8086 flags
        or      ah,[FlagN]              ;add in Z80 negative flag
        or      ah,[FlagX]              ;add in Z80 unused flags
        mov     di,[Z80SP]
        dec     di
        mov     BYTE [es:Z80+di],al     ;Z80 reg A on stack 1st
        dec     di
        mov     BYTE [es:Z80+di],ah     ;Z80 flag register last
        mov     [Z80SP],di
        popf
        XinstA

;***********************************************************
;*      PUSH    BC               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_PUSHbc:
        lahf
        mov     di,[Z80SP]
        dec     di
        dec     di
        mov     word [es:Z80+di],cx
        mov     [Z80SP],di
        XinstB

;***********************************************************
;*      PUSH    DE               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_PUSHde:
        lahf
        mov     di,[Z80SP]
        dec     di
        dec     di
        mov     word [es:Z80+di],dx
        mov     [Z80SP],di
        XinstB

;***********************************************************
;*      PUSH    HL               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_PUSHhl:
        lahf
        mov     di,[Z80SP]
        dec     di
        dec     di
        mov     word [es:Z80+di],bx
        mov     [Z80SP],di
        XinstB

;***********************************************************
;*      PUSH    IX               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_PUSHx:
        lahf
        mov     di,[Z80SP]
        dec     di
        dec     di
        mov     [Z80SP],di
        mov     si,[Z80IX]
        mov     word [es:Z80+di],si
        XinstB

;***********************************************************
;*      PUSH    IY               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_PUSHy:
        lahf
        mov     di,[Z80SP]
        dec     di
        dec     di
        mov     [Z80SP],di
        mov     si,[Z80IY]
        mov     word [es:Z80+di],si
        XinstB

;***********************************************************
;*      POP     AF               Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x ^ x  ^  ^ ^  *
;***********************************************************
Z_POPaf:
        mov     di,[Z80SP]
        mov     ah,[es:Z80+di]
        inc     di
        mov     al,[es:Z80+di]
        inc     di
        mov     [Z80SP],di
        mov     [FlagN],ah
        and     BYTE [FlagN],00000010b
        mov     [FlagX],ah
        and     BYTE [FlagX],00101000b
        and     ah,11010101b
        XinstB

;***********************************************************
;*      POP     BC               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_POPbc:
        lahf
        mov     di,[Z80SP]
        mov     cx,word [es:Z80+di]
        inc     di
        inc     di
        mov     [Z80SP],di
        XinstB

;***********************************************************
;*      POP     DE               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_POPde:
        lahf
        mov     di,[Z80SP]
        mov     dx,word [es:Z80+di]
        inc     di
        inc     di
        mov     [Z80SP],di
        XinstB

;***********************************************************
;*      POP     HL               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_POPhl:
        lahf
        mov     di,[Z80SP]
        mov     bx,word [es:Z80+di]
        inc     di
        inc     di
        mov     [Z80SP],di
        XinstB

;***********************************************************
;*      POP     IX               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_POPx:
        lahf
        mov     di,[Z80SP]
        mov     si,word [es:Z80+di]
        mov     [Z80IX],si
        inc     di
        inc     di
        mov     [Z80SP],di
        XinstB

;***********************************************************
;*      POP     IY               Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_POPy:
        lahf
        mov     di,[Z80SP]
        mov     si,word [es:Z80+di]
        mov     [Z80IY],si
        inc     di
        inc     di
        mov     [Z80SP],di
        XinstB

        end
