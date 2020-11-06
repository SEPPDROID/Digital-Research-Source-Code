@echo off
rem #1 = inputfn, #2 outputfn #3 = addr, #4 = lth
rem build a script for debug 
echo rcx>dbg.bat
echo %4>>dbg.bat
echo n%2>>dbg.bat
echo w%3>>dbg.bat
echo q>>dbg.bat
call debug %1 <dbg.bat
 