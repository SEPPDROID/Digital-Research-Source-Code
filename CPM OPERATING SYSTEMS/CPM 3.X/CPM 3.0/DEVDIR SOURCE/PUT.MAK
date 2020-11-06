:F2:asm80 putf.asm debug
:F1:plm80 put.plm xref pagewidth(100) debug optimize
:F3:link mcd80a.obj,put.obj,parse.obj,putf.obj,:F3:plm80.lib to put.mod
:F3:locate put.mod code(0100H) stacksize(100)
:F3:objhex put to put.hex
exit
