C***********************************************************************
C
C                               PSECTS.FOR
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
C    This module of the PL/M-VAX compiler handles changes in object
C    code program sections (PSECTs).
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    12NOV81  Alex Hunter  1. Add SETUP_COMMON_PSECT routine. (V6.1)
C
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION PSECT(P)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 NAME
      DATA CURRENT_PSECT/P_CODE/
 
      PSECT=CURRENT_PSECT
      IF (P.NE.CURRENT_PSECT.AND.P.NE.0) THEN
         CALL EMIT('.PSECT  '//PSECT_NAME(P))
      ENDIF
      CURRENT_PSECT=P
      RETURN
C-------------------------------------------------------
      ENTRY SETUP_COMMON_PSECT(NAME)
C------------------------------------
      DO I=P_MEMORY,P_MAX
         IF (PSECT_NAME(I).EQ.NAME) THEN
            SETUP_COMMON_PSECT=I
            RETURN
         ELSEIF (PSECT_NAME(I).EQ.' ') THEN
            PSECT_NAME(I)=NAME
            CALL EMIT('.PSECT  '//NAME(:LNB(NAME))//
     #                ',PIC,OVR,GBL,SHR,NOEXE,RD,WRT,LONG')
            CURRENT_PSECT=I
            SETUP_COMMON_PSECT=I
            RETURN
         ENDIF
      ENDDO
      CALL FATAL('TOO MANY DIFFERENT COMMON BLOCKS')
      END
