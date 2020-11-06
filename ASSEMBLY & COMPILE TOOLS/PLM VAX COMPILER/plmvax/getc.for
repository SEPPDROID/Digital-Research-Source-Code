C***********************************************************************
C
C                               GETC.FOR
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
C    This module of the PL/M-VAX compiler contains routines which
C    are called by the lexical analysis module (GETLEX) to obtain
C    the next (maybe non-blank) source character.  The source char-
C    acter may come from the source input file, an INCLUDE file, or
C    a macro body.  When a new source line is read, it is (possibly)
C    listed, and tested to see if it is a control line.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    04FEB82  Alex Hunter  1. Delete reference to GET_CNTRL_FLD. (V6.6)
C    			   2. Change name of LINE_SEQS common block.
C
C-----------------------------------------------------------------------
      SUBROUTINE GETC
C
C----- GET NEXT CHARACTER FROM INPUT STREAM.
C
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*2 W_LINE_NUMBER(0:99)
      COMMON /XQ_LINE_SEQS/ W_LINE_NUMBER
      CHARACTER*1 CR
      DATA CR /'0D'X/
      PARAMETER FIFO_MAX=10
      CHARACTER*133 FIFO_LINE(FIFO_MAX)
      INTEGER*2 FIFO_LEN(FIFO_MAX),FIFO_LINE_NO(FIFO_MAX),
     #          FIFO_IN(FIFO_MAX)
      CHARACTER*300 CARD1

10    COL=COL+1
20    CHAR = LITVAL(LITLEV)(COL:COL)
 
      IF (CHAR.EQ.EOL) THEN
        IF (LITLEV.EQ.1) THEN
30        IF (TABS.NE.0) THEN
             READ(IN,1000,END=100) L,CARD1
             J=1
             CARD=' '
             DO 31 I=1,L
                IF (CARD1(I:I).EQ.TAB) THEN
                   J=J+TABS-MOD(J-1,TABS)
                ELSEIF (J.LE.300) THEN
                   CARD(J:J)=CARD1(I:I)
                   J=J+1
                ENDIF
31           CONTINUE
             L=J-1
          ELSE
             READ(IN,1000,END=100) L,CARD
          ENDIF
1000      FORMAT(Q,A)
          LINES_READ=LINES_READ+1
          IF (W_LINE_NUMBER(IN).GE.0) THEN
             LIST_LINE_NO=W_LINE_NUMBER(IN)
          ELSE
             LIST_LINE_NO = -W_LINE_NUMBER(IN)
             W_LINE_NUMBER(IN) = W_LINE_NUMBER(IN)-1
          ENDIF
          IF (CARD(LEFTMARGIN:LEFTMARGIN).EQ.'$') THEN
             IF (.NOT.NON_CONTROL_LINE_READ) THEN
                FIFO_DEPTH=FIFO_DEPTH+1
                IF (FIFO_DEPTH.GT.FIFO_MAX)
     #             CALL FATAL('TOO MANY CONTROL LINES BEFORE FIRST '
     #                        //'NON-CONTROL LINE')
                FIFO_LINE(FIFO_DEPTH)=CARD
                FIFO_LEN(FIFO_DEPTH)=L
                FIFO_LINE_NO(FIFO_DEPTH)=LIST_LINE_NO
                FIFO_IN(FIFO_DEPTH)=IN
             ELSE
                CALL LIST_SOURCE_LINE(CARD(:L))
             ENDIF
             CARD(L+1:L+1)=CR
             CALL DQ SWITCH BUFFER(%REF(CARD(LEFTMARGIN+1:)),STATUS)
             CALL CONTROL_LINE
             GO TO 30
          ENDIF
 
          IF (.NOT.NON_CONTROL_LINE_READ) THEN
             NON_CONTROL_LINE_READ=.TRUE.
             CALL OPEN_OUTPUT_FILES
             CALL INIT_SYMTAB
             LISTING_TO_TERMINAL=PRINT_FILE_STRING(0).GE.3.AND.
     #          PRINT_FILE_STRING(1).EQ.'T'.AND.
     #          PRINT_FILE_STRING(2).EQ.'T'.AND.
     #          PRINT_FILE_STRING(3).EQ.':'
             CALL SUMMARY_HEAD
             LINE_NO_SAVE=LIST_LINE_NO
             IN_SAVE=IN
             SKIP_STATE_SAVE=SKIP_STATE
             SKIP_STATE=4
             DO 35 I=1,FIFO_DEPTH
                LIST_LINE_NO=FIFO_LINE_NO(I)
                IN=FIFO_IN(I)
                CALL LIST_SOURCE_LINE(FIFO_LINE(I)(:FIFO_LEN(I)))
35           CONTINUE
             LIST_LINE_NO=LINE_NO_SAVE
             IN=IN_SAVE
             SKIP_STATE=SKIP_STATE_SAVE
          ENDIF
 
          CALL LIST_SOURCE_LINE(CARD(:L))
 
          GO TO (40,30,30,40), SKIP_STATE
40        CONTINUE
 
          CARD(L+2:L+2) = EOL
          COL = LEFTMARGIN
        ELSE
          LITLEV = LITLEV-1
          COL = LITCOL(LITLEV)
        ENDIF
        GO TO 20
      ENDIF
      RETURN
 
100   IF (IN.EQ.8) THEN
         CHAR=EOF
      ELSE
         CLOSE(UNIT=IN)
         IN=IN-1
         GO TO 30
      ENDIF
      RETURN
      END
C-------------------------------------------------------
 
      SUBROUTINE GETNB
C
C------ GET NEXT NON-BLANK CHARACTER.
C
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 I
      CHARACTER*1 CH
 
10    DO 20 I=COL+1,999
      CH=LITVAL(LITLEV)(I:I)
      IF (CH.NE.' '.AND.CH.NE.TAB) GO TO 30
20    CONTINUE
      STOP 'GETNB BUG'
30    IF (CH.EQ.EOL) THEN
         COL=I-1
         CALL GETC
         IF (CHAR.EQ.' '.OR.CHAR.EQ.TAB) GO TO 10
      ELSE
         CHAR=CH
         COL=I
      ENDIF
      RETURN
      END
