set :F0:=..\isis\plm80
set :F1:=..\isis\utils
set :F2:=.
set :F3:=..\isis\asm80
..\isis\utils\isis :F0:plm80 :F2:bdos.plm
..\isis\utils\isis :F3:asm80 :F2:bdosi.src
..\isis\utils\isis :F1:link  :F2:bdosi.obj,:F2:bdos.obj,:F1:plm80.lib to :F2:bdos.mod
del bdosi.obj
del bdos.obj
..\isis\utils\isis :F1:locate :F2:bdos.mod code(0800h) stacksize(0)
del bdos.mod
..\isis\utils\isis :F1:objhex :F2:bdos to :F2:bdos.hex
del bdos
..\tools\hexcon bdos.hex bdos.abs
del bdos.hex


