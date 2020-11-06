C***********************************************************************
C
C                               EFFECTS.FOR
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
C    This module of the PL/M-VAX compiler determines the side effects
C    of storage assignments and procedure calls for use in common
C    subexpression elimination and basic block analysis.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    10NOV81  Alex Hunter  1. Written.
C
C***********************************************************************
      SUBROUTINE DETERMINE_EFFECTS_OF_ASSIGNMENT (LHS)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      IF (ATOM_MEM(LHS).NE.0) THEN
         MEMBER_SERIAL_NO(ATOM_MEM(LHS)) =
     #      MEMBER_SERIAL_NO(ATOM_MEM(LHS)) + 1
      ELSE
         SYMBOL_SERIAL_NO(ATOM_SYM(LHS)) =
     #      SYMBOL_SERIAL_NO(ATOM_SYM(LHS)) + 1
      ENDIF
 
      IF (ASSUME_EEQ .AND.
     #    SYMBOL_REF(ATOM_SYM(LHS)).EQ.S_EXT) THEN
         EXTERNAL_SERIAL_DELTA = EXTERNAL_SERIAL_DELTA + 1
		! Invalidate all externals.
      ENDIF
 
      IF (ASSUME_BRO) THEN
         BASED_SERIAL_DELTA = BASED_SERIAL_DELTA + 1
		! Invalidate all based references.
         IF (ATOM_BASE(LHS).NE.NULL) THEN
            END_OF_BASIC_BLOCK = .TRUE.
		! All bets are off.
         ENDIF
      ENDIF
 
      IF (.NOT.ASSUME_SWB) THEN
         SUBCRIPTED_SERIAL_DELTA = SUBSCRIPTED_SERIAL_DELTA + 1
		! Invalidate all array references.
         IF (ATOM_SUB(LHS).NE.NULL) THEN
            END_OF_BASIC_BLOCK = .TRUE.
		! All bets are off.
         ENDIF
      ENDIF
 
      IF ((SYMBOL_FLAGS(ATOM_SYM(LHS)).AND.S_OVERLAID).NE.0) THEN
         OVERLAID_SERIAL_DELTA = OVERLAID_SERIAL_DELTA + 1
         ! When equivalence chains are implemented, we will
         ! be able to refine this if ASSUME_SVE is true.
      ENDIF
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE DETERMINE_EFFECTS_OF_CALLING (PROC_IX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      IF (ASSUME_PSE .AND.
     #    (SYMBOL_FLAGS(PROC_IX).AND.S_NO_SIDE_EFFECTS).EQ.0) THEN
         SYMBOL_SERIAL_NO(PROC_IX) = SYMBOL_SERIAL_NO(PROC_IX) + 1
         END_OF_BASIC_BLOCK = .TRUE.
         	! All bets are off.
      ENDIF
      RETURN
      END
