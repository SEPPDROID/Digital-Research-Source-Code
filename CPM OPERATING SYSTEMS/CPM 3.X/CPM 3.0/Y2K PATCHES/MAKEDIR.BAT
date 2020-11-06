@ECHO OFF

call env.bat

rem Do the compilation. DIR.MAK contains the lines from CPM3PLM7.SUB which
rem compile DIR.COM. Output is in .HEX format

%ISIS% <DIR.MAK

rem Convert to CP/M .COM format

ERASE DIR.LNK
HEXCOM DIR.COM <DIR.HEX