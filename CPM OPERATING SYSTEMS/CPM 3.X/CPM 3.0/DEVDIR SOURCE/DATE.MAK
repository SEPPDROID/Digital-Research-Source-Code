:F1:plm80 date.plm pagewidth(100) debug optimize 
:F3:link mcd80a.obj,date.obj,:F1:plm80.lib to date.mod
:F3:locate date.mod code(0100H) stacksize(100)
:F3:objhex date to date.hex
exit
