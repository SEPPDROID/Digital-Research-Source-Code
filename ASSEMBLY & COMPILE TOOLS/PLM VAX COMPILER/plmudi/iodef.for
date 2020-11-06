	PARAMETER	IO$V_FCODE      = '00000000'X
	PARAMETER	IO$_NOP         = '00000000'X
	PARAMETER	IO$_UNLOAD      = '00000001'X
	PARAMETER	IO$_LOADMCODE   = '00000001'X
	PARAMETER	IO$_STARTMPROC  = '00000002'X
	PARAMETER	IO$_SEEK        = '00000002'X
	PARAMETER	IO$_SPACEFILE   = '00000002'X
	PARAMETER	IO$_RECAL       = '00000003'X
	PARAMETER	IO$_STOP        = '00000003'X
	PARAMETER	IO$_INITIALIZE  = '00000004'X
	PARAMETER	IO$_DRVCLR      = '00000004'X
	PARAMETER	IO$_SETCLOCKP   = '00000005'X
	PARAMETER	IO$_RELEASE     = '00000005'X
	PARAMETER	IO$V_DELDATA    = '00000006'X
	PARAMETER	IO$V_CANCTRLO   = '00000006'X
	PARAMETER	IO$V_SETEVF     = '00000006'X
	PARAMETER	IO$_ERASETAPE   = '00000006'X
	PARAMETER	IO$S_FCODE      = '00000006'X
	PARAMETER	IO$V_TYPEAHDCNT = '00000006'X
	PARAMETER	IO$_STARTDATAP  = '00000006'X
	PARAMETER	IO$_OFFSET      = '00000006'X
	PARAMETER	IO$V_NOECHO     = '00000006'X
	PARAMETER	IO$V_INTERRUPT  = '00000006'X
	PARAMETER	IO$V_WORD       = '00000006'X
	PARAMETER	IO$V_STARTUP    = '00000006'X
	PARAMETER	IO$V_NOW        = '00000006'X
	PARAMETER	IO$V_BINARY     = '00000006'X
	PARAMETER	IO$V_ACCESS     = '00000006'X
	PARAMETER	IO$V_REVERSE    = '00000006'X
	PARAMETER	IO$V_COMMOD     = '00000006'X
	PARAMETER	IO$V_READATTN   = '00000007'X
	PARAMETER	IO$V_ENABLMBX   = '00000007'X
	PARAMETER	IO$V_PACKED     = '00000007'X
	PARAMETER	IO$V_TIMED      = '00000007'X
	PARAMETER	IO$V_MOVETRACKD = '00000007'X
	PARAMETER	IO$V_RESET      = '00000007'X
	PARAMETER	IO$V_CREATE     = '00000007'X
	PARAMETER	IO$V_NOWAIT     = '00000007'X
	PARAMETER	IO$V_SHUTDOWN   = '00000007'X
	PARAMETER	IO$V_CTRLYAST   = '00000007'X
	PARAMETER	IO$_RETCENTER   = '00000007'X
	PARAMETER	IO$_QSTOP       = '00000007'X
	PARAMETER	IO$V_WRTATTN    = '00000008'X
	PARAMETER	IO$V_ATTNAST    = '00000008'X
	PARAMETER	IO$V_CTRLCAST   = '00000008'X
	PARAMETER	IO$V_DELETE     = '00000008'X
	PARAMETER	IO$V_DIAGNOSTIC = '00000008'X
	PARAMETER	IO$V_INTSKIP    = '00000008'X
	PARAMETER	IO$V_NOFORMAT   = '00000008'X
	PARAMETER	IO$_PACKACK     = '00000008'X
	PARAMETER	IO$V_CVTLOW     = '00000008'X
	PARAMETER	IO$V_ABORT      = '00000008'X
	PARAMETER	IO$_SPACERECORD = '00000009'X
	PARAMETER	IO$V_HANGUP     = '00000009'X
	PARAMETER	IO$V_SETFNCT    = '00000009'X
	PARAMETER	IO$V_SKPSECINH  = '00000009'X
	PARAMETER	IO$V_SYNCH      = '00000009'X
	PARAMETER	IO$V_OPPOSITE   = '00000009'X
	PARAMETER	IO$_SEARCH      = '00000009'X
	PARAMETER	IO$V_NOFILTR    = '00000009'X
	PARAMETER	IO$V_MOUNT      = '00000009'X
	PARAMETER	IO$V_DSABLMBX   = '0000000A'X
	PARAMETER	IO$_WRITECHECK  = '0000000A'X
	PARAMETER	IO$V_DMOUNT     = '0000000A'X
	PARAMETER	IO$V_DATAPATH   = '0000000A'X
	PARAMETER	IO$V_SWAP       = '0000000A'X
	PARAMETER	IO$V_CECYL      = '0000000A'X
	PARAMETER	IO$_WRITEPBLK   = '0000000B'X
	PARAMETER	IO$V_PURGE      = '0000000B'X
	PARAMETER	IO$V_INHERLOG   = '0000000B'X
	PARAMETER	IO$V_CYCLE      = '0000000C'X
	PARAMETER	IO$V_INHSEEK    = '0000000C'X
	PARAMETER	IO$V_TRMNOECHO  = '0000000C'X
	PARAMETER	IO$V_INHEXTGAP  = '0000000C'X
	PARAMETER	IO$_READPBLK    = '0000000C'X
	PARAMETER	IO$V_REFRESH    = '0000000D'X
	PARAMETER	IO$_WRITEHEAD   = '0000000D'X
	PARAMETER	IO$V_DATACHECK  = '0000000E'X
	PARAMETER	IO$_READHEAD    = '0000000E'X
	PARAMETER	IO$_WRITETRACKD = '0000000F'X
	PARAMETER	IO$V_INHRETRY   = '0000000F'X
	PARAMETER	IO$_READTRACKD  = '00000010'X
	PARAMETER	IO$_REREADN     = '00000016'X
	PARAMETER	IO$_REREADP     = '00000017'X
	PARAMETER	IO$_WRITERET    = '00000018'X
	PARAMETER	IO$_WRITECHECKH = '00000018'X
	PARAMETER	IO$_READPRESET  = '00000019'X
	PARAMETER	IO$_STARTSPNDL  = '00000019'X
	PARAMETER	IO$_SETCHAR     = '0000001A'X
	PARAMETER	IO$_SENSECHAR   = '0000001B'X
	PARAMETER	IO$_WRITEMARK   = '0000001C'X
	PARAMETER	IO$_DIAGNOSE    = '0000001D'X
	PARAMETER	IO$_WRTTMKR     = '0000001D'X
	PARAMETER	IO$_FORMAT      = '0000001E'X
	PARAMETER	IO$_CLEAN       = '0000001E'X
	PARAMETER	IO$_PHYSICAL    = '0000001F'X
	PARAMETER	IO$_WRITELBLK   = '00000020'X
	PARAMETER	IO$_READLBLK    = '00000021'X
	PARAMETER	IO$_REWINDOFF   = '00000022'X
	PARAMETER	IO$_SETMODE     = '00000023'X
	PARAMETER	IO$_REWIND      = '00000024'X
	PARAMETER	IO$_SKIPFILE    = '00000025'X
	PARAMETER	IO$_SKIPRECORD  = '00000026'X
	PARAMETER	IO$_SENSEMODE   = '00000027'X
	PARAMETER	IO$_WRITEOF     = '00000028'X
	PARAMETER	IO$_LOGICAL     = '0000002F'X
	PARAMETER	IO$_WRITEVBLK   = '00000030'X
	PARAMETER	IO$_READVBLK    = '00000031'X
	PARAMETER	IO$_ACCESS      = '00000032'X
	PARAMETER	IO$_CREATE      = '00000033'X
	PARAMETER	IO$_DEACCESS    = '00000034'X
	PARAMETER	IO$_DELETE      = '00000035'X
	PARAMETER	IO$_MODIFY      = '00000036'X
	PARAMETER	IO$_SETCLOCK    = '00000037'X
	PARAMETER	IO$_READPROMPT  = '00000037'X
	PARAMETER	IO$_STARTDATA   = '00000038'X
	PARAMETER	IO$_ACPCONTROL  = '00000038'X
	PARAMETER	IO$_MOUNT       = '00000039'X
	PARAMETER	IO$_TTYREADALL  = '0000003A'X
	PARAMETER	IO$_TTYREADPALL = '0000003B'X
	PARAMETER	IO$_CONINTREAD  = '0000003C'X
	PARAMETER	IO$_CONINTWRITE = '0000003D'X
	PARAMETER	IO$M_FCODE      = '0000003F'X
	PARAMETER	IO$_VIRTUAL     = '0000003F'X
	PARAMETER	IO$M_ACCESS     = '00000040'X
	PARAMETER	IO$M_TYPEAHDCNT = '00000040'X
	PARAMETER	IO$M_INTERRUPT  = '00000040'X
	PARAMETER	IO$M_SETEVF     = '00000040'X
	PARAMETER	IO$M_BINARY     = '00000040'X
	PARAMETER	IO$M_NOECHO     = '00000040'X
	PARAMETER	IO$M_STARTUP    = '00000040'X
	PARAMETER	IO$M_NOW        = '00000040'X
	PARAMETER	IO$M_DELDATA    = '00000040'X
	PARAMETER	IO$M_COMMOD     = '00000040'X
	PARAMETER	IO$M_REVERSE    = '00000040'X
	PARAMETER	IO$M_CANCTRLO   = '00000040'X
	PARAMETER	IO$M_WORD       = '00000040'X
	PARAMETER	IO$M_MOVETRACKD = '00000080'X
	PARAMETER	IO$M_ENABLMBX   = '00000080'X
	PARAMETER	IO$M_CTRLYAST   = '00000080'X
	PARAMETER	IO$M_TIMED      = '00000080'X
	PARAMETER	IO$M_PACKED     = '00000080'X
	PARAMETER	IO$M_SHUTDOWN   = '00000080'X
	PARAMETER	IO$M_NOWAIT     = '00000080'X
	PARAMETER	IO$M_CREATE     = '00000080'X
	PARAMETER	IO$M_READATTN   = '00000080'X
	PARAMETER	IO$M_RESET      = '00000080'X
	PARAMETER	IO$M_ATTNAST    = '00000100'X
	PARAMETER	IO$M_DELETE     = '00000100'X
	PARAMETER	IO$M_CTRLCAST   = '00000100'X
	PARAMETER	IO$M_WRTATTN    = '00000100'X
	PARAMETER	IO$M_CVTLOW     = '00000100'X
	PARAMETER	IO$M_ABORT      = '00000100'X
	PARAMETER	IO$M_DIAGNOSTIC = '00000100'X
	PARAMETER	IO$M_INTSKIP    = '00000100'X
	PARAMETER	IO$M_NOFORMAT   = '00000100'X
	PARAMETER	IO$M_OPPOSITE   = '00000200'X
	PARAMETER	IO$M_SETFNCT    = '00000200'X
	PARAMETER	IO$M_HANGUP     = '00000200'X
	PARAMETER	IO$M_SYNCH      = '00000200'X
	PARAMETER	IO$M_MOUNT      = '00000200'X
	PARAMETER	IO$M_SKPSECINH  = '00000200'X
	PARAMETER	IO$M_NOFILTR    = '00000200'X
	PARAMETER	IO$M_DMOUNT     = '00000400'X
	PARAMETER	IO$M_DSABLMBX   = '00000400'X
	PARAMETER	IO$M_DATAPATH   = '00000400'X
	PARAMETER	IO$M_CECYL      = '00000400'X
	PARAMETER	IO$M_SWAP       = '00000400'X
	PARAMETER	IO$M_INHERLOG   = '00000800'X
	PARAMETER	IO$M_PURGE      = '00000800'X
	PARAMETER	IO$M_INHSEEK    = '00001000'X
	PARAMETER	IO$M_INHEXTGAP  = '00001000'X
	PARAMETER	IO$M_CYCLE      = '00001000'X
	PARAMETER	IO$M_TRMNOECHO  = '00001000'X
	PARAMETER	IO$M_REFRESH    = '00002000'X
	PARAMETER	IO$M_DATACHECK  = '00004000'X
	PARAMETER	IO$M_INHRETRY   = '00008000'X
