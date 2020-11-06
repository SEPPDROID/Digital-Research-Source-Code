;      transient interface module for 8086
;	JULY, 1980
dgroup	group	dats
dats	segment	public	'DATS'
	db	'?'
dats	ends
cgroup	group	code
	extrn	disem:near
	public	boot
	public	conin,conout
code	segment public 'CODE'
	assume cs:cgroup
disent	proc
	org	1400h
	jmp	disem
disent	endp
boot	proc
	ret
boot	endp
conin	proc
	jmp	$-1301h
conin	endp
conout	proc
	jmp	$-1301h
conout	endp
code	ends
	end
