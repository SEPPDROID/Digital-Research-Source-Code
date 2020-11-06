C***********************************************************************
C
C                               REPLICA.FOR
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
C    This module of the PL/M-VAX compiler replicates (i.e., creates a
C    copy of) a code tree.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. Correct bug in replicating register nodes
C			      (e.g., for STACK$PTR).   (V5.3)
C			   2. Add consistency check REP-0.
C    24OCT81  Alex Hunter  1. Replicate serial no field of atom. (V5.6)
C
C***********************************************************************
      INTEGER*2 FUNCTION REPLICA(NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD=NODX
 
      IF (NOD.EQ.NULL.OR.CONSTANT(NOD).OR.REGISTER(NOD)) THEN
         REPLICA=NOD
 
      ELSEIF (ATOM(NOD)) THEN
         REPLICA=MAKE_ATOM(ATOM_SYM(NOD),ATOM_MEM(NOD),ATOM_BASE(NOD),
     #                     ATOM_SUB(NOD),NODE_TYPE(NOD),NODE_REG(NOD),
     #                     NODE_REFCT(NOD))
         ATOM_DISP(REPLICA)=ATOM_DISP(NOD)
         ATOM_FLAGS(REPLICA)=ATOM_FLAGS(NOD)
         ATOM_SERIAL_NO(REPLICA)=ATOM_SERIAL_NO(NOD)
 
      ELSEIF (FIXLIT(NOD)) THEN
         REPLICA=MAKE_FIXED(FIXED_VAL(NOD),NODE_TYPE(NOD))
 
      ELSEIF (FLOATLIT(NOD)) THEN
         REPLICA=MAKE_FLOAT(FLOAT_VAL(NOD),NODE_TYPE(NOD))
 
      ELSEIF (NODE(NOD)) THEN
 
         CALL PUSH(NOD,1)
         OPND1=REPLICA2(OPNODE_OPND1(NOD))
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL PUSH(OPND1,1)
         OPND2=REPLICA2(OPNODE_OPND2(NOD))
         CALL POP(OPND1,1)
         CALL POP(NOD,1)
 
         REPLICA=MAKE_NODE(OPNODE_OP(NOD),OPND1,OPND2,NODE_TYPE(NOD),
     #                     NODE_REG(NOD),NODE_REFCT(NOD))
 
      ELSE
         CALL BUG('REP-0')
      ENDIF
 
      RETURN
      END
C-------------------------------------------------------------------
      INTEGER*2 FUNCTION REPLICA2(NODX)
      IMPLICIT INTEGER*2 (A-Z)
      REPLICA2=REPLICA(NODX)
      RETURN
      END
