;
;	interface module for ddt86
;	6/15/81
;
dgroup	group	dats
dats	segment	public	'DATS'
	db	'?'
dats	ends
;
cgroup	group	code
	extrn	asm:near
	public	ddtgetline
	public	goddt
	public	ddtset
;
code	segment public 'CODE'
	assume cs:cgroup
;
assent	proc
	org	2300h
	jmp	asm
assent	endp
;
ddtgetline	proc
	jmp	$-21fah
ddtgetline	endp
;
goddt	proc
	jmp	$-21fah
goddt	endp
;
ddtset	proc
	jmp	$-21fah
ddtset	endp
;
code	ends
	end
