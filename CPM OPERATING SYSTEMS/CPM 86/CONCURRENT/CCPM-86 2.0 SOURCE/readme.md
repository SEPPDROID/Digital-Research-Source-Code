This seems to be a complete distribution of Concurrent CP/M-86 v. 2.0 from July 5, 1983. Included are sources (mainly in PL/M) and compilation tools.

/0read.me

Concurrent CP/M-86   version 2.0  Source Listings Account

July 5, 1983   Bill Fitler

This account contains the source for CCPM-86 version 2.0.  All of the
utilities and RSPs have their own directories.  Other directories include:

	KERN.DIR - containing SUP, RTM, MEM, CIO and SYSDAT.
	BDOS.DIR - just the BDOS...
	XIOS.DIR - just the XIOS...
	LOADER.DIR - BOOT, LBDOS, LOAD, TCOPY

In addition, there are the following files and directories:

	COMMON.DIR - a directory containing common literal and equates.
	COMMANDS.DIR - account-wide command files, plus Tables of Contents.
	PATCHES.DOC - all CCPM-86 patches are documented in this document.

These directories are NOT available for distribution outside of DRI:
	VAXTOOLS.DIR - PLM86 compilers, etc. used in compiling the modules.
	SERIALIZ.DIR - serialization utilities

What's missing?  
	- PATCHES.DOC has not been updated for a while.
	- Some of the 0READ.ME files contain working status, which can be
	ignored.
	- Documentation TEX files are not included.  
	- Some of the kernel listings have been removed.  
	- GENCCPM needs the C86 compiler, which is not included.
	- This is NOT a working account, which means that not all of the
	compile pathnames have been tested (it was transferred from another 
	account).  Caveat programmor.
