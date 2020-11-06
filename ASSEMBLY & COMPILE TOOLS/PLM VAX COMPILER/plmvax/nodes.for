C***********************************************************************
C
C                               NODES.FOR
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
C    This module of the PL/M-VAX compiler contains routines to create
C    nodes of a code tree, and to determine the type of a node.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C    10NOV81  Alex Hunter  1. Add serial no. deltas. (6.0)
C
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION MAKE_NODE(OP,OPND1,OPND2,TYPE,REG,REFCT)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      REAL*8 RVAL
      INTEGER*4 IVAL,IVAL1
      IF (NEXT_NODE.GT.NODE_MAX) CALL FATAL('NODE TABLE OVERFLOW')
      OPNODE_OP(NEXT_NODE)=OP
      OPNODE_OPND1(NEXT_NODE)=OPND1
      OPNODE_OPND2(NEXT_NODE)=OPND2
      NODE_TYPE(NEXT_NODE)=TYPE
      NODE_REG(NEXT_NODE)=REG
      NODE_REFCT(NEXT_NODE)=0
      NODE_CONTEXT(NEXT_NODE)=0
      MAKE_NODE=NEXT_NODE
      NEXT_NODE=NEXT_NODE+1
      RETURN
C---------------------------------------
      ENTRY MAKE_ATOM(SYM,MEM,BASE,SUBSCRIPT,TYPE,REG,REFCT)
C---------------------------------------
      IF (NEXT_ATOM.GT.ATOM_MAX) CALL FATAL('ATOM TABLE OVERFLOW')
      ATOM_SYM(NEXT_ATOM)=SYM
      ATOM_MEM(NEXT_ATOM)=MEM
      ATOM_BASE(NEXT_ATOM)=BASE
      ATOM_SUB(NEXT_ATOM)=SUBSCRIPT
      ATOM_DISP(NEXT_ATOM)=0
      ATOM_FLAGS(NEXT_ATOM)=0
      NODE_TYPE(NEXT_ATOM)=TYPE
      NODE_REG(NEXT_ATOM)=REG
      NODE_REFCT(NEXT_ATOM)=0
      NODE_CONTEXT(NEXT_ATOM)=CONTEXT(TYPE)
      IF (MEM.NE.0) THEN
         ATOM_SERIAL_NO(NEXT_ATOM)=MEMBER_SERIAL_NO(MEM)
      ELSEIF (SYM.NE.0) THEN
         ATOM_SERIAL_NO(NEXT_ATOM)=SYMBOL_SERIAL_NO(SYM)
      ELSE
         ATOM_SERIAL_NO(NEXT_ATOM)=-1
      ENDIF
      IF (SYM.NE.0.AND.SYMBOL_REF(SYM).EQ.S_EXT) THEN
         ATOM_SERIAL_NO(NEXT_ATOM) =
     #      ATOM_SERIAL_NO(NEXT_ATOM) + EXTERNAL_SERIAL_DELTA
      ENDIF
      IF (BASE.NE.NULL) THEN
         ATOM_SERIAL_NO(NEXT_ATOM) =
     #      ATOM_SERIAL_NO(NEXT_ATOM) + BASED_SERIAL_DELTA
      ENDIF
      IF (SUBSCRIPT.NE.NULL) THEN
         ATOM_SERIAL_NO(NEXT_ATOM) =
     #      ATOM_SERIAL_NO(NEXT_ATOM) + SUBSCRIPTED_SERIAL_DELTA
      ENDIF
      IF ((SYMBOL_FLAGS(SYM).AND.S_OVERLAID).NE.0) THEN
         ATOM_SERIAL_NO(NEXT_ATOM) =
     #      ATOM_SERIAL_NO(NEXT_ATOM) + OVERLAID_SERIAL_DELTA
      ENDIF
      MAKE_ATOM=NEXT_ATOM
      NEXT_ATOM=NEXT_ATOM+1
      RETURN
C---------------------------------------
      ENTRY MAKE_FIXED(IVAL,TYPE)
C---------------------------------------
      IVAL1=IVAL
      GO TO 10
C---------------------------------------
      ENTRY MAKE_FIXED2(I2VAL,TYPE)
C---------------------------------------
      IVAL1=I2VAL
10    IF (NEXT_FIXED.GT.FIX_MAX)
     #    CALL FATAL('FIXED POINT LITERAL TABLE OVERFLOW')
      FIXED_VAL(NEXT_FIXED)=IVAL1
      NODE_TYPE(NEXT_FIXED)=TYPE
      NODE_REG(NEXT_FIXED)=0
      NODE_REFCT(NEXT_FIXED)=0
      IF (TYPE.EQ.0) THEN
         NODE_CONTEXT(NEXT_FIXED)=0
      ELSEIF (TYPE.EQ.S_INTEGER) THEN
         NODE_CONTEXT(NEXT_FIXED)=CX_SIGNED
      ELSE
         NODE_CONTEXT(NEXT_FIXED)=CX_UNSIGNED
      ENDIF
      MAKE_FIXED=NEXT_FIXED
      NEXT_FIXED=NEXT_FIXED+1
      RETURN
C---------------------------------------
      ENTRY MAKE_FLOAT(RVAL,TYPE)
C---------------------------------------
      IF (NEXT_FLOAT.GT.FLT_MAX) 
     #    CALL FATAL('FLOATING POINT LITERAL TABLE OVERFLOW')
      FLOAT_VAL(NEXT_FLOAT)=RVAL
      NODE_TYPE(NEXT_FLOAT)=TYPE
      NODE_REG(NEXT_FLOAT)=0
      NODE_REFCT(NEXT_FLOAT)=0
      NODE_CONTEXT(NEXT_FLOAT)=CX_SIGNED
      MAKE_FLOAT=NEXT_FLOAT
      NEXT_FLOAT=NEXT_FLOAT+1
      RETURN
C---------------------------------------
      ENTRY MAKE_CONSTANT(LL,TYPE)
C---------------------------------------
      IF (NEXT_CONSTANT.GT.CON_MAX)
     #   CALL FATAL('CONSTANT TABLE OVERFLOW')
      CONSTANT_LABEL(NEXT_CONSTANT)=LL
      NODE_TYPE(NEXT_CONSTANT)=TYPE
      NODE_REG(NEXT_CONSTANT)=0
      NODE_REFCT(NEXT_CONSTANT)=0
      NODE_CONTEXT(NEXT_CONSTANT)=CONTEXT(TYPE)
      MAKE_CONSTANT=NEXT_CONSTANT
      NEXT_CONSTANT=NEXT_CONSTANT+1
      RETURN
C---------------------------------------
      ENTRY MAKE_REGISTER(REG,TYPE)
C---------------------------------------
      NODE_TYPE(REG)=TYPE
      NODE_REG(REG)=REG
      NODE_REFCT(REG)=0
      NODE_CONTEXT(REG)=CONTEXT(TYPE)
      MAKE_REGISTER=REG
      RETURN
      END
C--------------------------------------------------------------
      FUNCTION NODE(LINK)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      NODE=LINK.GE.NODE_MIN.AND.LINK.LE.NODE_MAX
      RETURN
C---------------------------------------
      ENTRY ATOM(LINK)
C---------------------------------------
      ATOM=LINK.GE.ATOM_MIN.AND.LINK.LE.ATOM_MAX
      RETURN
C---------------------------------------
      ENTRY LITERAL(LINK)
C---------------------------------------
      LITERAL=LINK.GE.FIX_MIN.AND.LINK.LE.FIX_MAX.OR.
     #        LINK.GE.FLT_MIN.AND.LINK.LE.FLT_MAX
      RETURN
C---------------------------------------
      ENTRY FIXLIT(LINK)
C---------------------------------------
      FIXLIT=LINK.GE.FIX_MIN.AND.LINK.LE.FIX_MAX
      RETURN
C---------------------------------------
      ENTRY FLOATLIT(LINK)
C---------------------------------------
      FLOATLIT=LINK.GE.FLT_MIN.AND.LINK.LE.FLT_MAX
      RETURN
C---------------------------------------
      ENTRY CONSTANT(LINK)
C---------------------------------------
      CONSTANT=LINK.GE.CON_MIN.AND.LINK.LE.CON_MAX
      RETURN
C---------------------------------------
      ENTRY REGISTER(LINK)
C---------------------------------------
      REGISTER=LINK.GE.REG_MIN.AND.LINK.LE.REG_MAX
      RETURN
      END
