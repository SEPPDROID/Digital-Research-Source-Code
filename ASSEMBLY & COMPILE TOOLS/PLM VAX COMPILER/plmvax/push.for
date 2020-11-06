C***********************************************************************
C
C                               PUSH.FOR
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
C    This module of the PL/M-VAX compiler implements the pushdown
C    stacks used by recursive FORTRAN subroutines and functions.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C
C-----------------------------------------------------------------------
      SUBROUTINE PUSH(DATA,NWORDS)
      IMPLICIT INTEGER*2 (A-Z)
      INTEGER*2 DATA(*), SP, STACK(1000)
      DATA SP/0/
C
      IF (SP+NWORDS.GT.1000) CALL FATAL('SYNTAX STACK OVERFLOW')
      DO 10 I=1,NWORDS
         STACK(SP+I) = DATA(I)
10    CONTINUE
      SP = SP+NWORDS
      RETURN
C------------------------------------------
      ENTRY POP(DATA,NWORDS)
      SP = SP-NWORDS
      IF (SP.LT.0) CALL BUG('SYNTAX STACK UNDERFLOW')
      DO 20 I=1,NWORDS
         DATA(I) = STACK(SP+I)
20    CONTINUE
      RETURN
      END
C------------------------------------------
      SUBROUTINE PUSHC(CHARS)
      IMPLICIT INTEGER*2 (A-Z)
      CHARACTER CHARS*(*), CSTACK(100)*32
      DATA SP/0/
C
      IF (SP.GE.100) CALL FATAL('CHAR STACK OVERFLOW')
      SP=SP+1
      CSTACK(SP)=CHARS
      RETURN
C------------------------------------------
      ENTRY POPC(CHARS)
      IF (SP.LE.0) CALL BUG('CHAR STACK UNDERFLOW')
      CHARS=CSTACK(SP)
      SP=SP-1
      RETURN
      END
