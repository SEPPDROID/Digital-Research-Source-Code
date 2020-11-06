C***********************************************************************
C
C                               SCOPES.FOR
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
C    This module of the PL/M-VAX compiler processes END statements,
C    IF statements, ELSE statements, and DO statements.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C    10NOV81  Alex Hunter  1. Fix DRC bugs. (V6.0)
C    14JAN82  Alex Hunter  1. Allow END <keyword>. (V6.5)
C
C-----------------------------------------------------------------------
      SUBROUTINE END_STATEMENT
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CALL MATCH(K_END)
      CALL POPC(LAST_LABEL)
      IF (TT.EQ.ID .OR. TT.GE.101.AND.TT.LE.199) THEN
         IF (LAST_LABEL.NE.IDENTIFIER) THEN
            CALL ERROR('END DOESN''T MATCH '//LAST_LABEL)
         ENDIF
         CALL GETTOK
      ENDIF
      CALL MATCH(D_SEMI)
      RETURN
      END
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION CONDITIONAL_CLAUSE(N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CALL MATCH(K_IF)
      CALL BREAK			! For now.
      CONDITION=MASSAGE(EXPRESSION(1),CX_UNSIGNED)
      CALL GENERATE_LOCAL_LABEL(LL0)
      CALL GENERATE_LOCAL_LABEL(LL1)
      CALL GENERATE_LOCAL_LABEL(LL2)
      CALL BRANCH_TO(CONDITION,LL1,LL0,LL0)
      CALL EMIT_LOCAL_LABEL(LL0)
      CALL EMIT('BRW    '//LOCAL_LABEL(LL2,N0))
      CALL EMIT_LOCAL_LABEL(LL1)
      CALL MATCH(K_THEN)
      CALL PUSH(LL2,1)
      CALL BREAK
      CALL UNIT
      CALL BREAK
      CALL POP(LL2,1)
      IF (TT.EQ.K_ELSE) THEN
         CALL GETTOK
         IF (PATH) THEN
            CALL GENERATE_LOCAL_LABEL(LL3)
            CALL EMIT('BRW    '//LOCAL_LABEL(LL3,N0))
         ELSE
            LL3=0
         ENDIF
         CALL EMIT_LOCAL_LABEL(LL2)
         CALL PUSH(LL3,1)
         CALL UNIT
         CALL BREAK
         CALL POP(LL3,1)
         CALL EMIT_LOCAL_LABEL(LL3)
      ELSE
         CALL EMIT_LOCAL_LABEL(LL2)
      ENDIF
      CONDITIONAL_CLAUSE=NULL
      RETURN
      END
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION DO_BLOCK(N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CALL PUSHC(LAST_LABEL)
      CALL MATCH(K_DO)
      IF (TT.EQ.D_SEMI) THEN
         CALL GETTOK
         CALL BLOCK_BEGIN
         CALL DECLARATIONS
         CALL UNITS
         CALL BLOCK_END
         CALL END_STATEMENT
      ELSEIF (TT.EQ.K_CASE) THEN
         CALL DO_CASE_BLOCK
      ELSEIF (TT.EQ.K_WHILE) THEN
         CALL BREAK			! For now.
         CALL GETTOK
         CALL GENERATE_LOCAL_LABEL(LL0)
         CALL GENERATE_LOCAL_LABEL(LL1)
         CALL GENERATE_LOCAL_LABEL(LL2)
         CALL GENERATE_LOCAL_LABEL(LL3)
         CALL EMIT_LOCAL_LABEL(LL1)
         CONDITION=MASSAGE(EXPRESSION(1),CX_UNSIGNED)
         CALL MATCH(D_SEMI)
         IF (FIXLIT(CONDITION)) THEN
            IF ((FIXED_VAL(CONDITION).AND.1).NE.0) THEN
               LL3=0		! DO WHILE TRUE.
            ELSE
                                ! DO WHILE FALSE.
               CALL EMIT('BRW     '//LOCAL_LABEL(LL3,N0))
               PATH=.FALSE.
            ENDIF
         ELSE
            CALL BRANCH_TO(CONDITION,LL2,LL0,LL0)
            CALL EMIT_LOCAL_LABEL(LL0)
            CALL EMIT('BRW    '//LOCAL_LABEL(LL3,N0))
            CALL EMIT_LOCAL_LABEL(LL2)
         ENDIF
         CALL PUSH(LL1,1)
         CALL PUSH(LL3,1)
         CALL BREAK
         CALL UNITS
         CALL BREAK
         CALL END_STATEMENT
         CALL POP(LL3,1)
         CALL POP(LL1,1)
         IF (PATH) CALL EMIT('BRW    '//LOCAL_LABEL(LL1,N0))
         PATH=.FALSE.
         CALL EMIT_LOCAL_LABEL(LL3)
      ELSE
         CALL ITERATIVE_DO_BLOCK
      ENDIF
      DO_BLOCK=NULL
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE ITERATIVE_DO_BLOCK
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 OPERAND,OPERAND1,OPERAND2,OPERAND3
      INTEGER*2 PROMOTE(1:6)
      DATA PROMOTE /
     # S_WORD,S_LONG,S_INTEGER,S_PTR,S_REAL,S_LONG/
      CHARACTER*6 ACB(1:6)
      DATA ACB/
     # 'ACBB','ACBW','ACBW','ACBL','ACBF','ACBL'/
 
      CALL BREAK			! For now.
      CALL SIMPLE_VARIABLE(DTYPE)
      INDEX=MAKE_ATOM(SYMBOL_INDEX,MEMBER_INDEX,NULL,NULL,DTYPE,0,0)
      CALL MATCH(D_EQ)
      START=EXPRESSION(1)
      CALL MATCH(K_TO)
      LIMIT=EXPRESSION(1)
      IF (TT.EQ.K_BY) THEN
         CALL GETTOK
         STEP=EXPRESSION(1)
      ELSE
         STEP=MAKE_FIXED(1,0)
      ENDIF
 
      CALL RESOLVE_CONTEXT(START)
      CALL RESOLVE_CONTEXT(STEP)
      CALL RESOLVE_CONTEXT(LIMIT)
  
      DCX=CONTEXT(DTYPE)
      IF (NODE_CONTEXT(START).EQ.0) CALL SET_CONTEXT(START,DCX)
      IF (NODE_CONTEXT(STEP).EQ.0) CALL SET_CONTEXT(STEP,DCX)
      IF (NODE_CONTEXT(LIMIT).EQ.0) CALL SET_CONTEXT(LIMIT,DCX)
 
      CALL COERCE_TYPES(START)
      CALL COERCE_TYPES(STEP)
      CALL COERCE_TYPES(LIMIT)
 
      FTYPE=PROMOTE(DTYPE)
      START=FORCE_TYPE(START,FTYPE)
      STEP=FORCE_TYPE(STEP,FTYPE)
      LIMIT=FORCE_TYPE(LIMIT,FTYPE)
 
      START=FOLD_CONSTANTS(START)
      STEP=FOLD_CONSTANTS(STEP)
      LIMIT=FOLD_CONSTANTS(LIMIT)
 
      IF (FTYPE.EQ.DTYPE) THEN
         WHERE=INDEX
      ELSE
         WHERE=R0
         NODE_TYPE(R0)=FTYPE
         NODE_REG(R0)=R0
      ENDIF
 
      CALL SAVE_CODE_TREE
 
      CALL PUSH(INDEX,1)
      CALL PUSH(STEP,1)
      CALL PUSH(LIMIT,1)
      CALL PUSH(DTYPE,1)
      CALL PUSH(FTYPE,1)
      CALL PUSH(WHERE,1)
 
      CALL GENERATE_LOCAL_LABEL(LL1)
      LL2=0
      LL3=0
  
      IF (LITERAL(START).AND.LITERAL(STEP).AND.LITERAL(LIMIT)) THEN
         CALL COMPUTE_REFERENCE_COUNTS(START)
         XX=GET_SOMEWHERE(START,WHERE)
 
         IF (DTYPE.EQ.S_REAL .AND.
     #         (FLOAT_VAL(STEP).LT.0.0 .AND.
     #            FLOAT_VAL(START).LT.FLOAT_VAL(LIMIT)
     #     .OR. FLOAT_VAL(STEP).GE.0.0 .AND.
     #            FLOAT_VAL(START).GT.FLOAT_VAL(LIMIT))
     # .OR.  DTYPE.NE.S_REAL .AND.
     #         (FIXED_VAL(STEP).LT.0 .AND.
     #            FIXED_VAL(START).LT.FIXED_VAL(LIMIT)
     #     .OR. FIXED_VAL(STEP).GE.0 .AND.
     #            FIXED_VAL(START).GT.FIXED_VAL(LIMIT))) THEN
         
            CALL GENERATE_LOCAL_LABEL(LL3)
            CALL EMIT('BRW    '//LOCAL_LABEL(LL3,N0))
         ENDIF
 
      ELSE
         INIT=MAKE_NODE(OP_SUB,START,STEP,FTYPE,0,0)
         INIT=FOLD_CONSTANTS(INIT)
         INIT=MERGE(INIT)
         CALL COMPUTE_REFERENCE_COUNTS(INIT)
         XX=GET_SOMEWHERE(INIT,WHERE)
         CALL GENERATE_LOCAL_LABEL(LL2)
         CALL EMIT('BRW    '//LOCAL_LABEL(LL2,N0))
      ENDIF
 
      CALL EMIT_LOCAL_LABEL(LL1)
 
      IF (INDEX.NE.WHERE) THEN
         CODE=FORCE_TYPE(WHERE,DTYPE)
         CALL COMPUTE_REFERENCE_COUNTS(CODE)
         XX=GET_SOMEWHERE(CODE,INDEX)
      ENDIF
 
      CALL PUSH(LL1,1)
      CALL PUSH(LL2,1)
      CALL PUSH(LL3,1)
 
      CALL MATCH(D_SEMI)
      CALL BREAK
      CALL UNITS
      CALL BREAK
   
      CALL POP(LL3,1)
      CALL POP(LL2,1)
      CALL POP(LL1,1)
 
      CALL POP(WHERE,1)
      CALL POP(FTYPE,1)
      CALL POP(DTYPE,1)
      CALL POP(LIMIT,1)
      CALL POP(STEP,1)
      CALL POP(INDEX,1)
 
      CALL RESTORE_CODE_TREE
 
      IF (INDEX.NE.WHERE) THEN
         NODE_TYPE(R0)=FTYPE
         NODE_REG(R0)=R0
         CODE=FORCE_TYPE(INDEX,FTYPE)
         CALL COMPUTE_REFERENCE_COUNTS(CODE)
         XX=GET_SOMEWHERE(CODE,WHERE)
      ENDIF
 
      IF (LL2.NE.0) CALL EMIT_LOCAL_LABEL(LL2)
 
      LIMIT=MERGE(LIMIT)
      CALL COMPUTE_REFERENCE_COUNTS(LIMIT)
      STEP=MERGE(STEP)
      CALL COMPUTE_REFERENCE_COUNTS(STEP)
 
      OPND1=GET_SOMEWHERE(LIMIT,ANY_WHERE)
      OPND2=GET_SOMEWHERE(STEP,ANY_WHERE)
 
      OPERAND1=OPERAND(OPND1,N1)
      OPERAND2=OPERAND(OPND2,N2)
      OPERAND3=OPERAND(WHERE,N3)
 
      CALL EMIT(ACB(FTYPE)//' '//OPERAND1(:N1)//','//OPERAND2(:N2)//
     #          ','//OPERAND3(:N3)//','//LOCAL_LABEL(LL1,N0))
 
      IF (LL3.NE.0) CALL EMIT_LOCAL_LABEL(LL3)
 
      IF (INDEX.NE.WHERE) THEN
         CODE=FORCE_TYPE(WHERE,DTYPE)
         CALL COMPUTE_REFERENCE_COUNTS(CODE)
         XX=GET_SOMEWHERE(CODE,INDEX)
      ENDIF
 
      CALL BREAK
      CALL END_STATEMENT
 
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE DO_CASE_BLOCK
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 OPERAND,OPERAND1,OPERAND2
      CHARACTER*32 LLL98,LLL
      CHARACTER*6 CASE(1:6)
      DATA CASE/
     # 'CASEB','CASEW','CASEW','----','----','CASEL'/
      INTEGER*2 LL(0:255)
 
      CALL BREAK			! For now.
      CALL GETTOK
      SELECTOR=EXPRESSION(1)
 
      CALL RESOLVE_CONTEXT(SELECTOR)
      IF (NODE_CONTEXT(SELECTOR).EQ.0)
     #   CALL SET_CONTEXT(SELECTOR,CX_UNSIGNED)
      CALL COERCE_TYPES(SELECTOR)
      IF (NODE_TYPE(SELECTOR).EQ.S_PTR .OR.
     #    NODE_TYPE(SELECTOR).EQ.S_REAL .OR.
     #    NODE_TYPE(SELECTOR).GT.S_LONG) THEN
         SELECTOR=FORCE_TYPE(SELECTOR,S_LONG)
      ENDIF
 
      SELECTOR=FOLD_CONSTANTS(SELECTOR)
      SELECTOR=MERGE(SELECTOR)
      CALL COMPUTE_REFERENCE_COUNTS(SELECTOR)
 
      CALL SAVE_CODE_TREE
      CALL PUSH(SELECTOR,1)
 
      CALL GENERATE_LOCAL_LABEL(LL97)
      CALL GENERATE_LOCAL_LABEL(LL98)
      CALL GENERATE_LOCAL_LABEL(LL99)
 
      CALL EMIT('BRW    '//LOCAL_LABEL(LL97,N0))
 
      CALL PUSH(LL97,1)
      CALL PUSH(LL98,1)
 
      I=-1
 
      CALL MATCH(D_SEMI)
 
10    IF (TT.NE.K_END) THEN
         IF (I.GE.255) THEN
            CALL ERROR('TOO MANY CASES')
         ELSE
            I=I+1
            CALL GENERATE_LOCAL_LABEL(LL0)
            CALL EMIT_LOCAL_LABEL(LL0)
            CALL PUSH(LL0,1)
         ENDIF
      
         CALL PUSH(LL99,1)
         CALL PUSH(I,1)
 
         CALL UNIT
         CALL BREAK
 
         CALL POP(I,1)
         CALL POP(LL99,1)
 
         IF (PATH) THEN
            CALL EMIT('BRW    '//LOCAL_LABEL(LL99,N0))
         ENDIF
  
         GO TO 10
      ENDIF
 
      DO 15 J=I,0,-1
         CALL POP(LL(J),1)
15    CONTINUE
 
      CALL POP(LL98,1)
      CALL POP(LL97,1)
      CALL POP(SELECTOR,1)
      CALL RESTORE_CODE_TREE
 
      CALL EMIT_LOCAL_LABEL(LL97)
 
      IF (I.GE.0) THEN
         OPND1=GET_SOMEWHERE(SELECTOR,ANY_WHERE)
         OPERAND1=OPERAND(OPND1,N1)
         OPERAND2=OPERAND(MAKE_FIXED2(I,0),N2)
 
         CALL EMIT(CASE(NODE_TYPE(SELECTOR))//' '//OPERAND1(:N1)//
     #             ',#0,'//OPERAND2(:N2))
 
         CALL EMIT_LOCAL_LABEL(LL98)
         LLL98=LOCAL_LABEL(LL98,L98)
 
         DO 20 J=0,I
        
            LLL=LOCAL_LABEL(LL(J),L)
            CALL EMIT('.SIGNED_WORD  '//LLL(:L)//'-'//LLL98(:L98))
 
20       CONTINUE
 
         CALL EMIT_LOCAL_LABEL(LL99)
      ENDIF
 
      CALL BREAK
      CALL END_STATEMENT
     
      RETURN
      END
