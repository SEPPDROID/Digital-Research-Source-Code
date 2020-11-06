C***********************************************************************
C
C                               PLM.FOR
C
C		D I S C L A I M E R   N O T I C E		      
C		-------------------   -----------
C
C  This document and/or portions of the material and data furnished
C  herewith, was developed under sponsorship of the U. S. Government.
C  Neither the U.S. nor the U.S.D.O.E., nor the Leland Stanford Junior
C  University, nor their employees, nor their respective contractors,
C  subcontractors, or their employees, makes any warranty, express or
C  implied, or assumes any liability or responsibility for accuracy,
C  completeness or usefulness of any information, apparatus, product
C  or process disclosed, or represents that its use will not infringe
C  privately-owned rights.  Mention of any product, its manufacturer,
C  or suppliers shall not, nor is it intended to, imply approval, dis-
C  approval, or fitness for any particular use.  The U. S. and the
C  University at all times retain the right to use and disseminate same
C  for any purpose whatsoever.  Such distribution shall be made by the
C  National Energy Software Center at the Argonne National Laboratory
C  and only subject to the distributee furnishing satisfactory proof
C  that he has a valid license from the Intel Corporation in effect.
C
C
C-----------------------------------------------------------------------
C
C    This is the main module for the PL/M-VAX compiler.  Default values
C    for controls are established, the invocation line is processed, a
C    compilation is performed, and the MACRO assembler is chained to
C    (if required).
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    VERSION 3.5  29APR81  AFH  FOR MODEL=1 (SMALL) ASSUME D.=0,
C                               AND MAKE APPROPRIATE SIMPLIFICATIONS.
C
C    VERSION 3.6  30APR81  AFH  IMPLEMENT PROCEDURE EXECUTION FREQUENCY
C                               COUNTS
C
C    VERSION 3.7  08MAY81  AFH  FIXED BUG IN CONTROL_LINE PROCEDURE
C                               WHICH COULD SOMETIMES CAUSE A CONTROL
C                               LINE TO BE IGNORED.  ALSO, CHECK
C			        SKIP_STATE AT END OF COMPILATION TO
C                               DETECT UNCLOSED CONDITIONAL COMPILATION
C				BLOCKS.
C
C    VERSION 3.8  14MAY81  AFH  1. Special handling for DO WHILE <const>
C				   (eliminates some spurious PATH warnings).
C				2. Fix invocation line bug introduced in
C                                  version 3.7.
C                               3. Allow spurious trailing comma in
C                                  initialization list (for compatibility
C                                  with PLM86).
C                               4. Put runtime statistics at end of listing.
C				5. Fix node_type(reg) bug in SOMEWHERE.
C                               6. Set attributes of all symbols in a
C                                  factored declaration before processing
C                                  the initialization list (in case list
C                                  contains restricted location refs to
C                                  any elements in the current declaration).
C
C    VERSION 3.9  AFH  15MAY81	1. Add STACK,STACKTOP,STACKPTR builtins.
C				2. Change default extent for PUBLICS to PBL.
C				3. Fix line #'s for unsequenced INCLUDE 
C				   files.
C
C    VERSION 4.0  AFH  28MAY81  (FIRST RELEASE TO INTEL.)
C				1. No traceback on FATAL error or BUG.
C				2. Fix 'input file not found' message
C				   to come out on terminal.
C				3. Try to type offending source line
C				   along with error message to terminal.
C
C    VERSION 4.1  AFH  14JUN81	1. Increase string space to 32K bytes.
C				2. Don't allow procedure as LHS of
C				   assignment statement.
C
C    VERSION 4.2  AFH  22JUN81  1. Temporary fix to allow dimensions>32K.
C
C    VERSION 4.3  AFH  23JUN81  (SECOND RELEASE TO INTEL.)
C                               1. Fix LAST,LENGTH,SIZE for dimension>32K.
C                               2. Generate in-line code for the following
C				   built-ins:
C				    DOUBLE,LOW,FLOAT,FIX,INT,SIGNED,
C				    UNSIGN.
C
C    VERSION 4.4  AFH  23JUN81  (THIRD RELEASE TO INTEL.)
C                               1. Allow AT(@external+offset).
C
C    VERSION 4.5  AFH  05JUL81	1. Implement ACALLS control.
C				2. Only two models (LARGE & SMALL),
C				   so make some simplifications.
C			 	3. Change FIND* library routine names
C				   to the unified versions.
C				4. Change most D.'s to 0's.
C				5. Generate 'MCOMB #0' instead of
C				   'MNEGB #1' for aesthetic reasons.
C				6. Generate word-displacement addressing
C				   for SMALL-model constants.
C				(Modules affected: PLMCOM, PLM, CONTROL,
C				PROCS, BASICS, LOCALS, MODULES, INIT,
C				EMIT, FOLD, GENCODE, OPERAND.)
C
C    VERSION 4.6  AFH  05JUL81	1. Allow %forward references in restricted
C				   location references.
C				2. Add ALIGN, FREQUENCIES, ACALLS to
C				   summary tail.
C
C    VERSION 4.7  AFH  16JUL81  1. Correct bug introduced by 4.5(6.).
C				   (addressing was wrong for SMALL model
C				   non-overlay module constants).
C				(Modules affected: PLM, OPERAND.)
C
C    VERSION 4.8  AFH  29JUL81	1. Add FRAME$PTR builtin.
C				(Modules affected: PLM, INIT, BUILTINS.)
C
C    VERSION 4.9  AFH  18AUG81  (FIFTH RELEASE TO INTEL.)
C                               1. Fold the special case of
C                                  symbol(const).member(const) where
C                                  element_size(symbol).ne.0
C				   (modulo element_size(member)).
C                               2. Increase max number of globals to 800.
C                               3. Fix bug in EXTRACT_DISPLACEMENT
C				   (downward/upward coercions are not
C				   transitive).
C				4. Change names of all out-of-line builtins
C				   to the unified versions.
C                               (Modules affected: PLM,PLMCOM,FOLD,INIT.)
C
C    VERSION 5.0  AFH  19AUG81	1. Support COMPACT and MEDIUM models as
C				   well as SMALL and LARGE.
C				2. Implement VECTOR control in place of
C				   ACALLS control.
C				(Modules affected: PLM,PLMCOM,SOMEWHERE,
C				CONTROL,OPERAND,SUMMARY,UNIQUE,PROCS,
C				BASICS,REGS,MODULES,EXPRS,INIT,EMIT,UNITS.)
C
C    VERSION 5.1  AFH  08SEP81  1. Compute reference counts (resolves
C				   semantic ambiguity of multiple assign-
C				   ment statements in favor of PL/M-86
C				   interpretation.
C				(Modules affected: PLM,SOMEWHERE,BASICS,
C				MASSAGE,COUNTS.)
C
C    VERSION 5.2  AFH  13SEP81  1. Implement the ALIGN control.
C				(Modules affected: PLM,MODULES,DECLS.)
C    
C    VERSION 5.3  AFH  29SEP81  1. Fix CRC-0 bug on reference to STACK$PTR.
C				2. Correct choice of SP for STACK$PTR.
C				3. Increase symbol table to 2000 entries.
C				4. Allow DATA attribute with EXTERNAL.
C				5. Allow dimensions >64K.
C				6. Allow structure member arrays to have
C				   explicit lower bounds.
C				7. Implement the builtin function FIRST.
C				8. Support the AT(@external.member)
C				   construct.
C				9. Increase max string size to 290 chars
C				   (for larger LITERALLY's).
C				(Modules changed: PLMCOM,PLM,COUNTS,
C                               BUILTINS,SYMTAB,DATA,DECLS,EMIT,REPLICA,
C				INIT,GETLEX.  All modules were recompiled
C				because of changes to PLMCOM.)
C
C    VERSION 5.4  AFH  15OCT81	1. Experimental version to try out
C				   reference count stuff.
C                               (Modules changed: PLM,COUNTS,SOMEWHERE,
C				REGS.)
C
C    VERSION 5.5  AFH  21OCT81	1. Add basic block analysis.
C				2. Implement %_signed and %_unsigned
C				   builtins.
C
C    VERSION 5.6  AFH  23OCT81  1. More peephole optimizations.
C				2. Add OP_BB operator.
C				3. No reference counts for OP_LOC
C				   and OP_ASSN.
C
C    VERSION 5.7  AFH  28OCT81  1. Add definitions for SELECTOR, DWORD,
C				   SHORT, and BOOLEAN data types.
C				2. Allow keywords to be re-declared.
C
C    VERSION 5.8  AFH  06NOV81  1. Add ASSUME control.
C
C    VERSION 5.9  AFH  09NOV81  1. Implement CSE,CTE,BBA,MCO assumptions.
C
C    VERSION 6.0  AFH  10NOV81  1. Add EFFECTS module.
C				2. Add DBG assumption.
C				3. Fix DRC bug in SCOPES.
C				4. Implement EEQ,BRO,SWB assumptions.
C
C    VERSION 6.1  AFH  12NOV81	1. Restore argument pointer display in
C				   transfer vector prologue.
C				2. Change psect names, and add the
C				   symbol_psect field to the symbol
C				   table.
C				3. Make ATOM_DISP be I*4. (All modules
C				   must be recompiled.)
C				4. Implement LAST(MEMORY), etc.
C				5. Allow structure arrays to be implicitly
C				   dimensioned.
C				6. Implement AT(arg) and AT(dynamic).
C
C    VERSION 6.2  AFH  14NOV81  1. Change addressing modes to reflect
C				   new psect usage.
C
C    VERSION 6.3  AFH  21NOV81  1. Temporarily change LOW back to an
C                                  external to correct a bug with
C				   extract_displacement.
C
C    VERSION 6.4  AFH  10JAN82  1. Change DOUBLE keyword to DOUBLE$-
C				   PRECISION to avoid conflict with
C				   the DOUBLE builtin.
C    				2. Set VMS delimiter set in CONTROL.
C
C    VERSION 6.5  AFH  14JAN82  1. Change ASSUME_S32 to ASSUME_S64.
C				2. Ignore $-signs in switch names.
C				3. Make <keyword>: and GOTO <keyword>
C				   work correctly.
C
C    VERSION 6.6  AFH  03FEB82  1. Fix bug for immediate operands
C			   	   under LARGE model (OPERAND).
C			        2. Change name of GET_CNTRL_FLD.
C
C    VERSION 6.7  AFH  08FEB82  1. Merge ARG opnodes.
C    				2. Change opcode column in emitted code
C				   to allow longer emitted code lines.
C
C***********************************************************************
 
      PROGRAM PLM
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*45 FILE1_CHARS,FILE2_CHARS,FILE3_CHARS
 
      VERSION=6.7
      IN=8
      OUT=4
      LST=6
      IXI=3
      FIFO_DEPTH=0
      LIST_LINE_NO=0
      LIST_STNO=1
      LIST_BLOCK_LEVEL=0
      LINE_OF_PAGE=1
      PAGE_NO=0
      LINES_READ=0
      ERRORS=0
      WARNINGS=0
      PREVIOUS_STNO=0
      EXTERNAL_SERIAL_DELTA=0
      BASED_SERIAL_DELTA=0
      SUBSCRIPTED_SERIAL_DELTA=0
      OVERLAID_SERIAL_DELTA=0
      PATH=.FALSE.
      BASIC_BLOCK=NULL
      CALL BREAK
 
C------- SET DEFAULT VALUES OF PRIMARY CONTROLS.
 
      LARGE=.FALSE.
      PAGELENGTH=LIB$LP_LINES()-5
      PAGEWIDTH=120
      OPTIMIZE=1
      MODEL=1
      PRINT_FLAG=.TRUE.
      XREF_FLAG=.FALSE.
      IXREF_FLAG=.FALSE.
      SYMBOLS_FLAG=.FALSE.
      PAGING_FLAG=.TRUE.
      INTVECTOR_FLAG=.TRUE.
      OBJECT_FLAG=.TRUE.
      OPRINT_FLAG=.FALSE.
      DEBUG_FLAG=.FALSE.
      TYPE_FLAG=.TRUE.
      ROM_FLAG=.FALSE.
      TITLE_STRING(0)=0
      TABS=8
      WARN_FLAG=.TRUE.
      PLM80_FLAG=.FALSE.
      GLOBALS_FLAG=.FALSE.
      PUBLICS_FLAG=.FALSE.
      OVERLAY_FLAG=.FALSE.
      ROOT_FLAG=.FALSE.
      ALIGN_FLAG=.FALSE.
      FREQ_FLAG=.FALSE.
      VECTOR_FLAG=.FALSE.
      
      CALL DATE(DATE_STRING(1))
      DATE_STRING(10)=' '
      CALL TIME(DATE_STRING(11))
      DATE_STRING(0)=18
 
C------- SET DEFAULT VALUES OF GENERAL CONTROLS.
 
      LEFTMARGIN=1
      RIGHTMARGIN=200
      LIST_FLAG=.TRUE.
      NON_CONTROL_LINE_READ=.FALSE.
      SKIP_STATE=0              ! READING INVOCATION LINE.
      CODE_FLAG=.FALSE.
      EJECT_FLAG=.TRUE.
      OVERFLOW_FLAG=.FALSE.
      COND_FLAG=.TRUE.
      SUBTITLE_STRING(0)=0
 
C-------- SET DEFAULT VALUES OF ASSUMPTION FLAGS.
 
      ASSUME_SCE=.TRUE.
      ASSUME_CSE=.TRUE.
      ASSUME_EEQ=.TRUE.
      ASSUME_PSE=.TRUE.
      ASSUME_BRO=.TRUE.
      ASSUME_BBA=.TRUE.
      ASSUME_CTE=.TRUE.
      ASSUME_MCO=.TRUE.
      ASSUME_CFA=.TRUE.
      ASSUME_SWB=.TRUE.
      ASSUME_OSR=.TRUE.
      ASSUME_SVE=.TRUE.
      ASSUME_S64=.TRUE.
      ASSUME_C7F=.TRUE.
      ASSUME_DBG=.FALSE.
 
C-------- PERFORM A COMPILATION.
 
      CALL INVOCATION_LINE
      SKIP_STATE=4		! READING AT LEVEL 0.
      CALL GETC
      CALL GETLEX
      CALL GETTOK
      CALL COMPILATION
 
C-------- CHAIN TO MACRO IF OBJECT WANTED.
 
      IF (OBJECT_FLAG) THEN
         IF (OPRINT_FLAG) THEN
            CALL LIB$DO_COMMAND(
     #         'MAC/OBJ=' //
     #         FILE1_CHARS(:MAKE_CHARS(FILE1_CHARS,OBJECT_FILE_STRING))
     #         // '/LIS=' //
     #         FILE2_CHARS(:MAKE_CHARS(FILE2_CHARS,OPRINT_FILE_STRING))
     #         // ' ' //
     #         FILE3_CHARS(:MAKE_CHARS(FILE3_CHARS,WORK_FILE_STRING)))
         ELSE
            CALL LIB$DO_COMMAND(
     #         'MAC/OBJ=' //
     #         FILE1_CHARS(:MAKE_CHARS(FILE1_CHARS,OBJECT_FILE_STRING))
     #         // '/NOLIS ' //
     #         FILE3_CHARS(:MAKE_CHARS(FILE3_CHARS,WORK_FILE_STRING)))
         ENDIF
      ENDIF
 
      END
