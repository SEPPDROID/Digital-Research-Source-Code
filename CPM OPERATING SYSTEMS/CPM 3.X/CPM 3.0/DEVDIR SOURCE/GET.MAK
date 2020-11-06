:F2:asm80 getf.asm debug
:F1:plm80 get.plm xref pagewidth(100) debug optimize
:F3:link mcd80a.obj,get.obj,parse.obj,getf.obj,:F1:plm80.lib to get.mod
:F3:locate get.mod code(0100H) stacksize(100)
:F3:objhex get to get.hex
exit
