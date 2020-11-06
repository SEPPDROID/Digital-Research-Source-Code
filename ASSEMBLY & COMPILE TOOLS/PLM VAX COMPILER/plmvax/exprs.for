C***********************************************************************
C
C                               EXPRS.FOR
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
C    This module of the PL/M-VAX compiler parses expressions and
C    generates the corresponding code trees.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Basic block anaylsis. (V5.5)
C    10NOV81  Alex Hunter  1. Add calls to EFFECTS module. (V6.0)
C    12NOV81  Alex Hunter  1. Delete reference to S_COMMON. (V6.1)
C
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION PRIMARY(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      IF (TT.EQ.FIXCON) THEN
         PRIMARY=MAKE_FIXED(FIXVAL,0)
         CALL GETTOK
      ELSEIF (TT.EQ.FLOATCON) THEN
         PRIMARY=MAKE_FLOAT(FLOATVAL,S_REAL)
         CALL GETTOK
      ELSEIF (TT.EQ.STRCON) THEN
         IF (STRLEN.GT.2) THEN
            CALL ERROR('STRING CONSTANT HAS MORE THAN 2 CHARACTERS')
         ENDIF
         IF (STRLEN.EQ.1) THEN
            PRIMARY=MAKE_FIXED2(ICHAR(STRING(1:1)),S_BYTE)
         ELSE
            PRIMARY=MAKE_FIXED2(ICHAR(STRING(1:1))*256
     #                        +ICHAR(STRING(2:2)),S_WORD)
         ENDIF
         CALL GETTOK
      ELSEIF (TT.EQ.ID) THEN
         PRIMARY=VARIABLE_REFERENCE(1)
      ELSEIF (TT.EQ.D_DOT.OR.TT.EQ.D_AT) THEN
         PRIMARY=LOCATION_REFERENCE(1)
      ELSEIF (TT.EQ.D_LP) THEN
         CALL GETTOK
         PRIMARY=EXPRESSION(1)
         CALL MATCH(D_RP)
      ELSE
         CALL MUSTBE(NT_EXPRESSION)
      ENDIF
      RETURN
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION VARIABLE_REFERENCE(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      CALL MUSTBE(ID)
      CALL LOOKUP_SYMBOL
      GO TO (100,200,200,300,100), SYMBOL_KIND(SYMBOL_INDEX)
100   CALL ERROR('IDENTIFIER ILLEGAL IN THIS CONTEXT: '//
     #             SYMBOL_PLM_ID(SYMBOL_INDEX))
      VARIABLE_REFERENCE=DUMMY
      CALL GETTOK
      RETURN
C
C----  SCALAR OR ARRAY.
C
200   VARIABLE_REFERENCE=DATA_REFERENCE(REFS,.FALSE.)
      RETURN
C
C----  PROCEDURE.
C
300   VARIABLE_REFERENCE=FUNCTION_REFERENCE(REFS)
      RETURN
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION DATA_REFERENCE(DREFS,MODEX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      LOGICAL*2 PARTIAL_OK
      EQUIVALENCE (PARTIAL_OK,MODE)
      COMMON /BUILTINS/ SYM_SUBS,MEM_SUBS
      REFS=DREFS
      MODE=MODEX
      CALL MATCH(ID)
      IF (SYMBOL_REF(SYMBOL_INDEX).EQ.S_BASED) THEN
         IF (SYMBOL_BASE_MEMBER(SYMBOL_INDEX).EQ.0) THEN
            BASE_TYPE=SYMBOL_TYPE(SYMBOL_BASE(SYMBOL_INDEX))
         ELSE
            BASE_TYPE=MEMBER_TYPE(SYMBOL_BASE_MEMBER(SYMBOL_INDEX))
         ENDIF
         BASE=MAKE_ATOM(SYMBOL_BASE(SYMBOL_INDEX),
     #                  SYMBOL_BASE_MEMBER(SYMBOL_INDEX),NULL,NULL,
     #                  BASE_TYPE,0,1)
      ELSE
         BASE=NULL
      ENDIF
      IF (TT.EQ.D_LP) THEN
         IF (SYMBOL_KIND(SYMBOL_INDEX).NE.S_ARRAY) THEN
            IF (MODE.EQ.2) GO TO 10
            CALL ERROR('NOT AN ARRAY: '//SYMBOL_PLM_ID(SYMBOL_INDEX))
         ENDIF
         CALL GETTOK
         CALL PUSH(BASE,1)
         CALL PUSH(REFS,1)
         CALL PUSH(MODE,1)
         CALL PUSH(SYMBOL_INDEX,1)
         SYM_SUBS=EXPRESSION(1)
         CALL POP(SYMBOL_INDEX,1)
         CALL POP(MODE,1)
         CALL POP(REFS,1)
         CALL POP(BASE,1)
         CALL MATCH(D_RP)
      ELSE
         IF (SYMBOL_KIND(SYMBOL_INDEX).EQ.S_ARRAY.AND..NOT.PARTIAL_OK) 
     #      THEN
            CALL ERROR('SUBSCRIPT MISSING AFTER '//
     #                 SYMBOL_PLM_ID(SYMBOL_INDEX))
         ENDIF
10       SYM_SUBS=NULL
      ENDIF
      IF (TT.EQ.D_DOT) THEN
         CALL GETTOK
         CALL MUSTBE(ID)
         CALL LOOKUP_MEMBER
         CALL GETTOK
         IF (TT.EQ.D_LP) THEN
            IF (MEMBER_KIND(MEMBER_INDEX).NE.S_ARRAY) THEN
               IF (MODE.EQ.2) GO TO 20
               CALL ERROR('NOT AN ARRAY: '//MEMBER_PLM_ID(MEMBER_INDEX))
            ENDIF
            CALL GETTOK
            CALL PUSH(BASE,1)
            CALL PUSH(REFS,1)
            CALL PUSH(MODE,1)
            CALL PUSH(SYMBOL_INDEX,1)
            CALL PUSH(MEMBER_INDEX,1)
            CALL PUSH(SYM_SUBS,1)
            MEM_SUBS=EXPRESSION(1)
            CALL POP(SYM_SUBS,1)
            CALL POP(MEMBER_INDEX,1)
            CALL POP(SYMBOL_INDEX,1)
            CALL POP(MODE,1)
            CALL POP(REFS,1)
            CALL POP(BASE,1)
            CALL MATCH(D_RP)
         ELSE
            IF (MEMBER_KIND(MEMBER_INDEX).EQ.S_ARRAY.AND.
     #         .NOT.PARTIAL_OK) THEN
               CALL ERROR('SUBSCRIPT MISSING AFTER '//
     #                    MEMBER_PLM_ID(MEMBER_INDEX))
            ENDIF
20          MEM_SUBS=NULL
         ENDIF
         IF (MEMBER_INDEX.EQ.0) THEN
            TYPE=SYMBOL_TYPE(SYMBOL_INDEX)
         ELSE
            TYPE=MEMBER_TYPE(MEMBER_INDEX)
         ENDIF
      ELSE
         IF (SYMBOL_TYPE(SYMBOL_INDEX).EQ.S_STRUC) THEN
            IF (.NOT.PARTIAL_OK)
     #         CALL ERROR('MEMBER NAME MISSING AFTER '//
     #                 SYMBOL_PLM_ID(SYMBOL_INDEX))
            SIZ=SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX)
            IF (SIZ.EQ.4) THEN
               TYPE=S_LONG
            ELSEIF (SIZ.EQ.2) THEN
               TYPE=S_WORD
            ELSE
               TYPE=S_BYTE
            ENDIF
         ELSE
            TYPE=SYMBOL_TYPE(SYMBOL_INDEX)
         ENDIF
         MEMBER_INDEX=0
         MEM_SUBS=NULL
      ENDIF
      IF (SYM_SUBS.EQ.NULL) THEN
         SUBS1=NULL
      ELSE
         IF (SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX).EQ.
     #           BYTE_SIZE(TYPE)) THEN
            SUBS1=SYM_SUBS
         ELSEIF (MOD(SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX),
     #               BYTE_SIZE(TYPE)).EQ.0) THEN
            SUBS1=MAKE_NODE(OP_MUL,SYM_SUBS,
     #                     MAKE_FIXED2(SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX)
     #                       /BYTE_SIZE(TYPE),0),
     #                      0,0,1)
         ELSE
            SUBSCRIPT=MAKE_NODE(OP_MUL,SYM_SUBS,
     #                 MAKE_FIXED2(SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX),0),
     #                 0,0,0)
            BASE1=MAKE_ATOM(SYMBOL_INDEX,MEMBER_INDEX,BASE,SUBSCRIPT,
     #                      S_BYTE,0,REFS)
            BASE=MAKE_NODE(OP_LOC,BASE1,NULL,0,0,0)
            DATA_REFERENCE=MAKE_ATOM(0,0,BASE,
     #                               MEM_SUBS,TYPE,0,REFS)
            RETURN
         ENDIF
      ENDIF
      IF (MEM_SUBS.EQ.NULL) THEN
         SUBSCRIPT=SUBS1
      ELSEIF (SUBS1.EQ.NULL) THEN
         SUBSCRIPT=MEM_SUBS
      ELSE
         SUBSCRIPT=MAKE_NODE(OP_ADD,SUBS1,MEM_SUBS,0,0,1)
      ENDIF
      DATA_REFERENCE=MAKE_ATOM(SYMBOL_INDEX,MEMBER_INDEX,BASE,
     #                         SUBSCRIPT,TYPE,0,REFS)
      RETURN
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION FUNCTION_REFERENCE(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      PROC_IX=SYMBOL_INDEX
      
      IF (SYMBOL_TYPE(PROC_IX).EQ.0) THEN
         CALL ERROR('UNTYPED PROCEDURE USED AS FUNCTION: '//
     #              IDENTIFIER)
      ENDIF
 
      CALL GETTOK
 
      IF (SYMBOL_REF(PROC_IX).EQ.S_BUILTIN) THEN
         FUNCTION_REFERENCE=BUILTIN_FUNCTION(PROC_IX)
         RETURN
      ENDIF
 
      ARGLIST=NULL
      NARGS=0
 
      IF (TT.EQ.D_LP) THEN
10       CALL GETTOK
         CALL PUSH(PROC_IX,1)
         CALL PUSH(ARGLIST,1)
         CALL PUSH(NARGS,1)
         ARG=EXPRESSION(1)
         CALL POP(NARGS,1)
         CALL POP(ARGLIST,1)
         CALL POP(PROC_IX,1)
         NARGS=NARGS+1
         ARGLIST=MAKE_NODE(OP_ARG,ARGLIST,ARG,0,0,0)
         IF (TT.EQ.D_COMMA) GO TO 10
         CALL MATCH(D_RP)
      ENDIF
 
      IF (NARGS.NE.SYMBOL_LIST_SIZE(PROC_IX)) THEN
         CALL ERROR('WRONG NUMBER OF ARGS TO '//
     #              SYMBOL_PLM_ID(PROC_IX))
      ENDIF
 
      PROC=MAKE_ATOM(PROC_IX,0,NULL,NULL,SYMBOL_TYPE(PROC_IX),0,0)
      FUNCTION_REFERENCE=MAKE_NODE(OP_CALL,PROC,ARGLIST,0,0,0)
 
      CALL DETERMINE_EFFECTS_OF_CALLING(PROC_IX)
      
      RETURN
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION LOCATION_REFERENCE(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 OPERAND,OPERAND1,RESTRICTED_LOCATION_REFERENCE
      LOGICAL*2 CONSTANT_LIST
      CHARACTER*7 DATA_POP(S_BYTE:S_QUAD)
      DATA DATA_POP
     // '.BYTE','.WORD','.WORD','.LONG','.FLOAT','.LONG','.DOUBLE'
     ,, '.QUAD'
     //
      REFS=DREFS
      IF (TT.EQ.D_DOT) THEN
         TYPE=S_LONG
         CALL GETTOK
      ELSE
         CALL MATCH(D_AT)
         TYPE=S_PTR
      ENDIF
      IF (TT.EQ.ID) THEN
         CALL LOOKUP_SYMBOL
         OPND1=DATA_REFERENCE(REFS,.TRUE.)
         IF (ATOM(OPND1) .AND. ATOM_SYM(OPND1).NE.0 .AND.
     #       SYMBOL_KIND(ATOM_SYM(OPND1)).EQ.S_PROC) THEN
            ATOM_FLAGS(OPND1)=ATOM_FLAGS(OPND1).OR.A_VECTOR
         ENDIF
         IF (NODE_TYPE(OPND1).EQ.0) NODE_TYPE(OPND1)=S_BYTE
		! ABOVE IS FOR .<UNTYPED PROCEDURE>
      ELSE
         OLD_PSECT=PSECT(P_CONSTANTS)
         CALL GENERATE_LOCAL_LABEL(LLC)
         CALL EMIT_LOCAL_LABEL(LLC)
         IF (TT.EQ.D_LP) THEN
            CALL GETTOK
            CONSTANT_LIST=.TRUE.
         ELSE
            CONSTANT_LIST=.FALSE.
         ENDIF
10       CONTINUE
         IF (TT.EQ.STRCON) THEN
            CALL EMIT('.ASCII `'//STRING(:STRLEN)//'`')
            CALL GETTOK
         ELSE
            CALL PUSH(CONSTANT_LIST,1)
            CALL PUSH(OLD_PSECT,1)
            CALL PUSH(LLC,1)
            CALL PUSH(TYPE,1)
            CONST=EXPRESSION(0)
            CALL POP(TYPE,1)
            CALL POP(LLC,1)
            CALL POP(OLD_PSECT,1)
            CALL POP(CONSTANT_LIST,1)
            CALL RESOLVE_CONTEXT(CONST)
            IF (NODE_CONTEXT(CONST).EQ.0)
     #         CALL SET_CONTEXT(CONST,CX_UNSIGNED)
            CALL COERCE_TYPES(CONST)
            CONST=FOLD_CONSTANTS(CONST)
            IF (NODE(CONST).AND.OPNODE_OP(CONST).EQ.OP_LOC) THEN
               OPERAND1=RESTRICTED_LOCATION_REFERENCE(CONST,N1)
               CALL EMIT(DATA_POP(NODE_TYPE(CONST))//' '//OPERAND1(:N1))
            ELSEIF (.NOT.LITERAL(CONST)) THEN
               CALL ERROR('CONSTANT LIST ELEMENT NOT A CONSTANT')
            ELSE
               OPERAND1=OPERAND(CONST,N1)
               CALL EMIT(DATA_POP(NODE_TYPE(CONST))//' '//
     #                   OPERAND1(2:N1))
            ENDIF
         ENDIF
         IF (CONSTANT_LIST) THEN
            IF (TT.EQ.D_COMMA) THEN
               CALL GETTOK
               GO TO 10
            ENDIF
            CALL MATCH(D_RP)
         ENDIF
         XX=PSECT(OLD_PSECT)
         OPND1=MAKE_CONSTANT(LLC,S_BYTE)
      ENDIF
      LOCATION_REFERENCE=MAKE_NODE(OP_LOC,OPND1,NULL,0,0,REFS)
      IF (TYPE.EQ.S_LONG) THEN
         LOCATION_REFERENCE=MAKE_NODE(OP_LONG,LOCATION_REFERENCE,
     #                                NULL,0,0,REFS)
      ENDIF
      RETURN
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION EXPRESSION(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      LOGICAL*1 CANT_BE_ASSN
      REFS=DREFS
      CANT_BE_ASSN = TT.EQ.D_LP
      CALL PUSH(REFS,1)
      OPND1=LOGICAL_FACTOR(REFS)
      CALL POP(REFS,1)
      IF (TT.EQ.D_ASSN.AND.ATOM(OPND1).AND..NOT.CANT_BE_ASSN) THEN
         CALL GETTOK
         CALL PUSH(OPND1,1)
         CALL PUSH(REFS,1)
         RHS=LOGICAL_EXPRESSION(REFS)
         CALL POP(REFS,1)
         CALL POP(OPND1,1)
         EXPRESSION=MAKE_NODE(OP_ASSN,RHS,OPND1,0,0,0)
         CALL DETERMINE_EFFECTS_OF_ASSIGNMENT(OPND1)
         RETURN
      ENDIF
10    IF (TT.EQ.K_OR.OR.TT.EQ.K_XOR) THEN
         IF (TT.EQ.K_OR) OP=OP_OR
         IF (TT.EQ.K_XOR) OP=OP_XOR
         CALL GETTOK
         CALL PUSH(OP,1)
         CALL PUSH(REFS,1)
         CALL PUSH(OPND1,1)
         OPND2=LOGICAL_FACTOR(REFS)
         CALL POP(OPND1,1)
         CALL POP(REFS,1)
         CALL POP(OP,1)
         OPND1=MAKE_NODE(OP,OPND1,OPND2,S_BYTE,0,REFS)
      ELSE
         EXPRESSION=OPND1
         RETURN
      ENDIF
      GO TO 10
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION LOGICAL_EXPRESSION(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      CALL PUSH(REFS,1)
      OPND1=LOGICAL_FACTOR(REFS)
      CALL POP(REFS,1)
10    IF (TT.EQ.K_OR.OR.TT.EQ.K_XOR) THEN
         IF (TT.EQ.K_OR) OP=OP_OR
         IF (TT.EQ.K_XOR) OP=OP_XOR
         CALL GETTOK
         CALL PUSH(OP,1)
         CALL PUSH(REFS,1)
         CALL PUSH(OPND1,1)
         OPND2=LOGICAL_FACTOR(REFS)
         CALL POP(OPND1,1)
         CALL POP(REFS,1)
         CALL POP(OP,1)
         OPND1=MAKE_NODE(OP,OPND1,OPND2,S_BYTE,0,REFS)
      ELSE
         LOGICAL_EXPRESSION=OPND1
         RETURN
      ENDIF
      GO TO 10
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION LOGICAL_FACTOR(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      CALL PUSH(REFS,1)
      OPND1=LOGICAL_SECONDARY(REFS)
      CALL POP(REFS,1)
10    IF (TT.EQ.K_AND) THEN
         CALL GETTOK
         CALL PUSH(REFS,1)
         CALL PUSH(OPND1,1)
         OPND2=LOGICAL_SECONDARY(REFS)
         CALL POP(OPND1,1)
         CALL POP(REFS,1)
         OPND1=MAKE_NODE(OP_AND,OPND1,OPND2,0,0,REFS)
      ELSE
         LOGICAL_FACTOR=OPND1
         RETURN
      ENDIF
      GO TO 10
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION LOGICAL_SECONDARY(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      IF (TT.EQ.K_NOT) THEN
         CALL GETTOK
         CALL PUSH(REFS,1)
         OPND1=LOGICAL_PRIMARY(REFS)
         CALL POP(REFS,1)
         LOGICAL_SECONDARY=MAKE_NODE(OP_NOT,OPND1,NULL,0,0,REFS)
      ELSE
         LOGICAL_SECONDARY=LOGICAL_PRIMARY(REFS)
      ENDIF
      RETURN
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION LOGICAL_PRIMARY(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      CALL PUSH(REFS,1)
      OPND1=ARITHMETIC_EXPRESSION(REFS)
      CALL POP(REFS,1)
      IF (TT.GE.D_LT.AND.TT.LE.D_GE) THEN
         OP=TT-D_LT+OP_LT
         CALL GETTOK
         CALL PUSH(OP,1)
         CALL PUSH(REFS,1)
         CALL PUSH(OPND1,1)
         OPND2=ARITHMETIC_EXPRESSION(REFS)
         CALL POP(OPND1,1)
         CALL POP(REFS,1)
         CALL POP(OP,1)
         OPND1=MAKE_NODE(OP,OPND1,OPND2,0,0,REFS)
      ENDIF
      LOGICAL_PRIMARY=OPND1
      RETURN
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION ARITHMETIC_EXPRESSION(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      CALL PUSH(REFS,1)
      OPND1=TERM(REFS)
      CALL POP(REFS,1)
10    IF (TT.EQ.D_PLUS.OR.TT.EQ.D_MINUS.OR.TT.EQ.K_PLUS.OR.
     #    TT.EQ.K_MINUS) THEN
         IF (TT.EQ.D_PLUS) THEN
            OP=OP_ADD
         ELSEIF (TT.EQ.D_MINUS) THEN
            OP=OP_SUB
         ELSEIF (TT.EQ.K_PLUS) THEN
            OP=OP_ADWC
            CALL WARN('PLUS PROBABLY WON''T WORK')
         ELSE
            OP=OP_SBWC
            CALL WARN('MINUS PROBABLY WON''T WORK')
         ENDIF
         CALL GETTOK
         CALL PUSH(OP,1)
         CALL PUSH(REFS,1)
         CALL PUSH(OPND1,1)
         OPND2=TERM(REFS)
         CALL POP(OPND1,1)
         CALL POP(REFS,1)
         CALL POP(OP,1)
         OPND1=MAKE_NODE(OP,OPND1,OPND2,0,0,REFS)
      ELSE
         ARITHMETIC_EXPRESSION=OPND1
         RETURN
      ENDIF
      GO TO 10
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION TERM(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      CALL PUSH(REFS,1)
      OPND1=SECONDARY(REFS)
      CALL POP(REFS,1)
10    IF (TT.EQ.D_STAR.OR.TT.EQ.D_SLASH.OR.TT.EQ.K_MOD) THEN
         IF (TT.EQ.D_STAR) OP=OP_MUL
         IF (TT.EQ.D_SLASH) OP=OP_DIV
         IF (TT.EQ.K_MOD) OP=OP_MOD
         CALL GETTOK
         CALL PUSH(OP,1)
         CALL PUSH(REFS,1)
         CALL PUSH(OPND1,1)
         OPND2=SECONDARY(REFS)
         CALL POP(OPND1,1)
         CALL POP(REFS,1)
         CALL POP(OP,1)
         OPND1=MAKE_NODE(OP,OPND1,OPND2,0,0,REFS)
      ELSE
         TERM=OPND1
         RETURN
      ENDIF
      GO TO 10
      END
C-------------------------------------------------
      INTEGER*2 FUNCTION SECONDARY(DREFS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REFS=DREFS
      IF (TT.EQ.D_MINUS) THEN
         CALL GETTOK
         CALL PUSH(REFS,1)
         OPND1=PRIMARY(REFS)
         CALL POP(REFS,1)
         SECONDARY=MAKE_NODE(OP_NEG,OPND1,NULL,0,0,REFS)
      ELSE
         IF (TT.EQ.D_PLUS) CALL GETTOK
         SECONDARY=PRIMARY(REFS)
      ENDIF
      RETURN
      END
C-------------------------------------------------
      CHARACTER*80 FUNCTION RESTRICTED_LOCATION_REFERENCE(NOD,N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 OPERAND
 
      ATM=OPNODE_OPND1(NOD)
 
      IF (.NOT.ATOM(ATM).OR.ATOM_BASE(ATM).NE.NULL.OR.
     #    ATOM_SUB(ATM).NE.NULL.OR.
     #    (SYMBOL_REF(ATOM_SYM(ATM)).NE.S_STATIC.AND.
     #     SYMBOL_REF(ATOM_SYM(ATM)).NE.S_LOCAL.AND.
     #     SYMBOL_REF(ATOM_SYM(ATM)).NE.S_FORWARD.AND.
     #     SYMBOL_REF(ATOM_SYM(ATM)).NE.S_EXT))  THEN
 
         CALL ERROR('NOT A RESTRICTED LOCATION REFERENCE')
      ENDIF
 
      ATOM_FLAGS(ATM)=ATOM_FLAGS(ATM).OR.A_IMMEDIATE
 
      RESTRICTED_LOCATION_REFERENCE=OPERAND(ATM,N)
 
      RESTRICTED_LOCATION_REFERENCE=RESTRICTED_LOCATION_REFERENCE(2:N)
      N=N-1
 
      RETURN
      END
