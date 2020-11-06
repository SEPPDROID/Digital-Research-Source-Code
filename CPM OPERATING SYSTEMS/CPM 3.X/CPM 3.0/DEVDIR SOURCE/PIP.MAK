:F2:asm80 inpout.asm debug
:F1:plm80 pip.plm debug optimize
:F3:link mcd80f.obj,inpout.obj,pip.obj,:F1:plm80.lib to pip.mod
:F3:locate pip.mod code(0100H) stacksize(100)
:F3:objhex pip to pip.hex
exit
