(*************************************************************************)
(*                                                                       *)
(*                       Z80 Hard Disk v1.00                             *)
(*             (C) Copyright S.J.Kay  6th December 1993                  *)
(*                                                                       *)
(*         Creates a disk file to be used by the Z80 Emulator            *)
(*                     program as a hard disk.                           *)
(*                                                                       *)
(*************************************************************************)

uses
   crt, dos;

const
   HDDMEG = 4;                               { size of HDD file in Mbytes }
   SECSZE = 512;
   SECTRK = 64;
   HDDSEC = (HDDMEG * 1024) * (1024 div SECSZE);
   FLENME = 'Z80HDD.DSK';
   MkeDrv : boolean = true;

var
   F      : file;
   i      : integer;
   Sector : array [0..SECSZE-1] of byte;
   DrvPth : string[80];
   ChkUsr : string[1];

begin
   clrscr;
   writeln;
   writeln('===================================================');
   writeln('Z80 Hard Disk v1.00  (C) Copyright S.J.Kay  6/12/93');
   writeln;
   writeln('Creates a disk file to be used by the Z80 Emulator');
   writeln('            program as a hard disk');
   writeln('===================================================');
   writeln;
   writeln('Hard disk size: ', HDDMEG, ' Megabytes');
   writeln;
   write('Enter drive path: ');
   readln(DrvPth);
   writeln;
   if (DrvPth <> '') and (not(DrvPth[length(DrvPth)] in [':', '\'])) then
      DrvPth := DrvPth + '\';
   assign(F, DrvPth + FLENME);
   Setfattr(F, 0);                                 { make file read/write }
{$I-}
   reset(F);
   if ioresult = 0 then
      begin
         close(F);
{$I+}
         writeln('WARNING - ', DrvPth + FLENME, ' will be lost forever');
         write(  'Do you wish to continue (y/n) ?: ');
         readln(ChkUsr);
         MkeDrv := (ChkUsr= 'Y') or (ChkUsr = 'y');
         writeln
      end;
   if MkeDrv then
      begin
         writeln('Creating ', DrvPth + FLENME);
         writeln;
         rewrite(F, SECSZE);
         fillchar(Sector, SECSZE, $E5);
         for i := 1 to HDDSEC do
            begin
               blockwrite(F, Sector, 1);
               if i mod SECTRK = 0 then
                  write(^M, i:5, ' of ', HDDSEC)
            end;
         close(F);
         writeln;
         writeln;
         writeln('Z80 hard disk file created')
      end;
   Setfattr(F, ReadOnly);
end.
