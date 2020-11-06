C***********************************************************************
C
C                               SAVETREE.FOR
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
C    This module of the PL/M-VAX compiler contains routines to save
C    and restore the current code tree.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    24OCT81  Alex Hunter  1. Save and restore atom serial no. (V5.6)
C    12NOV81  Alex Hunter  1. Change ATOM_DISP to I*4. (V6.1)
C
C-----------------------------------------------------------------------
      SUBROUTINE SAVE_CODE_TREE
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      N=NEXT_NODE-NODE_MIN
      IF (N.GT.0) THEN
         CALL PUSH(OPNODE_OP(NODE_MIN),N)
         CALL PUSH(OPNODE_OPND1(NODE_MIN),N)
         CALL PUSH(OPNODE_OPND2(NODE_MIN),N)
         CALL PUSH(NODE_REG(NODE_MIN),N)
         CALL PUSH(NODE_REFCT(NODE_MIN),N)
         CALL PUSH(NODE_TYPE(NODE_MIN),N)
         CALL PUSH(NODE_CONTEXT(NODE_MIN),N)
      ENDIF
      CALL PUSH(NEXT_NODE,1)
 
      N=NEXT_ATOM-FIRST_FREE_ATOM
      IF (N.GT.0) THEN
         CALL PUSH(ATOM_SYM(FIRST_FREE_ATOM),N)
         CALL PUSH(ATOM_MEM(FIRST_FREE_ATOM),N)
         CALL PUSH(ATOM_BASE(FIRST_FREE_ATOM),N)
         CALL PUSH(ATOM_SUB(FIRST_FREE_ATOM),N)
         CALL PUSH(ATOM_DISP(FIRST_FREE_ATOM),N*2)
         CALL PUSH(ATOM_FLAGS(FIRST_FREE_ATOM),N)
         CALL PUSH(ATOM_SERIAL_NO(FIRST_FREE_ATOM),N)
         CALL PUSH(NODE_REG(FIRST_FREE_ATOM),N)
         CALL PUSH(NODE_REFCT(FIRST_FREE_ATOM),N)
         CALL PUSH(NODE_TYPE(FIRST_FREE_ATOM),N)
         CALL PUSH(NODE_CONTEXT(FIRST_FREE_ATOM),N)
      ENDIF
      CALL PUSH(NEXT_ATOM,1)
 
      N=NEXT_FIXED-FIX_MIN
      IF (N.GT.0) THEN
         CALL PUSH(FIXED_VAL(FIX_MIN),N*2)
         CALL PUSH(NODE_REG(FIX_MIN),N)
         CALL PUSH(NODE_REFCT(FIX_MIN),N)
         CALL PUSH(NODE_TYPE(FIX_MIN),N)
         CALL PUSH(NODE_CONTEXT(FIX_MIN),N)
      ENDIF
      CALL PUSH(NEXT_FIXED,1)
 
      N=NEXT_FLOAT-FLT_MIN
      IF (N.GT.0) THEN
         CALL PUSH(FLOAT_VAL(FLT_MIN),N*4)
         CALL PUSH(NODE_REG(FLT_MIN),N)
         CALL PUSH(NODE_REFCT(FLT_MIN),N)
         CALL PUSH(NODE_TYPE(FLT_MIN),N)
         CALL PUSH(NODE_CONTEXT(FLT_MIN),N)
      ENDIF
      CALL PUSH(NEXT_FLOAT,1)
 
      N=NEXT_CONSTANT-CON_MIN
      IF (N.GT.0) THEN
         CALL PUSH(CONSTANT_LABEL(CON_MIN),N)
         CALL PUSH(NODE_REG(CON_MIN),N)
         CALL PUSH(NODE_REFCT(CON_MIN),N)
         CALL PUSH(NODE_TYPE(CON_MIN),N)
         CALL PUSH(NODE_CONTEXT(CON_MIN),N)
      ENDIF
      CALL PUSH(NEXT_CONSTANT,1)
      RETURN
 
C-------------------------------
      ENTRY RESTORE_CODE_TREE
C-------------------------------
 
      CALL POP(NEXT_CONSTANT,1)
      N=NEXT_CONSTANT-CON_MIN
      IF (N.GT.0) THEN
         CALL POP(NODE_CONTEXT(CON_MIN),N)
         CALL POP(NODE_TYPE(CON_MIN),N)
         CALL POP(NODE_REFCT(CON_MIN),N)
         CALL POP(NODE_REG(CON_MIN),N)
         CALL POP(CONSTANT_LABEL(CON_MIN),N)
      ENDIF
 
      CALL POP(NEXT_FLOAT,1)
      N=NEXT_FLOAT-FLT_MIN
      IF (N.GT.0) THEN
         CALL POP(NODE_CONTEXT(FLT_MIN),N)
         CALL POP(NODE_TYPE(FLT_MIN),N)
         CALL POP(NODE_REFCT(FLT_MIN),N)
         CALL POP(NODE_REG(FLT_MIN),N)
         CALL POP(FLOAT_VAL(FLT_MIN),N*4)
      ENDIF
 
      CALL POP(NEXT_FIXED,1)
      N=NEXT_FIXED-FIX_MIN
      IF (N.GT.0) THEN
         CALL POP(NODE_CONTEXT(FIX_MIN),N)
         CALL POP(NODE_TYPE(FIX_MIN),N)
         CALL POP(NODE_REFCT(FIX_MIN),N)
         CALL POP(NODE_REG(FIX_MIN),N)
         CALL POP(FIXED_VAL(FIX_MIN),N*2)
      ENDIF
 
      CALL POP(NEXT_ATOM,1)
      N=NEXT_ATOM-FIRST_FREE_ATOM
      IF (N.GT.0) THEN
         CALL POP(NODE_CONTEXT(FIRST_FREE_ATOM),N)
         CALL POP(NODE_TYPE(FIRST_FREE_ATOM),N)
         CALL POP(NODE_REFCT(FIRST_FREE_ATOM),N)
         CALL POP(NODE_REG(FIRST_FREE_ATOM),N)
         CALL POP(ATOM_SERIAL_NO(FIRST_FREE_ATOM),N)
         CALL POP(ATOM_FLAGS(FIRST_FREE_ATOM),N)
         CALL POP(ATOM_DISP(FIRST_FREE_ATOM),N*2)
         CALL POP(ATOM_SUB(FIRST_FREE_ATOM),N)
         CALL POP(ATOM_BASE(FIRST_FREE_ATOM),N)
         CALL POP(ATOM_MEM(FIRST_FREE_ATOM),N)
         CALL POP(ATOM_SYM(FIRST_FREE_ATOM),N)
      ENDIF
 
      CALL POP(NEXT_NODE,1)
      N=NEXT_NODE-NODE_MIN
      IF (N.GT.0) THEN
         CALL POP(NODE_CONTEXT(NODE_MIN),N)
         CALL POP(NODE_TYPE(NODE_MIN),N)
         CALL POP(NODE_REFCT(NODE_MIN),N)
         CALL POP(NODE_REG(NODE_MIN),N)
         CALL POP(OPNODE_OPND2(NODE_MIN),N)
         CALL POP(OPNODE_OPND1(NODE_MIN),N)
         CALL POP(OPNODE_OP(NODE_MIN),N)
      ENDIF
      RETURN
      END
