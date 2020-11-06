* 
*       Floating Point Division :
*               Front End to FFP Floating Point Package.
*
*               double
*               fpdiv(divisor,dividend)
*               double divisor, dividend;
*
*       Return : Floating Point Quotient
*
.globl fpdiv
.globl _fpdiv
.globl ffpdiv
.text
fpdiv:
_fpdiv:
~~fpdiv:
link    r14,#-4
movem.l d3-d7,-(sp)
move.l  8(r14),r7
move.l  12(r14),r6
jsr             ffpdiv
move.l  r7,r0
movem.l (sp)+,d3-d7
unlk    r14
rts

