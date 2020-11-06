C***********************************************************************
C
C                               CONTEXT.FOR
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
C    This module of the PL/M-VAX compiler resolves the signed/unsigned
C    context for all the nodes of a code tree, and performs any implicit
C    context coercions required.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Add OP_SIGNED and OP_UNSIGNED. (V5.5)
C
C-----------------------------------------------------------------------
      SUBROUTINE RESOLVE_CONTEXT(NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD=NODX
 
      IF (NOD.EQ.NULL) RETURN
      IF (CONSTANT(NOD)) RETURN
      IF (LITERAL(NOD)) RETURN
      IF (REGISTER(NOD)) RETURN
 
      IF (ATOM(NOD)) THEN
         CALL PUSH(NOD,1)
         CALL RESOLVE_CONTEXT2(ATOM_BASE(NOD))
         CALL POP(NOD,1)
         IF (NODE_CONTEXT(ATOM_BASE(NOD)).EQ.0)
     #         CALL SET_CONTEXT(ATOM_BASE(NOD),CX_UNSIGNED)
         CALL PUSH(NOD,1)
         CALL RESOLVE_CONTEXT2(ATOM_SUB(NOD))
         CALL POP(NOD,1)
         IF (NODE_CONTEXT(ATOM_SUB(NOD)).EQ.0)
     #         CALL SET_CONTEXT(ATOM_SUB(NOD),CX_UNSIGNED)
         RETURN
      ENDIF
 
      CALL PUSH(NOD,1)
      CALL RESOLVE_CONTEXT2(OPNODE_OPND1(NOD))
      CALL POP(NOD,1)
      CALL PUSH(NOD,1)
      CALL RESOLVE_CONTEXT2(OPNODE_OPND2(NOD))
      CALL POP(NOD,1)
 
      IF (OPNODE_OPND1(NOD).EQ.NULL) THEN
         NODE_CONTEXT(NOD)=NODE_CONTEXT(OPNODE_OPND2(NOD))
         RETURN
 
      ELSEIF (OPNODE_OP(NOD).GT.80.AND.OPNODE_OP(NOD).LT.100) THEN
         NODE_CONTEXT(NOD)=CONTEXT(OPNODE_OP(NOD)-80)
         IF (NODE_CONTEXT(OPNODE_OPND1(NOD)).EQ.0) THEN
            CALL SET_CONTEXT(OPNODE_OPND1(NOD),NODE_CONTEXT(NOD))
         ENDIF
         RETURN
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_SIGNED) THEN
         NODE_CONTEXT(NOD)=CX_SIGNED
         IF (NODE_CONTEXT(OPNODE_OPND1(NOD)).EQ.0) THEN
            CALL SET_CONTEXT(OPNODE_OPND1(NOD),CX_SIGNED)
         ENDIF
         OPNODE_OP(NOD)=OP_NOP
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_UNSIGNED) THEN
         NODE_CONTEXT(NOD)=CX_UNSIGNED
         IF (NODE_CONTEXT(OPNODE_OPND1(NOD)).EQ.0) THEN
            CALL SET_CONTEXT(OPNODE_OPND1(NOD),CX_UNSIGNED)
         ENDIF
         OPNODE_OP(NOD)=OP_NOP
 
      ELSEIF (OPNODE_OPND2(NOD).EQ.NULL.OR.OPNODE_OP(NOD).EQ.OP_CALL)
     #   THEN
         NODE_CONTEXT(NOD)=NODE_CONTEXT(OPNODE_OPND1(NOD))
         RETURN
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_ARG) THEN
         IF (NODE_CONTEXT(OPNODE_OPND2(NOD)).EQ.0) THEN
            CALL SET_CONTEXT(OPNODE_OPND2(NOD),CX_SIGNED)   !DEBATABLE.
         ENDIF
         RETURN
      ENDIF
 
      IF (NODE_CONTEXT(OPNODE_OPND1(NOD)).EQ.0) THEN
         NODE_CONTEXT(NOD)=NODE_CONTEXT(OPNODE_OPND2(NOD))
      ELSE
         NODE_CONTEXT(NOD)=NODE_CONTEXT(OPNODE_OPND1(NOD))
      ENDIF
 
      IF (NODE_CONTEXT(NOD).EQ.0) RETURN
 
      IF (NODE_CONTEXT(OPNODE_OPND1(NOD)).EQ.0) THEN
         CALL SET_CONTEXT(OPNODE_OPND1(NOD),NODE_CONTEXT(NOD))
      ELSEIF (NODE_CONTEXT(OPNODE_OPND2(NOD)).EQ.0) THEN
         CALL SET_CONTEXT(OPNODE_OPND2(NOD),NODE_CONTEXT(NOD))
      ENDIF
      RETURN
      END
C--------------------------------------------------------------
      SUBROUTINE RESOLVE_CONTEXT2(NODX)
      CALL RESOLVE_CONTEXT(NODX)
      RETURN
      END
C--------------------------------------------------------------
      SUBROUTINE SET_CONTEXT(NODX,CNTXTX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD=NODX
      CNTXT=CNTXTX
 
10    IF (NOD.EQ.NULL) RETURN
      NODE_CONTEXT(NOD)=CNTXT
      IF (.NOT. NODE(NOD)) RETURN
      CALL PUSH(NOD,1)
      CALL SET_CONTEXT2(OPNODE_OPND1(NOD),CNTXT)
      CALL POP(NOD,1)
      NOD=OPNODE_OPND2(NOD)
      GO TO 10
      END
C--------------------------------------------------------------
      SUBROUTINE SET_CONTEXT2(NODX,CNTXTX)
      CALL SET_CONTEXT(NODX,CNTXTX)
      RETURN
      END
