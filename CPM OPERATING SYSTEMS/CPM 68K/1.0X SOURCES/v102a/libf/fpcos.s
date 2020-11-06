* 
*       Floating Point Cosine :
*               Front End to FFP Floating Point Package.
*
*               double
*               cos(farg)
*               double farg;
*
*       Input : in radians
*       Returns : cosine of Floating point number
*
.globl cos
.globl _cos
.globl ffpcos
.text
cos:
_cos:
~~cos:
link    r14,#-4
movem.l d3-d7,-(sp)
move.l  8(r14),r7
jsr             ffpcos
move.l  r7,r0
movem.l (sp)+,d3-d7
unlk    r14
rts

