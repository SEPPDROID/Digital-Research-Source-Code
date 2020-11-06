;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*               Hardware interface support for Z80 system code                *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
; HDD image, Floppy, Keyboard, Serial, Centronic, Clock, Memory management
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 16 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Removed one occurence of label NEAR from: 'ctlalt: lea cx,[NEAR ReqAct]'
; - Changed 'lea' instructions to  'mov'.
; - Changed 'call dword [oldptr]' to 'call far [oldptr]'
; - Changed all uses of 'Z80' map segment to 'es:Z80'
; - Added 'align=16' to all SEGMENT declarations. (nasm def is align=1)
; - Changed serial functions to have a buffered byte for each com port and
;   added code to check com port number is in range and exists. 
; - Changed serial initilisation function to save values and only initialise
;   the serial port on first use. (prevents window warnings under NT5)
; - Changed Floppy read/write to use a local buffer when copying to/from the
;   requested DMA address. This prevents a 'DMA access across a 64K boundary
;   error' from occurring. (Error was seen under a cmd prompt on NT5)
; - Changed 'jp cpycm1' to 'jmp cpycm1' (jp = jump parity !)
;
; v1.00 - 9 June 2003 S.J.Kay
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

GLOBAL	kbd1in, kbd1st, kbd1ip, kbd2in, kbd2st, kbd2ip
GLOBAL	lptini, lptsta, lptout
GLOBAL	comini, comist, cominp, comost, comout
GLOBAL	gettme, settme, getdte, setdte
GLOBAL	rdflop, wrflop, rdhard, wrhard, GtHard, FlHard
GLOBAL	bnkuse, bnksel, bnkmve, bnkdta, bnkdma

GLOBAL	bnkini, bnkres, cpycom
GLOBAL	newkbd, oldkbd

GLOBAL	HrdFle, Handle
GLOBAL	Z80seg, Z80dma
GLOBAL	sveoff, sveseg, sveamt, reqcpy

; declared in WINDOW.ASM
EXTERN  OpnWnd, ClsWnd, ReqAct, GetAct
EXTERN  PrnStr, PrnByt, PrnHex

; declared in TABLE.ASM
EXTERN  T0

%include "macros.asm"

;===============================================================================
; Banked memory routines: Bank selection, Memory moves, etc
; bnkuse:-  Pass: BC(CX)=sets banking, size of bank #0, #1
; bnksel:-  Pass: A(AL)=bank 0 or 1
; bnkmve:-  Pass: HL(BX)=destination address
;                 DE(DX)=source address
;                 BC(CX)=amount to be moved
; bnkdta:-  Pass: B(CH)=destination bank
;                 C(CL)=source bank
; bnkdma:-  Pass: A(AL)=DMA bank 0 or 1
;===============================================================================
bnkini: mov     es,cx                   ;set Z80 map segment register
        mov     BYTE [bnksys],0         ;set banked system not used
        mov     BYTE [cdebnk],0         ;selected Z80 code bank
        mov     BYTE [mveflg],0
        mov     [Z80bk0],cx             ;bank #0 segment address
        mov     [Z80bk1],ax             ;bank #1 segment address
        mov     [Z80seg],cx             ;current Z80 bank segment address
        mov     [Z80dma],cx             ;current DMA for disk io xfers
        mov     WORD [memsze],0c000h    ;default bank size (48K)
        retn

bnkres: mov     cx,[Z80bk0]
        mov     ax,[Z80bk1]
        jmp     bnkini

bnkuse: mov     BYTE [bnksys],0ffh      ;set up banked system
        mov     [memsze],cx             ;set size of bank #0, #1
        retn

bnksel: cmp     BYTE [bnksys],0         ;banked system ?
        jz      bnkext
        and     al,00000001b
        cmp     al,[cdebnk]             ;same bank ?
        jz      bnkext                  ;do nothing if current bank
        mov     [cdebnk],al             ;save new bank
        or      al,al                   ;select bank #0 or #1
        mov     ax,[Z80bk1]
        jnz     setbnk                  ;jump if bank #1
        mov     ax,[Z80bk0]
setbnk: mov     dx,[Z80seg]             ;get segment to use as source
        mov     [Z80seg],ax             ;save new bank segment
        mov     es,ax                   ;set Z80 bank segment address
        mov     si,[memsze]             ;bank #0, #1 size (common mem base)
        mov     di,si                   ;destination offset address
        mov     cx,0000h
        sub     cx,si                   ;calculate amount to move
        cld                             ;do move forward
        push    ds
        mov     ds,dx                   ;source segment
rep     movsb
        pop     ds
bnkext: retn

bnkmve: cmp     BYTE [mveflg],0         ;bank to bank move ?
        jnz     bankmv
        mov     [dstseg],es             ;moves memory in current bank only
        mov     [sorseg],es
bankmv: mov     BYTE [mveflg],0
        push    ds                      ;moves memory bank to bank
        push    es
        mov     si,dx                   ;source offset address
        mov     di,bx                   ;destination offset address
        mov     es,[dstseg]             ;destination segment
        mov     [sveseg],es             ;save destination segment
        mov     [sveoff],bx             ;save destination address
        mov     [sveamt],cx             ;save amount to move
        mov     ds,[sorseg]             ;source segment
        cld                             ;do move forward
rep     movsb
        pop     es
        pop     ds
        mov     BYTE [reqcpy],0ffh
        call    cpycom                  ;copy common to common
        retn

bnkdta: mov     BYTE [mveflg],0ffh
        cmp     ch,0                    ;is destination bank #0 ?
        mov     di,[Z80bk0]             ;get bank #0 segment
        jz      setdst                  ;jump if bank #0 segment
        mov     di,[Z80bk1]             ;get bank #1 segment
setdst: mov     [dstseg],di             ;set destination segment
        cmp     cl,0                    ;is source bank #0 ?
        mov     si,[Z80bk0]             ;get bank #0 segment
        jz      setsor                  ;jump if bank #0 segment
        mov     si,[Z80bk1]             ;get bank #1 segment
setsor: mov     [sorseg],si             ;set source segment
        retn

bnkdma: or      al,al                   ;bank #0 or #1 ?
        mov     ax,[Z80bk0]             ;bank #0 segment
        jz      setdma
        mov     ax,[Z80bk1]             ;bank #1 segment
setdma: mov     [Z80dma],ax             ;set DMA segment address
        retn

cpycom: pushf
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        cmp     BYTE [bnksys],0
        jz      nomove                  ;skip if not banked system
        cmp     BYTE [reqcpy],0         ;is a copy to common required
        jz      nomove
        mov     bx,[sveoff]             ;get save offset
        mov     cx,[sveamt]             ;get saved amount
        mov     dx,[memsze]             ;banked memory size (common base)
        cmp     bx,dx                   ;is offset < common base address
        jc      cpycm0                  ;jump if it is
        mov     [mveoff],bx             ;use the same offset
        mov     [mveamt],cx             ;use the same amount
        jmp     cpycm1                  ;work out segment then move
cpycm0: add     bx,cx                   ;sveoff + sveamt
        cmp     bx,dx                   ;overlap into common memory ?
        jc      nomove                  ;no move required if not in common
        sub     bx,dx                   ;calculate the overlap amount
        mov     [mveamt],bx             ;amount to moved
        mov     [mveoff],dx             ;move memory from base of common
cpycm1: mov     dx,[Z80bk0]             ;bank #0 segment
        cmp     dx,[sveseg]             ;was data put into bank #0
        jnz     cpymem                  ;use bank #0 if different segment
        mov     dx,[Z80bk1]             ;else use bank #1 segment
cpymem: mov     si,[mveoff]             ;source offset
        mov     di,si                   ;destination offset = source
        mov     cx,[mveamt]             ;get amount to be moved
        push    ds
        push    es
        mov     es,dx                   ;destination segment
        mov     ds,[sveseg]             ;source segment
        cld                             ;do move forward
rep     movsb
        pop     es
        pop     ds
nomove: mov     BYTE [reqcpy],0         ;reset copy required flag
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        popf
        retn

bnksys  db      0               ;bank system use if <> 0
cdebnk  db      0               ;currently selected bank number
mveflg  db      0               ;move bank to bank if <> 0
reqcpy  db      0               ;copy to common needed if <> 0

Z80bk0  resw	1               ;segment for bank #0
Z80bk1  resw	1               ;segment for bank #1
Z80dma  resw	1               ;segment for DMA use
Z80seg  resw	1               ;current bank segment address

memsze  dw      0               ;size of bank #0, bank #1
sorseg  resw	1               ;source segment for memory move
dstseg  resw	1               ;destination segment for memory move

mveoff  resw	1
mveamt  resw	1
sveoff  resw	1               ;save destination address
sveseg  resw	1               ;save destination segment
sveamt  resw	1               ;save amount to move

;===============================================================================
; Line printer functions: Initialize, Status, Output
; lptini:-    Pass: DE(DX)=LPT port (0-3)
; lptsta:-    Pass: DE(DX)=LPT port (0-3)
;           Return: A(AL)=output status (not ready=0)
; lptout:-    Pass: DE(DX)=LPT port (0-3)
;                   C(CL)=output character
;===============================================================================
lptini: retn

lptsta: mov     ah,002h
        int     017h
        mov     al,0ffh
        and     ah,10000000b
        jnz     lptrdy
        xor     al,al
lptrdy: retn

lptout: mov     ah,002h
        int     017h
        and     ah,10000000b
        jz      lptout
        mov     ah,000h
        mov     al,cl
        int     017h
        retn

;===============================================================================
; KBD #1 Keyboard functions: Initialize, Status, Input
; kbd1in:-
; kbd1st:-  Return: A(AL)=Keyboard status (not ready=0)
; kbd1ip:-  Return: A(AL)=Keyboard character
;===============================================================================
kbd1in: retn

kbd1st: cmp     BYTE [kb1sta],0ffh      ;key in local buffer ?
        jz      kb1rdy
        mov     al,[kb1cnt]             ;number of times status checked
        inc     al
        and     al,00000011b            ;check for keys every 4 loops
        mov     [kb1cnt],al             ;save the count
        jnz     kbd1no                  ;if not 0 then not ready yet
        mov     ah,001h                 ;get keyboard status function
        int     016h
        jnz     kb1get
kbd1no: xor     al,al                   ;set to no key pending
        retn
kb1get: mov     ah,000h                 ;read key from keyboard function
        int     016h
        mov     [kb1buf],al             ;save the key in local buffer
kb1rdy: or      al,0ffh                 ;key ready status
        mov     [kb1sta],al             ;set key pending to true
        retn

kbd1ip: call    kbd1st                  ;check status else scan for key
        jz      kbd1ip
        mov     BYTE [kb1sta],0         ;set key pending to false
        mov     al,[kb1buf]             ;get key from local buffer
        retn

newkbd: push    es
        xor     ax,ax
        mov     es,ax
        mov     ax,word [es:009h*4+0]
        mov     WORD [oldptr+0],ax
        mov     ax,word [es:009h*4+2]
        mov     WORD [oldptr+2],ax
        mov     ax,kbintr
        cli
        mov     word [es:009h*4+0],ax
        mov     ax,cs
        mov     word [es:009h*4+2],ax
        sti
        pop     es
        retn

oldkbd: push    es
        xor     ax,ax
        mov     es,ax
        cli
        mov     ax,[oldptr+0]
        mov     word [es:009h*4+0],ax
        mov     ax,[oldptr+2]
        mov     word [es:009h*4+2],ax
        sti
        pop     es
        retn

kbintr:
        push    ax
        push    bx
        push    cx
        push    dx
        push    ds
        push    es
        mov     ax,cs
        mov     ds,ax
        in      al,60h                  ;read the keyboard scan code
        push    ax                      ;save the scan code
        pushf                           ;simulate an INT call
        call    far [oldptr]
        mov     ax,0040h                ;BIOS data segment
        mov     es,ax
        mov     al,byte [es:0017h]
        and     al,00001100b
        cmp     al,00001100b            ;is Ctrl and Alt keys down ?
        pop     ax                      ;restore the scan code
        jnz     notreq
        cmp     al,17h                  ;is scan code for key 'I' ?
        jnz     notreq
ctlalt: mov     cx,ReqAct
        mov     bx,0
setreq: mov     WORD [T0+bx],cx
        inc     bx
        inc     bx
        cmp     bx,512
        jc      setreq
notreq: pop     es
        pop     ds
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        iret

oldptr  dw      0,0
kb1sta  db      0
kb1buf  db      0
kb1cnt  db      0

;===============================================================================
; KBD #2 Keyboard functions (STDIN): Initialize, Status, Input
; kbd2in:-
; kbd2st:-  Return: A(AL)=STDIN status (not ready=0)
; kbd2ip:-  Return: A(AL)=STDIN character
;===============================================================================
kbd2in: mov     BYTE [kbdsta],0
        retn

kbd2st: mov     al,[kbdsta]
        or      al,al                   ;any key waiting to be read ?
        jz      kbdget
        retn
kbdget: mov     ah,006h                 ;direct console I/O function
        mov     dl,0ffh                 ;input function
        int     021h
        mov     [kbdkey],al             ;save key
        mov     al,0ffh
        jnz     kb2rdy
        xor     al,al
kb2rdy: mov     [kbdsta],al             ;save keyboard status
        retn                            ;status in Z80 reg A

kbd2ip: cmp     BYTE [kbdsta],0ffh      ;any key ready ?
        mov     BYTE [kbdsta],0
        jnz     kbd2
        mov     al,[kbdkey]
        retn                            ;keyboard key in Z80 reg A
kbd2:   mov     ah,006h                 ;direct console I/O function
        mov     dl,0ffh                 ;input function
        int     021h
        jz      kbd2
        retn

kbdsta  db      0
kbdkey  db      0

;===============================================================================
; Serial functions: Initialize, Status (I/O), Input, Output
; comini:-    Pass: DE(DX)=Coms port (0-3)
;                   A(AL)=baud/parity/data bits/stop bits
; comist:-    Pass: DE(DX)=Coms port (0-3)
;           Return: input status in reg A(AL)
; cominp:-    Pass: DE(DX)=Coms port (0-3)
;           Return: input character in reg A(AL)
; comost:-    Pass: DE(DX)=Coms port (0-3)
;           Return: output status in reg A(AL)
; comout:-    Pass: DE(DX)=Coms port (0-3), C(CL)=output character
;===============================================================================
comini: cmp     dx,4                    ;is com port in range ?
        jnc     comine
        mov     di,dx
        mov     BYTE [comsta+di],80h    ;set to 80h to indicate an init required
        mov     [comval+di],al          ;save the init values for later
        retn
comine  xor     ax,ax
        retn

comist: call    comchk                  ;check if com port exists, get index
        cmp     BYTE [comsta+di],0ffh   ;byte in local buffer ?
        jz      comrdy
        mov     ah,003h                 ;serial status (input) function
        int     014h
        and     ah,00000001b
        jnz     cminpt
        xor     al,al                   ;set to no byte pending
        retn
cminpt: mov     ah,002h                 ;read byte from serial function
        int     014h
        mov     [combuf+di],al          ;save the byte in local buffer
comrdy: or      al,0ffh                 ;byte ready status
        mov     [comsta+di],al          ;set byte pending to true
        retn

cominp: call    comchk                  ;check if com port exists, get index
        call    comist
        jz      cominp
        mov     BYTE [comsta+di],0      ;set byte pending to false
        mov     al,[combuf+di]          ;get byte from local buffer
        retn

comost: call    comchk                  ;check if com port exists, get index
        mov     ah,003h                 ;serial status (output) function
        int     014h
        xor     al,al                   ;reg A=0 output not ready
        and     ah,01100000b
        cmp     ah,01100000b
        jnz     cost
        dec     al                      ;reg A=FF output ready
cost:   retn

comout: call    comchk                  ;check if com port exists, get index
        call    comost                  ;serial status (output) function
        jz      comout
        mov     ah,001h                 ;serial output function
        mov     al,cl                   ;character to send in reg C
        int     014h
        retn

; check the com port DX is [0-3] and has a value in the BIOS data area
comchk: cmp     dx,4                    ;is com port in range ?
        jnc     comabt
        mov     si,dx
        shl     si,1                    ;si = si * 2
        push    ax
        push    es
        mov     ax,0040h                ;BIOS data area segment
        mov     es,ax
        cmp     WORD [es:0000h+si],0    ;have a base IO address entry ?
        pop     es
        pop     ax
        jz      comabt
        mov     di,dx
        cmp     BYTE [comsta+di],080h   ;init required before use ?
        jnz     comch0
        mov     BYTE [comsta+di],0      ;set byte pending to false
        push    ax
        mov     ah,000h                 ;initialize serial port function
        mov     al,[comval+di]          ;get the init values
        int     014h                    ;baud and other data in reg A
        pop     ax
comch0: retn                            ;return with the port index
comabt: pop     ax                      ;pop the return address of the stack
        xor     al,al                   ;return 0 for no com port
        retn                            ;exits the original call

comsta  db      0, 0, 0, 0
combuf  db      0, 0, 0, 0
comval  db      0, 0, 0, 0

;===============================================================================
; Time/Date functions: Get time, Set time, Get date, Set date
; gettme:-    Pass: reg HL(BX) = address to return results
;           Return: mem[HL(BX)+0] = hours in BCD
;                   mem[HL(BX)+1] = minutes in BCD
;                   mem[HL(BX)+2] = seconds in BCD
; settme:-    Pass: reg HL(BX) = address of parameters
;             Pass: mem[HL(BX)+0] = hours in BCD
;                   mem[HL(BX)+1] = minutes in BCD
;                   mem[HL(BX)+2] = seconds in BCD
; getdte:-    Pass: reg HL(BX) = address to return result
;           Return: mem[HL(BX)+0] = days since 31st December 1977
; setdte:-    Pass: reg HL(BX) = address of parameter
;           Return: mem[HL(BX)+0] = days since 31st December 1977
;===============================================================================
gettme: mov     ah,02ch                 ;get time function
        int     021h
        mov     dl,10                   ;ten's divider
        mov     al,cl                   ;minutes
        mov     cl,4                    ;used for 4 bit shift operations
        xor     ah,ah
        div     dl
        shl     al,cl
        add     al,ah
        mov     BYTE [es:Z80+BX+1],al   ;store minutes in BCD form
        mov     al,ch                   ;hours
        xor     ah,ah
        div     dl
        shl     al,cl
        add     al,ah
        mov     BYTE [es:Z80+BX],al     ;store hours in BCD form
        mov     al,dh                   ;seconds
        xor     ah,ah
        div     dl
        shl     al,cl
        add     al,ah
        mov     BYTE [es:Z80+BX+2],al   ;store seconds in BCD form
        retn

settme: mov     dl,10                   ;ten's multiplyer
        mov     cl,4                    ;used for 4 bit shift operations
        mov     al,[es:Z80+bx]          ;hours in 24 Hrs BCD form
        mov     ch,al
        and     ch,00001111b
        shr     al,cl
        mul     dl
        add     al,ch
        push    ax                      ;save hours
        mov     al,[es:Z80+bx+1]        ;minutes in BCD form
        mov     ch,al
        and     ch,00001111b
        shr     al,cl
        mul     dl
        add     al,ch
        push    ax                      ;save minutes
        mov     al,[es:Z80+bx+2]        ;seconds in BCD form
        mov     ch,al
        and     ch,00001111b
        shr     al,cl
        mul     dl
        add     al,ch
        xor     dl,dl                   ;100ths of seconds
        mov     dh,al                   ;seconds
        pop     ax
        mov     cl,al                   ;minutes
        pop     ax
        mov     ch,al                   ;hours
        mov     ah,02dh                 ;set time function
        int     021h
        retn

; convert day/month/year to days since 31st December 1977
getdte: mov     ah,02ah                 ;get date function
        int     021h
        push    dx                      ;save month in reg DH
        cmp     cx,1978
        jnc     y1978
        add     cx,100
y1978:  mov     ax,1978                 ;start with year 1978
        xor     dh,dh                   ;day counter (days in reg DL)
adyear: cmp     ax,cx                   ;at present year ?
        jz      admnth
        add     dx,366                  ;add days in a leap year
        test    al,00000011b            ;was it a leap year ?
        jz      Lyear0
        dec     dx                      ;adjust day counter for normal year
Lyear0: inc     ax
        jmp     adyear
admnth: mov     cx,dx                   ;days counted to reg CX
        pop     dx                      ;restore month in reg DH
        mov     ah,29
        test    al,00000011b
        jz      Lyear1
        dec     ah
Lyear1: mov     BYTE [Months+1],ah      ;set February for year type
        xor     si,si
        xor     ah,ah
mnthlp: dec     dh
        jz      gdate
        mov     al,[Months+si]          ;add days in the months passed
        add     cx,ax
        inc     si
        jmp     mnthlp
gdate:  mov     word [es:Z80+bx],cx
        retn

; convert days since 31st December 1977 to day/month/year
setdte: mov     ax,word [es:Z80+bx]     ;days since 31st December 1977
        mov     cx,1978                 ;year counter 1978-2077
        mov     bx,365                  ;days in a non leap year
nxyear: sub     ax,bx                   ;minus 1 year (365 days)
        jc      gtyear
        inc     cx
        sub     ax,bx                   ;subtract 365 days
        jc      gtyear
        inc     bx                      ;make a leap year (366 days)
        inc     cx
        sub     ax,bx                   ;subtract 366 days
        jc      gtyear
        dec     bx                      ;make a normal year (365 days)
        inc     cx
        sub     ax,bx                   ;minus 365 days
        jc      gtyear
        inc     cx
        jmp     nxyear
gtyear: add     ax,bx                   ;correct number of days
        xor     bl,0ffh                 ;bit 0 now set if 366 days in reg BX
        and     bl,1
        add     bl,28
        mov     BYTE [Months+1],bl
        xor     dh,dh                   ;month counter
        xor     si,si
getmth: mov     bl,[Months+si]          ;get days in month
        xor     bh,bh
        inc     si
        inc     dh
        sub     ax,bx
        jnc     getmth
        add     ax,bx
        mov     dl,al                   ;date in reg DL
        mov     ah,02bh                 ;set date function
        int     021h
        retn

Months  db      31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31

;===============================================================================
; HDD file disk functions: read sector, write sector
; rdhard, wrhard:-
;   Pass: B(CH)=track number
;         C(CL)=sector number
;         E(DL)=drive code (2=drive C, 3=drive D etc)
;         HL(BX)=buffer address
; Return: A(AL)=0 if no errors, 1 if error
;===============================================================================
rdhard: mov     BYTE [HrdFnc],03fh      ;read from HDD file function
        mov     WORD [sveamt],512
        mov     BYTE [reqcpy],0ffh      ;copy to common flag
        push    ax
        mov     ax,[Z80dma]
        mov     [sveseg],ax             ;save segment address
        mov     [sveoff],bx             ;save buffer address
        pop     ax
        jmp     rwhard

wrhard: mov     BYTE [HrdFnc],040h      ;write to HDD file function

rwhard: mov     al,1                    ;error if no HDD file exists
        cmp     BYTE [HrdFle],0ffh      ;HDD file opended ?
        jnz     hddfnd
        mov     BYTE [reqcpy],0         ;copy to common flag = false
        retn
hddfnd: push    bx                      ;save buffer address
        mov     bh,cl                   ;sector number SHL 8
        dec     bh
        shl     bh,1
        xor     bl,bl                   ;BX = sector offset
        mov     cl,ch                   ;get track number
        xor     ch,ch                   ;track value is (0..255)
        xor     dx,dx
        shr     cx,1                    ;CX = MSW offset
        rcr     dx,1                    ;DX = LSW offset
        add     dx,bx
        adc     cx,0
        mov     ah,042h                 ;move file pointer function
        mov     al,000h                 ;move file pointer method
        mov     bx,[Handle]             ;file handle
        int     021h                    ;move file pointer
        mov     ah,[HrdFnc]
        pop     dx                      ;buffer address (DS:DX)
        mov     bx,[Handle]             ;file handle
        mov     cx,512                  ;number of bytes to read/write
        push    es                      ;save current data segment
        push    ds
        mov     ds,[Z80dma]             ;get DMA segment address
        int     021h                    ;read/write to HDD file
        pop     ds
        pop     es                      ;restore data segment
        call    cpycom                  ;copy common if needed
        mov     al,0
        jc      hdderr
        retn
hdderr: inc     al
        retn

FlHard: cmp     BYTE [HrdFle],0ffh      ;is the HDD file open ?
        jnz     flush
        retn
flush:  mov     ah,068h                 ;commit file function (flush)
        mov     bx,[Handle]             ;file handle
        int     021h                    ;flush HDD file buffer
        retn

GtHard: mov     al,[HrdFle]             ;return HDD drive or FFh if none
        retn

HrdFle  db      0ffh                    ;save drive code for HDD file
Handle  resw	1                       ;handle for HDD file image
HrdFnc  resb	1

;===============================================================================
; Floppy disk functions: read sector, write sector
; rdflop, wrflop:-
;   Pass: A(AL)=sector size code (0=128, 1=256, 2=512, 3=1024)
;         B(CH)=track number
;         C(CL)=sector number
;         D(DH)=side select
;         E(DL)=drive code (0=drive A, 1=drive B)
;         HL(BX)=buffer address
; Return: A(AL)=0 if no errors, 1 if error
;===============================================================================
rdflop: mov     BYTE [reqcpy],0ffh      ;copy to common flag
        mov     di,DskMsB
        mov     BYTE [FlpFnc],002h      ;read floppy disk function
        jmp     rwflop

wrflop: mov     di,DskMsC
        mov     BYTE [FlpFnc],003h      ;write floppy disk function

rwflop: push    ax
        push    cx
        mov     cl,al                   ;0=128, 1=256, 2=512, 3=1024 b/s
        mov     ax,128
        shl     ax,cl                   ;calculate size of sector
        mov     [sveamt],ax
        mov     ax,[Z80dma]
        mov     [sveseg],ax             ;save segment address
        mov     [sveoff],bx             ;save buffer address
        pop     cx
        pop     ax

        push    di
        push    es                      ;save Z80 segment
        mov     si,0000h
        mov     es,si
        mov     si,word [es:0078h]
        mov     [DskOff],si
        mov     di,word [es:007ah]
        mov     [DskSeg],di
        mov     es,di
        mov     di,word [es:si+3]       ;save sector code / last sector
        mov     [DskDt0],di
        mov     di,word [es:si+5]       ;save gap for read/write sectors
        mov     [DskDt1],di
        mov     byte [es:si+3],al       ;new sector size code
        mov     byte [es:si+4],cl       ;make last sector this sector
;        mov     byte ptr es:[si+5],10   ;new gap for read/write
        pop     es                      ;restore Z80 segment
        pop     di
rwtrys: mov     ah,2                    ;allow 2 trys
        mov     [DskTrk],ch
        mov     [DskSec],cl
        mov     [DskSde],dh

; loop back here for a retry
flpret: push    ax                      ;save retry counter
        mov     ah,[FlpFnc]             ;read/write disk function
        mov     al,1                    ;sector count
        and     dl,00000011b            ;make sure hard disk not selected
        push    es                      ;save Z80 segment

; use a fixed buffer location to avoid DMA across 64K boundary problems
        cmp     ah,002h                 ;read floppy disk function ?
        jz      flprd

; copy Z80dma location to a local buffer then write to floppy
        push    ax
        push    cx                      ;save track, sector
        push    si
        push    di                      ;save disk message
        mov     cx,[sveamt]             ;amount to be copied
        mov     si,[sveoff]             ;the original Z80 offset
        mov     ax,seg buffer
        mov     di,buffer
        mov     es,ax
        push    ds
        mov     ds,[Z80dma]             ;the original Z80 segment (source)
        cld                             ;do move forward
rep     movsb
        pop     ds
        pop     di                      ;get back disk message
        pop     si
        pop     cx                      ;get back track, sector
        pop     ax

        push    bx
        push    ax
        mov     ax,seg buffer           ;use a local buffer
        mov	es,ax
        mov     bx,buffer
        pop     ax                      ;get back requested sector count
        int     013h                    ;read/write sector
        pop     bx
        jmp     flptst

; floppy read to a local buffer then copy to a Z80dma location
flprd:  push    bx
        push    ax
        mov     ax,seg buffer           ;use a local buffer
        mov	es,ax
        mov     bx,buffer
        pop     ax                      ;get back requested sector count
        int     013h                    ;read/write sector
        pop     bx
        pushf                           ;save the result flag

        push    ax
        push    cx                      ;save track, sector
        push    si
        push    di                      ;save disk message
        mov     cx,[sveamt]             ;amount to be copied
        mov     es,[Z80dma]             ;the original Z80 segment (destination)
        mov     di,[sveoff]             ;the original Z80 offset
        mov     ax,seg buffer
        mov     si,buffer
        push    ds
        mov     ds,ax
        cld                             ;do move forward
rep     movsb
        pop     ds
        pop     di                      ;get back disk message
        pop     si
        pop     cx                      ;get back track, sector
        pop     ax
        popf

; test for floppy read/write errors
flptst: pop     es                      ;restore Z80 segment
        jnc     flpok
        mov     [ErrCde],ah
        mov     ah,000h                 ;reset disk controller function
        int     013h                    ;reset disk controller
        pop     ax
        dec     ah
        jnz     flpret
        call    DskErr
        jc      rwtrys
        jmp     flpext                  ;return error in Z80 reg A

; exit floppy read/write restoring original floppy values
flpok:  pop     ax
        xor     al,al                   ;return no error in Z80 reg A
flpext: push    es                      ;save Z80 segment
        mov     es,[DskSeg]             ;restore disk table values
        mov     si,[DskOff]
        mov     di,[DskDt0]
        mov     word [es:si+3],di       ;restore, sector code / last sector
        mov     di,[DskDt1]
        mov     word [es:si+5],di       ;restore, gap for read/write
        pop     es                      ;restore Z80 segment
        call    cpycom                  ;copy common if needed
        retn

; display disk error message and get requested action
DskErr: push    bx
        push    cx
        push    dx
        add     dl,'A'
        mov     [DrvLet],dl
        mov     dh,8                    ;window Y start
        mov     dl,17                   ;window X start
        mov     ch,16                   ;window Y finish
        mov     cl,62                   ;window X finish
        call    NEAR OpnWnd
        mov     si,DskMsA
        call    NEAR PrnStr
        mov     si,di
        call    NEAR PrnStr
        mov     si,DskMsD
        call    NEAR PrnStr
        mov     bl,[ErrCde]
        cmp     bl,011h
        jc      ShwEr0
        mov     si,DskM20
        cmp     bl,020h
        jz      ShwEr1
        mov     si,DskM40
        cmp     bl,040h
        jz      ShwEr1
        mov     si,DskM80
        cmp     bl,080h
        jz      ShwEr1
        mov     si,DskMXX
        jmp     ShwEr1
ShwEr0: xor     bh,bh
        dec     bl
        add     bx,bx
        mov     si,[MsgTbl+bx]
ShwEr1: call    NEAR PrnStr
        cmp     si,DskMXX
        jnz     known
        mov     al,[ErrCde]
        call    NEAR PrnHex
        mov     si,DskMZZ
        call    NEAR PrnStr
known:  mov     si,DskMsE
        call    NEAR PrnStr
        mov     al,[DskTrk]
        call    NEAR PrnByt
        mov     si,DskMsF
        call    NEAR PrnStr
        mov     al,[DskSde]
        call    NEAR PrnByt
        mov     si,DskMsG
        call    NEAR PrnStr
        mov     al,[DskSec]
        call    NEAR PrnByt
        mov     si,DskMsH
        call    NEAR PrnStr
        mov     si,KeyRIF
        call    NEAR GetAct
        cmp     al,'R'
        stc                             ;set carry to retry
        jz      ExtErr
        xor     ah,ah                   ;return no error for Ignore
        cmp     al,'I'
        clc                             ;clear carry for no retrys
        jz      ExtErr
        mov     ah,1                    ;return error code 1 for Fail
ExtErr: mov     al,ah
        call    NEAR ClsWnd
        pop     dx
        pop     cx
        pop     bx
        retn

KeyRIF  db      'RIF$'
DskSeg  resw	1
DskOff  resw	1
DskDt0  resw	1
DskDt1  resw	1
DskTrk  resb	1
DskSde  resb	1
DskSec  resb	1
FlpFnc  resb	1
ErrCde  resb	1

MsgTbl  dw      DskM01, DskM02, DskM03, DskM04, DskMXX, DskM06, DskMXX
        dw      DskM08, DskM09, DskMXX, DskMXX, DskM0C, DskMXX, DskMXX
        dw      DskMXX, DskM10, DskMXX

DskMsA  db      '           BIOS DISK $'
DskMsB  db      'READ$'
DskMsC  db      'WRITE$'
DskMsD  db      ' ERROR', 0dh, 0ah
        db      'Drive: '
DrvLet  db      ' ', 0dh, 0ah
        db      'Error: $'
DskMsE  db      'Track: $'
DskMsF  db      '  Side: $'
DskMsG  db      '  Sector: $'
DskMsH  db      0dh, 0ah, 0ah, '[R]etry  [I]gnore  [F]ail'
        db      0dh, 0ah, 'Action ?: $'

DskM01  db      'Bad command', 0dh, 0ah, '$'
DskM02  db      'Address mark not found', 0dh, 0ah, '$'
DskM03  db      'Disk is write protected', 0dh, 0ah, '$'
DskM04  db      'Sector not found', 0dh, 0ah, '$'
DskM06  db      'Diskette removed', 0dh, 0ah, '$'
DskM08  db      'DMA overrun', 0dh, 0ah, '$'
DskM09  db      'DMA across 64KB boundary', 0dh, 0ah, '$'
DskM0C  db      'Bad media type', 0dh, 0ah, '$'
DskM10  db      'Bad CRC', 0dh, 0ah, '$'
DskM20  db      'Controller failed', 0dh, 0ah, '$'
DskM40  db      'Seek failed', 0dh, 0ah, '$'
DskM80  db      'Drive not ready', 0dh, 0ah, '$'
DskMXX  db      'Error unknown ($'
DskMZZ  db      'H)', 0dh, 0ah, '$'

buffer  resb    1024

        end
