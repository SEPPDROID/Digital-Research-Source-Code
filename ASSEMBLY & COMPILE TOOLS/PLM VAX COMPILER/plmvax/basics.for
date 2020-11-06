C***********************************************************************
C
C                               BASICS.FOR
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
C    This module of the PL/M-VAX compiler parses and generates code for
C    the following 'basic' statement types:  assignment statements,
C    call statements, goto statements, return statements, and i8086-
C    dependent statements.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    08SEP81  Alex Hunter  1. Use DO-WHILE (cosmetic change). (V5.1)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C    10NOV81  Alex Hunter  1. Add EFFECTS module. (V6.0)
C    14JAN82  Alex Hunter  1. Treat GOTO <keyword> as GOTO <identifier>.
C			      (V6.5)
C
C***********************************************************************
      INTEGER*2 FUNCTION ASSIGNMENT_STATEMENT(N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
   
      CODE=NULL
 
10    CALL MUSTBE(ID)
      CALL LOOKUP_SYMBOL
      IF (SYMBOL_KIND(SYMBOL_INDEX).EQ.S_PROC) THEN
         CALL ERROR('PROCEDURE ILLEGAL AS LEFTHAND SIDE OF ASSIGNMENT: '
     #              //SYMBOL_PLM_ID(SYMBOL_INDEX))
      ENDIF
      LHS=VARIABLE_REFERENCE(0)
      CODE=MAKE_NODE(OP_ALSO,CODE,MAKE_NODE(OP_MOV,NULL,LHS,0,0,0),
     #               0,0,0)
      IF (TT.EQ.D_COMMA) THEN
         CALL GETTOK
         GO TO 10
      ENDIF
 
      CALL MATCH(D_EQ)
      RHS=EXPRESSION(1)
 
      OPNODE_OPND1(OPNODE_OPND2(CODE))=RHS
      CALL DETERMINE_EFFECTS_OF_ASSIGNMENT(LHS)
 
      CODE1=OPNODE_OPND1(CODE)
      DO WHILE (CODE1.NE.NULL)
         OPNODE_OPND1(OPNODE_OPND2(CODE1))=REPLICA(RHS)
         LHS=OPNODE_OPND2(OPNODE_OPND2(CODE1))
         CALL DETERMINE_EFFECTS_OF_ASSIGNMENT(LHS)
         CODE1=OPNODE_OPND1(CODE1)
      ENDDO
 
      CALL MATCH(D_SEMI)
      ASSIGNMENT_STATEMENT=CODE
 
      RETURN
      END
C----------------------------------------------------
      INTEGER*2 FUNCTION CALL_STATEMENT(N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*2 ARGS(100)
 
      CALL MATCH(K_CALL)
      CALL MUSTBE(ID)
      CALL LOOKUP_SYMBOL
      PROC_IX=SYMBOL_INDEX
      IF (SYMBOL_KIND(PROC_IX).EQ.S_PROC) THEN
         IF (SYMBOL_TYPE(PROC_IX).NE.0) THEN
            CALL WARN('TYPED PROCEDURE USED IN CALL STATEMENT: '//
     #                SYMBOL_PLM_ID(PROC_IX))
         ENDIF
         PROC_BASE=NULL
         CALL GETTOK
      ELSE
         PROC_BASE=DATA_REFERENCE(0,2)
         IF (NODE_TYPE(PROC_BASE).NE.S_PTR.AND.
     #       NODE_TYPE(PROC_BASE).NE.S_WORD.AND.
     #       NODE_TYPE(PROC_BASE).NE.S_LONG) THEN
            CALL WARN('INDIRECT CALL THRU NON-WORD/POINTER '//
     #  'PROBABLY WON''T WORK')
         ENDIF
         PROC_IX=0
      ENDIF
 
      ARGLIST=NULL
      NARGS=0
      IF (TT.EQ.D_LP) THEN
10       CALL GETTOK
         NARGS=NARGS+1
         ARGLIST=MAKE_NODE(OP_ARG,ARGLIST,EXPRESSION(1),0,0,0)
         IF (TT.EQ.D_COMMA) GO TO 10
         CALL MATCH(D_RP)
      ENDIF
 
      IF (PROC_IX.NE.0.AND.NARGS.NE.SYMBOL_LIST_SIZE(PROC_IX)) THEN
         CALL WARN('WRONG NUMBER OF ARGS TO '//
     #              SYMBOL_PLM_ID(PROC_IX))
      ENDIF
 
      PROC=MAKE_ATOM(PROC_IX,0,PROC_BASE,NULL,S_BYTE,0,0)
      CODE=MAKE_NODE(OP_CALL,PROC,ARGLIST,0,0,0)
      CODE=MAKE_NODE(OP_MOV,CODE,R0,0,0,0)
      NODE_TYPE(R0)=S_BYTE
 
      CALL DETERMINE_EFFECTS_OF_CALLING(PROC_IX)
 
      CALL MATCH(D_SEMI)
      CALL_STATEMENT=CODE
      RETURN
      END
C----------------------------------------------------
      INTEGER*2 FUNCTION GOTO_STATEMENT(N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      IF (TT.EQ.K_GO) THEN
         CALL GETTOK
         CALL MATCH(K_TO)
      ELSE
         CALL MATCH(K_GOTO)
      ENDIF
      CALL BREAK
      IF (TT.LT.101.OR.TT.GT.199) CALL MUSTBE(ID)
      H=HASH(IDENTIFIER)
      SYMBOL_INDEX=HASH_BUCKET(H)
10    IF (SYMBOL_INDEX.GE.SYMBOL_TOP(BLOCK_LEVEL-1)+1) THEN
         IF (SYMBOL_PLM_ID(SYMBOL_INDEX).EQ.IDENTIFIER) THEN
            GO TO 20
         ENDIF
         SYMBOL_INDEX=SYMBOL_CHAIN(SYMBOL_INDEX)
         GO TO 10
      ENDIF
 
      CALL ENTER_SYMBOL
      SYMBOL_KIND(SYMBOL_INDEX)=S_LABEL
      SYMBOL_REF(SYMBOL_INDEX)=S_UNRESOLVED
 
20    IF (SYMBOL_REF(SYMBOL_INDEX).EQ.S_EXT) THEN
         CALL EMIT('JMP     '//SYMBOL_VAX_ID(SYMBOL_INDEX))
      ELSE
         CALL EMIT('BRW     '//SYMBOL_VAX_ID(SYMBOL_INDEX))
      ENDIF
 
      PATH=.FALSE.
 
      CALL GETTOK
      CALL MATCH(D_SEMI)
      GOTO_STATEMENT=NULL
      RETURN
      END
C----------------------------------------------------
      INTEGER*2 FUNCTION RETURN_STATEMENT(N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      CALL MATCH(K_RETURN)
 
      TYPE=SYMBOL_TYPE(PROC_INDEX(PROC_LEVEL))
 
      IF (TT.NE.D_SEMI) THEN
 
         IF (TYPE.EQ.0) THEN
            CALL ERROR('CAN''T RETURN VALUE FROM UNTYPED PROCEDURE')
            TYPE=S_LONG
         ENDIF
 
         RESULT=MAKE_NODE(OP_BYTE+TYPE-S_BYTE,EXPRESSION(1),NULL,0,0,0)
         RESULT=MAKE_NODE(OP_MOV,RESULT,R0,0,0,0)
         NODE_TYPE(R0)=TYPE
         BASIC_BLOCK=MAKE_NODE(OP_THEN,BASIC_BLOCK,RESULT,0,0,0)
 
      ELSEIF (TYPE.NE.0) THEN
         CALL ERROR('MUST RETURN VALUE FROM TYPED PROCEDURE')
      ENDIF
 
      CALL BREAK
      CALL MATCH(D_SEMI)
      CALL EMIT('RET')
 
      PATH=.FALSE.
 
      RETURN_STATEMENT=NULL
      RETURN
      END
C----------------------------------------------------
      INTEGER*2 FUNCTION i8086_DEPENDENT_STATEMENTS(N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CALL GETTOK
      CALL MATCH(D_SEMI)
      CALL WARN('8086 DEPENDENT STATEMENT IGNORED')
      i8086_DEPENDENT_STATEMENTS=NULL
      RETURN
      END
