* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                             *
* TIMELOGX.CMD                                                                *
* This program is called by TIMELOG.SUB. It reads the system WTMP file into   *
* the WTMP.DBF data base for processing in TIMELOGY.CMD which is the next     *
* command file to be called by the submit file.                               *
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
USE wtmp
APPEND FROM wtmp. SDF
QUIT
