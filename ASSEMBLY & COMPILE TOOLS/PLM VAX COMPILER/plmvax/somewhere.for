C***********************************************************************
C
C				SOMEWHERE.FOR
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
C    This module of the PL/M-VAX compiler is the main workhorse for
C    code generation.  It is invoked (recursively) to actually generate
C    code from a specified code tree, with the resulting value of the
C    code tree (if any) to be manifested in some desired place ('where').
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    08SEP81  Alex Hunter  1. Temporarily disabled ref_count checking
C			      for atoms. (V5.1)
C			   2. Fixed bug for procedure call when refct>1.
C    15OCT81  Alex Hunter  1. Experimental version. (V5.4)
C    23OCT81  Alex Hunter  1. Correct manifestation for OP_LOC. (V5.6)
C			   2. Improve choice of preferred register.
C
C***********************************************************************
      INTEGER*2 FUNCTION GET_SOMEWHERE(NODX,WHEREX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 OPERAND,OPERAND1,OPERAND2
C
      NOD=NODX
      WHERE=WHEREX
 
      IF (NOD.EQ.NULL) THEN
         GET_SOMEWHERE=NULL
         RETURN
      ENDIF
 
      IF (NODE_REG(NOD).NE.0) THEN
         REG=NODE_REG(NOD)
         NODE_TYPE(REG)=NODE_TYPE(NOD)
         GO TO 100
      ENDIF
 
      IF (LITERAL(NOD)) GO TO 50
 
      IF (CONSTANT(NOD)) GO TO 50
 
      IF (ATOM(NOD)) THEN
         CALL PUSH(NOD,1)
         CALL PUSH(WHERE,1)
         CALL MAKE_ADDRESSABLE(NOD)
         CALL POP(WHERE,1)
         CALL POP(NOD,1)
CCCC     IF (NODE_REFCT(NOD).GT.1) THEN    ! (Temporarily disabled)
CCCC        REG=ALLOCATE_REG(NODE_TYPE(NOD))
CCCC        CALL EMIT_CODE(OP_ASSN,NOD,NULL,REG)
CCCC        NODE_REG(NOD)=REG
CCCC        NODE_TYPE(REG)=NODE_TYPE(NOD)
CCCC        GO TO 100
CCCC     ENDIF
         GO TO 50
      ENDIF
 
C  -- NODE MUST BE AN OPNODE.
 
      IF (OPNODE_OP(NOD).EQ.OP_MOV) THEN
         XX=GET_SOMEWHERE2(OPNODE_OPND1(NOD),OPNODE_OPND2(NOD))
         GET_SOMEWHERE=NULL
         RETURN
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_ASSN) THEN
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         IF (WHERE.GE.REG_MIN.AND.WHERE.LE.REG_MAX) THEN
            REG=GET_SOMEWHERE2(OPNODE_OPND1(NOD),WHERE)
         ELSE
            REG=GET_SOMEWHERE2(OPNODE_OPND1(NOD),ANY_REG)
         ENDIF
         NODE_TYPE(REG)=NODE_TYPE(NOD)
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL PUSH(REG,1)
         CALL MAKE_ADDRESSABLE(OPNODE_OPND2(NOD))
         CALL POP(REG,1)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
         CALL EMIT_CODE(OP_ASSN,REG,NULL,OPNODE_OPND2(NOD))
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(OPNODE_OPND2(NOD)))
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(OPNODE_OPND2(NOD)))
         GO TO 100
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_CALL) THEN
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         NARGS=GET_SOMEWHERE2(OPNODE_OPND2(NOD),ON_STACK)
         CALL POP(NOD,1)
         
         CALL PUSH(NOD,1)
         CALL PUSH(NARGS,1)
         LOC1=GET_SOMEWHERE2(OPNODE_OPND1(NOD),ANY_WHERE)
         CALL POP(NARGS,1)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
 
         OPERAND2=OPERAND(LOC1,N2)
 
         IF (OPERAND2(1:1).EQ.'(') THEN ! Indirect Call.
            OPERAND2='L^V.'//OPERAND2
            N2=N2+4
         ENDIF
 
         IF (MODEL.EQ.2 .OR. MODEL.EQ.4) THEN
            OPND1=MAKE_FIXED2(NARGS,S_LONG)
            OPERAND1=OPERAND(OPND1,N1)
            CALL EMIT('CALLS  '//OPERAND1(:N1)//','//
     #                OPERAND2(:N2))
         ELSEIF (NARGS.EQ.0) THEN
            CALL EMIT('CALLS  #0,'//OPERAND2(:N2))
         ELSE
            CALL EMIT_CODE(OP_B2L,MAKE_FIXED2(NARGS,S_BYTE),NULL,
     #                       ON_STACK)
            CALL EMIT('CALLG  (R10),'//OPERAND2(:N2))
            CALL EMIT_CODE(OP_ADD,MAKE_FIXED2((NARGS+1)*4,S_LONG),NULL,
     #                       10)
         ENDIF
 
         NODE_TYPE(R0)=NODE_TYPE(NOD)
 
         IF (WHERE.EQ.ANY_WHERE .OR. WHERE.EQ.ANY_REG .OR.
     #       NODE_REFCT(NOD).GT.1) THEN
 
            REG=ALLOCATE_REG(NODE_TYPE(NOD))
            CALL EMIT_CODE(OP_ASSN,R0,NULL,REG)
            NODE_REG(NOD)=REG
            NODE_TYPE(REG)=NODE_TYPE(NOD)
            GO TO 100
         
         ELSEIF (WHERE.EQ.ON_STACK) THEN
            CALL EMIT_CODE(OP_ASSN,R0,NULL,ON_STACK)
            CALL DECREMENT_VALUE_COUNTS(NOD)
            GET_SOMEWHERE=ON_STACK
            RETURN
 
         ELSEIF (WHERE.GE.REG_MIN .AND. WHERE.LE.REG_MAX) THEN
            IF (WHERE.NE.R0) THEN
               CALL EMIT_CODE(OP_ASSN,R0,NULL,WHERE)
               CALL DECREMENT_VALUE_COUNTS(NOD)
            ENDIF
            GET_SOMEWHERE=WHERE
            RETURN
 
         ELSE		! (WHERE IS ATOM)
            IF (ATOM_SUB(WHERE).EQ.NULL.AND.ATOM_BASE(WHERE).EQ.NULL)
     #         THEN
               CALL EMIT_CODE(OP_ASSN,R0,NULL,WHERE)
               CALL DECREMENT_VALUE_COUNTS(NOD)
               GET_SOMEWHERE=WHERE
               RETURN
            ENDIF
            REG=ALLOCATE_REG(NODE_TYPE(NOD))
            CALL EMIT_CODE(OP_ASSN,R0,NULL,REG)
            NODE_REG(NOD)=REG
            NODE_TYPE(REG)=NODE_TYPE(NOD)
            GO TO 150
         ENDIF
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_ARG) THEN
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         XX=GET_SOMEWHERE2(OPNODE_OPND2(NOD),ON_STACK)
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         NARGS=GET_SOMEWHERE2(OPNODE_OPND1(NOD),ON_STACK)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
         GET_SOMEWHERE=NARGS+1
         RETURN
 
      ELSEIF ((OPNODE_OP(NOD).EQ.OP_ADWC.OR.OPNODE_OP(NOD).EQ.OP_SBWC)
     #        .AND.OPNODE_OPND1(NOD).NE.WHERE) THEN
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         REG=GET_SOMEWHERE2(OPNODE_OPND1(NOD),ANY_REG)
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL PUSH(REG,1)
         LOC2=GET_SOMEWHERE2(OPNODE_OPND2(NOD),ANY_WHERE)
         CALL POP(REG,1)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
         NODE_REG(OPNODE_OPND1(NOD))=0
         CALL EMIT_CODE(OPNODE_OP(NOD),LOC2,REG,REG)
         NODE_REG(NOD)=REG
         NODE_TYPE(REG)=NODE_TYPE(NOD)
         GO TO 100
      ENDIF
 
      IF (OPNODE_OP(NOD).EQ.OP_LOC) THEN
         LOC1=OPNODE_OPND1(NOD)
         LOC2=OPNODE_OPND2(NOD)
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         CALL PUSH(LOC1,1)
         CALL PUSH(LOC2,1)
         CALL MAKE_ADDRESSABLE(LOC1)
         CALL POP(LOC2,1)
         CALL PUSH(LOC2,1)
         CALL MAKE_ADDRESSABLE(LOC2)
         CALL POP(LOC2,1)
         CALL POP(LOC1,1)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
      ELSE
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         LOC1=GET_SOMEWHERE2(OPNODE_OPND1(NOD),ANY_WHERE)
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL PUSH(LOC1,1)
         LOC2=GET_SOMEWHERE2(OPNODE_OPND2(NOD),ANY_WHERE)
         CALL POP(LOC1,1)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
      ENDIF
 
      IF (OPNODE_OP(NOD).EQ.OP_THEN.OR.OPNODE_OP(NOD).EQ.OP_ALSO) THEN
         GET_SOMEWHERE=NULL
         RETURN
 
      ELSEIF (WHERE.EQ.ANY_WHERE .OR. WHERE.EQ.ANY_REG .OR.
     #    NODE_REFCT(NOD).GT.1) THEN
 
         IF (OPNODE_OP(NOD).EQ.OP_LOC) THEN
            IF (ATOM(LOC1)) THEN
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(LOC1))
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(LOC1))
            ENDIF
         ELSE
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND1(NOD))
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND2(NOD))
         ENDIF
         IF (REGISTER(LOC1)) THEN
            PREFERRED_REG=LOC1
         ELSE
            PREFERRED_REG=LOC2
         ENDIF
         REG=ALLOCATE_REG_WITH_PREFERENCE(NODE_TYPE(NOD),PREFERRED_REG)
         CALL EMIT_CODE(OPNODE_OP(NOD),LOC2,LOC1,REG)
         NODE_REG(NOD)=REG
         NODE_TYPE(REG)=NODE_TYPE(NOD)
         GO TO 100
 
      ELSEIF (WHERE.EQ.ON_STACK) THEN
         CALL EMIT_CODE(OPNODE_OP(NOD),LOC2,LOC1,ON_STACK)
         IF (OPNODE_OP(NOD).EQ.OP_LOC) THEN
            IF (ATOM(LOC1)) THEN
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(LOC1))
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(LOC1))
            ENDIF
         ELSE
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND1(NOD))
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND2(NOD))
         ENDIF
         CALL DECREMENT_REFERENCE_COUNTS(NOD)
         GET_SOMEWHERE=ON_STACK
         RETURN
 
      ELSEIF (WHERE.GE.REG_MIN.AND.WHERE.LE.REG_MAX) THEN
         CALL EMIT_CODE(OPNODE_OP(NOD),LOC2,LOC1,WHERE)
         IF (OPNODE_OP(NOD).EQ.OP_LOC) THEN
            IF (ATOM(LOC1)) THEN
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(LOC1))
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(LOC1))
            ENDIF
         ELSE
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND1(NOD))
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND2(NOD))
         ENDIF
         NODE_REG(NOD)=WHERE
         GET_SOMEWHERE=WHERE
         RETURN
 
      ELSE     ! (WHERE IS ATOM)
 
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         CALL PUSH(LOC1,1)
         CALL PUSH(LOC2,1)
         CALL MAKE_ADDRESSABLE(WHERE)
         CALL POP(LOC2,1)
         CALL POP(LOC1,1)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
         CALL EMIT_CODE(OPNODE_OP(NOD),LOC2,LOC1,WHERE)
         IF (OPNODE_OP(NOD).EQ.OP_LOC) THEN
            IF (ATOM(LOC1)) THEN
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(LOC1))
               CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(LOC1))
            ENDIF
         ELSE
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND1(NOD))
            CALL DECREMENT_VALUE_COUNTS(OPNODE_OPND2(NOD))
         ENDIF
         CALL DECREMENT_REFERENCE_COUNTS(NOD)
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(WHERE))
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(WHERE))
         GET_SOMEWHERE=WHERE
         RETURN
      ENDIF
 
C --- NODE IS ATOM OR LITERAL.
 
50    IF (WHERE.EQ.ANY_WHERE) THEN
         GET_SOMEWHERE=NOD
         RETURN
 
      ELSEIF (WHERE.EQ.ANY_REG) THEN
         REG=ALLOCATE_REG(NODE_TYPE(NOD))
         CALL EMIT_CODE(OP_ASSN,NOD,NULL,REG)
         NODE_REG(NOD)=REG
         GET_SOMEWHERE=REG
         RETURN
 
      ELSEIF (WHERE.EQ.ON_STACK) THEN
         CALL EMIT_CODE(OP_ASSN,NOD,NULL,ON_STACK)
         CALL DECREMENT_VALUE_COUNTS(NOD)
         GET_SOMEWHERE=ON_STACK
         RETURN
 
      ELSEIF (WHERE.GE.REG_MIN.AND.WHERE.LE.REG_MAX) THEN
         CALL EMIT_CODE(OP_ASSN,NOD,NULL,WHERE)
         NODE_REG(NOD)=WHERE
         GET_SOMEWHERE=WHERE
         RETURN
 
      ELSE    ! (WHERE IS ATOM)
 
         CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         CALL MAKE_ADDRESSABLE(WHERE)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
         CALL EMIT_CODE(OP_ASSN,NOD,NULL,WHERE)
         CALL DECREMENT_VALUE_COUNTS(NOD)
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(WHERE))
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(WHERE))
         GET_SOMEWHERE=WHERE
         RETURN
      ENDIF
 
C --- VALUE OF NODE IS ALREADY IN REGISTER.
 
100   IF (WHERE.EQ.ANY_WHERE .OR. WHERE.EQ.ANY_REG .OR.
     #    WHERE.EQ.REG) THEN
         GET_SOMEWHERE=REG
         RETURN
 
      ELSEIF(WHERE.EQ.ON_STACK) THEN
         CALL EMIT_CODE(OP_ASSN,REG,NULL,ON_STACK)
         CALL DECREMENT_VALUE_COUNTS(NOD)
         GET_SOMEWHERE=ON_STACK
         RETURN
 
      ELSEIF (WHERE.GE.REG_MIN.AND.WHERE.LE.REG_MAX) THEN
         CALL EMIT_CODE(OP_ASSN,REG,NULL,WHERE)
         CALL DECREMENT_VALUE_COUNTS(NOD)   ! ???
         GET_SOMEWHERE=WHERE
         RETURN
 
      ELSE    ! (WHERE IS ATOM)
 
150      CALL PUSH(WHERE,1)
         CALL PUSH(NOD,1)
         CALL PUSH(REG,1)
         CALL MAKE_ADDRESSABLE(WHERE)
         CALL POP(REG,1)
         CALL POP(NOD,1)
         CALL POP(WHERE,1)
         CALL EMIT_CODE(OP_ASSN,REG,NULL,WHERE)
         CALL DECREMENT_VALUE_COUNTS(NOD)
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(WHERE))
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(WHERE))
         GET_SOMEWHERE=WHERE
         RETURN
      ENDIF
      END
C--------------------------------------------------------------
      INTEGER*2 FUNCTION GET_SOMEWHERE2(NODX,WHEREX)
      IMPLICIT INTEGER*2 (A-Z)
      GET_SOMEWHERE2=GET_SOMEWHERE(NODX,WHEREX)
      RETURN
      END
C--------------------------------------------------------------
      SUBROUTINE MAKE_ADDRESSABLE(ATMX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      ATM=ATMX
      IF (ATOM(ATM)) THEN
         CALL PUSH(ATM,1)
         BASEREG=GET_SOMEWHERE(ATOM_BASE(ATM),ANY_REG)
         CALL POP(ATM,1)
         XREG=GET_SOMEWHERE(ATOM_SUB(ATM),ANY_REG)
      ENDIF
      RETURN
      END
