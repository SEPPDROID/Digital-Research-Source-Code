;*******************************************************************************
;*                                  z80em86                                    *
;*           A Z80 CPU emulator coded in Intel 86 assembly language            *
;*                                                                             *
;*                           Main assembly module                              *
;*                                                                             *
;*                    Copyright (C) 1992-2009 Stewart Kay                      *
;*******************************************************************************
;
;===============================================================================
; ChangeLog (most recent entries are at top)
;===============================================================================
; v1.0.0 - 20 February 2009, S.J.Kay
; - Convert sources from TASM to NASM format and prepare for GPL release.
; - Removed one occurence of label NEAR from: 'lea dx,[NEAR criter]'
; - Changed 'lea' instructions to  'mov'.
; - Removed 'seg' from 'mov ax,seg stkseg' and 'mov bx,seg Zend'
; - Removed 'ss:' from 'mov bx,ss:topstk'
; - Added '..start:' before the 'Z80emu:' label.
; - Changed all uses of Z80 map segment to 'es:Z80'
; - Added 'align=16' to all SEGMENT declarations. (nasm def is align=1)
; - Changed 'SgnMsg' version to use APPVER passed from Makefile.
; - Changed command line drive options from A: and B: to now use -A and -B
;   and also allow Z80 system parameters to be flagged even if floppy
;   options are specified.
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

GLOBAL	Z80res, Z80ext, Z80opc

GLOBAL	BufSeg, VidSeg, VidBuf
GLOBAL	prmbuf, prmflg, vidoff

; declared in WINDOW.ASM
EXTERN  OpnWnd, ClsWnd
EXTERN  PrnStr, GetKey, EraScr, HdeCur

; declared in HARDWARE.ASM
EXTERN  bnkini, bnkres
EXTERN  newkbd, oldkbd
EXTERN  rdflop, rdhard
EXTERN  HrdFle, Handle

; declared in FUNCTION.ASM
EXTERN  criter, fboot,  SysErr
EXTERN  BufCnt, BootDr

; declared in INSTHAND.ASM
EXTERN  FlagX,  FlagN,  FlagNx, IFF
EXTERN  Z80I,   Z80R,   Z80SP,  Z80IX, Z80IY
EXTERN  Z80AF,  Z80BC,  Z80DE,  Z80HL

%include "macros.asm"

BLKMAX  equ     64              ;maximum number of 16 K-byte memory blocks
prmbuf  resb    80              ;buffer for command tail parameters
BufSeg  resw    BLKMAX          ;table of segments for 16 K-byte blocks
VidSeg  resw	1
VidBuf  resw	1
VidMde  resb	1
vidoff  db      0               ;disable output to video drivers flag
prmflg  db      0               ;command tail passed flag
flpdrv  db      0               ;floppy to boot up from, 0=A or 1=B
flpflg  db      0               ;boot from floppy flag
CtrlC   resb	1               ;saved state of Ctrl-C flag

; Z80 Emulator execution begins here
..start:
Z80emu: 
        mov     ax,stkseg
        mov     bx,topstk
        cli
        mov     ss,ax                   ;set stack segment register
        mov     sp,bx                   ;set stack pointer register
        sti
        mov     ax,cs
        mov     ds,ax                   ;set data segment=code segment

; Save command tail in Program Segment Prifix (PSP)
        push    ds
        push    es
        mov     si,0080h                ;source offset
        mov     di,prmbuf               ;destination offset buffer
        mov     cx,80                   ;amount to move
        mov     ax,es
        mov     ds,ax                   ;set source segment (PSP)
        mov     ax,cs
        mov     es,ax                   ;set destination segment
        cld                             ;move forward
rep     movsb
        pop     es
        pop     ds

; Check command tail in Program Segment Prifix (PSP)
        mov     si,0080h
        mov     cl,[es:si]              ;size of command tail (ES=PSP)
        mov     di,si                   ;copy of command tail offset
        xor     ch,ch
        add     di,cx                   ;point to last character

chkspc: cmp     byte [es:di],' '        ;trailing space character ?
        jnz     chkprm
        dec     cl
        jz      setvid
        dec     di                      ;work backwards
        jmp     chkspc

chkprm: or      cl,cl
        jz      setvid                  ;no tail remaining if count is 0
        dec     cl
        inc     si
        mov     al,[es:si]              ;get character from command tail
        cmp     al,' '                  ;ignore leading space characters
        jz      chkprm

        cmp     al,'-'                  ;leading hyphen character ?
        jnz     chkdir                  ;no, then check for redirection
        or      cl,cl
        jz      setvid                  ;error in option if end of tail
        inc     si
        mov     al,[es:si]
        and     al,11011111b            ;convert to uppercase
        cmp     al,'A'                  ;force boot from A: ?
        jz      flpopt
        cmp     al,'B'                  ;force boot from B: ?
        jnz     setvid                  ;error in floppy selection

flpopt: mov     [DrvChr],al             ;insert letter into message
        sub     al,'A'
        mov     [flpdrv],al             ;drive code of floppy to be booted
        mov     BYTE [flpflg],0ffh      ;boot from floppy flag
        dec     cl                      ;any more tail left ?
        jz      setvid
        inc     si

chkdir: cmp     BYTE [es:si],'<'        ;DOS input redirection ?
        jz      setvid
        cmp     BYTE [es:si],'>'        ;DOS output redirection ?
        jz      setvid
        mov     BYTE [prmflg],0ffh      ;set command tail flag true
        mov     BYTE [vidoff],0ffh      ;disable video output

; Determine and set video display mode
setvid: mov     ah,00fh                 ;get current video mode function
        int     010h
        mov     [VidMde],al             ;save video mode
        int     011h                    ;get equipment list interrupt
        and     al,00110000b            ;keep initial video mode bits
        mov     ah,007h                 ;monochrome 25 * 80 text display
        mov     WORD [VidSeg],0b000h    ;monochrome video segment
        or      al,al                   ;unused video system ?
        jz      MonVid
        cmp     al,00110000b            ;monochrome video system ?
        jz      MonVid
        mov     ah,002h                 ;colour 25 * 80 text display
        mov     WORD [VidSeg],0b800h    ;colour video segment
MonVid: cmp     BYTE [prmflg],0         ;do not set video if command tail
        jnz     seterr
        mov     al,ah
        mov     ah,000h                 ;set video mode function
        int     010h

; Setup local DOS critical error handler
seterr: mov     ah,025h                 ;set interrupt function
        mov     al,024h                 ;critical error handler vector
        mov     dx,criter
        int     21h                     ;change vector via DOS

; Setup keyboard intercept and Ctrl C flag
        mov     ah,033h                 ;get/set Ctrl-C flag function
        mov     al,000h                 ;get function
        int     021h
        mov     [CtrlC],al              ;save flag for later
        mov     ah,033h                 ;get/set Ctrl-C flag function
        mov     al,001h                 ;set function
        xor     dl,dl                   ;turn of Ctrl-C action
        int     021h
        call    NEAR newkbd

; Resize memory for Z80 Emulator program
        mov     ax,es                   ;get PSP address in ES
        mov     bx,Zend
        sub     bx,ax                   ;program size in BX
        mov     ah,04ah                 ;modify allocated memory function
        push    ds
        int     021h
        pop     ds
        jnc     Z80mp0
        jmp     ErrAlo

; Allocate memory for Z80 address map (2 maps of 64k)
Z80mp0: mov     ah,048h                 ;allocate memory function
        mov     bx,4096                 ;64K-bytes for Z80 bank 0
        int     021h
        mov     cx,ax                   ;save bank #0 segment address
        jnc     Z80mp1
        jmp     ErrAlo
Z80mp1: mov     ah,048h                 ;allocate memory function
        mov     bx,4096                 ;64K-bytes for Z80 bank 1
        int     021h
        call    NEAR bnkini             ;initialize variables
        jnc     AloWnd
        jmp     ErrAlo

; Allocate memory for window useage
AloWnd: mov     ah,048h                 ;allocate memory function
        mov     bx,1024                 ;allocate 16 K-bytes
        int     021h
        jnc     VidSet
        jmp     ErrAlo
VidSet: mov     [VidBuf],ax             ;save segment of 16 K-byte block

; Allocate remaining memory in 16 K-byte blocks
        mov     BYTE [BufCnt],0         ;set block counter to 0
        xor     bx,bx                   ;set table offset to 0
nxtalo: push    bx
        mov     ah,048h                 ;allocate memory function
        mov     bx,1024                 ;allocate 16 K-bytes at a time
        push    ds
        push    es
        int     021h
        pop     es
        pop     ds
        pop     bx
        jc      OpnHrd
        mov     WORD [BufSeg+bx],ax     ;save segment of 16 K-byte block
        inc     BYTE [BufCnt]           ;count 16 K-byte blocks
        inc     bx
        inc     bx
        cmp     BYTE [BufCnt],BLKMAX    ;allocate upto 64 blocks of memory
        jnz     nxtalo

; Open Z80 hard disk file image on current drive
OpnHrd: mov     ah,019h                 ;get current disk function
        int     021h                    ;A=0, B=1, C=2, etc
        cmp     al,02h
        jc      Z80res                  ;HDD file not supported on floppy
        mov     dx,Fname                ;offset address of ASCIIZ string
        mov     ah,043h                 ;get/set file attributes function
        mov     al,001h                 ;set attribute function
        mov     cx,0                    ;read/write file attribute
        int     021h                    ;change attribute
        mov     dx,Fname                ;offset address of ASCIIZ string
        mov     ah,03dh                 ;open file function
        mov     al,002h                 ;access mode, 2=read/write
        int     021h                    ;open disk file
        jc      Z80res                  ;HDD file not found if carry
        mov     [Handle],ax             ;save file handle
        mov     ah,019h                 ;get current disk function
        int     021h
        mov     [HrdFle],al             ;save drive code

; Z80 reset point, check what drive to boot from
Z80res: call    NEAR bnkres             ;reset bank #0, #1
        call    NEAR EraScr             ;clear the screen
        cmp     BYTE [flpflg],0         ;force boot up from floppy ?
        jnz     LdBoot
        cmp     BYTE [HrdFle],0ffh      ;HDD file present ?
        jz      Prompt
        mov     BYTE [es:Z80+0080h],0   ;kill any existing $C3 jump code
        mov     dl,[HrdFle]             ;HDD file drive
        mov     [BootDr],dl             ;save boot drive
        xor     ch,ch                   ;track #0
        mov     cl,1                    ;sector #1
        mov     bx,0080h                ;BX=buffer
        call    NEAR rdhard             ;read 1st sector from HDD file
        jmp     chkcde

; Prompt the user for a Z80 system disk in drive A
Prompt: mov     dh,9
        mov     dl,18
        mov     ch,15
        mov     cl,62
        call    NEAR OpnWnd
        mov     si,SgnMsg
        call    NEAR PrnStr
        call    NEAR HdeCur             ;hide the cursor
        mov     si,SgnKey               ;keys ESC, ^C, ENTER
        call    NEAR GetKey
        call    NEAR ClsWnd
        cmp     al,27                   ;ESC key pressed ?
        jz      Z80abt
        cmp     al,3                    ;Ctrl-C key pressed ?
        jz      Z80abt                  ;fall through if ENTER key

; Load 1st sector from Z80 system disk in drive A
LdBoot: mov     BYTE [es:Z80+0080h],0   ;kill any existing $C3 jump code
        mov     dh,0                    ;side #0
        mov     dl,[flpdrv]             ;drive code
        mov     [BootDr],dl             ;save boot drive
        mov     ch,0                    ;track #0
        mov     cl,1                    ;sector #1
        mov     al,2                    ;sector size code (2=512)
        mov     bx,0080h                ;BX=buffer
        call    NEAR rdflop             ;read 1st sector from floppy disk
chkcde: or      al,al
        jz      loaded
        jmp     NEAR fboot              ;failed to boot disk if here
loaded: cmp     BYTE [es:Z80+0080h],0c3h ;Z80 jump in 1st byte of sector ?
        jz      Z80exe
        jmp     NEAR SysErr             ;system not designed for Z80 Emulator
Z80abt: jmp     Z80ext                  ;exit the Z80 Emulator program

; Reset Z80 registers and execute Z80 code
Z80exe: xor     ax,ax                   ;Z80 reg   A=0
        mov     cx,ax                   ;Z80 reg  BC=0
        mov     dx,ax                   ;Z80 reg  DE=0
        mov     bx,ax                   ;Z80 reg  HL=0
        mov     [Z80IX],ax              ;Z80 reg  IX=0
        mov     [Z80IY],ax              ;Z80 reg  IY=0
        mov     [Z80AF],ax              ;Z80 reg AF'=0
        mov     [Z80BC],ax              ;Z80 reg BC'=0
        mov     [Z80DE],ax              ;Z80 reg DE'=0
        mov     [Z80HL],ax              ;Z80 reg HL'=0
        mov     [FlagX],ah              ;Z80 unused flag bits = 0
        mov     [FlagN],ah              ;Z80 N  flag=0
        mov     [FlagNx],ah             ;Z80 N' flag=0
        mov     [IFF],ah                ;Z80 interrupt Flip Flop = 0 (DI)
        sahf                            ;Z80 reg   F=0
        mov     bp,0080h                ;Z80 reg  PC=0080 hex
        mov     WORD [Z80SP],0ffffh     ;Z80 reg  SP=FFFF hex
Z80opc: XinstA                          ;execute 1st Z80 instruction

; Insufficient memory for Z80 map / Window buffers
ErrAlo: mov     dh,11
        mov     dl,20
        mov     ch,14
        mov     cl,59
        call    NEAR OpnWnd
        mov     si,MemMsg
        call    NEAR PrnStr
        call    NEAR HdeCur             ;hide the cursor
        mov     si,MemKey
        call    NEAR GetKey             ;wait for ESC key
        call    NEAR ClsWnd

; Exit the Z80 Emulator through here
Z80ext: cmp     BYTE [HrdFle],0ffh      ;Z80 hard disk file image opened ?
        jz      NotHDD
        mov     ah,03eh                 ;close file function
        mov     bx,[Handle]
        int     021h                    ;flush and close file
        mov     dx,Fname                ;offset address of ASCIIZ string
        mov     ah,043h                 ;get/set file attributes function
        mov     al,001h                 ;set attribute function
        mov     cx,001h                 ;read only file attribute
        int     021h                    ;change attribute
NotHDD: call    NEAR oldkbd
        mov     ah,033h                 ;get/set Ctrl-C flag function
        mov     al,001h                 ;set function
        mov     dl,[CtrlC]              ;restore original Ctrl-C flag
        int     021h
        cmp     BYTE [prmflg],0         ;was a command tail passed ?
        jnz     retcde
        mov     al,[VidMde]             ;set original video mode
        and     al,01111111b            ;force screen to be cleared
        mov     ah,000h                 ;set video mode function
        int     010h
retcde: mov     ax,4c00h                ;return to DOS with code function
        int     21h

SgnKey  db      27, 3, 13, '$'
MemKey  db      27, '$'

SgnMsg  db      '      Z80EM86-', APPVER, ' Z80 CPU Emulator', 0dh, 0ah
        db      '   Copyright (C) 1992-2009 Stewart Kay', 0dh, 0ah, 0ah
        db      '  Insert Z80 system disk into drive '
DrvChr  db      'A:', 0dh, 0ah
        db      '         Press ENTER when ready$'
MemMsg  db      'Insufficient memory for Z80 Emulator', 0dh, 0ah
        db      '     Press ESC to return to DOS$'

Fname   db      'Z80HDD.DSK', 0

; Local stack area setup here
SEGMENT	stkseg  align=16
        resw    256                     ;reserve space for stack
topstk  equ     $

; Empty segment for finding program size
SEGMENT	Zend    align=16

        end
