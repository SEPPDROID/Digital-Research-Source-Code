A collection of C sources for CP/M-68K, apparently for versions 1.01, 1.02, 1.02a, and 1.03.

There are some BIOS/BDOS/CCP sources in here, but all seem to be revisions of version 1.1 of the OS.
The bulk of the code looks like revisions of Alcyon C (compiler, assembler, linker and libraries). As far as I can see, this compiler emits 68K assembler and should run under CP/M-68K, REGULUS, VMS on a VAX and Unix on a PDP-11.
Reading v101/doc/ and v102a/doc/ makes interesting trivia:

    1. "The Pascal version of CP/M-68K..." (cpm0706.rno)
    2. "Allows up to 8Gb of on-line disk(ette) storage" (pdd.rno)
    3. "CP/M-68K Alpha Release III 2/15/84" (a3.rno) 