C***********************************************************************
C
C                               MODULES.FOR
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
C    This module of the PL/M-VAX compiler processes a (series of) PL/M
C    program modules.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    13SEP81  Alex Hunter  1. Add LONG attribute to P_DATA psect. (V5.2)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C    12NOV81  Alex Hunter  1. Define APD and MEMORY psects. (V6.1)
C    14NOV81  Alex Hunter  1. Change addressing modes and psect usage.
C			      (V6.2)
C
C***********************************************************************
      SUBROUTINE COMPILATION
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CALL MODULE
      IF (TT.EQ.EOFTOK.OR.TT.EQ.K_EOF) RETURN
      CALL ERROR('MULTIPLE COMPILATIONS NOT CURRENTLY SUPPORTED')
      RETURN
      END
C--------------------------------------------------------------
      SUBROUTINE MODULE
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*80 TITLE
      CHARACTER*32 MODULE_NAME,START_NAME,ID_LINE,PUBLIQUE
      INTEGER*4 NTICKS,NTICKS1,NFAULTS,NFAULTS1
      REAL*4 TIME,TIME1,CPUTIM
      INTEGER*4 HANDLE
      CHARACTER*40 REGISTER_MASK
C
      HANDLE=0
      CALL LIB$INIT_TIMER(HANDLE)
      TIME1=SECNDS(0.0)
      CALL JPI(NTICKS1,NFAULTS1)
      IF (NLABELS.EQ.0) THEN
         CALL ERROR('MODULE NAME MISSING: MAIN. ASSUMED')
         NLABELS=1
         LABELS(NLABELS) = 'MAIN.'
      ENDIF
 
      PROC_ENTRY_MASK(PROC_LEVEL)=0
 
      DO I=1,NLABELS-1
      CALL ERROR('EXTRANEOUS LABEL IGNORED -- '//LABELS(I))
      ENDDO
      MODULE_NAME=PUBLIQUE(LABELS(NLABELS))
      CALL PUSHC(LABELS(NLABELS))
      CALL EMIT('.TITLE  '//MODULE_NAME(:LNB(MODULE_NAME))//' '//
     #          TITLE(:MAKE_CHARS(TITLE,TITLE_STRING)))
      ENCODE(32,1001,ID_LINE) VERSION
1001  FORMAT('.IDENT  "PL/M-VAX V',F3.1,'"')
      CALL EMIT(ID_LINE)
      CALL EMIT('.ENABLE GLOBAL')
      CALL EMIT('.ENABLE LOCAL_BLOCK')
      IF (DEBUG_FLAG) CALL EMIT('.ENABLE DEBUG')
      IF (PSECT_NAME(P_CONSTANTS).NE.PSECT_NAME(P_DATA)) THEN
         CALL EMIT('.PSECT  '//
     #          PSECT_NAME(P_CONSTANTS)(:LNB(PSECT_NAME(P_CONSTANTS)))//
     #          ',RD,NOWRT,EXE,GBL,CON')
      ENDIF
      CALL EMIT('.PSECT  '//
     #          PSECT_NAME(P_STACK)(:LNB(PSECT_NAME(P_STACK)))//
     #          ',RD,WRT,EXE,GBL,CON')
 
      IF (VECTOR_FLAG) THEN
         CALL EMIT('.PSECT  '//
     #             PSECT_NAME(P_VECTOR)(:LNB(PSECT_NAME(P_VECTOR)))//
     #             ',RD,NOWRT,EXE,GBL,CON')
      ENDIF
 
      CALL EMIT('.PSECT  '//
     #          PSECT_NAME(P_APD)(:LNB(PSECT_NAME(P_APD)))//
     #          ',RD,WRT,NOEXE,GBL,CON,LONG')
 
      IF (FREQ_FLAG) THEN
         CALL EMIT('.PSECT  '//
     #             PSECT_NAME(P_FREQ)(:LNB(PSECT_NAME(P_FREQ)))//
     #             ',RD,WRT,NOEXE,GBL,CON')
      ENDIF
 
      CALL EMIT('.PSECT  '//
     #          PSECT_NAME(P_MEMORY)(:LNB(PSECT_NAME(P_MEMORY)))//
     #          ',PIC,OVR,GBL,SHR,NOEXE,RD,WRT,LONG')
      CALL EMIT1('MEMORY.:')
 
      CALL EMIT('.PSECT  '//
     #          PSECT_NAME(P_DATA)(:LNB(PSECT_NAME(P_DATA)))//
     #         ',RD,WRT,NOEXE,GBL,CON,LONG')
      IF (MODEL.NE.4) THEN
         CALL EMIT1('K. = ^X8000')
      ENDIF
      IF (MODEL.EQ.4.AND..NOT.OVERLAY_FLAG) THEN
         CALL EMIT1('M. = .+128')
      ENDIF
      CALL EMIT('.PSECT  '//
     #          PSECT_NAME(P_CODE)(:LNB(PSECT_NAME(P_CODE)))//
     #          ',RD,NOWRT,EXE,GBL,CON')
      NLABELS=0
      CALL MATCH(K_DO)
      CALL MATCH(D_SEMI)
      CALL BLOCK_BEGIN
      CALL DECLARATIONS
      IF (TT.NE.K_END) THEN
         CALL PSECT(P_APD)
         CALL EMIT1('FPSP. = .')
         CALL EMIT('.BLKQ   1')
         CALL PSECT(P_CODE)
         PATH=.TRUE.
         IF (OVERLAY_FLAG) THEN
            CALL EMIT1(MODULE_NAME(:LNB(MODULE_NAME))//'::')
            CALL EMIT('.WORD   MSK.')
            START_NAME=' '
         ELSE
            CALL EMIT1('START.:  .WORD   MSK.')
            START_NAME='START.'
         ENDIF
         IF (MODEL.NE.4) THEN
            CALL EMIT('MOVL    #K.,R11')
            CALL PRESERVE_REG(11)
         ELSEIF (.NOT.OVERLAY) THEN
            CALL EMIT('MOVAB   M.,R11')
            CALL PRESERVE_REG(11)
         ENDIF
         IF (MODEL.EQ.1 .OR. MODEL.EQ.3) THEN
            CALL EMIT('MOVAB   S.,R10')
            CALL PRESERVE_REG(10)
         ENDIF
         CALL EMIT('MOVQ    FP,FPSP.')
 
         CALL UNITS
         CALL BREAK
 
         IF (PATH) THEN
            CALL EMIT('MOVL    #1,R0')
            CALL EMIT('RET')
         ENDIF
 
         CALL EMIT1('MSK. = '//
     #               REGISTER_MASK(PROC_ENTRY_MASK(PROC_LEVEL)))
      ELSE
         START_NAME=' '
      ENDIF
 
      CALL OUTPUT_PUBLICS(MODULE_NAME)
 
      CALL BLOCK_END
      CALL END_STATEMENT
 
      IF (SKIP_STATE.NE.4) THEN
         CALL ERROR('$ENDIF MISSING AT END OF COMPILATION')
      ENDIF
 
      CALL EMIT('.END    '//START_NAME)
 
      IF (PRINT_FLAG) THEN
         CALL SUMMARY_TAIL
         CALL ROOM_FOR(8)
         CALL ADVANCE_ONE_LINE
         TIME=SECNDS(TIME1)
         CALL JPI(NTICKS,NFAULTS)
         CPUTIM=(NTICKS-NTICKS1)*.01
         WRITE(LST,1000) CPUTIM,TIME,NFAULTS-NFAULTS1
1000     FORMAT(//'  PL/M-VAX COMPILATION STATISTICS'//
     #            '    CPU Time:',T21,F8.2' seconds'/
     #            '    Elapsed Time:'T21,F8.2' seconds'/
     #            '    Page Faults:'T21,I8)
      ENDIF
      CALL LIB$SHOW_TIMER(HANDLE)
      RETURN
      END
