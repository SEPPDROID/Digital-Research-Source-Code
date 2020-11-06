$SET VERIFY
$!		MAKETAPE.COM
$!
$!
$!  Command file to generate the build-it-from-source kit
$!  for the PL/M-VAX compiler.  (Note that the UDI build-
$!  it-from-source kit is also required.)
$!
$!  02FEB82  Alex Hunter  1. Original version.
$!  06APR82  Alex Hunter  1. Allocate MTA0 instead of MT.
$!
$ALLOCATE MTA0 TAPE
$INIT/DENSITY=1600 TAPE PLMVAX
$MOUNT TAPE PLMVAX
$COPY/LOG *.* TAPE
$DIR/SIZ/DAT TAPE
$DISMOUNT TAPE
$DEALLOCATE TAPE
$SET NOVERIFY
