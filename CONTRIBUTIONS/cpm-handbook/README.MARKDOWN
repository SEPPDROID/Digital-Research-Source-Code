Source code examples from "The Programmers CP/M Handbook": by Andy Johnson-Laird, copyright 1983 by Osborne/McGraw-Hill. Andy has given his permission to share this code for non-commercial use. 

/READ-ME.txt

[READ.ME]
The Programmer's CP/M Handbook Source Code Examples
Version 1.0  August 18, 1983
(c) 1983 Johnson-Laird Inc.

We have tried to include as many of the examples from the CP/M 
Programmer's Handbook as was possible to fit onto 2 single-sided
single-density diskettes (or alternately, 1 single-density 
double-sided "flippy" diskette).  Our original intention was to 
include the source code for every figure in the book as well as all 
the example listings in Chapter 5.  Since this proved to be impossible 
to fit onto two diskettes, we have chosen those figures and examples 
which we felt would be the most useful.  The only major omission is 
Figure 9-4 which deals with bad sector management. 

We have fixed one bug and added one line to Figure 8-10.  This is 
noted in the source code.  The lines which were left out in the book 
after page 261 are also included. You may notice some minor 
differences in capitalization, indentation, etc. in the comments where 
we have not included changes made in copyediting.

We would appreciate hearing from you, especially about any bugs, 
typos, other horrible goofs, suggestions for improvements.

                           Contents of Diskette No 1

Filename    Page No. Figure Title

FIG5-2.ASM     70    Equates for BDOS function code numbers
FIG5-3.ASM     74    Write console byte example, output null-byte
                     terminated message from specified address
FIG5-4.ASM     74    Write console byte example, output null-byte
                     terminated message following call to subroutine
FIG5-5.ASM     76    Read line from reader device
FIG5-6.ASM     78    Write line to punch device
FIG5-7.ASM     79    Write line to list device
FIG5-8.ASM     81    Read/Write string from/to console using raw I/O
FIG5-10.ASM    86    IOBYTE equates          
FIG5-11.ASM    87    Simple terminal emulator
FIG5-12.ASM    89    Display $-terminated message on console
FIG5-13.ASM    92    Read console string for keyboard options
FIG5-14.ASM    95    Determine the CP/M version number
FIG5-15.ASM    96    Reset requested disk drive
FIG5-16.ASM   100    Open file request
FIG5-17.ASM   104    Search first/next calls for ambiguous filename
FIG5-18.ASM   110    Read next character from sequential disk file
FIG5-19.ASM   113    Write next character to sequential disk file
FIG5-20.ASM   115    Create file request
FIG5-21.ASM   117    Rename file request
FIG5-22.ASM   122    Set file attributes
FIG5-23.ASM   123    Get file attributes
FIG5-24.ASM   126    Accessing disk parameter block data
FUNCTN33.ASM  131    Example for function 33, read random
FIG5-25.ASM   135    Create random file
FIG5-26.ASM   136    Read/write variable length records randomly
FIG6-4.ASM    159    Simple BIOS listing
FIG7-5.ASM    191    Example PUTCP/M
FIG7-7.ASM    198    Example CP/M cold bootstrap loader
FIG8-6.ASM    226    Device table equates
FIG10-5.ASM   363    Testbed for real time clock driver in the BIOS
FIG10-6.ASM   365    Testbed for disk I/O drivers in the BIOS
ERASE.C       410    Figure 11-3, requests confirmation before erasing
UNERASE.C     412    Figure 11-4, "revives" erased files
FIND.C        417    Figure 11-5, locates specific files or groups of files
SPACE.C       420    Figure 11-6, displays how much disk storage is used
                     or available
MOVE.C        424    Figure 11-7, "moves" files from one user to another
MAKE.C        428    Figure 11-8, makes files "invisible" and protected
                     or makes them "visible," accessible and unprotected
SPEED.C       431    Figure 11-9, sets the baud rate for a specific device
PROTOCOL.C    435    Figure 11-10, sets the protocol governing input and output
                     of a specified serial device
ASSIGN.C      439    Figure 11-11, assigns a logical devices input and output
                     to two or more physical devices
DATE.C        443    Figure 11-12, makes the current date part of the system
TIME.C        444    Figure 11-13, makes the current time part of the system
FUNKEY.C      446    Figure 11-14, sets the character strings associated with
                     specific function keys

                            Contents of Diskette 2

FIG8-10.ASM   237    Enhanced BIOS listing
FIG9-5.ASM    312    User-friendly disk error processor
FIG10-2.ASM   331    Debug subroutines
FIG10-4.ASM   355    Testbed for character I/O drivers
LIBRARY.C     372    Figure 11-1, commonly used functions in C
LIBRARY.H     390    Figure 11-2, code to be included at the beginning of
                     any program that calls LIBRARY functions in Figure 11-1



					 
/_dedication.txt

The included files are the source code examples from Andy Johnson-Laird's book, "The Programmers CP/M Handbook", copyright 1983 by Osborne/McGraw-Hill.

Andy has given his permission to share this code for non-commercial use and asked that the following information be included with this distribution -

*******************************************

Please give me credit for the code.

You might even consider putting in a two line tribute to the late Gary Kildall.

Andy has requested that those who use this code pause to consider where we would 
be if the late Gary Kildall, a pioneer's pioneer, had not taken the time to write CP/M.
 
If you want to know the *real* story, I can recommend the book "They Made America" by Sir Harold Evans. It will tell you just how significant Gary's work was, and how Mr. Gates got to where he is.
The second edition has a far more complete story.

Regards
Andy

********************************************

Posted 10/8/07
Jack Rubin