# Digital-Research-Source-Code-Repo
A repository with all the Digital Research Source files i had. This is by all means an UNOFFICIAL github repo, just a personal collection i dont want to lose.
All the source files can be found [here](http://www.cpm.z80.de/source.html)
And all the binaries [here](http://www.cpm.z80.de/binary.html).
This all and more can be found on the [The Unofficial CP/M Web site](http://www.cpm.z80.de)
Most of the Readme comments belong to Gaby Chaudry. For actual licensing please visit [The Unofficial CP/M Web site](http://www.cpm.z80.de) _(because some of the licensing included in the folders might not be up to date)_

[**In Memory of Dr. Gary A. Kildall**](http://www.digitalresearch.biz/DR/Gary/eulogy.html)

# Contributing

Feel free to push any unknown or other Digital Research source files that are missing.
##### _non-Digital Research files can be pushed to the `[CONTRIBUTIONS]` directory._

# Included

### Compilers & Assembly tools
* Basic-E
* PLM Compiler
* PLM VAX Compiler
* PLM to C 
* PLM to C 2
### Contributions (cpm.z80.de)
* CPM 1.4
* cpm-handbook
* exchange 0.1
* MOSS 2.2
* PLM DosBox Compiler
* TEX-REV
* Z80 EMU 86
### CPM Operating Systems
* CPM 1.x
* CPM 2.x
* CPM 3.x
* CPM 68k
* CPM 86
* CPM 8000 (CPM8K)
* CPM.Z80.DE (PERSONAL)
### CP-NET
* CPNET-80 Source
* CPNET-86 Source
### GSX
* GSX-86 Driver Sources
* JohnElliot (Mirrored from [John's Site](http://www.seasip.info/Cpm/software/gsx86.html))
### MPM Operating Systems
* MPM I
* MPM II
* MPM 86
### Sample code
* CPM 2.2 Samples
* PLI Programs Samples

# Tree directory listing
```
Folder PATH listing 
D:\DRDOS\DIGITAL RESEARCH
├───ASSEMBLY & COMPILE TOOLS
│   ├───Basic-E
│   │   └───source
│   ├───PLM COMPILER
│   ├───PLM VAX COMPILER
│   │   ├───plmudi
│   │   ├───plmvax
│   │   └───util86
│   ├───PLM-2-C
│   │   └───PLMZIP
│   │       └───PLMGZ
│   └───PLM-2-C 2
│       ├───avl-subs
│       ├───plm
│       └───plm2c
├───CONTRIBUTIONS
│   ├───cpm-handbook
│   │   └───cpmsrc
│   ├───cpm1.4
│   │   ├───bdosplm
│   │   ├───binaries
│   │   ├───bldbdos
│   │   ├───bldccp
│   │   ├───doc
│   │   ├───isis
│   │   ├───tgtbin
│   │   ├───tools
│   │   └───tools2
│   ├───exchange-0.1
│   ├───MOSS 2.2
│   │   ├───8800-Manual
│   │   ├───MOSS22
│   │   ├───tif
│   │   └───Z80LIB
│   ├───plm-80 DOSbox compiler
│   ├───tex-rev
│   └───z80em86
│       ├───bios
│       ├───cpm3
│       ├───cpm3_y2k
│       ├───doc
│       ├───extras
│       ├───scripts
│       ├───support
│       └───z80em86_readme_downloads
│           └───z80em86-1.0.1
│               ├───doc
│               │   └───GPL
│               ├───scripts
│               └───src
├───CP-NET
│   ├───CPNET-80 SOURCE
│   └───CPNET-86 SOURCE
├───CPM OPERATING SYSTEMS
│   ├───CPM 1.X
│   │   └───CPM 1.0
│   │       ├───BDOS
│   │       │   ├───IMG
│   │       │   └───SOURCE
│   │       ├───CPM
│   │       └───LOAD COM
│   ├───CPM 2.X
│   │   ├───CPM 2.0
│   │   └───CPM 2.2
│   │       ├───ASM80
│   │       ├───CPM 2.2 ASM
│   │       ├───CPM 2.2 SOURCE
│   │       ├───CPM 2.2 SRC IMG
│   │       └───PATCHES
│   │           └───SUBMIT001
│   ├───CPM 3.X
│   │   └───CPM 3.0
│   │       ├───3.0 PLM SOURCE
│   │       ├───3.0 SOURCE
│   │       ├───3ON2
│   │       ├───DEVDIR SOURCE
│   │       ├───SOURCE
│   │       └───Y2K PATCHES
│   ├───CPM 68K
│   │   ├───1.0X SOURCES
│   │   │   ├───v101
│   │   │   │   ├───asmlink
│   │   │   │   ├───bdos
│   │   │   │   ├───bios
│   │   │   │   ├───boot
│   │   │   │   ├───c
│   │   │   │   │   ├───ccp
│   │   │   │   │   ├───cgen
│   │   │   │   │   ├───ctest
│   │   │   │   │   ├───fcgen
│   │   │   │   │   ├───fcgen2
│   │   │   │   │   ├───fparser
│   │   │   │   │   ├───fpreproc
│   │   │   │   │   ├───lib7
│   │   │   │   │   ├───libe
│   │   │   │   │   ├───libf
│   │   │   │   │   ├───parser
│   │   │   │   │   ├───preproc
│   │   │   │   │   └───unixlib
│   │   │   │   ├───clib
│   │   │   │   ├───ddt
│   │   │   │   ├───doc
│   │   │   │   ├───klib
│   │   │   │   ├───newas
│   │   │   │   ├───newlink
│   │   │   │   ├───sid
│   │   │   │   ├───test
│   │   │   │   ├───utils
│   │   │   │   └───vms
│   │   │   ├───v102
│   │   │   │   ├───as68
│   │   │   │   ├───bdos
│   │   │   │   ├───bin
│   │   │   │   ├───bios
│   │   │   │   ├───boot
│   │   │   │   ├───bugs
│   │   │   │   ├───c068
│   │   │   │   ├───c168
│   │   │   │   ├───clib
│   │   │   │   ├───cp68
│   │   │   │   ├───cross
│   │   │   │   │   ├───bin
│   │   │   │   │   ├───com
│   │   │   │   │   ├───cpm
│   │   │   │   │   ├───lib
│   │   │   │   │   └───serial
│   │   │   │   ├───flxtp
│   │   │   │   ├───klib
│   │   │   │   ├───libe
│   │   │   │   ├───libf
│   │   │   │   ├───link68
│   │   │   │   ├───lo68
│   │   │   │   ├───sid
│   │   │   │   ├───test
│   │   │   │   └───user0
│   │   │   ├───v102a
│   │   │   │   ├───al40
│   │   │   │   │   ├───as68
│   │   │   │   │   ├───c068
│   │   │   │   │   ├───c168
│   │   │   │   │   ├───c68
│   │   │   │   │   ├───dri
│   │   │   │   │   │   ├───c068
│   │   │   │   │   │   ├───c168
│   │   │   │   │   │   ├───cp68
│   │   │   │   │   │   ├───flxtp
│   │   │   │   │   │   ├───klib
│   │   │   │   │   │   ├───libe
│   │   │   │   │   │   ├───libf
│   │   │   │   │   │   └───lo68
│   │   │   │   │   ├───fp
│   │   │   │   │   │   ├───libE
│   │   │   │   │   │   └───libF
│   │   │   │   │   ├───lib7
│   │   │   │   │   ├───oldas68
│   │   │   │   │   ├───test
│   │   │   │   │   ├───utillib
│   │   │   │   │   └───utils
│   │   │   │   ├───alclib
│   │   │   │   │   └───v7
│   │   │   │   ├───as68
│   │   │   │   ├───bdos
│   │   │   │   ├───boot
│   │   │   │   ├───c068
│   │   │   │   ├───c168
│   │   │   │   ├───cc
│   │   │   │   ├───clib
│   │   │   │   ├───cp68
│   │   │   │   ├───cutils
│   │   │   │   ├───doc
│   │   │   │   │   ├───cmnd
│   │   │   │   │   ├───files
│   │   │   │   │   ├───misc
│   │   │   │   │   └───subs
│   │   │   │   ├───include
│   │   │   │   │   └───sys
│   │   │   │   ├───lib7
│   │   │   │   ├───libe
│   │   │   │   ├───libf
│   │   │   │   ├───lo68
│   │   │   │   ├───misc
│   │   │   │   ├───orgas68
│   │   │   │   ├───orgc068
│   │   │   │   ├───orgc168
│   │   │   │   ├───orgc268
│   │   │   │   ├───orgcp68
│   │   │   │   ├───orglibe
│   │   │   │   ├───orglibf
│   │   │   │   ├───orglo68
│   │   │   │   ├───plmutils
│   │   │   │   ├───sid
│   │   │   │   ├───test
│   │   │   │   │   └───asm
│   │   │   │   ├───user0
│   │   │   │   ├───util
│   │   │   │   └───xutils
│   │   │   └───v103
│   │   │       ├───as
│   │   │       ├───cc
│   │   │       ├───cgen
│   │   │       ├───clib
│   │   │       │   └───v7
│   │   │       ├───cpp
│   │   │       ├───doc
│   │   │       │   ├───cmnd
│   │   │       │   ├───files
│   │   │       │   ├───misc
│   │   │       │   └───subs
│   │   │       ├───libe
│   │   │       ├───libf
│   │   │       ├───lo68
│   │   │       ├───misc
│   │   │       ├───parser
│   │   │       ├───test
│   │   │       │   └───asm
│   │   │       └───util
│   │   ├───1.2 SOURCE
│   │   │   ├───0
│   │   │   ├───1
│   │   │   ├───10
│   │   │   ├───11
│   │   │   ├───12
│   │   │   ├───13
│   │   │   ├───14
│   │   │   ├───15
│   │   │   ├───2
│   │   │   ├───3
│   │   │   ├───4
│   │   │   ├───5
│   │   │   ├───6
│   │   │   ├───7
│   │   │   ├───8
│   │   │   └───9
│   │   ├───1.2 SOURCE IMG
│   │   └───cpm68k_pgms
│   │       ├───sdbas
│   │       ├───snobol4
│   │       └───utils
│   ├───CPM 8000 (CPM8K)
│   │   ├───CPM 8K 1.1
│   │   │   ├───disk1
│   │   │   ├───disk2
│   │   │   ├───disk3
│   │   │   └───disk4
│   │   ├───P-CP M-Z8K 1.3 SOURCE
│   │   └───P-CP M-Z8K SOURCES
│   │       ├───as8000
│   │       ├───bdos
│   │       ├───bios
│   │       ├───ccp
│   │       ├───ddt
│   │       ├───games
│   │       ├───link
│   │       ├───newos
│   │       ├───olivetti
│   │       ├───pg
│   │       ├───sg
│   │       └───utils
│   ├───CPM 86
│   │   ├───86DEV
│   │   ├───CALPC
│   │   │   ├───CALBIOS
│   │   │   ├───CALDOC
│   │   │   └───CALUTILS
│   │   ├───CONCURRENT
│   │   │   ├───CCPM-86 & CPM-86 SOURCES
│   │   │   │   ├───ASM
│   │   │   │   ├───CCPM
│   │   │   │   ├───DDT
│   │   │   │   └───STUFF
│   │   │   ├───CCPM-86 2.0 SOURCE
│   │   │   │   ├───abort
│   │   │   │   ├───asm86
│   │   │   │   ├───bdos
│   │   │   │   ├───clock
│   │   │   │   ├───commands
│   │   │   │   ├───common
│   │   │   │   ├───date
│   │   │   │   ├───ddt86
│   │   │   │   ├───dir
│   │   │   │   ├───echo
│   │   │   │   ├───ed
│   │   │   │   ├───era
│   │   │   │   ├───eraq
│   │   │   │   ├───genccpm
│   │   │   │   ├───gencmd
│   │   │   │   ├───help
│   │   │   │   ├───initdir
│   │   │   │   ├───kern
│   │   │   │   ├───loader
│   │   │   │   ├───pin
│   │   │   │   ├───pip
│   │   │   │   ├───ren
│   │   │   │   ├───sdir
│   │   │   │   ├───serializ
│   │   │   │   ├───set
│   │   │   │   ├───show
│   │   │   │   ├───submit
│   │   │   │   ├───systat
│   │   │   │   ├───tmp
│   │   │   │   ├───type
│   │   │   │   ├───vcmode
│   │   │   │   ├───vout
│   │   │   │   └───xios
│   │   │   ├───CCPM-86 3.1 SOURCE
│   │   │   │   ├───D1
│   │   │   │   ├───D10
│   │   │   │   ├───D11
│   │   │   │   ├───D12
│   │   │   │   ├───D2
│   │   │   │   ├───D3
│   │   │   │   ├───D4
│   │   │   │   ├───D5
│   │   │   │   ├───D6
│   │   │   │   ├───D7
│   │   │   │   ├───D8
│   │   │   │   └───D9
│   │   │   └───SET-SHOW SOURCE
│   │   ├───CPM 86 # SOURCE
│   │   ├───CPM 86 1.1 SOURCE
│   │   └───CPM86ANNOTATE
│   │       └───CPM86BUG
│   └───CPM.Z80.DE
│       └───PERSONAL
│           ├───PCPM
│           │   └───PCPM-80 1.0 SOURCE
│           └───PCPM-86
│               ├───BDOS 3.1
│               ├───BDOS 3.3
│               ├───BDOS 4.1
│               ├───PCPM 1.0-5B
│               │   └───SRC
│               └───PCPM 2.1-1
│                   └───SRC
├───GSX
│   ├───GSX-86 DRIVER SOURCES
│   └───JohnElliot
├───MPM OPERATING SYSTEMS
│   ├───MPM I
│   │   ├───MPM BDOS SOURCE
│   │   └───MPM I SOURCE
│   │       ├───01
│   │       ├───02
│   │       ├───03
│   │       ├───04
│   │       ├───05
│   │       ├───06
│   │       ├───07
│   │       ├───08
│   │       ├───09
│   │       ├───10
│   │       ├───11
│   │       ├───12
│   │       ├───13
│   │       ├───14
│   │       └───15
│   ├───MPM II
│   │   ├───MPM II SOURCE
│   │   │   ├───BNKBDOS
│   │   │   ├───CONTROL
│   │   │   ├───MPMLDR
│   │   │   ├───NUCLEUS
│   │   │   ├───PLM_WORK
│   │   │   ├───SERIAL
│   │   │   ├───TEX
│   │   │   ├───UTIL1
│   │   │   ├───UTIL2
│   │   │   ├───UTIL3
│   │   │   ├───UTIL4
│   │   │   ├───UTIL5
│   │   │   ├───UTIL6
│   │   │   ├───UTIL7
│   │   │   ├───UTIL8
│   │   │   └───UTIL9
│   │   └───MPM II SOURCE 2
│   │       ├───ASM
│   │       │   └───DDT
│   │       ├───bdos
│   │       ├───Control_1
│   │       ├───Control_2
│   │       ├───CUSPs_1
│   │       ├───CUSPs_2
│   │       ├───CUSPs_3
│   │       ├───CUSPs_4
│   │       ├───CUSPs_5
│   │       ├───LITs
│   │       ├───mpmldr
│   │       │   └───gensys
│   │       ├───nucleus
│   │       ├───OEM
│   │       ├───RSPs
│   │       └───TEX_Files
│   └───MPM-86
│       ├───MISC DRI DISKS
│       │   ├───01
│       │   ├───02
│       │   ├───03
│       │   ├───04
│       │   ├───05
│       │   ├───06
│       │   ├───07
│       │   ├───08
│       │   ├───09
│       │   ├───10
│       │   ├───11
│       │   ├───12
│       │   ├───13
│       │   ├───14
│       │   ├───15
│       │   ├───16
│       │   ├───17
│       │   ├───18
│       │   ├───19
│       │   ├───20
│       │   ├───21
│       │   ├───22
│       │   ├───23
│       │   ├───24
│       │   └───25
│       └───MPM-86 2.0 SOURCES
│           ├───01
│           ├───02
│           ├───03
│           ├───04
│           ├───05
│           ├───06
│           ├───07
│           ├───08
│           ├───09
│           ├───10
│           └───11
└───SAMPLE CODE
    ├───CPM 2.2 SAMPLE CODE
    └───PLI PROG SAMPLE CODE
    
    
```

# Usefull Links 

[The Unofficial CP/M Web site](http://www.cpm.z80.de)

[In Memory of Dr. Gary A. Kildall](http://www.digitalresearch.biz/DR/Gary/eulogy.html)

[John Elliott's homepage (CP/M Main Page)](http://www.seasip.info/index.html)

[Digital Research Home](http://www.digitalresearch.biz/)

[Digital Research History and Documents, by Herb Johnson](http://retrotechnology.com/dri/d_dri.html)

[Public server of gaby.de containing various CP/M downloads](http://ftp.gaby.de/)


---


```
Ask Bill [Gates] why the string in [MS-DOS] function 9 is terminated by a dollar sign. 
Ask him, because he can't answer. Only I know that. - Gary 
```
#### **_The world wouldn't have been the same without Gary..._**

1942-1994
