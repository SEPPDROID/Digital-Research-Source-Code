C***********************************************************************
C
C                               EMIT.FOR
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
C    This module of the PL/M-VAX compiler contains routines for emitting
C    symbolic code and label definitions.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. Add EMIT_ABSDEF4 and EMIT_RELDEF4 entry
C			      points.  (V5.3)
C    12NOV81  Alex Hunter  1. Use symbol_psect attribute. (V6.1)
C    14NOV81  Alex Hunter  1. Change addressing modes. (V6.2)
C    15FEB81  Alex Hunter  1. Change opcode column to permit longer
C			      code lines. (V6.7)
C
C***********************************************************************
      SUBROUTINE EMIT(CODE)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*(*) CODE,PC
      CHARACTER*32 NAME,LOC_LAB,PUBLIQUE,S1
      CHARACTER*10 STRING10,DSTRING
      INTEGER*4 IVAL,IFSD,OFFSET,OFFSET4
 
      IF (OBJECT_FLAG) WRITE(OUT,1000) CODE
1000  FORMAT(2X,A)
      IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,2000) CODE
2000     FORMAT(32X,A)
      ENDIF
      RETURN
C----------------------------------------------------------
      ENTRY EMIT_LABEL(IX)
      IF ((SYMBOL_FLAGS(IX).AND.S_PUBLIC).NE.0) THEN
         S1=PUBLIQUE(SYMBOL_PLM_ID(IX))
         IF (OBJECT_FLAG) THEN
            IF (MODEL.NE.4) THEN
               WRITE(OUT,5002) S1(:LNB(S1))
5002           FORMAT(X,A,'::'/2X,'MOVL    #K.,R11')
            ELSE IF (.NOT.OVERLAY_FLAG) THEN
               WRITE(OUT,1002) S1(:LNB(S1))
1002           FORMAT(X,A,'::'/2X,'MOVAB   M.,R11')
            ENDIF
         ENDIF
         IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
            CALL ADVANCE_ONE_LINE
            WRITE(LST,2002) S1(1:LNB(S1))
2002        FORMAT(31X,A,'::')
            IF (MODEL.NE.4) THEN
               CALL ADVANCE_ONE_LINE
               WRITE(LST,4002)
4002           FORMAT(32X,'MOVL    #K.,R11')
            ELSE IF (.NOT.OVERLAY_FLAG) THEN
               CALL ADVANCE_ONE_LINE
               WRITE(LST,3002)
3002           FORMAT(32X,'MOVAB   M.,R11')
            ENDIF
         ENDIF
      ENDIF
      IF (OBJECT_FLAG)
     #   WRITE(OUT,1003) SYMBOL_VAX_ID(IX)(1:LNB(SYMBOL_VAX_ID(IX)))
1003  FORMAT(X,A,':')
      IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,2003) SYMBOL_VAX_ID(IX)(1:LNB(SYMBOL_VAX_ID(IX)))
2003     FORMAT(31X,A,':')
      ENDIF
      RETURN
C----------------------------------------------------------
      ENTRY EMIT_ABSDEF(NAME,OFF)
      IVAL=OFF
      GO TO 10
C----------------------------
      ENTRY EMIT_ABSDEF4(NAME,OFFSET4)
      IVAL=OFFSET4
10    CONTINUE
      DSTRING=STRING10(IVAL,IFSD)
      IF (OBJECT_FLAG) WRITE(OUT,1001) NAME(1:LNB(NAME)),DSTRING(IFSD:)
1001  FORMAT(X,A,' = ',A)
      IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,2001) NAME(1:LNB(NAME)),DSTRING(IFSD:)
2001     FORMAT(31X,A,' = ',A)
      ENDIF
      RETURN
C----------------------------------------------------------
      ENTRY EMIT_RELDEF(IX,PC,OFFSET2)
      OFFSET=OFFSET2
      GO TO 20
C----------------------------
      ENTRY EMIT_RELDEF4(IX,PC,OFFSET4)
      OFFSET=OFFSET4
20    CONTINUE
      IF (OFFSET.NE.0) THEN
         IVAL=OFFSET
         DSTRING=STRING10(IVAL,IFSD)
         IF (IVAL.GT.0) THEN
            IFSD=IFSD-1
            DSTRING(IFSD:IFSD)='+'
         ENDIF
      ELSE
         DSTRING=' '
         IFSD=10
      ENDIF
      IF ((SYMBOL_FLAGS(IX).AND.S_PUBLIC).NE.0) THEN
         S1=PUBLIQUE(SYMBOL_PLM_ID(IX))
         IF (OBJECT_FLAG)
     #      WRITE(OUT,1004) S1(:LNB(S1)),PC,
     #                      DSTRING(IFSD:)
1004     FORMAT(X,A,' == ',2A)
         IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
            CALL ADVANCE_ONE_LINE
            WRITE(LST,2004) S1(:LNB(S1)),PC,
     #                      DSTRING(IFSD:)
2004        FORMAT(31X,A,' == ',2A)
         ENDIF
      ENDIF
      IF (MODEL.EQ.4.AND..NOT.OVERLAY_FLAG.AND.
     #    SYMBOL_PSECT(IX).EQ.P_DATA) THEN
         IF (OBJECT_FLAG)
     #      WRITE(OUT,1005) SYMBOL_VAX_ID(IX)(:LNB(SYMBOL_VAX_ID(IX))),
     #                      PC,DSTRING(IFSD:)
1005     FORMAT(X,A,' = ',A,'-M.',A)
         IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
            CALL ADVANCE_ONE_LINE
            WRITE(LST,2005) SYMBOL_VAX_ID(IX)(:LNB(SYMBOL_VAX_ID(IX))),
     #                      PC,DSTRING(IFSD:)
2005        FORMAT(31X,A,' = ',A,'-M.',A)
         ENDIF
      ELSE
         IF (OBJECT_FLAG)
     #      WRITE(OUT,1007) SYMBOL_VAX_ID(IX)(:LNB(SYMBOL_VAX_ID(IX))),
     #                      PC,DSTRING(IFSD:)
1007     FORMAT(X,A,' = ',2A)
         IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
            CALL ADVANCE_ONE_LINE
            WRITE(LST,2007) SYMBOL_VAX_ID(IX)(:LNB(SYMBOL_VAX_ID(IX))),
     #                      PC,DSTRING(IFSD:)
2007        FORMAT(31X,A,' = ',2A)
         ENDIF
      ENDIF
      RETURN
C----------------------------------------------------------
      ENTRY EMIT_LOCAL_LABEL(LL)
      IF (LL.EQ.0) RETURN
      LOC_LAB=LOCAL_LABEL(LL,N1)
      IF (OBJECT_FLAG) WRITE(OUT,1003) LOC_LAB(:N1)
      IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,2003) LOC_LAB(:N1)
      ENDIF
      PATH=.TRUE.
      RETURN
C----------------------------------------------------------
      ENTRY EMIT1(CODE)
      IF (OBJECT_FLAG) WRITE(OUT,1006) CODE
1006  FORMAT(X,A)
      IF (CODE_FLAG.AND.PRINT_FLAG.AND.LIST_FLAG) THEN
         CALL ADVANCE_ONE_LINE
         WRITE(LST,2006) CODE
2006     FORMAT(31X,A)
      ENDIF
      RETURN
      END
