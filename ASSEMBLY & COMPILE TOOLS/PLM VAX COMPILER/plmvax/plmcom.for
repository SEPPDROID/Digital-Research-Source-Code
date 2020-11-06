C***********************************************************************
C
C                               PLMCOM.FOR
C
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
C    This include-file supplies all global definitions for the
C    PL/M-VAX compiler.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. Increase SYMBOL_MAX. (V5.3)
C			   2. Increase max string size to 290.
C			   3. Change relevant SYMBOL and MEMBER arrays
C			      to INTEGER*4.
C			   4. Add MEMBER_LOWER_BOUND array to support
C			      lower bounds for structure member arrays.
C			   5. Add MEMBER_OFFSET array to support the
C			      AT(@external.member) construct.
C    21OCT81  Alex Hunter  1. Add stuff for basic block analysis. (V5.5)
C			   2. Add OP_SIGNED and OP_UNSIGNED operators.
C    28OCT81  Alex Hunter  1. Add new keywords (SELECTOR-REGISTER). (V5.7)
C                          2. Add new symbol type attributes.
C    09NOV81  Alex Hunter  1. Add assumption flags. (V5.8)
C    10NOV81  Alex Hunter  1. Add S_NO_SIDE_EFFECTS, ASSUME_DBG, and
C			      serial no. deltas. (V6.0)
C    12NOV81  Alex Hunter  1. Add S_REGISTER,S_SPECIAL,SYMBOL_PSECT,
C			      SYM_MLAST, et al. (V6.1)
C			   2. Change ATOM_DISP to I*4.
C			   3. Delete predefined atoms.
C			   4. Add new PSECTS and change names.
C    14JAN82  Alex Hunter  1. Change ASSUME_S32 to ASSUME_S64. (V6.5)
C
C-----------------------------------------------------------------------
C
      IMPLICIT INTEGER*2 (A-Z)
 
C-----------------------------------------------------------------------
C
C    Reserved word token values.
C
C-----------------------------------------------------------------------
      PARAMETER K_ADDRESS=101, K_AND=102, K_AT=103, K_BASED=104,
     #          K_BY=105, K_BYTE=106, K_CALL=107, K_CASE=108,
     #          K_DATA=109, K_DECLARE=110, K_DISABLE=111, K_DO=112,
     #          K_ELSE=113, K_ENABLE=114, K_END=115, K_EOF=116,
     #          K_EXTERNAL=117, K_GO=118, K_GOTO=119, K_HALT=120,
     #          K_IF=121, K_INITIAL=122, K_INTEGER=123,
     #          K_INTERRUPT=124, K_LABEL=125, K_LITERALLY=126,
     #          K_MINUS=127, K_MOD=128, K_NOT=129, K_OR=130,
     #          K_PLUS=131, K_POINTER=132, K_PROCEDURE=133,
     #          K_PUBLIC=134, K_REAL=135, K_REENTRANT=136,
     #          K_RETURN=137, K_STRUCTURE=138, K_THEN=139, K_TO=140,
     #          K_WHILE=141, K_WORD=142, K_XOR=143,
     #          K_COMMON=144, K_LONG=145, K_DOUBLE=146, K_OTHERWISE=147,
     #          K_QUAD=148,K_FORWARD=149,K_SELECTOR=150,K_DWORD=151,
     #		K_SHORT=152,K_BOOLEAN=153,K_REGISTER=154
 
C-----------------------------------------------------------------------
C
C    Delimiter token values.
C
C-----------------------------------------------------------------------
      PARAMETER D_PLUS=201, D_MINUS=202, D_STAR=203, D_SLASH=204,
     #          D_LT=205, D_GT=206, D_EQ=207, D_NE=208, D_LE=209,
     #          D_GE=210, D_ASSN=211, D_COLON=212, D_SEMI=213,
     #          D_DOT=214, D_COMMA=215, D_LP=216, D_RP=217, D_AT=218
      
C-----------------------------------------------------------------------
C
C    Non-terminal token values.
C
C-----------------------------------------------------------------------
      PARAMETER NT_STATEMENT=301,NT_EXPRESSION=302,NT_TYPE=303
 
 
C-----------------------------------------------------------------------
C
C    Controls.
C
C-----------------------------------------------------------------------
      INTEGER*2 LEFTMARGIN,RIGHTMARGIN,SKIP_STATE,PAGELENGTH,
     #          PAGEWIDTH,OPTIMIZE,MODEL,TABS
      LOGICAL*1 LIST_FLAG,LARGE,NON_CONTROL_LINE_READ
      LOGICAL*1 PRINT_FLAG,CODE_FLAG,XREF_FLAG,IXREF_FLAG,SYMBOLS_FLAG,
     #          PAGING_FLAG,EJECT_FLAG,INTVECTOR_FLAG,OBJECT_FLAG,
     #          OVERFLOW_FLAG,DEBUG_FLAG,TYPE_FLAG,ROM_FLAG,COND_FLAG,
     #          OPRINT_FLAG,WARN_FLAG,PLM80_FLAG,GLOBALS_FLAG,
     #          PUBLICS_FLAG,OVERLAY_FLAG,ROOT_FLAG,ALIGN_FLAG,
     #          FREQ_FLAG,VECTOR_FLAG
      PARAMETER MAX_IN=20
      BYTE PRINT_FILE_STRING(0:45),IXREF_FILE_STRING(0:45),
     #     WORK_FILE_STRING(0:45),OBJECT_FILE_STRING(0:45),
     #     DATE_STRING(0:80),TITLE_STRING(0:80),SUBTITLE_STRING(0:80),
     #     IN_FILE_STRING(0:45,8:MAX_IN+1),OPRINT_FILE_STRING(0:45),
     #     GLOBALS_FILE_STRING(0:45),PUBLICS_FILE_STRING(0:45),
     #     OVERLAY_PREFIX(0:80)
      COMMON/CONTROLS/ LEFTMARGIN,RIGHTMARGIN,LIST_FLAG,LARGE,
     #     NON_CONTROL_LINE_READ,SKIP_STATE,PAGELENGTH,PAGEWIDTH,
     #     OPTIMIZE,MODEL,PRINT_FLAG,CODE_FLAG,XREF_FLAG,IXREF_FLAG,
     #     SYMBOLS_FLAG,PAGING_FLAG,EJECT_FLAG,INTVECTOR_FLAG,
     #     OBJECT_FLAG,OVERFLOW_FLAG,DEBUG_FLAG,TYPE_FLAG,ROM_FLAG,
     #     COND_FLAG,
     #     PRINT_FILE_STRING,IXREF_FILE_STRING,WORK_FILE_STRING,
     #     OBJECT_FILE_STRING,DATE_STRING,TITLE_STRING,
     #     SUBTITLE_STRING,IN_FILE_STRING,OPRINT_FILE_STRING,
     #     OPRINT_FLAG,TABS,WARN_FLAG,PLM80_FLAG,GLOBALS_FLAG,
     #     PUBLICS_FLAG,OVERLAY_FLAG,ROOT_FLAG,GLOBALS_FILE_STRING,
     #     PUBLICS_FILE_STRING,OVERLAY_PREFIX,ALIGN_FLAG,FREQ_FLAG,
     #     VECTOR_FLAG
 
C-----------------------------------------------------------------------
C
C    Character stream input and macro expansion stuff.
C
C-----------------------------------------------------------------------
      PARAMETER LITMAX=16
      PARAMETER EOLCHAR='01'X, EOFCHAR='02'X
      INTEGER*2 COL, LITLEV, LITCOL(LITMAX)
      CHARACTER*1 CHAR, EOL, EOF, TAB
      CHARACTER*300 LITVAL(LITMAX), CARD
      EQUIVALENCE (CARD,LITVAL(1))
      COMMON /LEXICAL/ COL,LITLEV,LITCOL
      COMMON /LEXCHAR/ LITVAL,CHAR,EOL,EOF,TAB
      DATA COL/72/, CARD(73:73)/EOLCHAR/, LITLEV/1/
      DATA EOL/EOLCHAR/, EOF/EOFCHAR/, TAB/'09'X/
 
C-----------------------------------------------------------------------
C
C    Lexical token analysis stuff.
C
C-----------------------------------------------------------------------
      PARAMETER STRING_SIZE_MAX=290    ! (V5.3)
      CHARACTER DELIMITER*2, IDENTIFIER*32, STRING*(STRING_SIZE_MAX)
      CHARACTER NEXT_DELIMITER*2, NEXT_IDENTIFIER*32,
     #          NEXT_STRING*(STRING_SIZE_MAX)
      REAL*8 FLOATVAL, NEXT_FLOATVAL
      INTEGER*4 FIXVAL, NEXT_FIXVAL
      INTEGER*2 TOKENTYPE, TT, STRLEN
      INTEGER*2 NEXT_TOKENTYPE, NEXT_STRLEN
      PARAMETER INVALID=0, DELIM=1, ID=2, FIXCON=3, FLOATCON=4,
     #          STRCON=5, EOFTOK=6
      COMMON /TOKEN/ TOKENTYPE,FIXVAL,FLOATVAL,STRLEN,
     #               NEXT_TOKENTYPE,NEXT_FIXVAL,NEXT_FLOATVAL,
     #               NEXT_STRLEN
      COMMON /TOKENCHAR/ DELIMITER,IDENTIFIER,STRING,
     #            NEXT_DELIMITER,NEXT_IDENTIFIER,NEXT_STRING
      EQUIVALENCE (TT,TOKENTYPE)
 
C-----------------------------------------------------------------------
C
C    I/O unit definitions.
C
C-----------------------------------------------------------------------
      COMMON /IO/ IN,OUT,LST,IXI,GBL,PUB
      DATA IN/8/, OUT/4/, LST/6/, IXI/3/, GBL/1/, PUB/2/
 
C-----------------------------------------------------------------------
C
C    Label structures.
C
C-----------------------------------------------------------------------
      PARAMETER MAX_LABELS=10
      CHARACTER*32 LABELS(MAX_LABELS), LAST_LABEL
      INTEGER*2 NLABELS
      COMMON /LABEL/ NLABELS
      COMMON /LABELC/ LABELS, LAST_LABEL
      DATA NLABELS /0/
 
C-----------------------------------------------------------------------
C
C    Symbol attribute values.
C
C-----------------------------------------------------------------------
      PARAMETER S_MACRO=1,S_SCALAR=2,S_ARRAY=3,S_PROC=4,S_LABEL=5,
     #          S_KEYWORD=6
 
      PARAMETER S_BYTE=1,S_WORD=2,S_INTEGER=3,S_PTR=4,S_REAL=5,
     #          S_LONG=6,S_DOUBLE=7,S_QUAD=8,S_SHORT=9,S_DWORD=10,
     #          S_SELECTOR=11,S_BOOLEAN=12,
     #          S_STRUC=100
 
      PARAMETER S_EXT=1,S_STATIC=2,S_BASED=3,S_ARG=4,S_FORWARD=5,
     #          S_DYNAMIC=6,           S_VALUE=8,S_UNRESOLVED=9,
     #          S_BUILTIN=10,S_LOCAL=11,S_REGISTER=12
 
      PARAMETER S_PUBLIC=1,S_UNDEF=2,S_INTERRUPT=4,S_REENT=8,
     #          S_FORCE_STATIC=16,S_DATA=32,S_OVERLAID=64,
     #          S_SAME_OVERLAY=128,S_NOTPUBLIC=256,
     #          S_NO_SIDE_EFFECTS=512,S_SPECIAL=1024
 
 
C-----------------------------------------------------------------------
C
C    Symbol table structures.
C
C-----------------------------------------------------------------------
      PARAMETER SYMBOL_MAX=2000, MEMBER_MAX=500, PARAM_MAX=100
      PARAMETER SYM_MLAST=1,SYM_MLEN=2,SYM_MSIZ=3,
     #          SYM_SLAST=4,SYM_SLEN=5,SYM_SSIZ=6,
     #          FIRST_AVAILABLE_SYMBOL_INDEX=7
 
      CHARACTER*32 SYMBOL_PLM_ID(SYMBOL_MAX), SYMBOL_VAX_ID(SYMBOL_MAX)
      INTEGER*2 SYMBOL_KIND(SYMBOL_MAX), SYMBOL_TYPE(SYMBOL_MAX),
     #          SYMBOL_LINK(SYMBOL_MAX), SYMBOL_LIST_SIZE(SYMBOL_MAX),
     #          SYMBOL_REF(SYMBOL_MAX), SYMBOL_BASE(SYMBOL_MAX),
     #          SYMBOL_BASE_MEMBER(SYMBOL_MAX), 
     #          SYMBOL_FLAGS(SYMBOL_MAX), SYMBOL_INDEX,
     #          SYMBOL_CHAIN(SYMBOL_MAX),
     #          SYMBOL_SERIAL_NO(SYMBOL_MAX),
     #          SYMBOL_PSECT(SYMBOL_MAX)
      INTEGER*4 SYMBOL_DISP(SYMBOL_MAX),SYMBOL_NBR_ELEMENTS(SYMBOL_MAX),
     #          SYMBOL_LOWER_BOUND(SYMBOL_MAX),
     #          SYMBOL_ELEMENT_SIZE(SYMBOL_MAX)
      LOGICAL*4 SAME_OVERLAY
      COMMON/SYMBOLC/SYMBOL_PLM_ID,SYMBOL_VAX_ID
      COMMON/SYMBOL/SYMBOL_KIND,SYMBOL_TYPE,
     #          SYMBOL_LINK,SYMBOL_LIST_SIZE,
     #          SYMBOL_REF,SYMBOL_BASE,SYMBOL_BASE_MEMBER,
     #          SYMBOL_FLAGS,SYMBOL_INDEX,SYMBOL_CHAIN,SAME_OVERLAY,
     #          SYMBOL_SERIAL_NO,SYMBOL_PSECT
      COMMON/SYMBOL4/SYMBOL_DISP,SYMBOL_NBR_ELEMENTS,
     #               SYMBOL_LOWER_BOUND,SYMBOL_ELEMENT_SIZE
 
C-----------------------------------------------------------------------
C
C    Member-symbol table structures.
C
C-----------------------------------------------------------------------
      CHARACTER*32 MEMBER_PLM_ID(MEMBER_MAX), MEMBER_VAX_ID(MEMBER_MAX)
      INTEGER*2 MEMBER_KIND(MEMBER_MAX), MEMBER_TYPE(MEMBER_MAX),
     #          MEMBER_INDEX,MEMBER_SERIAL_NO(MEMBER_MAX)
      INTEGER*4 MEMBER_NBR_ELEMENTS(MEMBER_MAX),
     #          MEMBER_LOWER_BOUND(MEMBER_MAX),
     #          MEMBER_ELEMENT_SIZE(MEMBER_MAX),
     #          MEMBER_OFFSET(MEMBER_MAX)
      COMMON/MEMBERC/MEMBER_PLM_ID,MEMBER_VAX_ID
      COMMON/MEMBER/MEMBER_KIND,MEMBER_TYPE,MEMBER_INDEX,
     #		MEMBER_SERIAL_NO
      COMMON/MEMBER4/MEMBER_NBR_ELEMENTS,MEMBER_LOWER_BOUND,
     #               MEMBER_ELEMENT_SIZE,MEMBER_OFFSET
 
C-----------------------------------------------------------------------
C
C    Parameter list structures.
C
C-----------------------------------------------------------------------
      INTEGER*2 PARAM_TYPE(PARAM_MAX)
      COMMON/PARAM/PARAM_TYPE
 
C-----------------------------------------------------------------------
C
C    Block scope structures.
C
C-----------------------------------------------------------------------
      PARAMETER BLOCK_MAX=20
      INTEGER*2 BLOCK_LEVEL,SYMBOL_TOP(0:BLOCK_MAX),
     #          MEMBER_TOP(0:BLOCK_MAX),PARAM_TOP(0:BLOCK_MAX),
     #          STRINGS_TOP(0:BLOCK_MAX)
      COMMON/BLOCK/BLOCK_LEVEL,SYMBOL_TOP,MEMBER_TOP,PARAM_TOP,
     #          STRINGS_TOP
      DATA BLOCK_LEVEL/0/
      DATA MEMBER_TOP(0)/0/,PARAM_TOP(0)/0/,
     #     STRINGS_TOP(0)/0/
 
C-----------------------------------------------------------------------
C
C    String space structures.
C
C-----------------------------------------------------------------------
      PARAMETER STRINGS_MAX=32000
      CHARACTER*(STRINGS_MAX) STRINGS
      COMMON/STRINGS/STRINGS
 
C-----------------------------------------------------------------------
C
C    Miscellaneous stuff.
C
C-----------------------------------------------------------------------
      INTEGER*2 BYTE_SIZE(S_BYTE:S_QUAD)
      COMMON/TABLES/BYTE_SIZE
      DATA BYTE_SIZE/1,2,2,4,4,4,8,8/
 
      PARAMETER NULL=0, DUMMY=0
 
      PARAMETER R0=16
 
C-----------------------------------------------------------------------
C
C    Node space definitions.
C
C-----------------------------------------------------------------------
      PARAMETER NODE_MIN=20,NODE_MAX=200,
     #          REG_MIN=1,REG_MAX=16,
     #          ANY_WHERE=-3,ANY_REG=-1,ON_STACK=-2,
     #          CON_MIN=-9,CON_MAX=-4,
     #          ATOM_MIN=-200,ATOM_MAX=-10,
     #          FIRST_FREE_ATOM=ATOM_MIN,
     #          FIX_MIN=-300,FIX_MAX=-201,
     #          FLT_MIN=-400,FLT_MAX=-301
 
C-----------------------------------------------------------------------
C
C    Operator node structures.
C
C-----------------------------------------------------------------------
      INTEGER*2 OPNODE_OP(NODE_MIN:NODE_MAX),
     #          OPNODE_OPND1(NODE_MIN:NODE_MAX),
     #          OPNODE_OPND2(NODE_MIN:NODE_MAX),
     #          NEXT_NODE
 
C-----------------------------------------------------------------------
C
C    Atom node structures.
C
C-----------------------------------------------------------------------
      INTEGER*2 ATOM_SYM(ATOM_MIN:ATOM_MAX),
     #          ATOM_MEM(ATOM_MIN:ATOM_MAX),
     #          ATOM_BASE(ATOM_MIN:ATOM_MAX),
     #          ATOM_SUB(ATOM_MIN:ATOM_MAX),
     #          ATOM_FLAGS(ATOM_MIN:ATOM_MAX),
     #		ATOM_SERIAL_NO(ATOM_MIN:ATOM_MAX),
     #          NEXT_ATOM
      INTEGER*4 ATOM_DISP(ATOM_MIN:ATOM_MAX)
 
      PARAMETER A_L2P=1,A_P2L=2,A_IMMEDIATE=4,A_CTIM=8,A_VECTOR=16
 
C-----------------------------------------------------------------------
C
C    Literal and constant node structures.
C
C-----------------------------------------------------------------------
      INTEGER*4 FIXED_VAL(FIX_MIN:FIX_MAX),
     #          NEXT_FIXED
 
      REAL*8    FLOAT_VAL(FLT_MIN:FLT_MAX)
      INTEGER*2 NEXT_FLOAT
 
      INTEGER*2 CONSTANT_LABEL(CON_MIN:CON_MAX),
     #          NEXT_CONSTANT
 
C-----------------------------------------------------------------------
C
C    Structures common to all nodes.
C
C-----------------------------------------------------------------------
      INTEGER*2 NODE_REG(FLT_MIN:NODE_MAX),
     #          NODE_REFCT(FLT_MIN:NODE_MAX),
     #          NODE_TYPE(FLT_MIN:NODE_MAX),
     #          NODE_CONTEXT(FLT_MIN:NODE_MAX)
 
C-----------------------------------------------------------------------
C
C    Code tree common block.
C
C-----------------------------------------------------------------------
      COMMON/TREE/OPNODE_OP,OPNODE_OPND1,OPNODE_OPND2,
     #            ATOM_SYM,ATOM_MEM,ATOM_BASE,
     #            ATOM_SUB,ATOM_DISP,
     #            FIXED_VAL,FLOAT_VAL,
     #            NODE_REG,NODE_REFCT,NODE_CONTEXT,NODE_TYPE,
     #            NEXT_NODE,NEXT_ATOM,NEXT_FIXED,NEXT_FLOAT,
     #            CONSTANT_LABEL,NEXT_CONSTANT,ATOM_FLAGS,
     #		  ATOM_SERIAL_NO
 
C-----------------------------------------------------------------------
C
C    Context resolution stuff.
C
C-----------------------------------------------------------------------
      PARAMETER CX_UNSIGNED=1, CX_SIGNED=2
 
      INTEGER*2 CONTEXT(S_BYTE:S_QUAD)
      COMMON /CX/ CONTEXT
      DATA CONTEXT
     # /CX_UNSIGNED,CX_UNSIGNED,CX_SIGNED,CX_UNSIGNED,CX_SIGNED,
     #  CX_SIGNED,CX_SIGNED,CX_SIGNED/
 
C-----------------------------------------------------------------------
C
C    Miscellaneous declarations.
C
C-----------------------------------------------------------------------
      LOGICAL*4 ATOM,NODE,LITERAL,FIXLIT,FLOATLIT,CONSTANT,REGISTER
      CHARACTER*32 LOCAL_LABEL
 
C-----------------------------------------------------------------------
C
C    Operator value definitions.
C
C-----------------------------------------------------------------------
      PARAMETER OP_NOP=0,
     #    OP_ADD=1,OP_SUB=2,OP_MUL=3,OP_DIV=4,OP_ADWC=5,OP_SBWC=6,
     #    OP_NEG=7,OP_NOT=8,OP_EXT=9,OP_OR=10,OP_XOR=11,OP_LT=12,
     #    OP_GT=13,OP_EQ=14,OP_NE=15,OP_LE=16,OP_GE=17,OP_LOC=18,
     #    OP_ASSN=19,OP_MOD=20,OP_THEN=21,OP_BIT=22,OP_ALSO=23,
     #    OP_CALL=24,OP_ARG=25,OP_AND=26,OP_MOV=27,
     #    OP_SIGNED=71,OP_UNSIGNED=72,
     #    OP_BYTE=81,OP_WORD=82,OP_INTEGER=83,OP_PTR=84,
     #    OP_REAL=85,OP_LONG=86,OP_DOUBLE=87,OP_QUAD=88,
     #    OP_B2W=101,OP_B2I=102,OP_B2L=103,OP_B2R=104,OP_W2B=105,
     #    OP_W2L=106,OP_I2B=107,OP_I2R=108,OP_I2L=109,OP_R2L=110,
     #    OP_R2I=111,OP_L2W=112,OP_L2R=113,OP_L2B=114,OP_R2B=115,
     #    OP_R2W=116,OP_L2D=117,OP_L2Q=118,OP_R2D=119,OP_D2B=120,
     #    OP_D2I=121,OP_D2R=122,OP_D2L=123,OP_Q2L=124,OP_I2D=125,
     #    OP_L2P=126,OP_P2L=127,
     #    OP_BNE=201,OP_BLB=202,OP_BB=203
 
C-----------------------------------------------------------------------
C
C    Program section definitions.
C
C-----------------------------------------------------------------------
      PARAMETER P_MAX=100
      PARAMETER P_CONSTANTS=1,P_STACK=2,P_DATA=3,P_CODE=4,P_FREQ=5,
     #          P_VECTOR=6,P_APD=7,P_MEMORY=8
      CHARACTER*32 PSECT_NAME(P_CONSTANTS:P_MAX),BASEC
      CHARACTER*3 BASEV
      COMMON /PSECTC/ BASEC,BASEV,PSECT_NAME
      DATA PSECT_NAME
     // '$PLM_ROM','$DGROUP_STACK','$DGROUP_DATA'
     ,, '$PLM_CODE','$PLM_FREQ','$CGROUP_VECTOR'
     ,, '$PLM_APD','MEMORY'
     ,, 92*' '
     //
 
      INTEGER*2 NC
      COMMON /PSECTS/ NC
 
C-----------------------------------------------------------------------
C
C    Compiler listing stuff.
C
C-----------------------------------------------------------------------
      REAL*4 VERSION
      LOGICAL*1 LISTING_TO_TERMINAL,LAST_LINE_EXISTS
      COMMON /LIST/ LIST_LINE_NO,LIST_STNO,LIST_BLOCK_LEVEL,
     #              PREVIOUS_STNO,LINE_OF_PAGE,PAGE_NO,VERSION,
     #              LINES_READ,ERRORS,WARNINGS,LISTING_TO_TERMINAL,
     #              FIFO_DEPTH,LAST_LINE_EXISTS
 
C-----------------------------------------------------------------------
C
C    Symbol table hash buckets.
C
C-----------------------------------------------------------------------
      INTEGER*2 HASH_BUCKET(0:210)
      COMMON /HASH/ HASH_BUCKET,FIRST_KEYWORD
 
C-----------------------------------------------------------------------
C
C    Procedure scope structures.
C
C-----------------------------------------------------------------------
      PARAMETER PROC_MAX=16     ! MAX STATIC NESTING DEPTH OF PROCS.
      PARAMETER PROC_MAIN=1,PROC_EXT=2,PROC_FORWARD=4,PROC_REENT=8
 
      INTEGER*2 PROC_FLAGS(PROC_MAX),PROC_DYN_OFF(PROC_MAX),
     #          PROC_INDEX(PROC_MAX),PROC_DYN_INDEX(PROC_MAX),
     #          PROC_ENTRY_MASK(PROC_MAX),PROC_ENTRY_INDEX(PROC_MAX),
     #          PROC_AP(0:PROC_MAX)
      COMMON /PROCS/ PROC_LEVEL,PROC_FLAGS,PROC_DYN_OFF,PROC_INDEX,
     #               PROC_DYN_INDEX,PROC_ENTRY_MASK,PROC_ENTRY_INDEX,
     #               PROC_AP
      DATA PROC_LEVEL/1/, PROC_FLAGS(1)/PROC_MAIN/,
     #     PROC_AP(0)/1/, PROC_AP(1)/1/
 
C-----------------------------------------------------------------------
C
C    Path analysis stuff.
C
C-----------------------------------------------------------------------
      LOGICAL*4 PATH
      COMMON /PATH_ANALYSIS/ PATH
 
C-----------------------------------------------------------------------
C
C    GLOBALS symbol table stuff.
C
C-----------------------------------------------------------------------
      PARAMETER GBL_MAX=800     ! MAX # OF GLOBALLY INPUT SYMBOLS.
      CHARACTER*32 GLOBAL_SYMBOL(GBL_MAX)
      INTEGER*2 LAST_GLOBAL
      COMMON /GLOBALS/ LAST_GLOBAL
      COMMON /GLOBALC/ GLOBAL_SYMBOL
 
C-----------------------------------------------------------------------
C
C    Basic block analysis stuff.
C
C-----------------------------------------------------------------------
      LOGICAL*4 END_OF_BASIC_BLOCK
      INTEGER*2 BASIC_BLOCK
      INTEGER*2 EXTERNAL_SERIAL_DELTA,BASED_SERIAL_DELTA,
     #          SUBSCRIPTED_SERIAL_DELTA,OVERLAID_SERIAL_DELTA
      COMMON /BASIC_BLOCKS/ END_OF_BASIC_BLOCK,BASIC_BLOCK,
     #       EXTERNAL_SERIAL_DELTA,BASED_SERIAL_DELTA,
     #       SUBSCRIPTED_SERIAL_DELTA,OVERLAID_SERIAL_DELTA
 
C-----------------------------------------------------------------------
C
C    Assumption flags.
C
C-----------------------------------------------------------------------
      LOGICAL*1 ASSUME_SCE,ASSUME_CSE,ASSUME_EEQ,ASSUME_PSE,
     #		ASSUME_BRO,ASSUME_BBA,ASSUME_CTE,ASSUME_MCO,
     #		ASSUME_CFA,ASSUME_SWB,ASSUME_OSR,ASSUME_SVE,
     #          ASSUME_S64,ASSUME_C7F,ASSUME_DBG
 
      COMMON /ASSUMPTIONS/
     #          ASSUME_SCE,ASSUME_CSE,ASSUME_EEQ,ASSUME_PSE,
     #		ASSUME_BRO,ASSUME_BBA,ASSUME_CTE,ASSUME_MCO,
     #		ASSUME_CFA,ASSUME_SWB,ASSUME_OSR,ASSUME_SVE,
     #          ASSUME_S64,ASSUME_C7F,ASSUME_DBG
 
