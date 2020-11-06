
ZIPFILES= bdos.a86     cmdio.c    equates.a86  mem.a86   system.a86 \
	  serial.a86   cmdio.h    exe2cmd.c    misc.a86  proctbl.a86 \
      	  xios.cmd     ccp.cmd    dpgen.c      makefile  rtm.a86 \
	  cio.a86      entry.a86  sup.a86      bdos41ex.inp \
	  makefile.211 readme.txt sysdosp.a86  serdosp.a86 \
  sys211e.a86 sys211d.a86 sys204e.a86 sys204d.a86 sys202e.a86 sys202d.a86 \
  ser211e.a86 ser211d.a86 ser204e.a86 ser204d.a86 ser202e.a86 ser202d.a86 

OBJECTS=entry.obj sup.obj rtm.obj mem.obj cio.obj misc.obj bdos.obj proctbl.obj 

all:	bdos.cmd new.sys

new.sys:	bdos.cmd ccp.cmd xios.cmd dpgen.exe
	dpgen

bdos41.exe:	$(OBJECTS)
	linkexe bdos41ex[i

bdos.cmd:	bdos41.exe exe2cmd.exe
	exe2cmd bdos41.exe bdos.cmd

%.obj:	%.a86 equates.a86 system.a86
	rasm86 $<

misc.obj:	misc.a86 equates.a86 system.a86 serial.a86

dpgen.exe:	dpgen.c cmdio.c cmdio.h
	pacc -Bl dpgen.c cmdio.c

exe2cmd.exe:	exe2cmd.c
	pacc -Bs $<

clean:
	xdel $(OBJECTS)
	xdel bdos.cmd new.sys

zip:
	zip bdos41.zip $(ZIPFILES)
