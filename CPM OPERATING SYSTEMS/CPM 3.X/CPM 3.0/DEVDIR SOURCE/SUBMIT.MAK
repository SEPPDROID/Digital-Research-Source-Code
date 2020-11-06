:F2:asm80 getf.asm debug
:F1:plm80 submit.plm xref pagewidth(100) debug optimize
:F3:link mcd80a.obj,submit.obj,parse.obj,getf.obj,:F1:plm80.lib to submit.mod
:F3:locate submit.mod code(0100H) stacksize(100)
:F3:objhex submit to submit.hex
exit
