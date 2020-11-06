C***********************************************************************
C
C                               OPEN.FOR
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
C    This module of the PL/M-VAX compiler contains routines used to
C    open input and output files.  A 'USEROPEN' procedure is invoked
C    when an input file is opened to allow access to the VFC line
C    numbers created by text editors.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    04FEB82  Alex Hunter  1. Change name of useropen procedure
C			      and its common block. (V6.6)
C
C-----------------------------------------------------------------------
      SUBROUTINE OPEN_SOS_FILE(UNIT,FILE_STRING)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      EXTERNAL XQ_GET_CNTRL_FLD
      BYTE FILE_STRING(0:45)
      INTEGER*2 W_LINE_NUMBER(0:99)
      COMMON /XQ_LINE_SEQS/ W_LINE_NUMBER
 
      FILE_STRING(FILE_STRING(0)+1)=0
      W_LINE_NUMBER(UNIT)=-1
      I=1
      IF (FILE_STRING(1).EQ.':') I=2
 
      OPEN(UNIT=UNIT,FILE=FILE_STRING(I),STATUS='OLD',READONLY,
     #     USEROPEN=XQ_GET_CNTRL_FLD,ERR=99)
      RETURN
 
99    IF (PRINT_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,1000) (FILE_STRING(I),I=1,FILE_STRING(0))
      ENDIF
      IF (.NOT.LISTING_TO_TERMINAL) THEN
         CALL TYPE_LAST_SOURCE_LINE
         WRITE(7,1000) (FILE_STRING(I),I=1,FILE_STRING(0))
      ENDIF
1000  FORMAT(' **** Input File Not Found: ',99A1)
      STOP 'COMPILATION ABORTED'
 
C----------------------------------
      ENTRY OPEN_OUTPUT_FILE(UNIT,FILE_STRING)
C----------------------------------
      FILE_STRING(FILE_STRING(0)+1)=0
      I=1
      IF (FILE_STRING(1).EQ.':') I=2
 
      OPEN(UNIT=UNIT,FILE=FILE_STRING(I),STATUS='NEW')
 
      RETURN
      END
