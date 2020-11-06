C***********************************************************************
C
C                               UNIQUE.FOR
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
C    generate unique identifiers for local symbols, public symbols,
C    and transfer vector entry points.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    14NOV81  Alex Hunter  1. Use full 31-character external names
C			      (hopefully the bug in MACRO has been
C			      fixed). (V6.2)
C
C-----------------------------------------------------------------------
      CHARACTER*32 FUNCTION UNIQUE(IDENT)
      IMPLICIT INTEGER*2 (A-Z)
      CHARACTER IDENT*(*), STRING10*10, Q*10
      INTEGER*4 N, IFSD
      DATA N/0/
C
      N=N+1
      Q=STRING10(N,IFSD)
      K=MIN(16,LNB(IDENT))
      UNIQUE=IDENT(1:K)//'.'//Q(IFSD:10)
      RETURN
      END
C------------------------------------------------
      CHARACTER*32 FUNCTION PUBLIQUE(IDENT)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 IDENT
      CHARACTER*3 BAD_ID(20)
      CHARACTER*4 GOOD_ID(20)
      DATA BAD_ID
     //'R0','R1','R2','R3','R4','R5','R6','R7','R8','R9','R10'
     ,,'R11','R12','R13','R14','R15','AP','FP','SP','PC'
     //
      DATA GOOD_ID
     //'R0.','R1.','R2.','R3.','R4.','R5.','R6.','R7.','R8.','R9.'
     ,,'R10.','R11.','R12.','R13.','R14.','R15.','AP.','FP.','SP.'
     ,,'PC.'
     //
 
      DO 10 I=1,20
      IF (IDENT.EQ.BAD_ID(I)) THEN
         PUBLIQUE=GOOD_ID(I)
         GO TO 20
      ENDIF
10    CONTINUE
      PUBLIQUE=IDENT(1:31)  ! Use first 31 chars of name.
      SAME_OVERLAY=.FALSE.
20    IF (OVERLAY_FLAG) THEN
         DO 30 I=1,LAST_GLOBAL
         IF (PUBLIQUE.EQ.GLOBAL_SYMBOL(I)) GO TO 40
30       CONTINUE
         SAME_OVERLAY=.TRUE.
         PUBLIQUE(OVERLAY_PREFIX(0)+1:)='.'//PUBLIQUE
         CALL MAKE_CHARS(PUBLIQUE,OVERLAY_PREFIX)
      ENDIF
40    PUBLIQUE(32:32)=' '   ! Truncate to 31 chars.
      RETURN
      END
C---------------------------------------------------------------------
      CHARACTER*32 FUNCTION VECNIQUE(IDENT)
      IMPLICIT INTEGER*2 (A-Z)
      CHARACTER*32 IDENT
C
      VECNIQUE='V.'//IDENT
      VECNIQUE(32:32)=' '   ! Truncate to 31 chars.
      RETURN
      END
