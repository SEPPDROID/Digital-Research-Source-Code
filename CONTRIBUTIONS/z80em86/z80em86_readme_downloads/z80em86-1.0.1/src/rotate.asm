;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                          Rotate and Shift Group                             *
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

GLOBAL	Z_RLCw, Z_RRCw, Z_RLw, Z_RRw, Z_SLAw, Z_SRAw, Z_SRLw
GLOBAL	Z_RLCv, Z_RRCv, Z_RLv, Z_RRv, Z_SLAv, Z_SRAv, Z_SRLv

GLOBAL	Z_RLCA, Z_RRCA, Z_RLA, Z_RRA

GLOBAL	Z_RLD, Z_RRD

GLOBAL	Z_RLCb, Z_RLCc, Z_RLCd, Z_RLCe, Z_RLCh, Z_RLCl, Z_RLCz, Z_RLCa
GLOBAL	Z_RRCb, Z_RRCc, Z_RRCd, Z_RRCe, Z_RRCh, Z_RRCl, Z_RRCz, Z_RRCa
GLOBAL	Z_RLb,  Z_RLc,  Z_RLd,  Z_RLe,  Z_RLh,  Z_RLl,  Z_RLz,  Z_RLa
GLOBAL	Z_RRb,  Z_RRc,  Z_RRd,  Z_RRe,  Z_RRh,  Z_RRl,  Z_RRz,  Z_RRa
GLOBAL	Z_SLAb, Z_SLAc, Z_SLAd, Z_SLAe, Z_SLAh, Z_SLAl, Z_SLAz, Z_SLAa
GLOBAL	Z_SRAb, Z_SRAc, Z_SRAd, Z_SRAe, Z_SRAh, Z_SRAl, Z_SRAz, Z_SRAa
GLOBAL	Z_SRLb, Z_SRLc, Z_SRLd, Z_SRLe, Z_SRLh, Z_SRLl, Z_SRLz, Z_SRLa

; declared in INSTHAND.ASM
EXTERN  Z80IX, Z80IY, FlagN

%include "macros.asm"

svebyt  resb	1

;***********************************************************
;*      RLCA                     Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  .  0 ^  *
;***********************************************************
Z_RLCA:
        rol     al,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      RLA                      Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  .  0 ^  *
;***********************************************************
Z_RLA:
        rcl     al,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      RRCA                     Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  .  0 ^  *
;***********************************************************
Z_RRCA:
        ror     al,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      RRA                      Flags: S Z - H - P/V N C  *
;*                                      . . x 0 x  .  0 ^  *
;***********************************************************
Z_RRA:
        rcr     al,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      RLC     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCb:
        rol     ch,1
        mov     BYTE [FlagN],0
        inc     ch
        dec     ch
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCc:
        rol     cl,1
        mov     BYTE [FlagN],0
        inc     cl
        dec     cl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCd:
        rol     dh,1
        mov     BYTE [FlagN],0
        inc     dh
        dec     dh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCe:
        rol     dl,1
        mov     BYTE [FlagN],0
        inc     dl
        dec     dl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCh:
        rol     bh,1
        mov     BYTE [FlagN],0
        inc     bh
        dec     bh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCl:
        rol     bl,1
        mov     BYTE [FlagN],0
        inc     bl
        dec     bl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCa:
        rol     al,1
        mov     BYTE [FlagN],0
        inc     al
        dec     al
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCz:
        rol     BYTE [es:Z80+bx],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+bx]
        dec     BYTE [es:Z80+bx]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RLC     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;*      RLC     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLCw:
Z_RLCv:
        rol     BYTE [es:Z80+si],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+si]
        dec     BYTE [es:Z80+si]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLb:
        rcl     ch,1
        mov     BYTE [FlagN],0
        inc     ch
        dec     ch
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLc:
        rcl     cl,1
        mov     BYTE [FlagN],0
        inc     cl
        dec     cl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLd:
        rcl     dh,1
        mov     BYTE [FlagN],0
        inc     dh
        dec     dh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLe:
        rcl     dl,1
        mov     BYTE [FlagN],0
        inc     dl
        dec     dl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLh:
        rcl     bh,1
        mov     BYTE [FlagN],0
        inc     bh
        dec     bh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLl:
        rcl     bl,1
        mov     BYTE [FlagN],0
        inc     bl
        dec     bl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLa:
        rcl     al,1
        mov     BYTE [FlagN],0
        inc     al
        dec     al
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLz:
        rcl     BYTE [es:Z80+bx],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+bx]
        dec     BYTE [es:Z80+bx]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RL      (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;*      RL      (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RLw:
Z_RLv:
        rcl     BYTE [es:Z80+si],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+si]
        dec     BYTE [es:Z80+si]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCb:
        ror     ch,1
        mov     BYTE [FlagN],0
        inc     ch
        dec     ch
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCc:
        ror     cl,1
        mov     BYTE [FlagN],0
        inc     cl
        dec     cl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCd:
        ror     dh,1
        mov     BYTE [FlagN],0
        inc     dh
        dec     dh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCe:
        ror     dl,1
        mov     BYTE [FlagN],0
        inc     dl
        dec     dl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCh:
        ror     bh,1
        mov     BYTE [FlagN],0
        inc     bh
        dec     bh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCl:
        ror     bl,1
        mov     BYTE [FlagN],0
        inc     bl
        dec     bl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCa:
        ror     al,1
        mov     BYTE [FlagN],0
        inc     al
        dec     al
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCz:
        ror     BYTE [es:Z80+bx],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+bx]
        dec     BYTE [es:Z80+bx]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRC     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;*      RRC     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRCw:
Z_RRCv:
        ror     BYTE [es:Z80+si],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+si]
        dec     BYTE [es:Z80+si]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRb:
        rcr     ch,1
        mov     BYTE [FlagN],0
        inc     ch
        dec     ch
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRc:
        rcr     cl,1
        mov     BYTE [FlagN],0
        inc     cl
        dec     cl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRd:
        rcr     dh,1
        mov     BYTE [FlagN],0
        inc     dh
        dec     dh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRe:
        rcr     dl,1
        mov     BYTE [FlagN],0
        inc     dl
        dec     dl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRh:
        rcr     bh,1
        mov     BYTE [FlagN],0
        inc     bh
        dec     bh
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRl:
        rcr     bl,1
        mov     BYTE [FlagN],0
        inc     bl
        dec     bl
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRa:
        rcr     al,1
        mov     BYTE [FlagN],0
        inc     al
        dec     al
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRz:
        rcr     BYTE [es:Z80+bx],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+bx]
        dec     BYTE [es:Z80+bx]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RR      (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;*      RR      (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_RRw:
Z_RRv:
        rcr     BYTE [es:Z80+si],1
        mov     BYTE [FlagN],0
        inc     BYTE [es:Z80+si]
        dec     BYTE [es:Z80+si]
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      SLA     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAb:
        shl     ch,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAc:
        shl     cl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAd:
        shl     dh,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAe:
        shl     dl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAh:
        shl     bh,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAl:
        shl     bl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAa:
        shl     al,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAz:
        shl     BYTE [es:Z80+bx],1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SLA     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;*      SLA     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SLAw:
Z_SLAv:
        shl     BYTE [es:Z80+si],1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAb:
        sar     ch,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAc:
        sar     cl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAd:
        sar     dh,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAe:
        sar     dl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAh:
        sar     bh,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAl:
        sar     bl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAa:
        sar     al,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAz:
        sar     BYTE [es:Z80+bx],1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRA     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;*      SRA     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRAw:
Z_SRAv:
        sar     BYTE [es:Z80+si],1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     B                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLb:
        shr     ch,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     C                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLc:
        shr     cl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     D                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLd:
        shr     dh,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     E                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLe:
        shr     dl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     H                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLh:
        shr     bh,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     L                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLl:
        shr     bl,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     A                Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLa:
        shr     al,1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     (HL)             Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLz:
        shr     BYTE [es:Z80+bx],1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      SRL     (IX+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;*      SRL     (IY+NN)          Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 ^  *
;***********************************************************
Z_SRLw:
Z_SRLv:
        shr     BYTE [es:Z80+si],1
        lahf
        and     ah,11101111b
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      RLD                      Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 .  *
;***********************************************************
Z_RLD:
        pushf                           ;must save C flag
        mov     [svebyt],al
        and     BYTE [svebyt],00001111b
        and     al,11110000b
        mov     ah,[es:Z80+bx]
        shr     ah,1
        shr     ah,1
        shr     ah,1
        shr     ah,1
        shl     BYTE [es:Z80+bx],1
        shl     BYTE [es:Z80+bx],1
        shl     BYTE [es:Z80+bx],1
        shl     BYTE [es:Z80+bx],1
        or      al,ah
        mov     ah,[svebyt]
        or      BYTE [es:Z80+bx],ah
        popf
        mov     BYTE [FlagN],0
        inc     al
        dec     al
        lahf
        and     ah,11101111b
        XinstB

;***********************************************************
;*      RRD                      Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x  P  0 .  *
;***********************************************************
Z_RRD:
        pushf                           ;must save C flag
        mov     [svebyt],al
        shl     BYTE [svebyt],1
        shl     BYTE [svebyt],1
        shl     BYTE [svebyt],1
        shl     BYTE [svebyt],1
        and     al,11110000b
        mov     ah,[es:Z80+bx]
        and     ah,00001111b
        shr     BYTE [es:Z80+bx],1
        shr     BYTE [es:Z80+bx],1
        shr     BYTE [es:Z80+bx],1
        shr     BYTE [es:Z80+bx],1
        or      al,ah
        mov     ah,[svebyt]
        or      BYTE [es:Z80+bx],ah
        popf
        mov     BYTE [FlagN],0
        inc     al
        dec     al
        lahf
        and     ah,11101111b
        XinstB

        end
