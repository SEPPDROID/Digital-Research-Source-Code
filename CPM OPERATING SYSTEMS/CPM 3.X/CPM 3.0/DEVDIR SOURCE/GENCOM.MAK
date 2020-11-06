:F1:PLM80 GENCOM.PLM debug optimize PAGEWIDTH(132)
:F3:link mcd80a.obj,parse.obj,GENCOM.obj,:F1:plm80.lib to gencom.mod 
:F3:locate gencom.mod code(0100H) stacksize(100) map print(gencom.tra)
:F3:objhex gencom to gencom.hex
exit
