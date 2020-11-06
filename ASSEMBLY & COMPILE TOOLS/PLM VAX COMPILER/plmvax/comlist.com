$SET VERIFY
$!
$!		COMLIST.COM
$!
$!  Command file to produce short listings for the PL/M-VAX
$!  compiler.
$!
$!  02FEB82  Alex Hunter  1. Original version.
$!
$PRI/HEAD *.FOR
$PRI CONTROL
$SET NOVERIFY
