C***********************************************************************
C
C                               UNITS.FOR
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
C    This module of the PL/M-VAX compiler contains routines to process
C    one or a series of statement units, including any labels associated
C    with each statement unit.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C    09NOV81  Alex Hunter  1. Implement BBA assumption. (V5.9)
C    14NOV81  Alex Hunter  1. Put FPSP. in P_APD psect. (V6.2)
C
C-----------------------------------------------------------------------
      SUBROUTINE UNITS
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 TEMP_ID
 
      CALL PSECT(P_CODE)
      FLAG=1
      GO TO 10
C------------------------------
      ENTRY UNIT
C------------------------------
      FLAG=2
10    CALL PUSH(FLAG,1)
      TEMP_ID=IDENTIFIER
 
      IF (NLABELS.NE.0) CALL BREAK
 
      DO 18 I=1,NLABELS
      IDENTIFIER=LABELS(I)
      H=HASH(IDENTIFIER)
      SYMBOL_INDEX=HASH_BUCKET(H)
12    IF (SYMBOL_INDEX.GT.SYMBOL_TOP(BLOCK_LEVEL-1)) THEN
         IF (SYMBOL_PLM_ID(SYMBOL_INDEX).EQ.IDENTIFIER) GO TO 14
         SYMBOL_INDEX=SYMBOL_CHAIN(SYMBOL_INDEX)
         GO TO 12
      ENDIF
C----------- NO ENTRY AT THIS BLOCK LEVEL -- MAKE ONE.
      CALL ENTER_SYMBOL
      SYMBOL_KIND(SYMBOL_INDEX)=S_LABEL
14    SYMBOL_INDEX=HASH_BUCKET(H)
16    IF (SYMBOL_INDEX.GT.SYMBOL_TOP(BLOCK_LEVEL-1)) THEN
         IF (SYMBOL_PLM_ID(SYMBOL_INDEX).EQ.IDENTIFIER) THEN
            IF (SYMBOL_KIND(SYMBOL_INDEX).NE.S_LABEL) THEN
               CALL ERROR('ALREADY DECLARED NOT A LABEL: '//IDENTIFIER)
            ELSEIF ((SYMBOL_FLAGS(SYMBOL_INDEX).AND.S_UNDEF).EQ.0) THEN
               CALL ERROR('MULTIPLY DEFINED LABEL: '//IDENTIFIER)
            ELSE
               SYMBOL_REF(SYMBOL_INDEX)=S_STATIC
               SYMBOL_FLAGS(SYMBOL_INDEX)=SYMBOL_FLAGS(SYMBOL_INDEX)
     #                                    .AND..NOT.S_UNDEF
               CALL EMIT_LABEL(SYMBOL_INDEX)
            ENDIF
         ENDIF
         SYMBOL_INDEX=SYMBOL_CHAIN(SYMBOL_INDEX)
         GO TO 16
      ENDIF
18    CONTINUE
      IDENTIFIER=TEMP_ID
      IF (NLABELS.GT.0) THEN
         LAST_LABEL=LABELS(NLABELS)
         PATH=.TRUE.
         IF (BLOCK_LEVEL.EQ.1) THEN
            CALL EMIT('MOVQ    FPSP.,FP')
            IF (MODEL.EQ.1 .OR. MODEL.EQ.3) CALL EMIT('MOVAB   S.,R10')
         ENDIF
      ELSE
         LAST_LABEL='.BLANK.'
      ENDIF
      NLABELS=0
      IF (.NOT.PATH.AND.TT.NE.K_END) THEN
         CALL WARN('NO PATH TO THIS STATEMENT')
         CALL BREAK
         PATH=.TRUE.
      ENDIF
      IF (TT.EQ.K_IF) THEN
         CODE=CONDITIONAL_CLAUSE(0)
      ELSEIF (TT.EQ.K_DO) THEN
         CODE=DO_BLOCK(0)
      ELSEIF (TT.EQ.ID) THEN
         CODE=ASSIGNMENT_STATEMENT(0)
      ELSEIF (TT.EQ.K_CALL) THEN
         CODE=CALL_STATEMENT(0)
      ELSEIF (TT.EQ.K_GO.OR.TT.EQ.K_GOTO) THEN
         CODE=GOTO_STATEMENT(0)
      ELSEIF (TT.EQ.K_RETURN) THEN
         CODE=RETURN_STATEMENT(0)
      ELSEIF (TT.EQ.K_DISABLE.OR.TT.EQ.K_ENABLE.OR.TT.EQ.K_HALT) THEN
         CODE=i8086_DEPENDENT_STATEMENTS(0)
      ELSEIF (TT.EQ.D_SEMI) THEN
         CODE=NULL
         CALL GETTOK
      ELSE
         CALL POP(FLAG,1)
         IF (FLAG.EQ.2) CALL MUSTBE(NT_STATEMENT)
         RETURN
      ENDIF
      IF (CODE.NE.NULL) THEN
         BASIC_BLOCK=MAKE_NODE(OP_THEN,BASIC_BLOCK,CODE,0,0,0)
      ENDIF
      IF (END_OF_BASIC_BLOCK .OR. .NOT.ASSUME_BBA) CALL BREAK
      CALL POP(FLAG,1)
      GO TO (10,20), FLAG
20    RETURN
      END
