                                        February 16, 1982
					Alex Hunter
 
                        READ.ME
 
This directory contains the source files, include files, and
command files needed to build the PL/M-VAX runtime library
(including the UDI routines), and the configuration macro library.
 
The *.PLM, *.MAR, and *.FOR files are source files.  The *.LIT,
*.DEF, EXCEPT.FOR, RMSDEF.FOR, IODEF.FOR, and XQCOMMON.FOR files are
include files.  (RMSDEF.FOR and IODEF.FOR are copies of parameter
definition files from SYS$LIBRARY.  Apparently these files are not
present in all VMS systems.)

UDIMSGS.MSG is the source file for the UDI message facility.
 
LOGNAMES.COM is a command file containing logical name assignments
used by other command files in this directory.  LOGNAMES.COM should
be edited to reflect the directory names in use on your system.
Any command file which makes use of system-dependent logical name
assignments will contain a call to LOGNAMES.COM, so LOGNAMES.COM
should be the only command file requiring editing.
 
PLMRUN.BLD is a command file to build the PLMRUN.OLB library
from scratch.  XQIO.BLD is a command file to rebuild just the
XQIO package.
 
PLMMAC.BLD is a command file to build the PLMMAC.MLB configuration
macro library from scratch.
 
DM.MAR is a source file used to produce the dummy (null) object file
DM.OBJ which is referenced by various *.LNK command files to satisfy
the VMS linker's need for an object file specification in the CLUSTER
command.  DM.COM is a command file used to assemble DM.MAR.
 
VMSRTL.V23 is version 2.3 of the VMS shared runtime library.  Various
*.LNK command files reference this copy (rather than the currently
installed shared library) in order to produce executable images
which will run under VMS 2.3 (and later).
 
MAKETAPE.COM is the command file used to write the contents of
this directory to mag tape.
 
