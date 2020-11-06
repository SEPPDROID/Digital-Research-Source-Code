C***********************************************************************
C
C                               GETTOK.FOR
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
C    This module of the PL/M-VAX compiler contains the token analysis
C    routine which is called to obtain the next token from the input
C    stream.  The token analyzer looks ahead one lexical element to
C    determine if the next token is a label, and if so stores the
C    label in the current label list.  This list must be emptied
C    before the next token is obtained, or an error will be diagnosed.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    14JAN82  Alex Hunter  1. Treat <keyword>: as <identifier>:. (V6.5)
C
C-----------------------------------------------------------------------
      SUBROUTINE GETTOK
      INCLUDE 'PLMCOM.FOR/NOLIST'
C
      DO 5 I=1,NLABELS
      CALL ERROR('MISPLACED LABEL -- '//LABELS(I))
5     CONTINUE
      NLABELS=0
10    TOKENTYPE=NEXT_TOKENTYPE
      DELIMITER=NEXT_DELIMITER
      IDENTIFIER=NEXT_IDENTIFIER
      STRING=NEXT_STRING
      STRLEN=NEXT_STRLEN
      FIXVAL=NEXT_FIXVAL
      FLOATVAL=NEXT_FLOATVAL
     
      CALL GETLEX
 
      IF (NEXT_TOKENTYPE.EQ.D_COLON .AND.
     #    (TOKENTYPE.EQ.ID.OR.(TOKENTYPE.GE.101.AND.TOKENTYPE.LE.199)))
     #THEN
         IF (NLABELS.GE.MAX_LABELS) THEN
            CALL ERROR('TOO MANY LABELS -- '//IDENTIFIER)
         ELSE
            NLABELS=NLABELS+1
            LABELS(NLABELS) = IDENTIFIER
         ENDIF
         CALL GETLEX
         GO TO 10
      ENDIF
      RETURN
      END
