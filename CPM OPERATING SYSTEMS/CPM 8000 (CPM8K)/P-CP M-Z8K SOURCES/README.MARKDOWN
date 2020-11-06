These are the C sources for a CP/M-8000 version from May 5, 1983 and October 20, 1983. Version number unknown, possibly 1.0. Some sources contain the string "P-CP/M for the Olivetti M20 (Z8001)". Included is the BIOS for the Olivetti M20 Z8001 and adaptions for the Kontron Z8002. Also included is a set of manuals in TEX format.
Here are some comments I recently received regarding this and the two following archives:

    Reading 'bdos/bdosmisc.c' reveals: "CPM-Z8000 Version 1.2 03/14/83". This file seems to be the usual place for version and copyright strings in CP/M's written in C.
    In the same archive, 'newos/bdosmisc.c' says "CPM-Z8000 Version 1.3 06/14/83". Sad, really, that 'newos/bdosmain.c' is the cut-down 'loader' version of the BDOS. Sadder still that BDOSMA.C in cpm8k13.zip is also the loader BDOS.
    Might as well dip into cpm8k11, then...

        1. The BIOS (source) is for the Olivetti M20.
        2. There's no source for BDOS and utils.
        3. There is a C compiler: "Zilog CP/M-Z8000 C Compiler v1.01e 12/26/84". In the readme on disk 4 it is claimed that the -D option is broken on this version. Clearly not good. 

