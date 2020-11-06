:F1:plm80 type.plm pagewidth(100) debug optimize
:F3:link mcd80a.obj,type.obj,parse.obj,:F1:plm80.lib to type.mod
:F3:locate type.mod code(0100H) stacksize(100)
:F3:objhex type to type.hex
exit
