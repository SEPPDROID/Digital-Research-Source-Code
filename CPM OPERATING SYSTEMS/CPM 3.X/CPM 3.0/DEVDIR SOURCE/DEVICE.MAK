:F1:plm80 device.plm pagewidth(100) debug optimize
:F3:link mcd80a.obj,device.obj,:F1:plm80.lib to device.mod
:F3:locate device.mod code(0100H) stacksize(100)
:F3:objhex device to device.hex
exit
