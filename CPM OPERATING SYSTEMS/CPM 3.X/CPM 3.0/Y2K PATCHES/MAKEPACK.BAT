@echo off

rem The build tools

arc a cpm2000s hexcom.c hexcom.exe 
arc a cpm2000s dir.mak date.mak show.mak setdef.mak mcd80a.mak mcd80f.mak parse.mak
arc a cpm2000s make2000.bat makepack.bat makecom.bat 
arc a cpm2000s makemcd.bat env.bat

rem CP/M sources...

arc a cpm2000s date.plm show.plm setdef.plm mcd80a.asm mcd80f.asm parse.asm
arc a cpm2000s main80.plm scan.plm search.plm sort.plm disp.plm dpb80.plm
arc a cpm2000s util.plm timest.plm main.plm *.lit

rem binaries...

arc a cpm2000s date.com dir.com setdef.com show.com 
arc a cpm2000b date.com dir.com setdef.com show.com 

rem docs...

arc a cpm2000s cpm2000.doc compile.doc
arc a cpm2000b cpm2000.doc
