* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                             *
* TIMELOGY.CMD                                                                *
* This command file follows TIMELOGX.CMD as they are called by the submit     *
* file TIMELOG.SUB.  It uses the info in WTMP.DBF to create TIMELOGX.DBF,     *
* which contains all the data needed to print time accounting reports later.  *
* User login times are matched to their corresponding logout times and the    *
* elapsed time is calculated.  This info may be added to previously logged    *
* data or it may replace the previous info. This command file expects to      *
* return control to the submit file and call TIMELOGZ.CMD subsequently.       *
*                                                                             *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                 *                                           *
*  Written By Michael Burch       *  Copyright (C) G&G Engineering 1982       *
*  For Gifford Computer Systems   *  Reproduction or modification of this     *
*  Date: November 17, 1982        *  program, in whole or in part, only with  *
*  Last Revision: 04/12/83        *  written permission from G&G Engineering  *
*                                 *                                           *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
SET TALK OFF
STORE ' ' TO choice
DO WHILE choice <> '1' .AND. choice <> '2'
   ERASE
   STORE ' ' TO choice
   @ 5,5 SAY 'Would you like to:'
   @ 7,5 SAY '1 = Add current log to previous log'
   @ 8,9 SAY 'or'
   @ 9,5 SAY '2 = Purge the previous log and keep only the current log'
   @ 11,5 SAY 'Please choose 1 or 2 (Default is 1) ' GET choice PICTURE '9'
   READ
   IF choice = ' '
      STORE '1' TO choice
   ENDIF
ENDDO
@ 13,0 SAY 'Preparing login/logout data.'
@ 14,0 SAY 'Please stand by.'
SELECT PRIMARY
USE wtmp
STORE length TO len:wtmp
DELETE ALL FOR length > len:wtmp
PACK
COPY STRUCTURE TO wtmpx
SELECT SECONDARY
STORE 'timelogx' TO filename
IF choice = '2' .OR. (.NOT. FILE(filename))
   USE timelog
   COPY STRUCTURE TO timelogx
ENDIF
USE timelogx
INDEX ON mpm:day:o+tty+con TO timelogx
SET INDEX TO timelogx
SELECT PRIMARY
LOCATE FOR name <> '         '
STORE 0 TO openusers
DO WHILE .NOT. EOF
   DO CASE
   CASE name = 'Boot' .AND. openusers > 0
*
*     This is the routine to replace all open accounts with the last recorded
*     up time.
*
      SKIP - 1
      SELECT SECONDARY
      STORE t TO found
      DO WHILE found
         FIND '     '
         IF # = 0
            STORE f TO found
         ELSE
            STORE t TO found
            REPLACE mpm:day:o WITH mpm:day, hour:o WITH hour, min:o WITH min
            REPLACE sec:o WITH sec, jday:o WITH jday, week:o WITH week
            REPLACE month:o WITH month, day:o WITH day, year:o WITH year
            STORE VAL(mpm:day:i) TO day:in
            STORE VAL(mpm:day:o) TO day:out
            STORE VAL(hour:i) TO hour:in
            STORE VAL(hour:o) TO hour:out
            STORE VAL(min:i) TO min:in
            STORE VAL(min:o) TO min:out
            STORE (((hour:in*60)+min:in)*60)+VAL(sec:i) TO sec:in
            STORE (((hour:out*60)+min:out)*60)+VAL(sec:o) TO sec:out
            IF sec:in > sec:out
               STORE day:out-1 TO day:out
               STORE sec:out+86400 TO sec:out
            ENDIF
            REPLACE total:sec WITH sec:out-sec:in+(day:out-day:in)*86400
            REPLACE time:hr WITH STR(total:sec/3600,4)
            REPLACE time:min WITH STR((total:sec-(VAL(time:hr)*3600))/60,2)
            REPLACE time:sec WITH STR(total:sec-VAL(time:hr)*3600-VAL(time:min)*60,2)
            STORE openusers-1 TO openusers
         ENDIF
      ENDDO
      SELECT PRIMARY
    CASE name = '         '
*
*     This is the routine to match a user's logout time with his
*     corresponding login time
*
      SELECT SECONDARY
      FIND '     '+p.tty+p.con
      IF # <> 0
         REPLACE mpm:day:o WITH mpm:day, hour:o WITH hour, min:o WITH min
         REPLACE sec:o WITH sec, jday:o WITH jday, week:o WITH week
         REPLACE month:o WITH month, day:o WITH day, year:o WITH year
         STORE VAL(mpm:day:i) TO day:in
         STORE VAL(mpm:day:o) TO day:out
         STORE VAL(hour:i) TO hour:in
         STORE VAL(hour:o) TO hour:out
         STORE VAL(min:i) TO min:in
         STORE VAL(min:o) TO min:out
         STORE (((hour:in*60)+min:in)*60)+VAL(sec:i) TO sec:in
         STORE (((hour:out*60)+min:out)*60)+VAL(sec:o) TO sec:out
         IF sec:in > sec:out
            STORE day:out-1 TO day:out
            STORE sec:out+86400 TO sec:out
         ENDIF
         REPLACE total:sec WITH sec:out-sec:in+(day:out-day:in)*86400
         REPLACE time:hr WITH STR(total:sec/3600,4)
         REPLACE time:min WITH STR((total:sec-VAL(time:hr)*3600)/60,2)
         REPLACE time:sec WITH STR(total:sec-VAL(time:hr)*3600-VAL(time:min)*60,2)
         STORE openusers-1 TO openusers
      ENDIF
      SELECT PRIMARY
    OTHERWISE
*
*     This is the routine to add a user's login time
*
      SELECT SECONDARY
      APPEND BLANK
      REPLACE s.tty WITH p.tty, s.name WITH p.name, s.con WITH p.con
      REPLACE mpm:day:i WITH mpm:day,hour:i WITH hour, min:i WITH min
      REPLACE sec:i WITH sec, jday:i WITH jday, week:i WITH week
      REPLACE month:i WITH month, day:i WITH day, year:i WITH year
      STORE openusers+1 TO openusers
      SELECT PRIMARY
   ENDCASE
   SKIP
ENDDO
SELECT SECONDARY
SET INDEX TO
DELETE FILE timelogx.ndx
REPLACE ALL hour:i WITH '0'+$(hour:i,2,1) FOR $(hour:i,1,1) = ' '
REPLACE ALL min:i WITH '0'+$(min:i,2,1) FOR $(min:i,1,1) = ' '
REPLACE ALL sec:i WITH '0'+$(sec:i,2,1) FOR $(sec:i,1,1) = ' '
REPLACE ALL month:i WITH '0'+$(month:i,2,1) FOR $(month:i,1,1) = ' '
REPLACE ALL day:i WITH '0'+$(day:i,2,1) FOR $(day:i,1,1) = ' '
REPLACE ALL hour:o WITH '0'+$(hour:o,2,1) FOR $(hour:o,1,1) = ' '
REPLACE ALL min:o WITH '0'+$(min:o,2,1) FOR $(min:o,1,1) = ' '
REPLACE ALL sec:o WITH '0'+$(sec:o,2,1) FOR $(sec:o,1,1) = ' '
REPLACE ALL month:o WITH '0'+$(month:o,2,1) FOR $(month:o,1,1) = ' '
REPLACE ALL day:o WITH '0'+$(day:o,2,1) FOR $(day:o,1,1) = ' '
REPLACE ALL time:min WITH '0'+$(time:min,2,1) FOR $(time:min,1,1) = ' '
REPLACE ALL time:sec WITH '0'+$(time:sec,2,1) FOR $(time:sec,1,1) = ' '
STORE 1 TO num
IF openusers > 0
   SELECT SECONDARY
   GOTO TOP
   COPY TO timelog FOR mpm:day:o = '     '
   DELETE ALL FOR mpm:day:o = '     '
   PACK
   USE wtmpx
   SELECT PRIMARY
   USE timelog
   DO WHILE .NOT. EOF
      SELECT SECONDARY
      APPEND BLANK
      REPLACE s.tty WITH p.tty, s.name WITH p.name, s.con WITH p.con
      REPLACE mpm:day WITH mpm:day:i, hour WITH hour:i, min WITH min:i
      REPLACE sec WITH sec:i, jday WITH jday:i, week WITH week:i
      REPLACE month WITH month:i, day WITH day:i, year WITH year:i
      REPLACE length WITH STR(num,5)
      SELECT PRIMARY
      STORE num+1 TO num
      SKIP
   ENDDO
   STORE num-1 TO num
ENDIF
SELECT SECONDARY
USE
SELECT PRIMARY
USE timelogx
COPY STRUCTURE TO timelog
INDEX ON name+con TO namecon
INDEX ON con+name TO conname
USE wtmpx
IF openusers = 0
   APPEND BLANK
ENDIF
COPY STRUCTURE TO wtmp
USE wtmp
APPEND FROM wtmp. SDF
GOTO TOP
STORE length TO end
USE wtmpx
APPEND FROM wtmp. SDF FOR length > len:wtmp .AND. length <= end
COPY STRUCTURE TO wtmp
GOTO num
DO WHILE .NOT. EOF
   REPLACE length WITH STR(num,5)
   STORE num+1 TO num
   SKIP
ENDDO
STORE num-1 TO num
GOTO TOP
REPLACE length WITH STR(num,5)
COPY TO wtmp. SDF
QUIT
