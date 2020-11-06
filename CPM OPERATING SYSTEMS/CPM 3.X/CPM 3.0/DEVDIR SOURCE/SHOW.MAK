:F1:PLM80 show.PLM debug optimize PAGEWIDTH(132)
:F3:link mcd80a.obj,show.obj,:F1:plm80.lib to show.mod 
:F3:locate show.mod code(0100H) stacksize(100) map print(show.tra)
:F3:objhex show to show.hex
exit
