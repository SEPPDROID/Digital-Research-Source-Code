* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                             *
* TIMELOG1.CMD                                                                *
* This command file is called by TIMELOG.CMD and will return to TIMELOG.CMD   *
* when it is finished.  It prints a time accounting report to a file          *
* specified by the user. This report displays access data for one specific    *
* account on one specific terminal.                                           *
*                                                                             *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                 *                                           *
*  Written By Michael Burch       *  Copyright (C) G&G Engineering 1982       *
*  For Gifford Computer Systems   *  Reproduction or modification of this     *
*  Date: November 17, 1982        *  program, in whole or in part, only with  *
*  Last Revision: 04/12/83        *  written permission from G&G Engineering  *
*                                 *                                           *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
STORE account+device TO key
FIND '&key'
STORE 'SunMonTueWedThuFriSat' TO weekday
STORE 0 TO kount
STORE 0 TO total:time
STORE name TO account
STORE con TO device
STORE tty TO device1
SET ALTERNATE ON
IF how = '2'
   ? 'Account   Terminal'
   ? '========= ============'
ELSE
   ? 'Account   Terminal            Log In                 Log Out           Elapsed'
   ? '========= ============ =====================  =====================       Time'
ENDIF
STORE 2 TO line
DO WHILE .NOT. EOF .AND. account = name .AND. device = con
   IF line > 56
      DO WHILE line < 66
         ?
         STORE line+1 TO line
      ENDDO
      IF how = '2'
	 ? 'Account   Terminal'
         ? '========= ============'
      ELSE
         ? 'Account   Terminal            Log In                 Log Out           Elapsed'
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
   STORE kount+1 TO kount
   STORE total:time+total:sec TO total:time
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
IF how = '1'
   ?
   STORE line+1 TO line
ENDIF
?
? account+' '+device+'/'+device1+'       '
?? STR(kount,5)+' Accesses   Total elapsed time = '+tot:time
STORE line+2 TO line
DO WHILE line < 66
   ?
   STORE line+1 TO line
ENDDO
SET ALTERNATE OFF
RETURN
