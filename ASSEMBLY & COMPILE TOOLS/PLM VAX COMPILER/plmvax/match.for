C***********************************************************************
C
C                               MATCH.FOR
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
C    This module of the PL/M-VAX compiler contains routines to test the
C    next token for a match.  Syntax errors are detected and analyzed
C    by this module.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    18OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    28OCT81  Alex Hunter  1. Add new keywords. (V5.7)
C
C-----------------------------------------------------------------------
      SUBROUTINE MATCH(TOKEN)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 ACTUAL1,ACTUAL2,ACTUAL3,TOKEN1,TOKEN2,TOKEN3
 
      IF (TOKEN.EQ.TT) THEN
         CALL GETTOK
         RETURN
      ENDIF
      GO TO 10
C----------------------------
      ENTRY MUSTBE(TOKEN)
C----------------------------
      IF (TOKEN.EQ.TT) RETURN
C
C
C  SYNTAX ERROR......
C
10    CALL ANALYZE_TOKEN(TOKEN1,ACTUAL1,TOKENTYPE,FIXVAL,FLOATVAL,
     #                   STRLEN,DELIMITER,IDENTIFIER,STRING)
      CALL ANALYZE_TOKEN(TOKEN2,ACTUAL2,NEXT_TOKENTYPE,NEXT_FIXVAL,
     #                   NEXT_FLOATVAL,NEXT_STRLEN,NEXT_DELIMITER,
     #                   NEXT_IDENTIFIER,NEXT_STRING)
      CALL ANALYZE_TOKEN(TOKEN3,ACTUAL3,TOKEN,0,0.0,1,' ',' ',' ')
 
      IF (PRINT_FLAG) THEN
         WRITE(LST,1001) ACTUAL1(:LNB(ACTUAL1)),ACTUAL2(:LNB(ACTUAL2)),
     #                TOKEN3(:LNB(TOKEN3)),ACTUAL3(:LNB(ACTUAL3)),
     #                TOKEN1(:LNB(TOKEN1)),ACTUAL1(:LNB(ACTUAL1))
      ENDIF
 
      IF (.NOT.LISTING_TO_TERMINAL) THEN
         CALL TYPE_LAST_SOURCE_LINE
         WRITE(7,1001) ACTUAL1(:LNB(ACTUAL1)),ACTUAL2(:LNB(ACTUAL2)),
     #                TOKEN3(:LNB(TOKEN3)),ACTUAL3(:LNB(ACTUAL3)),
     #                TOKEN1(:LNB(TOKEN1)),ACTUAL1(:LNB(ACTUAL1))
      ENDIF
 
1001  FORMAT(/' ***** Syntax Error Near ',A,X,A,' ****'//
     #        ' ***** Expected:         ',A,X,A/
     #        ' ***** Actually Found:   ',A,X,A/)
 
      STOP '**** Compilation Aborted (Syntax Error) ****'
 
      END
C------------------------------------------------------------
      SUBROUTINE ANALYZE_TOKEN(TOKEN_STRING,ACTUAL_STRING,TOKEN,
     #                         FIXV,FLOATV,STRL,D_STRING,ID_STRING,
     #                         S_STRING)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER TOKEN_STRING*32,ACTUAL_STRING*32,D_STRING*(*),
     #          ID_STRING*(*),S_STRING*(*)
      INTEGER*4 FIXV
      REAL*8 FLOATV
      CHARACTER*10 KEYWORD(101:154)
      CHARACTER*2 DD(201:218)
      COMMON /ANALYZE/ KEYWORD,DD
      CHARACTER*16 NON_TERMINAL(301:303)
      DATA NON_TERMINAL /
     # '<statement>','<expression>','<type>'/
 
      IF (TOKEN.EQ.INVALID) THEN
         TOKEN_STRING='<illegal character>'
         ENCODE(32,1000,ACTUAL_STRING) ICHAR(D_STRING),D_STRING(1:1)
1000     FORMAT('X''',Z2,''' (',A1,')')
         RETURN
 
      ELSEIF (TOKEN.EQ.ID) THEN
         TOKEN_STRING='<identifier>'
         ACTUAL_STRING=ID_STRING
         RETURN
 
      ELSEIF (TOKEN.EQ.FIXCON) THEN
         TOKEN_STRING='<fixed point constant>'
         ENCODE(32,1001,ACTUAL_STRING) FIXV
1001     FORMAT(I10)
         RETURN
 
      ELSEIF (TOKEN.EQ.FLOATCON) THEN
         TOKEN_STRING='<floating point constant>'
         ENCODE(32,1002,ACTUAL_STRING) FLOATV
1002     FORMAT(G14.7)
         RETURN
 
      ELSEIF (TOKEN.EQ.STRCON) THEN
         TOKEN_STRING='<string constant>'
         ACTUAL_STRING=''''//S_STRING
         IF (STRL.LE.30) THEN
            ACTUAL_STRING(STRL+2:STRL+2)=''''
         ELSE
            ACTUAL_STRING(30:32)='...'
         ENDIF
         RETURN
 
      ELSEIF (TOKEN.EQ.EOFTOK) THEN
         TOKEN_STRING='<eof>'
         ACTUAL_STRING=' '
         RETURN
 
      ELSEIF (TOKEN.GE.101.AND.TOKEN.LE.199) THEN
         TOKEN_STRING='<keyword>'
         ACTUAL_STRING=KEYWORD(TOKEN)
         RETURN
 
      ELSEIF (TOKEN.GE.201.AND.TOKEN.LE.299) THEN
         TOKEN_STRING='<delimiter>'
         ACTUAL_STRING=DD(TOKEN)
         RETURN
 
      ELSE	! MUST BE NON_TERMINAL PSEUDO_TOKEN.
 
         TOKEN_STRING=NON_TERMINAL(TOKEN)
         ACTUAL_STRING=' '
         RETURN
 
      ENDIF
      END
