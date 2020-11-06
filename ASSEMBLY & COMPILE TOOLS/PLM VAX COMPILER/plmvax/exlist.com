$SET VERIFY
$!
$!              EXLIST.COM
$!
$!  Command file to produce listings for the export version
$!  of the PL/M-VAX compiler.
$!
$!  02FEB82  Alex Hunter  1. Deleted PLM$UDI listings.
$!
$PRI/HEAD *.FOR
$PRI CONTROL
$PRI/HEAD PLM.BLD,.CMP,.LNK
$SET NOVERIFY
