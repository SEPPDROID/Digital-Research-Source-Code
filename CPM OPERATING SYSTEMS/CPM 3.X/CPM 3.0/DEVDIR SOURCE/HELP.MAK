:F1:plm80 help.plm debug optimize
:F3:link mcd80a.obj,help.obj,:F1:plm80.lib to help.mod
:F3:locate help.mod code(0100H) stacksize(100)
:F3:objhex help to help.hex
exit
