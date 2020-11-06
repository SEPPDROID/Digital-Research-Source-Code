# Makefile for Portable CP/M Peripheral Interchange Program, PIP.
#
# Change the variables INCLUDE, BIN, UBIN, CC, LD, LIB as necessary for a
# particular environment.
#
# Usage:
#	make			makes executable file PIP for CP/M target
#	make pipn		makes non-split version of above
#	make pip.o		compiles pip.c
#	make pip.cpm		makes file pip.cpm for loading with mkfile
#	make list		sends formatted listing of ALL source
#				files to print spooler
#
# Author: D Dunlop, Zilog Inc.		11/19/82

INCLUDE	= /u/Tests/include
BIN	= /bin
UBIN	= /usr/bin
CC	= $(BIN)/zcc -M1 -c
LD	= $(UBIN)/ld8k -i -w
ULIB	= /usr/lib
LIB	= /lib
PSOURCE	= makefile pip.c portab.h bdos.h\
	  basepage.h setjmp.h
ESOURCE	= makefile ed.c portab.h bdos.h\
	  basepage.h setjmp.h
SSOURCE	= makefile stat.c portab.h bdos.h\
	  basepage.h setjmp.h
SOURCE	= makefile ed.c pip.c stat.c portab.h bdos.h\
	  basepage.h setjmp.h
POBJS	= pip.o $(ULIB)/libcpm.a $(ULIB)/startup.o
EOBJS	= ed.o $(ULIB)/libcpm.a $(ULIB)/startup.o
SOBJS	= stat.o $(ULIB)/libcpm.a $(ULIB)/startup.o

pip:	$(POBJS)
	$(LD) -o pip $(ULIB)/startup.o pip.o -lcpm

pip.o:	$(PSOURCE)
	$(CC) pip.c

pip.cpm:	pip
	$(UBIN)/imager pip

plist:
	pr $(PSOURCE) | lpr&

elist:
	pr $(ESOURCE) | lpr&

slist:
	pr $(SSOURCE) | lpr&

list:
	pr $(SOURCE) | lpr&

pipn:	$(POBJS)
	/bin/ld8k -w -o pipn $(ULIB)/startup.o pip.o -lcpm

ed:	$(EOBJS)
	$(LD) -o ed $(ULIB)/startup.o ed.o -lcpm

ed.o:	$(ESOURCE)
	$(CC) ed.c

ed.cpm:	ed
	$(UBIN)/imager ed

stat:	$(SOBJS)
	$(LD) -o stat $(ULIB)/startup.o stat.o -lcpm

stat.o:	$(SSOURCE)
	$(CC) stat.c

stat.cpm:	stat
	$(UBIN)/imager stat
