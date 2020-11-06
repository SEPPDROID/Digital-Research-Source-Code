:F1:plm80 rename.plm pagewidth(100) debug optimize
:F3:link mcd80a.obj,rename.obj,parse.obj,:F1:plm80.lib to rename.mod
:F3:locate rename.mod code(0100H) stacksize(100)
:F3:objhex rename to rename.hex
exit
