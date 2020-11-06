This is the original source for CP/M 2.2. The source for CP/M 2.2 is in assembler, for an Intel MDS-800 development system. The only files that are in PLM are the Utilities such as PIP, STAT, etc. 

/README.TXT 

Comparison of Caldera's PLM.ZIP file (215.462 bytes, 1997-10-14 21:22,
CRC 65A8) and this very archive reveals the following difference:

- One particular file OS3BDOS.ASM has the same file length in both issues,
  but different checksums. Both files differ in a single short sequence
  of code, but they do differ, and hence the Caldera issue should be
  preserved as well. 
It has been added to this archive under the name of "OS3BDOS1.ASM".



/README.PIP




     Command:  PIP22 A:=B:*.*[R

          This function copies all files from drive b: to drive a:
     It also copies any hidden/System files from b: to a: (This 
     is what the [R is for.

