$!		ERRFIND.COM
$!
$!  Command file to search a PL/M-VAX source file and display all
$!  calls to the ERROR message subroutines.
$!  (Requires the WYLBUR text editor.)
$!
$!  02FEB82  Alex Hunter  1. Original version.
$!
$USE 'P1'.FOR
L 'CALL ERROR' OR 'CALL FATAL' OR 'CALL WARN' OR 'CALL BUG'
LO
