C***********************************************************************
C
C                               OPERAND.FOR
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
C    This module of the PL/M-VAX compiler is used to translate a
C    code tree operand into a symbolic assembly-language character
C    string.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C  			   2. Generate long-displacement addressing
C			      for constants in ROM.
C    12NOV81  Alex Hunter  1. Qualify S_STATIC with P_DATA. (V6.1)
C    14NOV81  Alex Hunter  1. Major rewrite to change addressing modes.
C			      (V6.2)
C    03FEB82  Alex Hunter  1. Fix bug for immediate operands in LARGE
C			      model. (V6.6)
C
C-----------------------------------------------------------------------
      CHARACTER*80 FUNCTION OPERAND(OPND,N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 IVAL,IFSD,ILSD
      CHARACTER*32 VECNIQUE
      CHARACTER*14 STRINGG,LITG
      CHARACTER*10 LIT,STRING10
      CHARACTER*2 DISPL
      CHARACTER*3 REGNAME(REG_MIN:REG_MAX)
      DATA REGNAME /'R1','R2','R3','R4','R5','R6','R7','R8','R9',
     #              'R10','R11','AP','FP','SP','PC','R0'/
      INTEGER*2 REGNAME_LENGTH(REG_MIN:REG_MAX)
      DATA REGNAME_LENGTH /2,2,2,2,2,2,2,2,2,3,3,2,2,2,2,2/
 
      IF (OPND.GE.REG_MIN.AND.OPND.LE.REG_MAX) THEN
         OPERAND=REGNAME(OPND)
         N=REGNAME_LENGTH(OPND)
         RETURN
 
      ELSEIF (OPND.EQ.ON_STACK) THEN
         IF (MODEL.EQ.2 .OR. MODEL.EQ.4) THEN
            OPERAND='-(SP)'
            N=5
         ELSE
            OPERAND='-(R10)'
            N=6
         ENDIF
         RETURN
 
      ELSEIF (FIXLIT(OPND)) THEN
         IVAL=FIXED_VAL(OPND)
         LIT=STRING10(IVAL,IFSD)
         OPERAND='#'//LIT(IFSD:10)
         N=12-IFSD
         RETURN
 
      ELSEIF (FLOATLIT(OPND)) THEN
         LITG=STRINGG(FLOAT_VAL(OPND),IFSD,ILSD)
         OPERAND='#'//LITG(IFSD:ILSD)
         N=ILSD-IFSD+2
         RETURN
      ENDIF
 
C -- OPERAND MUST BE A CONSTANT OR AN ATOM.
 
C
C   ---   COMPUTE WHICH ADDRESSING SCHEMA TO USE.
C
      SCHEMA=1   ! Default schema.
      ATMSYM=0   ! In case opnd is a constant.
 
      IF (CONSTANT(OPND)) THEN
         IF (ROM_FLAG.OR.MODEL.EQ.4) THEN
            SCHEMA=3
         ELSEIF (OVERLAY_FLAG) THEN
            SCHEMA=7
         ELSE
            SCHEMA=5
         ENDIF
      ELSEIF (ATOM(OPND)) THEN
         ATMSYM=ATOM_SYM(OPND)   ! We'll need this a lot.
         IF ((ATOM_FLAGS(OPND).AND.A_VECTOR).NE.0) THEN
            SCHEMA=4
         ELSEIF (ATOM_BASE(OPND).NE.0) THEN
            SCHEMA=2
            BASEREG=NODE_REG(ATOM_BASE(OPND))
         ELSEIF (ATMSYM.NE.0) THEN
            IF (SYMBOL_REF(ATMSYM).EQ.S_STATIC) THEN
               IF (SYMBOL_PSECT(ATMSYM).EQ.P_DATA) THEN
                  IF (OVERLAY_FLAG) THEN
                     SCHEMA=7
                  ELSE
                     SCHEMA=5
                  ENDIF
               ELSEIF (SYMBOL_PSECT(ATMSYM).EQ.P_CONSTANT) THEN
                  SCHEMA=3
               ELSEIF (SYMBOL_PSECT(ATMSYM).EQ.P_MEMORY) THEN
                  SCHEMA=8
               ELSEIF (SYMBOL_PSECT(ATMSYM).GT.P_MEMORY) THEN
                  SCHEMA=3    ! User common.
               ENDIF
            ELSEIF (SYMBOL_REF(ATMSYM).EQ.S_EXT) THEN
               IF (SYMBOL_PSECT(ATMSYM).EQ.P_DATA) THEN
                  IF ((SYMBOL_FLAGS(ATMSYM).AND.S_SAME_OVERLAY).NE.0)
     #            THEN
                     SCHEMA=7
                  ELSE
                     SCHEMA=6
                  ENDIF
               ELSEIF (SYMBOL_PSECT(ATMSYM).EQ.P_CONSTANT) THEN
                  SCHEMA=3
               ELSEIF (SYMBOL_PSECT(ATMSYM).EQ.P_MEMORY) THEN
                  SCHEMA=8
               ELSEIF (SYMBOL_PSECT(ATMSYM).GT.P_MEMORY) THEN
                  SCHEMA=3    ! User common.
               ENDIF
            ELSEIF (SYMBOL_REF(ATMSYM).EQ.S_ARG) THEN
               SCHEMA=2
               BASEREG=PROC_AP(SYMBOL_LINK(ATMSYM))
            ELSEIF (SYMBOL_REF(ATMSYM).EQ.S_DYNAMIC) THEN
               SCHEMA=2
               BASEREG=12
            ENDIF
         ENDIF
      ENDIF
 
C
C   ---   COMPUTE THE DISPLACEMENT MODE FIELD.
C
      IF (ATOM(OPND).AND.(ATOM_FLAGS(OPND).AND.A_IMMEDIATE).NE.0 .OR.
     #    ATMSYM.NE.0.AND.SYMBOL_REF(ATMSYM).EQ.S_VALUE) THEN
         OPERAND='#'
         N=1
      ELSEIF (SCHEMA.GE.5.AND.MODEL.NE.4.AND.
     #        .NOT.(MODEL.EQ.2.AND.SCHEMA.EQ.8)) THEN
         OPERAND='W^'
         N=2
      ELSE
         OPERAND=' '
         N=0
      ENDIF
 
C
C   ---   COMPUTE THE SYMBOLIC VALUE FIELD.
C
      IF (CONSTANT(OPND)) THEN
         OPERAND(N+1:)=LOCAL_LABEL(CONSTANT_LABEL(OPND),N1)
         N=N+N1
      ELSEIF (SCHEMA.EQ.4) THEN
         OPERAND(N+1:)=VECNIQUE(SYMBOL_VAX_ID(ATMSYM))
         N=LNB(OPERAND)
      ELSE
         IF (ATMSYM.NE.0) THEN
            OPERAND(N+1:)=SYMBOL_VAX_ID(ATMSYM)
            N=LNB(OPERAND)
         ENDIF
         IF (ATOM_MEM(OPND).NE.0) THEN
            OPERAND(N+1:)='+'//MEMBER_VAX_ID(ATOM_MEM(OPND))
            N=LNB(OPERAND)
         ENDIF
         IVAL=ATOM_DISP(OPND)
         IF (ATMSYM.NE.0) THEN
            IVAL=IVAL+SYMBOL_DISP(ATMSYM)
         ENDIF
         IF (IVAL.NE.0) THEN
            LIT=STRING10(IVAL,IFSD)
            IF (IVAL.GE.0) THEN
               OPERAND(N+1:)='+'//LIT(IFSD:10)
               N=N+12-IFSD
            ELSE
               OPERAND(N+1:)=LIT(IFSD:10)
               N=N+11-IFSD
            ENDIF
         ENDIF
      ENDIF
 
C
C   ---   COMPUTE RUNTIME RELOCATION FIELD.
C
      IF (SCHEMA.EQ.7.AND.
     #    (.NOT.ATOM(OPND).OR.(ATOM_FLAGS(OPND).AND.A_CTIM).EQ.0)) THEN
         OPERAND(N+1:)='-'//BASEC
         N=N+NC+1
      ENDIF
 
C
C   ---   COMPUTE THE BASE FIELD.
C
      IF (SCHEMA.EQ.2) THEN
         OPERAND(N+1:)='('//REGNAME(BASEREG)(:REGNAME_LENGTH(BASEREG))
     #                 //')'
         N=N+REGNAME_LENGTH(BASEREG)+2
      ELSEIF (SCHEMA.EQ.4) THEN
         OPERAND(N+1:)='-V.'
         N=N+3
      ELSEIF (SCHEMA.GE.5.AND.MODEL.NE.4.AND.
     #        .NOT.(MODEL.EQ.2.AND.SCHEMA.EQ.8)) THEN
         IF (.NOT.ATOM(OPND).OR.(ATOM_FLAGS(OPND).AND.A_IMMEDIATE).EQ.0)
     #   THEN
            OPERAND(N+1:)='-K.(R11)'
            N=N+8
         ENDIF
      ELSEIF (SCHEMA.EQ.5.AND.MODEL.EQ.4) THEN
         IF (.NOT.ATOM(OPND).OR.(ATOM_FLAGS(OPND).AND.A_IMMEDIATE).EQ.0)
     #   THEN
            OPERAND(N+1:)='(R11)'
            N=N+5
         ELSE
            OPERAND(N+1:)='+M.'
            N=N+3
         ENDIF
      ENDIF
 
C
C   ---   COMPUTE THE INDEX FIELD.
C
      IF (ATOM(OPND).AND.ATOM_SUB(OPND).NE.NULL) THEN
         XREG=NODE_REG(ATOM_SUB(OPND))
         OPERAND(N+1:)='['//REGNAME(XREG)(:REGNAME_LENGTH(XREG))//']'
         N=N+REGNAME_LENGTH(XREG)+2
      ENDIF
 
      RETURN
      END
