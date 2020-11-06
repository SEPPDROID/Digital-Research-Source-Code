C***********************************************************************
C
C                               DATA.FOR
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
C    This module of the PL/M-VAX compiler handles the INITIAL and
C    DATA attributes of a declaration.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. Allow DATA attribute with EXTERNAL. (V5.3)
C    14NOV81  Alex Hunter  1. Change psect if constant data is to be
C			      placed in $PLM_ROM. (V6.2)
C
C***********************************************************************
      SUBROUTINE INITIALIZATION(REF,THIS_PSECT)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 BLOCK_SIZE
      LOGICAL*2 NO_MORE_DATA,STRINGLEFT
      COMMON /INITDATA/ NO_MORE_DATA,STRINGLEFT,BLOCK_SIZE
 
      IF ((ROM_FLAG.OR.MODEL.EQ.4).AND.TT.EQ.K_DATA) THEN
         THIS_PSECT=P_CONSTANTS   ! Place data in $PLM_ROM.
      ENDIF
 
      IF (REF.EQ.S_EXT .AND. TT.EQ.K_DATA) THEN
         CALL GETTOK
         NO_MORE_DATA=.TRUE.
 
      ELSEIF (TT.EQ.K_INITIAL.OR.TT.EQ.K_DATA) THEN
 
         CALL GETTOK
         CALL MATCH(D_LP)
         NO_MORE_DATA=.FALSE.
         STRINGLEFT=.FALSE.
 
      ELSE
         NO_MORE_DATA=.TRUE.
      ENDIF
 
      RETURN
      
C--------------------------------
      ENTRY POST_INITIALIZATION
C--------------------------------
 
      IF (NO_MORE_DATA) RETURN
 
      CALL ERROR('TOO MUCH DATA IN INITIALIZATION LIST')
 
10    CALL INITIAL_DATA(S_WORD)
      IF (.NOT.NO_MORE_DATA) GO TO 10
 
      RETURN
      END
C------------------------------------------------------------------
      SUBROUTINE INITIAL_DATA(TYPE)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 BLOCK_SIZE
      LOGICAL*2 NO_MORE_DATA,STRINGLEFT
      COMMON /INITDATA/ NO_MORE_DATA,STRINGLEFT,BLOCK_SIZE
      CHARACTER*300 STRING1
      COMMON /FLUSH_A/ S_INDEX,S_NEXT
      COMMON /FLUSH_AC/ STRING1
      CHARACTER*80 OPERAND,OPERAND1,RESTRICTED_LOCATION_REFERENCE
      CHARACTER*7 DATA_POP(S_BYTE:S_QUAD)
      DATA DATA_POP
     // '.BYTE','.WORD','.WORD','.LONG','.FLOAT','.LONG','.DOUBLE'
     ,, '.QUAD'
     //
 
      BS = BYTE_SIZE(TYPE)
 
      IF (NO_MORE_DATA) THEN
         BLOCK_SIZE=BLOCK_SIZE+BS
         RETURN
      ENDIF
 
      IF (STRINGLEFT.OR.TT.EQ.STRCON) THEN
 
         IF (.NOT.STRINGLEFT) THEN
            STRING1=STRING
            S_INDEX=1
            S_NEXT=1
            S_LENGTH=STRLEN
            STRINGLEFT=.TRUE.
         ENDIF
 
         S_NEXT=S_NEXT+BS
 
         IF (S_NEXT-S_INDEX.GE.32) CALL FLUSH_ASCII
         IF (S_NEXT.LE.S_LENGTH) RETURN
         CALL FLUSH_ASCII
         STRINGLEFT=.FALSE.
         CALL GETTOK
 
      ELSE
 
         CALL BREAK
         CONST=EXPRESSION(0)
         CALL RESOLVE_CONTEXT(CONST)
         IF (NODE_CONTEXT(CONST).EQ.0)
     #      CALL SET_CONTEXT(CONST,CONTEXT(TYPE))
         CALL COERCE_TYPES(CONST)
         CONST=FORCE_TYPE(CONST,TYPE)
         CONST=FOLD_CONSTANTS(CONST)
 
         IF (NODE(CONST).AND.OPNODE_OP(CONST).GT.100.AND.
     #       OPNODE_OP(CONST).LT.OP_L2P) THEN
            CONST=OPNODE_OPND1(CONST)
         ENDIF
 
         IF (NODE(CONST).AND.OPNODE_OP(CONST).EQ.OP_LOC) THEN
 
            OPERAND1=RESTRICTED_LOCATION_REFERENCE(CONST,N1)
            CALL EMIT(DATA_POP(TYPE)//' '//OPERAND1(:N1))
 
         ELSEIF (LITERAL(CONST)) THEN
 
            OPERAND1=OPERAND(CONST,N1)
            CALL EMIT(DATA_POP(TYPE)//' '//OPERAND1(2:N1))
 
         ELSE
 
            CALL ERROR('INITIALIZATION LIST ELEMENT NOT A CONSTANT')
            CALL EMIT(DATA_POP(TYPE)//' 0')
 
         ENDIF
      ENDIF
 
      IF (TT.EQ.D_COMMA) THEN
         CALL GETTOK
         IF (TT.NE.D_RP) RETURN		! ALLOW ',)' AT END OF LIST.
      ENDIF
 
      CALL MATCH(D_RP)
      NO_MORE_DATA=.TRUE.
      RETURN
      END
C-----------------------------------------------------------------
      SUBROUTINE FLUSH_ASCII
      IMPLICIT INTEGER*2 (A-Z)
      CHARACTER*300 STRING1
      COMMON /FLUSH_A/ S_INDEX,S_NEXT
      COMMON /FLUSH_AC/ STRING1
 
      IF (S_NEXT.GT.S_INDEX) THEN
         CALL EMIT('.ASCII  `'//STRING1(S_INDEX:S_NEXT-1)//'`')
         S_INDEX=S_NEXT
      ENDIF
      RETURN
      END
