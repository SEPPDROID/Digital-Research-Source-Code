C***********************************************************************
C
C                               MERGE.FOR
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
C    This module of the PL/M-VAX compiler is used to merge identical
C    nodes of a code tree, which effectively eliminates common sub-
C    expressions.  Note that after 'merging' a code tree, the code
C    'tree' is no longer necessarily a tree, but rather a directed
C    acyclic graph.  This means that the code 'tree' may no longer be
C    traversed without some form of 'node marking' to detect already-
C    visited nodes.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    18OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C    09NOV81  Alex Hunter  1. Implement CSE assumption. (V5.9)
C    14NOV81  Alex Hunter  1. Don't merge certain opnode ops. (V6.2)
C    08FEB82  Alex Hunter  1. Do want to merge ARG opnodes. (V6.7)
C
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION MERGE(NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD=NODX
 
      IF (.NOT.ASSUME_CSE.OR.NOD.EQ.NULL.OR.CONSTANT(NOD).OR.
     #    REGISTER(NOD)) THEN
         MERGE=NOD
         RETURN
      ENDIF
 
      IF (FIXLIT(NOD)) THEN
         DO 100 MERGE=FIX_MIN,NEXT_FIXED-1
         IF (FIXED_VAL(MERGE).EQ.FIXED_VAL(NOD).AND.
     #       NODE_TYPE(MERGE).EQ.NODE_TYPE(NOD)) RETURN
100      CONTINUE
         CALL BUG('MERGE-100')
 
      ELSEIF (FLOATLIT(NOD)) THEN
         DO 200 MERGE=FLT_MIN,NEXT_FLOAT-1
         IF (FLOAT_VAL(MERGE).EQ.FLOAT_VAL(NOD).AND.
     #       NODE_TYPE(MERGE).EQ.NODE_TYPE(NOD)) RETURN
200      CONTINUE
         CALL BUG('MERGE-200')
 
      ELSEIF (ATOM(NOD)) THEN
         IF (NOD.LT.FIRST_FREE_ATOM) THEN
            MERGE=NOD
            RETURN
         ENDIF
         CALL PUSH(NOD,1)
         BASE=MERGE2(ATOM_BASE(NOD))
         CALL POP(NOD,1)
         CALL PUSH(NOD,1)
         CALL PUSH(BASE,1)
         SUB=MERGE2(ATOM_SUB(NOD))
         CALL POP(BASE,1)
         CALL POP(NOD,1)
 
         DO 300 MERGE=FIRST_FREE_ATOM,NEXT_ATOM-1
         IF (ATOM_SYM(MERGE).EQ.ATOM_SYM(NOD).AND.
     #       ATOM_MEM(MERGE).EQ.ATOM_MEM(NOD).AND.
     #       ATOM_BASE(MERGE).EQ.BASE.AND.
     #       ATOM_SUB(MERGE).EQ.SUB.AND.
     #       ATOM_FLAGS(MERGE).EQ.ATOM_FLAGS(NOD).AND.
     #       ATOM_SERIAL_NO(MERGE).EQ.ATOM_SERIAL_NO(NOD).AND.
     #       ATOM_DISP(MERGE).EQ.ATOM_DISP(NOD)) RETURN
CCCC #       NODE_TYPE(MERGE).EQ.NODE_TYPE(NOD)) RETURN
300      CONTINUE
 
         ATOM_BASE(NOD)=BASE
         ATOM_SUB(NOD)=SUB
         MERGE=NOD
         RETURN
      ENDIF
 
C--------- NODE MUST BE AN OPNODE.
 
      CALL PUSH(NOD,1)
      OPND1=MERGE2(OPNODE_OPND1(NOD))
      CALL POP(NOD,1)
      CALL PUSH(NOD,1)
      CALL PUSH(OPND1,1)
      OPND2=MERGE2(OPNODE_OPND2(NOD))
      CALL POP(OPND1,1)
      CALL POP(NOD,1)
 
      IF (OPNODE_OP(NOD).NE.OP_MOV.AND.
     #    OPNODE_OP(NOD).NE.OP_ASSN.AND.
     #    OPNODE_OP(NOD).NE.OP_THEN.AND.
     #    OPNODE_OP(NOD).NE.OP_ALSO)
     #THEN
         DO MERGE=NODE_MIN,NEXT_NODE-1
            IF (OPNODE_OP(MERGE).EQ.OPNODE_OP(NOD).AND.
     #          OPNODE_OPND1(MERGE).EQ.OPND1.AND.
     #          OPNODE_OPND2(MERGE).EQ.OPND2.AND.
     #          NODE_TYPE(MERGE).EQ.NODE_TYPE(NOD)) RETURN
         ENDDO
      ENDIF
      
      OPNODE_OPND1(NOD)=OPND1
      OPNODE_OPND2(NOD)=OPND2
      MERGE=NOD
      RETURN
      END
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION MERGE2(NODX)
      IMPLICIT INTEGER*2 (A-Z)
      MERGE2=MERGE(NODX)
      RETURN
      END
