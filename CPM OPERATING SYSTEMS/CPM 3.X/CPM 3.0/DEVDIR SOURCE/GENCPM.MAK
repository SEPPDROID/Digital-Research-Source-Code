:F1:plm80 gencpm.plm debug optimize
:F1:plm80 getdef.plm debug optimize
:F1:plm80 setbuf.plm debug optimize
:F1:plm80 crdef.plm debug optimize
:F2:asm80 datmod.asm debug
:F2:asm80 ldrlwr.asm debug
:F2:asm80 mcd80f.asm
:F3:link mcd80f.obj,gencpm.obj,setbuf.obj,getdef.obj,crdef.obj,ldrlwr.obj,datmod.obj,:F1:plm80.lib to gencpm.mod
:F3:locate gencpm.mod code(0100H) stacksize(100)
:F3:objhex gencpm to gencpm.hex
exit
