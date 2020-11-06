C***********************************************************************
C
C                               GENCODE.FOR
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
C    This module of the PL/M-VAX compiler generates actual symbolic
C    MACRO assembly code from the abstract operators and operands of
C    of a code tree node.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    23OCT81  Alex Hunter  1. Add peephole optimizations for trivial
C			      conversions and commutative binary
C			      operators. (V5.6)
C    09NOV81  Alex Hunter  1. Implement MCO assumption. (V5.9)
C
C-----------------------------------------------------------------------
C!!!!!!!! COMPILE ME WITH  /CONT=99  PLEASE!!!!!!!!!!
C
      SUBROUTINE EMIT_CODE(OP,OPND1X,OPND2X,OPND3)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 OPERAND,OPERAND1,OPERAND2,OPERAND3,TEMPOPND
      CHARACTER*6 MNEM(S_BYTE:S_QUAD,2:3,1:22)
 
C      BYTE    WORD   INTEGER POINTER  REAL    LONG   DOUBLE   QUAD
 
      DATA MNEM/
     #'ADDB2','ADDW2','ADDW2','ADDL2','ADDF2','ADDL2','ADDD2','---- ',
     #'ADDB3','ADDW3','ADDW3','ADDL3','ADDF3','ADDL3','ADDD3','---- ',
     #'SUBB2','SUBW2','SUBW2','SUBL2','SUBF2','SUBL2','SUBD2','---- ',
     #'SUBB3','SUBW3','SUBW3','SUBL3','SUBF3','SUBL3','SUBD3','---- ',
     #'MULB2','MULW2','MULW2','MULL2','MULF2','MULL2','MULD2','---- ',
     #'MULB3','MULW3','MULW3','MULL3','MULF3','MULL3','MULD3','---- ',
     #'DIVB2','DIVW2','DIVW2','DIVL2','DIVF2','DIVL2','DIVD2','---- ',
     #'DIVB3','DIVW3','DIVW3','DIVL3','DIVF3','DIVL3','DIVD3','---- ',
     #'---- ','---- ','---- ','ADWC ','---- ','ADWC ','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'---- ','---- ','---- ','SBWC ','---- ','SBWC ','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','----', '---- ','---- ',
     #'MNEGB','MNEGW','MNEGW','MNEGL','MNEGF','MNEGL','MNEGD','---- ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'MCOMB','MCOMW','MCOMW','MCOML','---- ','MCOML','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'BICB2','BICW2','BICW2','BICL2','---- ','BICL2','---- ','---- ',
     #'BICB3','BICW3','BICW3','BICL3','---- ','BICL3','---- ','---- ',
     #'BISB2','BISW2','BISW2','BISL2','---- ','BISL2','---- ','---- ',
     #'BISB3','BISW3','BISW3','BISL3','---- ','BISL3','---- ','---- ',
     #'XORB2','XORW2','XORW2','XORL2','---- ','XORL2','---- ','---- ',
     #'XORB3','XORW3','XORW3','XORL3','---- ','XORL3','---- ','---- ',
     #'CMPB ','CMPW ','CMPW ','CMPL ','CMPF ','CMPL ','CMPD ','---- ',
     #'BLSSU','BLSSU','BLSS ','BLSSU','BLSS ','BLSS ','BLSS ','---- ',
     #'CMPB ','CMPW ','CMPW ','CMPL ','CMPF ','CMPL ','CMPD ','---- ',
     #'BGTRU','BGTRU','BGTR ','BGTRU','BGTR ','BGTR ','BGTR ','---- ',
     #'CMPB ','CMPW ','CMPW ','CMPL ','CMPF ','CMPL ','CMPD ','---- ',
     #'BEQLU','BEQLU','BEQL ','BEQLU','BEQL ','BEQL ','BEQL ','---- ',
     #'CMPB ','CMPW ','CMPW ','CMPL ','CMPF ','CMPL ','CMPD ','---- ',
     #'BNEQU','BNEQU','BNEQ ','BNEQU','BNEQ ','BNEQ ','BNEQ ','---- ',
     #'CMPB ','CMPW ','CMPW ','CMPL ','CMPF ','CMPL ','CMPD ','---- ',
     #'BLEQU','BLEQU','BLEQ ','BLEQU','BLEQ ','BLEQ ','BLEQ ','---- ',
     #'CMPB ','CMPW ','CMPW ','CMPL ','CMPF ','CMPL ','CMPD ','---- ',
     #'BGEQU','BGEQU','BGEQ ','BGEQU','BGEQ ','BGEQ ','BGEQ ','---- ',
     #'MOVAB','MOVAW','MOVAW','MOVAL','MOVAF','MOVAL','MOVAD','MOVAQ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'MOVB ','MOVW ','MOVW ','MOVL ','MOVF ','MOVL ','MOVD ','MOVQ ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','EDIV ','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- ',
     #'BITB ','BITW ','BITW ','BITL ','---- ','BITL ','---- ','---- ',
     #'---- ','---- ','---- ','---- ','---- ','---- ','---- ','---- '/
      CHARACTER*6 CLROP(8),INCOP(8),DECOP(8),PUSHAOP(8),PUSHLOP(8),
     #            TSTOP(8)
      DATA CLROP,INCOP,DECOP,PUSHAOP,PUSHLOP,TSTOP/
     #'CLRB ','CLRW ','CLRW ','CLRL ','CLRF ','CLRL ','CLRD ','CLRQ ',
     #'INCB ','INCW ','INCW ','INCL ','---- ','INCL ','---- ','---- ',
     #'DECB ','DECW ','DECW ','DECL ','---- ','DECL ','---- ','---- ',
     #'PUSHAB','PUSHAW','PUSHAW','PUSHAL','PUSHAF','PUSHAL','PUSHAD',
     #   'PUSHAQ',
     #'---- ','---- ','---- ','PUSHL','PUSHL','PUSHL','---- ','---- ',
     #'TSTB ','TSTW ','TSTW ','TSTL ','TSTF ','TSTL ','TSTD ','---- '/
 
      CHARACTER*6 CNVT(OP_B2W:OP_I2D)
      DATA CNVT/
     # 'MOVZBW','MOVZBW','MOVZBL','CVTBF ','CVTWB ',
     # 'MOVZWL','CVTWB ','CVTWF ','CVTWL ','CVTFL ',
     # 'CVTFW ','CVTLW ','CVTLF ','CVTLB ','CVTFB ',
     # 'CVTFW ','CVTLD ','----  ','CVTFD ','CVTDB ',
     # 'CVTDW ','CVTDF ','CVTDL ','----  ','CVTWD '/
 
      LOGICAL*1 NONTRIVIAL_CONVERSION(OP_B2W:OP_I2D)
      DATA NONTRIVIAL_CONVERSION/
     # .TRUE., .TRUE., .TRUE., .TRUE.,.FALSE.,
     # .TRUE.,.FALSE., .TRUE., .TRUE., .TRUE.,
     # .TRUE.,.FALSE., .TRUE.,.FALSE., .TRUE.,
     # .TRUE., .TRUE., .TRUE., .TRUE., .TRUE.,
     # .TRUE., .TRUE., .TRUE., .TRUE., .TRUE./
 
      LOGICAL*1 COMMUTATIVE(OP_ADD:OP_BIT)
      DATA COMMUTATIVE/
     #  .TRUE.,.FALSE., .TRUE.,.FALSE.,.FALSE.,.FALSE.,
     # .FALSE.,.FALSE.,.FALSE., .TRUE., .TRUE.,.FALSE.,
     # .FALSE., .TRUE., .TRUE.,.FALSE.,.FALSE.,.FALSE.,
     # .FALSE.,.FALSE.,.FALSE.,.FALSE./
 
      IF (OPND1X.EQ.NULL) THEN
         OPND1=OPND2X
         OPERAND2=' '
      ELSEIF (OPND2X.EQ.NULL) THEN
         OPND1=OPND1X
         OPERAND2=' '
      ELSE
         OPND1=OPND1X
         OPND2=OPND2X
         OPERAND2=OPERAND(OPND2,N2)
      ENDIF
 
      OPERAND1=OPERAND(OPND1,N1)
      IF (OPND3.NE.NULL) OPERAND3=OPERAND(OPND3,N3)
 
      TYPE=NODE_TYPE(OPND1)
      IF (TYPE.EQ.0) CALL BUG('EC-0')
 
      IF (OP.GE.101) THEN
         IF (OP.EQ.OP_L2Q) THEN
            IF (.NOT.REGISTER(OPND3)) CALL BUG('GC-L2Q')
            CALL EMIT('EMUL    #1,'//OPERAND1(:N1)//',#0,'//
     #                               OPERAND3(:N3))
         ELSEIF (OP.EQ.OP_Q2L) THEN
            IF (.NOT.REGISTER(OPND1)) CALL BUG('GC-Q2L')
            IF (OPERAND1.NE.OPERAND3) THEN
               CALL EMIT('MOVL    '//OPERAND1(:N1)//','//OPERAND3(:N3))
            ENDIF
         ELSEIF (OP.EQ.OP_L2P) THEN
            IF (OPERAND1.EQ.OPERAND3) THEN
               CALL EMIT('ADDL2   '//BASEV//','//OPERAND3(:N3))
            ELSE
               CALL EMIT('ADDL3   '//BASEV//','//OPERAND1(:N1)//','//
     #                                   OPERAND3(:N3))
            ENDIF
         ELSEIF (OP.EQ.OP_P2L) THEN
            IF (OPERAND1.EQ.OPERAND3) THEN
               CALL EMIT('SUBL2   '//BASEV//','//OPERAND3(:N3))
            ELSE
               CALL EMIT('SUBL3   '//BASEV//','//OPERAND1(:N1)//','//
     #                                   OPERAND3(:N3))
            ENDIF
         ELSE
            IF (.NOT.ASSUME_MCO.OR.
     #          NONTRIVIAL_CONVERSION(OP).OR.OPERAND1.NE.OPERAND3.OR.
     #          OPERAND1(N1:N1).EQ.']') THEN
               CALL EMIT(CNVT(OP)//' '//OPERAND1(:N1)//','
     #                   //OPERAND3(:N3))
            ENDIF
         ENDIF
 
      ELSEIF (OP.GE.OP_LT.AND.OP.LE.OP_GE) THEN
         IF (ASSUME_MCO.AND.
     #       (OPERAND1.EQ.'#0'.OR.OPERAND1.EQ.'#0.0')) THEN
            CALL EMIT(TSTOP(TYPE)//' '//OPERAND2(:N2))
         ELSE
            CALL EMIT(MNEM(TYPE,2,OP)//' '//OPERAND2(:N2)//','//
     #                OPERAND1(:N1))
         ENDIF
         IF (OPND3.NE.NULL) THEN
            CALL GENERATE_LOCAL_LABEL(LL1)
            CALL GENERATE_LOCAL_LABEL(LL2)
            CALL EMIT(MNEM(TYPE,3,OP)//' '//
     #                     LOCAL_LABEL(LL1,N0))
            CALL EMIT('CLRB   '//OPERAND3(:N3))
            CALL EMIT('BRB    '//LOCAL_LABEL(LL2,N0))
            CALL EMIT_LOCAL_LABEL(LL1)
            CALL EMIT('MCOMB  #0,'//OPERAND3(:N3))
            CALL EMIT_LOCAL_LABEL(LL2)
         ENDIF
      ELSE
 
         IF (ASSUME_MCO.AND.
     #       COMMUTATIVE(OP).AND.OPERAND1.EQ.OPERAND3) THEN
            TEMPOPND=OPERAND1
            OPERAND1=OPERAND2
            OPERAND2=TEMPOPND
            NT=N1
            N1=N2
            N2=NT
         ENDIF
 
         IF (ASSUME_MCO.AND.
     #       (OPERAND2.EQ.' '.OR.(OPERAND2.EQ.OPERAND3.AND.
     #        MNEM(TYPE,2,OP).NE.'----'))) THEN
            IF (OP.EQ.OP_ASSN.AND.(OPERAND1.EQ.'#0'.OR.
     #                             OPERAND1.EQ.'#0.0')) THEN
               CALL EMIT(CLROP(TYPE)//' '//OPERAND3(:N3))
            ELSEIF (OP.EQ.OP_ADD.AND.OPERAND1.EQ.'#1') THEN
               CALL EMIT(INCOP(TYPE)//' '//OPERAND3(:N3))
            ELSEIF (OP.EQ.OP_SUB.AND.OPERAND1.EQ.'#1') THEN
               CALL EMIT(DECOP(TYPE)//' '//OPERAND3(:N3))
            ELSEIF (OP.EQ.OP_LOC.AND.OPERAND3.EQ.'-(SP)') THEN
               CALL EMIT(PUSHAOP(TYPE)//' '//OPERAND1(:N1))
            ELSEIF (OP.EQ.OP_ASSN.AND.BYTE_SIZE(TYPE).EQ.4.AND.
     #              OPERAND3.EQ.'-(SP)') THEN
               CALL EMIT(PUSHLOP(TYPE)//' '//OPERAND1(:N1))
            ELSE
               CALL EMIT(MNEM(TYPE,2,OP)//' '//OPERAND1(:N1)//','//
     #                   OPERAND3(:N3))
            ENDIF
         ELSEIF (OPERAND2.EQ.' ') THEN
            CALL EMIT(MNEM(TYPE,2,OP)//' '//OPERAND1(:N1)//','//
     #                OPERAND3(:N3))
         ELSEIF (OP.EQ.OP_MOD) THEN
            CALL EMIT(MNEM(TYPE,3,OP)//' '//OPERAND1(:N1)//','//
     #                OPERAND2(:N2)//',R0,'//
     #                OPERAND3(:N3))
         ELSE
            CALL EMIT(MNEM(TYPE,3,OP)//' '//OPERAND1(:N1)//
     #                  ','//OPERAND2(:N2)//','//OPERAND3(:N3))
         ENDIF
      ENDIF
      RETURN
      END
