$title	('PRL Externals')
	name	x0200
	CSEG
offset	equ	0100h

mon1	equ	0005h+offset
mon2	equ	0005h+offset
mon2a	equ	0005h+offset
mon3	equ	0005h+offset
	public	mon1,mon2,mon2a,mon3
fcb	equ	005ch+offset
fcb16	equ	006ch+offset
tbuff	equ	0080h+offset
	public	fcb,fcb16,tbuff
bdisk	equ	0004h+offset
maxb	equ	0006h+offset
buff	equ	0080h+offset
boot	equ	0000h+offset
	public	bdisk,maxb,buff,boot
	END
