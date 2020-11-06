* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                             *
* TIMELOGZ.CMD                                                                *
* This command file will, upon the user's request, print all the login and    *
* logout times for all the users.  This is for the the user's convenience. If *
* the user would rather not have2 this list, he/she can simply skip it. This  *
* command file is called by TIMELOG.SUB and should turn control over to       *
* TIMELOG.CMD for a report menu.                                              *
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
STORE 1 TO scr:line
SET INTENSITY OFF
@ scr:line,1 SAY 'You now have the option of printing a report of the raw TIMELOG data'
STORE scr:line+1 TO scr:line
@ scr:line,1 SAY 'to a file. This file may then be printed or edited at your convenience.'
STORE scr:line+1 TO scr:line
@ scr:line,1 SAY 'This step is not mandatory. You may skip this step by inputting "N" or'
STORE scr:line+1 TO scr:line
@ scr:line,1 SAY 'you may continue by inputting "Y" '
STORE scr:line+2 TO scr:line
STORE ' ' TO go
DO WHILE go <> 'Y' .AND. go <> 'N'
   @ scr:line,1 SAY 'Input Y or N here ' GET go PICTURE '!'
   READ
ENDDO
STORE scr:line+3 TO scr:line
IF go = 'N'
   DO timelog
ENDIF
USE timelogx
STORE 'SunMonTueWedThuFriSat' TO weekday
STORE total:sec TO max
STORE total:sec TO min
STORE time:hr+':'+time:min+':'+time:sec TO max:time
STORE time:hr+':'+time:min+':'+time:sec TO min:time
STORE 0 TO kount
STORE 0 TO total:time
STORE '        ' TO output
DO WHILE output = '        '
   @ scr:line,1 SAY 'Input the name of the file to which this report will be written ' GET output PICTURE '!!!!!!!!'
   READ
   STORE scr:line+1 TO scr:line
   IF output <> '        ' .OR. @('.',output)=0
      STORE TRIM(output)+'.TXT' TO filechek
      IF FILE(filechek)
         STORE ' ' TO killit
         DO WHILE killit <> 'Y' .AND. killit <> 'N'
            @ scr:line,1 SAY output+' already exists. Overwrite it? (Y/N) ' GET killit PICTURE '!'
            READ
         ENDDO
         STORE scr:line+2 TO scr:line
         IF killit = 'Y'
            DELETE FILE &filechek
         ELSE
            STORE scr:line-3 TO scr:line
            STORE '        ' TO output
         ENDIF
      ENDIF
   ELSE
      STORE scr:line-1 TO scr:line
      STORE '        ' TO output
   ENDIF
ENDDO
SET ALTERNATE TO &OUTPUT
@ scr:line,1 SAY 'Raw TIMELOG data is now being written to '+filechek
SET INTENSITY ON
SET CONSOLE OFF
SET ALTERNATE ON
?
? 'Account   Device/Name         Log In                 Log Out           Elapsed'
? '========= ============ =====================  =====================       Time'
?
STORE 3 TO line
DO WHILE .NOT. EOF
   IF line > 56
      DO WHILE line < 66
         ?
         STORE line+1 TO line
      ENDDO
      ? 'Account   Device/Name         Log In                 Log Out           Elapsed'
      ? '========= ============ =====================  =====================       Time'
      ?
      STORE 3 TO line
   ENDIF
   ? name+' '+con+'/'+tty+' '+hour:i+':'+min:i+':'+sec:i+' '+$(weekday,VAL(week:i)*3+1,3)
   ?? month:i+'/'+day:i+'/'+$(year:i,3,2)+'  '+hour:o+':'+min:o+':'+sec:o
   ?? $(weekday,VAL(week:o)*3+1,3)+' '+month:o+'/'+day:o+'/'+$(year:o,3,2)
   ?? time:hr+':'+time:min+':'+time:sec
   STORE line+1 TO line
   STORE kount+1 TO kount
   STORE total:time+total:sec TO total:time
   IF total:sec < min
      STORE total:sec TO min
      STORE time:hr+':'+time:min+':'+time:sec TO min:time
   ENDIF
   IF total:sec > max
      STORE total:sec TO max
      STORE time:hr+':'+time:min+':'+time:sec TO max:time
   ENDIF
   SKIP
ENDDO
STORE STR(total:time/3600,4)+':' TO tot:time
STORE tot:time+STR((total:time-INT(total:time/3600)*3600)/60,2)+':' TO tot:time
IF $(tot:time,6,1) = ' '
   STORE $(tot:time,1,5)+'0'+$(tot:time,7,2) TO tot:time
ENDIF
STORE tot:time+STR(total:time-INT(total:time/60)*60,2) TO tot:time
IF $(tot:time,9,1) = ' '
   STORE $(tot:time,1,8)+'0'+$(tot:time,10,2) TO tot:time
ENDIF
STORE total:time/kount TO total:time
STORE STR(total:time/3600,4)+':' TO avg:time
STORE avg:time+STR((total:time-INT(total:time/3600)*3600)/60,2)+':' TO avg:time
IF $(avg:time,6,1) = ' '
   STORE $(avg:time,1,5)+'0'+$(avg:time,7,2) TO avg:time
ENDIF
STORE avg:time+STR(total:time-INT(total:time/60)*60,2) TO avg:time
IF $(avg:time,9,1) = ' '
   STORE $(avg:time,1,8)+'0'+$(avg:time,10,2) TO avg:time
ENDIF
?
? 'Number of "log in"s =      '+STR(kount,5)+'            '+'Shortest elapsed time = '+ min:time
? 'Total elapsed time  = '+tot:time+'             '+'Longest elapsed time = '+max:time
? '                                             Average elapsed time = '+avg:time
STORE line+4 TO line
DO WHILE line < 66
   ?
   STORE line+1 TO line
ENDDO 
SET ALTERNATE OFF
SET CONSOLE ON
DO timelog
