C***********************************************************************
C
C				REGS.FOR
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
C-----------------------------------------------------------------------
C
C    This module of the PL/M-VAX compiler contains routines to manage
C    register allocation and preservation.
C
C-----------------------------------------------------------------------
C
C    15OCT81  Alex Hunter  1. Experimental version. (V54.)
C    14NOV81  Alex Hunter  1. If LARGE OVERLAY, then add R12 to
C			      register pool. (V6.2)
C
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION ALLOCATE_REG_WITH_PREFERENCE(TYPE,POSSIBLE_REG)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 LAST_REG,FIRST_REG,R
      LOGICAL*4 AVAILABLE(1:16)
 
      R = POSSIBLE_REG
 
C------- First try to allocate the preferred register.
 
      IF (REGISTER(R) .AND. AVAILABLE(R) .AND.
     #    (BYTE_SIZE(TYPE).LE.4 .OR. AVAILABLE(R+1))) GO TO 10
 
C------- Otherwise, use the normal allocation algorithm.
 
C---------------------------------------------
      ENTRY ALLOCATE_REG(TYPE)
C---------------------------------------------
      DO R = LAST_REG-1, FIRST_REG, -1
         IF (AVAILABLE(R) .AND.
     #       (BYTE_SIZE(TYPE).LE.4 .OR. AVAILABLE(R+1))) GO TO 10
      ENDDO
 
      CALL ERROR('REGISTER POOL EXHAUSTED')
      ALLOCATE_REG = LAST_REG-1   ! Just so we can keep compiling.
      NODE_TYPE(ALLOCATE_REG) = TYPE
      RETURN
 
C------- Here when we have an available register.
 
10    CONTINUE
      ALLOCATE_REG = R
      CALL PRESERVE_REG(R)
      AVAILABLE(R) = .FALSE.
      NODE_TYPE(R) = TYPE
      IF (BYTE_SIZE(TYPE).GT.4) THEN
         CALL PRESERVE_REG(R+1)
         AVAILABLE(R+1) = .FALSE.
         NODE_TYPE(R+1) = TYPE
      ENDIF
      RETURN
C--------------------------------------
      ENTRY FREE_REGS()
C--------------------------------------
      IF (MODEL.EQ.4.AND.OVERLAY_FLAG) THEN
         LAST_REG=12
      ELSEIF (MODEL.EQ.2 .OR. MODEL.EQ.4) THEN
         LAST_REG=11
      ELSE
         LAST_REG=10
      ENDIF
      FIRST_REG=PROC_AP(PROC_LEVEL)+1
      DO R=1,LAST_REG
         AVAILABLE(R) = R.GE.FIRST_REG .AND. R.LT.LAST_REG
      ENDDO
      RETURN
C--------------------------------------
      ENTRY FREE_REG(REG)
C--------------------------------------
      AVAILABLE(REG) = .TRUE.
      IF (BYTE_SIZE(NODE_TYPE(REG)).GT.4) THEN
         AVAILABLE(REG+1) = .TRUE.
      ENDIF
      RETURN
      END
C-------------------------------------------------------
      SUBROUTINE PRESERVE_REG(REG)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      PROC_ENTRY_MASK(PROC_LEVEL)=PROC_ENTRY_MASK(PROC_LEVEL).OR.
     #                            ISHFT(1,REG)
      RETURN
      END
C--------------------------------------------------------
      CHARACTER*40 FUNCTION REGISTER_MASK(MASK)
      IMPLICIT INTEGER*2 (A-Z)
      CHARACTER*3 REGNAME(0:15)
      DATA REGNAME
     //'R0','R1','R2','R3','R4','R5','R6','R7','R8','R9'
     ,,'R10','R11','AP','FP','SP','PC'
     //
      INTEGER*2 REGLEN(0:15)
      DATA REGLEN
     //2,2,2,2,2,2,2,2,2,2,3,3,2,2,2,2/
 
      REGISTER_MASK='^M<'
      N=4
 
      DO 10 I=0,15
      IF ((ISHFT(MASK,-I).AND.1).NE.0) THEN
         IF (N.GT.4) THEN
            REGISTER_MASK(N:)=','
            N=N+1
         ENDIF
         REGISTER_MASK(N:)=REGNAME(I)
         N=N+REGLEN(I)
      ENDIF
10    CONTINUE
 
      REGISTER_MASK(N:)='>'
   
      RETURN
      END
