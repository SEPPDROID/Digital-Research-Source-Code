C***********************************************************************
C
C                               PROCS.FOR
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
C    This module of the PL/M-VAX compiler processes procedure
C    declarations.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C    12NOV81  Alex Hunter  1. Save and restore argument pointer displays
C			      for indirect procedure calls. (V6.1)
C    14NOV81  Alex Hunter  1. Change addressing modes and psect usage.
C			      (V6.2)
C			   2. Use full 31-character external names.
C			   3. Increase max nesting of procs with args.
C			   4. Allow keyword as formal parameter.
C    14JAN82  Alex Hunter  1. Fix minor bug from V6.2. (V6.5)
C
C-----------------------------------------------------------------------
      SUBROUTINE PROCEDURE_DEFINITION
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 PROC_NAME,FREQ_NAME,VECNIQUE,VEC_NAME,APD_NAME
      CHARACTER*40 REGISTER_MASK
      CHARACTER*41 APD_MASK
      CHARACTER*80 OPERAND,OPERAND1
      LOGICAL*4 PROC_IS_PUBLIC
 
      PROC_LEVEL=PROC_LEVEL+1
      IF (PROC_LEVEL.GT.PROC_MAX) 
     #   CALL FATAL('PROCEDURES NESTED TOO DEEPLY')
 
      CALL PROCEDURE_STATEMENT
 
      PROC_ENTRY_MASK(PROC_LEVEL)=0
      CALL PUSHC(IDENTIFIER)
      IDENTIFIER='MSK.'//SYMBOL_PLM_ID(PROC_INDEX(PROC_LEVEL))
      CALL ENTER_SYMBOL
      PROC_ENTRY_INDEX(PROC_LEVEL)=SYMBOL_INDEX
      SYMBOL_KIND(SYMBOL_INDEX)=S_SCALAR
      SYMBOL_TYPE(SYMBOL_INDEX)=S_WORD
      SYMBOL_NBR_ELEMENTS(SYMBOL_INDEX)=1
      SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX)=BYTE_SIZE(S_WORD)
      SYMBOL_REF(SYMBOL_INDEX)=S_VALUE
      CALL POPC(IDENTIFIER)
 
      PROC_FLAGS(PROC_LEVEL)=0
      IF (SYMBOL_REF(PROC_INDEX(PROC_LEVEL)).EQ.S_EXT)
     #   PROC_FLAGS(PROC_LEVEL)=PROC_EXT
      IF (SYMBOL_REF(PROC_INDEX(PROC_LEVEL)).EQ.S_FORWARD)
     #   PROC_FLAGS(PROC_LEVEL)=PROC_FORWARD
      IF ((SYMBOL_FLAGS(PROC_INDEX(PROC_LEVEL)).AND.S_REENT).NE.0)
     #   PROC_FLAGS(PROC_LEVEL)=PROC_FLAGS(PROC_LEVEL).OR.PROC_REENT
 
      PROC_DYN_OFF(PROC_LEVEL)=0  ! INITIAL DYNAMIC_OFFSET.
 
      CALL DECLARATIONS
 
      DO 10 I=SYMBOL_TOP(BLOCK_LEVEL-1)+1,SYMBOL_TOP(BLOCK_LEVEL)
         IF (SYMBOL_REF(I).EQ.S_ARG.AND.SYMBOL_FLAGS(I).EQ.S_UNDEF)
     #      THEN
            CALL ERROR('NO DECLARATION FOR FORMAL PARAMETER '//
     #                  SYMBOL_PLM_ID(I))
         ENDIF
10    CONTINUE
 
      IF (SYMBOL_REF(PROC_INDEX(PROC_LEVEL)).EQ.S_EXT.OR.
     #    SYMBOL_REF(PROC_INDEX(PROC_LEVEL)).EQ.S_FORWARD) GO TO 20
 
      CALL PSECT(P_CODE)
      CALL BREAK
 
      IF (PATH) THEN
         CALL GENERATE_LOCAL_LABEL(LL)
         CALL EMIT('BRW     '//LOCAL_LABEL(LL,N0))
      ELSE
         LL=0
      ENDIF
      PATH=.TRUE.
 
      PROC_NAME=SYMBOL_VAX_ID(PROC_INDEX(PROC_LEVEL))
      PROC_IS_PUBLIC = (SYMBOL_FLAGS(PROC_INDEX(PROC_LEVEL)).AND.
     #                  S_PUBLIC).NE.0
 
      IF (PROC_IS_PUBLIC) THEN
         CALL EMIT1(PROC_NAME(:LNB(PROC_NAME))//'::')
      ELSE
         CALL EMIT1(PROC_NAME(:LNB(PROC_NAME))//':')
      ENDIF
  
      CALL EMIT('.WORD   '//
     #           SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL)))
 
      IF (VECTOR_FLAG) THEN
         CALL PSECT(P_VECTOR)
         VEC_NAME=VECNIQUE(PROC_NAME)
         IF (PROC_AP(PROC_LEVEL-1).LE.1) THEN
            APD_MASK=' '
         ELSE
            MASK=0
            DO I=2,PROC_LEVEL-1
               MASK=MASK .OR. ISHFT(1,I)
            ENDDO
            APD_MASK='!'//REGISTER_MASK(MASK)
         ENDIF
         N1=LNB(SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL)))
         IF (PROC_IS_PUBLIC) THEN
            CALL EMIT1(VEC_NAME(:LNB(VEC_NAME))//'::')
            CALL EMIT('.WORD   '//
     #                 SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL))(:N1)
     #                 //APD_MASK)
         ELSE
            CALL EMIT1(VEC_NAME(:LNB(VEC_NAME))//':')
            IF (MODEL.NE.4.OR.OVERLAY_FLAG) THEN
               CALL EMIT('.WORD   '//
     #                  SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL))(:N1)
     #                  //APD_MASK)
            ELSE
               CALL EMIT('.WORD   ^M<R11>!'//
     #                  SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL))(:N1)
     #                  //APD_MASK)
               CALL EMIT('MOVAB   M.,R11')
            ENDIF
         ENDIF
         DO I=2,PROC_LEVEL-1
            IF (PROC_AP(I).NE.PROC_AP(I-1)) THEN
               OPERAND1=OPERAND(PROC_AP(I),N1)
               APD_NAME=SYMBOL_VAX_ID(PROC_ENTRY_INDEX(I))
               APD_NAME(1:3)='APD'
               CALL EMIT('MOVL    '//APD_NAME(:LNB(APD_NAME))//','//
     #                             OPERAND1(:N1))
            ENDIF
         ENDDO
         CALL GENERATE_LOCAL_LABEL(LL1)
         CALL EMIT('JMP     '//LOCAL_LABEL(LL1,N0))
         CALL PSECT(P_CODE)
         CALL EMIT_LOCAL_LABEL(LL1)
      ENDIF
 
      IF (PROC_IS_PUBLIC.AND.MODEL.EQ.4.AND..NOT.OVERLAY_FLAG) THEN
         CALL EMIT('MOVAB   M.,R11')
         CALL PRESERVE_REG(11)
      ENDIF
 
      IF (FREQ_FLAG) THEN
         FREQ_NAME='FRQ.'//PROC_NAME
         FREQ_NAME(32:)=' '
         CALL EMIT('INCL    '//FREQ_NAME)
         CALL PSECT(P_FREQ)
         CALL EMIT1(FREQ_NAME(:LNB(FREQ_NAME))//'::')
         CALL EMIT('.LONG   0')
         CALL PSECT(P_CODE)
      ENDIF
 
      IF (PROC_AP(PROC_LEVEL).NE.PROC_AP(PROC_LEVEL-1)) THEN
         CALL PRESERVE_REG(PROC_AP(PROC_LEVEL))
         OPERAND1=OPERAND(PROC_AP(PROC_LEVEL),N1)
         CALL EMIT('MOVL    AP,'//OPERAND1(:N1))
         IF (VECTOR_FLAG) THEN
            APD_NAME=SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL))
            APD_NAME(1:3)='APD'
            CALL EMIT('MOVL    AP,'//APD_NAME)
            CALL PSECT(P_APD)
            CALL EMIT1(APD_NAME(:LNB(APD_NAME))//':')
            CALL EMIT('.LONG   0')
            CALL PSECT(P_CODE)
         ENDIF
      ENDIF
      
      IF ((PROC_FLAGS(PROC_LEVEL).AND.PROC_REENT).NE.0) THEN
         CALL PUSHC(IDENTIFIER)
         IDENTIFIER='DYN.'//SYMBOL_PLM_ID(PROC_INDEX(PROC_LEVEL))
         CALL ENTER_SYMBOL
         PROC_DYN_INDEX(PROC_LEVEL)=SYMBOL_INDEX
         CALL POPC(IDENTIFIER)
         SYMBOL_KIND(PROC_DYN_INDEX(PROC_LEVEL))=S_SCALAR
         SYMBOL_TYPE(PROC_DYN_INDEX(PROC_LEVEL))=S_LONG
         SYMBOL_NBR_ELEMENTS(PROC_DYN_INDEX(PROC_LEVEL))=1
         SYMBOL_ELEMENT_SIZE(PROC_DYN_INDEX(PROC_LEVEL))=
     #      BYTE_SIZE(S_LONG)
         SYMBOL_REF(PROC_DYN_INDEX(PROC_LEVEL))=S_VALUE
         DYN_SIZE=MAKE_ATOM(PROC_DYN_INDEX(PROC_LEVEL),0,NULL,NULL,
     #                      S_LONG,0,0)
         SF=12
CCCC     CALL PRESERVE_REG(SF)  ! AP ALREADY PRESERVED BY CALL.
         IF (MODEL.EQ.2 .OR. MODEL.EQ.4) THEN
            SP=14
         ELSE
            SP=10
            CALL PRESERVE_REG(SP)
         ENDIF
         CALL EMIT_CODE(OP_SUB,DYN_SIZE,NULL,SP)
         NODE_TYPE(SP)=S_PTR
         CALL EMIT_CODE(OP_ASSN,SP,NULL,SF)
      ENDIF
 
      CALL PUSH(LL,1)
      CALL UNITS
      CALL BREAK
      CALL POP(LL,1)
 
      IF (PATH) THEN
         IF (SYMBOL_TYPE(PROC_INDEX(PROC_LEVEL)).NE.0) THEN
            CALL WARN('RETURN MISSING AT END OF TYPED PROCEDURE')
            CALL EMIT('CLRL    R0')
         ENDIF
         CALL EMIT('RET')
      ENDIF
      PATH=.FALSE.
      CALL EMIT_LOCAL_LABEL(LL)
 
      CALL EMIT1(SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL))
     #            (:LNB(SYMBOL_VAX_ID(PROC_ENTRY_INDEX(PROC_LEVEL))))
     #           //' = '//REGISTER_MASK(PROC_ENTRY_MASK(PROC_LEVEL)))
      SYMBOL_FLAGS(PROC_ENTRY_INDEX(PROC_LEVEL))=0  ! RESET S_UNDEF.
 
      IF ((PROC_FLAGS(PROC_LEVEL).AND.PROC_REENT).NE.0) THEN
         CALL EMIT_ABSDEF(SYMBOL_VAX_ID(PROC_DYN_INDEX(PROC_LEVEL)),
     #                    PROC_DYN_OFF(PROC_LEVEL))
         SYMBOL_FLAGS(PROC_DYN_INDEX(PROC_LEVEL))=0
      ENDIF
 
20    CALL BLOCK_END
      PROC_LEVEL=PROC_LEVEL-1
      CALL END_STATEMENT
 
      RETURN
      END
C-------------------------------------------------------
      SUBROUTINE PROCEDURE_STATEMENT
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 PROC_NAME,PUBLIQUE
      IF (NLABELS.EQ.0) THEN
         CALL ERROR('PROCEDURE NAME MISSING: XXX ASSUMED')
         NLABELS=1
         LABELS(NLABELS) = 'XXX'
      ENDIF
      DO 10 I=1,NLABELS-1
      CALL ERROR('EXTRANEOUS LABEL IGNORED -- '//LABELS(I))
10    CONTINUE
      PROC_NAME=LABELS(NLABELS)
      CALL PUSHC(PROC_NAME)     ! TO MATCH LABEL ON END.
      NLABELS=0
      CALL MUSTBE(K_PROCEDURE)
      IDENTIFIER=PROC_NAME
      CALL ENTER_SYMBOL
      PROC_INDEX(PROC_LEVEL)=SYMBOL_INDEX
      PROC_IX=SYMBOL_INDEX
      CALL GETTOK
      CALL BLOCK_BEGIN
      IF (TT.EQ.D_LP) THEN
         CALL FORMAL_PARAMETER_LIST(NARGS)
      ELSE
         NARGS=0
      ENDIF
      IF (TT.EQ.K_INTEGER.OR.TT.EQ.K_REAL.OR.TT.EQ.K_POINTER
     #    .OR.TT.EQ.K_BYTE.OR.TT.EQ.K_WORD.OR.TT.EQ.K_ADDRESS
     #    .OR.TT.EQ.K_LONG.OR.TT.EQ.K_DOUBLE.OR.TT.EQ.K_QUAD) THEN
         CALL BASIC_TYPE(PTYPE)
      ELSE
         PTYPE=0
      ENDIF
      CALL PROCEDURE_ATTRIBUTES(FLAGS,REF)
      IF (SYMBOL_REF(PROC_IX).EQ.S_FORWARD) THEN
         IF (SYMBOL_TYPE(PROC_IX).NE.PTYPE.OR.
     #       SYMBOL_LIST_SIZE(PROC_IX).NE.NARGS.OR.
     #       SYMBOL_FLAGS(PROC_IX).NE.FLAGS) THEN
            CALL ERROR('FORWARD DECLARATION DOESN''T MATCH THIS '//
     #        'DECLARATION OF '//SYMBOL_PLM_ID(PROC_IX))
         ENDIF
      ENDIF
      IF ((FLAGS.AND.S_PUBLIC).NE.0.OR.REF.EQ.S_EXT) THEN
         SYMBOL_VAX_ID(PROC_IX)=PUBLIQUE(SYMBOL_PLM_ID(PROC_IX))
      ENDIF
      SYMBOL_KIND(PROC_IX)=S_PROC
      SYMBOL_TYPE(PROC_IX)=PTYPE
      SYMBOL_NBR_ELEMENTS(PROC_IX)=0
      SYMBOL_ELEMENT_SIZE(PROC_IX)=0
      SYMBOL_LINK(PROC_IX)=0
      SYMBOL_LIST_SIZE(PROC_IX)=NARGS
      SYMBOL_REF(PROC_IX)=REF
      SYMBOL_BASE(PROC_IX)=0
      SYMBOL_BASE_MEMBER(PROC_IX)=0
      SYMBOL_FLAGS(PROC_IX)=FLAGS
 
      IF (NARGS.EQ.0) THEN
         PROC_AP(PROC_LEVEL)=PROC_AP(PROC_LEVEL-1)
      ELSE
         PROC_AP(PROC_LEVEL)=PROC_AP(PROC_LEVEL-1)+1
         IF (PROC_AP(PROC_LEVEL).GT.9)
     #      CALL FATAL('PROCEDURES WITH ARGUMENTS NESTED TOO DEEPLY')
      ENDIF
 
      CALL MATCH(D_SEMI)
      RETURN
      END
C-------------------------------------------------------
      SUBROUTINE FORMAL_PARAMETER_LIST(NARGS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NARGS=0
10    CALL GETTOK
      IF (TT.LT.101.OR.TT.GT.199) CALL MUSTBE(ID)
      NARGS=NARGS+1
 
      CALL ENTER_SYMBOL
      IF (SYMBOL_REF(SYMBOL_INDEX).EQ.S_ARG) THEN
         CALL ERROR('DUPLICATE ARG NAME: '//IDENTIFIER)
         GO TO 20
      ENDIF
 
      SYMBOL_REF(SYMBOL_INDEX)=S_ARG
      SYMBOL_LINK(SYMBOL_INDEX)=PROC_LEVEL	! REMEMBER PROC_LEVEL.
 
      IF (SYMBOL_REF(PROC_INDEX(PROC_LEVEL)).NE.S_EXT) THEN
C     ----- OOPS  - DON'T KNOW YET IF PROC IS EXTERNAL ------
         CALL EMIT_ABSDEF(SYMBOL_VAX_ID(SYMBOL_INDEX),NARGS*4)
      ENDIF
 
20    CALL GETTOK
      IF (TT.EQ.D_COMMA) GO TO 10
 
      CALL MATCH(D_RP)
      RETURN
      END
C-------------------------------------------------------
      SUBROUTINE PROCEDURE_ATTRIBUTES(FLAGS,REF)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      FLAGS=0
      REF=S_LOCAL
10    IF (TT.EQ.K_INTERRUPT) THEN
         CALL GETTOK
         FLAGS=FLAGS.OR.S_INTERRUPT
         CALL MATCH(FIXCON)
      ELSEIF (TT.EQ.K_REENTRANT) THEN
         CALL GETTOK
         FLAGS=FLAGS.OR.S_REENT
      ELSEIF (TT.EQ.K_PUBLIC.AND.REF.NE.S_EXT) THEN
         CALL GETTOK
         FLAGS=FLAGS.OR.S_PUBLIC
      ELSEIF (TT.EQ.K_EXTERNAL.AND.REF.EQ.S_LOCAL.AND.
     #        (FLAGS.AND.S_PUBLIC).EQ.0) THEN
         CALL GETTOK
         REF=S_EXT
      ELSEIF (TT.EQ.K_FORWARD.AND.REF.EQ.S_LOCAL) THEN
         CALL GETTOK
         REF=S_FORWARD
      ELSE
         RETURN
      ENDIF
      GO TO 10
      END
