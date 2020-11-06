C***********************************************************************
C
C                               DECLS.FOR
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
C    This module of the PL/M-VAX compiler processes declarations at
C    the beginning of a procedure or block.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    13SEP81  Alex Hunter  1. Implement ALIGN control. (V5.2)
C    29SEP81  Alex Hunter  1. Change call to INITIALIZATION. (V5.3)
C			   2. Reduce macro body size by 1.
C			   3. Allow dimensions >64K.
C			   4. Allow structure member arrays to have
C			      explicit lower bounds.
C    21OCT81  Alex Hunter  1. Set S_OVERLAID attribute properly. (V5.5)
C    28OCT81  Alex Hunter  1. Allow keywords to be re-declared. (V5.7)
C    12NOV81  Alex Hunter  1. Implement psect numbers. (V6.1)
C			   2. Allow PUBLIC AT(.MEMORY).
C			   3. Allow AT(arg) and AT(dynamic).
C			   4. Allow structure array to be implicitly
C			      dimensioned.
C    14NOV81  Alex Hunter  1. Add this_psect arg to INITIALIZATION.
C			      (V6.2)
C    14JAN82  Alex Hunter  1. Fix minor bug from V5.7. (V6.5)
C
C***********************************************************************
C   ---  Compile me with /NOCHECK please!!
 
      SUBROUTINE DECLARATIONS
      INCLUDE 'PLMCOM.FOR/NOLIST'
10    IF (TT.EQ.K_DECLARE) THEN
         CALL DECLARE_STATEMENT
      ELSEIF (TT.EQ.K_PROCEDURE) THEN
         CALL PROCEDURE_DEFINITION
      ELSEIF (TT.EQ.K_COMMON) THEN
         CALL COMMON_STATEMENT
      ELSE
         RETURN
      ENDIF
      GO TO 10
      END
C----------------------------------------------------
      SUBROUTINE DECLARE_STATEMENT
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CALL MATCH(K_DECLARE)
10    CALL DECLARE_ELEMENT(P_DATA)
      IF (TT.EQ.D_COMMA) THEN
         CALL GETTOK
         GO TO 10
      ENDIF
      CALL MATCH(D_SEMI)
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE COMMON_STATEMENT
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 COMMON_NAME
 
      CALL MATCH(K_COMMON)
      COMMON_NAME='.BLANK.'
 
      IF (TT.EQ.D_SLASH) THEN
         CALL GETTOK
         IF (TT.NE.D_SLASH) THEN
            CALL MUSTBE(ID)
            COMMON_NAME=IDENTIFIER
            CALL GETTOK
         ENDIF
         CALL MATCH(D_SLASH)
      ENDIF
 
      COMMON_PSECT=SETUP_COMMON_PSECT(COMMON_NAME)
 
10    CALL DECLARE_ELEMENT(COMMON_PSECT)
      IF (TT.EQ.D_COMMA) THEN
         CALL GETTOK
         GO TO 10
      ENDIF
 
      CALL MATCH(D_SEMI)
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE DECLARE_ELEMENT(DEFAULT_PSECT)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      LOGICAL*4 FACTORED_LIST
      INTEGER*2 KIND,TYPE,
     #          LINK,LIST_SIZE,REF,BASE,BASE_MEMBER,FLAGS
      INTEGER*2 INDEX(32),REFX(32),BASEX(32),BASE_MEMBERX(32)
      COMMON/DECLS/KIND,TYPE,NBR_ELEMENTS,ELEMENT_SIZE,
     #          LINK,LIST_SIZE,REF,BASE,BASE_MEMBER,FLAGS
      LOGICAL*2 NO_MORE_DATA,STRINGLEFT
      INTEGER*4 BLOCK_SIZE,NBR_ELEMENTS,LOWER_BOUND,IFSD,ELEMENT_SIZE
      COMMON /INITDATA/ NO_MORE_DATA,STRINGLEFT,BLOCK_SIZE
      COMMON /AT_FLAG/ AT,ATM
      CHARACTER*10 STRING10
      CHARACTER*80 OPERAND,OPERAND1
      CHARACTER*32 PUBLIQUE
      CHARACTER*4 ALIGNMENT(1:8)
      DATA ALIGNMENT 
     # /'BYTE','WORD','----','LONG','----','----','----','LONG'/
C
 
      FLAGS=0
      N=0
      REF=0
      THIS_PSECT=DEFAULT_PSECT
      IF (TT.EQ.D_LP) THEN
         CALL GETTOK
         FACTORED_LIST=.TRUE.
      ELSE
         FACTORED_LIST=.FALSE.
      ENDIF
10    CONTINUE
         IF (TT.LT.101.OR.TT.GT.199) CALL MUSTBE(ID)
         IF (N.GE.32) THEN
            CALL ERROR('TOO MANY ELEMENTS IN FACTORED LIST')
         ELSE
            CALL ENTER_SYMBOL
            IF (SYMBOL_REF(SYMBOL_INDEX).EQ.S_ARG) 
     #         SYMBOL_FLAGS(SYMBOL_INDEX)=0
            N=N+1
            INDEX(N)=SYMBOL_INDEX
         ENDIF
         CALL GETTOK
         IF (TT.EQ.K_BASED) THEN
            CALL GETTOK
            CALL SIMPLE_VARIABLE(BTYPE)
            REFX(N)=S_BASED
            BASEX(N)=SYMBOL_INDEX
            BASE_MEMBERX(N)=MEMBER_INDEX
            IF (MEMBER_INDEX.EQ.0) THEN
               IF (BTYPE.NE.S_WORD.AND.BTYPE.NE.S_PTR.AND.
     #             BTYPE.NE.S_LONG) THEN
                  CALL ERROR('BASE SPECIFIER NOT LONG/ADDRESS/POINTER: '
     #               //SYMBOL_PLM_ID(SYMBOL_INDEX))
               ENDIF
            ELSE
               IF (BTYPE.NE.S_WORD.AND.BTYPE.NE.S_PTR.AND.
     #             BTYPE.NE.S_LONG) THEN
                  CALL ERROR('BASE SPECIFIER NOT LONG/ADDRESS/POINTER: '
     #             //SYMBOL_PLM_ID(SYMBOL_INDEX)//'.'//
     #               MEMBER_PLM_ID(MEMBER_INDEX))
               ENDIF
            ENDIF
         ELSE
            REFX(N)=S_STATIC
            BASEX(N)=0
            BASE_MEMBERX(N)=0
         ENDIF
      IF (FACTORED_LIST) THEN
         IF (TT.EQ.D_COMMA) THEN
            CALL GETTOK
            GO TO 10
         ENDIF
         CALL MATCH(D_RP)
      ENDIF
      LINK=0
      NO_MORE_DATA=.TRUE.
      IF (TT.EQ.K_LITERALLY) THEN
         CALL GETTOK
         CALL MUSTBE(STRCON)
CCCC     STRLEN=STRLEN+1        ! Is this necessary?
         S_TOP=STRINGS_TOP(BLOCK_LEVEL)
         IF (S_TOP+STRLEN.GT.STRINGS_MAX)
     #      CALL FATAL('STRING SPACE EXHAUSTED')
         STRINGS(S_TOP+1:S_TOP+STRLEN)=STRING
         STRINGS_TOP(BLOCK_LEVEL)=S_TOP+STRLEN
         KIND=S_MACRO
         TYPE=0
         NBR_ELEMENTS=0
         ELEMENT_SIZE=STRLEN
         LINK=S_TOP+1
         LIST_SIZE=0
         DO J=1,N
            IF (BASEX(J).NE.0) THEN
               CALL ERROR('LITERAL CANNOT BE BASED: '//
     #            SYMBOL_PLM_ID(INDEX(J)))
            ENDIF
            REFX(J)=0
            BASEX(J)=0
            BASE_MEMBERX(J)=0
         ENDDO
CC---    CALL GETTOK    -- DONE LATER, CAUSE NEXT TOKEN MIGHT BE THIS
CC                         MACR0!!
      ELSEIF (TT.EQ.K_LABEL) THEN
         CALL GETTOK
         REF=S_FORWARD
         IF (TT.EQ.K_PUBLIC) THEN
            FLAGS=FLAGS.OR.S_PUBLIC
            CALL GETTOK
         ELSEIF (TT.EQ.K_EXTERNAL) THEN
            REF=S_EXT
            CALL GETTOK
         ENDIF
         IF (REF.NE.S_EXT) THEN
            FLAGS=FLAGS.OR.S_UNDEF
         ENDIF
         KIND=S_LABEL
         TYPE=0
         NBR_ELEMENTS=0
         ELEMENT_SIZE=0
         LINK=0
         LIST_SIZE=0
         DO J=1,N
            IF (BASEX(J).NE.0) THEN
               CALL ERROR('LABEL CANNOT BE BASED: '//
     #            SYMBOL_PLM_ID(INDEX(J)))
            ENDIF
            REFX(J)=S_STATIC
            BASEX(J)=0
            BASE_MEMBERX(J)=0
         ENDDO
      ELSE
         IF (TT.EQ.D_LP) THEN
            KIND=S_ARRAY
            CALL DIMENSION(NBR_ELEMENTS,LOWER_BOUND)
         ELSE
            NBR_ELEMENTS=1
            LOWER_BOUND=0
            KIND=S_SCALAR
         ENDIF
         CALL VARIABLE_TYPE
         CALL VARIABLE_ATTRIBUTES(FLAGS,REF,THIS_PSECT)
         CALL INITIALIZATION(REF,THIS_PSECT)
         IF (NBR_ELEMENTS.EQ.-1.AND.N.NE.1) THEN
            CALL ERROR('INVALID USE OF IMPLICIT DIMENSION')
            NBR_ELEMENTS=0
         ENDIF
      ENDIF
C
C---- ASSIGN ATTRIBUTES TO THE SYMBOLS.
C
      DO 700 J=1,N
         I=INDEX(J)
         IF (REF.EQ.S_EXT) THEN
            SYMBOL_VAX_ID(I)=PUBLIQUE(SYMBOL_PLM_ID(I))
            IF (SAME_OVERLAY) FLAGS=FLAGS.OR.S_SAME_OVERLAY
         ENDIF
         SYMBOL_KIND(I)=KIND
         SYMBOL_TYPE(I)=TYPE
         SYMBOL_ELEMENT_SIZE(I)=ELEMENT_SIZE
         SYMBOL_LINK(I)=LINK
         SYMBOL_LIST_SIZE(I)=LIST_SIZE
         SYMBOL_PSECT(I)=THIS_PSECT
         IF ((REF.EQ.S_EXT.OR.(FLAGS.AND.S_PUBLIC).NE.0) .AND.
     #        REFX(J).NE.S_STATIC) THEN
            CALL ERROR('EXTERNAL/PUBLIC VARIABLE MUST BE STATIC: '//
     #                  SYMBOL_PLM_ID(I))
         ENDIF
         IF (AT.NE.0.AND.REFX(J).EQ.S_BASED) THEN
            CALL ERROR('BASED VARIABLE CANNOT HAVE AT-ATTRIBUTE: '//
     #                 SYMBOL_PLM_ID(I))
         ENDIF
         IF (REF.EQ.0) THEN
            REF1=REFX(J)
         ELSE
            REF1=REF
         ENDIF
         IF (REF1.EQ.S_ARG) THEN
            IF (NBR_ELEMENTS*ELEMENT_SIZE.GT.4) THEN
               CALL WARN('DUBIOUS ARGUMENT OVERLAY: '//
     #                   SYMBOL_PLM_ID(I))
            ENDIF
         ELSEIF (SYMBOL_REF(I).EQ.S_ARG) THEN
            IF (KIND.NE.S_SCALAR.OR.TYPE.EQ.S_STRUC.OR.
     #          BYTE_SIZE(TYPE).GT.4.OR.REF1.NE.S_STATIC.OR.
     #          THIS_PSECT.NE.P_DATA) THEN
               CALL ERROR('ILLEGAL DECLARATION FOR FORMAL PARAMETER: '//
     #                     SYMBOL_PLM_ID(I))
            ENDIF
            REF1=S_ARG
            SYMBOL_LINK(I)=PROC_LEVEL
         ELSE
            IF ((PROC_FLAGS(PROC_LEVEL).AND.
     #         (PROC_EXT.OR.PROC_FORWARD)).NE.0) THEN
               CALL ERROR('LOCAL DECLARATION NOT ALLOWED IN EXTERNAL'//
     #                    '/FORWARD  PROCEDURE: '//SYMBOL_PLM_ID(I))
            ENDIF
            IF ((PROC_FLAGS(PROC_LEVEL).AND.PROC_REENT).NE.0.AND.
     #         REF1.EQ.S_STATIC.AND.THIS_PSECT.EQ.P_DATA) THEN
               REF1=S_DYNAMIC
            ENDIF
         ENDIF
         SYMBOL_REF(I)=REF1
         SYMBOL_BASE(I)=BASEX(J)
         SYMBOL_BASE_MEMBER(I)=BASE_MEMBERX(J)
         SYMBOL_FLAGS(I)=FLAGS
700   CONTINUE
C
C---- SET PSECT AND PERFORM ALIGNMENT IF REQUIRED.
C
      CALL PSECT(THIS_PSECT)
      IF (AT.NE.0.AND.SYMBOL_REF(ATOM_SYM(ATM)).EQ.S_STATIC) THEN
         OPERAND1=OPERAND(ATM,N1)
         CALL EMIT1('PC.SAVE = .')
         CALL EMIT1('. = '//OPERAND1(2:N1))
      ENDIF
      IF (ALIGN_FLAG .AND. AT.EQ.0 .AND. 
     #    BYTE_SIZE(TYPE).GT.1 .AND. TYPE.NE.S_STRUC) THEN
         DO J=1,N
            IF (SYMBOL_REF(INDEX(J)).EQ.S_STATIC.AND.
     #          THIS_PSECT.EQ.P_DATA) THEN
               CALL EMIT('.ALIGN  '//ALIGNMENT(BYTE_SIZE(TYPE)))
               GO TO 801
            ENDIF
         ENDDO
801      CONTINUE
      ENDIF
C
C---- DEFINE SYMBOLS WITH POSSIBLE INITIAL VALUES.
C
      OFFSET=0
      DO 910 J=1,N
         I=INDEX(J)
         REF1=SYMBOL_REF(I)
         IF (.NOT.NO_MORE_DATA.AND.REF1.NE.S_STATIC) THEN
            CALL ERROR('ATTEMPT TO INITIALIZE NON-STATIC VARIABLE: '
     #                 //SYMBOL_PLM_ID(I))
         ENDIF
         IF (REF1.EQ.S_STATIC) THEN
            CALL EMIT_RELDEF4(I,'.',-LOWER_BOUND*ELEMENT_SIZE)
            BLOCK_SIZE=0
 
            IF (NBR_ELEMENTS.EQ.-1) THEN	! IMPLICIT DIMENSION.
               NBR_ELEMENTS=0
               IF (NO_MORE_DATA) THEN
                  CALL ERROR(
     #             'IMPLICIT DIMENSION WITHOUT INITIALIZATION LIST')
               ELSEIF (TYPE.EQ.S_STRUC) THEN
901               DO M=LINK,LINK+LIST_SIZE-1
                     DO M1=1,MEMBER_NBR_ELEMENTS(M)
                        CALL INITIAL_DATA(MEMBER_TYPE(M))
                     ENDDO
                  ENDDO
                  NBR_ELEMENTS=NBR_ELEMENTS+1
                  IF (.NOT.NO_MORE_DATA) GO TO 901
               ELSE
902               CALL INITIAL_DATA(TYPE)
                  NBR_ELEMENTS=NBR_ELEMENTS+1
                  IF (.NOT.NO_MORE_DATA) GO TO 902
               ENDIF
          
            ELSEIF (NO_MORE_DATA) THEN		! NO INITIALIZATION.
               BLOCK_SIZE=NBR_ELEMENTS*ELEMENT_SIZE
 
            ELSE				! PROCESS INITIAL/DATA.
 
               DO K=1,NBR_ELEMENTS
                  IF (TYPE.EQ.S_STRUC) THEN
                     DO M=LINK,LINK+LIST_SIZE-1
                        DO M1=1,MEMBER_NBR_ELEMENTS(M)
                           CALL INITIAL_DATA(MEMBER_TYPE(M))
                        ENDDO
                     ENDDO
                  ELSE
                     CALL INITIAL_DATA(TYPE)
                  ENDIF
               ENDDO
               CALL FLUSH_ASCII
            ENDIF
 
            IF (BLOCK_SIZE.NE.0) THEN
               OPERAND1=STRING10(BLOCK_SIZE,IFSD)
               CALL EMIT('.BLKB  '//OPERAND1(IFSD:10))
            ENDIF
 
         ELSEIF (REF1.EQ.S_BASED) THEN
            CALL EMIT_ABSDEF4(SYMBOL_VAX_ID(I),
     #                         -LOWER_BOUND*ELEMENT_SIZE)
         ELSEIF (AT.NE.0) THEN
            SYMBOL_VAX_ID(I)=SYMBOL_VAX_ID(ATOM_SYM(ATM))
            SYMBOL_FLAGS(I)=SYMBOL_FLAGS(ATOM_SYM(ATM)).OR.S_NOTPUBLIC
            SYMBOL_DISP(I)=SYMBOL_DISP(I)+SYMBOL_DISP(ATOM_SYM(ATM))+
     #                     ATOM_DISP(ATM)+OFFSET
            IF (ATOM_MEM(ATM).NE.0) THEN
               SYMBOL_DISP(I)=SYMBOL_DISP(I)+
     #                        MEMBER_OFFSET(ATOM_MEM(ATM))
            ENDIF
            IF (REF1.EQ.S_ARG) THEN
               SYMBOL_LINK(I)=SYMBOL_LINK(ATOM_SYM(ATM))
            ENDIF
            OFFSET=OFFSET+NBR_ELEMENTS*ELEMENT_SIZE
 
         ELSEIF (REF1.EQ.S_DYNAMIC) THEN
            CALL EMIT_ABSDEF4(SYMBOL_VAX_ID(I),
     #                PROC_DYN_OFF(PROC_LEVEL)-LOWER_BOUND*ELEMENT_SIZE)
            PROC_DYN_OFF(PROC_LEVEL)=PROC_DYN_OFF(PROC_LEVEL)+
     #                               NBR_ELEMENTS*ELEMENT_SIZE
         ENDIF
         SYMBOL_NBR_ELEMENTS(I)=NBR_ELEMENTS
         SYMBOL_LOWER_BOUND(I)=LOWER_BOUND
910   CONTINUE
      CALL POST_INITIALIZATION
      IF (AT.NE.0.AND.SYMBOL_REF(ATOM_SYM(ATM)).EQ.S_STATIC) THEN
         CALL EMIT1('. = PC.SAVE')
      ENDIF
      IF (KIND.EQ.S_MACRO) CALL GETTOK   ! WE PROMISED WE WOULD!
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE SIMPLE_VARIABLE(TYPE)
      INCLUDE 'PLMCOM.FOR/NOLIST'
C
      CALL MUSTBE(ID)
      CALL LOOKUP_SYMBOL
      IF (SYMBOL_KIND(SYMBOL_INDEX).NE.S_SCALAR.OR.
     #    SYMBOL_REF(SYMBOL_INDEX).EQ.S_BASED) THEN
         CALL ERROR('NOT A SIMPLE VARIABLE: '//
     #               SYMBOL_PLM_ID(SYMBOL_INDEX))
      ENDIF
      CALL GETTOK
      IF (TT.EQ.D_DOT) THEN
         CALL GETTOK
         CALL MUSTBE(ID)
         CALL LOOKUP_MEMBER
         IF (MEMBER_KIND(MEMBER_INDEX).NE.S_SCALAR) THEN
            CALL ERROR('NOT A SIMPLE VARIABLE: '//
     #            SYMBOL_PLM_ID(SYMBOL_INDEX)//'.'//
     #            MEMBER_PLM_ID(MEMBER_INDEX))
         ENDIF
         CALL GETTOK
      ELSE
         MEMBER_INDEX=0
         IF (SYMBOL_TYPE(SYMBOL_INDEX).EQ.S_STRUC) THEN
            CALL ERROR('NOT A FULLY QUALIFIED REFERENCE: '//
     #                  SYMBOL_PLM_ID(SYMBOL_INDEX))
         ENDIF
      ENDIF
      IF (MEMBER_INDEX.EQ.0) THEN
         TYPE = SYMBOL_TYPE(SYMBOL_INDEX)
      ELSE
         TYPE = MEMBER_TYPE(MEMBER_INDEX)
      ENDIF
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE VARIABLE_ATTRIBUTES(FLAGS,REF,THIS_PSECT)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 OFF
      COMMON /AT_FLAG/ AT,ATM
      AT=0				! ASSUME NO AT-ATTRIBUTE.
      IF (TT.EQ.K_EXTERNAL) THEN
         REF = S_EXT
         CALL GETTOK
      ELSE
         IF (TT.EQ.K_PUBLIC) THEN
            FLAGS = FLAGS.OR.S_PUBLIC
            CALL GETTOK
         ENDIF
         IF (TT.EQ.K_AT) THEN
            CALL GETTOK
            CALL MATCH(D_LP)
            FLAGS = FLAGS .OR. S_OVERLAID
            CALL BREAK
            AT=MASSAGE(EXPRESSION(0),CX_UNSIGNED)
            IF (NODE(AT).AND.OPNODE_OP(AT).EQ.OP_LOC) THEN
               ATM=OPNODE_OPND1(AT)
               IF (.NOT.ATOM(ATM).OR.ATOM_BASE(ATM).NE.NULL.OR.
     #             ATOM_SUB(ATM).NE.NULL) THEN
                  CALL ERROR('NOT A RESTRICTED LOCATION REFERENCE')
                  AT=0
               ENDIF
               SYMBOL_FLAGS(ATOM_SYM(ATM))=SYMBOL_FLAGS(ATOM_SYM(ATM))
     #                                     .OR. S_OVERLAID
               ATOM_FLAGS(ATM)=ATOM_FLAGS(ATM).AND..NOT.(A_P2L+A_L2P)
     #                           .OR. A_CTIM ! USE COMPILE-TIME ADDR.
     #                           .OR. A_IMMEDIATE 
               IF (SYMBOL_REF(ATOM_SYM(ATM)).EQ.S_EXT) THEN
                  IF ((FLAGS.AND.S_PUBLIC).NE.0) THEN
                     CALL ERROR('PUBLIC ATTRIBUTE CONFLICTS WITH '//
     #                          'AT-EXTERNAL')
                  ENDIF
               ELSEIF (SYMBOL_REF(ATOM_SYM(ATM)).NE.S_STATIC.AND.
     #                 SYMBOL_REF(ATOM_SYM(ATM)).NE.S_ARG.AND.
     #                 SYMBOL_REF(ATOM_SYM(ATM)).NE.S_DYNAMIC) THEN
                  CALL ERROR('NOT A RESTRICTED LOCATION REFERENCE')
                  AT=0
               ENDIF
            ELSE
               CALL ERROR('AT MUST BE LOCATION REFERENCE')
               AT=0
            ENDIF
            CALL MATCH(D_RP)
         ENDIF
      ENDIF
      IF (AT.NE.0) THEN
         REF=SYMBOL_REF(ATOM_SYM(ATM))
         THIS_PSECT=SYMBOL_PSECT(ATOM_SYM(ATM))
      ENDIF
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE DIMENSION(NBR_ELEMENTS,LOWER_BOUND)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 NBR_ELEMENTS,LOWER_BOUND
 
      CALL MATCH(D_LP)
      LOWER_BOUND=0
      IF (TT.EQ.D_STAR) THEN
         NBR_ELEMENTS=-1
         CALL GETTOK
      ELSE
         CALL BREAK
         N1=EXPRESSION(0)
         CALL RESOLVE_CONTEXT(N1)
         IF (NODE_CONTEXT(N1).EQ.0) CALL SET_CONTEXT(N1,CX_SIGNED)
         CALL COERCE_TYPES(N1)
         N1=FORCE_TYPE(N1,S_LONG)
         N1=FOLD_CONSTANTS(N1)
         IF (FIXLIT(N1)) THEN
            NBR_ELEMENTS=FIXED_VAL(N1)
         ELSE
            CALL ERROR('ARRAY DIMENSION NOT A CONSTANT')
            NBR_ELEMENTS=0
         ENDIF
         IF (TT.EQ.D_COLON) THEN
            CALL GETTOK
            LOWER_BOUND=NBR_ELEMENTS
            N2=EXPRESSION(0)
            CALL RESOLVE_CONTEXT(N2)
            IF (NODE_CONTEXT(N2).EQ.0) CALL SET_CONTEXT(N2,CX_SIGNED)
            CALL COERCE_TYPES(N2)
            N2=FORCE_TYPE(N2,S_LONG)
            N2=FOLD_CONSTANTS(N2)
            IF (FIXLIT(N2)) THEN
               NBR_ELEMENTS=FIXED_VAL(N2)-LOWER_BOUND+1
            ELSE
               CALL ERROR('UPPER BOUND NOT A CONSTANT')
               NBR_ELEMENTS=0
            ENDIF
         ENDIF
         IF (NBR_ELEMENTS.LT.0) THEN
            CALL ERROR('ARRAY SIZE IS NEGATIVE')
            NBR_ELEMENTS=0
         ENDIF
      ENDIF
      CALL MATCH(D_RP)
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE VARIABLE_TYPE
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 NBR_ELEMENTS,ELEMENT_SIZE
      COMMON/DECLS/KIND,TYPE,NBR_ELEMENTS,ELEMENT_SIZE,
     #          LINK,LIST_SIZE,REF,BASE,BASE_MEMBER,FLAGS
      IF (TT.EQ.K_STRUCTURE) THEN
         CALL STRUCTURE_TYPE
      ELSE
         CALL BASIC_TYPE(TYPE)
         ELEMENT_SIZE = BYTE_SIZE(TYPE)
      ENDIF
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE BASIC_TYPE(TYPE)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      IF (TT.EQ.K_INTEGER) THEN
         TYPE = S_INTEGER
         CALL GETTOK
      ELSEIF (TT.EQ.K_REAL) THEN
         TYPE = S_REAL
         CALL GETTOK
      ELSEIF (TT.EQ.K_POINTER) THEN
         TYPE = S_PTR
         CALL GETTOK
      ELSEIF (TT.EQ.K_WORD.OR.TT.EQ.K_ADDRESS) THEN
         TYPE = S_WORD
         CALL GETTOK
      ELSEIF (TT.EQ.K_BYTE) THEN
         TYPE = S_BYTE
         CALL GETTOK
      ELSEIF (TT.EQ.K_LONG) THEN
         TYPE = S_LONG
         CALL GETTOK
      ELSEIF (TT.EQ.K_DOUBLE) THEN
          TYPE = S_DOUBLE
          CALL GETTOK
      ELSEIF (TT.EQ.K_QUAD) THEN
          TYPE = S_QUAD
          CALL GETTOK
      ELSE
         CALL MUSTBE(NT_TYPE)
      ENDIF
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE STRUCTURE_TYPE
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 NBR_ELEMENTS,ELEMENT_SIZE,OFF
      COMMON/DECLS/KIND,TYPE,NBR_ELEMENTS,ELEMENT_SIZE,
     #          LINK,LIST_SIZE,REF,BASE,BASE_MEMBER,FLAGS
      CALL MATCH(K_STRUCTURE)
      TYPE = S_STRUC
      LINK = MEMBER_TOP(BLOCK_LEVEL)+1
      LIST_SIZE = 0
      OFF = 0
      CALL MATCH(D_LP)
10    CALL MEMBER_ELEMENT(OFF,N)
      LIST_SIZE = LIST_SIZE+N
      IF (TT.EQ.D_COMMA) THEN
         CALL GETTOK
         GO TO 10
      ENDIF
      CALL MATCH(D_RP)
      ELEMENT_SIZE = OFF
      RETURN
      END
C----------------------------------------------------
      SUBROUTINE MEMBER_ELEMENT(OFF,N)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*2 INDEX(32)
      INTEGER*4 MNBR,LB,OFF
C
      N=0
      IF (TT.EQ.D_LP) THEN
10       CALL GETTOK
         CALL MUSTBE(ID)
         IF (N.GE.32) THEN
            CALL ERROR('TOO MANY ELEMENTS IN FACTORED LIST')
         ELSE
            CALL ENTER_MEMBER
            N=N+1
            INDEX(N)=MEMBER_INDEX
         ENDIF
         CALL GETTOK
         IF (TT.EQ.D_COMMA) GO TO 10
         CALL MATCH(D_RP)
      ELSE
         CALL ENTER_MEMBER
         N=1
         INDEX(N)=MEMBER_INDEX
         CALL GETTOK
      ENDIF
      IF (TT.EQ.D_LP) THEN
         MKIND = S_ARRAY
         CALL DIMENSION(MNBR,LB)
         IF (MNBR.EQ.-1) THEN
            CALL ERROR('IMPLICIT DIMENSION NOT ALLOWED FOR MEMBER')
            MNBR = 0
         ENDIF
      ELSE
         MKIND = S_SCALAR
         MNBR = 1
         LB=0
      ENDIF
      CALL BASIC_TYPE(MTYPE)
      DO J=1,N
         I = INDEX(J)
         MEMBER_KIND(I) = MKIND
         MEMBER_TYPE(I) = MTYPE
         MEMBER_NBR_ELEMENTS(I) = MNBR
         MEMBER_LOWER_BOUND(I) = LB
         MEMBER_ELEMENT_SIZE(I) = BYTE_SIZE(MTYPE)
         MEMBER_OFFSET(I) = OFF-LB*MEMBER_ELEMENT_SIZE(I)
         CALL EMIT_ABSDEF4(MEMBER_VAX_ID(I),MEMBER_OFFSET(I))
         OFF = OFF+MEMBER_ELEMENT_SIZE(I)*MNBR
      ENDDO
      RETURN
      END
