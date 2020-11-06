$SET VERIFY
$!		MAKETAPE.COM
$!
$!
$!  Command file to generate the build-it-from-source kit
$!  for the PL/M-VAX runtime library (including the UDI
$!  routines).
$!
$!  05FEB82  Alex Hunter  1. Original version.
$!  06APR82  Alex Hunter  1. Allocate MTA0 instead of MT.
$!
$ALLOCATE MTA0 TAPE
$INIT/DENSITY=1600 TAPE PLMUDI
$MOUNT TAPE PLMUDI
$COPY/LOG *.* TAPE
$DIR/SIZ/DAT TAPE
$DISMOUNT TAPE
$DEALLOCATE TAPE
$SET NOVERIFY
