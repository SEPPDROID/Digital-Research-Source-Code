C***********************************************************************
C
C                               LIST.FOR
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
C    This module of the PL/M-VAX compiler contains routines for listing
C    lines to the print file.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    18OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C
C-----------------------------------------------------------------------
      SUBROUTINE LIST_SOURCE_LINE(LINE)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*(*) LINE
      CHARACTER*10 STRING10,S1,S2,S3
      INTEGER*4 IVAL,IFSD
      
      LAST_LINE_EXISTS=.TRUE.
 
      IVAL=LIST_LINE_NO+100000
      S1=STRING10(IVAL,IFSD)
 
      IF (IN.EQ.8) THEN
         S2=' '
      ELSE
         IVAL=(IN-9)*10
         S2=STRING10(IVAL,IFSD)
         S2(8:8)='='
      ENDIF
 
      IF (OBJECT_FLAG.AND.OPRINT_FLAG.AND.LIST_FLAG) THEN
         WRITE(OUT,1001) S1(6:10),S2(8:9),LINE
1001     FORMAT(' ;',A5,A2,X,A)
      ENDIF
 
      IF (.NOT.PRINT_FLAG .OR. .NOT.LIST_FLAG) RETURN
 
      GO TO (20,10,10,20), SKIP_STATE
10    IF (.NOT.COND_FLAG) RETURN
20    CONTINUE
 
      CALL ADVANCE_ONE_LINE
 
      IF (LIST_STNO.NE.PREVIOUS_STNO.AND.LINE.NE.' '.AND.
     #    LINE(LEFTMARGIN:LEFTMARGIN).NE.'$'.AND.
     #    SKIP_STATE.NE.2.AND.SKIP_STATE.NE.3) THEN
 
         IVAL=LIST_BLOCK_LEVEL*10
         S3=STRING10(IVAL,IFSD)
 
         WRITE(LST,1002) S1(6:10),LIST_STNO,S3(8:9),S2(8:9),LINE
1002     FORMAT(X,A5,X,I4,X,A2,X,A2,X,A)
 
         PREVIOUS_STNO=LIST_STNO
 
      ELSE
         WRITE(LST,1003) S1(6:10),S2(8:9),LINE
1003     FORMAT(X,A5,9X,A2,X,A)
      ENDIF
      RETURN
C-------------------------
      ENTRY FORCE_LIST_SOURCE
C------------------------
      IF (.NOT.PRINT_FLAG .OR. .NOT.LIST_FLAG .OR. COND_FLAG) RETURN
      IF (.NOT.LAST_LINE_EXISTS) RETURN
 
      CALL ADVANCE_ONE_LINE
      WRITE(LST,1003) S1(6:10),S2(8:9),CARD(:LNB(CARD))
      RETURN
C----------------------------------------
      ENTRY TYPE_LAST_SOURCE_LINE
C----------------------------------------
      IF (.NOT.LAST_LINE_EXISTS) RETURN
 
      WRITE(7,1003) S1(6:10),S2(8:9),CARD(:LNB(CARD))
      RETURN
C-------------------------------------
      ENTRY LIST_LINE(LINE)
C-------------------------------------
      IF (.NOT.PRINT_FLAG) RETURN
      
      CALL ADVANCE_ONE_LINE
      WRITE(LST,1004) LINE(:LNB(LINE))
1004  FORMAT(X,A)
      RETURN
      END
C-----------------------------------------------------------------
      SUBROUTINE ADVANCE_ONE_LINE
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 TITLE,SUBTITLE,DATE
      CHARACTER*45 SOURCE_FILE
 
      LINE_OF_PAGE=LINE_OF_PAGE+1
 
      IF (PAGING_FLAG.AND.(EJECT_FLAG.OR.LINE_OF_PAGE.GT.PAGELENGTH))
     #   THEN
 
         PAGE_NO=PAGE_NO+1
 
         N1=MAKE_CHARS(TITLE,TITLE_STRING)
         N2=MAKE_CHARS(SUBTITLE,SUBTITLE_STRING)
         N3=MAKE_CHARS(DATE,DATE_STRING)
         N4=MAKE_CHARS(SOURCE_FILE,IN_FILE_STRING(0,8))
 
         T1=55-N1/2
         T2=55-N2/2
         T3=110-N3
 
         WRITE(LST,1001) TITLE(:N1),DATE(:N3),PAGE_NO,SOURCE_FILE(:N4),
     #                   SUBTITLE(:N2)
 
1001     FORMAT('1PL/M-VAX COMPILER',T<T1>,A,T<T3>,A,T112,'Page ',I4/
     #          X,A,T<T2>,A/)
 
         EJECT_FLAG=.FALSE.
         LINE_OF_PAGE=4
 
      ENDIF
      RETURN
      END
C--------------------------------------------------------
      SUBROUTINE ROOM_FOR(NBR_OF_LINES)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      IF (LINE_OF_PAGE+NBR_OF_LINES.GT.PAGELENGTH) EJECT_FLAG=.TRUE.
 
      RETURN
      END
