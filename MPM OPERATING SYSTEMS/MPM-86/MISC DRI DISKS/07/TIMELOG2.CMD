* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                             *
* TIMELOG2.CMD                                                                *
* This command file is called by TIMELOG.CMD and prints a time accounting     *
* report to a file specified by the user. The report contains access data for *
* one specific account on any terminal. The command file returns control to   *
* TIMELOG.CMD                                                                 *
*                                                                             *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                 *                                           *
*  Written By Michael Burch       *  Copyright (C) G&G Engineering 1982       *
*  For Gifford Computer Systems   *  Reproduction or modification of this     *
*  Date: November 17, 1982        *  program, in whole or in part, only with  *
*  Last Revision: 04/12/83        *  written permission from G&G Engineering  *
*                                 *                                           *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
FIND '&account'
STORE 'SunMonTueWedThuFriSat' TO weekday
STORE 0 TO dev:kount
STORE 0 TO dev:time
STORE 0 TO all:kount
STORE 0 TO all:time
STORE name TO account
STORE con TO device
STORE tty TO device1
SET ALTERNATE ON
IF how = '2'
   ? 'Account   Device/Name'
   ? '========= ============'
ELSE
   ? 'Account   Device/Name         Log In                 Log Out           Elapsed'
   ? '========= ============ =====================  =====================       Time'
ENDIF
STORE 2 TO line
DO WHILE .NOT. EOF .AND. account = name
   IF line > 56
      DO WHILE line < 66
         ?
         STORE line+1 TO line
      ENDDO
      IF how = '2'
         ? 'Account   Device/Name'
         ? '========= ============'
      ELSE
         ? 'Account   Device/Name         Log In                 Log Out           Elapsed'
         ? '========= ============ =====================  =====================       Time'
      ENDIF
      STORE 2 TO line
   ENDIF
   IF how = '1'
      ? name+' '+con+'/'+tty+' '+hour:i+':'+min:i+':'+sec:i
      ?? $(weekday,VAL(week:i)*3+1,3)+' '+month:i+'/'+day:i+'/'+$(year:i,3,2)
      ?? ' '+hour:o+':'+min:o+':'+sec:o+' '+$(weekday,VAL(week:o)*3+1,3)
      ?? month:o+'/'+day:o+'/'+$(year:o,3,2)+' '+time:hr+':'+time:min+':'+time:sec
      STORE line+1 TO line
   ENDIF
   STORE dev:kount+1 TO dev:kount
   STORE dev:time+total:sec TO dev:time
   SKIP
   IF device <> con .OR. account <> name .OR. EOF
      IF how = '1'
         ?
         STORE line+1 TO line
      ENDIF
      STORE STR(dev:time/3600,4)+':' TO tot:time
      STORE tot:time+STR((dev:time-INT(dev:time/3600)*3600)/60,2)+':' TO tot:time
      IF $(tot:time,6,1) = ' '
         STORE $(tot:time,1,5)+'0'+$(tot:time,7,2) TO tot:time
      ENDIF
      STORE tot:time+STR(dev:time-INT(dev:time/60)*60,2) TO tot:time
      IF $(tot:time,9,1) = ' '
         STORE $(tot:time,1,8)+'0'+$(tot:time,10,2) TO tot:time
      ENDIF
      ? account+' '+device+'/'+device1+'        '+STR(dev:kount,5)+' Accesses'
      ?? '  Total elapsed time = '+tot:time
      ?
      STORE line+2 TO line
      STORE con TO device
      STORE tty TO device1
      STORE dev:kount+all:kount TO all:kount
      STORE dev:time+all:time TO all:time
      STORE 0 TO dev:kount
      STORE 0 TO dev:time
      IF account = name .AND. (.NOT. EOF)
         IF line > 56
            DO WHILE line < 66
               ?
               STORE line+1 TO line
            ENDDO
            IF how = '2'
               ? 'Account   Device/Name'
               ? '========= ============'
            ELSE
               ? 'Account   Device/Name         Log In                 Log Out           Elapsed'
               ? '========= ============ =====================  =====================       Time'
            ENDIF
            STORE 2 TO line
         ELSE
            IF how = '1'
               ?
               ? 'Account   Device/Name         Log In                 Log Out           Elapsed'
               ? '========= ============ =====================  =====================       Time'
               STORE line+3 TO line
            ENDIF
         ENDIF
      ENDIF
   ENDIF
ENDDO
STORE STR(all:time/3600,4)+':' TO tot:time
STORE tot:time+STR((all:time-INT(all:time/3600)*3600)/60,2)+':' TO tot:time
IF $(tot:time,6,1) = ' '
   STORE $(tot:time,1,5)+'0'+$(tot:time,7,2) TO tot:time
ENDIF
STORE tot:time+STR(all:time-INT(all:time/60)*60,2) TO tot:time
IF $(tot:time,9,1) = ' '
   STORE $(tot:time,1,8)+'0'+$(tot:time,10,2) TO tot:time
ENDIF
? account+' All Devices         '+STR(all:kount,5)+' Accesses   Total elapsed'
?? 'time = '+tot:time
STORE line+1 TO line
DO WHILE line < 66
   ?
   STORE line+1 TO line
ENDDO
SET ALTERNATE OFF
RETURN
