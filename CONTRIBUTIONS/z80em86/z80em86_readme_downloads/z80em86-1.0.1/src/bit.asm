;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                              Bit Test Group                                 *
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

GLOBAL	Z_BIT0w, Z_BIT1w, Z_BIT2w, Z_BIT3w, Z_BIT4w, Z_BIT5w, Z_BIT6w, Z_BIT7w
GLOBAL	Z_BIT0v, Z_BIT1v, Z_BIT2v, Z_BIT3v, Z_BIT4v, Z_BIT5v, Z_BIT6v, Z_BIT7v
GLOBAL	Z_BIT0b, Z_BIT0c, Z_BIT0d, Z_BIT0e, Z_BIT0h, Z_BIT0l, Z_BIT0z, Z_BIT0a
GLOBAL	Z_BIT1b, Z_BIT1c, Z_BIT1d, Z_BIT1e, Z_BIT1h, Z_BIT1l, Z_BIT1z, Z_BIT1a
GLOBAL	Z_BIT2b, Z_BIT2c, Z_BIT2d, Z_BIT2e, Z_BIT2h, Z_BIT2l, Z_BIT2z, Z_BIT2a
GLOBAL	Z_BIT3b, Z_BIT3c, Z_BIT3d, Z_BIT3e, Z_BIT3h, Z_BIT3l, Z_BIT3z, Z_BIT3a
GLOBAL	Z_BIT4b, Z_BIT4c, Z_BIT4d, Z_BIT4e, Z_BIT4h, Z_BIT4l, Z_BIT4z, Z_BIT4a
GLOBAL	Z_BIT5b, Z_BIT5c, Z_BIT5d, Z_BIT5e, Z_BIT5h, Z_BIT5l, Z_BIT5z, Z_BIT5a
GLOBAL	Z_BIT6b, Z_BIT6c, Z_BIT6d, Z_BIT6e, Z_BIT6h, Z_BIT6l, Z_BIT6z, Z_BIT6a
GLOBAL	Z_BIT7b, Z_BIT7c, Z_BIT7d, Z_BIT7e, Z_BIT7h, Z_BIT7l, Z_BIT7z, Z_BIT7a

; declared in INSTHAND.ASM
EXTERN  Z80IX, Z80IY, FlagN

%include "macros.asm"

;***********************************************************
;*      BIT     0,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,B              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7b:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    ch,10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,C              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7c:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    cl,10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx                   ;use CX here instead of DX
        mov     ch,ah
        test    dh,00000001b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx                   ;use CX here instead of DX
        mov     ch,ah
        test    dh,00000010b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx                   ;use CX here instead of DX
        mov     ch,ah
        test    dh,00000100b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx                   ;use CX here instead of DX
        mov     ch,ah
        test    dh,00001000b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx                   ;use CX here instead of DX
        mov     ch,ah
        test    dh,00010000b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx                   ;use CX here instead of DX
        mov     ch,ah
        test    dh,00100000b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx                   ;use CX here instead of DX
        mov     ch,ah
        test    dh,01000000b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,D              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7d:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,cx
        mov     ch,ah                   ;use CX here instead of DX
        test    dh,10000000b
        lahf
        and     ah,01000000b
        or      ah,ch
        mov     cx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,E              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7e:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    dl,10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,H              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7h:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bh,10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,L              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7l:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    bl,10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,A              Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7a:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    al,10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,(HL)           Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7z:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+bx],10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     0,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     0,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT0w:
Z_BIT0v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],00000001b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     1,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     1,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT1w:
Z_BIT1v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],00000010b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     2,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     2,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT2w:
Z_BIT2v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],00000100b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     3,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     3,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT3w:
Z_BIT3v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],00001000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     4,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     4,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT4w:
Z_BIT4v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],00010000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     5,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     5,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT5w:
Z_BIT5v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],00100000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     6,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     6,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT6w:
Z_BIT6v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],01000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

;***********************************************************
;*      BIT     7,(IX+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;*      BIT     7,(IY+NN)        Flags: S Z - H - P/V N C  *
;*                                      x ^ x 1 x  x  0 .  *
;***********************************************************
Z_BIT7w:
Z_BIT7v:
        lahf
        and     ah,00000001b
        or      ah,00010000b
        mov     di,dx
        mov     dh,ah
        test    BYTE [es:Z80+si],10000000b
        lahf
        and     ah,01000000b
        or      ah,dh
        mov     dx,di
        mov     BYTE [FlagN],0
        XinstB

        end
