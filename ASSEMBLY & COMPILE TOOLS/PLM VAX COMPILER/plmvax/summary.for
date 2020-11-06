C***********************************************************************
C
C                               SUMMARY.FOR
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
C    This module of the PL/M-VAX compiler is used to produce the
C    summaries at the beginning and end of a compilation listing.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    09NOV81  Alex Hunter  1. Print assumptions in summary tail. (V5.8)
C    10NOV81  Alex Hunter  1. Add DBG assumption. (V6.0)
C    14JAN82  Alex Hunter  1. Change "S32" to "S64". (V6.5)
C
C-----------------------------------------------------------------------
      SUBROUTINE SUMMARY_HEAD
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*256 TAIL
      COMMON /COMMAND_TAIL/ TAIL
      CHARACTER*1 CR
      DATA CR /'0D'X/
      CHARACTER*132 LINE
      CHARACTER*10 STRING10,S1
      INTEGER*4 IFSD,IVAL
      LOGICAL*1 ASSUMPTIONS(15)
      EQUIVALENCE (ASSUMPTIONS,ASSUME_SCE)
      CHARACTER*3 ASSUMPTION_NAME(15)
      DATA ASSUMPTION_NAME /
     # 'SCE','CSE','EEQ','PSE','BRO','BBA','CTE',
     # 'MCO','CFA','SWB','OSR','SVE','S64','C7F','DBG'/
 
      ENCODE(132,1001,LINE) VERSION,
     #       (IN_FILE_STRING(I,8),I=1,IN_FILE_STRING(0,8))
1001  FORMAT('PL/M-VAX V',F3.1,' Compilation of ',45A1)
      CALL LIST_LINE(LINE)
 
      IF (GLOBALS_FLAG) THEN
         ENCODE(132,1008,LINE)
     #          (GLOBALS_FILE_STRING(I),I=1,GLOBALS_FILE_STRING(0))
1008     FORMAT('Global Symbols read from ',45A1)
         CALL LIST_LINE(LINE)
      ENDIF
   
      IF (PRINT_FLAG) THEN
         ENCODE(132,1006,LINE)
     #          (PRINT_FILE_STRING(I),I=1,PRINT_FILE_STRING(0))
1006     FORMAT('Source Listing written to ',45A1)
         CALL LIST_LINE(LINE)
      ENDIF
 
      IF (OBJECT_FLAG) THEN
         ENCODE(132,1002,LINE)
     #          (WORK_FILE_STRING(I),I=1,WORK_FILE_STRING(0))
1002     FORMAT('Intermediate Code placed in ',45A1)
         CALL LIST_LINE(LINE)
      ENDIF
 
      IF (PUBLICS_FLAG) THEN
         ENCODE(132,1009,LINE)
     #          (PUBLICS_FILE_STRING(I),I=1,PUBLICS_FILE_STRING(0))
1009     FORMAT('Public Symbols placed in ',45A1)
         CALL LIST_LINE(LINE)
      ENDIF
 
      IF (IXREF_FLAG) THEN
         ENCODE(132,1003,LINE) 
     #          (IXREF_FILE_STRING(I),I=1,IXREF_FILE_STRING(0))
1003     FORMAT('Intermodule XREF placed in ',45A1)
         CALL LIST_LINE(LINE)
      ENDIF
 
      IF (OBJECT_FLAG) THEN
         ENCODE(132,1004,LINE)
     #           (OBJECT_FILE_STRING(I),I=1,OBJECT_FILE_STRING(0))
1004     FORMAT('Object Module placed in ',45A1)
         CALL LIST_LINE(LINE)
      ENDIF
   
      IF (OPRINT_FLAG.AND.OBJECT_FLAG) THEN
         ENCODE(132,1007,LINE)
     #          (OPRINT_FILE_STRING(I),I=1,OPRINT_FILE_STRING(0))
1007     FORMAT('Object Listing placed in ',45A1)
         CALL LIST_LINE(LINE)
      ENDIF
 
      DO 10 I=1,100
      IF (TAIL(I:I).EQ.CR) GO TO 20
10    CONTINUE
      I=101
20    I=I-1
      ENCODE(132,1005,LINE) TAIL(1:I)
1005  FORMAT('Compiler invoked by:  PLM ',A)
      CALL LIST_LINE(LINE)
 
      CALL LIST_LINE(' ')
      CALL LIST_LINE(' ')
      CALL LIST_LINE(' ')
 
      RETURN
C------------------------------------------
      ENTRY SUMMARY_TAIL
C------------------------------------------
      CALL ROOM_FOR(15)
      CALL LIST_LINE(' ')
      CALL LIST_LINE(' ')
 
      CALL LIST_LINE('Compilation Summary:')
      CALL LIST_LINE(' ')
 
      ENCODE(132,1011,LINE) OPTIMIZE,PAGELENGTH,PAGEWIDTH,TABS
1011  FORMAT('Primary Controls:',T20,'OPTIMIZE(',I1,') PAGELENGTH(',
     #       I3,') PAGEWIDTH(',I3,') TABS(',I2,')')
      CALL LIST_LINE(LINE)
 
      LINE=' '
      N=20
 
      IF (MODEL.EQ.1) LINE(N:)='SMALL'
      IF (MODEL.EQ.2) LINE(N:)='COMPACT'
      IF (MODEL.EQ.3) LINE(N:)='MEDIUM'
      IF (MODEL.EQ.4) LINE(N:)='LARGE'
      N=LNB(LINE)+2
 
      IF (PRINT_FLAG) THEN
         LINE(N:)='PRINT'
      ELSE
         LINE(N:)='NOPRINT'
      ENDIF
      N=LNB(LINE)+2
 
      IF (XREF_FLAG) THEN
         LINE(N:)='XREF'
      ELSE
         LINE(N:)='NOXREF'
      ENDIF
      N=LNB(LINE)+2
 
      IF (IXREF_FLAG) THEN
         LINE(N:)='IXREF'
      ELSE 
         LINE(N:)='NOIXREF'
      ENDIF
      N=LNB(LINE)+2
 
      IF (OBJECT_FLAG) THEN
         LINE(N:)='OBJECT'
      ELSE
         LINE(N:)='NOOBJECT'
      ENDIF
      N=LNB(LINE)+2
 
      IF (OPRINT_FLAG) THEN
         LINE(N:)='OPRINT'
      ELSE
         LINE(N:)='NOOPRINT'
      ENDIF
      N=LNB(LINE)+2
 
      IF (SYMBOLS_FLAG) THEN
         LINE(N:)='SYMBOLS'
      ELSE
         LINE(N:)='NOSYMBOLS'
      ENDIF
      N=LNB(LINE)+2
 
      IF (DEBUG_FLAG) THEN
         LINE(N:)='DEBUG'
      ELSE
         LINE(N:)='NODEBUG'
      ENDIF
      N=LNB(LINE)+2
 
      IF (PAGING_FLAG) THEN
         LINE(N:)='PAGING'
      ELSE
         LINE(N:)='NOPAGING'
      ENDIF
      N=LNB(LINE)+2
 
      IF (INTVECTOR_FLAG) THEN
         LINE(N:)='INTVECTOR'
      ELSE
         LINE(N:)='NOINTVECTOR'
      ENDIF
      N=LNB(LINE)+2
 
      CALL LIST_LINE(LINE)
 
      LINE=' '
      N=20
 
      IF (TYPE_FLAG) THEN
         LINE(N:)='TYPE'
      ELSE
         LINE(N:)='NOTYPE'
      ENDIF
      N=LNB(LINE)+2
  
      IF (ROM_FLAG) THEN
         LINE(N:)='ROM'
      ELSE
         LINE(N:)='RAM'
      ENDIF
      N=LNB(LINE)+2
 
      IF (WARN_FLAG) THEN
         LINE(N:)='WARN'
      ELSE
         LINE(N:)='NOWARN'
      ENDIF
      N=LNB(LINE)+2
 
      IF (PLM80_FLAG) THEN
         LINE(N:)='PLM80'
         N=LNB(LINE)+2
      ENDIF
 
      IF (GLOBALS_FLAG) THEN
         LINE(N:)='GLOBALS'
      ELSE
         LINE(N:)='NOGLOBALS'
      ENDIF
      N=LNB(LINE)+2
 
      IF (PUBLICS_FLAG) THEN
         LINE(N:)='PUBLICS'
      ELSE
         LINE(N:)='NOPUBLICS'
      ENDIF
      N=LNB(LINE)+2
 
      IF (OVERLAY_FLAG) THEN
         LINE(N:)='OVERLAY'
      ELSE
         LINE(N:)='NOOVERLAY'
      ENDIF
      N=LNB(LINE)+2
 
      IF (ROOT_FLAG) THEN
         LINE(N:)='ROOT'
      ELSE
         LINE(N:)='NOROOT'
      ENDIF
      N=LNB(LINE)+2
 
      IF (ALIGN_FLAG) THEN
         LINE(N:)='ALIGN'
      ELSE
         LINE(N:)='NOALIGN'
      ENDIF
      N=LNB(LINE)+2
 
      IF (FREQ_FLAG) THEN
         LINE(N:)='FREQUENCIES'
         N=LNB(LINE)+2
      ENDIF
 
      IF (VECTOR_FLAG) THEN
         LINE(N:)='VECTOR'
      ELSE
         LINE(N:)='NOVECTOR'
      ENDIF
      N=LNB(LINE)+2
 
      CALL LIST_LINE(LINE)
      CALL LIST_LINE(' ')
 
      LINE='Assumptions:'
      N=20
 
      DO I=1,15
         IF (.NOT.ASSUMPTIONS(I)) THEN
            LINE(N:)='NO'
            N=N+2
         ENDIF
         LINE(N:)=ASSUMPTION_NAME(I)
         N=N+4
      ENDDO
 
      CALL LIST_LINE(LINE)
      CALL LIST_LINE(' ')
 
      IVAL=LINES_READ
      S1=STRING10(IVAL,IFSD)
      ENCODE(132,1012,LINE) S1(IFSD:)
1012  FORMAT(T20,A,' Lines Read')
      CALL LIST_LINE(LINE)
 
      IVAL=ERRORS
      S1=STRING10(IVAL,IFSD)
      ENCODE(132,1013,LINE) S1(IFSD:)
1013  FORMAT(T20,A,' Program Error(s)')
      CALL LIST_LINE(LINE)
 
      IF (WARNINGS.NE.0) THEN
         IVAL=WARNINGS
         S1=STRING10(IVAL,IFSD)
         ENCODE(132,1014,LINE) S1(IFSD:)
1014     FORMAT(T20,A,' Program Warning(s)')
         CALL LIST_LINE(LINE)
      ENDIF
 
      CALL LIST_LINE(' ')
      CALL LIST_LINE('End of PL/M-VAX Compilation')
 
      RETURN
      END
