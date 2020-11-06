;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                              8-bit Load Group                               *
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

GLOBAL	Z_LDwn, Z_LDbw, Z_LDcw, Z_LDdw, Z_LDew
GLOBAL	Z_LDhw, Z_LDlw, Z_LDwb, Z_LDwc, Z_LDwd, Z_LDwe, Z_LDwh, Z_LDwl
GLOBAL	Z_LDwa, Z_LDaw

GLOBAL	Z_LDvn, Z_LDbv, Z_LDcv, Z_LDdv, Z_LDev
GLOBAL	Z_LDhv, Z_LDlv, Z_LDvb, Z_LDvc, Z_LDvd, Z_LDve, Z_LDvh, Z_LDvl
GLOBAL	Z_LDva, Z_LDav

GLOBAL	Z_LDia, Z_LDra, Z_LDai, Z_LDar

GLOBAL	Z_LDbca, Z_LDbn, Z_LDabc, Z_LDcn, Z_LDdea, Z_LDdn, Z_LDade
GLOBAL	Z_LDen,  Z_LDhn, Z_LDln,  Z_LDma, Z_LDzn,  Z_LDam, Z_LDan

GLOBAL	Z_LDbb, Z_LDbc, Z_LDbd, Z_LDbe, Z_LDbh, Z_LDbl, Z_LDbz, Z_LDba
GLOBAL	Z_LDcb, Z_LDcc, Z_LDcd, Z_LDce, Z_LDch, Z_LDcl, Z_LDcz, Z_LDca
GLOBAL	Z_LDdb, Z_LDdc, Z_LDdd, Z_LDde, Z_LDdh, Z_LDdl, Z_LDdz, Z_LDda
GLOBAL	Z_LDeb, Z_LDec, Z_LDed, Z_LDee, Z_LDeh, Z_LDel, Z_LDez, Z_LDea
GLOBAL	Z_LDhb, Z_LDhc, Z_LDhd, Z_LDhe, Z_LDhh, Z_LDhl, Z_LDhz, Z_LDha
GLOBAL	Z_LDlb, Z_LDlc, Z_LDld, Z_LDle, Z_LDlh, Z_LDll, Z_LDlz, Z_LDla
GLOBAL	Z_LDzb, Z_LDzc, Z_LDzd, Z_LDze, Z_LDzh, Z_LDzl,         Z_LDza
GLOBAL	Z_LDab, Z_LDac, Z_LDad, Z_LDae, Z_LDah, Z_LDal, Z_LDaz, Z_LDaa

; declared in INSTHAND.ASM
EXTERN  Z80IX, Z80IY, Z80I, Z80R
EXTERN  IFF,   FlagN

%include "macros.asm"

;***********************************************************
;*      LD      B,B              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbb:
        XinstA

;***********************************************************
;*      LD      B,C              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbc:
        mov     ch,cl
        XinstA

;***********************************************************
;*      LD      B,D              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbd:
        mov     ch,dh
        XinstA

;***********************************************************
;*      LD      B,E              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbe:
        mov     ch,dl
        XinstA

;***********************************************************
;*      LD      B,H              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbh:
        mov     ch,bh
        XinstA

;***********************************************************
;*      LD      B,L              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbl:
        mov     ch,bl
        XinstA

;***********************************************************
;*      LD      B,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDba:
        mov     ch,al
        XinstA

;***********************************************************
;*      LD      C,B              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcb:
        mov     cl,ch
        XinstA

;***********************************************************
;*      LD      C,C              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcc:
        XinstA

;***********************************************************
;*      LD      C,D              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcd:
        mov     cl,dh
        XinstA

;***********************************************************
;*      LD      C,E              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDce:
        mov     cl,dl
        XinstA

;***********************************************************
;*      LD      C,H              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDch:
        mov     cl,bh
        XinstA

;***********************************************************
;*      LD      C,L              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcl:
        mov     cl,bl
        XinstA

;***********************************************************
;*      LD      C,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDca:
        mov     cl,al
        XinstA

;***********************************************************
;*      LD      D,B              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdb:
        mov     dh,ch
        XinstA

;***********************************************************
;*      LD      D,C              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdc:
        mov     dh,cl
        XinstA

;***********************************************************
;*      LD      D,D              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdd:
        XinstA

;***********************************************************
;*      LD      D,E              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDde:
        mov     dh,dl
        XinstA

;***********************************************************
;*      LD      D,H              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdh:
        mov     dh,bh
        XinstA

;***********************************************************
;*      LD      D,L              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdl:
        mov     dh,bl
        XinstA

;***********************************************************
;*      LD      D,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDda:
        mov     dh,al
        XinstA

;***********************************************************
;*      LD      E,B              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDeb:
        mov     dl,ch
        XinstA

;***********************************************************
;*      LD      E,C              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDec:
        mov     dl,cl
        XinstA

;***********************************************************
;*      LD      E,D              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDed:
        mov     dl,dh
        XinstA

;***********************************************************
;*      LD      E,E              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDee:
        XinstA

;***********************************************************
;*      LD      E,H              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDeh:
        mov     dl,bh
        XinstA

;***********************************************************
;*      LD      E,L              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDel:
        mov     dl,bl
        XinstA

;***********************************************************
;*      LD      E,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDea:
        mov     dl,al
        XinstA

;***********************************************************
;*      LD      H,B              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhb:
        mov     bh,ch
        XinstA

;***********************************************************
;*      LD      H,C              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhc:
        mov     bh,cl
        XinstA

;***********************************************************
;*      LD      H,D              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhd:
        mov     bh,dh
        XinstA

;***********************************************************
;*      LD      H,E              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhe:
        mov     bh,dl
        XinstA

;***********************************************************
;*      LD      H,H              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhh:
        XinstA

;***********************************************************
;*      LD      H,L              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhl:
        mov     bh,bl
        XinstA

;***********************************************************
;*      LD      H,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDha:
        mov     bh,al
        XinstA

;***********************************************************
;*      LD      L,B              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDlb:
        mov     bl,ch
        XinstA

;***********************************************************
;*      LD      L,C              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDlc:
        mov     bl,cl
        XinstA

;***********************************************************
;*      LD      L,D              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDld:
        mov     bl,dh
        XinstA

;***********************************************************
;*      LD      L,E              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDle:
        mov     bl,dl
        XinstA

;***********************************************************
;*      LD      L,H              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDlh:
        mov     bl,bh
        XinstA

;***********************************************************
;*      LD      L,L              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDll:
        XinstA

;***********************************************************
;*      LD      L,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDla:
        mov     bl,al
        XinstA

;***********************************************************
;*      LD      A,B              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDab:
        mov     al,ch
        XinstA

;***********************************************************
;*      LD      A,C              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDac:
        mov     al,cl
        XinstA

;***********************************************************
;*      LD      A,D              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDad:
        mov     al,dh
        XinstA

;***********************************************************
;*      LD      A,E              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDae:
        mov     al,dl
        XinstA

;***********************************************************
;*      LD      A,H              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDah:
        mov     al,bh
        XinstA

;***********************************************************
;*      LD      A,L              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDal:
        mov     al,bl
        XinstA

;***********************************************************
;*      LD      A,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDaa:
        XinstA

;***********************************************************
;*      LD      B,NN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbn:
        mov     ch,[es:Z80+bp]
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      C,NN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcn:
        mov     cl,[es:Z80+bp]
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      D,NN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdn:
        mov     dh,[es:Z80+bp]
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      E,NN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDen:
        mov     dl,[es:Z80+bp]
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      H,NN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhn:
        mov     bh,[es:Z80+bp]
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      L,NN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDln:
        mov     bl,[es:Z80+bp]
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      A,NN             Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDan:
        mov     al,[es:Z80+bp]
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      B,(HL)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbz:
        mov     ch,[es:Z80+bx]
        XinstA

;***********************************************************
;*      LD      C,(HL)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcz:
        mov     cl,[es:Z80+bx]
        XinstA

;***********************************************************
;*      LD      D,(HL)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdz:
        mov     dh,[es:Z80+bx]
        XinstA

;***********************************************************
;*      LD      E,(HL)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDez:
        mov     dl,[es:Z80+bx]
        XinstA

;***********************************************************
;*      LD      H,(HL)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhz:
        mov     bh,[es:Z80+bx]
        XinstA

;***********************************************************
;*      LD      L,(HL)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDlz:
        mov     bl,[es:Z80+bx]
        XinstA

;***********************************************************
;*      LD      A,(HL)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDaz:
        mov     al,[es:Z80+bx]
        XinstA

;***********************************************************
;*      LD      B,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     ch,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      C,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     cl,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      D,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     dh,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      E,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDew:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     dl,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      H,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     bh,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      L,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDlw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     bl,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      A,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDaw:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     al,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      B,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     ch,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      C,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDcv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     cl,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      D,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     dh,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      E,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDev:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     dl,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      H,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDhv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     bh,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      L,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDlv:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     bl,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      A,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDav:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     al,[es:Z80+si]
        XinstB

;***********************************************************
;*      LD      (HL),B           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDzb:
        mov     BYTE [es:Z80+bx],ch
        XinstA

;***********************************************************
;*      LD      (HL),C           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDzc:
        mov     BYTE [es:Z80+bx],cl
        XinstA

;***********************************************************
;*      LD      (HL),D           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDzd:
        mov     BYTE [es:Z80+bx],dh
        XinstA

;***********************************************************
;*      LD      (HL),E           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDze:
        mov     BYTE [es:Z80+bx],dl
        XinstA

;***********************************************************
;*      LD      (HL),H           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDzh:
        mov     BYTE [es:Z80+bx],bh
        XinstA

;***********************************************************
;*      LD      (HL),L           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDzl:
        mov     BYTE [es:Z80+bx],bl
        XinstA

;***********************************************************
;*      LD      (HL),A           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDza:
        mov     BYTE [es:Z80+bx],al
        XinstA

;***********************************************************
;       LD      (IX+NN),B        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwb:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],ch
        XinstB

;***********************************************************
;*      LD      (IX+NN),C        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwc:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],cl
        XinstB

;***********************************************************
;*      LD      (IX+NN),D        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwd:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],dh
        XinstB

;***********************************************************
;*      LD      (IX+NN),E        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwe:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],dl
        XinstB

;***********************************************************
;*      LD      (IX+NN),H        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwh:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],bh
        XinstB

;***********************************************************
;*      LD      (IX+NN),L        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwl:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],bl
        XinstB

;***********************************************************
;*      LD      (IX+NN),A        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwa:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],al
        XinstB

;***********************************************************
;*      LD      (IY+NN),B        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDvb:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],ch
        XinstB

;***********************************************************
;*      LD      (IY+NN),C        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDvc:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],cl
        XinstB

;***********************************************************
;*      LD      (IY+NN),D        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDvd:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],dh
        XinstB

;***********************************************************
;*      LD      (IY+NN),E        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDve:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],dl
        XinstB

;***********************************************************
;*      LD      (IY+NN),H        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDvh:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],bh
        XinstB

;***********************************************************
;*      LD      (IY+NN),L        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDvl:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],bl
        XinstB

;***********************************************************
;*      LD      (IY+NN),A        Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDva:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]
        inc     bp
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        mov     ax,di
        mov     BYTE [es:Z80+si],al
        XinstB

;***********************************************************
;*      LD      (HL),NN          Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDzn:
        mov     ah,[es:Z80+bp]
        mov     BYTE [es:Z80+bx],ah
        lahf
        inc     bp
        XinstB

;***********************************************************
;*      LD      (IX+NN),NN       Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDwn:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]          ;index offset value NN
        cbw
        add     ax,[Z80IX]
        mov     si,ax
        inc     bp
        mov     ah,[es:Z80+bp]          ;imediate value NN
        inc     bp
        mov     BYTE [es:Z80+si],ah
        mov     ax,di
        XinstB

;***********************************************************
;*      LD      (IY+NN),NN       Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDvn:
        lahf
        mov     di,ax
        mov     al,[es:Z80+bp]          ;index offset value NN
        cbw
        add     ax,[Z80IY]
        mov     si,ax
        inc     bp
        mov     ah,[es:Z80+bp]          ;imediate value NN
        inc     bp
        mov     BYTE [es:Z80+si],ah
        mov     ax,di
        XinstB

;***********************************************************
;*      LD      A,(BC)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDabc:
        mov     di,cx
        mov     al,[es:Z80+di]
        XinstA

;***********************************************************
;*      LD      A,(DE)           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDade:
        mov     di,dx
        mov     al,[es:Z80+di]
        XinstA

;***********************************************************
;*      LD      A,(NNNN)         Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDam:
        mov     di,word [es:Z80+bp]
        mov     al,[es:Z80+di]
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      (BC),A           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDbca:
        mov     di,cx
        mov     BYTE [es:Z80+di],al
        XinstA

;***********************************************************
;*      LD      (DE),A           Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDdea:
        mov     di,dx
        mov     BYTE [es:Z80+di],al
        XinstA

;***********************************************************
;*      LD      (NNNN),A         Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDma:
        mov     di,word [es:Z80+bp]
        mov     BYTE [es:Z80+di],al
        lahf
        inc     bp
        inc     bp
        XinstB

;***********************************************************
;*      LD      A,I              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x IFF 0 .  *
;***********************************************************
Z_LDai:
        mov     BYTE [FlagN],0
        mov     al,[Z80I]
        inc     al                      ;updates S and Z flags
        dec     al
        lahf
        and     ah,11101011b            ;lose half carry, P/V flag
        or      ah,[IFF]                ;IFF status into P/V flag
        XinstB

;***********************************************************
;*      LD      A,R              Flags: S Z - H - P/V N C  *
;*                                      ^ ^ x 0 x IFF 0 .  *
;***********************************************************
Z_LDar:
        mov     BYTE [FlagN],0
        mov     al,[Z80R]
        inc     al                      ;updates S and Z flags
        dec     al
        lahf
        and     ah,11101011b            ;lose half carry, P/V flag
        or      ah,[IFF]                ;IFF status into P/V flag
        inc     BYTE [Z80R]             ;reg R simulated update
        and     BYTE [Z80R],01111111b   ;reg R is 7 bits when read
        XinstB

;***********************************************************
;*      LD      I,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDia:
        lahf
        mov     [Z80I],al
        and     BYTE [Z80I],11111110b   ;simulate reg I
        XinstB

;***********************************************************
;*      LD      R,A              Flags: S Z - H - P/V N C  *
;*                                      . . x . x  .  . .  *
;***********************************************************
Z_LDra:
        lahf
        mov     [Z80R],al
        and     BYTE [Z80R],01111111b   ;simulate reg R
        XinstB

        end
