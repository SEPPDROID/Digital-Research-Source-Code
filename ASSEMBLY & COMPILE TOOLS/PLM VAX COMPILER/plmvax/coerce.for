C***********************************************************************
C
C                               COERCE.FOR
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
C    This module of the PL/M-VAX compiler coerces nodes of a code
C    tree to the proper type, according to the implicit type coercion
C    rules.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C
C-----------------------------------------------------------------------
      SUBROUTINE COERCE_TYPES(NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*2 CVT_TYPE(OP_B2W:OP_P2L)
      DATA CVT_TYPE/
     #    S_WORD,S_INTEGER,   S_LONG,   S_REAL,   S_BYTE,
     #    S_LONG,   S_BYTE,   S_REAL,   S_LONG,   S_LONG,
     # S_INTEGER,   S_WORD,   S_REAL,   S_BYTE,   S_BYTE,
     # S_INTEGER, S_DOUBLE,   S_QUAD, S_DOUBLE,   S_BYTE,
     # S_INTEGER,   S_REAL,   S_LONG,   S_LONG, S_DOUBLE,
     #     S_PTR,   S_LONG/
      INTEGER*2 MUL_TYPE(1:7,1:7)
      DATA MUL_TYPE
     // S_WORD,S_LONG,S_INTEGER,0,S_REAL,S_LONG,S_DOUBLE
     ,, S_LONG,S_LONG,S_LONG,0,S_REAL,S_LONG,S_DOUBLE
     ,, S_INTEGER,S_LONG,S_INTEGER,0,S_REAL,S_LONG,S_DOUBLE
     ,, 0,0,0,0,0,0,0
     ,, S_REAL,S_REAL,S_REAL,0,S_REAL,S_DOUBLE,S_DOUBLE
     ,, S_LONG,S_LONG,S_LONG,0,S_DOUBLE,S_LONG,S_DOUBLE
     ,, S_DOUBLE,S_DOUBLE,S_DOUBLE,0,S_DOUBLE,S_DOUBLE,S_DOUBLE
     //
      INTEGER*2 ADD_TYPE(1:7,1:7)
      DATA ADD_TYPE
     // S_BYTE,S_WORD,S_INTEGER,S_PTR,S_REAL,S_LONG,S_DOUBLE
     ,, S_WORD,S_WORD,S_INTEGER,S_PTR,S_REAL,S_LONG,S_DOUBLE
     ,, S_INTEGER,S_INTEGER,S_INTEGER,S_PTR,S_REAL,S_LONG,S_DOUBLE
     ,, S_PTR,S_PTR,S_PTR,0,0,S_PTR,0
     ,, S_REAL,S_REAL,S_REAL,0,S_REAL,S_DOUBLE,S_DOUBLE
     ,, S_LONG,S_LONG,S_LONG,S_PTR,S_DOUBLE,S_LONG,S_DOUBLE
     ,, S_DOUBLE,S_DOUBLE,S_DOUBLE,0,S_DOUBLE,S_DOUBLE,S_DOUBLE
     //
      INTEGER*2 OPND_TYPE(1:7,1:7)
      DATA OPND_TYPE
     // S_BYTE,S_WORD,S_INTEGER,S_PTR,S_REAL,S_LONG,S_DOUBLE
     ,, S_WORD,S_WORD,S_INTEGER,S_PTR,S_REAL,S_LONG,S_DOUBLE
     ,, S_INTEGER,S_INTEGER,S_INTEGER,S_PTR,S_REAL,S_LONG,S_DOUBLE
     ,, S_LONG,S_LONG,S_LONG,0,0,S_LONG,0
     ,, S_REAL,S_REAL,S_REAL,0,S_REAL,S_DOUBLE,S_DOUBLE
     ,, S_LONG,S_LONG,S_LONG,S_PTR,S_DOUBLE,S_LONG,S_DOUBLE
     ,, S_DOUBLE,S_DOUBLE,S_DOUBLE,0,S_DOUBLE,S_DOUBLE,S_DOUBLE
     //
 
      NOD=NODX
 
      IF (NOD.EQ.NULL) RETURN
 
      IF (CONSTANT(NOD)) RETURN
 
      IF (REGISTER(NOD)) RETURN
 
      IF (FLOATLIT(NOD)) THEN
         RETURN
 
      ELSEIF (FIXLIT(NOD)) THEN
         IF (NODE_TYPE(NOD).EQ.0) THEN
            IF (NODE_CONTEXT(NOD).EQ.CX_SIGNED) THEN
               NODE_TYPE(NOD)=S_INTEGER
            ELSEIF (FIXED_VAL(NOD).GE.0.AND.FIXED_VAL(NOD).LE.255) THEN
               NODE_TYPE(NOD)=S_BYTE
            ELSE
               NODE_TYPE(NOD)=S_WORD
            ENDIF
         ENDIF
         RETURN
 
      ELSEIF (ATOM(NOD)) THEN
         
         CALL PUSH(NOD,1)
         CALL COERCE_TYPES2(ATOM_BASE(NOD))
         CALL POP(NOD,1)
         ATOM_BASE(NOD)=FORCE_TYPE(ATOM_BASE(NOD),S_PTR)
 
         CALL PUSH(NOD,1)
         CALL COERCE_TYPES2(ATOM_SUB(NOD))
         CALL POP(NOD,1)
         ATOM_SUB(NOD)=FORCE_TYPE(ATOM_SUB(NOD),S_LONG)
         RETURN
      ENDIF
 
C ----  NODE IS AN OPNODE.
 
      CALL PUSH(NOD,1)
      CALL COERCE_TYPES2(OPNODE_OPND1(NOD))
      CALL POP(NOD,1)
 
      CALL PUSH(NOD,1)
      CALL COERCE_TYPES2(OPNODE_OPND2(NOD))
      CALL POP(NOD,1)
 
      IF (OPNODE_OP(NOD).EQ.OP_ASSN.OR.OPNODE_OP(NOD).EQ.OP_MOV) THEN
         NODE_TYPE(NOD)=NODE_TYPE(OPNODE_OPND2(NOD))
         OPNODE_OPND1(NOD)=FORCE_TYPE(OPNODE_OPND1(NOD),NODE_TYPE(NOD))
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_LOC) THEN
         NODE_TYPE(NOD)=S_PTR
 
      ELSEIF (OPNODE_OP(NOD).GT.100) THEN
         NODE_TYPE(NOD)=CVT_TYPE(OPNODE_OP(NOD))
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_CALL) THEN
         NODE_TYPE(NOD)=NODE_TYPE(OPNODE_OPND1(NOD))
         
      ELSEIF (OPNODE_OP(NOD).EQ.OP_ARG) THEN
         IF (BYTE_SIZE(NODE_TYPE(OPNODE_OPND2(NOD))).EQ.4) THEN
            NODE_TYPE(NOD)=NODE_TYPE(OPNODE_OPND2(NOD))
         ELSE
            NODE_TYPE(NOD)=S_LONG
            OPNODE_OPND2(NOD)=FORCE_TYPE(OPNODE_OPND2(NOD),S_LONG)
         ENDIF
 
      ELSEIF (OPNODE_OP(NOD).GT.80.AND.OPNODE_OP(NOD).LT.100) THEN
         NODE_TYPE(NOD)=OPNODE_OP(NOD)-80
         OPNODE_OPND1(NOD)=FORCE_TYPE(OPNODE_OPND1(NOD),NODE_TYPE(NOD))
         OPNODE_OP(NOD)=OP_NOP
 
      ELSEIF (OPNODE_OP(NOD).EQ.OP_THEN.OR.OPNODE_OP(NOD).EQ.OP_ALSO) 
     #   THEN
         RETURN
 
      ELSEIF (OPNODE_OPND2(NOD).EQ.NULL) THEN
         NODE_TYPE(NOD)=NODE_TYPE(OPNODE_OPND1(NOD))
 
      ELSE
         IF (OPNODE_OP(NOD).EQ.OP_MUL.OR.OPNODE_OP(NOD).EQ.OP_DIV) THEN
            NODE_TYPE(NOD)=MUL_TYPE(NODE_TYPE(OPNODE_OPND1(NOD)),
     #                              NODE_TYPE(OPNODE_OPND2(NOD)))
            OPND1_TYPE=NODE_TYPE(NOD)
            OPND2_TYPE=NODE_TYPE(NOD)
         ELSEIF (OPNODE_OP(NOD).EQ.OP_ADWC.OR.OPNODE_OP(NOD).EQ.OP_SBWC)
     #      THEN
            NODE_TYPE(NOD)=S_LONG
            OPND1_TYPE=S_LONG
            OPND2_TYPE=S_LONG
         ELSEIF (OPNODE_OP(NOD).EQ.OP_MOD) THEN
            NODE_TYPE(NOD)=S_LONG
            OPND1_TYPE=S_QUAD
            OPND2_TYPE=S_LONG
         ELSE
            NODE_TYPE(NOD)=ADD_TYPE(NODE_TYPE(OPNODE_OPND1(NOD)),
     #                              NODE_TYPE(OPNODE_OPND2(NOD)))
            OPND1_TYPE=OPND_TYPE(NODE_TYPE(OPNODE_OPND1(NOD)),
     #                           NODE_TYPE(OPNODE_OPND2(NOD)))
            OPND2_TYPE=OPND_TYPE(NODE_TYPE(OPNODE_OPND2(NOD)),
     #                           NODE_TYPE(OPNODE_OPND1(NOD)))
         ENDIF
         IF (NODE_TYPE(NOD).EQ.0) THEN
            CALL WARN('ILLEGAL MIXING OF TYPES')
            NODE_TYPE(NOD)=NODE_TYPE(OPNODE_OPND1(NOD))
            OPND1_TYPE=NODE_TYPE(OPNODE_OPND1(NOD))
            OPND2_TYPE=NODE_TYPE(OPNODE_OPND2(NOD))
         ENDIF
         OPNODE_OPND1(NOD)=FORCE_TYPE(OPNODE_OPND1(NOD),OPND1_TYPE)
         OPNODE_OPND2(NOD)=FORCE_TYPE(OPNODE_OPND2(NOD),OPND2_TYPE)
         IF (OPNODE_OP(NOD).GE.OP_LT.AND.OPNODE_OP(NOD).LE.OP_GE) THEN
            NODE_TYPE(NOD)=S_BYTE
         ELSEIF (OPNODE_OP(NOD).EQ.OP_AND) THEN
            OPNODE_OP(NOD)=OP_EXT
            NEW_OPND2=MAKE_NODE(OP_NOT,OPNODE_OPND2(NOD),NULL,0,0,0)
            NODE_TYPE(NEW_OPND2)=OPND2_TYPE
            NODE_CONTEXT(NEW_OPND2)=NODE_CONTEXT(OPNODE_OPND2(NOD))
            OPNODE_OPND2(NOD)=NEW_OPND2
         ENDIF
      ENDIF
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE COERCE_TYPES2(NODX)
      CALL COERCE_TYPES(NODX)
      RETURN
      END
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION FORCE_TYPE(NODX,TYPEX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      NOD=NODX
      TYPE=TYPEX
 
      IF (NOD.EQ.NULL.OR.NODE_TYPE(NOD).EQ.TYPE) THEN
         FORCE_TYPE=NOD
         RETURN
      ENDIF
 
      GOTO (1000,2000,3000,4000,5000,6000,70000,80000), NODE_TYPE(NOD)
      CALL BUG('FT-0')
 
1000  GOTO (9000,1200,1300,1400,1500,1600,1700,1800), TYPE
      CALL BUG('FT-1')
1200  OP=OP_B2W
      GOTO 8000
1300  OP=OP_B2I
      GOTO 8000
1400  OP1=OP_B2L
      OP2=OP_L2P
      GOTO 7000
1500  OP1=OP_B2L
      OP2=OP_L2R
      GOTO 7000
1600  OP=OP_B2L
      GOTO 8000
1700  OP1=OP_B2L
      OP2=OP_L2D
      GO TO  7000
1800  OP1=OP_B2L
      OP2=OP_L2Q
      GO TO 7000
 
2000  GOTO (2100,9000,9000,2400,2500,2600,2700,2800), TYPE
      CALL BUG('FT-2')
2100  OP=OP_W2B
      GOTO 8000
2400  OP1=OP_W2L
      OP2=OP_L2P
      GOTO 7000
2500  OP1=OP_W2L
      OP2=OP_L2R
      GOTO 7000
2600  OP=OP_W2L
      GOTO 8000
2700  OP1=OP_W2L
      OP2=OP_L2D
      GO TO 7000
2800  OP1=OP_W2L
      OP2=OP_L2Q
      GO TO 7000
 
3000  GOTO (3100,9000,9000,3400,3500,3600,3700,3800), TYPE
      CALL BUG('FT-3')
3100  OP=OP_I2B
      GOTO 8000
3400  OP1=OP_I2L
      OP2=OP_L2P
      GOTO 7000
3500  OP=OP_I2R
      GOTO 8000
3600  OP=OP_I2L
      GOTO 8000
3700  OP=OP_I2D
      GO TO 8000
3800  OP1=OP_I2L
      OP2=OP_L2Q
      GO TO 7000
 
4000  GOTO (4100,4200,4300,9000,8500,4600,8500,4800), TYPE
      CALL BUG('FT-4')
4100  OP1=OP_P2L
      OP2=OP_L2B
      GOTO 7000
4200  CONTINUE
4300  OP1=OP_P2L
      OP2=OP_L2W
      GOTO 7000
4600  OP=OP_P2L
      GOTO 8000
4800  OP1=OP_P2L
      OP2=OP_L2Q
      GOTO 7000
 
5000  GOTO (5100,5200,5300,8500,9000,5600,5700,5800), TYPE
      CALL BUG('FT-5')
5100  OP=OP_R2B
      GOTO 8000
5200  OP=OP_R2W
      GOTO 8000
5300  OP=OP_R2I
      GOTO 8000
5600  OP=OP_R2L
      GOTO 8000
5700  OP=OP_R2D
      GO TO 8000
5800  OP1=OP_R2L
      OP2=OP_L2Q
      GO TO 7000
 
6000  GOTO (6100,6200,6300,6400,6500,9000,6700,6800), TYPE
      CALL BUG('FT-6')
6100  OP=OP_L2B
      GOTO 8000
6200  CONTINUE
6300  OP=OP_L2W
      GOTO 8000
6400  OP=OP_L2P
      GOTO 8000
6500  OP=OP_L2R
      GOTO 8000
6700  OP=OP_L2D
      GO TO 8000
6800  OP=OP_L2Q
      GO TO 8000
 
70000 GOTO (71000,72000,73000,8500,75000,76000,9000,78000), TYPE
      CALL BUG('FT-7')
71000 OP=OP_D2B
      GOTO 8000
72000 OP=OP_D2I
      GO TO 8000
73000 OP=OP_D2I
      GO TO 8000
75000 OP=OP_D2R
      GO TO 8000
76000 OP=OP_D2L
      GO TO 8000
78000 OP1=OP_D2L
      OP2=OP_L2Q
      GO TO 8000
 
80000 GOTO (81000,82000,83000,84000,85000,86000,87000,9000), TYPE
      CALL BUG('FT-8')
81000 OP2=OP_L2B
      GO TO 80999
82000 CONTINUE
83000 OP2=OP_L2W
      GO TO 80999
84000 OP2=OP_L2P
      GO TO 80999
85000 OP2=OP_L2R
      GO TO 80999
86000 OP=OP_Q2L
      GO TO 8000
87000 OP2=OP_L2D
80999 OP1=OP_Q2L
      GO TO 7000
 
7000  FORCE_TYPE=MAKE_NODE(OP2,MAKE_NODE(OP1,NOD,NULL,S_LONG,0,0),
     #                      NULL,TYPE,0,0)
      RETURN
 
8000  FORCE_TYPE=MAKE_NODE(OP,NOD,NULL,TYPE,0,0)
      RETURN
 
8500  CALL WARN('ILLEGAL TYPE CONVERSION')
 
9000  NODE_TYPE(NOD)=TYPE
      FORCE_TYPE=NOD
      RETURN
      END
