
ZIPFILES= bdos.a86  cmdio.c    equates.a86  mem.a86   system.a86 \
	  serial.a86 cmdio.h    exe2cmd.c    misc.a86  proctbl.a86 \
      	  xios.cmd  ccp.cmd    dpgen.c      makefile  rtm.a86 \
	  cio.a86   entry.a86  sup.a86   bdos31ex.inp \
	  ccpldr.a86

OBJECTS=entry.obj sup.obj rtm.obj mem.obj cio.obj misc.obj bdos.obj \
	ccpldr.obj proctbl.obj

all:	bdos.cmd new.sys

new.sys:	bdos.cmd ccp.cmd xios.cmd dpgen.exe
	dpgen base=70

bdos31.exe:	$(OBJECTS)
	linkexe bdos31ex[i

bdos.cmd:	bdos31.exe exe2cmd.exe
	exe2cmd bdos31.exe bdos.cmd base=70

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
	zip bdos31.zip $(ZIPFILES)
