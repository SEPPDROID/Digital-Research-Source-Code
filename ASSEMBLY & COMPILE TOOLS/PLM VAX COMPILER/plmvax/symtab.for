C***********************************************************************
C
C                               SYMTAB.FOR
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
C    This module of the PL/M-VAX compiler contains procedures to enter
C    and lookup identifiers in the symbol and member-symbol tables.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. Re-compiled with larger symbol table.
C    21OCT81  Alex Hunter  1. Initialize serial no. fields. (V5.5)
C    12NOV81  Alex Hunter  1. Initialize psect field. (V6.1)
C
C***********************************************************************
      SUBROUTINE ENTER_SYMBOL
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 UNIQUE
C
      H=HASH(IDENTIFIER)
      I=HASH_BUCKET(H)
10    IF (I.GE.SYMBOL_TOP(BLOCK_LEVEL-1)+1) THEN
         IF (SYMBOL_PLM_ID(I).EQ.IDENTIFIER) THEN
            IF (SYMBOL_REF(I).EQ.S_ARG.AND.SYMBOL_FLAGS(I).EQ.S_UNDEF
     #          .OR.SYMBOL_REF(I).EQ.S_FORWARD)
     #      THEN
               SYMBOL_INDEX=I
               RETURN
            ENDIF
            CALL ERROR('DUPLICATE DECLARATION FOR '//IDENTIFIER)
            GO TO 20
         ENDIF
         I=SYMBOL_CHAIN(I)
         GO TO 10
      ENDIF
20    SYMBOL_INDEX=SYMBOL_TOP(BLOCK_LEVEL)+1
      IF (SYMBOL_INDEX.GT.SYMBOL_MAX)
     #   CALL FATAL('SYMBOL TABLE OVERFLOW')
      SYMBOL_TOP(BLOCK_LEVEL)=SYMBOL_INDEX
      SYMBOL_PLM_ID(SYMBOL_INDEX)=IDENTIFIER
      SYMBOL_VAX_ID(SYMBOL_INDEX)=UNIQUE(IDENTIFIER)
      SYMBOL_KIND(SYMBOL_INDEX)=0
      SYMBOL_TYPE(SYMBOL_INDEX)=0
      SYMBOL_NBR_ELEMENTS(SYMBOL_INDEX)=0
      SYMBOL_ELEMENT_SIZE(SYMBOL_INDEX)=0
      SYMBOL_LINK(SYMBOL_INDEX)=0
      SYMBOL_LIST_SIZE(SYMBOL_INDEX)=0
      SYMBOL_REF(SYMBOL_INDEX)=0
      SYMBOL_BASE(SYMBOL_INDEX)=0
      SYMBOL_BASE_MEMBER(SYMBOL_INDEX)=0
      SYMBOL_FLAGS(SYMBOL_INDEX)=S_UNDEF
      SYMBOL_DISP(SYMBOL_INDEX)=0
      SYMBOL_SERIAL_NO(SYMBOL_INDEX)=0
      SYMBOL_PSECT(SYMBOL_INDEX)=0
      SYMBOL_CHAIN(SYMBOL_INDEX)=HASH_BUCKET(H)
      HASH_BUCKET(H)=SYMBOL_INDEX
      RETURN
C-------------------------------
      ENTRY LOOKUP_SYMBOL
      H=HASH(IDENTIFIER)
      SYMBOL_INDEX=HASH_BUCKET(H)
30    IF (SYMBOL_INDEX.GT.0) THEN
         IF (SYMBOL_PLM_ID(SYMBOL_INDEX).EQ.IDENTIFIER) THEN
            IF ((SYMBOL_FLAGS(SYMBOL_INDEX).AND.S_UNDEF).NE.0) THEN
               CALL ERROR('NO DECLARATION FOR '//IDENTIFIER)
            ENDIF
            RETURN
         ENDIF
         SYMBOL_INDEX=SYMBOL_CHAIN(SYMBOL_INDEX)
         GO TO 30
      ENDIF
      CALL ERROR('UNDEFINED SYMBOL: '//IDENTIFIER)
      GO TO 20
      END
C-----------------------------------------------------------------------
      SUBROUTINE ENTER_MEMBER
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 UNIQUE
C
      MEMBER_INDEX=MEMBER_TOP(BLOCK_LEVEL)+1
      IF (MEMBER_INDEX.GT.MEMBER_MAX)
     #    CALL FATAL('MEMBER TABLE OVERFLOW')
      MEMBER_TOP(BLOCK_LEVEL)=MEMBER_INDEX
      MEMBER_PLM_ID(MEMBER_INDEX)=IDENTIFIER
      MEMBER_VAX_ID(MEMBER_INDEX)=UNIQUE(IDENTIFIER)
      MEMBER_KIND(MEMBER_INDEX)=0
      MEMBER_TYPE(MEMBER_INDEX)=0
      MEMBER_NBR_ELEMENTS(MEMBER_INDEX)=0
      MEMBER_ELEMENT_SIZE(MEMBER_INDEX)=0
      MEMBER_SERIAL_NO(MEMBER_INDEX)=0
      RETURN
C-------- CHECK FOR DUPLICATE MEMBER NAMES!!! ------
C
C-------------------------------
      ENTRY LOOKUP_MEMBER
      IF (SYMBOL_TYPE(SYMBOL_INDEX).NE.S_STRUC) THEN
         CALL ERROR('NOT A STRUCTURE: '//
     #               SYMBOL_PLM_ID(SYMBOL_INDEX))
      ELSE
         DO 20 MEMBER_INDEX=SYMBOL_LINK(SYMBOL_INDEX),
     #                      SYMBOL_LINK(SYMBOL_INDEX)+
     #                        SYMBOL_LIST_SIZE(SYMBOL_INDEX)-1
            IF (MEMBER_PLM_ID(MEMBER_INDEX).EQ.IDENTIFIER) RETURN
20       CONTINUE
         CALL ERROR('UNDEFINED MEMBER: '//IDENTIFIER)
      ENDIF
      MEMBER_INDEX=0
      RETURN
      END
