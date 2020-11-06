CP/M 3 Operating System for the 'z80em86' emulator
==================================================
The CP/M 3 implementation contained in this distribution is intended for
running under the 'z80em86' Z80 CPU emulator.  The emulator sources and
binaries can be downloaded from here:

http://www.cpm.z80.de/source.html
http://www.gaby.de/z80/z80emu.htm

The 'z80em86' emulator documentation will describe the hardware emulated. To
exit CP/M 3 back to the system the supplied 'exit.com' program may be run or
the emulator's CTRL+ALT+I keys may be pressed and exit selected from the DOS
dialogue window that appears.

Freeware License
================
The "customised sections" (see below) of this distribution are "Freeware".
The software comes with absolutely no warranties and is to be used at your
own risk.  The code is provided "as-is".

The customised source/scripts and binary code are allowed to be modified for
personal use only, these must not be used in any other projects in original
or modified form without written permission from the author.

The "customised sections" in this distribution consists of the following
directories and contents thereof:

bios (except for DR, SPR and REL files)
support
scripts
doc

Distribution
============
The ZIP file should be unzipped to the same location where the 'z80em86'
emulator directory is located.  The build scripts will expect this install
location.

A bootable HDD disk image and sources to re-build the system are provided in
this distribution.  The pre-built system has been assembled using the Y2K
fixes from the 'CP/M 3 BINARY w/YEAR 2000 fixes' package available from
http://www.cpm.z80.de/binary.html and is also included in this package.

This distribution contains several directories.  The 'scripts' directory
contains some BASH and 'submit' file scripts to build either the original
CP/M 3 system or the fixed Y2K version.

The distribution should contain these directories and files:

z80hdd.dsk
bios
cpm3
cpm3_y2k
support
extras
scripts
doc

4MB HDD disk image (z80hdd.dsk)
-------------------------------
This is the bootable HDD disk image containing the banked and non banked
CP/M 3 systems.  Either system may be made bootable, the default is the
banked system.  It contains the original CP/M 3 (and Y2K fix binaries) and
custom support files.  The 'cp.com' copy program is also provided.

The directory is formatted for date and time stamping of files.

bios directory
--------------
Customised BIOS CP/M 3 source files required to build the systems.  This
directory also includes the CP/M 3 SPR and REL files from the normal
distribution.  When building the Y2K system the SPR and REL files from the
'cpm3_y2k' directory will be used instead.

The source files are all coded in Z80 assembly language.  The 'bios.asm'
file was converted to Z80 assembly language (zbios.mac).

cpm3 directory
--------------
All the CP/M 3 files from a normal distribution.

cpm3_y2k directory
------------------
A copy of files from the 'CP/M 3 BINARY w/YEAR 2000 fixes' package available
from http://www.cpm.z80.de/binary.html

These files will be used to replace the matching files in the 'cpm3'
directory and the SPR and REL files in the 'bios' directory when building a
Y2K system.

support directory
-----------------
Contains the customised support programs and sources.  Turbo Pascal v2.00A
is required to re-compile the pascal sources.

extras directory
----------------
Contains any files that should be included when using the build scripts. The
'cp.com' copy program is the only supplied program in the distribution but
users may add other files before running any build scripts.

scripts directory
-----------------
Contains Linux BASH and CP/M 3 submit script files used for building various
systems.

doc directory
-------------
Contains this README file and the ChangeLog.

Building the CP/M 3 systems (optional)
======================================
1. Create a CP/M formatted floppy disk.  The default floppy disk format for
   drive 'A' is identical to a 1.44MB MSDOS formatted disk but filled with
   E5 hex.
2. To build the systems 'm80.com' and 'l80.com' are required.  These should
   be copied to the HDD disk image using the CP/M 3 'dostocpm.com' program.
3. To build the systems on a Linux platform with 'DosEmu' installed run one
   of the following scripts:

   Build the 'Y2K' banked and non banked systems:
   sh scripts\make_y2k.sh

   Build the original banked and non banked systems:
   sh scripts\make_dr.sh

   After the systems are built on drive 'A' an image of the floppy disk will
   be created in the 'floppy' directory.

   Alternatively the submit files can be run manually.  See the submit files
   in the 'scripts' directory for more information.

Creating a HDD bootable disk image
==================================
1. Make a bootable floppy disk with the required system files and support
   programs (see 'Building the CP/M 3 systems').

2. The DOS 'z80hdd.com' program needs to be run in the 'z80em86' directory.
   This will create a 4MB 'z80hdd.dsk' disk image formatted with E5 hex. 
   You may want to rename any existing 'z80hdd.dsk' file if you don't want
   to lose it.

3. To place a system onto the HDD disk image under Linux with 'DosEmu'
   installed run the following script:

   sh scripts/make_hdd.sh

   Alternatively the submit file can be run manually.  See the
   'make_hdd.sub' file in the 'scripts' directory for more information.

Command line parameters
=======================
The start-up process for booting from a disk is described in the z80em86
emulator's README file in more detail.

If no drive is specified the emulator will attempt to boot from the HDD disk
image on the current drive.  If no image is found then a DOS window message
will prompt the user for a floppy disk in drive 'A'.

Booting from a floppy disk in drives 'A' or 'B' can be forced using options
'-a' or '-b'.

Additional command line parameters allows CP/M 3 commands to be run on boot
up before exiting automatically. (requires 'cpmjob.com' to be active)

A CP/M 3 command line or a submit file may be passed.  If the submit file
ends with '.sub' then the 'cpmjob.com' program will upload the submit file
from DOS.

Examples:

Boot from floppy drive A:
>z80em86 -a

Boot from floppy drive B:
>z80em86 -b

Boot the HDD image and execute a full directory command.
>z80em86 dir [full]

Boot floppy drive 'A' and execute a full directory command.
>z80em86 -a dir [full]

Boot and upload and execute commands from a submit file on the host system.
>z80em86 mysubmit.sub

Set conout to use STDOUT, full directory command redirected to 'output.txt'
>z80em86 device conout:=crt2!dir *.* [full] > output.txt

Support programs
================
The following programs and sources are provided in the 'support' directory
and most are designed to only run under this CP/M 3 system.

cls.com
-------
Clears the screen display for device CRT1 and places the cursor in the top
left corner of the display.

cpmjob.com
----------
This program allows a CCP command or a submit file to be specified from the
DOS command line.  The 'profile.sub' submit file needs the following entries
to allow this to work automatically when CP/M 3 boots:

cpmjob m:
m:cpmjob.sub

The first line creates a submit file on the RAM disk 'M' based on the
parameters passed on the DOS command line.

The second line executes the submit file.

cpmjob.sub files will contain the 'exit.com' command at the end of the file
so will exit the emulator after completing the submit commands.

cpmtodos.com
------------
Copies files from CP/M to DOS.  Run the program without any options for
usage information.

dosdir.com
----------
Displays a directory listing of DOS files.  The command output looks the
same as that from a MSDOS directory command. Wildcards and DOS paths may be
specified.  There are no options available.

The 'bytes free' size may report incorrectly.

dostocpm.com
------------
Copies files from DOS to CP/M.  Run the program without any options for
usage information.

exit.com
--------
Exits the emulator back to DOS with no confirmation.

fmfd.com
----------
A high level floppy disk formatting utility.  This program overwrites
existing data on a pre-formatted floppy disk with E5 hex.  This program can
be used instead of the supplied 'format.com' program.

Typical use is to re-format a DS HD 3.5" 1.44MB floppy disk to a blank CP/M
format.  Single sided (SS) or double sided (DS) and the number of tracks on
each side are supplied on the command line.  The program works out the
Sectors Per Track (SPT) value from CP/M 3.

Usage:- fmfd SS/DS 40/80 drive:
 SS/DS = SS is single sided, DS is double sided disk
 40/80 = 40 or 80 tracks per disk side
drive: = CP/M drive A: or B:

To format a DS HD 3.5" 1.44MB floppy disk to CP/M format in drive 'A':
>fmfd ds 80 a:

fixturbo.com
------------
A program to overcome a limitation for programs compiled with Turbo Pascal
v2.00A where only a very limited command line was possible.  This program is
run on compiled Turbo Pascal executables and requires a constant string
defined in the source file.  See some of the example sources.

Turbo Pascal developed support programs supplied in this distribution that
require extended parameters require this.

A later Turbo Pascal version would fix this problem and the work around code
could be removed.

format.com
----------
A low level floppy disk formatting utility.  This program is designed to
write E5 hex to the disk but on later hardware/systems writes F6 instead of
E5.  This means the directory structure will not be cleared correctly and
will not work correctly under CP/M.

Use the supplied high level formatting utility 'fmfd.com' after using
'format.com' to correct this.

modem.com
---------
A very simple file receiving program implementing the 'Ward Christensen file
transfer protocol' with file name support for receiving of files.

The program makes use of AUXIN and AUXOUT settings so these must be
configured first.

putldr.com
----------
This program places the 'cpmldr.sys' file onto the system tracks.  Two
parameters are required, the first parameter determines what the default
system type is. 'b' is used for a banked system and 'n' for a non banked
system.

The second parameter is the destination drive 'x:'

The current drive type (DPB, skew, etc) data for drive 'A' will also be
placed into 'cpmldr.sys'.  If the drive type has been temporarily changed
with the 'setdrive.com' program then those values will be used.

To make disk C: boot the banked system:
>putldr b c:

To make disk C: boot the non banked system:
>putldr n c:

A boot disk will require some system files and a console command processor
as a minimum.  Copy system files 'stdcpm.sys', 'bnkcpm.sys', 'cpmldr.sys'
and 'ccp.com' to the boot disk.

reset.com
---------
Resets the emulator in one of four ways, the changes are not permanent:

boot up non banked CP/M 3 system
>reset n

boot up banked CP/M 3 system
>reset b

boot up default CP/M 3 system
>reset d

boot up same system
>reset

setdrive.com
------------
Sets a floppy drive to use another CP/M format:

>setdrive D: ID

'D:' is the floppy drive.

See the 'cpmdpb.dat' text file for the structure and the ID (KEY) names that
may be used.

How well this works depends on the underlying floppy hardware. Some formats
may work on one machine and not on others.

terminal.com
------------
A simple terminal program.  The terminal(s) emulated depends on the setting
of CONIN and CONOUT devices and any terminal drivers in use.

CRT1 provides a basic Adm3a/Televideo terminal emulation in the emulator's
CRT1 video driver.

CRT2 (STDOUT) may be able to run through a DOS terminal driver but this has
not been tested.

The program makes use of AUXIN and AUXOUT settings so these must be
configured first.

video.com
---------
Turns video on or off.  This is mainly intended when a parameter is passed
on the command line when invoking the emulator.  The emulator will disable
video output so that the sign on screen is hidden.  A 'cpmjob.sub' submit
file will issue a 'video on' to switch the video back on before executing
submit commands. (see 'cpmjob.com')

Turn video off:
>video off

Turn video on:
>video on

CP/M 3 Devices
==============
The devices available under CP/M 3 are shown below,  the CP/M 3 'device'
program can be used to configure the settings.  See the 'z80em86' emulator's
README file for more detailed information.

KBD1: Uses direct keyboard handler
KBD2: Uses STDIN input
CRT1: Uses BIOS output (supports colour and monochrome)
CRT2: Uses STDOUT
COM1: Serial #1 I/O communications (no interrupts)
COM2: Serial #2 I/O communications (no interrupts)
COM3: Serial #3 I/O communications (no interrupts)
COM4: Serial #4 I/O communications (no interrupts)
LST1: Output to printer port #1
LST2: Output to printer port #2
LST3: Output to printer port #3

Disk drives
===========
Drives 'A' and 'B' allow access to floppy disk drives, there is no support
for disk images for these two drives.  Floppy disks may be changed when the
drive is not active.  The default floppy formats are shown below but may be
changed using the 'setdrive.com' program.

A RAM drive 'M' will be allocated to the remaining free memory from DOS's
640K memory, typically this results in a RAM drive of ~400K.

Drive 'C' is a HDD disk image file located on non floppy drive.

Drive A: - 80T DS HD  18 s/t  512 b/s
dpb0:   dpb     512,  18, 160, 2048,  512, 1
 
Drive B: - 80T DS HD  18 s/t  512 b/s  
dpb1:   dpb     512,  18, 160, 2048,  512, 1

Drive C: - HDD-DISK (4 Megabytes)
dpb2:   dpb     512,  64, 128, 2048, 1024, 1, 8000h

Drive M: - RAM-DISK (16k default)
dpb3:   dpb     128, 128,   1, 2048,  128, 0, 8000h

LINKS
=====
CP/M 3 and other DRI Manuals:

"Tim Olmstead Memorial Digital Research CP/M Library"
http://www.cpm.z80.de/drilib.html

"The Unofficial CP/M Web site"
http://www.cpm.z80.de/index.html

http://www.gaby.de/ehome.htm
http://www.seasip.demon.co.uk/Cpm/

http://www.z80.eu/cpmcomp.html

Contact
=======
Email to: ubee512@gmail.com

Please state which project your referring to.

Stewart
