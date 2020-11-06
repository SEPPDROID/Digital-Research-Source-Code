;--------------------------------------------------------
;
;	PROGRAM ID:	GBIOS3.ASM
;			GENERIC LOADER AND BIOS FOR CP/M PLUS
;			FOR CP/M-80 V2.2 SYSTEMS
;			UNBANKED
;			NO HASHING
;			BIOS PERFORMS BLOCKING/DEBLOCKING
;
;--------------------------------------------------------
;
;	ORGANIZATIONAL COMMENTARY
;
;--------------------------------------------------------
;	This BIOS is designed to be the user's first step
;in making an upgrade from CP/M-80 v2.2 to CP/M Plus v3.0.
;This BIOS will function properly in most CP/M-2 systems,
;even those with the BIOS in PROM.
;
;FUNCTION:
;	This PROGRAM implements an UNBANKED, NON-HASHED,
;UNBUFFERED BIOS for CP/M Plus version 3.0.
;
;METHOD:
;	CP/M Plus allows many levels of sophistocation in
;the implementation of the BIOS.  At the highest levels,
;the BIOS allows CP/M Plus to manage a potentially large
;memory pool which acts like an instant access disk drive;
;a substantial performence enhancement.  At the lowest level,
;the performance of CP/M Plus is the same as that of CP/M-80
;version 2.2.  This interface program is a simple implementation
;of a CP/M Plus BIOS which operates as a parasite to the user's
;existing BIOS for CP/M-80 version 2.2.
;
;PURPOSE:
;	This BIOS allows the CP/M-80 v2.2 user to easily
;upgrade to CP/M Plus v3.0 and enjoy its logical enhancements
;at the same level of performance he has enjoyed with v2.2.
;This BIOS can be an end-point since system performance
;will be satisfactory.  For users who own the hardware
;required to implement hashing and buffering, this BIOS
;will serve as a welcome starting point for further enhancement.
;
;ORGANIZATION:
;	This BIOS is organized as follows:
;
;	CONSTANTS
;	CONFIGURATION CONSTANTS AND CONDITIONS  ***
;	EXTERNAL REFERENCES
;	PUBLIC DECLARATIONS	(NULL)
;	RAM DEFINITIONS
;	CP/M 2.2 LINKAGE EQUATES
;
;	CP/M PLUS JUMP TABLE
;	INITIALIZATION PROCESSES
;	LOCAL AND NULL PROCESSES
;	SIMPLE INDEXED PROCESSES
;	COMPLEX INDEXED PROCESSES
;	LOCAL SUBROUTINES
;	INITIALIZED STORAGE
;	UNINITIALIZED STORAGE
;	END
;
;	***	edit these
;--------------------------------------------------------
;
;	OPERATIONAL COMMENTARY
;
;--------------------------------------------------------
;
;	Digital Research owes much of its success to the
;foresight of its operating system designers who have seen
;fit to make their products upward compatible with one
;another.  CP/M Plus is no exception, and so it is no
;surprise that the CP/M Plus BIOS shares many constructs
;with the earlier CP/M-80 v2.2.
;
;	The CP/M Plus BIOS implements 32 entry points in
;its jump table.  Of these, the first 17 correspond to the
;17 entries in the CP/M-80 v2.2 jump table.  14 of these
;are treated as identical to those in 2.2.  These index
;directly into the 2.2 jump table.  The remaining three in the
;2.2 set are BOOT, which is null, WBOOT, which loads CCP.COM,
;and SELDSK which creates all of the necessary disk data
;structures required by CP/M Plus based on those in the 2.2 BIOS.
;The remaining 15 entries in the CP/M Plus BIOS, with the
;single exception of MOVE, are implemented in one of
;the following ways:
;
;	1.)	null, simply return.
;	2.)	nearly null, do something really simple.
;	3.)	tell a white lie.
;
;--------------------------------------------------------
;
;	constants
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	configuration constants and conditions
;
;--------------------------------------------------------
;ldrbios	equ	true
;--------------------------------------------------------
;
;	external references
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	public declarations
;
;--------------------------------------------------------
;
;currently null
;
;--------------------------------------------------------
;
;	ram definitions
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	cp/m v2.2 linkage equates
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	cp/m plus version 3.0 jump table
;
;--------------------------------------------------------
;	org	(bios-(200h+64*drives)) and 0ff00h
;
;
;
;--------------------------------------------------------
;
;	initialization processes
;
;--------------------------------------------------------
;
;	initialize low memory jumps
;
;
;	load ccp.com into tpa
;
;
;	the load loop
;
;
;
;
;
;--------------------------------------------------------
;
;	local and null processes
;
;--------------------------------------------------------
;	z80 users may code as:
;	xchg
;	db	0edh,0b0h	;ldir
;	xchg
;	ret
;
;--------------------------------------------------------
;
;	simple indexed processes
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	complex indexed functions
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	local subroutines
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	initialized storage
;
;--------------------------------------------------------
;--------------------------------------------------------
;
;	uninitialized storage
;
;--------------------------------------------------------
;--------------------------------------------------------
;cp/m is a registered trademark of digital research, inc.
;you already knew that, but they wanted us to remind you.
;--------------------------------------------------------
