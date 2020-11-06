;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                                  Macros                                     *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 10 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Added 'BITS 16' and 'CPU 8086' and make all ASM files include this file.
; - Changed all uses of 'Z80' map segment to 'es:Z80'
;
; v1.00 - 5 April 1995 S.J.Kay
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

BITS 16
CPU 8086

EXTERN  T0

%MACRO	XinstA	0
        lahf                            ;get Z80 reg F into reg ah
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1                    ;T0 instruction table offset
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set 8086 Flags with Z80 reg F (ah)
        jmp     WORD [T0+di]
        %ENDMACRO

%MACRO	XinstB	0
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1                    ;T0 instruction table offset
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set 8086 Flags with Z80 reg F (ah)
        jmp     WORD [T0+di]
        %ENDMACRO

%MACRO	XinstC	0
        lahf                            ;get Z80 reg F into reg ah
        and     ah,11111011b            ;reset P/V to indicate no overflow
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1                    ;T0 instruction table offset
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set 8086 Flags with Z80 reg F (ah)
        jmp     WORD [T0+di]
        %ENDMACRO

%MACRO	XinstD	0
        lahf                            ;get Z80 reg F into reg ah
        or      ah,00000100b            ;set P/V to indicate overflow
        mov     si,ax                   ;save Z80 regs A and F
        xor     ah,ah
        mov     al,[es:Z80+bp]          ;get Z80 opcode
        mov     di,ax
        shl     di,1                    ;T0 instruction table offset
        inc     bp                      ;next Z80 PC
        mov     ax,si                   ;restore Z80 reg A and F
        sahf                            ;set 8086 Flags with Z80 reg F (ah)
        jmp     WORD [T0+di]
        %ENDMACRO
