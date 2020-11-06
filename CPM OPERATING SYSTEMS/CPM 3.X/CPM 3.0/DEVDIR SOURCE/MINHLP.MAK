:F1:plm80 minhlp.plm debug optimize
:F3:link mcd80a.obj,minhlp.obj,:F1:plm80.lib to minhlp.mod
:F3:locate minhlp.mod code(0100H) stacksize(100)
:F3:objhex minhlp to minhlp.hex
exit
