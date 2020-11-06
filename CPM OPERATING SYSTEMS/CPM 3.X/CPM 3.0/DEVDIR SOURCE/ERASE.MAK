:F1:PLM80 ERASE.PLM debug optimize PAGEWIDTH(132)
:F3:link mcd80a.obj,parse.obj,ERASE.obj,:F1:plm80.lib to erase.mod 
:F3:locate erase.mod code(0100H) stacksize(100) map print(erase.tra)
:F3:objhex erase to erase.hex
exit
