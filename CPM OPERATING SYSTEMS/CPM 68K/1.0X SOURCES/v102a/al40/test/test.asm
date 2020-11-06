.globl _uu
.comm _uu,4
.globl _s1
.comm _s1,8
.globl _flags
.comm _flags,2
.globl _zz
.comm _zz,24
.globl _c
.comm _c,2
.data
L1:.dc.b 115,116,114,105,110,103,32,49,0
.even
.text
.bss
L3:.ds.b 4
.text
.globl _l1
.comm _l1,4
.globl _l2
.comm _l2,4
.globl _l3
.comm _l3,4
.globl _ldivr
.comm _ldivr,4
.globl _initi
.data
_initi:
.dc.w 1
.globl _initk
.data
_initk:
.dc.w 1
.dc.w 2
.dc.w 3
.globl _initl
.data
_initl:
.dc.w 1
.dc.w 2
.dc.w 3
.dc.w 4
.dc.w 5
.dc.w 6
.dc.w 7
.dc.w 8
.dc.w 9
.dc.w 10
.dc.w 11
.dc.w 12
.globl _initx
.data
_initx:
*line 51
.dc.l L4
.dc.w 0,0
.globl _inits
.data
_inits:
.dc.w 0
.dc.w 0,0
.dc.w 1
.dc.w 0,1
.dc.w 2
.dc.w 0,2
.globl _initc
.data
_initc:
.dc.b 116,104,105,115,32,105,115,32,105,116,0
.ds.b 9
.globl _initd
.data
_initd:
.dc.b 116,104,105,115,32,109,105,103,104,116,32,98,101,32,105,116,0
.even
.globl _inite
.data
_inite:
.dc.b 1
.dc.b 2
.dc.b 3
.dc.b 4
.dc.b 5
.dc.b 6
.dc.b 7
.dc.b 8
.dc.b 9
.even
.globl _initcc
.data
_initcc:
*line 65
.dc.l L7
.globl _ccinit
.data
_ccinit:
.dc.w 258
.globl _ac
.comm _ac,4
.globl _ai
.comm _ai,8
.globl _pai
.data
_pai:
*line 70
.dc.l 2+_ai
*line 70
.dc.l 6+_ai
.globl _al
.comm _al,16
.globl _p0
.comm _p0,4
.globl _x
.comm _x,10
.globl _sound
.data
_sound:
.dc.w 1
*line 95
.dc.l 2+_x
.globl _largear
.comm _largear,20000
.globl _main
.text
_main:
~~main:
~a=-26
~i=R7
~j=-2
~l=L3
~n=-16
~p=R13
~u1=R6
~u2=R5
~u3=-18
~pi=-22
~ai2=-14
~lr1=R4
~str=L1
link R14,#-26
movem.l R3-R7/R13-R13,-(sp)
*line 131
move.l #L9,(sp)
jsr _putstrin
*line 132
clr -2(R14)
bra L12
L13:
*line 133
move.b #65,_c
*line 134
clr R7
bra L16
L17:
*line 135
move.b _c,R0
ext.w R0
move R0,(sp)
jsr _putchar
*line 136
add.b #1,_c
L15:
*line 134
add #1,R7
L16:
*line 134
cmp #62,R7
blt L17
L14:
*line 138
move #10,(sp)
jsr _putchar
L11:
*line 132
add #1,-2(R14)
L12:
*line 132
cmp #5,-2(R14)
blt L13
L10:
*line 140
bra L18
*line 140
clr (sp)
jsr _error
L18:
*line 141
move R7,-2(R14)
*line 142
cmp -2(R14),R7
beq L19
*line 142
move #10,(sp)
jsr _error
L19:
*line 143
bra L20
*line 143
move #11,(sp)
jsr _error
L20:
*line 144
bra L21
*line 144
move #12,(sp)
jsr _error
L21:
*line 145
bra L22
*line 145
move #13,(sp)
jsr _error
L22:
*line 146
bra L23
*line 146
move #14,(sp)
jsr _error
L23:
*line 147
bra L24
*line 147
move #15,(sp)
jsr _error
L24:
*line 148
bra L25
*line 148
move #16,(sp)
jsr _error
L25:
*line 149
clr R7
*line 149
tst R7
beq L26
*line 149
move #1,(sp)
jsr _error
L26:
*line 150
tst R7
beq L27
*line 150
move #2,(sp)
jsr _error
L27:
*line 151
add #1,R7
move R7,R0
cmp #1,R0
beq L28
*line 151
move #3,(sp)
jsr _error
L28:
*line 152
asl #2,R7
*line 153
cmp #4,R7
beq L29
*line 153
move #4,(sp)
jsr _error
L29:
*line 154
move R7,R0
add #2,R0
add R0,R7
*line 155
cmp #10,R7
beq L30
*line 155
move #5,(sp)
jsr _error
L30:
*line 156
muls #6,R7
*line 157
cmp #60,R7
beq L31
*line 157
move #6,(sp)
jsr _error
L31:
*line 158
ext.l R7
divs #3,R7
*line 159
cmp #20,R7
beq L32
*line 159
move #7,(sp)
jsr _error
L32:
*line 160
add #12,R7
or #7,R7
*line 161
cmp #39,R7
beq L33
*line 161
move #8,(sp)
jsr _error
L33:
*line 162
move R7,R0
move R7,R1
asr #1,R1
sub R1,R0
move R0,R7
sub #2,R7
*line 163
cmp #18,R7
beq L34
*line 163
move #9,(sp)
jsr _error
L34:
*line 164
ext.l R7
divs #6,R7
swap R7
*line 165
tst R7
beq L35
*line 165
move #16,(sp)
jsr _error
L35:
*line 166
clr -2(R14)
*line 167
tst -2(R14)
beq L36
*line 167
move #17,(sp)
jsr _error
L36:
*line 168
tst -2(R14)
beq L37
*line 168
move #18,(sp)
jsr _error
L37:
*line 169
add #1,-2(R14)
move -2(R14),R0
cmp #1,R0
beq L38
*line 169
move #19,(sp)
jsr _error
L38:
*line 170
move -2(R14),R0
asl #2,R0
move R0,-2(R14)
*line 171
cmp #4,-2(R14)
beq L39
*line 171
move #20,(sp)
jsr _error
L39:
*line 172
move -2(R14),R0
add #2,R0
add R0,-2(R14)
*line 173
cmp #10,-2(R14)
beq L40
*line 173
move #21,(sp)
jsr _error
L40:
*line 174
move -2(R14),R0
muls #6,R0
move R0,-2(R14)
*line 175
cmp #60,-2(R14)
beq L41
*line 175
move #22,(sp)
jsr _error
L41:
*line 176
move -2(R14),R0
ext.l R0
divs #3,R0
move R0,-2(R14)
*line 177
cmp #20,-2(R14)
beq L42
*line 177
move #23,(sp)
jsr _error
L42:
*line 178
move -2(R14),R0
add #12,R0
or #7,R0
move R0,-2(R14)
*line 179
cmp #39,-2(R14)
beq L43
*line 179
move #24,(sp)
jsr _error
L43:
*line 180
move -2(R14),R0
move -2(R14),R1
asr #1,R1
sub R1,R0
sub #2,R0
move R0,-2(R14)
*line 181
cmp #18,-2(R14)
beq L44
*line 181
move #25,(sp)
jsr _error
L44:
*line 182
move #1229,-2(R14)
*line 183
asl -2(R14)
*line 184
cmp #2458,-2(R14)
beq L45
*line 184
move #527,(sp)
jsr _error
L45:
*line 185
clr.l L3
*line 186
tst.l L3
beq L46
*line 186
move #33,(sp)
jsr _error
L46:
*line 187
tst.l L3
beq L47
*line 187
move #34,(sp)
jsr _error
L47:
*line 188
add.l #1,L3
move.l L3,R0
cmp.l #1,R0
beq L48
*line 188
move #35,(sp)
jsr _error
L48:
*line 189
move.l L3,R0
asl.l #2,R0
move.l R0,L3
*line 190
cmp.l #4,L3
beq L49
*line 190
move #36,(sp)
jsr _error
L49:
*line 191
move.l L3,R0
add.l #2,R0
add.l R0,L3
*line 192
cmp.l #10,L3
beq L50
*line 192
move #37,(sp)
jsr _error
L50:
*line 193
move.l #6,-(sp)
move.l L3,-(sp)
jsr lmul
addq.l #8,sp
move.l R0,L3
*line 194
cmp.l #60,L3
beq L51
*line 194
move #38,(sp)
jsr _error
L51:
*line 195
move.l L3,R0
divs #3,R0
ext.l R0
move.l R0,L3
*line 196
cmp.l #20,L3
beq L52
*line 196
move #39,(sp)
jsr _error
L52:
*line 197
move.l L3,R0
add.l #12,R0
or.l #7,R0
move.l R0,L3
*line 198
cmp.l #39,L3
beq L53
*line 198
move #40,(sp)
jsr _error
L53:
*line 199
move.l L3,R0
move.l L3,R1
asr.l #1,R1
sub.l R1,R0
sub.l #2,R0
move.l R0,L3
*line 200
cmp.l #18,L3
beq L54
*line 200
move #41,(sp)
jsr _error
L54:
*line 201
move.l L3,R0
divs #6,R0
swap R0
ext.l R0
move.l R0,L3
*line 202
tst.l L3
beq L55
*line 202
move #656,(sp)
jsr _error
L55:
*line 203
move.l #$3039,L3
*line 204
cmp.l #12345,L3
beq L56
*line 204
move #657,(sp)
jsr _error
L56:
*line 205
move.l #$fffffffe,L3
*line 206
cmp.l #-2,L3
beq L57
*line 206
move #658,(sp)
jsr _error
L57:
*line 207
move.l #$9c40,L3
*line 207
move.l L3,R0
divs #10,R0
ext.l R0
move.l R0,L3
cmp.l #4000,R0
bne L10000
cmp.l #4000,L3
beq L58
L10000:*line 207
move #659,(sp)
jsr _error
L58:
*line 208
move.l #$9c40,L3
*line 208
move.l L3,R0
divs #10,R0
ext.l R0
move.l R0,L3
*line 208
cmp.l #4000,L3
beq L59
*line 208
move #660,(sp)
jsr _error
L59:
*line 209
move.l #$9c40,L3
*line 209
move.l #$a,-(sp)
move.l L3,-(sp)
jsr ldiv
addq.l #8,sp
move.l R0,L3
*line 209
cmp.l #4000,L3
beq L60
*line 209
move #661,(sp)
jsr _error
L60:
*line 210
move.l #$9c40,L3
*line 210
move.l L3,R0
divs #400,R0
swap R0
ext.l R0
tst.l R0
beq L61
*line 210
move #662,(sp)
jsr _error
L61:
*line 211
move.l #$9c40,L3
*line 211
move.l #$190,-(sp)
move.l L3,-(sp)
jsr lrem
addq.l #8,sp
tst.l R0
beq L62
*line 211
move #663,(sp)
jsr _error
L62:
*line 212
move #-18363,(sp)
move #1000,-(sp)
move #1,-(sp)
jsr _targs
addq.l #4,sp
*line 213
move #65,(sp)
move #7,-(sp)
move #2,-(sp)
jsr _addargs
addq.l #4,sp
move R0,R7
*line 214
cmp #74,R7
beq L63
*line 214
move #48,(sp)
jsr _error
L63:
*line 215
move.l #$2710,_l1
*line 216
move.l #4273,_l2
*line 217
move.l #$abcd5678,R4
*line 218
move.l _l2,(sp)
move.l _l1,-(sp)
jsr _addlargs
addq.l #4,sp
move.l R0,L3
move.l _l1,R1
add.l _l2,R1
cmp.l R1,R0
beq L64
*line 218
move #784,(sp)
jsr _error
L64:
*line 219
move.l _l1,R0
add.l _l2,R0
move.l R0,(sp)
move.l _l1,-(sp)
jsr _addlargs
addq.l #4,sp
move.l R0,-(sp)
move.l _l1,-(sp)
move.l _l1,R0
add.l _l2,R0
move.l R0,-(sp)
jsr _addlargs
addq.l #8,sp
cmp.l (sp)+,R0
beq L65
*line 219
move #785,(sp)
jsr _error
L65:
*line 220
move.l #L66,R13
*line 221
move.l R13,(sp)
jsr _strlen
ext.l R0
add.l R13,R0
move.l R0,R8
move.b #115,-1(R8)
*line 222
move.l #L67,(sp)
move.l R13,-(sp)
jsr _eqstr
addq.l #4,sp
tst R0
bne L68
*line 222
move #786,(sp)
jsr _error
L68:
*line 223
cmp.l #$abcd5678,R4
beq L69
*line 223
move #787,(sp)
jsr _error
L69:
*line 224
move.l #L71,(sp)
move.l #L70,-(sp)
jsr _eqstr
addq.l #4,sp
tst R0
bne L72
*line 224
move #788,(sp)
jsr _error
L72:
*line 225
move.l #L74,(sp)
move.l #L73,-(sp)
jsr _eqstr
addq.l #4,sp
tst R0
beq L75
*line 225
move #789,(sp)
jsr _error
L75:
*line 226
move.l #L77,(sp)
jsr _strlen
move R0,-(sp)
move.l #L76,-(sp)
jsr _strlen
addq.l #4,sp
cmp (sp)+,R0
bne L78
*line 226
move #790,(sp)
jsr _error
L78:
*line 227
move.l #L80,(sp)
jsr _strlen
move R0,-(sp)
move.l #L79,-(sp)
jsr _strlen
addq.l #4,sp
add #1,R0
cmp (sp)+,R0
beq L81
*line 227
move #791,(sp)
jsr _error
L81:
*line 228
move #1,-2(R14)
clr R7
beq L82
*line 228
move #792,(sp)
jsr _error
L82:
*line 229
clr -2(R14)
move #1,R7
move R7,R0
cmp #1,R0
beq L83
*line 229
move #793,(sp)
jsr _error
L83:
*line 230
move.l #-3347,_l1
*line 231
move.l #7913,_l2
*line 232
move.l #_l2,(sp)
move.l #_l1,-(sp)
jsr _addpargs
addq.l #4,sp
move.l R0,L3
move.l _l1,R1
add.l _l2,R1
cmp.l R1,R0
beq L84
*line 232
move #52,(sp)
jsr _error
L84:
*line 233
move.l #60,L3
*line 234
move.l #9,-(sp)
move.l L3,-(sp)
jsr lmul
addq.l #8,sp
move.l R0,L3
cmp.l #540,R0
beq L85
*line 234
move #54,(sp)
jsr _error
L85:
*line 235
move.b #255,_c
*line 236
cmp.b #255,_c
beq L86
*line 236
move #54,(sp)
jsr _error
L86:
*line 237
move.b _c,R7
ext.w R7
*line 238
cmp #-1,R7
beq L87
*line 238
move #55,(sp)
jsr _error
L87:
*line 239
move.b _c,R0
ext.w R0
move R0,-2(R14)
*line 240
cmp #-1,-2(R14)
beq L88
*line 240
move #56,(sp)
jsr _error
L88:
*line 241
move.b _c,R0
ext.w R0
ext.l R0
move.l R0,L3
*line 242
cmp.l #-1,L3
beq L89
*line 242
move #57,(sp)
jsr _error
L89:
*line 243
move.b #31,_c
*line 244
btst #4,_c
bne L90
*line 244
move #922,(sp)
jsr _error
L90:
*line 245
btst #6,_c
beq L91
*line 245
move #923,(sp)
jsr _error
L91:
*line 246
clr _ai
*line 247
move #1,2+_ai
*line 248
move #2,4+_ai
*line 249
move #3,6+_ai
*line 250
tst _ai
beq L92
*line 250
move #64,(sp)
jsr _error
L92:
*line 251
cmp #1,2+_ai
beq L93
*line 251
move #65,(sp)
jsr _error
L93:
*line 252
cmp #2,4+_ai
beq L94
*line 252
move #66,(sp)
jsr _error
L94:
*line 253
cmp #3,6+_ai
beq L95
*line 253
move #67,(sp)
jsr _error
L95:
*line 254
clr R7
bra L98
L99:
*line 255
move R7,R8
add.l R8,R8
cmp _ai(R8),R7
beq L100
*line 255
move #68,(sp)
jsr _error
L100:
*line 256
move R7,R8
add.l R8,R8
move _ai(R8),R0
move R7,R9
move.b R0,_ac(R9)
L97:
*line 254
add #1,R7
L98:
*line 254
cmp #3,R7
ble L99
L96:L101:
*line 259
clr R7
bra L104
L105:
*line 260
move R7,R8
move.b _ac(R8),R0
ext.w R0
cmp R0,R7
beq L106
*line 260
move #69,(sp)
jsr _error
L106:
*line 261
move R7,R0
muls #30000,R0
move R7,R9
add.l R9,R9
add.l R9,R9
move.l R0,_al(R9)
L103:
*line 259
add #1,R7
L104:
*line 259
cmp #4,R7
blt L105
L102:
*line 263
clr R7
bra L109
L110:
*line 264
move R7,R8
add.l R8,R8
add.l R8,R8
move.l _al(R8),R0
move R7,R1
muls #30000,R1
cmp.l R1,R0
beq L111
*line 264
move #70,(sp)
jsr _error
L111:
*line 265
move R7,R8
add.l R8,R8
add.l R8,R8
move.l _al(R8),R0
divs #30000,R0
ext.l R0
move R7,R8
cmp.l R8,R0
beq L112
*line 265
move #71,(sp)
jsr _error
L112:
*line 266
move R7,R8
add.l R8,R8
add.l R8,R8
move.l _al(R8),R0
divs #100,R0
ext.l R0
move R7,R1
muls #300,R1
cmp.l R1,R0
beq L113
*line 266
move #72,(sp)
jsr _error
L113:L108:
*line 263
add #1,R7
L109:
*line 263
cmp #4,R7
blt L110
L107:
*line 268
move.b #3,-16(R14)
*line 269
move.b -16(R14),R0
ext.w R0
move R0,R8
cmp.b #105,L1(R8)
beq L114
*line 269
move #1162,(sp)
jsr _error
L114:
*line 270
move.b -16(R14),R0
ext.w R0
move R0,R8
add.l R8,R8
move _ai(R8),R0
move.b -16(R14),R1
ext.w R1
cmp R1,R0
beq L115
*line 270
move #1163,(sp)
jsr _error
L115:
*line 271
move.b -16(R14),R0
ext.w R0
move R0,R8
cmp.b #3,_ac(R8)
bne L10001
move.b -16(R14),R0
ext.w R0
move R0,R8
add.l R8,R8
add.l R8,R8
move.l _al(R8),R0
move.b -16(R14),R1
ext.w R1
muls #30000,R1
cmp.l R1,R0
beq L116
L10001:*line 271
move #1164,(sp)
jsr _error
L116:
*line 272
clr R7
*line 273
clr -2(R14)
bra L119
L120:
*line 274
add #5,R7
L118:
*line 273
add #1,-2(R14)
L119:
*line 273
cmp #100,-2(R14)
blt L120
L117:
*line 275
cmp #100,-2(R14)
beq L121
*line 275
move #73,(sp)
jsr _error
L121:
*line 276
cmp #500,R7
beq L122
*line 276
move #80,(sp)
jsr _error
L122:
*line 277
move #-9363,R7
*line 278
move R7,R0
eor R0,R7
*line 279
tst R7
beq L123
*line 279
move #81,(sp)
jsr _error
L123:
*line 280
move.b #64,_s1
*line 281
move #1250,2+_s1
*line 282
move.l #9264,4+_s1
*line 283
move.l #_s1,R13
*line 284
cmp.b #64,_s1
bne L10002
cmp.b #64,(R13)
beq L124
L10002:*line 284
move #82,(sp)
jsr _error
L124:
*line 285
cmp #1250,2+_s1
bne L10003
cmp #1250,2(R13)
beq L125
L10003:*line 285
move #83,(sp)
jsr _error
L125:
*line 286
cmp.l #9264,4+_s1
bne L126
cmp.l #9264,4(R13)
bne L126
bra L127
L126:
*line 286
move #84,(sp)
jsr _error
L127:
*line 287
move #-50,-2(R14)
*line 288
tst -2(R14)
blt L128
*line 288
move #85,(sp)
jsr _error
L128:
*line 289
tst -2(R14)
bge L129
bra L130
L129:
*line 289
move #86,(sp)
jsr _error
L130:
*line 290
move.l #7777,L3
*line 291
cmp.l #7776,L3
bge L131
*line 291
move #87,(sp)
jsr _error
L131:
*line 292
cmp.l #7776,L3
ble L132
bra L133
L132:
*line 292
move #88,(sp)
jsr _error
L133:
*line 293
move.l #_ac,R13
*line 294
move.l #3+_ac,_p0
*line 295
cmp.l _p0,R13
blo L134
*line 295
move #89,(sp)
jsr _error
L134:
*line 296
cmp.l _p0,R13
bhi L135
bra L136
L135:
*line 296
move #96,(sp)
jsr _error
L136:
*line 297
move.l R13,R0
bne L137
*line 297
move #97,(sp)
jsr _error
L137:
*line 298
tst.l _p0
beq L138
bra L139
L138:
*line 298
move #98,(sp)
jsr _error
L139:
*line 299
clr R7
bra L142
L143:
*line 300
clr -2(R14)
bra L146
L147:
*line 301
move R7,R0
add #2,R0
move -2(R14),R1
add #3,R1
muls R1,R0
move.l R14,R9
move -2(R14),R10
add.l R10,R10
add.l R10,R9
move R7,R2
muls #6,R2
add.l R2,R9
move R0,-14(R9)
L145:
*line 300
add #1,-2(R14)
L146:
*line 300
cmp #3,-2(R14)
blt L147
L144:L141:
*line 299
add #1,R7
L142:
*line 299
cmp #2,R7
blt L143
L140:
*line 304
clr R7
bra L150
L151:
*line 305
clr -2(R14)
bra L154
L155:
*line 306
move R7,R0
muls #6,R0
move.l R0,R8
move.l R14,R9
move -2(R14),R10
add.l R10,R10
add.l R10,R9
move -14(R8,R9.l),R0
move R7,R1
add #2,R1
move -2(R14),R2
add #3,R2
muls R2,R1
cmp R1,R0
beq L156
*line 306
move #99,(sp)
jsr _error
L156:L153:
*line 305
add #1,-2(R14)
L154:
*line 305
cmp #3,-2(R14)
blt L155
L152:L149:
*line 304
add #1,R7
L150:
*line 304
cmp #2,R7
blt L151
L148:
*line 309
jsr _swt1
cmp.l #1,R0
beq L157
*line 309
move #100,(sp)
jsr _error
L157:
*line 310
jsr _swt2
tst.l R0
beq L158
*line 310
move #101,(sp)
jsr _error
L158:
*line 311
jsr _swt3
cmp.l #-1,R0
beq L159
*line 311
move #101,(sp)
jsr _error
L159:
*line 312
jsr _swt4
cmp.l #4,R0
beq L160
*line 312
move #102,(sp)
jsr _error
L160:
*line 314
move.l #20000,_l1
*line 315
move.l #10,_l2
*line 316
move.l _l2,(sp)
move.l _l1,-(sp)
jsr _ldiv
addq.l #4,sp
move.l R0,_l3
*line 317
cmp.l #2000,_l3
beq L161
*line 317
move #513,(sp)
jsr _error
L161:
*line 318
tst.l _ldivr
beq L162
*line 318
move #514,(sp)
jsr _error
L162:
*line 319
move.l #9999,_l2
*line 320
move.l _l2,(sp)
move.l _l1,-(sp)
jsr _ldiv
addq.l #4,sp
cmp.l #2,R0
beq L163
*line 320
move #515,(sp)
jsr _error
L163:
*line 321
cmp.l #2,_ldivr
beq L164
*line 321
move #516,(sp)
jsr _error
L164:
*line 322
move.l #-123,_l2
*line 323
move.l _l2,(sp)
move.l _l1,-(sp)
jsr _ldiv
addq.l #4,sp
cmp.l #-162,R0
beq L165
*line 323
move #519,(sp)
jsr _error
L165:
*line 324
cmp.l #-74,_ldivr
beq L166
*line 324
move #520,(sp)
jsr _error
L166:L167:
*line 326
move #-32768,R6
*line 327
move #4096,R5
*line 328
cmp R5,R6
bcc L168
*line 328
move #521,(sp)
jsr _error
L168:
*line 329
move #1,_uu
*line 330
move.l _uu,R0
cmp.l #$10000,R0
beq L169
*line 330
move #528,(sp)
jsr _error
L169:
*line 332
move #10,R7
*line 336
cmp #10,R7
beq L170
*line 336
move #529,(sp)
jsr _error
L170:
*line 337
clr R7
*line 347
tst R7
beq L171
*line 347
move #530,(sp)
jsr _error
L171:
*line 349
bra L172
*line 349
move #531,(sp)
jsr _error
L172:
*line 350
move #14,2+_zz
*line 351
move.b #127,8+_zz
*line 352
move.l #$30d3f,20+_zz
*line 353
cmp #14,2+_zz
beq L173
*line 353
move #532,(sp)
jsr _error
L173:
*line 354
cmp.b #127,8+_zz
beq L174
*line 354
move #533,(sp)
jsr _error
L174:
*line 355
move.l 20+_zz,R0
cmp.l #$30d3f,R0
beq L175
*line 355
move #534,(sp)
jsr _error
L175:
*line 357
move #1,R7
*line 359
add #1,R7
*line 363
add #1,R7
*line 368
cmp #3,R7
beq L176
*line 368
move #536,(sp)
jsr _error
L176:
*line 376
or #1,_flags
*line 377
and #-7,_flags
*line 378
move _flags,R0
and #-57,R0
or #40,R0
move R0,_flags
*line 379
btst #0,1+_flags
bne L177
*line 379
move #545,(sp)
jsr _error
L177:
*line 380
move _flags,R0
lsr #1,R0
and #3,R0
beq L178
*line 380
move #546,(sp)
jsr _error
L178:
*line 381
move _flags,R0
lsr #3,R0
and #7,R0
cmp #5,R0
beq L179
*line 381
move #547,(sp)
jsr _error
L179:
*line 382
move _flags,R0
and #-57,R0
move _flags,R1
lsr #3,R1
and #7,R1
add #2,R1
and #7,R1
asl #3,R1
or R1,R0
move R0,_flags
*line 383
move _flags,R0
lsr #3,R0
and #7,R0
cmp #7,R0
beq L180
*line 383
move #548,(sp)
jsr _error
L180:
*line 384
move.l #_flags,R13
*line 385
and #-2,(R13)
*line 386
move (R13),R0
and #-7,R0
or #2,R0
move R0,(R13)
*line 387
move (R13),R0
and #-57,R0
or #24,R0
move R0,(R13)
*line 388
btst #0,1(R13)
beq L181
*line 388
move #8778,(sp)
jsr _error
L181:
*line 389
move (R13),R0
lsr #1,R0
and #3,R0
cmp #1,R0
beq L182
*line 389
move #8779,(sp)
jsr _error
L182:
*line 390
move (R13),R0
lsr #3,R0
and #7,R0
cmp #3,R0
beq L183
*line 390
move #8780,(sp)
jsr _error
L183:
*line 391
move.l #_flags,_p0
*line 392
move.l _p0,R8
or #1,(R8)
*line 393
move.l _p0,R8
move (R8),R0
and #-7,R0
or #4,R0
move.l _p0,R9
move R0,(R9)
*line 394
move.l _p0,R8
or #56,(R8)
*line 395
move.l _p0,R8
btst #0,1(R8)
bne L184
*line 395
move #8781,(sp)
jsr _error
L184:
*line 396
move.l _p0,R8
move (R8),R0
lsr #1,R0
and #3,R0
cmp #2,R0
beq L185
*line 396
move #8782,(sp)
jsr _error
L185:
*line 397
move.l _p0,R8
move (R8),R0
lsr #3,R0
and #7,R0
cmp #7,R0
beq L186
*line 397
move #8783,(sp)
jsr _error
L186:
*line 398
move #13,R7
*line 399
move R7,R0
add #5,R0
ext.l R0
move.l R0,(sp)
jsr _checklv
*line 400
jsr _checkreg
*line 401
jsr _chkinits
*line 402
bra L187
*line 402
move #549,(sp)
jsr _error
L187:
*line 403
move #-12,-2(R14)
*line 404
move -2(R14),R8
move.l R8,(sp)
jsr _chklext
*line 405
move.l #$ea60,R0
move R0,R6
*line 406
move R6,R5
clr.l R0
move R5,R0
divu #20,R0
move R0,R5
*line 407
cmp #3000,R5
beq L188
*line 407
move #550,(sp)
jsr _error
L188:
*line 408
move.l #$d753,_l1
*line 409
clr.l R0
move R6,R0
add.l _l1,R0
move.l R0,_l1
*line 410
move.l _l1,R0
cmp.l #$1c1b3,R0
beq L189
*line 410
move #551,(sp)
jsr _error
L189:
*line 411
clr.l R0
move R6,R0
sub.l R0,_l1
*line 412
move.l _l1,R0
cmp.l #$d753,R0
beq L190
*line 412
move #552,(sp)
jsr _error
L190:
*line 413
move.l #$1869f,12+_al
*line 414
move #3,R6
*line 415
move R6,R0
lsl #2,R0
swap R0
clr R0
swap R0
move.l R0,R8
move.l _al(R8),R0
cmp.l #$1869f,R0
beq L191
*line 415
move #553,(sp)
jsr _error
L191:
*line 416
move.l #$2bf20,_l1
*line 417
move.l #$ea60,R0
move R0,R6
*line 418
clr.l R0
move R6,R0
move.l R0,-(sp)
move.l _l1,-(sp)
jsr ldiv
addq.l #8,sp
move R0,R5
*line 419
cmp #3,R5
beq L192
*line 419
move #560,(sp)
jsr _error
L192:
*line 420
move #12000,R5
*line 421
move #5,-18(R14)
*line 422
move R5,R6
mulu -18(R14),R6
*line 423
clr.l R0
move R6,R0
cmp.l #$ea60,R0
beq L193
*line 423
move #561,(sp)
jsr _error
L193:
*line 424
move.l #5,_l1
*line 425
move.l #$ea60,R0
move R0,R6
*line 426
move.l _l1,-(sp)
clr.l R0
move R6,R0
move.l R0,-(sp)
jsr lmul
addq.l #8,sp
move.l R0,_l1
*line 427
move.l _l1,R0
cmp.l #$493e0,R0
beq L194
*line 427
move #562,(sp)
jsr _error
L194:
*line 430
move.b #100,-16(R14)
*line 431
move.b -16(R14),R7
ext.w R7
asl #3,R7
add #2,R7
*line 432
move.b -16(R14),R0
ext.w R0
asl #4,R0
sub #2,R0
move R0,-2(R14)
*line 433
cmp #802,R7
beq L195
*line 433
move #576,(sp)
jsr _error
L195:
*line 434
cmp #1598,-2(R14)
beq L196
*line 434
move #577,(sp)
jsr _error
L196:
*line 435
move #50,R7
*line 436
move.b -16(R14),R0
ext.w R0
cmp R0,R7
blt L197
*line 436
move #578,(sp)
jsr _error
L197:
*line 437
move #100,-2(R14)
*line 438
move.b -16(R14),R0
ext.w R0
cmp -2(R14),R0
beq L198
*line 438
move #579,(sp)
jsr _error
L198:
*line 439
lea -2(R14),R8
move.l R8,-22(R14)
*line 440
move.l -22(R14),R8
move (R8),R0
move.b -16(R14),R1
ext.w R1
cmp R1,R0
beq L199
*line 440
move #9274,(sp)
jsr _error
L199:
*line 441
move.l -22(R14),R8
move (R8),R0
cmp -2(R14),R0
beq L200
*line 441
move #9275,(sp)
jsr _error
L200:
*line 442
move.l #100,L3
*line 443
move.b -16(R14),R0
ext.w R0
ext.l R0
move.l L3,R1
cmp.l R1,R0
beq L201
*line 443
move #580,(sp)
jsr _error
L201:
*line 444
move.b #255,-16(R14)
*line 445
move.b -16(R14),R0
ext.w R0
ext.l R0
move.l L3,R1
cmp.l R1,R0
bgt L10004
move.b -16(R14),R0
ext.w R0
cmp -2(R14),R0
bge L10004
move.b -16(R14),R0
ext.w R0
cmp R0,R7
bge L202
L10004:*line 445
move #597,(sp)
jsr _error
L202:
*line 446
move.b -16(R14),R7
ext.w R7
add #500,R7
*line 447
cmp #499,R7
beq L203
*line 447
move #598,(sp)
jsr _error
L203:
*line 448
move.b -16(R14),R0
ext.w R0
sub #4,R0
cmp #-5,R0
beq L204
*line 448
move #599,(sp)
jsr _error
L204:
*line 449
move #1,R7
move #2,R7
move #3,R7
*line 449
cmp #3,R7
beq L205
*line 449
move #600,(sp)
jsr _error
L205:
*line 450
move #3,R7
move #2,-2(R14)
move #1,R7
move R7,R0
cmp #1,R0
bne L10005
cmp #1,R7
bne L10005
cmp #2,-2(R14)
beq L206
L10005:*line 450
move #601,(sp)
jsr _error
L206:
*line 451
cmp #1,R7
move sr,R0
add #1,R7
move R0,ccr
bne L10006
cmp #2,R7
move sr,R0
sub #1,R7
move R0,ccr
bne L10006
cmp #1,R7
beq L207
L10006:*line 451
move #608,(sp)
jsr _error
L207:
*line 452
move R7,R0
add -2(R14),R0
cmp #3,R0
move sr,R0
add #1,-2(R14)
add #1,R7
move R0,ccr
bne L10007
cmp #2,R7
bne L10007
cmp #3,-2(R14)
beq L208
L10007:*line 452
move #609,(sp)
jsr _error
L208:
*line 454
jsr _ckcast
*line 456
move.l #5,R13
move.l R13,R0
beq L209
bra L210
L209:
*line 456
move #769,(sp)
jsr _error
L210:
*line 457
sub.l #5,R13
move.l R13,R0
beq L211
*line 457
move #770,(sp)
jsr _error
L211:
*line 458
add.l #1,R13
move.l R13,R0
and.l #1,R0
bne L212
*line 458
move #771,(sp)
jsr _error
L212:
*line 459
clr.l _p0
*line 460
move.l _p0,R13
move.l R13,R0
beq L213
*line 460
move #772,(sp)
jsr _error
L213:
*line 461
move.l #_p0,_p0
*line 462
move.l _p0,R13
move.l R13,R0
sub.l #1,_p0
tst.l R0
beq L214
bra L215
L214:
*line 462
move #773,(sp)
jsr _error
L215:
*line 463
move.l 4+_ai,R13
add.l 28+_zz,R13
*line 464
move.l #16+_zz,R13
*line 465
move #2,18+_zz
*line 466
btst #0,3(R13)
beq L216
*line 466
move #774,(sp)
jsr _error
L216:
*line 467
tst 2(R13)
beq L10008
clr R0
bra L10009
L10008:move #1,R0
L10009:and #2,R0
beq L217
*line 467
move #775,(sp)
jsr _error
L217:
*line 468
move.l #56,R13
*line 469
move.l R13,R0
and.l #1,R0
beq L218
*line 469
move #776,(sp)
jsr _error
L218:
*line 470
move.l R13,R0
and.l #32,R0
bne L219
*line 470
move #777,(sp)
jsr _error
L219:
*line 471
move #105,-2(R14)
*line 472
move.l #105,R13
*line 473
cmp -2(R14),R13
beq L220
*line 473
move #800,(sp)
jsr _error
L220:
*line 474
move #-30875,6+_ai
*line 475
move #1,-2(R14)
*line 476
move -2(R14),R8
add.l R8,R8
add.l R8,R8
move.l _pai(R8),R8
move (R8),R7
*line 477
cmp #-30875,R7
beq L221
*line 477
move #801,(sp)
jsr _error
L221:
*line 478
move.l #1000,R13
*line 479
move.l #_p0,-26(R14)
*line 480
lea 1(R13),R8
move.l R8,-22(R14)
move.l R8,R0
move.l -26(R14),R9
move.l R0,(R9)
*line 481
cmp.l #1001,_p0
beq L222
*line 481
move #802,(sp)
jsr _error
L222:
*line 482
jsr _chkptrs
*line 483
jsr _tembedas
*line 484
jsr _macrotes
*line 485
jsr _lextest
*line 486
jsr _cmplxptr
*line 488
move.b #100,-16(R14)
*line 489
move #1024,R7
*line 490
move.l #31234,L3
*line 491
move.b #90,_c
*line 493
move.b _c,R0
ext.w R0
move R0,(sp)
move.l #L1,-(sp)
move.l L3,-(sp)
move R7,-(sp)
move R7,-(sp)
move.b -16(R14),R0
ext.w R0
move R0,-(sp)
move.b -16(R14),R0
ext.w R0
move R0,-(sp)
move.l #L223,-(sp)
jsr _printf
adda.l #20,sp
*line 494
move.l #$a9b7c3,L3
*line 495
move #-3612,R7
*line 496
move R7,(sp)
move.l L3,-(sp)
move.l #L224,-(sp)
jsr _printf
addq.l #8,sp
*line 497
jsr _v33
*line 498
jsr _v34
*line 500
move.l #L225,(sp)
jsr _putstrin
L8:tst.l (sp)+
movem.l (sp)+,R4-R7/R13-R13
unlk R14
rts
.globl _v33
.text
_v33:
~~v33:
~i=-4
~l=L3
~bb1=-18
~bb2=-30
~str=L1
link R14,#-34
*line 509
lea -18(R14),R8
move.l R8,-26(R14)
*line 510
move #23,-14(R14)
*line 511
move.l -26(R14),R8
move 4(R8),(sp)
move -14(R14),-(sp)
move.l #L227,-(sp)
jsr _printf
addq.l #6,sp
*line 512
move #99,2+_x
*line 513
move.l 2+_sound,R8
move (R8),(sp)
move 2+_x,-(sp)
move.l #L228,-(sp)
jsr _printf
addq.l #6,sp
*line 514
move.l #$111111,-4(R14)
*line 515
move.l -4(R14),(sp)
move.l #L229,-(sp)
jsr _printf
addq.l #4,sp
L226:unlk R14
rts
.globl _v34
.text
_v34:
~~v34:
~i=-34
~l=L3
~q=-4
~s=-60
~d1=-38
~d2=-42
~f1=-28
~f2=-32
~p3=R13
~p4=-12
~p5=-20
~ar34=-24
~ptr=-50
~str=L1
~sop1=-46
link R14,#-60
movem.l R7-R7/R13-R13,-(sp)
*line 530
move.l -4(R14),R0
and.l #7,R0
beq L231
L231:bra L232
*line 533
move #3584,(sp)
jsr _error
L232:
*line 535
bra L233
*line 535
move #3585,(sp)
jsr _error
L233:
*line 537
move.l #$100000,L3
*line 538
move #1,-34(R14)
*line 539
move -34(R14),R1
move.l L3,R0
asl.l R1,R0
move.l R0,L3
*line 540
move.l L3,R0
cmp.l #$200000,R0
beq L234
*line 540
move #3586,(sp)
jsr _error
L234:
*line 541
move.l #16,L3
*line 542
move -34(R14),R1
move.l L3,R0
asl.l R1,R0
move.l R0,L3
*line 543
cmp.l #32,L3
beq L235
*line 543
move #3587,(sp)
jsr _error
L235:
*line 544
move.l #$1,_l1
*line 545
move.l #$1,_l2
*line 546
move #5,-34(R14)
*line 547
move -34(R14),R1
move.l _l1,R0
asl.l R1,R0
move.l R0,_l1
*line 548
move.l _l2,R0
move -34(R14),R1
asl.l R1,R0
move.l R0,_l2
*line 549
move.l _l1,R0
cmp.l _l2,R0
beq L236
*line 549
move #3588,(sp)
jsr _error
L236:
*line 551
jsr _f
move.l R0,R13
cmp.l #-1,R13
beq L237
*line 551
move #3589,(sp)
jsr _error
L237:
*line 553
move.b #112,-12(R14)
*line 554
lea -20(R14),R13
*line 555
move.l R14,R9
add.l #-12,R9
move.l R13,R8
move #3,r0
L10010:move (r9)+,(r8)+
dbra r0,L10010
*line 556
move.b (R13),R0
ext.w R0
cmp.b -12(R14),R0
beq L238
*line 556
move #3600,(sp)
jsr _error
L238:
*line 557
move.b #69,-12(R14)
*line 558
move.l R14,R9
add.l #-12,R9
move.l R14,R8
add.l #-20,R8
move #3,r0
L10011:move (r9)+,(r8)+
dbra r0,L10011
*line 559
move.b -20(R14),R0
ext.w R0
cmp.b -12(R14),R0
beq L239
*line 559
move #3601,(sp)
jsr _error
L239:
*line 560
move.b #55,(R13)
*line 561
move.l R13,R9
move.l R14,R8
add.l #-12,R8
move #3,r0
L10012:move (r9)+,(r8)+
dbra r0,L10012
*line 562
move.b -12(R14),R0
ext.w R0
cmp.b (R13),R0
beq L240
*line 562
move #3602,(sp)
jsr _error
L240:L230:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _f
.text
_f:
~~f:
~l=L3
~str=L1
link R14,#-4
*line 568
move #-1,R0
bra L241
L241:unlk R14
rts
.globl _chklext
.text
_chklext:
~~chklext:
~l=L3
~al9=8
~str=L1
link R14,#-4
*line 575
cmp.l #$fffffff4,8(R14)
beq L243
*line 575
move #885,(sp)
jsr _error
L243:L242:unlk R14
rts
.globl _chkinits
.text
_chkinits:
~~chkinits:
.data
L245:.dc.w 15,16959
.text
.data
L246:
*line 586
.dc.l L247
.dc.w 1
.dc.w 3,-191
*line 586
.dc.l L248
.dc.w 2
.dc.w 0,143
*line 586
.dc.l L249
.dc.w 3
.ds.b 4
.text
~i=R7
~l=L3
~ss=L246
~str=L1
~liv1=L245
link R14,#0
movem.l R6-R7,-(sp)
*line 589
move.l L245,R0
cmp.l #$f423f,R0
beq L250
*line 589
move #4081,(sp)
jsr _error
L250:
*line 590
cmp #2,14+L246
beq L251
*line 590
move #4082,(sp)
jsr _error
L251:
*line 591
move.l #L252,(sp)
move.l 20+L246,-(sp)
jsr _eqstr
addq.l #4,sp
tst R0
bne L253
*line 591
move #4083,(sp)
jsr _error
L253:
*line 592
move.l 6+L246,R0
cmp.l #$3ff41,R0
beq L254
*line 592
move #4084,(sp)
jsr _error
L254:
*line 593
cmp.l #$8f,16+L246
beq L255
*line 593
move #4085,(sp)
jsr _error
L255:
*line 594
tst.l 26+L246
beq L256
*line 594
move #4086,(sp)
jsr _error
L256:
*line 595
btst #7,19+L246
beq L257
bra L258
L257:
*line 595
move #4087,(sp)
jsr _error
L258:
*line 596
cmp #1,_initi
beq L259
*line 596
move #4088,(sp)
jsr _error
L259:
*line 597
clr R7
bra L262
L263:
*line 598
move R7,R8
add.l R8,R8
move _initk(R8),R0
move R7,R1
add #1,R1
cmp R1,R0
beq L264
*line 598
move #4089,(sp)
jsr _error
L264:
*line 599
move R7,R0
asl #3,R0
ext.l R0
move.l R0,R8
move _initl(R8),R0
move R7,R1
asl #2,R1
add #1,R1
cmp R1,R0
beq L265
*line 599
move #-240,(sp)
jsr _error
L265:
*line 600
move R7,R0
muls #6,R0
move.l R0,R8
cmp _inits(R8),R7
beq L266
*line 600
move #-239,(sp)
jsr _error
L266:
*line 601
move R7,R0
muls #6,R0
move.l R0,R8
move.l 2+_inits(R8),R0
move R7,R8
cmp.l R8,R0
beq L267
*line 601
move #-238,(sp)
jsr _error
L267:
*line 602
move R7,R0
muls #3,R0
move.l R0,R8
move.b _inite(R8),R0
ext.w R0
move R7,R1
muls #3,R1
add #1,R1
cmp R1,R0
beq L268
*line 602
move #-237,(sp)
jsr _error
L268:L261:
*line 597
add #1,R7
L262:
*line 597
cmp #3,R7
blt L263
L260:
*line 604
move.l #L269,(sp)
move.l #_initc,-(sp)
jsr _eqstr
addq.l #4,sp
tst R0
bne L270
*line 604
move #-236,(sp)
jsr _error
L270:
*line 605
move.l #L271,(sp)
move.l #_initd,-(sp)
jsr _eqstr
addq.l #4,sp
tst R0
bne L272
*line 605
move #-235,(sp)
jsr _error
L272:
*line 606
move.l #L273,(sp)
move.l _initcc,-(sp)
jsr _eqstr
addq.l #4,sp
tst R0
bne L274
*line 606
move #-234,(sp)
jsr _error
L274:
*line 607
bra L275
*line 607
move #-233,(sp)
jsr _error
L275:
*line 608
bra L276
*line 608
move #-232,(sp)
jsr _error
L276:
*line 609
bra L277
*line 609
move #-231,(sp)
jsr _error
L277:
*line 610
cmp #258,_ccinit
beq L278
*line 610
move #-224,(sp)
jsr _error
L278:L244:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.globl _checkreg
.text
_checkreg:
~~checkreg:
~l=L3
~rx1=R7
~rx2=R6
~rx3=R5
~rx4=R4
~rx5=R3
~rx6=-2
~rx7=-4
~rx8=-6
~rx9=-8
~str=L1
link R14,#-8
movem.l R2-R7,-(sp)
*line 618
move #1,R7
*line 619
move #2,R6
*line 620
move #5,R3
*line 621
move #8,-6(R14)
*line 622
move #9,-8(R14)
*line 623
move R7,R0
add R6,R0
cmp #3,R0
beq L280
*line 623
move #864,(sp)
jsr _error
L280:
*line 624
move R6,R0
add R3,R0
cmp #7,R0
beq L281
*line 624
move #865,(sp)
jsr _error
L281:
*line 625
move -8(R14),R0
sub R3,R0
cmp #4,R0
beq L282
*line 625
move #866,(sp)
jsr _error
L282:
*line 626
move -6(R14),R0
add -8(R14),R0
cmp #17,R0
beq L283
*line 626
move #867,(sp)
jsr _error
L283:L279:tst.l (sp)+
movem.l (sp)+,R3-R7
unlk R14
rts
.globl _checklv
.text
_checklv:
~~checklv:
~l=L3
~alv=8
~str=L1
link R14,#-4
*line 632
cmp.l #$12,8(R14)
beq L285
*line 632
move #13902,(sp)
jsr _error
L285:L284:unlk R14
rts
.globl _error
.text
_error:
~~error:
~l=L3
~pc=8
~str=L1
link R14,#-4
*line 637
move 8(R14),(sp)
move.l #L287,-(sp)
jsr _printf
addq.l #4,sp
*line 639
jsr _exit
L286:unlk R14
rts
.globl _putstrin
.text
_putstrin:
~~putstrin:
~l=L3
~p=R13
~pc=8
~str=L1
link R14,#0
movem.l R7-R7/R13-R13,-(sp)
*line 648
move.l 8(R14),R13
bra L291
L290:
*line 650
move.b (R13)+,R0
ext.w R0
move R0,(sp)
jsr _putchar
L291:
*line 650
tst.b (R13)
bne L290
L289:
*line 651
move #10,(sp)
jsr _putchar
L288:tst.l (sp)+
movem.l (sp)+,R13-R13
unlk R14
rts
.globl _targs
.text
_targs:
~~targs:
~l=L3
~a1=9
~a2=10
~a3=12
~str=L1
link R14,#-4
*line 657
cmp.b #1,9(R14)
bne L10013
cmp #1000,10(R14)
bne L10013
cmp #-18363,12(R14)
beq L293
L10013:*line 657
move #-512,(sp)
jsr _error
L293:
*line 658
move.l #$9c40,R0
cmp.l #$9c40,R0
beq L294
*line 658
move #-511,(sp)
jsr _error
L294:L292:unlk R14
rts
.globl _addargs
.text
_addargs:
~~addargs:
~l=L3
~a1=8
~a2=10
~a3=12
~str=L1
link R14,#-4
*line 664
move 8(R14),R0
add 10(R14),R0
add 12(R14),R0
bra L295
L295:unlk R14
rts
.globl _eqstr
.text
_eqstr:
~~eqstr:
~l=L3
~p1=R13
~p2=R12
~ap1=8
~ap2=12
~str=L1
link R14,#0
movem.l R7-R7/R12-R13,-(sp)
*line 672
move.l 8(R14),R13
*line 673
move.l 12(R14),R12
bra L299
L298:
*line 676
cmpm.b (R13)+,(R12)+
beq L300
*line 676
clr R0
bra L296
L300:L299:
*line 677
tst.b (R13)
bne L298
L297:
*line 679
tst.b (R12)
beq L301
*line 679
clr R0
bra L296
L301:
*line 680
move #1,R0
bra L296
L296:tst.l (sp)+
movem.l (sp)+,R12-R13
unlk R14
rts
.globl _addlargs
.text
_addlargs:
~~addlargs:
~i=R7
~j=R6
~k=R5
~l=L3
~a1=8
~a2=12
~lr2=R4
~str=L1
link R14,#0
movem.l R3-R7,-(sp)
*line 689
move #1000,R7
*line 690
move.l #$bbbbbbbb,R4
*line 691
move #-18363,(sp)
move R7,-(sp)
move #1,-(sp)
jsr _targs
addq.l #4,sp
*line 692
cmp.l #$bbbbbbbb,R4
beq L303
*line 692
move #-510,(sp)
jsr _error
L303:
*line 694
move.l 8(R14),R0
add.l 12(R14),R0
bra L302
L302:tst.l (sp)+
movem.l (sp)+,R4-R7
unlk R14
rts
.globl _addpargs
.text
_addpargs:
~~addpargs:
~l=L3
~a1=R13
~a2=R12
~str=L1
link R14,#0
movem.l R7-R7/R12-R13,-(sp)
*line 701
move.l 8(R14),R13
*line 701
move.l 12(R14),R12
*line 701
move.l (R13),R0
add.l (R12),R0
bra L304
L304:tst.l (sp)+
movem.l (sp)+,R12-R13
unlk R14
rts
.globl _swt1
.text
_swt1:
~~swt1:
~i=R7
~l=L3
~str=L1
link R14,#0
movem.l R6-R7,-(sp)
*line 709
move #-1,R7
bra L308
L309:
*line 710
move R7,R0
bra L311
*line 712
move #320,(sp)
jsr _error
L312:
*line 715
cmp #-1,R7
beq L313
*line 715
move #321,(sp)
jsr _error
L313:bra L307
L314:
*line 718
tst R7
beq L315
*line 718
move #322,(sp)
jsr _error
L315:bra L307
L316:
*line 721
cmp #1,R7
beq L317
*line 721
move #323,(sp)
jsr _error
L317:bra L307
L318:
*line 724
cmp #4,R7
beq L319
*line 724
move #324,(sp)
jsr _error
L319:bra L307
L320:
*line 727
cmp #2,R7
beq L321
*line 727
move #325,(sp)
jsr _error
L321:bra L307
L322:
*line 730
cmp #3,R7
beq L323
*line 730
move #326,(sp)
jsr _error
L323:bra L307
L324:
*line 733
cmp #5,R7
beq L325
*line 733
move #327,(sp)
jsr _error
L325:bra L307
L326:bra L310
bra L310
L311:sub #-1,R0
cmp #7,R0
bhi L324
asl #2,R0
move R0,R8
add.l #L327,R8
move.l (R8),R8
jmp (R8)
.data
L327:.dc.l L312
.dc.l L314
.dc.l L316
.dc.l L320
.dc.l L322
.dc.l L318
.dc.l L324
.dc.l L326
.text
L310:bra L306
L307:
*line 709
add #1,R7
L308:
*line 709
cmp #10,R7
blt L309
L306:
*line 741
cmp #6,R7
beq L328
*line 741
move #328,(sp)
jsr _error
L328:
*line 742
move.l #1,R0
bra L305
L305:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.globl _swt2
.text
_swt2:
~~swt2:
~i=R7
~l=L3
~str=L1
link R14,#0
movem.l R6-R7,-(sp)
*line 749
clr R7
bra L332
L333:
*line 750
move R7,R0
bra L335
*line 752
move #336,(sp)
jsr _error
L336:bra L334
L337:
*line 757
cmp #10,R7
beq L338
*line 757
move #337,(sp)
jsr _error
L338:bra L331
L339:
*line 760
tst R7
beq L340
*line 760
move #343,(sp)
jsr _error
L340:bra L331
L341:
*line 763
cmp #20,R7
beq L342
*line 763
move #338,(sp)
jsr _error
L342:bra L331
L343:
*line 766
cmp #40,R7
beq L344
*line 766
move #339,(sp)
jsr _error
L344:bra L331
L345:
*line 769
cmp #50,R7
beq L346
*line 769
move #340,(sp)
jsr _error
L346:bra L331
L347:
*line 772
cmp #30,R7
beq L348
*line 772
move #341,(sp)
jsr _error
L348:bra L331
bra L334
L335:ext.l R0
move.l #L349,R8
move #6,R1
L350:cmp.l (R8)+,R0
dbeq R1,L350
move.l 24(R8),R8
jmp (R8)
.data
L349:.dc.l 0
.dc.l 10
.dc.l 20
.dc.l 40
.dc.l 50
.dc.l 60
.dc.l 0
.dc.l L339
.dc.l L337
.dc.l L341
.dc.l L343
.dc.l L345
.dc.l L336
.dc.l L347
.text
L334:bra L330
L331:
*line 749
add #10,R7
L332:
*line 749
cmp #100,R7
blt L333
L330:
*line 778
cmp #60,R7
beq L351
*line 778
move #342,(sp)
jsr _error
L351:
*line 779
clr.l R0
bra L329
L329:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.globl _swt3
.text
_swt3:
~~swt3:
~i=R7
~j=R6
~l=L3
~str=L1
link R14,#0
movem.l R5-R7,-(sp)
*line 786
clr R7
bra L355
L356:
*line 787
move #1,R6
clr.l R0
move R7,R0
asl R0,R6
move R6,R0
bra L358
*line 789
move #352,(sp)
jsr _error
L359:
*line 792
cmp #8,R6
beq L360
*line 792
move #353,(sp)
jsr _error
L360:bra L354
L361:bra L357
L362:
*line 797
cmp #4,R6
beq L363
*line 797
move #354,(sp)
jsr _error
L363:bra L354
L364:
*line 800
cmp #2,R6
beq L365
*line 800
move #355,(sp)
jsr _error
L365:bra L354
L366:
*line 803
cmp #1,R6
beq L367
*line 803
move #356,(sp)
jsr _error
L367:bra L354
L368:
*line 806
cmp #16,R6
beq L369
*line 806
move #357,(sp)
jsr _error
L369:bra L354
bra L357
L358:ext.l R0
move.l #L370,R8
move #5,R1
L371:cmp.l (R8)+,R0
dbeq R1,L371
move.l 20(R8),R8
jmp (R8)
.data
L370:.dc.l 1
.dc.l 2
.dc.l 4
.dc.l 8
.dc.l 32
.dc.l 0
.dc.l L366
.dc.l L364
.dc.l L362
.dc.l L359
.dc.l L361
.dc.l L368
.text
L357:bra L353
L354:
*line 786
add #1,R7
L355:
*line 786
cmp #10,R7
blt L356
L353:
*line 812
cmp #32,R6
beq L372
*line 812
move #358,(sp)
jsr _error
L372:
*line 813
move.l #-1,R0
bra L352
L352:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _swt4
.text
_swt4:
~~swt4:
~i=R7
~j=R6
~l=L3
~str=L1
link R14,#0
movem.l R5-R7,-(sp)
*line 820
clr R7
bra L376
L377:
*line 821
move #1,R6
clr.l R0
move R7,R0
asl R0,R6
move R6,R0
bra L379
*line 823
move #368,(sp)
jsr _error
L380:bra L378
L381:
*line 828
cmp #2,R6
beq L382
*line 828
move #371,(sp)
jsr _error
L382:bra L375
L383:
*line 831
cmp #1,R6
beq L384
*line 831
move #372,(sp)
jsr _error
L384:bra L375
L385:bra L378
bra L378
L379:cmp #1,R0
beq L383
cmp #2,R0
beq L381
cmp #4,R0
beq L380
bra L385
L378:bra L374
L375:
*line 820
add #1,R7
L376:
*line 820
cmp #10,R7
blt L377
L374:
*line 839
cmp #4,R6
beq L386
*line 839
move #374,(sp)
jsr _error
L386:
*line 840
move R6,R0
ext.l R0
bra L373
L373:tst.l (sp)+
movem.l (sp)+,R6-R7
unlk R14
rts
.globl _ckcast
.text
_ckcast:
~~ckcast:
~arypt=-22
~l=L3
~str=L1
~cst19=-18
link R14,#-26
*line 851
lea -18(R14),R8
move.l R8,-22(R14)
*line 852
move.l -22(R14),R8
move.l #6,(R8)
*line 853
move.l -22(R14),R8
move.l (R8),R0
add.l R0,-22(R14)
*line 854
move.l R14,R0
add.l #-12,R0
cmp.l -22(R14),R0
beq L388
*line 854
move #-509,(sp)
jsr _error
L388:L387:unlk R14
rts
.globl _chkptrs
.text
_chkptrs:
~~chkptrs:
~l=L3
~pc=R13
~pi=R12
~pl=R11
~apc=-4
~api=-8
~apl=-12
~str=L1
link R14,#-12
movem.l R7-R7/R11-R13,-(sp)
*line 867
move.l #4+L1,R13
*line 868
cmp.b #110,(R13)+
beq L390
*line 868
move #1024,(sp)
jsr _error
L390:
*line 869
move.l R13,R8
cmp.b #103,(R8)
move sr,R0
sub.l #1,R13
move R0,ccr
beq L391
*line 869
move #1025,(sp)
jsr _error
L391:
*line 870
cmp.b #105,-(R13)
beq L392
*line 870
move #1026,(sp)
jsr _error
L392:
*line 871
cmp.b #49,4(R13)
beq L393
*line 871
move #1027,(sp)
jsr _error
L393:
*line 873
move.l #4+L1,R13
*line 874
cmp.l #4+L1,R13
bls L394
*line 874
move #1040,(sp)
jsr _error
L394:
*line 875
cmp.l #5+L1,R13
blo L395
*line 875
move #1089,(sp)
jsr _error
L395:
*line 876
cmp.l #4+L1,R13
bcc L396
*line 876
move #1090,(sp)
jsr _error
L396:
*line 877
cmp.l #3+L1,R13
bhi L397
*line 877
move #1091,(sp)
jsr _error
L397:
*line 878
cmp.l #3+L1,R13
bne L398
*line 878
move #1092,(sp)
jsr _error
L398:
*line 879
cmp.l #4+L1,R13
beq L399
*line 879
move #1093,(sp)
jsr _error
L399:
*line 881
move.l #4+L1,-4(R14)
*line 882
move.l -4(R14),R8
cmp.b #110,(R8)
move sr,R0
add.l #1,-4(R14)
move R0,ccr
beq L400
*line 882
move #1056,(sp)
jsr _error
L400:
*line 883
move.l -4(R14),R8
cmp.b #103,(R8)
move sr,R0
sub.l #1,-4(R14)
move R0,ccr
beq L401
*line 883
move #1057,(sp)
jsr _error
L401:
*line 884
sub.l #1,-4(R14)
move.l -4(R14),R8
cmp.b #105,(R8)
beq L402
*line 884
move #1058,(sp)
jsr _error
L402:
*line 885
move.l -4(R14),R8
cmp.b #49,4(R8)
beq L403
*line 885
move #1059,(sp)
jsr _error
L403:
*line 887
move.l #4+L1,-4(R14)
*line 888
move.l #4+L1,R0
cmp.l -4(R14),R0
bcc L404
*line 888
move #1072,(sp)
jsr _error
L404:
*line 889
move.l #5+L1,R0
cmp.l -4(R14),R0
bhi L405
*line 889
move #1073,(sp)
jsr _error
L405:
*line 890
move.l #4+L1,R0
cmp.l -4(R14),R0
bls L406
*line 890
move #1074,(sp)
jsr _error
L406:
*line 891
move.l #3+L1,R0
cmp.l -4(R14),R0
blo L407
*line 891
move #1075,(sp)
jsr _error
L407:
*line 892
move.l #3+L1,R0
cmp.l -4(R14),R0
bne L408
*line 892
move #1076,(sp)
jsr _error
L408:
*line 893
move.l #4+L1,R0
cmp.l -4(R14),R0
beq L409
*line 893
move #1077,(sp)
jsr _error
L409:
*line 895
move #14,_ai
*line 896
move #33,2+_ai
*line 897
move #194,4+_ai
*line 898
move #555,6+_ai
*line 899
move.l _pai,R8
cmp #33,(R8)
beq L410
*line 899
move #1028,(sp)
jsr _error
L410:
*line 900
move.l #2+_ai,R12
*line 901
add.l #2,R12
move.l R12,R8
cmp #194,(R8)
beq L411
*line 901
move #1029,(sp)
jsr _error
L411:
*line 902
sub.l #2,R12
*line 903
move (R12),R0
add #5,R0
cmp #38,R0
beq L412
*line 903
move #1030,(sp)
jsr _error
L412:
*line 904
clr _ai
*line 905
tst -(R12)
beq L413
*line 905
move #1031,(sp)
jsr _error
L413:
*line 906
lea 6(R12),R8
move.l R8,-8(R14)
*line 907
move.l -8(R14),R8
cmp #555,(R8)
move sr,R0
add.l #2,-8(R14)
move R0,ccr
beq L414
*line 907
move #1032,(sp)
jsr _error
L414:
*line 909
move.l #_pai,R11
*line 910
add.l #4,R11
move.l (R11),R0
cmp.l #6+_ai,R0
beq L415
*line 910
move #1033,(sp)
jsr _error
L415:
*line 911
jsr _ckstrpt
L389:tst.l (sp)+
movem.l (sp)+,R11-R13
unlk R14
rts
.globl _ckstrpt
.text
_ckstrpt:
~~ckstrpt:
~l=L3
~p=R13
~nn=R7
~lll=R6
~str=L1
link R14,#0
movem.l R5-R7/R13-R13,-(sp)
*line 921
move #1987,10+_zz
*line 922
move.b #255,8+_zz
*line 923
move.b #13,16+_zz
*line 924
move #-5,18+_zz
*line 925
move.l #_zz,R13
*line 926
add.l #8,R13
move.l R13,R8
cmp #1987,2(R8)
beq L417
*line 926
move #1104,(sp)
jsr _error
L417:
*line 927
cmp.b #-1,(R13)
beq L418
*line 927
move #1105,(sp)
jsr _error
L418:
*line 928
cmp #-5,10(R13)
beq L419
*line 928
move #1106,(sp)
jsr _error
L419:
*line 929
add.l #8,R13
*line 930
cmp.b #13,(R13)
beq L420
*line 930
move #1107,(sp)
jsr _error
L420:
*line 931
sub #1,2(R13)
move 2(R13),R0
cmp #-6,R0
beq L421
*line 931
move #1108,(sp)
jsr _error
L421:
*line 932
sub.l #8,R13
move.l R13,R8
cmp #1987,2(R8)
beq L422
*line 932
move #17738,(sp)
jsr _error
L422:
*line 933
move.l #8+_zz,_p0
*line 934
move.l _p0,R8
cmp.b #-1,(R8)
beq L423
*line 934
move #1109,(sp)
jsr _error
L423:
*line 935
move.l _p0,R8
cmp #1987,2(R8)
beq L424
*line 935
move #1110,(sp)
jsr _error
L424:
*line 936
move.l _p0,R8
btst #0,3(R8)
beq L10014
move.l _p0,R8
btst #1,3(R8)
bne L425
L10014:*line 936
move #1111,(sp)
jsr _error
L425:
*line 937
move.l R13,R0
and.l #1,R0
beq L426
*line 937
move #1112,(sp)
jsr _error
L426:
*line 938
move.l #_zz,R13
*line 939
move #2,R7
*line 940
move R7,R0
asl #3,R0
ext.l R0
add.l R0,R13
*line 941
cmp.b #13,(R13)
beq L427
*line 941
move #1113,(sp)
jsr _error
L427:
*line 942
btst #1,R7
beq L428
bra L429
L428:
*line 942
move #1120,(sp)
jsr _error
L429:
*line 943
move #-32768,R7
*line 944
btst #15,R7
beq L430
bra L431
L430:
*line 944
move #1121,(sp)
jsr _error
L431:
*line 945
move.l #$3f001,R6
*line 946
btst #6,R6
beq L432
*line 946
move #1122,(sp)
jsr _error
L432:
*line 947
move.l R6,R0
and.l #3840,R0
beq L433
*line 947
move #1123,(sp)
jsr _error
L433:
*line 948
btst #15,R6
beq L434
bra L435
L434:
*line 948
move #1124,(sp)
jsr _error
L435:
*line 949
move.l R6,R0
and.l #$10000,R0
beq L436
bra L437
L436:
*line 949
move #1125,(sp)
jsr _error
L437:
*line 950
move.l R6,R0
and.l #$f0000,R0
cmp.l #$30000,R0
beq L438
*line 950
move #1126,(sp)
jsr _error
L438:L416:tst.l (sp)+
movem.l (sp)+,R6-R7/R13-R13
unlk R14
rts
.globl _tembedas
.text
_tembedas:
~~tembedas:
~ctype=-4
~l=L3
~p=R13
~t=R6
~c9=-6
~mcc=R7
~str=L1
link R14,#-6
movem.l R5-R7/R13-R13,-(sp)
*line 962
move.b #2,R7
*line 963
move.b #90,-2(R14)
*line 964
lea -4(R14),R13
*line 965
move.b R7,R0
ext.w R0
move.b R0,(R13)+
move.b -4(R14,R0),R0
ext.w R0
move R0,R6
*line 966
cmp #90,R6
beq L440
*line 966
move #-508,(sp)
jsr _error
L440:
*line 967
cmp.b #2,-4(R14)
beq L441
*line 967
move #-507,(sp)
jsr _error
L441:
*line 968
move.b #2,-6(R14)
*line 969
or.b #1,-6(R14)
*line 970
cmp.b #3,-6(R14)
beq L442
*line 970
move #-504,(sp)
jsr _error
L442:
*line 971
and.b #1,-6(R14)
*line 972
cmp.b #1,-6(R14)
beq L443
*line 972
move #-503,(sp)
jsr _error
L443:L439:tst.l (sp)+
movem.l (sp)+,R6-R7/R13-R13
unlk R14
rts
.globl _macrotes
.text
_macrotes:
~~macrotes:
~l=L3
~str=L1
link R14,#-4
L444:unlk R14
rts
.globl _lextest
.text
_lextest:
~~lextest:
~l=L3
~iii=-2
~lll=-6
~str=L1
link R14,#-10
*line 1086
move #-10,-2(R14)
*line 1087
move -2(R14),R0
asl #1,R0
ext.l R0
move.l R0,-6(R14)
*line 1088
cmp.l #$ffffffec,-6(R14)
beq L446
*line 1088
move #-505,(sp)
jsr _error
L446:L445:unlk R14
rts
.globl _cmplxptr
.text
_cmplxptr:
~~cmplxptr:
~i=R7
~l=L3
~v=-2
~dd=-10
~pd=-26
~str=L1
link R14,#-26
movem.l R6-R7,-(sp)
*line 1098
clr -2(R14)
bra L450
L451:
*line 1099
move.l R14,R0
move -2(R14),R1
asl #1,R1
ext.l R1
add.l R1,R0
add.l #-10,R0
move.l R14,R9
move -2(R14),R10
add.l R10,R10
add.l R10,R10
add.l R10,R9
move.l R0,-26(R9)
*line 1100
move -2(R14),R0
add #100,R0
move.l R14,R9
move -2(R14),R10
add.l R10,R10
add.l R10,R9
move R0,-10(R9)
L449:
*line 1098
add #1,-2(R14)
L450:
*line 1098
cmp #4,-2(R14)
blt L451
L448:
*line 1102
move #1,-2(R14)
*line 1103
move -2(R14),R8
add.l R8,R8
add.l R8,R8
move.l -26(R14,R8.l),R8
move (R8),R7
move.l R14,R9
move -2(R14),R10
add.l R10,R10
add.l R10,R10
add.l R10,R9
add.l #2,-26(R9)
*line 1104
cmp #101,R7
beq L452
*line 1104
move #-488,(sp)
jsr _error
L452:
*line 1105
move -2(R14),R8
add.l R8,R8
add.l R8,R8
move.l -26(R14,R8.l),R8
cmp #102,(R8)
beq L453
*line 1105
move #-487,(sp)
jsr _error
L453:L447:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.globl _strlen
.text
_strlen:
~~strlen:
~i=R7
~l=L3
~p=8
~str=L1
link R14,#0
movem.l R6-R7,-(sp)
*line 1113
clr R7
bra L457
L456:
*line 1115
add #1,R7
L457:
*line 1115
move.l 8(R14),R8
move.b (R8),R0
ext.w R0
add.l #1,8(R14)
tst.b R0
bne L456
L455:
*line 1116
move R7,R0
bra L454
L454:tst.l (sp)+
movem.l (sp)+,R7-R7
unlk R14
rts
.data
L4:.dc.b 116,104,97,116,39,115,32,97,108,108,44,32,102,111,108,107,115,46,46,46,0
L7:.dc.b 116,104,105,115,32,105,115,32,97,108,115,111,32,105,116,0
L9:.dc.b 66,101,103,105,110,110,105,110,103,32,54,56,48,48,48,32,67,32,67,111,109,112,105,108,101,114,32,84,101,115,116,0
L66:.dc.b 120,46,99,0
L67:.dc.b 120,46,115,0
L70:.dc.b 97,122,120,57,53,51,35,59,107,57,0
L71:.dc.b 97,122,120,57,53,51,35,59,107,57,0
L73:.dc.b 116,114,121,32,109,101,0
L74:.dc.b 116,114,121,32,109,0
L76:.dc.b 120,46,115,0
L77:.dc.b 120,46,115,115,0
L79:.dc.b 120,46,115,0
L80:.dc.b 120,46,115,115,0
L223:.dc.b 49,48,48,61,37,100,32,36,54,52,61,37,120,32,49,48,50,52,61,37,100,32,48,50,48,48,48,61,37,111,32,51,49,50,51,52,108,61,37,108,100,32,115,116,114,105,110,103,32,49,61,37,115,32,90,61,37,99,10,0
L224:.dc.b 48,120,97,57,98,55,99,51,61,37,108,120,32,48,88,70,49,69,52,61,37,88,10,0
L225:.dc.b 70,97,114,32,70,114,101,97,107,105,110,103,32,79,117,116,33,33,33,33,0
L227:.dc.b 37,100,32,61,32,37,100,32,0
L228:.dc.b 37,100,32,61,32,37,100,32,0
L229:.dc.b 48,120,49,49,49,49,49,49,32,61,32,37,108,120,10,0
L247:.dc.b 97,98,99,0
L248:.dc.b 100,101,102,0
L249:.dc.b 103,104,105,0
L252:.dc.b 103,104,105,0
L269:.dc.b 116,104,105,115,32,105,115,32,105,116,0
L271:.dc.b 116,104,105,115,32,109,105,103,104,116,32,98,101,32,105,116,0
L273:.dc.b 116,104,105,115,32,105,115,32,97,108,115,111,32,105,116,0
L287:.dc.b 101,114,114,111,114,32,37,120,10,0
