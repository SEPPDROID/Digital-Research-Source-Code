C***********************************************************************
C
C                               GETLEX.FOR
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
C    This is the lexical analysis module of the PL/M-VAX compiler.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    29SEP81  Alex Hunter  1. Increase max string size. (V5.3)
C			   2. Replace null strings with ' '.
C    28OCT81  Alex Hunter  1. Add new keywords. (V5.7)
C
C***********************************************************************
C   ---  Compile me with /NOCHECK please !  ---
 
      SUBROUTINE GETLEX
C
C----- GET A LEXICAL ELEMENT.
C
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*100 NUMBER
      INTEGER*2 I,J,LAST,DIG,RADIX
      CHARACTER*1 UPPER(97:122)
      DATA UPPER /'A','B','C','D','E','F','G','H','I','J','K','L','M',
     #            'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'/
      CHARACTER*10 KEYWORD(101:154)
      DATA KEYWORD
     //'ADDRESS   ','AND       ','AT        ','BASED     ','BY        '
     ,,'BYTE      ','CALL      ','CASE      ','DATA      ','DECLARE   '
     ,,'DISABLE   ','DO        ','ELSE      ','ENABLE    ','END       '
     ,,'EOF       ','EXTERNAL  ','GO        ','GOTO      ','HALT      '
     ,,'IF        ','INITIAL   ','INTEGER   ','INTERRUPT ','LABEL     '
     ,,'LITERALLY ','MINUS     ','MOD       ','NOT       ','OR        '
     ,,'PLUS      ','POINTER   ','PROCEDURE ','PUBLIC    ','REAL      '
     ,,'REENTRANT ','RETURN    ','STRUCTURE ','THEN      ','TO        '
     ,,'WHILE     ','WORD      ','XOR       ','COMMON    ','LONG      '
     ,,'DOUBLE    ','OTHERWISE ','QUAD      ','FORWARD   ','SELECTOR  '
     ,,'DWORD     ','SHORT     ','BOOLEAN   ','REGISTER  '
     //
      CHARACTER*2 DD(201:218)
      DATA DD
     //'+ ','- ','* ','/ ','< ','> ','= ','<>','<=','>='
     ,,':=',': ','; ','. ',', ','( ',') ','@ '
     //
      COMMON /ANALYZE/ KEYWORD,DD
C
100    IF (CHAR.EQ.' '.OR.CHAR.EQ.TAB) CALL GETNB
C
C          /* COMMENT */   OR '/' DELIMITER.
C
      IF (CHAR.EQ.'/') THEN
        NEXT_DELIMITER = CHAR
        CALL GETC
        IF (CHAR.NE.'*') GO TO 695
110     CALL GETC
120     IF (CHAR.EQ.EOF) CALL FATAL('EOF BEFORE END OF COMMENT')
        IF (CHAR.NE.'*') GO TO 110
        CALL GETC
        IF (CHAR.NE.'/') GO TO 120
        CALL GETC
        GO TO 100
C
C        IDENTIFIER.
C
      ELSEIF (CHAR.GE.'A'.AND.CHAR.LE.'Z' .OR. CHAR.EQ.'%' .OR.
     #        CHAR.GE.'a'.AND.CHAR.LE.'z' .OR. CHAR.EQ.'_') THEN
        I=1
        NEXT_IDENTIFIER=' '
200     IF (I.LE.32) THEN
           IF (CHAR.EQ.'%') THEN
              CHAR='$'
           ELSEIF (CHAR.GE.'a'.AND.CHAR.LE.'z') THEN
              CHAR=UPPER(ICHAR(CHAR))
           ENDIF
          NEXT_IDENTIFIER(I:I)=CHAR
          I=I+1
        ENDIF
210     CALL GETC
        IF (CHAR.GE.'A' .AND. CHAR.LE.'Z' .OR. CHAR.EQ.'_' .OR.
     #      CHAR.GE.'0' .AND. CHAR.LE.'9') GO TO 200
        IF (CHAR.EQ.'%' .OR. CHAR.GE.'a'.AND.CHAR.LE.'z') GO TO 200
        IF (CHAR.EQ.'$') GO TO 210
        I=HASH_BUCKET(HASH(NEXT_IDENTIFIER))
225     IF (I.GT.0) THEN
          IF (SYMBOL_PLM_ID(I).EQ.NEXT_IDENTIFIER) THEN
           IF (SYMBOL_KIND(I).EQ.S_KEYWORD) GO TO 230
           IF (SYMBOL_KIND(I).EQ.S_MACRO) GO TO 240
           GO TO 226
          ENDIF
          I=SYMBOL_CHAIN(I)
          GO TO 225
         ENDIF
226     NEXT_TOKENTYPE=ID
        RETURN
C
C	 KEYWORD.
C
230     I=SYMBOL_LINK(I)        ! TOKEN_VALUE OF KEYWORD.
        NEXT_TOKENTYPE=I
        IF (I.EQ.K_THEN) THEN
           LIST_STNO=LIST_STNO+1
        ELSEIF (I.EQ.K_DO.OR.I.EQ.K_PROCEDURE) THEN
           LIST_BLOCK_LEVEL=LIST_BLOCK_LEVEL+1
        ELSEIF (I.EQ.K_END) THEN
           LIST_BLOCK_LEVEL=LIST_BLOCK_LEVEL-1
        ENDIF
        RETURN
C
C	PARAMETERLESS MACRO.
C
240     IF (LITLEV.EQ.LITMAX) CALL FATAL('MACRO STACK OVERFLOW')
        LITCOL(LITLEV)=COL
        LITLEV=LITLEV+1
        COL=0
        LITVAL(LITLEV)=STRINGS(SYMBOL_LINK(I):SYMBOL_LINK(I)+
     #                          SYMBOL_ELEMENT_SIZE(I)-1)//EOL
        CALL GETC
        GO TO 100
C
C	 NUMERIC CONSTANT.
C
      ELSEIF (CHAR.GE.'0' .AND. CHAR.LE.'9') THEN
        NUMBER=' '
        I=1
300     IF (I.LE.100) NUMBER(I:I)=CHAR
        I=I+1
310     CALL GETC
        IF (CHAR.GE.'0'.AND.CHAR.LE.'9' .OR.
     #      CHAR.GE.'A'.AND.CHAR.LE.'Z') GO TO 300
        IF (CHAR.GE.'a'.AND.CHAR.LE.'z') THEN
           CHAR=UPPER(ICHAR(CHAR))
           GO TO 300
        ENDIF
        IF (CHAR.EQ.'$') GO TO 310
        IF (CHAR.EQ.'.') GO TO 350
C
C		FIXED POINT CONSTANT.
C
        LAST=I-1
        IF (NUMBER(LAST:LAST).EQ.'B') THEN
          RADIX=2
          LAST=LAST-1
        ELSEIF (NUMBER(LAST:LAST).EQ.'O' .OR.
     #          NUMBER(LAST:LAST).EQ.'Q') THEN
          RADIX=8
          LAST=LAST-1
        ELSEIF (NUMBER(LAST:LAST).EQ.'D') THEN
          RADIX=10
          LAST=LAST-1
        ELSEIF (NUMBER(LAST:LAST).EQ.'H') THEN
          RADIX=16
          LAST=LAST-1
        ELSE
          RADIX=10
        ENDIF
        NEXT_FIXVAL=0
        DO 320 J=1,LAST
          IF (NUMBER(J:J).GE.'A') THEN
            DIG=ICHAR(NUMBER(J:J))-ICHAR('A')+10
          ELSE
            DIG=ICHAR(NUMBER(J:J))-ICHAR('0')
          ENDIF
          IF (DIG.GE.RADIX) 
     #        CALL ERROR('Illegal digit in numeric constant')
          NEXT_FIXVAL=NEXT_FIXVAL*RADIX+DIG
320     CONTINUE
        NEXT_TOKENTYPE=FIXCON
        GO TO 400
C
C		FLOATING POINT CONSTANT.
C
350     IF (I.LE.100) NUMBER(I:I)=CHAR
        I=I+1
360     CALL GETC
        IF (CHAR.GE.'0'.AND.CHAR.LE.'9') GO TO 350
        IF (CHAR.EQ.'$') GO TO 360
        IF (CHAR.NE.'E'.AND.CHAR.NE.'e') GO TO 390
        IF (I.LE.100) NUMBER(I:I)=CHAR
        I=I+1
        CALL GETC
        IF (CHAR.NE.'+'.AND.CHAR.NE.'-') GO TO 380
370     IF (I.LE.100) NUMBER(I:I)=CHAR
        I=I+1
375     CALL GETC
380     IF (CHAR.GE.'0'.AND.CHAR.LE.'9') GO TO 370
        IF (CHAR.EQ.'$') GO TO 375
390     NEXT_TOKENTYPE=FLOATCON
        DECODE(I-1,9999,NUMBER,ERR=410) NEXT_FLOATVAL
9999    FORMAT(G)
400     IF (I.GT.101) CALL ERROR('Numeric constant too long')
        RETURN
410     CALL ERROR('Invalid floating point constant')
        RETURN
C
C	 STRING.
C
      ELSEIF (CHAR.EQ.'''') THEN
        NEXT_STRING=' '
        I=1
500     CALL GETC
        IF (CHAR.EQ.EOF) THEN
          CALL ERROR('String is missing final quote')
          NEXT_TOKENTYPE=EOFTOK
          RETURN
        ELSEIF (CHAR.EQ.'''') THEN
          CALL GETC
          IF (CHAR.NE.'''') GO TO 510
        ENDIF
        IF (I.LE.STRING_SIZE_MAX) THEN
          NEXT_STRING(I:I)=CHAR
          I=I+1
        ELSE
          CALL ERROR('String constant is too long')
          GO TO 510
        ENDIF
        GO TO 500
510     NEXT_TOKENTYPE=STRCON
        NEXT_STRLEN=I-1
        IF (NEXT_STRLEN.EQ.0) THEN
          CALL WARN('NULL STRING REPLACED BY '' ''')
          NEXT_STRLEN=1
        ENDIF
        RETURN
C
C	 END OF FILE.
C
      ELSEIF (CHAR.EQ.EOF) THEN
        NEXT_TOKENTYPE=EOFTOK
        RETURN
C
C 	 DELIMITER.
C
      ELSE
        NEXT_DELIMITER=CHAR
        IF (CHAR.EQ.';') THEN
           LIST_STNO=LIST_STNO+1
           GO TO 690
        ENDIF
        IF (CHAR.EQ.'+'.OR.CHAR.EQ.'-'.OR.CHAR.EQ.'*'.OR.
     #      CHAR.EQ.'='.OR.CHAR.EQ.'.'.OR.
     #      CHAR.EQ.','.OR.CHAR.EQ.'('.OR.CHAR.EQ.')'.OR.
     #      CHAR.EQ.'@') GO TO 690
        IF (CHAR.EQ.'<') THEN
          CALL GETC
          IF (CHAR.EQ.'>'.OR.CHAR.EQ.'=') GO TO 680
          GO TO 695
        ELSEIF (CHAR.EQ.'>'.OR.CHAR.EQ.':') THEN
          CALL GETC
          IF (CHAR.EQ.'=') GO TO 680
          GO TO 695
        ENDIF
        NEXT_TOKENTYPE=INVALID
        CALL GETC
        RETURN
680     NEXT_DELIMITER(2:2)=CHAR
690     CALL GETC
695     DO 697 NEXT_TOKENTYPE=201,218
          IF (NEXT_DELIMITER.EQ.DD(NEXT_TOKENTYPE)) RETURN
697     CONTINUE
        CALL BUG('DELIMITER NOT FOUND IN DD TABLE')
      ENDIF
      END
