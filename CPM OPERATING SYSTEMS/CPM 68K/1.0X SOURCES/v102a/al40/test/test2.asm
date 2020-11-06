.globl _low
.comm _low,2
.globl _high
.comm _high,2
.globl _err
.comm _err,2
.globl _globali
.data
_globali:
.dc.w -32768
.globl _globalu
.data
_globalu:
.dc.w -11232
.globl _main
.text
_main:
~~main:
link R14,#-4
*line 19
jsr _l_vs_i
*line 20
jsr _rl_vs_i
*line 21
jsr _l_vs_ri
*line 22
jsr _rl_vs_ri
*line 23
jsr _v41
*line 25
tst _err
bne L2
*line 25
move.l #L3,(sp)
jsr _printf
bra L4
L2:
*line 27
move.l #L5,(sp)
jsr _printf
L4:L1:unlk R14
rts
.globl _l_vs_i
.text
_l_vs_i:
~~l_vs_i:
~autol=-4
~autol1=-8
~i=-10
link R14,#-14
*line 35
move #3,-10(R14)
*line 36
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 37
move -10(R14),R0
ext.l R0
move.l R0,-(sp)
move.l -8(R14),-(sp)
jsr lmul
addq.l #8,sp
move.l R0,-8(R14)
*line 38
move -10(R14),R0
ext.l R0
move.l R0,-(sp)
move.l -4(R14),-(sp)
jsr lmul
addq.l #8,sp
move.l R0,-4(R14)
*line 39
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L7
*line 39
clr (sp)
jsr _error
L7:
*line 41
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 42
move.l -8(R14),R0
divs -10(R14),R0
ext.l R0
move.l R0,-8(R14)
*line 43
move.l -4(R14),R0
divs -10(R14),R0
ext.l R0
move.l R0,-4(R14)
*line 44
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L8
*line 44
move #1,(sp)
jsr _error
L8:
*line 46
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 47
move -10(R14),R0
ext.l R0
add.l R0,-8(R14)
*line 48
move -10(R14),R0
ext.l R0
add.l -4(R14),R0
move.l R0,-4(R14)
*line 49
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L9
*line 49
move #2,(sp)
jsr _error
L9:
*line 51
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 52
move -10(R14),R0
ext.l R0
sub.l R0,-8(R14)
*line 53
move.l -4(R14),R0
move -10(R14),R1
ext.l R1
sub.l R1,R0
move.l R0,-4(R14)
*line 54
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L10
*line 54
move #3,(sp)
jsr _error
L10:
*line 56
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 57
move.l -8(R14),R0
divs -10(R14),R0
swap R0
ext.l R0
move.l R0,-8(R14)
*line 58
move.l -4(R14),R0
divs -10(R14),R0
swap R0
ext.l R0
move.l R0,-4(R14)
*line 59
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L11
*line 59
move #4,(sp)
jsr _error
L11:
*line 61
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 62
move -10(R14),R1
move.l -8(R14),R0
asl.l R1,R0
move.l R0,-8(R14)
*line 63
move.l -4(R14),R0
move -10(R14),R1
asl.l R1,R0
move.l R0,-4(R14)
*line 64
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L12
*line 64
move #5,(sp)
jsr _error
L12:
*line 66
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 67
move -10(R14),R1
move.l -8(R14),R0
asr.l R1,R0
move.l R0,-8(R14)
*line 68
move.l -4(R14),R0
move -10(R14),R1
asr.l R1,R0
move.l R0,-4(R14)
*line 69
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L13
*line 69
move #6,(sp)
jsr _error
L13:
*line 71
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 72
move -10(R14),R0
ext.l R0
and.l R0,-8(R14)
*line 73
move -10(R14),R0
ext.l R0
and.l -4(R14),R0
move.l R0,-4(R14)
*line 74
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L14
*line 74
move #7,(sp)
jsr _error
L14:
*line 76
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 77
move -10(R14),R0
ext.l R0
or.l R0,-8(R14)
*line 78
move -10(R14),R0
ext.l R0
or.l -4(R14),R0
move.l R0,-4(R14)
*line 79
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L15
*line 79
move #8,(sp)
jsr _error
L15:
*line 81
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 82
move -10(R14),R0
ext.l R0
eor.l R0,-8(R14)
*line 83
move -10(R14),R0
ext.l R0
move.l -4(R14),R1
eor.l R1,R0
move.l R0,-4(R14)
*line 84
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L16
*line 84
move #9,(sp)
jsr _error
L16:L6:unlk R14
rts
.globl _rl_vs_i
.text
_rl_vs_i:
~~rl_vs_i:
~i=-2
~regl=R7
~regl1=R6
link R14,#-2
movem.l R5-R7,-(sp)
*line 92
move #3,-2(R14)
*line 93
move.l #100,R6
move.l R6,R7
*line 94
move -2(R14),R0
ext.l R0
move.l R0,-(sp)
move.l R6,-(sp)
jsr lmul
addq.l #8,sp
move.l R0,R6
*line 95
move -2(R14),R0
ext.l R0
move.l R0,-(sp)
move.l R7,-(sp)
jsr lmul
addq.l #8,sp
move.l R0,R7
*line 96
cmp.l R7,R6
beq L18
*line 96
move #16,(sp)
jsr _error
L18:
*line 98
move.l #100,R6
move.l R6,R7
*line 99
divs -2(R14),R6
ext.l R6
*line 100
divs -2(R14),R7
ext.l R7
*line 101
cmp.l R7,R6
beq L19
*line 101
move #17,(sp)
jsr _error
L19:
*line 103
move.l #100,R6
move.l R6,R7
*line 104
move -2(R14),R0
ext.l R0
add.l R0,R6
*line 105
move -2(R14),R0
ext.l R0
add.l R0,R7
*line 106
cmp.l R7,R6
beq L20
*line 106
move #18,(sp)
jsr _error
L20:
*line 108
move.l #100,R6
move.l R6,R7
*line 109
move -2(R14),R0
ext.l R0
sub.l R0,R6
*line 110
move -2(R14),R0
ext.l R0
sub.l R0,R7
*line 111
cmp.l R7,R6
beq L21
*line 111
move #19,(sp)
jsr _error
L21:
*line 113
move.l #100,R6
move.l R6,R7
*line 114
divs -2(R14),R6
swap R6
ext.l R6
*line 115
divs -2(R14),R7
swap R7
ext.l R7
*line 116
cmp.l R7,R6
beq L22
*line 116
move #20,(sp)
jsr _error
L22:
*line 118
move.l #100,R6
move.l R6,R7
*line 119
clr.l R0
move -2(R14),R0
asl.l R0,R6
*line 120
clr.l R0
move -2(R14),R0
asl.l R0,R7
*line 121
cmp.l R7,R6
beq L23
*line 121
move #21,(sp)
jsr _error
L23:
*line 123
move.l #100,R6
move.l R6,R7
*line 124
clr.l R0
move -2(R14),R0
asr.l R0,R6
*line 125
clr.l R0
move -2(R14),R0
asr.l R0,R7
*line 126
cmp.l R7,R6
beq L24
*line 126
move #22,(sp)
jsr _error
L24:
*line 128
move.l #100,R6
move.l R6,R7
*line 129
move -2(R14),R0
ext.l R0
and.l R0,R6
*line 130
move -2(R14),R0
ext.l R0
and.l R0,R7
*line 131
cmp.l R7,R6
beq L25
*line 131
move #23,(sp)
jsr _error
L25:
*line 133
move.l #100,R6
move.l R6,R7
*line 134
move -2(R14),R0
ext.l R0
or.l R0,R6
*line 135
move -2(R14),R0
ext.l R0
or.l R0,R7
*line 136
cmp.l R7,R6
beq L26
*line 136
move #24,(sp)
jsr _error
L26:
*line 138
move.l #100,R6
move.l R6,R7
*line 139
move -2(R14),R0
ext.l R0
eor.l R0,R6
*line 140
move -2(R14),R0
ext.l R0
eor.l R0,R7
*line 141
cmp.l R7,R6
beq L27
*line 141
move #25,(sp)
jsr _error
L27:L17:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _l_vs_ri
.text
_l_vs_ri:
~~l_vs_ri:
~autol=-4
~autol1=-8
~i=R7
link R14,#-8
movem.l R6-R7,-(sp)
*line 149
move #2,R7
*line 150
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 151
move R7,R0
ext.l R0
move.l R0,-(sp)
move.l -8(R14),-(sp)
jsr lmul
addq.l #8,sp
move.l R0,-8(R14)
*line 152
move R7,R0
ext.l R0
move.l R0,-(sp)
move.l -4(R14),-(sp)
jsr lmul
addq.l #8,sp
move.l R0,-4(R14)
*line 153
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L29
*line 153
move #32,(sp)
jsr _error
L29:
*line 155
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 156
move.l -8(R14),R0
divs R7,R0
ext.l R0
move.l R0,-8(R14)
*line 157
move.l -4(R14),R0
divs R7,R0
ext.l R0
move.l R0,-4(R14)
*line 158
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L30
*line 158
move #33,(sp)
jsr _error
L30:
*line 160
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 161
move R7,R0
ext.l R0
add.l R0,-8(R14)
*line 162
move R7,R0
ext.l R0
add.l -4(R14),R0
move.l R0,-4(R14)
*line 163
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L31
*line 163
move #34,(sp)
jsr _error
L31:
*line 165
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 166
move R7,R0
ext.l R0
sub.l R0,-8(R14)
*line 167
move.l -4(R14),R0
move R7,R1
ext.l R1
sub.l R1,R0
move.l R0,-4(R14)
*line 168
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L32
*line 168
move #35,(sp)
jsr _error
L32:
*line 170
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 171
move.l -8(R14),R0
divs R7,R0
swap R0
ext.l R0
move.l R0,-8(R14)
*line 172
move.l -4(R14),R0
divs R7,R0
swap R0
ext.l R0
move.l R0,-4(R14)
*line 173
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L33
*line 173
move #36,(sp)
jsr _error
L33:
*line 175
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 176
move R7,R1
move.l -8(R14),R0
asl.l R1,R0
move.l R0,-8(R14)
*line 177
move.l -4(R14),R0
move R7,R1
asl.l R1,R0
move.l R0,-4(R14)
*line 178
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L34
*line 178
move #37,(sp)
jsr _error
L34:
*line 180
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 181
move R7,R1
move.l -8(R14),R0
asr.l R1,R0
move.l R0,-8(R14)
*line 182
move.l -4(R14),R0
move R7,R1
asr.l R1,R0
move.l R0,-4(R14)
*line 183
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L35
*line 183
move #38,(sp)
jsr _error
L35:
*line 185
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 186
move R7,R0
ext.l R0
and.l R0,-8(R14)
*line 187
move R7,R0
ext.l R0
and.l -4(R14),R0
move.l R0,-4(R14)
*line 188
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L36
*line 188
move #39,(sp)
jsr _error
L36:
*line 190
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 191
move R7,R0
ext.l R0
or.l R0,-8(R14)
*line 192
move R7,R0
ext.l R0
or.l -4(R14),R0
move.l R0,-4(R14)
*line 193
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L37
*line 193
move #40,(sp)
jsr _error
L37:
*line 195
move #100,R0
ext.l R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 196
move R7,R0
ext.l R0
eor.l R0,-8(R14)
*line 197
move R7,R0
ext.l R0
move.l -4(R14),R1
eor.l R1,R0
move.l R0,-4(R14)
*line 198
move.l -8(R14),R0
cmp.l -4(R14),R0
beq L38
*line 198
move #41,(sp)
jsr _error
L38:L28:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.globl _rl_vs_ri
.text
_rl_vs_ri:
~~rl_vs_ri:
~i=R5
~regl=R7
~regl1=R6
link R14,#0
movem.l R4-R7,-(sp)
*line 206
move #3,R5
*line 207
move.l #100,R6
move.l R6,R7
*line 208
move R5,R0
ext.l R0
move.l R0,-(sp)
move.l R6,-(sp)
jsr lmul
addq.l #8,sp
move.l R0,R6
*line 209
move R5,R0
ext.l R0
move.l R0,-(sp)
move.l R7,-(sp)
jsr lmul
addq.l #8,sp
move.l R0,R7
*line 210
cmp.l R7,R6
beq L40
*line 210
move #48,(sp)
jsr _error
L40:
*line 212
move.l #100,R6
move.l R6,R7
*line 213
divs R5,R6
ext.l R6
*line 214
divs R5,R7
ext.l R7
*line 215
cmp.l R7,R6
beq L41
*line 215
move #49,(sp)
jsr _error
L41:
*line 217
move.l #100,R6
move.l R6,R7
*line 218
move R5,R0
ext.l R0
add.l R0,R6
*line 219
move R5,R0
ext.l R0
add.l R0,R7
*line 220
cmp.l R7,R6
beq L42
*line 220
move #50,(sp)
jsr _error
L42:
*line 222
move.l #100,R6
move.l R6,R7
*line 223
move R5,R0
ext.l R0
sub.l R0,R6
*line 224
move R5,R0
ext.l R0
sub.l R0,R7
*line 225
cmp.l R7,R6
beq L43
*line 225
move #51,(sp)
jsr _error
L43:
*line 227
move.l #100,R6
move.l R6,R7
*line 228
divs R5,R6
swap R6
ext.l R6
*line 229
divs R5,R7
swap R7
ext.l R7
*line 230
cmp.l R7,R6
beq L44
*line 230
move #52,(sp)
jsr _error
L44:
*line 232
move.l #100,R6
move.l R6,R7
*line 233
clr.l R0
move R5,R0
asl.l R0,R6
*line 234
clr.l R0
move R5,R0
asl.l R0,R7
*line 235
cmp.l R7,R6
beq L45
*line 235
move #53,(sp)
jsr _error
L45:
*line 237
move.l #100,R6
move.l R6,R7
*line 238
clr.l R0
move R5,R0
asr.l R0,R6
*line 239
clr.l R0
move R5,R0
asr.l R0,R7
*line 240
cmp.l R7,R6
beq L46
*line 240
move #54,(sp)
jsr _error
L46:
*line 242
move.l #100,R6
move.l R6,R7
*line 243
move R5,R0
ext.l R0
and.l R0,R6
*line 244
move R5,R0
ext.l R0
and.l R0,R7
*line 245
cmp.l R7,R6
beq L47
*line 245
move #55,(sp)
jsr _error
L47:
*line 247
move.l #100,R6
move.l R6,R7
*line 248
move R5,R0
ext.l R0
or.l R0,R6
*line 249
move R5,R0
ext.l R0
or.l R0,R7
*line 250
cmp.l R7,R6
beq L48
*line 250
move #56,(sp)
jsr _error
L48:
*line 252
move.l #100,R6
move.l R6,R7
*line 253
move R5,R0
ext.l R0
eor.l R0,R6
*line 254
move R5,R0
ext.l R0
eor.l R0,R7
*line 255
cmp.l R7,R6
beq L49
*line 255
move #57,(sp)
jsr _error
L49:L39:tst.l (sp)+
movem.l (sp)+,R5-R7
unlk R14
rts
.globl _v41
.text
_v41:
~~v41:
~l=-4
~l1=-8
link R14,#-12
*line 263
move _low,R0
and #-4096,R0
move _high,R1
move #12,R2
lsr R2,R1
and #15,R1
or R1,R0
move R0,_low
*line 264
move _high,R0
and #-4096,R0
move _low,R1
move #12,R2
lsr R2,R1
and #15,R1
or R1,R0
move R0,_high
*line 266
move.l #$2d,R0
move.l R0,-8(R14)
move.l R0,-4(R14)
*line 268
*line 269
move.l -4(R14),R0
cmp.l -8(R14),R0
beq L51
*line 269
move #64,(sp)
jsr _error
L51:
*line 270
*line 271
move.l -4(R14),R0
cmp.l -8(R14),R0
beq L52
*line 271
move #65,(sp)
jsr _error
L52:
*line 272
*line 273
move.l -4(R14),R0
cmp.l -8(R14),R0
beq L53
*line 273
move #66,(sp)
jsr _error
L53:
*line 274
clr.l -4(R14)
*line 275
tst.l -4(R14)
beq L54
*line 275
move #67,(sp)
jsr _error
L54:
*line 276
clr.l -8(R14)
*line 277
tst.l -8(R14)
beq L55
*line 277
move #68,(sp)
jsr _error
L55:L50:unlk R14
rts
.globl _error
.text
_error:
~~error:
~pc=8
link R14,#-4
*line 282
move 8(R14),(sp)
move.l #L57,-(sp)
jsr _printf
addq.l #4,sp
*line 283
add #1,_err
L56:unlk R14
rts
.data
L3:.dc.b 70,97,114,32,70,114,101,97,107,105,110,103,32,79,117,116,10,0
L5:.dc.b 84,111,117,103,104,32,76,117,99,107,46,46,46,46,46,10,0
L57:.dc.b 101,114,114,111,114,32,37,120,10,0
