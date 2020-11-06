:F1:plm80 setdef.plm pagewidth(132) debug optimize
:F3:link mcd80a.obj,setdef.obj,:F1:plm80.lib to setdef.mod
:F3:locate setdef.mod code(0100H) stacksize(100)
:F3:objhex setdef to setdef.hex
exit
