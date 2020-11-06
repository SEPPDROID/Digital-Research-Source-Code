:F1:PLM80 set.PLM debug optimize PAGEWIDTH(132)
:F3:link mcd80a.obj,parse.obj,set.obj,:F1:plm80.lib to set.mod 
:F3:locate set.mod code(0100H) stacksize(100) map print(set.tra)
:F3:objhex set to set.hex
exit
