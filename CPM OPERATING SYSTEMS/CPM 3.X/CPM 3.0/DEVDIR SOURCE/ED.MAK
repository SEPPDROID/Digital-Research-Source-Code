:F1:plm80 ed.plm optimize debug
:F3:link mcd80a.obj,ed.obj,:F1:plm80.lib to ed.mod
:F3:locate ed.mod code(0100h) stacksize(100) map print(ed.tra)
:F3:objhex ed to ed.hex
exit
