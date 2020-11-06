C***********************************************************************
C
C    				COUNTS.FOR
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
C    This module of the PL/M-VAX compiler computes reference counts
C    for the nodes of a code tree.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    08SEP81  Alex Hunter  1. Written. (V5.1)
C    28SEP81  Alex Hunter  2. STACKPTR caused CRC-0 bug. (V5.3)
C    15OCT81  Alex Hunter  1. Experimental version. (V5.4)
C    23OCT81  Alex Hunter  1. Compute correct reference counts for
C			      operand 1 of OP_LOC and LHS of OP_MOV
C			      and OP_ASSN. (V5.6)
C    10NOV81  Alex Hunter  1. Implement DBG assumption. (V6.0)
C    08FEB82  Alex Hunter  1. Correct count for merged ARG opnodes. (V6.7)
C
C***********************************************************************
      SUBROUTINE COMPUTE_REFERENCE_COUNTS (NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD = NODX	! Call by value.
 
      IF (NOD.EQ.NULL .OR. REGISTER(NOD)) THEN
         RETURN
 
      ELSEIF (LITERAL(NOD) .OR. CONSTANT(NOD)) THEN
         NODE_REFCT(NOD) = NODE_REFCT(NOD) + 1
 
      ELSEIF (ATOM(NOD)) THEN
         NODE_REFCT(NOD) = NODE_REFCT(NOD) + 1
         CALL PUSH(NOD,1)
         CALL COMPUTE_REFERENCE_COUNTS2 (ATOM_BASE(NOD))
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL COMPUTE_REFERENCE_COUNTS2 (ATOM_SUB(NOD))
         CALL POP(NOD,1)
 
      ELSEIF (NODE(NOD)) THEN
         NODE_REFCT(NOD) = NODE_REFCT(NOD) + 1
         IF (NODE_REFCT(NOD).EQ.1.OR.OPNODE_OP(NOD).EQ.OP_ARG) THEN
            CALL PUSH(NOD,1)
            IF (OPNODE_OP(NOD).EQ.OP_LOC) THEN
               CALL COMPUTE_ATOM_REFERENCE_COUNTS (OPNODE_OPND1(NOD))
            ELSE
               CALL COMPUTE_REFERENCE_COUNTS2 (OPNODE_OPND1(NOD))
            ENDIF
            CALL POP(NOD,1)
            CALL PUSH(NOD,1)
            IF (OPNODE_OP(NOD).EQ.OP_MOV .OR. OPNODE_OP(NOD).EQ.OP_ASSN)
     #         THEN
               CALL COMPUTE_ATOM_REFERENCE_COUNTS (OPNODE_OPND2(NOD))
            ELSE
               CALL COMPUTE_REFERENCE_COUNTS2 (OPNODE_OPND2(NOD))
            ENDIF
            CALL POP(NOD,1)
         ENDIF
 
      ELSE
         CALL BUG ('CRC-0 -- Invalid kind of node.')
      ENDIF
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE COMPUTE_REFERENCE_COUNTS2 (NODX)
      IMPLICIT INTEGER*2 (A-Z)
 
      CALL COMPUTE_REFERENCE_COUNTS (NODX)
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE COMPUTE_ATOM_REFERENCE_COUNTS (NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD = NODX	! Call by value.
 
      IF (ATOM(NOD)) THEN
         CALL PUSH(NOD,1)
         CALL COMPUTE_REFERENCE_COUNTS (ATOM_BASE(NOD))
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL COMPUTE_REFERENCE_COUNTS (ATOM_SUB(NOD))
         CALL POP(NOD,1)
      ENDIF
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE DECREMENT_VALUE_COUNTS (NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD = NODX	! Call by value.
 
      CALL DECREMENT_REFERENCE_COUNTS(NOD)
 
      IF (ATOM(NOD)) THEN
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_BASE(NOD))
         CALL DECREMENT_REFERENCE_COUNTS(ATOM_SUB(NOD))
      ENDIF
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE DECREMENT_REFERENCE_COUNTS (NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD = NODX	! Call by value.
 
      IF (NOD.EQ.NULL .OR. REGISTER(NOD)) RETURN
 
      NODE_REFCT(NOD) = NODE_REFCT(NOD) - 1
 
      IF (ASSUME_DBG) WRITE(OUT,1001) NOD, NODE_REFCT(NOD)
1001  FORMAT(' ;*DRC*  nod',I6,' refct decremented to',I6)
 
      IF (NODE_REFCT(NOD).EQ.-1) THEN
         CALL BUG('DRC -- Node reference count decremented to -1.')
      ENDIF
 
      IF (NODE_REFCT(NOD).EQ.0 .AND. NODE_REG(NOD).NE.0) THEN
         IF (ASSUME_DBG) WRITE(OUT,1002) NODE_REG(NOD)
1002     FORMAT(' ;*DRC* register ',I2,' can be reused...')
         CALL FREE_REG(NODE_REG(NOD))
      ENDIF
      RETURN
      END
