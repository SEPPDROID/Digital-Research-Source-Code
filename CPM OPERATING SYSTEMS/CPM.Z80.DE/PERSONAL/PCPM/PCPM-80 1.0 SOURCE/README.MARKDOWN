Source files of PCP/M-80 1.0.

/READ.ME

The following list of corrections should be made to the Personal CP/M 8-bit
version 1.0 documentation.

Programmer's Guide

        page 2-10
        BDOS function 2
                text says 'CONSOLE INPUT'
                should be 'CONSOLE OUTPUT'

        page 2-45
        BDOS function 33
                'Entry Parameters' add after Register C line:
                'Register DE:   FCB Address'

        page 2-49
        BDOS function 35
                replace information about values returned in registers with:
                'Random record field of FCB set'

System Guide

        Section 2
                References to the BDOS size being 1100h bytes are incorrect.
                The BDOS code segment is 1000h bytes, and the BDOS data
                segment is 00BFh bytes.  With the standard distibution,
                BDOSH.REL and BDOSL.REL will link these in a separate area
                from the BDOS code segment.  OEMs that purchase the source
                can set an assembly-time switch that will make the data areas
                part of the code segment so that it will all be linked as one
                segment of 1100h bytes if the BDOS will execute in RAM.

        page 4-15
        BIOS function WRITE
                Entry Parameters:  Register C = 0:  normal sector write
                                                1:  write to directory sector
                                                2:  write to the first sector
                                                    of a new data block


/README.TOO

Please note: line 2528 in BDOS.MAC is corrupted. It should read

        jp      z,COPY$DIRLOC   ;stop at end of dir

--------------------------

This zip file contains the original source for Personal CP/M 1.0. 
If anybody figures out anything about this code, please drop an
email message to me at : 

gaby@gaby.de

and I'll pass it on.

tnx
