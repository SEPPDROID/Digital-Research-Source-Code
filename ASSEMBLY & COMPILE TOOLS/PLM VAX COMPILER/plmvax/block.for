C***********************************************************************
C
C                               BLOCK.FOR
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
C    This module of the PL/M-VAX compiler handles block entries
C    and exits.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    16OCT81  Alex Hunter  1. Added disclaimer notice.
C    14NOV81  Alex Hunter  1. Avoid unnecessary jump if no path. (V6.2)
C                          2. Copy symbol serial no. and psect fields.
C
C***********************************************************************
      SUBROUTINE BLOCK_BEGIN
      INCLUDE 'PLMCOM.FOR/NOLIST'
C
      IF (BLOCK_LEVEL.GE.BLOCK_MAX) 
     #    CALL FATAL('BLOCKS NESTED TOO DEEPLY')
      BLOCK_LEVEL=BLOCK_LEVEL+1
      SYMBOL_TOP(BLOCK_LEVEL)=SYMBOL_TOP(BLOCK_LEVEL-1)
      MEMBER_TOP(BLOCK_LEVEL)=MEMBER_TOP(BLOCK_LEVEL-1)
      PARAM_TOP(BLOCK_LEVEL)=PARAM_TOP(BLOCK_LEVEL-1)
      STRINGS_TOP(BLOCK_LEVEL)=STRINGS_TOP(BLOCK_LEVEL-1)
      RETURN
C
C---------------------------
      ENTRY BLOCK_END
C---------------------------
      IF (BLOCK_LEVEL.EQ.0) CALL BUG('BLOCK LEVEL UNDERFLOW')
      DO 10 I=SYMBOL_TOP(BLOCK_LEVEL),SYMBOL_TOP(BLOCK_LEVEL-1)+1,-1
         H=HASH(SYMBOL_PLM_ID(I))
         HASH_BUCKET(H)=SYMBOL_CHAIN(I)
10    CONTINUE
      BLOCK_LEVEL=BLOCK_LEVEL-1
 
C---------- HANDLE UNRESOLVED LABELS AND UNDEFINED FORWARD REFS
 
      DO 40 I=SYMBOL_TOP(BLOCK_LEVEL)+1,SYMBOL_TOP(BLOCK_LEVEL+1)
      IF (SYMBOL_REF(I).EQ.S_FORWARD.OR.
     #    BLOCK_LEVEL.EQ.0.AND.(SYMBOL_FLAGS(I).AND.S_UNDEF).NE.0) THEN
         CALL ERROR('NEVER GOT DEFINED: '//SYMBOL_PLM_ID(I))
      ELSEIF (SYMBOL_KIND(I).EQ.S_LABEL.AND.
     #        (SYMBOL_FLAGS(I).AND.S_UNDEF).NE.0) THEN
         ! -- UNRESOLVED LABEL. ----
         DO 20 J=SYMBOL_TOP(BLOCK_LEVEL-1)+1,SYMBOL_TOP(BLOCK_LEVEL)
         IF (SYMBOL_PLM_ID(I).EQ.SYMBOL_PLM_ID(J)) THEN
            IF (SYMBOL_KIND(J).NE.S_LABEL) THEN
               CALL ERROR('GOTO TARGET NOT A LABEL: '//SYMBOL_PLM_ID(I))
            ELSEIF ((SYMBOL_FLAGS(J).AND.S_UNDEF).EQ.0) THEN
               IF (SYMBOL_REF(J).EQ.S_EXT) THEN
                  IF (PATH) CALL GENERATE_LOCAL_LABEL(LL)
                  IF (PATH) CALL EMIT('BRB     '//LOCAL_LABEL(LL,N0))
                  CALL EMIT_LABEL(I)
                  CALL EMIT('JMP     '//SYMBOL_VAX_ID(J))
                  IF (PATH) CALL EMIT_LOCAL_LABEL(LL)
               ELSE
                  CALL EMIT1(SYMBOL_VAX_ID(I)(:LNB(SYMBOL_VAX_ID(I)))
     #                       //' = '//
     #                       SYMBOL_VAX_ID(J)(:LNB(SYMBOL_VAX_ID(J))))
               ENDIF
            ELSE
               SYMBOL_REF(I)=SYMBOL_REF(J)
               SYMBOL_FLAGS(I)=SYMBOL_FLAGS(J).AND..NOT.S_PUBLIC
               GO TO 30
            ENDIF
            GO TO 40
         ENDIF
20       CONTINUE
C---------- LABEL STILL UNRESOLVED -- COPY DOWN TO OUTER BLOCK.
30       SYMBOL_TOP(BLOCK_LEVEL)=SYMBOL_TOP(BLOCK_LEVEL)+1
         IX=SYMBOL_TOP(BLOCK_LEVEL)
         SYMBOL_PLM_ID(IX)=SYMBOL_PLM_ID(I)
         SYMBOL_VAX_ID(IX)=SYMBOL_VAX_ID(I)
         SYMBOL_KIND(IX)=SYMBOL_KIND(I)
         SYMBOL_TYPE(IX)=SYMBOL_TYPE(I)
         SYMBOL_NBR_ELEMENTS(IX)=SYMBOL_NBR_ELEMENTS(I)
         SYMBOL_ELEMENT_SIZE(IX)=SYMBOL_ELEMENT_SIZE(I)
         SYMBOL_LINK(IX)=SYMBOL_LINK(I)
         SYMBOL_LIST_SIZE(IX)=SYMBOL_LIST_SIZE(I)
         SYMBOL_REF(IX)=SYMBOL_REF(I)
         SYMBOL_BASE(IX)=SYMBOL_BASE(I)
         SYMBOL_BASE_MEMBER(IX)=SYMBOL_BASE_MEMBER(I)
         SYMBOL_FLAGS(IX)=SYMBOL_FLAGS(I)
         SYMBOL_SERIAL_NO(IX)=SYMBOL_SERIAL_NO(I)
         SYMBOL_PSECT(IX)=SYMBOL_PSECT(I)
         H=HASH(SYMBOL_PLM_ID(I))
         SYMBOL_CHAIN(IX)=HASH_BUCKET(H)
         HASH_BUCKET(H)=IX
      ENDIF
40    CONTINUE
      RETURN
      END
