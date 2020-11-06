* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                             *
* TIMELOG.CMD                                                                 *
* This program provides choices for timelog reports.  It set up the output    *
* files to which each report will be written and also sets up the variables   *
* which will be passed to the specific report generator.                      *
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
ERASE
USE timelogx
SET INTENSITY OFF
DO WHILE t
   ERASE
   @ 1,0 SAY 'Here are your options for timelog reports:'
   @ 2,5 SAY '1 = Report access by a specific account and specific device'
   @ 3,5 SAY '2 = Report access by a specific account, all devices'
   @ 4,5 SAY '3 = Report access by a specific device, all accounts'
   @ 5,5 SAY '4 = Report access by all accounts & devices, subtotal by device'
   @ 6,5 SAY '5 = Report access by all devices & accounts, subtotal by account'
   @ 7,5 SAY '6 = Return to the operating system'
   STORE ' ' TO which
   DO WHILE which = ' '
      @ 8,0 SAY 'Input your choice ' GET which PICTURE '9'
      READ
      IF VAL(which) < 1 .OR. VAL(which) > 6
         STORE ' ' TO which
      ENDIF
   ENDDO
   IF which = '6'
      QUIT
   ENDIF
   STORE f TO gomenu
   STORE '         ' TO account
   STORE '  ' TO device
   IF VAL(which) < 4
      IF VAL(which) < 3
         DO WHILE account = '         ' .AND. (.NOT. gomenu)
            @ 10,0 SAY 'Which specific account? ' GET account
            READ
            @ 10,40
            IF account = '         '
               STORE t TO gomenu
            ELSE
               SET INDEX TO namecon
               FIND '&account'
               IF # = 0
                  @ 10,40 SAY 'No time recorded for that account.'
                  STORE '         ' TO account
               ENDIF
            ENDIF
         ENDDO
      ENDIF
      IF VAL(which) <> 2
         DO WHILE device = '  ' .AND. (.NOT. gomenu)
            @ 11,0 SAY 'Which specific device? ' GET device PICTURE '99'
            READ
            @ 11,40
            IF device = '  '
               STORE t TO gomenu
            ELSE
               STORE VAL(device) TO x
               STORE STR(x,2) TO device
               RELEASE x
               IF which = '3'
                  SET INDEX TO conname
                  FIND '&device'
               ELSE
                  STORE account+device TO x
                  FIND '&x'
                  RELEASE x
               ENDIF
               IF # = 0
                  @ 11,40 SAY 'No time recorded for that device.'
                  STORE '         ' TO device
               ENDIF
            ENDIF
         ENDDO
      ENDIF
   ENDIF
   STORE ' ' TO how
   DO WHILE how = ' ' .AND. (.NOT. gomenu)
      @ 13,5 SAY '1 = List all access times'
      @ 14,5 SAY '2 = List totals only'
      @ 15,0 SAY 'Your choice ' GET how PICTURE '9'
      READ
      IF how = ' '
         STORE t TO gomenu
      ENDIF
      IF how <> '1' .AND. how <> '2'
         STORE ' ' TO how
      ENDIF
   ENDDO
   STORE '        ' TO output
   DO WHILE output = '        ' .AND. (.NOT. gomenu)
      @ 17,0 SAY 'Input the name of the file to which this report'
      @ 18,0 SAY 'will be written (e.g. timelog1 or fred12) ' GET output
      READ
      @ 20,0
      STORE !(output) TO output
      IF output = ' '
         STORE t TO gomenu
      ELSE
         STORE TRIM(output)+'.TXT' TO filechek
         IF FILE(filechek)
            STORE ' ' TO killit
            DO WHILE killit <> 'Y' .AND. killit <> 'N'
               @ 20,0 SAY output+' already exists. Overwrite it? (Y/N) ' GET killit PICTURE '!'
               READ
            ENDDO
            IF killit = 'Y'
               @ 20,0
               DELETE FILE &filechek
            ELSE
               STORE '        ' TO output
            ENDIF
         ENDIF
      ENDIF
   ENDDO
   IF .NOT.(gomenu)
      SET ALTERNATE TO &OUTPUT
      ERASE
      DO CASE
      CASE which = '1'
         SET INDEX TO namecon
         DO timelog1
      CASE which = '2'
         SET INDEX TO namecon
         DO timelog2
      CASE which = '3'
         SET INDEX TO conname
         DO timelog3
      CASE which = '4'
         SET INDEX TO namecon
         DO timelog4
      CASE which = '5'
         SET INDEX TO conname
         DO timelog5
      ENDCASE
      SET ALTERNATE TO
   ENDIF
ENDDO
