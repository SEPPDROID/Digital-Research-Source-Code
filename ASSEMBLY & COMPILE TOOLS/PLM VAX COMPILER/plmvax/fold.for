C***********************************************************************
C
C                               FOLD.FOR
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
C    This module of the PL/M-VAX compiler examines a code tree and
C    folds operator nodes having all constant operands.  Some binary
C    operator nodes having one constant operand are also simplified.
C    Constant displacements within atom base and subscript subtrees
C    are extracted and incorporated into the atom's displacement
C    field.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    09NOV81  Alex Hunter  1. Implement CTE assumption. (V5.9)
C
C-----------------------------------------------------------------------
C!!!!!   COMPILE ME WITH  /NOCHECK  PLEASE!!!!!!!!!
C
      INTEGER*2 FUNCTION FOLD_CONSTANTS(NODX)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 I,I1,I2
      REAL*8 R,R1,R2
      INTEGER*4 MASK(S_BYTE:S_QUAD)
      DATA MASK/'FF'X,'FFFF'X,'FFFFFFFF'X,'FFFFFFFF'X,
     #          'FFFFFFFF'X,'FFFFFFFF'X,'FFFFFFFF'X,'FFFFFFFF'X/
   
      NOD=NODX
 
1     IF (NOD.EQ.NULL) GO TO 9000
 
      IF (LITERAL(NOD)) GO TO 9000
 
      IF (CONSTANT(NOD)) GO TO 9000
 
      IF (REGISTER(NOD)) GO TO 9000
 
      IF (ATOM(NOD)) THEN
         CALL PUSH(NOD,1)
         BASE=FOLD_CONSTANTS2(ATOM_BASE(NOD))
         CALL POP(NOD,1)
         ATOM_BASE(NOD)=BASE
 
         CALL PUSH(NOD,1)
         CALL PUSH(BASE,1)
         SUB=FOLD_CONSTANTS2(ATOM_SUB(NOD))
         CALL POP(BASE,1)
         CALL POP(NOD,1)
         ATOM_SUB(NOD)=SUB
 
         IF (NODE(BASE).AND.OPNODE_OP(BASE).EQ.OP_L2P) THEN
            ATOM_FLAGS(NOD)=ATOM_FLAGS(NOD).OR.A_L2P
            ATOM_BASE(NOD)=OPNODE_OPND1(BASE)
         ENDIF
 
         ELEMENT_SIZE=BYTE_SIZE(NODE_TYPE(NOD))
 
         NOD1=ATOM_SUB(NOD)
         IF (NODE(NOD1).AND.OPNODE_OP(NOD1).GT.100)
     #         NOD1=OPNODE_OPND1(NOD1)
 
         IF (NODE(NOD1).AND.OPNODE_OP(NOD1).EQ.OP_MUL.AND.
     #       FIXLIT(OPNODE_OPND2(NOD1))) THEN
            FACTOR=FIXED_VAL(OPNODE_OPND2(NOD1))
            OPNODE_OPND1(NOD1)=EXTRACT_DISPLACEMENT(OPNODE_OPND1(NOD1)
     #                                               ,DISP)
            ATOM_DISP(NOD)=ATOM_DISP(NOD)+DISP*FACTOR*ELEMENT_SIZE
         ELSE
            ATOM_SUB(NOD)=EXTRACT_DISPLACEMENT(ATOM_SUB(NOD),DISP)
            ATOM_DISP(NOD)=ATOM_DISP(NOD)+DISP*ELEMENT_SIZE
         ENDIF
 
         ! Check for special case of symbol(const).member(const) where
         ! size(symbol_element).ne.0 modulo size(member_element).
 
         IF (ATOM_SYM(NOD).EQ.0 .AND. ATOM_MEM(NOD).EQ.0 .AND.
     #       ATOM_SUB(NOD).EQ.NULL .AND. NODE(ATOM_BASE(NOD)) .AND.
     #       OPNODE_OP(ATOM_BASE(NOD)).EQ.OP_LOC .AND.
     #       ATOM(OPNODE_OPND1(ATOM_BASE(NOD))) .AND.
     #       ATOM_SUB(OPNODE_OPND1(ATOM_BASE(NOD))).EQ.NULL ) THEN
 
            NOD1=OPNODE_OPND1(ATOM_BASE(NOD))
            NODE_TYPE(NOD1)=NODE_TYPE(NOD)
            ATOM_DISP(NOD1)=ATOM_DISP(NOD1)+ATOM_DISP(NOD)
	    FOLD_CONSTANTS=NOD1
            RETURN
         ENDIF
 
         GO TO 9000
 
      ENDIF
 
C-------------- NODE MUST BE AN OPNODE.
 
      IF (OPNODE_OP(NOD).EQ.OP_NOP .OR.
     #    (OPNODE_OP(NOD).EQ.OP_L2P .OR.
     #                      OPNODE_OP(NOD).EQ.OP_P2L)) THEN
         NOD=OPNODE_OPND1(NOD)
         GO TO 1
      ENDIF
 
      IF (.NOT.ASSUME_CTE) RETURN
 
      CALL PUSH(NOD,1)
      OPND1=FOLD_CONSTANTS2(OPNODE_OPND1(NOD))
      CALL POP(NOD,1)
      OPNODE_OPND1(NOD)=OPND1
 
      CALL PUSH(NOD,1)
      CALL PUSH(OPND1,1)
      OPND2=FOLD_CONSTANTS2(OPNODE_OPND2(NOD))
      CALL POP(OPND1,1)
      CALL POP(NOD,1)
      OPNODE_OPND2(NOD)=OPND2
 
      OP=OPNODE_OP(NOD)
      IF (OP.EQ.OP_CALL.OR.OP.EQ.OP_ARG.OR.OP.EQ.OP_THEN.OR.
     #    OP.EQ.OP_ALSO) GO TO 9000
 
CC      IF (OP.EQ.OP_P2L) THEN
CC         IF (NODE(OPND1).AND.OPNODE_OP(OPND1).EQ.OP_LOC.AND.
CC     #       ATOM(OPNODE_OPND1(OPND1))) THEN
CC            ATOM_FLAGS(OPNODE_OPND1(OPND1))=
CC     #         ATOM_FLAGS(OPNODE_OPND1(OPND1)).OR.A_P2L
CC            NODE_TYPE(OPND1)=S_LONG
CC            FOLD_CONSTANTS=OPND1
CC            RETURN
CC         ELSE
CC            GO TO 9000
CC         ENDIF
CC      ENDIF
 
      IF (.NOT.LITERAL(OPND1).AND..NOT.LITERAL(OPND2)) GO TO 9000
 
      TYPE=NODE_TYPE(NOD)
      TYPE1=NODE_TYPE(OPNODE_OPND1(NOD))
      TYPE2=NODE_TYPE(OPNODE_OPND2(NOD))
 
      IF (LITERAL(OPND1)) THEN
         IF (TYPE1.EQ.S_REAL.OR.TYPE1.EQ.S_DOUBLE) THEN
            R1=FLOAT_VAL(OPND1)
         ELSE
            I1=FIXED_VAL(OPND1).AND.MASK(TYPE1)
         ENDIF
      ENDIF
 
      IF (LITERAL(OPND2)) THEN
         IF (TYPE2.EQ.S_REAL.OR.TYPE2.EQ.S_DOUBLE) THEN
            R2=FLOAT_VAL(OPND2)
         ELSE
            I2=FIXED_VAL(OPND2).AND.MASK(TYPE1)
         ENDIF
      ENDIF
 
      IF (LITERAL(OPND1).AND.(LITERAL(OPND2).OR.OPND2.EQ.NULL)) THEN
         IF (TYPE1.NE.S_REAL.AND.TYPE1.NE.S_DOUBLE) THEN
            GO TO (10,20,30,40,50,60,70,80,90,100,110,120,130,140,
     #             150,160,170,180,190,200), OP
         ELSE
            GO TO (15,25,35,45,55,65,75,85,95,105,115,125,135,145,
     #             155,165,175,185,195,205), OP
         ENDIF
         GO TO (1010,1020,1030,1040,1050,1060,1070,1080,1090,1100,
     #          1110,1120,1130,1140,1150,1160,1170,1180,1190,1200,
     #          1210,1220,1230,1240,1250,1260,1270), OP-100
         CALL BUG('FC-1')
      ENDIF
 
C---------- BINARY OPERATION WITH EXACTLY ONE LITERAL OPERAND.
 
      IF (LITERAL(OPND1)) THEN
         LITOPND=OPND1
         OPND=OPND2
         I=I1
         R=R1
      ELSE
         LITOPND=OPND2
         OPND=OPND1
         I=I2
         R=R2
      ENDIF
 
      IF (TYPE1.NE.S_REAL.AND.TYPE1.NE.S_DOUBLE) THEN
         GO TO (13,23,33,43,53,63,73,83,93,103,113), OP
         IF (OP.EQ.20) GO TO 203
      ELSE
         GO TO (18,28,38,48,58,68,78,88,98,108,118), OP
         IF (OP.EQ.20) GO TO 208
      ENDIF
 
      GO TO 9000
 
C--------- SIMPLIFY BINARY OPERATIONS WITH ONE CONSTANT OPERAND.
 
13    IF (I.EQ.0) GO TO 9100                            ! ADD
      IF (FIXLIT(OPND1)) THEN
         FOLD_CONSTANTS=FOLD_LOC_REF(NOD,OPND2,I)
      ELSE
         FOLD_CONSTANTS=FOLD_LOC_REF(NOD,OPND1,I)
      ENDIF
      RETURN
18    IF (R.EQ.0.0) GO TO 9100
      GO TO 9000
 
23    IF (FIXLIT(OPND1).AND.I1.EQ.0) GO TO 9300		! SUB
      IF (FIXLIT(OPND2).AND.I2.EQ.0) GO TO 9100
      IF (FIXLIT(OPND2)) THEN
         FOLD_CONSTANTS=FOLD_LOC_REF(NOD,OPND1,-I)
         RETURN
      ENDIF
      GO TO 9000
28    IF (FLOATLIT(OPND1).AND.R1.EQ.0.0) GO TO 9300
      IF (FLOATLIT(OPND2).AND.R2.EQ.0.0) GO TO 9100
      GO TO 9000
 
33    IF (I.EQ.0) GO TO 9200				! MUL
      IF (I.EQ.1) GO TO 9100
      IF (I.EQ.-1) GO TO 9300
      GO TO 9000
38    IF (R.EQ.0.0) GO TO 9200
      IF (R.EQ.1.0) GO TO 9100
      IF (R.EQ.-1.0) GO TO 9300
      GO TO 9000
 
43    IF (FIXLIT(OPND1).AND.I.EQ.0) GO TO 9200		! DIV
      IF (FIXLIT(OPND2)) THEN
         IF (I.EQ.0) GO TO 9900
         IF (I.EQ.1) GO TO 9100
         IF (I.EQ.-1) GO TO 9300
      ENDIF
      GO TO 9000
48    IF (FLOATLIT(OPND1).AND.R.EQ.0.0) GO TO 9200
      IF (FLOATLIT(OPND2)) THEN
         IF (R.EQ.0.0) GO TO 9900
         IF (R.EQ.1.0) GO TO 9100
         IF (R.EQ.-1.0) GO TO 9300
      ENDIF
      GO TO 9000
 
53    GO TO 9000					! ADWC
58    GO TO 9000
 
63    GO TO 9000					! SUBWC
68    GO TO 9000
 
73    CONTINUE						! NEG
78    CONTINUE
 
83    CONTINUE						! NOT
88    CONTINUE
      CALL BUG ('FC-88')
 
93    IF (FIXLIT(OPND1).AND.I1.EQ.0) GO TO 9200		! EXT
      IF (FIXLIT(OPND1).AND.I1.EQ.MASK(TYPE1)) THEN
         IF (OPNODE_OP(OPND2).EQ.OP_NOT) THEN
            FOLD_CONSTANTS=OPNODE_OPND1(OPND2)
            RETURN
         ELSE
            GO TO 9400
         ENDIF
      ENDIF
      IF (FIXLIT(OPND2).AND.I2.EQ.0) GO TO 9100
      IF (FIXLIT(OPND2).AND.I2.EQ.MASK(TYPE1)) THEN
         I=0
         GO TO 8000
      ENDIF
      GO TO 9000
98    GO TO 8900
 
103   IF (I.EQ.0) GO TO 9100				! OR
      IF (I.EQ.MASK(TYPE1)) GO TO 9200
      GO TO 9000
108   GO TO 8900
 
113   IF (I.EQ.0) GO TO 9100				! XOR
      IF (I.EQ.MASK(TYPE1)) GO TO 9400
      GO TO 9000
118   GO TO 8900
 
203   IF (FIXLIT(OPND1).AND.I.EQ.0) GO TO 9200		! MOD
      IF (FIXLIT(OPND2)) THEN
         IF (I.EQ.0) GO TO 9900
         IF (I.EQ.1.OR.I.EQ.-1) THEN
            FOLD_CONSTANTS=MAKE_FIXED(0,TYPE)
            RETURN
         ENDIF
      ENDIF
      GO TO 9000
208   GO TO 8900
 
C------------- REDUCE OPERATIONS WITH CONSTANT OPERANDS.
 
10    I=I1+I2						! ADD
      GO TO 8000
15    R=R1+R2
      GO TO 8005
 
20    I=I1-I2						! SUB
      GO TO 8000
25    R=R1-R2
      GO TO 8005
 
30    I=I1*I2						! MUL
      GO TO 8000
35    R=R1*R2
      GO TO 8005
 
40    IF (I2.EQ.0) GO TO 9900				! DIV
      I=I1/I2
      GO TO 8000
45    IF (R2.EQ.0.0) GO TO 9900
      R=R1/R2
      GO TO 8005
 
50    GO TO 9000					! ADWC
55    GO TO 8900
 
60    GO TO 9000					! SBWC
65    GO TO 8900
 
70    I=-I1						! NEG
      GO TO 8000
75    R=-R1
      GO TO 8005
 
80    I=.NOT.I1						! NOT
      GO TO 8000
85    GO TO 8900
 
90    I=I1.AND..NOT.I2                                  ! EXT
      GO TO 8000
95    GO TO 8900
 
100   I=I1.OR.I2					! OR
      GO TO 8000
105   GO TO 8900
 
110   I=I1.XOR.I2					! XOR
      GO TO 8000
115   GO TO 8900
 
120   I=I1.LT.I2					! LT
      GO TO 8000
125   I=R1.LT.R2
      GO TO 8000
 
130   I=I1.GT.I2					! GT
      GO TO 8000
135   I=R1.GT.R2
      GO TO 8000
 
140   I=I1.EQ.I2					! EQ
      GO TO 8000
145   I=R1.EQ.R2
      GO TO 8000
 
150   I=I1.NE.I2					! NE
      GO TO 8000
155   I=R1.NE.R2
      GO TO 8000
 
160   I=I1.LE.I2					! LE
      GO TO 8000
165   I=R1.LE.R2
      GO TO 8000
 
170   I=I1.GE.I2					! GE
      GO TO 8000
175   R=R1.GE.R2
      GO TO 8000
 
180   CALL BUG('FC-180')				! LOC
185   CALL BUG('FC-185')
 
190   CALL BUG('FC-190')				! ASSN
195   CALL BUG('FC-195')
 
200   IF (I2.EQ.0) GO TO 9900				! MOD
      I=MOD(I1,I2)
      GO TO 8000
205   GO TO 8900
 
C----------- CONVERT TYPE OF LITERAL OPERAND.
 
1010  CONTINUE						! B2W
1020  CONTINUE						! B2I
1030  CONTINUE						! B2L
1050  CONTINUE						! W2B
1060  CONTINUE						! W2L
1070  CONTINUE						! I2B
1090  CONTINUE						! I2L
1120  CONTINUE						! L2W
1140  CONTINUE						! L2B
1180  CONTINUE						! L2Q
1240  CONTINUE						! Q2L
      I=I1
      GO TO 8000
 
1040  CONTINUE						! B2R
1080  CONTINUE						! I2R
1130  CONTINUE						! L2R
1170  CONTINUE						! L2D
1250  CONTINUE						! I2D
      R=I1
      GO TO 8005
 
1100  CONTINUE						! R2L
1110  CONTINUE						! R2I
1150  CONTINUE						! R2B
1160  CONTINUE						! R2W
1200  CONTINUE						! D2B
1210  CONTINUE						! D2I
1230  CONTINUE						! D2L
      I=R1
      GO TO 8000
 
1190  CONTINUE						! R2D
1220  CONTINUE						! D2R
      R=R1
      GO TO 8005
 
1260  CONTINUE						! L2P
1270  CONTINUE						! P2L
      GO TO 9000
 
C---------------------------------------------------
 
8000  FOLD_CONSTANTS=MAKE_FIXED(I.AND.MASK(TYPE),TYPE)
      RETURN
 
8005  FOLD_CONSTANTS=MAKE_FLOAT(R,TYPE)
      RETURN
 
8900  CALL ERROR('FC - ILLEGAL MIXING OF TYPES')
9000  FOLD_CONSTANTS=NOD
      RETURN
 
9100  FOLD_CONSTANTS=OPND
      RETURN
 
9200  FOLD_CONSTANTS=LITOPND
      RETURN
 
9300  FOLD_CONSTANTS=MAKE_NODE(OP_NEG,OPND,NULL,TYPE,0,0)
      RETURN
 
9400  FOLD_CONSTANTS=MAKE_NODE(OP_NOT,OPND,NULL,TYPE,0,0)
      RETURN
 
9900  CALL WARN('FC - ATTEMPTED DIVISION BY ZERO')
      GO TO 9000
      END
C----------------------------------------------------
      INTEGER*2 FUNCTION FOLD_CONSTANTS2(NODX)
      IMPLICIT INTEGER*2 (A-Z)
      FOLD_CONSTANTS2=FOLD_CONSTANTS(NODX)
      RETURN
      END
C----------------------------------------------------
      INTEGER*2 FUNCTION EXTRACT_DISPLACEMENT(NOD,DISP)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*2 COMBOP(S_BYTE:S_QUAD,S_BYTE:S_QUAD)
      DATA COMBOP/
     #      0,     0,     0,     0,     0,     0,     0,     0,
     # OP_B2W,     0,     0,     0,     0,     0,     0,     0,
     # OP_B2I,     0,     0,     0,     0,     0,     0,     0,
     #      0,     0,     0,     0,     0,     0,     0,     0,
     #      0,     0,     0,     0,     0,     0,     0,     0,
     # OP_B2L,OP_W2L,OP_I2L,     0,     0,     0,     0,     0,
     #      0,     0,     0,     0,     0,     0,     0,     0,
     #      0,     0,     0,     0,     0,     0,     0,     0/
 
      NOD1=NOD
 
      IF (FIXLIT(NOD1)) THEN
         DISP=FIXED_VAL(NOD1)
         EXTRACT_DISPLACEMENT=NULL
         RETURN
      ENDIF
 
      IF (.NOT.NODE(NOD1)) GO TO 900
 
      IF (OPNODE_OP(NOD1).GT.100) NOD1=OPNODE_OPND1(NOD1)
 
      IF (OPNODE_OP(NOD1).EQ.OP_ADD) THEN
 
         IF (FIXLIT(OPNODE_OPND2(NOD1))) THEN
            DISP=FIXED_VAL(OPNODE_OPND2(NOD1))
            NOD2=OPNODE_OPND1(NOD1)
         ELSEIF (FIXLIT(OPNODE_OPND1(NOD1))) THEN
            DISP=FIXED_VAL(OPNODE_OPND1(NOD1))
            NOD2=OPNODE_OPND2(NOD1)
         ELSE
            GO TO 900
         ENDIF
 
      ELSEIF (OPNODE_OP(NOD1).EQ.OP_SUB) THEN
         
         IF (FIXLIT(OPNODE_OPND2(NOD1))) THEN
            DISP=-FIXED_VAL(OPNODE_OPND2(NOD1))
            NOD2=OPNODE_OPND1(NOD1)
         ELSE
            GO TO 900
         ENDIF
 
      ELSE
         GO TO 900
      ENDIF
 
      IF (OPNODE_OP(NOD).LE.100) THEN
         EXTRACT_DISPLACEMENT=NOD2
         RETURN
      ENDIF
 
      IF (.NOT.NODE(NOD2) .OR. OPNODE_OP(NOD2).LE.100 .OR.
     #    NODE_TYPE(OPNODE_OPND1(NOD2)).GT.NODE_TYPE(NOD2)) THEN
C------- (Note that downward/upward coercions are not transitive!) ---
         OPNODE_OPND1(NOD)=NOD2
         EXTRACT_DISPLACEMENT=NOD
         RETURN
      ENDIF
 
      NOD2=OPNODE_OPND1(NOD2)
      NEWOP=COMBOP(NODE_TYPE(NOD2),NODE_TYPE(NOD))
      IF (NEWOP.EQ.0) CALL BUG('ED-0')
      EXTRACT_DISPLACEMENT=MAKE_NODE(NEWOP,NOD2,NULL,NODE_TYPE(NOD),
     #                               0,0)
      RETURN
 
900   DISP=0
      EXTRACT_DISPLACEMENT=NOD
      RETURN
      END
C----------------------------------------------------
      INTEGER*2 FUNCTION FOLD_LOC_REF(NOD,OPND,I)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      INTEGER*4 I
 
      IF (NODE(OPND).AND.OPNODE_OP(OPND).EQ.OP_LOC) THEN
 
         ATM=OPNODE_OPND1(OPND)
 
         IF (.NOT.ATOM(ATM)) GO TO 900
 
         ATOM_DISP(ATM)=ATOM_DISP(ATM)+I
         FOLD_LOC_REF=OPND
         RETURN
 
      ENDIF
 
900   FOLD_LOC_REF=NOD
      RETURN
      END
