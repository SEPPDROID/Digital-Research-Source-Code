;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                 Video function support for Z80 system code                  *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
; Televideo 920/ADM-3A Video Terminal,  DOS video service
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 10 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
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

GLOBAL	crt1in, crt1st, crt1op
GLOBAL	crt2in, crt2st, crt2op

; declared in MAIN.ASM
EXTERN  vidoff

%include "macros.asm"
;
;===============================================================================
; CRT #2 Video functions (STDOUT): Initialize, Status, Output
;   crt2in:-
;   crt2st:-  Return: STDOUT status in reg A
;   crt2op:-    Pass: STDOUT character in reg C
;===============================================================================
crt2in: retn

crt2st: mov     al,0ffh                 ;output always ready
        retn

crt2op: cmp     BYTE [vidoff],0         ;is video turned off ?
        jz      crt2o
        retn
crt2o:  mov     ah,002h                 ;character output function
        mov     dl,cl                   ;character to reg DL
        int     021h
        retn

;===============================================================================
; CRT #1 Video functions: Initialize, Status, Output
;   crt1in:-
;   crt1st:-  Return: status in reg A
;   crt1op:-    Pass: character in reg C
;===============================================================================
; Initialize CRT output driver
crt1in: mov     BYTE [atrbyt],00000111b ;set video atributes to use
        mov     BYTE [atrflg],00000000b
        mov     BYTE [esccnt],0
        mov     cl,26
        jmp     crt1op                  ;clear screen
;
; Return CRT status in Z80 reg A
crt1st: mov     al,0ffh                 ;return CRT always ready
        retn
;
; Send character in Z80 reg C to CRT
crt1op: cmp     BYTE [vidoff],0         ;is video turned off ?
        jz      crt1o
        retn
crt1o:  cmp     BYTE [esccnt],0         ;any escape codes expected ?
        jnz     esccde
        cmp     cl,' '                  ;control or printable character ?
        jnc     print0
        xor     ch,ch
        mov     di,cx
        shl     di,1
        jmp     WORD [ctltbl+di]        ;jump to control character handler
;
setesc: mov     BYTE [esccnt],1
        retn
esccde: inc     BYTE [esccnt]
        cmp     BYTE [esccnt],2
        jnz     escact
        mov     [escchr],cl             ;save the 1st escape character
escact: mov     al,[escchr]
        xor     ah,ah
        mov     di,ax
        shl     di,1
        jmp     WORD [esctbl+di]        ;jump to escape sequence handler
;
esc___: mov     BYTE [esccnt],0         ;unsupported escape codes here
        jmp     crt1op                  ;print the character
;
ctl___: retn                            ;unsupported control codes here
;
print0: mov     ah,009h                 ;write char and attribute function
        mov     al,' '
        mov     bh,000h                 ;page #0
        mov     bl,[atrbyt]             ;attribute value
        mov     dl,cl                   ;save the character to display
        mov     cx,1                    ;write 1 character
        int     010h
        mov     cl,dl                   ;restore the character to display
print1: mov     ah,00eh                 ;write TTY function
        mov     al,cl                   ;character to AL
        mov     bh,000h                 ;page #0
        int     010h
        retn
;
hghint: mov     BYTE [esccnt],0         ;no more escape codes follow
        test    BYTE [atrflg],00000100b ;is reverse video on ?
        jnz     inten1
        test    BYTE [atrflg],00000001b ;is underline on ?
        mov     ah,00001001b
        jnz     inten0
        mov     ah,00001111b
inten0: and     BYTE [atrbyt],10000000b
        or      [atrbyt],ah
inten1: or      BYTE [atrflg],00000010b ;set high intensity flag on
        retn
;
lowint: mov     BYTE [esccnt],0         ;no more escape codes follow
        test    BYTE [atrflg],00000100b ;is reverse video on ?
        jnz     inten3
        test    BYTE [atrflg],00000001b ;is underline on ?
        mov     ah,00000001b
        jnz     inten2
        mov     ah,00000111b
inten2: and     BYTE [atrbyt],10000000b
        or      [atrbyt],ah
inten3: and     BYTE [atrflg],11111101b ;set high intensity flag off
        retn
;
blnkon: mov     BYTE [esccnt],0         ;no more escape codes follow
        or      BYTE [atrbyt],10000000b
        retn
;
blnkof: mov     BYTE [esccnt],0         ;no more escape codes follow
        and     BYTE [atrbyt],01111111b
        retn
;
rvseon: mov     BYTE [esccnt],0         ;no more escape codes follow
        or      BYTE [atrflg],00000100b ;set reverse flag on
        and     BYTE [atrbyt],10000000b
        or      BYTE [atrbyt],01110000b
        retn
;
rvseof: mov     BYTE [esccnt],0         ;no more escape codes follow
        and     BYTE [atrflg],11111011b ;set reverse flag off
        test    BYTE [atrflg],00000010b ;set low or high intensity ?
        jz      lowint
        jmp     hghint
;
undron: mov     BYTE [esccnt],0         ;no more escape codes follow
        test    BYTE [atrflg],00000100b
        jnz     under1
        test    BYTE [atrbyt],00001000b
        mov     ah,00001001b
        jnz     under0
        mov     ah,00000001b
under0: and     BYTE [atrbyt],10000000b
        or      [atrbyt],ah
under1: or      BYTE [atrflg],00000001b ;set underline flag on
        retn
;
undrof: mov     BYTE [esccnt],0         ;no more escape codes follow
        test    BYTE [atrflg],00000100b
        jnz     under3
        test    BYTE [atrbyt],00001000b
        mov     ah,00001111b
        jnz     under2
        mov     ah,00000111b
under2: and     BYTE [atrbyt],11110000b
        or      [atrbyt],ah
under3: and     BYTE [atrflg],11111110b ;set underline flag off
        retn
;
setcur: cmp     BYTE [esccnt],2
        jnz     stcur0
        retn
stcur0: cmp     BYTE [esccnt],3
        jnz     stcur1
        mov     [escdta],cl             ;save Y cursor position
extcur: retn
stcur1: mov     BYTE [esccnt],0         ;no more escape codes follow
        mov     dh,[escdta]
        sub     dh,' '                  ;adjust (Y = 0 for 1st line)
        cmp     dh,25                   ;is Y in range ?
        jnc     extcur
        mov     dl,cl
        sub     dl,' '                  ;adjust (X = 0 for 1st column)
        cmp     dl,80                   ;is X in range ?
        jnc     extcur
        jmp     cursor
;
clreos: mov     BYTE [esccnt],0         ;no more escape codes follow
        mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        cmp     dh,25
        jc      clesc0
        retn
clesc0: cmp     dl,80
        jc      clesc1
        retn
clesc1: mov     al,80
        mul     dh
        mov     dh,0
        add     ax,dx
        mov     cx,2000
        sub     cx,ax                   ;number of characters to clear
        jmp     scrclr
;
clreol: mov     BYTE [esccnt],0         ;no more escape codes follow
        mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        cmp     dl,80
        jc      clrlne
        retn
clrlne: mov     cx,80
        mov     dh,0
        sub     cx,dx                   ;number of characters to clear
scrclr: mov     ah,009h                 ;write char and attribute function
        mov     al,' '
        mov     bh,000h                 ;page #0
        mov     bl,007h                 ;attribute value
        int     010h
        retn
;
lneins: mov     BYTE [esccnt],0         ;no more escape codes follow
        mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        mov     ch,dh
        mov     cl,0
        mov     dh,24
        mov     dl,79
        mov     ah,007h                 ;scroll page down function
        mov     al,1                    ;lines to blank
        mov     bh,07h                  ;attribute for blank
        int     010h
        retn
;
lnedel: mov     BYTE [esccnt],0         ;no more escape codes follow
        mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        mov     ch,dh
        mov     cl,0
        mov     dh,24
        mov     dl,79
        mov     ah,006h                 ;scroll page up function
        mov     al,1                    ;lines to blank
        mov     bh,07h                  ;attribute for blank
        int     010h
        retn
;
clrscr: mov     BYTE [esccnt],0         ;no more escape codes follow
        mov     ah,005h                 ;select active page for func #6, #7
        mov     al,0                    ;page #0
        int     010h
        mov     cx,0                    ;top X and Y co-ordinates
        mov     dh,24                   ;bottom Y co-ordinate
        mov     dl,79                   ;bottom X co-ordinate
        mov     ah,006h                 ;scroll page up function
        mov     al,0                    ;clear entire display
        mov     bh,07h                  ;attribute for blank
        int     010h                    ;fall through to home cursor
;
hmecur: mov     dx,0000h                ;set cursor to 0, 0
cursor: mov     ah,002h
        mov     bh,000h
        int     010h
        retn
;
vertab: mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        cmp     dh,0                    ;is cursor Y on 1st line ?
        jnz     moveup
        retn
moveup: dec     dh                      ;decrement cursor Y
        jmp     cursor                  ;and set cursor
;
hortab: mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        and     dl,7
        neg     dl
        add     dl,8
nxtspc: mov     ah,009h                 ;write char and attribute function
        mov     al,' '
        mov     bh,000h                 ;page #0
        mov     bl,[atrbyt]             ;attribute value
        mov     cx,1                    ;write 1 character
        int     010h
        mov     ah,00eh                 ;write TTY function
        mov     al,' '
        mov     bh,000h                 ;page #0
        int     010h
        dec     dl                      ;decrement tab counter
        jnz     nxtspc
        retn
;
curfwd: mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        cmp     dl,79
        jnc     newlne
        inc     dl                      ;increment cursor X
        jmp     cursor                  ;and set cursor
newlne: mov     dl,0                    ;set cursor X to zero
        mov     ah,002h
        mov     bh,000h
        int     010h
        mov     cl,00ah                 ;line feed character
        jmp     crt1op                  ;print it
;
esccnt  db      0
escchr  db      0
escdta  db      0
atrbyt  db      00000111b
atrflg  db      00000000b
;
ctltbl  dw      ctl___, ctl___, ctl___, ctl___, ctl___, ctl___, ctl___, print1
        dw      print1, hortab, print1, vertab, curfwd, print1, ctl___, ctl___
        dw      ctl___, rvseof, ctl___, ctl___, ctl___, ctl___, ctl___, ctl___
        dw      ctl___, rvseon, clrscr, setesc, ctl___, ctl___, hmecur, ctl___
;
esctbl  dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      lowint, hghint, clrscr, clrscr, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, setcur, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, lneins, esc___, esc___
        dw      esc___, esc___, rvseon, rvseof, undron, undrof, esc___, esc___
        dw      esc___, blnkof, lnedel, esc___, clreol, esc___, esc___, esc___
        dw      esc___, clreos, esc___, esc___, esc___, esc___, blnkon, esc___
        dw      esc___, esc___, esc___, esc___, esc___, lneins, esc___, esc___
        dw      esc___, esc___, rvseon, rvseof, undron, undrof, esc___, esc___
        dw      esc___, blnkof, lnedel, esc___, clreol, esc___, esc___, esc___
        dw      esc___, clreos, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
        dw      esc___, esc___, esc___, esc___, esc___, esc___, esc___, esc___
;
        end
