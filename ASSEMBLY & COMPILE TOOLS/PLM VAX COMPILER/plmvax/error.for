C***********************************************************************
C
C                               ERROR.FOR
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
C    This module of the PL/M-VAX compiler processes error messages
C    of several degrees of severity.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C
C-----------------------------------------------------------------------
      SUBROUTINE ERROR(T)
C
C----- REPORT AN ERROR.
C
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*(*) T
C
      IF (PRINT_FLAG) THEN 
         CALL ADVANCE_ONE_LINE
         WRITE(LST,1000) T(:LNB(T))
      ENDIF
      IF (.NOT.LISTING_TO_TERMINAL) THEN
         CALL TYPE_LAST_SOURCE_LINE
         WRITE(7,1000) T(:LNB(T))
      ENDIF
1000  FORMAT(' ******** Error: 'A)
      IF (OBJECT_FLAG) WRITE(OUT,1003) T(:LNB(T))
1003  FORMAT(' .ERROR ; ',A)
      ERRORS=ERRORS+1
      RETURN
C--------------------------
      ENTRY FATAL(T)
C--------------------------
      IF (PRINT_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,1001) T(:LNB(T))
      ENDIF
      IF (.NOT.LISTING_TO_TERMINAL) THEN
         CALL TYPE_LAST_SOURCE_LINE
         WRITE(7,1001) T(:LNB(T))
      ENDIF
1001  FORMAT(' ******** Fatal Error: ',A)
      IF (OBJECT_FLAG) WRITE(OUT,1003) T(:LNB(T))
100   STOP '** COMPILATION ABORTED **'
C--------------------------
      ENTRY WARN(T)
C--------------------------
      IF (.NOT.WARN_FLAG) RETURN
      IF (PRINT_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,1002) T(:LNB(T))
      ENDIF
      IF (.NOT.LISTING_TO_TERMINAL) THEN
         CALL TYPE_LAST_SOURCE_LINE
         WRITE(7,1002) T(:LNB(T))
      ENDIF
1002  FORMAT(' ******** Warning: ',A)
      IF (OBJECT_FLAG) WRITE(OUT,1004) T(:LNB(T))
1004  FORMAT(' .WARN ; ',A)
      WARNINGS=WARNINGS+1
      RETURN
      END
C--------------------------
      SUBROUTINE BUG(T)
C--------------------------
      IMPLICIT INTEGER*2 (A-Z)
      CHARACTER*(*) T
      CALL ERROR('COMPILER BUG -- '//T)
200   RETURN
      END
