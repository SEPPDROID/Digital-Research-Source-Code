$ lo68 -r -o c168.rel canon.o codegen.o cskels.o init.o interf.o main.o -
optab.o putexpr.o smatch.o sucomp.o tabl.o util.o version.o 
