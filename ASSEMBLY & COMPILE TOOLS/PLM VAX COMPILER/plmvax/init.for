C***********************************************************************
C
C                               INIT.FOR
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
C    This module of the PL/M-VAX compiler contains initialization
C    routines which are called just before the first non-control line
C    is processed (i.e., after all primary controls have been processed
C    but before the first program text is processed).
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. LENGTH,FIRST,LAST, and SIZE are now LONG
C                             procedures.
C    24OCT81  Alex Hunter  1. Change BI_NBR_ELEMENTS and BI_ELEMENT_SIZE
C			      to INTEGER*4 per changes to corresponding
C			      SYMBOL arrays. (V5.6)
C    28OCT81  Alex Hunter  1. Add new keywords and delete '%' from
C			      existing keywords, since keywords may
C			      now be re-declared. (V5.7)
C    10NOV81  Alex Hunter  1. Add S_NO_SIDE_EFFECTS attribute. (V6.0)
C    12NOV81  Alex Hunter  1. Implement LAST(MEMORY), etc. (V6.1)
C			   2. Add BI_PSECT.
C			   3. Change the way psect names are fixed up.
C    14NOV81  Alex Hunter  1. Append overlay name to P_CODE psect name.
C			      (V6.2)
C    21NOV81  Alex Hunter  1. Temporarily change LOW back to an external.
C			      (V6.3)
C    10JAN81  Alex Hunter  1. Change DOUBLE keyword to DOUBLE$PRECISION
C			      to avoid conflict with DOUBLE builtin.
C			      (V6.4).
C
C***********************************************************************
      SUBROUTINE INIT_SYMTAB
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 SYMBOL
      CHARACTER*1 MODL
 
      DATA SYMBOL_VAX_ID(SYM_MLAST),SYMBOL_REF(SYM_MLAST)
     //		'MEMORY.LAST',		S_VALUE /
      DATA SYMBOL_VAX_ID(SYM_MLEN),SYMBOL_REF(SYM_MLEN)
     //		'MEMORY.LEN',		S_VALUE /
      DATA SYMBOL_VAX_ID(SYM_MSIZ),SYMBOL_REF(SYM_MSIZ)
     //		'MEMORY.SIZ',		S_VALUE /
      DATA SYMBOL_VAX_ID(SYM_SLAST),SYMBOL_REF(SYM_SLAST)
     //		'STACK.LAST',		S_VALUE /
      DATA SYMBOL_VAX_ID(SYM_SLEN),SYMBOL_REF(SYM_SLEN)
     //		'STACK.LEN',		S_VALUE /
      DATA SYMBOL_VAX_ID(SYM_SSIZ),SYMBOL_REF(SYM_SSIZ)
     //		'STACK.SIZ',		S_VALUE /
 
      PARAMETER M = FIRST_AVAILABLE_SYMBOL_INDEX
      PARAMETER N=54    ! # OF BUILTINS.
 
      CHARACTER*32 BI_PLM_ID(N),BI_VAX_ID(N)
      INTEGER*2 BI_KIND(N),BI_TYPE(N),
     #          BI_LINK(N),
     #          BI_LIST_SIZE(N),BI_REF(N),BI_BASE(N),
     #          BI_BASE_MEMBER(N),BI_FLAGS(N),
     #		BI_SERIAL_NO(N),BI_PSECT(N)
      INTEGER*4 BI_NBR_ELEMENTS(N),BI_ELEMENT_SIZE(N),
     #		BI_LOWER_BOUND(N),BI_DISP(N)
 
      EQUIVALENCE (BI_PLM_ID,SYMBOL_PLM_ID(M))
     ,,           (BI_VAX_ID,SYMBOL_VAX_ID(M))
     ,,           (BI_KIND,SYMBOL_KIND(M))
     ,,           (BI_TYPE,SYMBOL_TYPE(M))
     ,,           (BI_NBR_ELEMENTS,SYMBOL_NBR_ELEMENTS(M))
     ,,           (BI_ELEMENT_SIZE,SYMBOL_ELEMENT_SIZE(M))
     ,,		  (BI_LOWER_BOUND,SYMBOL_LOWER_BOUND(M))
     ,,           (BI_LINK,SYMBOL_LINK(M))
     ,,           (BI_LIST_SIZE,SYMBOL_LIST_SIZE(M))
     ,,           (BI_REF,SYMBOL_REF(M))
     ,,           (BI_BASE,SYMBOL_BASE(M))
     ,,           (BI_BASE_MEMBER,SYMBOL_BASE_MEMBER(M))
     ,,           (BI_FLAGS,SYMBOL_FLAGS(M))
     ,,		  (BI_DISP,SYMBOL_DISP(M))
     ,,		  (BI_SERIAL_NO,SYMBOL_SERIAL_NO(M))
     ,,		  (BI_PSECT,SYMBOL_PSECT(M))
 
      DATA BI_PLM_ID
     // 'LENGTH','LAST','FIRST','SIZE','MEMORY','MEMORYTOP'
     ,, 'STACK','STACKTOP','STACKPTR','FRAMEPTR'
     ,, 'ABS','CMPB','CMPW','DOUBLE','FINDB','FINDRB','FINDRW'
     ,, 'FINDW','FIX','FLOAT','HIGH','IABS','INT','LOW'
     ,, 'MOVB','MOVE','MOVRB','MOVRW','MOVW','ROL'
     ,, 'ROR','SAL','SAR','SETB','SETW','SHL'
     ,, 'SHR','SIGNED','SKIPB','SKIPRB','SKIPRW','SKIPW'
     ,, 'UNSIGN','XLAT'
     ,, '$_BYTE','$_WORD','$_INTEGER','$_POINTER','$_REAL'
     ,, '$_LONG','$_DOUBLE','$_QUAD','$_SIGNED','$_UNSIGNED'
     //
      DATA BI_VAX_ID
     // '...','...','...','...','MEMORY.','MEMORY.TOP'
     ,, 'S.BOT','S.','...','...'
     ,, 'ABS.','CMPB.','CMPW.','...','FINDB.','FINDRB.'
     ,, 'FINDRW.'
     ,, 'FINDW.','...','...','HIGH.','IABS.','...','LOW.'
     ,, 'MOVB.','MOVE.','MOVRB.','MOVRW.','MOVW.','ROL.'
     ,, 'ROR.','SAL.','SAR.','SETB.','SETW.','SHL.'
     ,, 'SHR.','...','SKIPB.','SKIPRB.','SKIPRW.','SKIPW.'
     ,, '...','XLAT.'
     ,, '...','...','...','...','...'
     ,, '...','...','...','...','...'
     //
      DATA BI_KIND
     // 4*S_PROC,S_ARRAY,S_SCALAR
     ,, S_ARRAY,S_SCALAR,S_PROC,S_PROC
     ,, 7*S_PROC
     ,, 7*S_PROC
     ,, 6*S_PROC
     ,, 6*S_PROC
     ,, 6*S_PROC
     ,, 2*S_PROC
     ,, 5*S_PROC
     ,, 5*S_PROC
     //
      DATA BI_TYPE
     // 4*S_LONG,S_BYTE,S_BYTE
     ,, S_BYTE,S_BYTE,S_PTR,S_PTR
     ,, S_REAL,S_WORD,S_WORD,S_WORD,S_WORD,S_WORD,S_WORD
     ,, S_WORD,S_INTEGER,S_REAL,S_BYTE,S_INTEGER,S_LONG,S_BYTE
     ,, 0,0,0,0,0,S_BYTE
     ,, S_BYTE,S_INTEGER,S_INTEGER,0,0,S_WORD
     ,, S_WORD,S_INTEGER,S_WORD,S_WORD,S_WORD,S_WORD
     ,, S_WORD,0
     ,, S_BYTE,S_WORD,S_INTEGER,S_PTR,S_REAL
     ,, S_LONG,S_DOUBLE,S_QUAD,-1,-1
     //
      DATA BI_NBR_ELEMENTS
     // 4*0, 0, 0
     ,, 0, 0, 0, 0
     ,, 7*0
     ,, 7*0
     ,, 6*0
     ,, 6*0
     ,, 6*0
     ,, 2*0
     ,, 5*0
     ,, 5*0
     //
      DATA BI_ELEMENT_SIZE
     // 4*4, 1, 1
     ,, 1,1,4,4
     ,, 4,2,2,2,2,2,2
     ,, 2,2,4,1,2,4,1
     ,, 0,0,0,0,0,1
     ,, 1,2,2,0,0,2
     ,, 2,2,2,2,2,2
     ,, 2,0
     ,, 1,2,2,4,4
     ,, 4,8,8,-1,-1
     //
      DATA BI_LOWER_BOUND
     // N*0
     //
      DATA BI_LINK
     // N*0
     //
      DATA BI_LIST_SIZE
     // 4*1, 0, 0
     ,, 0,0,0,0
     ,, 1,3,3,1,3,3,3
     ,, 3,1,1,1,1,1,1
     ,, 3,3,3,3,3,2
     ,, 2,2,2,3,3,2
     ,, 2,1,3,3,3,3
     ,, 1,4
     ,, 1,1,1,1,1
     ,, 1,1,1,1,1
     //
      DATA BI_REF
     // 4*S_BUILTIN, S_STATIC, S_EXT
     ,, S_EXT,S_EXT,S_BUILTIN,S_BUILTIN
     ,, 3*S_EXT,S_BUILTIN,3*S_EXT
     ,, S_EXT,S_BUILTIN,S_BUILTIN,S_EXT,S_EXT,S_BUILTIN,S_EXT
     ,, 6*S_EXT
     ,, 6*S_EXT
     ,, S_EXT,S_BUILTIN,4*S_EXT
     ,, S_BUILTIN,S_EXT
     ,, 5*S_BUILTIN
     ,, 5*S_BUILTIN
     //
      DATA BI_BASE
     // N*0
     //
      DATA BI_BASE_MEMBER
     // N*0
     //
      DATA BI_FLAGS
     // 4*S_NO_SIDE_EFFECTS,S_SPECIAL,0
     ,, S_SPECIAL,0,2*S_NO_SIDE_EFFECTS
     ,, 7*S_NO_SIDE_EFFECTS
     ,, 7*S_NO_SIDE_EFFECTS
     ,, 5*0,S_NO_SIDE_EFFECTS
     ,, 3*S_NO_SIDE_EFFECTS,2*0,S_NO_SIDE_EFFECTS
     ,, 6*S_NO_SIDE_EFFECTS
     ,, 2*S_NO_SIDE_EFFECTS
     ,, 5*S_NO_SIDE_EFFECTS
     ,, 5*S_NO_SIDE_EFFECTS
     //
      DATA BI_DISP
     // N*0
     //
      DATA BI_SERIAL_NO
     // N*0
     //
      DATA BI_PSECT
     // 4*0,2*P_MEMORY
     ,, 2*P_STACK,2*0
     ,, 7*0
     ,, 7*0
     ,, 6*0
     ,, 6*0
     ,, 6*0
     ,, 2*0
     ,, 5*0
     ,, 5*0
     //
      PARAMETER K=54            ! # OF KEYWORDS.
     
      CHARACTER*32 KW_PLM_ID(K)
      INTEGER*2 KW_KIND(K),KW_LINK(K)
      EQUIVALENCE (KW_PLM_ID,SYMBOL_PLM_ID(M+N))
     ,,           (KW_KIND,SYMBOL_KIND(M+N))
     ,,           (KW_LINK,SYMBOL_LINK(M+N))
 
      DATA KW_PLM_ID
     //'ADDRESS   ','AND       ','AT        ','BASED     ','BY        '
     ,,'BYTE      ','CALL      ','CASE      ','DATA      ','DECLARE   '
     ,,'DISABLE   ','DO        ','ELSE      ','ENABLE    ','END       '
     ,,'EOF       ','EXTERNAL  ','GO        ','GOTO      ','HALT      '
     ,,'IF        ','INITIAL   ','INTEGER   ','INTERRUPT ','LABEL     '
     ,,'LITERALLY ','MINUS     ','MOD       ','NOT       ','OR        '
     ,,'PLUS      ','POINTER   ','PROCEDURE ','PUBLIC    ','REAL      '
     ,,'REENTRANT ','RETURN    ','STRUCTURE ','THEN      ','TO        '
     ,,'WHILE     ','WORD      ','XOR       ','COMMON    ','LONG      '
     ,,'DOUBLEPRECISION'
     ,,             'OTHERWISE ','QUAD      ','FORWARD   ','SELECTOR  '
     ,,'DWORD     ','SHORT     ','BOOLEAN   ','REGISTER  '
     //
      DATA KW_KIND
     // K*S_KEYWORD
     //
      DATA KW_LINK
     // 101,102,103,104,105,106,107,108,109,110
     ,, 111,112,113,114,115,116,117,118,119,120
     ,, 121,122,123,124,125,126,127,128,129,130
     ,, 131,132,133,134,135,136,137,138,139,140
     ,, 141,142,143,144,145,146,147,148,149,150
     ,, 151,152,153,154
     //
 
      SYMBOL_TOP(0)=M+N+K-1
      FIRST_KEYWORD=M+N
 
C-------- IF PLM80, DISGUISE NON-PLM80 KEYWORDS.
C
C     IF (PLM80_FLAG) THEN
C        KW_PLM_ID(K_INTEGER-100)='$INTEGER'
C        KW_PLM_ID(K_POINTER-100)='$POINTER'
C        KW_PLM_ID(K_REAL-100)='$REAL'
C        KW_PLM_ID(K_WORD-100)='$WORD'
C     ENDIF

C-------- FIXUP VAX_ID'S OF BUILTINS WHICH DEPEND ON MODEL SIZE.
 
!     IF (LARGE) THEN
!        MODL='L'
!     ELSE
!        MODL='S'
!     ENDIF
!
!     DO I=1,N
!        DO J=1,32
!           IF (BI_VAX_ID(I)(J:J).EQ.'#') BI_VAX_ID(I)(J:J)=MODL
!        ENDDO
!     ENDDO
 
C-------- CHAIN BUILTINS AND KEYWORDS INTO HASH BUCKETS.
 
      DO 10 I=M,SYMBOL_TOP(0)
         H=HASH(SYMBOL_PLM_ID(I))
         SYMBOL_CHAIN(I)=HASH_BUCKET(H)
         HASH_BUCKET(H)=I
10    CONTINUE
 
C-------- READ IN GLOBAL SYMBOLS IF REQUIRED.
 
      LAST_GLOBAL=0
      IF (GLOBALS_FLAG) THEN
20       READ(GBL,1001,END=30) SYMBOL
1001     FORMAT(X,A)
         IF (SYMBOL(1:1).NE.'*') THEN
            IF (LAST_GLOBAL.GE.GBL_MAX) 
     #         CALL FATAL('TOO MANY GLOBALS')
            LAST_GLOBAL=LAST_GLOBAL+1
            GLOBAL_SYMBOL(LAST_GLOBAL)=SYMBOL
         ENDIF
         GO TO 20
30       CLOSE (UNIT=GBL)
      ENDIF
 
C-------- FIX UP PSECT NAMES AND COMPILE TIME BASES.
 
      IF (MODEL.EQ.4) THEN
         PSECT_NAME(P_DATA)='$PLM_DATA'
      ENDIF
 
      IF (OVERLAY_FLAG) THEN
         NC=LNB(PSECT_NAME(P_DATA))
         PSECT_NAME(P_DATA)(NC+1:)='_'
         CALL MAKE_CHARS(PSECT_NAME(P_DATA)(NC+2:),OVERLAY_PREFIX)
 
         NC=LNB(PSECT_NAME(P_CODE))
         PSECT_NAME(P_CODE)(NC+1:)='_'
         CALL MAKE_CHARS(PSECT_NAME(P_CODE)(NC+2:),OVERLAY_PREFIX)
 
         BASEC='D.'
         CALL MAKE_CHARS(BASEC(3:),OVERLAY_PREFIX)
         NC=LNB(BASEC)
      ENDIF
 
      IF (.NOT.ROM_FLAG .AND. MODEL.NE.4) THEN
         PSECT_NAME(P_CONSTANTS)=PSECT_NAME(P_DATA)
      ENDIF
 
      IF (OVERLAY_FLAG) THEN
         BASEV='R11'
      ELSE
         BASEV='#D.'
      ENDIF
 
      RETURN
      END
