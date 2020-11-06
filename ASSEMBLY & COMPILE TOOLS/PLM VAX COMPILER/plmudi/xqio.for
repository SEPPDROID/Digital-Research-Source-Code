C***********************************************************************
C
C                          XQIO.FOR
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
C-----------------------------------------------------------------------
C
C    XQIO  --   UDI I/O PACKAGE FOR VAX/VMS UTILIZING THE
C                 "CORE FILE" CONCEPT.
C
C-----------------------------------------------------------------------
C
C   H001  20MAY81  ALEX HUNTER  	1. WRITTEN.
C   H002  09JUN81  ALEX HUNTER          1. IMPLEMENTED DQ$SPECIAL.
C   H003  10JUN81  ALEX HUNTER		1. DQ$EXIT CALLS LIB$STOP.
C   H004  08JAN82  ALEX HUNTER		1. TREAT 'LIST:...' AS INTER-
C					   ACTIVE (NON-CORE FILE) ON
C					   OUTPUT.
C   H005  12JAN82  Alex Hunter  1. Allocate core dynamically.
C   H006  31JAN82  Alex Hunter  1. Add indirect command file completion
C			     	   codes to DQ$EXIT.
C				2. Handle EOF on :CI:.
C   H007  03FEB82  Alex Hunter  1. Change routine names.
C   H008  06FEB82  Alex Hunter  1. Use local copies of RMSDEF.FOR and
C				   IODEF.FOR include files.
C
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION DQATTACH (PATH,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      BYTE PATH(*)
      INTEGER*2 STATUS
 
      INTEGER*4 N
 
      INTEGER*4 DESCRIPTOR(2)
      DATA DESCRIPTOR /1,0/
 
      CHARACTER*45 XQ___PATH,FULLY_QUALIFIED_NAME
      LOGICAL*4 FILE_EXISTS,EOF,XQ___READ,XQ___ENSURE_ALLOCATED
      CHARACTER*10 CC
 
      DQATTACH=0
 
      DO CONN=CONN_MIN,CONN_MAX
         IF (STATE(CONN).EQ.STATE_UNATTACHED) GO TO 100
      ENDDO
 
      STATUS=E$CONTEXT
      RETURN
 
100   FILENAME(CONN)=XQ___PATH(PATH,STATUS)
      IF (STATUS.NE.E$OK) RETURN
 
      LENGTH(CONN)=0
      MARKER(CONN)=0
 
      IF (FILENAME(CONN)(1:3).EQ.'BB:') THEN
         TYPE(CONN)=BYTE_BUCKET
         ACCESS_RIGHTS(CONN)=AR_READ+AR_WRITE+AR_UPDATE
         SEEK_CAPABILITY(CONN)=SC_FORWARD+SC_BACKWARD
 
      ELSE
         INQUIRE (FILE=FILENAME(CONN),NAME=FULLY_QUALIFIED_NAME,
     #            EXIST=FILE_EXISTS,CARRIAGECONTROL=CC,ERR=900)
 
         IF (.NOT.FILE_EXISTS) THEN
            STATUS=E$FNEXIST
            RETURN
         ENDIF
 
         OPEN (UNIT=CONN,FILE=FILENAME(CONN),STATUS='OLD',
     #         ACCESS='SEQUENTIAL',READONLY,ERR=910)
 
         IF (FILENAME(CONN)(1:3).EQ.'CI:' .OR.
     #       FULLY_QUALIFIED_NAME(1:3).EQ.'_TT' .OR.
     #       FULLY_QUALIFIED_NAME.EQ.'SYS$INPUT:' .OR.
     #       FULLY_QUALIFIED_NAME.EQ.'SYS$COMMAND:') THEN
 
            TYPE(CONN)=INTERACTIVE
            ACCESS_RIGHTS(CONN)=AR_READ
            SEEK_CAPABILITY(CONN)=0
            SPECIAL_MODE(CONN)=LINE_EDITED
 
         ELSE
            TYPE(CONN)=NORMAL
            ACCESS_RIGHTS(CONN)=AR_DELETE+AR_READ+AR_WRITE+AR_UPDATE
            SEEK_CAPABILITY(CONN)=SC_FORWARD+SC_BACKWARD
 
            DO WHILE (.TRUE.)
               ! Ensure room for max size record + CRLF.
               IF (.NOT.XQ___ENSURE_ALLOCATED(CONN,
     #                                        LENGTH(CONN)+MAX_INPUT+1))
     #         THEN
                  STATUS=E$MEM	! Can't get core.
                  CLOSE (UNIT=CONN)
                  RETURN
               ENDIF
 
               IF (LENGTH(CONN)/CHUNK_SIZE.EQ.
     #             (LENGTH(CONN)+MAX_INPUT-1)/CHUNK_SIZE)
     #         THEN
                  ! Record is guaranteed to fit in current chunk.
                  DESCRIPTOR(2) =
     #               CHUNK_ADDRESS(LENGTH(CONN)/CHUNK_SIZE,CONN) +
     #               MOD(LENGTH(CONN),CHUNK_SIZE)
                  EOF=XQ___READ(CONN,DESCRIPTOR,N)
               ELSE
                  ! Record might cross chunk boundary, so read it
                  ! into a temporary buffer and then copy it to the 
                  ! core file.
                  EOF=XQ___READ(CONN,%DESCR(TEMPORARY_BUFFER),N)
                  CALL XQ___MOVE_TO_FILE (CONN,TEMPORARY_BUFFER,
     #                                    LENGTH(CONN),N)
               ENDIF
 
               IF (EOF) GO TO 200
               LENGTH(CONN)=LENGTH(CONN)+N
               IF (CC.NE.'NONE') THEN
                  CALL XQ___MOVE_TO_FILE(CONN,%REF(CRLF),LENGTH(CONN),2)
                  LENGTH(CONN)=LENGTH(CONN)+2
               ENDIF
            ENDDO
      
200         CLOSE (UNIT=CONN)
         ENDIF
      ENDIF
 
      MODIFIED(CONN)=.FALSE.
      STATE(CONN)=STATE_ATTACHED
      STATUS=E$OK
      DQATTACH = CONN
      RETURN
 
900   CONTINUE
910   CONTINUE
      STATUS=E$FACCESS
      RETURN
      END
C-----------------------------------------------------------------------
      LOGICAL*4 FUNCTION XQ___READ (CONN,BUFFER,N)
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      CHARACTER*1 BUFFER
      INTEGER*4 N
 
      READ(CONN,1000,END=200) N,BUFFER(1:N)
1000  FORMAT(Q,A)
 
      XQ___READ=.FALSE.
      RETURN
 
200   XQ___READ=.TRUE.
      N=0
      RETURN
      END
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION DQCREATE (PATH,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      BYTE PATH(*)
      INTEGER*2 STATUS
 
      CHARACTER*45 XQ___PATH,FULLY_QUALIFIED_NAME
      LOGICAL*4 FILE_EXISTS
 
      DQCREATE = 0
 
      DO CONN=CONN_MIN,CONN_MAX
         IF (STATE(CONN).EQ.STATE_UNATTACHED) GO TO 100
      ENDDO
  
      STATUS=E$CONTEXT
      RETURN
 
100   FILENAME(CONN)=XQ___PATH(PATH,STATUS)
      IF (STATUS.NE.E$OK) RETURN
 
      IF (FILENAME(CONN)(1:5).EQ.'WORK:') THEN
         TYPE(CONN)=WORK_FILE
         ACCESS_RIGHTS(CONN)=AR_READ+AR_WRITE+AR_UPDATE
         SEEK_CAPABILITY(CONN)=SC_FORWARD+SC_BACKWARD
     
      ELSEIF (FILENAME(CONN)(1:3).EQ.'BB:') THEN
         TYPE(CONN)=BYTE_BUCKET
         ACCESS_RIGHTS(CONN)=AR_READ+AR_WRITE+AR_UPDATE
         SEEK_CAPABILITY(CONN)=SC_FORWARD+SC_BACKWARD
 
      ELSE
         INQUIRE (FILE=FILENAME(CONN),NAME=FULLY_QUALIFIED_NAME,
     #            EXIST=FILE_EXISTS,ERR=900)
 
         IF (FILENAME(CONN)(1:3).EQ.'CO:' .OR.
     #       FILENAME(CONN)(1:5).EQ.'LIST:' .OR.
     #       FULLY_QUALIFIED_NAME(1:3).EQ.'_TT' .OR.
     #       FULLY_QUALIFIED_NAME.EQ.'SYS$OUTPUT:' .OR.
     #       FULLY_QUALIFIED_NAME.EQ.'SYS$ERROR:') THEN
 
            TYPE(CONN)=INTERACTIVE
            ACCESS_RIGHTS(CONN)=AR_WRITE
            SEEK_CAPABILITY(CONN)=0
            SPECIAL_MODE(CONN)=LINE_EDITED
            OPEN (UNIT=CONN,FILE=FILENAME(CONN),STATUS='UNKNOWN',
     #            ERR=900)
 
         ELSE
            TYPE(CONN)=NORMAL
            ACCESS_RIGHTS(CONN)=AR_DELETE+AR_READ+AR_WRITE+AR_UPDATE
            SEEK_CAPABILITY(CONN)=SC_FORWARD+SC_BACKWARD
         ENDIF
      ENDIF
 
      LENGTH(CONN)=0
      MARKER(CONN)=0
      MODIFIED(CONN)=.FALSE.
      STATE(CONN)=STATE_ATTACHED
      STATUS=E$OK
      DQCREATE=CONN
      RETURN
 
900   STATUS=E$FACCESS
      RETURN
      END
C-----------------------------------------------------------------------
      CHARACTER*45 FUNCTION XQ___PATH (PATH,STATUS)
 
      IMPLICIT INTEGER*2 (A-Z)
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      BYTE PATH(*)
      INTEGER*2 STATUS
 
      XQ___PATH=' '
      N=PATH(1)
 
      IF (N.LE.0 .OR. N.GT.45) THEN
         STATUS=E$SYNTAX
         RETURN
      ENDIF
 
      DO I=1,N
         XQ___PATH(I:I)=CHAR(PATH(I+1))
      ENDDO
 
      IF (XQ___PATH(1:1).EQ.':') XQ___PATH=XQ___PATH(2:)
 
      STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE DQDELETE (PATH,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      BYTE PATH(*)
      INTEGER*2 STATUS
 
      CHARACTER*45 XQ___PATH,FILE
 
      FILE=XQ___PATH(PATH,STATUS)
      IF (STATUS.NE.E$OK) RETURN
 
      OPEN (UNIT=99,FILE=FILE,STATUS='OLD',ERR=900)
      CLOSE (UNIT=99,DISP='DELETE',ERR=950)
 
      STATUS=E$OK
      RETURN
 
900   STATUS=E$FNEXIST
      RETURN
 
950   STATUS=E$FACCESS
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE DQRENAME (OLD,NEW,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
      INCLUDE 'RMSDEF.FOR/NOLIST'
 
      BYTE OLD(*), NEW(*)
      INTEGER*2 STATUS
 
      INTEGER*4 XQ___RENAME,IRMS
      CHARACTER*45 XQ___PATH,OLD_FILE,NEW_FILE
 
      INTEGER*4 RMSCODE(10)
      DATA RMSCODE
     //RMS$_SUC,RMS$_DEV,RMS$_DIR,RMS$_FEX,RMS$_FNF,RMS$_FNM
     ,,RMS$_IDR,RMS$_PRV,RMS$_SUP,RMS$_SYN
     //
      INTEGER*2 UDICODE(10)
      DATA UDICODE
     //E$OK,E$SUPPORT,E$SYNTAX,E$FEXIST,E$FNEXIST,E$SYNTAX
     ,,E$CROSSFS,E$FACCESS,E$SUPPORT,E$SYNTAX
     //
 
      OLD_FILE=XQ___PATH(OLD,STATUS)
      IF (STATUS.NE.E$OK) RETURN
 
      NEW_FILE=XQ___PATH(NEW,STATUS)
      IF (STATUS.NE.E$OK) RETURN
 
      IRMS=XQ___RENAME(OLD_FILE,NEW_FILE)
 
      DO I=1,10
         IF (IRMS.EQ.RMSCODE(I)) THEN
            STATUS=UDICODE(I)
            RETURN
         ENDIF
      ENDDO
 
      CALL LIB$SIGNAL(%VAL(IRMS))
      STATUS=E$FACCESS                  ! For lack of anything better.
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_DETACH (CONN,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 CONN,STATUS
      INTEGER*4 I,N
 
      INTEGER*4 DESCRIPTOR(2)
      DATA DESCRIPTOR /1,0/
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ENDIF
 
      IF (STATE(CONN).EQ.STATE_OPEN) CALL XQ_CLOSE(CONN,STATUS)
 
      STATE(CONN)=STATE_UNATTACHED
 
      IF (TYPE(CONN).EQ.NORMAL) THEN
       
         IF (MODIFIED(CONN)) THEN
            OPEN (UNIT=CONN,FILE=FILENAME(CONN),STATUS='NEW',
     #            FORM='FORMATTED',CARRIAGECONTROL='NONE',
     #            ORGANIZATION='SEQUENTIAL',RECL=OUTPUT_RECL,
     #            RECORDTYPE='VARIABLE',ERR=900)
 
            DO I=0,LENGTH(CONN)-1,OUTPUT_RECL
               N = MIN(LENGTH(CONN)-I,OUTPUT_RECL) ! Next record size.
               IF (I/CHUNK_SIZE.EQ.(I+N-1)/CHUNK_SIZE) THEN
                  ! Next record lies entirely within one chunk,
                  ! so we can write it out directly.
                  DESCRIPTOR(2) =
     #               CHUNK_ADDRESS(I/CHUNK_SIZE,CONN) +
     #               MOD(I,CHUNK_SIZE)
                  CALL XQ___WRITE(CONN,DESCRIPTOR,N)
               ELSE
                  ! Next record crosses a chunk boundary, so first
                  ! copy it to a temporary buffer before writing it out.
                  CALL XQ___MOVE_FROM_FILE(CONN,I,TEMPORARY_BUFFER,N)
                  CALL XQ___WRITE(CONN,%DESCR(TEMPORARY_BUFFER),N)
               ENDIF
            ENDDO
 
            CLOSE (UNIT=CONN)
         ENDIF
 
      ELSEIF (TYPE(CONN).EQ.INTERACTIVE) THEN
         CLOSE (UNIT=CONN)
      ENDIF
 
      STATUS=E$OK
      RETURN
 
900   STATUS=E$FACCESS
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ___WRITE (CONN,BUFFER,N)
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      CHARACTER*1 BUFFER
      INTEGER*4 N
 
      WRITE(CONN,1001) BUFFER(1:N)
1001  FORMAT(A)
      
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_GET CONNECTION STATUS (CONN,INFO,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 CONN,STATUS
      BYTE INFO(7)
 
      INTEGER*4 FILE_PTR
      BYTE FP(4)
      EQUIVALENCE (FILE_PTR,FP)
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ENDIF
 
      FILE_PTR=MARKER(CONN)
 
      INFO(1)=STATE(CONN).EQ.STATE_OPEN
      INFO(2)=ACCESS_RIGHTS(CONN)
      INFO(3)=SEEK_CAPABILITY(CONN)
      INFO(4)=FP(1)
      INFO(5)=FP(2)
      INFO(6)=FP(3)
      INFO(7)=FP(4)
 
      STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_OPEN (CONN,ACCESS,NUMBUF,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 CONN,STATUS
      BYTE ACCESS,NUMBUF
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
      ELSEIF (STATE(CONN).EQ.STATE_OPEN) THEN
         STATUS=E$OPEN
         RETURN
      ELSEIF (ACCESS.LT.1 .OR. ACCESS.GT.3) THEN
         STATUS=E$PARAM
         RETURN
      ENDIF
 
      ACCESS_MODE(CONN)=ACCESS
 
      IF (ACCESS.EQ.AM_WRITE .OR. ACCESS.EQ.AM_UPDATE) THEN
         MODIFIED(CONN)=.TRUE.
      ENDIF
 
      MARKER(CONN)=0
      STATE(CONN)=STATE_OPEN
      STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_SEEK (CONN,MODE,HIGH_OFFSET,LOW_OFFSET,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 CONN,HIGH_OFFSET,LOW_OFFSET,STATUS
      BYTE MODE
 
      INTEGER*4 OFFSET
      INTEGER*2 OFF(2)
      EQUIVALENCE (OFFSET,OFF)
 
      INTEGER*4 I
      LOGICAL*4 XQ___ENSURE_ALLOCATED
 
      BYTE ZEROES(512)
      DATA ZEROES /512*0/
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ELSEIF (STATE(CONN).NE.STATE_OPEN) THEN
         STATUS=E$NOPEN
         RETURN
      ENDIF
  
      IF (TYPE(CONN).EQ.BYTE_BUCKET) GO TO 999
 
      OFF(1)=LOW_OFFSET
      OFF(2)=HIGH_OFFSET
 
      GO TO (100,200,300,400), MODE
      STATUS=E$PARAM
      RETURN
C
C------ MODE 1: SEEK BACKWARD.
C
100   IF ((SEEK_CAPABILITY(CONN).AND.SC_BACKWARD).EQ.0) THEN
         STATUS=E$SUPPORT
         RETURN
      ENDIF
      MARKER(CONN)=MAX(MARKER(CONN)-OFFSET,0)
      GO TO 999
C
C------ MODE 2: SEEK ABSOLUTE.
C
200   IF (SEEK_CAPABILITY(CONN).NE.SC_FORWARD+SC_BACKWARD) THEN
         STATUS=E$SUPPORT
         RETURN
      ENDIF
      MARKER(CONN)=OFFSET
      GO TO 950
C
C------ MODE 3: SEEK FORWARD.
C
300   IF ((SEEK_CAPABILITY(CONN).AND.SC_FORWARD).EQ.0) THEN
         STATUS=E$SUPPORT
         RETURN
      ENDIF
      MARKER(CONN)=MARKER(CONN)+OFFSET
      GO TO 950
C
C------ MODE 4: SEEK BACKWARD FROM END OF FILE.
C
400   IF ((SEEK_CAPABILITY(CONN).AND.SC_BACKWARD).EQ.0) THEN
         STATUS=E$SUPPORT
         RETURN
      ENDIF
      MARKER(CONN)=MAX(LENGTH(CONN)-OFFSET,0)
      GO TO 999
 
C
C------ TEST IF FILE NEEDS TO BE EXTENDED WITH NULLS.
C
950   IF (ACCESS_MODE(CONN).EQ.AM_READ) THEN
         MARKER(CONN)=MIN(MARKER(CONN),LENGTH(CONN))
    
      ELSEIF (MARKER(CONN).GT.LENGTH(CONN)) THEN
         IF (.NOT.XQ___ENSURE_ALLOCATED(CONN,MARKER(CONN)-1)) THEN
            STATUS=E$MEM	! Can't get core.
            MARKER(CONN)=LENGTH(CONN)
            RETURN
         ENDIF
         DO I=LENGTH(CONN),MARKER(CONN)-1,512
            CALL XQ___MOVE_TO_FILE(CONN,ZEROES,I,
     #                             MIN(MARKER(CONN)-I,512))
         ENDDO
         LENGTH(CONN)=MARKER(CONN)
      ENDIF
 
999   STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      INTEGER*2 FUNCTION XQ_READ (CONN,BUF,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
      INCLUDE 'IODEF.FOR/NOLIST'
 
      INTEGER*2 CONN,STATUS
      CHARACTER*(*) BUF
 
      INTEGER*4 N,K
 
      INTEGER*4 NO_TERMINATORS(2), IO_FUNCTION_CODE
      DATA NO_TERMINATORS/0,0/
      INTEGER*2 IOSB(4)
      LOGICAL*4 SS,SYS$ASSIGN
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ELSEIF (STATE(CONN).NE.STATE_OPEN) THEN
         STATUS=E$NOPEN
         RETURN
      ELSEIF (ACCESS_MODE(CONN).EQ.AM_WRITE) THEN
         STATUS=E$OWRITE
         RETURN
      ENDIF
 
      IF (TYPE(CONN).EQ.INTERACTIVE) THEN
         IF (SPECIAL_MODE(CONN).EQ.LINE_EDITED) THEN
            READ(CONN,1002,END=999) N,BUF(1:MIN(N,LEN(BUF)-2))
1002        FORMAT(Q,A)
            K=MIN(N+2,LEN(BUF))
            BUF(K-1:K)=CRLF
 
         ELSE   ! --- TRANSPARENT.
            IF (.NOT.TT_CHANNEL_ASSIGNED) THEN
               SS=SYS$ASSIGN('TT',TT_CHANNEL,,)
               IF (.NOT.SS) CALL LIB$SIGNAL(%VAL(SS))
               TT_CHANNEL_ASSIGNED=.TRUE.
            ENDIF
 
            IO_FUNCTION_CODE=IO$_READVBLK+IO$M_NOECHO+IO$M_NOFILTR
            IF (SPECIAL_MODE(CONN).EQ.TRANSPARENT_NOWAIT) THEN
               IO_FUNCTION_CODE=IO_FUNCTION_CODE+IO$M_TIMED
            ENDIF
 
            CALL SYS$QIOW(,%VAL(TT_CHANNEL),%VAL(IO_FUNCTION_CODE),
     #                    IOSB,,,%REF(BUF),%VAL(LEN(BUF)),%VAL(0),
     #                    %REF(NO_TERMINATORS),,)
            K=IOSB(2)  ! # BYTES ACTUALLY READ.
         ENDIF
 
      ELSEIF (TYPE(CONN).EQ.BYTE_BUCKET) THEN
999      K=0    ! End of file.
 
      ELSE
         K=MIN(LEN(BUF),LENGTH(CONN)-MARKER(CONN))
         CALL XQ___MOVE_FROM_FILE(CONN,MARKER(CONN),%REF(BUF),K)
         MARKER(CONN)=MARKER(CONN)+K
      ENDIF
 
      STATUS=E$OK
      XQ_READ=K
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_WRITE (CONN,BUF,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 CONN,STATUS
      CHARACTER*(*) BUF
 
      INTEGER*4 I
 
      LOGICAL*4 XQ___ENSURE_ALLOCATED
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ELSEIF (STATE(CONN).NE.STATE_OPEN) THEN
         STATUS=E$NOPEN
         RETURN
      ELSEIF (ACCESS_MODE(CONN).EQ.AM_READ) THEN
         STATUS=E$OREAD
         RETURN
      ENDIF
 
      IF (TYPE(CONN).EQ.INTERACTIVE) THEN
         DO I=1,LEN(BUF),80
            WRITE(CONN,1003) BUF(I:MIN(LEN(BUF),I+79))
         ENDDO
1003     FORMAT('+',A,$)
 
      ELSEIF (TYPE(CONN).EQ.BYTE_BUCKET) THEN
         ! NO-OP.
 
      ELSE
         IF (.NOT.XQ___ENSURE_ALLOCATED(CONN,MARKER(CONN)+LEN(BUF)-1)) 
     #   THEN
            STATUS=E$MEM	! Can't get core.
            RETURN
         ENDIF
         CALL XQ___MOVE_TO_FILE(CONN,%REF(BUF),MARKER(CONN),LEN(BUF))
         MARKER(CONN)=MARKER(CONN)+LEN(BUF)
         LENGTH(CONN)=MAX(LENGTH(CONN),MARKER(CONN))
      ENDIF
 
      STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_TRUNCATE (CONN,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 CONN,STATUS
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ELSEIF (STATE(CONN).NE.STATE_OPEN) THEN
         STATUS=E$NOPEN
         RETURN
      ELSEIF (ACCESS_MODE(CONN).EQ.AM_READ) THEN
         STATUS=E$OREAD
         RETURN
      ENDIF
 
      LENGTH(CONN)=MARKER(CONN)
 
      STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_CLOSE (CONN,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 CONN,STATUS
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ELSEIF (STATE(CONN).NE.STATE_OPEN) THEN
         STATUS=E$NOPEN
         RETURN
      ENDIF
 
      STATE(CONN)=STATE_ATTACHED
 
      STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_SPECIAL (TYP,PARAMETER,STATUS)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      BYTE TYP
      INTEGER*2 PARAMETER,STATUS
 
      INTEGER*2 CONN
 
      GO TO (100,200,300), TYP
      STATUS=E$PARAM
      RETURN
 
100   CONTINUE
200   CONTINUE
300   CONTINUE
      CONN=PARAMETER
 
      IF (CONN.LT.CONN_MIN .OR. CONN.GT.CONN_MAX) THEN
         STATUS=E$PARAM
         RETURN
      ELSEIF (STATE(CONN).EQ.STATE_UNATTACHED) THEN
         STATUS=E$EXIST
         RETURN
      ELSEIF (TYPE(CONN).NE.INTERACTIVE) THEN
         STATUS=E$SUPPORT
         RETURN
      ENDIF
 
      SPECIAL_MODE(CONN)=TYP
      STATUS=E$OK
      RETURN
      END
C-----------------------------------------------------------------------
      SUBROUTINE XQ_EXIT (COMPLETION_CODE)
 
      INCLUDE 'XQCOMMON.FOR/NOLIST'
      INCLUDE 'EXCEPT.FOR/NOLIST'
 
      INTEGER*2 COMPLETION_CODE
      INTEGER*2 STATUS
 
      EXTERNAL UDI_OK,UDI_WARNINGS,UDI_ERRORS,UDI_FATAL,UDI_ABORT
      EXTERNAL UDI_BADINDSYN,UDI_INDNOTLAS,UDI_BADINDFIL,UDI_INDTOOBIG
 
      DO CONN=CONN_MIN,CONN_MAX
         IF (STATE(CONN).NE.STATE_UNATTACHED) THEN
            CALL XQ_DETACH(CONN,STATUS)
         ENDIF
      ENDDO
 
      GO TO (1,2,3,4), COMPLETION_CODE+1
      GO TO (101,102,103,104), COMPLETION_CODE-100
      CALL LIB$SIGNAL(UDI_ABORT)
1     CALL EXIT
2     CALL LIB$SIGNAL(UDI_WARNINGS)
      CALL EXIT
3     CALL LIB$SIGNAL(UDI_ERRORS)
      CALL EXIT
4     CALL LIB$SIGNAL(UDI_FATAL)
      CALL EXIT
101   CALL LIB$SIGNAL(UDI_BADINDSYN)
      CALL EXIT
102   CALL LIB$SIGNAL(UDI_INDNOTLAS)
      CALL EXIT
103   CALL LIB$SIGNAL(UDI_BADINDFIL)
      CALL EXIT
104   CALL LIB$SIGNAL(UDI_INDTOOBIG)
      CALL EXIT
      
      END
      LOGICAL*4 FUNCTION XQ___ENSURE_ALLOCATED (CONN, BYTE_INDEX)
C-----------------------------------------------------------------------
C
C    This function is called to ensure that enough core is allocated
C    to contain bytes 0..BYTE_INDEX for connection CONN.
C
C    Returns .TRUE. if enough core is allocated.
C    Returns .FALSE. if core not available, or chunk table size
C                    would be exceeded.
C
C    Assumes CONN is a valid connection number.
C    Assumes BYTE_INDEX > 0.
C    Assumes chunks are consecutively allocated from chunk 0 up.
C
C-----------------------------------------------------------------------
      INCLUDE 'XQCOMMON.FOR/NOLIST'
 
      INTEGER*2 CONN            ! Connection token.
      INTEGER*4 BYTE_INDEX	! Highest byte index necessary to be
				! allocated.
 
      INTEGER*4 CHUNK
      LOGICAL*4 LIB$GET_VM
 
      IF (BYTE_INDEX.GE.MAX_CORE_FILE_SIZE) THEN
         XQ___ENSURE_ALLOCATED=.FALSE.      ! Chunk table size exceeded.
         RETURN
      ENDIF
 
      IF (CHUNK_ADDRESS(BYTE_INDEX/CHUNK_SIZE,CONN).NE.0) THEN
         XQ___ENSURE_ALLOCATED=.TRUE.       ! Already allocated.
         RETURN
      ENDIF
 
      ! Allocate any missing chunks, up through the highest one needed.
 
      DO CHUNK=0,BYTE_INDEX/CHUNK_SIZE
         IF (CHUNK_ADDRESS(CHUNK,CONN).EQ.0) THEN
            IF (.NOT.LIB$GET_VM(CHUNK_SIZE,CHUNK_ADDRESS(CHUNK,CONN)))
     #      THEN
               XQ___ENSURE_ALLOCATED=.FALSE. ! Can't get core.
               CHUNK_ADDRESS(CHUNK,CONN)=0
               RETURN
            ENDIF
         ENDIF
      ENDDO
 
      XQ___ENSURE_ALLOCATED=.TRUE.        ! Successfully allocated core.
      RETURN
      END
      SUBROUTINE XQ___MOVE_TO_FILE (CONN, BUFFER, BYTE_INDEX, N_BYTES)
C-----------------------------------------------------------------------
C
C    This subroutine is called to copy a bufferful of bytes into
C    a core file.
C
C    Assumes N_BYTES < 64K.
C    Assumes the necessary core in the core file has already been
C     allocated.
C
C-----------------------------------------------------------------------
      INCLUDE 'XQCOMMON.FOR/NOLIST'
 
      INTEGER*2 CONN		! Connection token.
      BYTE BUFFER(0:*)		! Buffer to move from.
      INTEGER*4 BYTE_INDEX	! Index in core file to start copying to.
      INTEGER*4 N_BYTES		! Number of bytes to move (< 64K).
 
      INTEGER*4 I,N,K,START_INDEX
 
      I = 0                     ! Index into buffer.
      N = N_BYTES               ! Number of bytes left to move.
      START_INDEX = BYTE_INDEX	! Core file index to start next move.
 
      DO WHILE (N.GT.0)
         K = MIN(N, CHUNK_SIZE-MOD(START_INDEX,CHUNK_SIZE))
         ! Max bytes we can transfer without crossing chunk boundary.
         CALL PLM$MOVE (%VAL(K),
     #              BUFFER(I),
     #              %VAL(CHUNK_ADDRESS(START_INDEX/CHUNK_SIZE,CONN)+
     #               MOD(START_INDEX,CHUNK_SIZE)))
         I = I+K
         START_INDEX = START_INDEX+K
         N = N-K
      ENDDO
      RETURN
      END
      SUBROUTINE XQ___MOVE_FROM_FILE (CONN, BYTE_INDEX, BUFFER, N_BYTES)
C-----------------------------------------------------------------------
C
C    This subroutine is called to copy a bufferful of bytes out of a
C    core file
C
C    Assumes N_BYTES < 64K.
C    Assumes the necessary core in the core file is already allocated.
C
C-----------------------------------------------------------------------
      INCLUDE 'XQCOMMON.FOR/NOLIST'
 
      INTEGER*2 CONN		! Connection token.
      INTEGER*4 BYTE_INDEX	! Index in core file to start copying
				!  from.
      BYTE      BUFFER(0:*)	! Buffer to move to.
      INTEGER*4 N_BYTES		! Number of bytes to move (< 64K).
 
      INTEGER*4 I,N,K,START_INDEX
 
      I = 0			! Index into buffer.
      N = N_BYTES		! Number of bytes left to move.
      START_INDEX = BYTE_INDEX  ! Core file index to start next move.
 
      DO WHILE (N.GT.0)
         K = MIN(N,CHUNK_SIZE-MOD(START_INDEX,CHUNK_SIZE))
         ! Max bytes we can transfer without crossing chunk boundary.
         CALL PLM$MOVE (%VAL(K),
     #              %VAL(CHUNK_ADDRESS(START_INDEX/CHUNK_SIZE,CONN)+
     #               MOD(START_INDEX,CHUNK_SIZE)),
     #              BUFFER(I))
         I = I+K
         START_INDEX = START_INDEX+K
         N = N-K
      ENDDO
      RETURN
      END
