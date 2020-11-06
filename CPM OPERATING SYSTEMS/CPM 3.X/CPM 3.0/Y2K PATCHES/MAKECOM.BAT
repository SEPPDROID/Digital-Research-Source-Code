@ECHO OFF

call env.bat

rem Do the compilation. %1.MAK contains the lines from CPM3PLM*.SUB which
rem compile %1.COM. Output is in .HEX format

%ISIS% <%1.MAK

rem Convert to CP/M .COM format

ERASE %1.MOD
HEXCOM %1.COM <%1.HEX 
ERASE %1
ERASE %1.HEX
