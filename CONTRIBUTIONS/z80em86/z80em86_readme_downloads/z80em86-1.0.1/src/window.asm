;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*             Window and Screen Output support for error messages             *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
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

GLOBAL	OpnWnd, ClsWnd
GLOBAL	PrnByt, PrnHex, PrnStr, PrnChr, EraScr, HdeCur
GLOBAL	GetAct, GetKey

; declared in MAIN.ASM
EXTERN  VidSeg, VidBuf, prmflg

%include "macros.asm"

;===============================================================================
; Window box
;   Pass: dh=window Y start
;         dl=window X start
;         ch=window Y finish
;         cl=window X finish
; Return: nothing
;===============================================================================
OpnWnd: pushf
        push    ax
        push    bx
        push    si
        push    di
        mov     al,[WndNmb]
        cmp     al,4
        jnz     setwnd
        jmp     wndful
setwnd: push    cx
        push    dx
        xor     ah,ah
        mov     di,ax
        shl     di,1
        mov     ah,003h                 ;read cursor position function
        mov     bh,000h                 ;page #0
        int     010h
        mov     WORD [SveCur+di],dx     ;save current cursor position
        mov     cx,4000
        mov     al,[WndNmb]
        xor     ah,ah
        mul     cx
        mov     di,ax
        xor     si,si
        push    ds
        push    es
        mov     es,[VidBuf]
        mov     ds,[VidSeg]
        cld
rep     movsb
        pop     es
        pop     ds
        pop     dx
        pop     cx
        inc     BYTE [WndNmb]
        call    WndBox
wndful: pop     di
        pop     si
        pop     bx
        pop     ax
        popf
        retn

;===============================================================================
; Window close
;   Pass: nothing
; Return: nothing
;===============================================================================
ClsWnd: pushf
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        mov     al,[WndNmb]
        or      al,al
        jnz     reswnd
        jmp     notwnd
reswnd: dec     al
        mov     [WndNmb],al
        xor     ah,ah
        mov     di,ax
        shl     di,1
        mov     dx,[SveCur+di]          ;get cursor position
        mov     ah,002h                 ;set cursor function
        mov     bh,000h                 ;page #0
        int     010h
        mov     cx,4000
        mov     al,[WndNmb]
        xor     ah,ah
        mul     cx
        mov     si,ax
        xor     di,di
        push    ds
        push    es
        mov     es,[VidSeg]
        mov     ds,[VidBuf]
        cld
rep     movsb
        pop     es
        pop     ds
notwnd: pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        popf
        retn

WndBox: pushf
        push    ax
        push    bx
        mov     [WndX0],dl
        mov     [WndY0],dh
        mov     [WndX1],cl
        mov     [WndY1],ch
        sub     ch,dh
        dec     ch
        mov     [BoxDep],ch
        sub     cl,dl
        dec     cl
        mov     [BoxWid],cl
        mov     ah,002h                 ;set cursor function
        mov     bh,000h                 ;page #0
        int     010h
        mov     BYTE [BoxCh0],201
        mov     BYTE [BoxCh1],205
        mov     BYTE [BoxCh2],187
        call    DrwBox
        mov     ch,[BoxDep]
        or      ch,ch
        jz      BoxBot
        mov     BYTE [BoxCh0],186
        mov     BYTE [BoxCh1],' '
        mov     BYTE [BoxCh2],186
BoxDwn: call    DrwBox
        dec     ch
        jnz     BoxDwn
BoxBot: mov     BYTE [BoxCh0],200
        mov     BYTE [BoxCh1],205
        mov     BYTE [BoxCh2],188
        call    DrwBox
        mov     dl,[WndX0]              ;top X position of window
        add     dl,2                    ;start text 2 positions in
        mov     dh,[WndY0]              ;top Y position of window
        inc     dh                      ;start text on next line
        mov     ah,002h                 ;set cursor function
        int     010h
        pop     bx
        pop     ax
        popf
        retn

DrwBox: push    cx
        mov     ah,009h                 ;write char and attribute function
        mov     bh,000h                 ;page #0
        mov     bl,070h                 ;attribute value
        mov     al,[BoxCh0]
        mov     cx,1                    ;write 1 character
        int     010h
        mov     ah,003h                 ;read cursor position function
        inc     dl
        mov     ah,002h                 ;set cursor function
        int     010h
        mov     ah,009h                 ;write char and attribute function
        mov     al,[BoxCh1]
        xor     ch,ch                   ;write ? characters
        mov     cl,[BoxWid]
        int     010h
        add     dl,cl
        mov     ah,002h                 ;set cursor function
        int     010h
        mov     ah,009h                 ;write char and attribute function
        mov     al,[BoxCh2]
        mov     cx,1                    ;write 1 character
        int     010h
        mov     dl,[WndX0]
        inc     dh                      ;draw window on next line
        mov     ah,002h                 ;set cursor function
        int     010h
        pop     cx
        retn

;===============================================================================
; Print string
;   Pass: 
; Return: nothing
;===============================================================================
PrnStr: pushf
        push    ax
        push    si
PrnLp:  mov     al,[si]
        inc     si
        cmp     al,'$'
        jz      PrnExt
        call    PrnChr
        jmp     PrnLp
PrnExt: pop     si
        pop     ax
        popf
        retn

;===============================================================================
; Print byte
;   Pass: 
; Return: nothing
;===============================================================================
PrnByt: pushf
        push    ax
        push    bx
        mov     bl,100
        xor     ah,ah
        div     bl
        or      al,al
        jz      PrnBy1
        add     al,'0'
        call    PrnChr                  ;print hundreds digit
PrnBy1: mov     al,ah                   ;remainder
        mov     bl,10
        xor     ah,ah
        div     bl
        or      al,al
        jz      PrnBy2
        add     al,'0'
        call    PrnChr                  ;print tens digits
PrnBy2: mov     al,ah                   ;remainder
        add     al,'0'
        call    PrnChr                  ;print units
        pop     bx
        pop     ax
        popf
        retn

;===============================================================================
; Print hex
;   Pass: 
; Return: nothing
;===============================================================================
PrnHex: pushf
        push    ax
        push    cx
        mov     ch,al
        mov     cl,4
        shr     al,cl
        add     al,'0'
        cmp     al,':'
        jc      Hex0
        add     al,007h
Hex0:   call    PrnChr
        mov     al,ch
        and     al,00fh
        add     al,'0'
        cmp     al,':'
        jc      Hex1
        add     al,007h
Hex1:   call    PrnChr
        pop     cx
        pop     ax
        popf
        retn

PrnChr: pushf
        push    ax
        push    bx
        push    cx
        push    dx
        mov     bh,000h                 ;page #0
        mov     dl,al                   ;save the character to display
        cmp     al,' '
        jc      PrnCh1
        mov     ah,009h                 ;write char and attribute function
        mov     bl,070h                 ;attribute value
        mov     cx,1                    ;write 1 character
        int     010h
PrnCh1: mov     al,dl                   ;restore the character to display
        cmp     al,00dh
        jz      PrnCh2
        cmp     al,00ah
        jz      PrnCh3
        mov     ah,00eh                 ;write TTY function
        jmp     PrnCh4
PrnCh2: mov     ah,003h                 ;read cursor position function
        int     010h
        mov     dl,[WndX0]
        add     dl,2
        mov     ah,002h                 ;set cursor function
        jmp     PrnCh4
PrnCh3: mov     ah,003h                 ;read cursor position function
        int     010h
        inc     dh
        mov     ah,002h                 ;set cursor function
PrnCh4: int     010h
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        popf
        retn

;===============================================================================
; Erase screen
;   Pass: 
; Return: nothing
;===============================================================================
EraScr: pushf
        cmp     BYTE [prmflg],0ffh
        jz      skpcls                  ;skip CLS if command tail
        push    ax
        push    bx
        push    cx
        mov     ah,005h                 ;select active page for func #6, #7
        xor     al,al                   ;page #0
        int     010h
        xor     cx,cx                   ;top X and Y co-ordinates
        mov     dh,24                   ;bottom Y co-ordinate
        mov     dl,79                   ;bottom X co-ordinate
        mov     ah,006h                 ;scroll page up function
        xor     al,al                   ;entire display cleared if 0
        mov     bh,007h                 ;attribute for blank
        int     010h                    ;fall through to home cursor
        xor     dx,dx                   ;set cursor to 0, 0
        mov     ah,002h
        xor     bh,bh                   ;page #0
        int     010h
        pop     cx
        pop     bx
        pop     ax
skpcls: popf
        retn

;===============================================================================
; Hide cursor
;   Pass: nothing
; Return: nothing
;===============================================================================
HdeCur: push    ax
        push    bx
        push    dx
        mov     dx,0ffffh               ;cursor position
        mov     ah,002h                 ;set cursor function
        mov     bh,000h                 ;page #0
        int     010h
        pop     dx
        pop     bx
        pop     ax
        retn

;===============================================================================
; Get user key action
;   Pass: nothing
; Return: AL=requested action
;===============================================================================
GetAct: call    GetKey                  ;return key in reg AL
        push    ax                      ;save key pressed
        call    PrnChr                  ;print user's selection
GetRet: call    keyinp
        cmp     al,008h
        jnz     notbck
        call    PrnChr
        mov     al,' '
        call    PrnChr
        mov     al,008h
        call    PrnChr
        pop     ax                      ;dump key
        jmp     GetAct                  ;retry again for another key
notbck: cmp     al,00dh                 ;ENTER key pressed ?
        jnz     GetRet
        pop     ax
        and     al,11011111b            ;convert to upper case
        retn

;===============================================================================
; Get user key action
;   Pass: 
; Return: al=key
;===============================================================================
GetKey: push    dx                      ;returns key in reg AL
nxttry: call    keyinp
        push    si
        mov     dl,al
        and     dl,11011111b            ;convert to upper case
nxtkey: cmp     byte[si],dl
        jz      gotkey
        inc     si
        cmp     byte[si],'$'
        jnz     nxtkey
        pop     si
        jmp     nxttry
gotkey: pop     si
        pop     dx
        retn

keyinp: mov     ah,001h                 ;get keyboard status function
        int     016h
        jz      empbuf
        mov     ah,000h                 ;read key from keybaud function
        int     016h
        jmp     keyinp                  ;empty keyboard buffer
empbuf: mov     ah,000h                 ;read key from keybaud function
        int     016h
        retn                            ;keyboard key in AL

SveCur  resw    4
WndNmb  db      0

WndX0   resb	1
WndY0   resb	1
WndX1   resb	1
WndY1   resb	1
BoxCh0  resb	1
BoxCh1  resb	1
BoxCh2  resb	1
BoxDep  resb	1
BoxWid  resb	1

        end
