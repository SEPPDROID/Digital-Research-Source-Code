C***********************************************************************
C
C                               BUILTINS.FOR
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
C    This module of the PL/M-VAX compiler handles those built-in
C    functions which potentially generate in-line code.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. Implement the FIRST function. (V5.3)
C			   2. Allow LENGTH,FIRST,LAST,SIZE to be >64K.
C			   3. Choose correct value of SP for STACK$PTR.
C    21OCT81  Alex Hunter  1. Implement %_signed and %_unsigned. (V5.5)
C    10NOV81  Alex Hunter  1. Determine procedure side effects. (V6.0)
C    12NOV81  Alex Hunter  1. Implement LAST(MEMORY), et al. (V6.1)
C
C***********************************************************************
      INTEGER*2 FUNCTION BUILTIN_FUNCTION(DPIX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 LENGTH,SIZE,LOWER_BOUND
      COMMON /BUILTINS/ SYM_SUBS,MEM_SUBS
 
      PIX=DPIX
 
      IF (SYMBOL_PLM_ID(PIX).EQ.'LENGTH'.OR.
     #    SYMBOL_PLM_ID(PIX).EQ.'FIRST'.OR.
     #    SYMBOL_PLM_ID(PIX).EQ.'LAST') THEN
 
         CALL MATCH(D_LP)
 
         IF (TT.EQ.FIXCON.OR.TT.EQ.FLOATCON.OR.TT.EQ.STRCON) THEN
            LENGTH=1
            LOWER_BOUND=0
            CALL GETTOK
         ELSE
            CALL MUSTBE(ID)
            CALL LOOKUP_SYMBOL
            CALL PUSH(PIX,1)
            ARG=DATA_REFERENCE(0,.TRUE.)
            CALL POP(PIX,1)
            IF ((SYMBOL_FLAGS(SYMBOL_INDEX).AND.S_SPECIAL).NE.0.AND.
     #          SYMBOL_PLM_ID(PIX).NE.'FIRST') THEN
               IF (SYMBOL_PLM_ID(SYMBOL_INDEX).EQ.'MEMORY') THEN
                  IF (SYMBOL_PLM_ID(PIX).EQ.'LENGTH') THEN
                     SYM=SYM_MLEN
                  ELSE
                     SYM=SYM_MLAST
                  ENDIF
               ELSE
                  IF (SYMBOL_PLM_ID(PIX).EQ.'LENGTH') THEN
                     SYM=SYM_SLEN
                  ELSE
                     SYM=SYM_SLAST
                  ENDIF
               ENDIF
               BUILTIN_FUNCTION=MAKE_ATOM(SYM,0,NULL,NULL,S_LONG,0,0)
               GO TO 10
            ELSEIF (MEMBER_INDEX.EQ.0) THEN
               IF (SYM_SUBS.EQ.NULL) THEN
                  LENGTH=SYMBOL_NBR_ELEMENTS(SYMBOL_INDEX)
                  LOWER_BOUND=SYMBOL_LOWER_BOUND(SYMBOL_INDEX)
               ELSE
                  LENGTH=1
                  LOWER_BOUND=0
               ENDIF
            ELSE
               IF (MEM_SUBS.EQ.NULL) THEN
                  LENGTH=MEMBER_NBR_ELEMENTS(MEMBER_INDEX)
                  LOWER_BOUND=MEMBER_LOWER_BOUND(MEMBER_INDEX)
               ELSE
                  LENGTH=1
                  LOWER_BOUND=0
               ENDIF
            ENDIF
         ENDIF
 
         IF (SYMBOL_PLM_ID(PIX).EQ.'LENGTH') THEN
            BUILTIN_FUNCTION=MAKE_FIXED(LENGTH,S_LONG)
         ELSEIF (SYMBOL_PLM_ID(PIX).EQ.'FIRST') THEN
            BUILTIN_FUNCTION=MAKE_FIXED(LOWER_BOUND,S_LONG)
         ELSE
            BUILTIN_FUNCTION=MAKE_FIXED(LOWER_BOUND+LENGTH-1,S_LONG)
         ENDIF
 
10       CALL MATCH(D_RP)
 
      ELSEIF (SYMBOL_PLM_ID(PIX).EQ.'SIZE') THEN
 
         CALL MATCH(D_LP)
 
         IF (TT.EQ.FIXCON) THEN
            IF (FIXVAL.LE.255) THEN
               SIZE=1
            ELSEIF (FIXVAL.LE.'FFFF'X) THEN
               SIZE=2
            ELSE
               SIZE=4
            ENDIF
            CALL GETTOK
         ELSEIF (TT.EQ.FLOATCON) THEN
            SIZE=4
            CALL GETTOK
         ELSEIF (TT.EQ.STRCON) THEN
            SIZE=STRLEN
            CALL GETTOK
         ELSE
            CALL MUSTBE(ID)
            CALL LOOKUP_SYMBOL
            CALL PUSH(PIX,1)
            ARG=DATA_REFERENCE(0,.TRUE.)
            CALL POP(PIX,1)
            IF ((SYMBOL_FLAGS(SYMBOL_INDEX).AND.S_SPECIAL).NE.0) THEN
               IF (SYMBOL_PLM_ID(SYMBOL_INDEX).EQ.'MEMORY') THEN
                  SYM=SYM_MSIZ
               ELSE
                  SYM=SYM_SSIZ
               ENDIF
               BUILTIN_FUNCTION=MAKE_ATOM(SYM,0,NULL,NULL,S_LONG,0,0)
               GO TO 20
            ELSEIF (MEMBER_INDEX.EQ.0) THEN
               IF (SYM_SUBS.EQ.NULL) THEN
                  SIZE=SYMBOL_NBR_ELEMENTS(SYMBOL_INDEX)
                  SIZE=SIZE*SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX)
               ELSE
                  SIZE=SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX)
               ENDIF
            ELSE
               IF (MEM_SUBS.EQ.NULL) THEN
                  SIZE=MEMBER_NBR_ELEMENTS(MEMBER_INDEX)*
     #                 MEMBER_ELEMENT_SIZE(MEMBER_INDEX)
               ELSE
                  SIZE=MEMBER_ELEMENT_SIZE(MEMBER_INDEX)
               ENDIF
            ENDIF
         ENDIF
 
         BUILTIN_FUNCTION=MAKE_FIXED(SIZE,S_LONG)
 
20       CALL MATCH(D_RP)
 
      ELSEIF (SYMBOL_PLM_ID(PIX).EQ.'STACKPTR') THEN
         IF (MODEL.EQ.2 .OR. MODEL.EQ.4) THEN
            SP=14
         ELSE
            SP=10
         ENDIF
         BUILTIN_FUNCTION=MAKE_REGISTER(SP,S_PTR)
 
      ELSEIF (SYMBOL_PLM_ID(PIX).EQ.'FRAMEPTR') THEN
         BUILTIN_FUNCTION=MAKE_REGISTER(13,S_PTR)
 
      ELSEIF (SYMBOL_PLM_ID(PIX)(1:2).EQ.'$_' .OR.
     #        SYMBOL_PLM_ID(PIX).EQ.'DOUBLE' .OR.
     #        SYMBOL_PLM_ID(PIX).EQ.'LOW'    .OR.
     #        SYMBOL_PLM_ID(PIX).EQ.'FLOAT'  .OR.
     #        SYMBOL_PLM_ID(PIX).EQ.'FIX'    .OR.
     #        SYMBOL_PLM_ID(PIX).EQ.'INT'    .OR.
     #        SYMBOL_PLM_ID(PIX).EQ.'SIGNED' .OR.
     #        SYMBOL_PLM_ID(PIX).EQ.'UNSIGN') THEN
 
         CALL MATCH(D_LP)
         CALL PUSH(PIX,1)
         ARG=EXPRESSION(1)
         CALL POP(PIX,1)
         CALL MATCH(D_RP)
 
         IF (SYMBOL_PLM_ID(PIX).EQ.'$_SIGNED') THEN
            BUILTIN_FUNCTION=MAKE_NODE(OP_SIGNED,ARG,NULL,0,0,0)
         ELSEIF (SYMBOL_PLM_ID(PIX).EQ.'$_UNSIGNED') THEN
            BUILTIN_FUNCTION=MAKE_NODE(OP_UNSIGNED,ARG,NULL,0,0,0)
         ELSE
            IF (SYMBOL_PLM_ID(PIX).EQ.'INT' .OR.
     #          SYMBOL_PLM_ID(PIX).EQ.'SIGNED') THEN
               ARG=MAKE_NODE(OP_WORD,ARG,NULL,S_WORD,0,0)
            ELSEIF (SYMBOL_PLM_ID(PIX).EQ.'UNSIGN') THEN
               ARG=MAKE_NODE(OP_INTEGER,ARG,NULL,S_INTEGER,0,0)
            ENDIF
            BUILTIN_FUNCTION=MAKE_NODE(OP_BYTE+SYMBOL_TYPE(PIX)-S_BYTE,
     #                            ARG,NULL,SYMBOL_TYPE(PIX),0,0)
         ENDIF
 
      ELSE
 
         CALL ERROR('UNIMPLEMENTED BUILTIN FUNCTION: '//
     #              SYMBOL_PLM_ID(PIX))
         BUILTIN_FUNCTION=NULL
      ENDIF
 
      CALL DETERMINE_EFFECTS_OF_CALLING(PIX)
 
      RETURN
      END
