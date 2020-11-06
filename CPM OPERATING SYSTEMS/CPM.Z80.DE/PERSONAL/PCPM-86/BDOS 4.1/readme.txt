BDOS 4.1 source tree
====================

  This file contains disassembled source code and a build system for the 
CP/M-86 BDOS, version 4.1. This is the version that is included in 
Personal CP/M-86 2.x and also DOS Plus 1.2.

How to build
============

  In order to use the build system, you will need:
* The Digital Research assembler and linker: RASM86 and LINKEXE.
* Pacific C, to build the C tools EXE2CMD and DPGEN.
* GNU Make.

  You should just be able to type 'make' and a new system file (new.sys)
will be generated containing the newly-built BDOS.

  Note, however, that certain structures need to be paragraph-aligned, and 
the A86 files do not enforce this. Therefore if you add or remove 
code or variables, you will need to check for bits marked PADDING and ensure
they do pad out the correct number of bytes.

Notes on the source
===================

  The source code was disassembled using the IDA disassembler. It has been
split up into modules named after their counterparts in Concurrent CP/M
(SUP, RTM etc.) plus a few extras.

  Rather than use LINKCMD to generate a CMD file, I was forced to use 
LINKEXE to generate an EXE, and then write a custom EXE2CMD program to 
convert the one to the other. This is because LINKCMD starts the data 
segment at offset 100h, but to build the BDOS you need it at 0. The 
'official' DRI way of doing this involves a patched linker (LINK0.CMD) but
there's no DOS version and it didn't seem to like running in a CP/M emulator,
so I was forced to adopt other tactics.

  There are two variables which, despite being in the BDOS data segment, refer 
to addresses in the XIOS. These are:

freemem:	The address of the first segment available for programs. This
		is left as an unresolved external symbol in the provided 
		source code, and is populated by DPGEN.

bdos_sysflags:	The address of the system flag table in the XIOS. The only
		way I found of making that come out right was to store its
		value as an EQU directive in SYSTEM.A86. When I was writing 
		my modified XIOS for DOS Plus, I came across the same problem
		from the other direction; the modified XIOS initialises this
		variable at startup time so that the value in the BDOS is no 
		longer relevant.

Support for multiple versions
=============================

  The build system is currently configured to build Personal CP/M v2.04e. If 
you want to build other versions:

1. Get hold of the .SYS file for the version you want to build.
2. Use DPSPLIT (not supplied) to break it into BDOS.CMD, XIOS.CMD and CCP.CMD.
  Delete BDOS.CMD but hang on to the other two.
3. Copy the correct SYS*.A86 to SYSTEM.A86, and the correct SER*.A86 to 
  SERIAL.A86:

System		Version		Language	Files to copy
==========================================================================
PCP/M		2.02		German		SER202D.A86 / SYS202D.A86
PCP/M		2.02		English		SER202E.A86 / SYS202E.A86
PCP/M		2.04		German		SER204D.A86 / SYS204D.A86
PCP/M		2.04		English		SER204E.A86 / SYS204E.A86
PCP/M		2.11		German		SER211D.A86 / SYS211D.A86
PCP/M		2.11		English		SER211E.A86 / SYS211E.A86
DOS Plus	1.2		English		SERDOSP.A86 / SYSDOSP.A86

4. Rerun the build.

  From the point of view of the BDOS, the changes are mostly trivial: 
differences are mostly caused by such things as a different serial number,
alternative screen dimensions etc. One exception is that DOS Plus omits a
few BDOS patches which are present in all known PCP/M-86 2.x versions.

