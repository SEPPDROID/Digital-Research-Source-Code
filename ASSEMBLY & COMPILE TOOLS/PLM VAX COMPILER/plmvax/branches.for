C***********************************************************************
C
C                               BRANCHES.FOR
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
C    This module of the PL/M-VAX compiler generates optimized
C    conditional branch code for short-circuit evaluation of
C    Boolean expressions.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Use OP_BB opcode. (V5.6)
C			   2. Recode the BRANCH2 table.
C
C-----------------------------------------------------------------------
      SUBROUTINE BRANCH_TO(NODX,TRUEX,FALSEX,FALL_THRUX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD=NODX
      TRUE=TRUEX
      FALSE=FALSEX
      FALL_THRU=FALL_THRUX
 
      IF (NODE(NOD).AND.OPNODE_OP(NOD).EQ.OP_EXT) THEN
         
         CALL GENERATE_LOCAL_LABEL(LL1)
         CALL PUSH(TRUE,1)
         CALL PUSH(FALSE,1)
         CALL PUSH(FALL_THRU,1)
         CALL PUSH(NOD,1)
         CALL PUSH(LL1,1)
         CALL BRANCH_TO2(OPNODE_OPND1(NOD),LL1,FALSE,LL1)
         CALL POP(LL1,1)
         CALL POP(NOD,1)
         CALL POP(FALL_THRU,1)
         CALL POP(FALSE,1)
         CALL POP(TRUE,1)
         CALL EMIT_LOCAL_LABEL(LL1)
         CALL BRANCH_TO2(OPNODE_OPND2(NOD),FALSE,TRUE,FALL_THRU)
 
      ELSEIF (NODE(NOD).AND.OPNODE_OP(NOD).EQ.OP_OR) THEN
 
         CALL GENERATE_LOCAL_LABEL(LL1)
         CALL PUSH(TRUE,1)
         CALL PUSH(FALSE,1)
         CALL PUSH(FALL_THRU,1)
         CALL PUSH(NOD,1)
         CALL PUSH(LL1,1)
         CALL BRANCH_TO2(OPNODE_OPND1(NOD),TRUE,LL1,LL1)
         CALL POP(LL1,1)
         CALL POP(NOD,1)
         CALL POP(FALL_THRU,1)
         CALL POP(FALSE,1)
         CALL POP(TRUE,1)
         CALL EMIT_LOCAL_LABEL(LL1)
         CALL BRANCH_TO2(OPNODE_OPND2(NOD),TRUE,FALSE,FALL_THRU)
 
      ELSEIF (NODE(NOD).AND.OPNODE_OP(NOD).EQ.OP_NOT) THEN
 
         CALL BRANCH_TO2(OPNODE_OPND1(NOD),FALSE,TRUE,FALL_THRU)
 
      ELSEIF (NODE(NOD).AND.OPNODE_OP(NOD).GE.OP_LT.AND.
     #        OPNODE_OP(NOD).LE.OP_GE) THEN
 
         CALL PUSH(TRUE,1)
         CALL PUSH(FALSE,1)
         CALL PUSH(FALL_THRU,1)
         CALL PUSH(NOD,1)
         OPND1=GET_SOMEWHERE(OPNODE_OPND1(NOD),ANY_WHERE)
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL PUSH(OPND1,1)
         OPND2=GET_SOMEWHERE(OPNODE_OPND2(NOD),ANY_WHERE)
         CALL POP(OPND1,1)
         CALL POP(NOD,1)
         CALL POP(FALL_THRU,1)
         CALL POP(FALSE,1)
         CALL POP(TRUE,1)
         CALL EMIT_CODE(OPNODE_OP(NOD),OPND2,OPND1,NULL)
         CALL EMIT_BRANCH(OPNODE_OP(NOD),OPND1,TRUE,FALSE,FALL_THRU)
 
      ELSE
 
         CALL PUSH(TRUE,1)
         CALL PUSH(FALSE,1)
         CALL PUSH(FALL_THRU,1)
         CALL PUSH(NOD,1)
         TEST=GET_SOMEWHERE(NOD,ANY_WHERE)
         CALL POP(NOD,1)
         CALL POP(FALL_THRU,1)
         CALL POP(FALSE,1)
         CALL POP(TRUE,1)
 
         IF (ATOM(TEST).AND.ATOM_SUB(TEST).NE.NULL.AND.
     #        NODE_TYPE(TEST).EQ.S_BYTE) THEN
             
            CALL EMIT_BRANCH(OP_BB,TEST,TRUE,FALSE,FALL_THRU)
 
         ELSEIF (ATOM(TEST).AND.ATOM_SUB(TEST).NE.NULL.AND.
     #        (NODE_TYPE(TEST).EQ.S_WORD.OR.
     #         NODE_TYPE(TEST).EQ.S_INTEGER)) THEN
 
            CALL EMIT_CODE(OP_BIT,NULL,MAKE_FIXED(1,NODE_TYPE(TEST)),
     #                     TEST)
            CALL EMIT_BRANCH(OP_BNE,NULL,TRUE,FALSE,FALL_THRU)
         ELSE
            CALL EMIT_BRANCH(OP_BLB,TEST,TRUE,FALSE,FALL_THRU)
         ENDIF
      ENDIF
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE BRANCH_TO2(NODX,TRUEX,FALSEX,FALL_THRUX)
      IMPLICIT INTEGER*2 (A-Z)
      NOD=NODX
      TRUE=TRUEX
      FALSE=FALSEX
      FALL_THRU=FALL_THRUX
      CALL BRANCH_TO(NOD,TRUE,FALSE,FALL_THRU)
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE EMIT_BRANCH(OP,OPND,TRUE,FALSE,FALL_THRU)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 OPERAND,OPERAND1
      CHARACTER*6 BR
      CHARACTER*32 LABEL
      CHARACTER*6 BRANCH1(1:2,OP_BNE:OP_BB)
      DATA BRANCH1/
     # 'BNEQ  ','BEQL  ',
     # 'BLBS  ','BLBC  ',
     # 'BBS   ','BBC   '/
      CHARACTER*6 BRANCH2(CX_UNSIGNED:CX_SIGNED,1:2,OP_LT:OP_GE)
      DATA BRANCH2/
     #        'BLSSU','BLSS ',
     #        'BGEQU','BGEQ ',
     #        'BGTRU','BGTR ',
     #        'BLEQU','BLEQ ',
     #        'BEQLU','BEQL ',
     #        'BNEQU','BNEQ ',
     #        'BNEQU','BNEQ ',
     #        'BEQLU','BEQL ',
     #        'BLEQU','BLEQ ',
     #        'BGTRU','BGTR ',
     #        'BGEQU','BGEQ ',
     #        'BLSSU','BLSS '/
 
      IF (FALL_THRU.EQ.FALSE) THEN
         BRANCH=TRUE
         TF=1
      ELSEIF (FALL_THRU.EQ.TRUE) THEN
         BRANCH=FALSE
         TF=2
      ELSE
         CALL BUG('EB-0')
      ENDIF
 
      LABEL=LOCAL_LABEL(BRANCH,L1)
 
      IF (OP.GE.OP_LT.AND.OP.LE.OP_GE) THEN
         BR=BRANCH2(CONTEXT(NODE_TYPE(OPND)),TF,OP)
      ELSE
         BR=BRANCH1(TF,OP)
      ENDIF
 
      IF (OP.EQ.OP_BLB) THEN
         OPERAND1=OPERAND(OPND,N1)
         CALL EMIT(BR//' '//OPERAND1(:N1)//','//LABEL(:L1))
      ELSEIF (OP.EQ.OP_BB) THEN
         OPERAND1=OPERAND(OPND,N1)
         CALL EMIT(BR//' #0,'//OPERAND1(:N1)//','//LABEL(:L1))
      ELSE
         CALL EMIT(BR//' '//LABEL(:L1))
      ENDIF
      RETURN
      END
