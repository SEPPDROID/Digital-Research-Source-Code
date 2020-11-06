(*************************************************************************)
(*                                 FMFD                                  *)
(*                                                                       *)
(*                  High level disk formatting utility                   *)
(*                     for SS/DS 40/80T floppy disks                     *)
(*                                                                       *)
(*                      (c) 2009 Copyright S.J.Kay                       *)
(*************************************************************************)

{    WARNING - Make sure the END address is lowered to say about $8000    }
{    before compiling to disk otherwise it will crash when used from      }
{    within a SUBMIT file.  This happens to any TURBO v2.00a compiled     }
{    program because it does not check the TPA size !.                    }

{$C-}                                       { turn off ^C and ^S checking }

const
   APPNAME = 'FMFD';
   APPVERS = '1.0.0';
   APPDATE = '26 February 2009';

   FORMAT_BYTE = $e5;

type
   String80 = string[80];
   String127 = string[127];

var
   Version    : integer;
   DrvChr     : char;
   DrvCde     : integer;
   BnkFlg     : byte;
   Parameters : String80;
   ComLne     : String80 absolute $0080;
   Buffer     : array [0..1023] of byte;
   SecTrk     : integer;
   DiskSides  : integer;
   DiskTracks : integer;

   BIOSPB  : record
                FN : byte;
                 A : byte;
                BC : integer;
                DE : integer;
                HL : integer
             end;


procedure BiosCall (Fn, A : byte; BC, DE, HL : integer);
begin
   BIOSPB.FN := Fn;
   BIOSPB.A  := A;
   BIOSPB.BC := BC;
   BIOSPB.DE := DE;
   BIOSPB.HL := HL;
   bdos(50, addr(BIOSPB))
end;


function BiosFunc (Fn, A : byte; BC, DE, HL : integer) : byte;
begin
   BIOSPB.FN := Fn;
   BIOSPB.A  := A;
   BIOSPB.BC := BC;
   BIOSPB.DE := DE;
   BIOSPB.HL := HL;
   BiosFunc := bdos(50, addr(BIOSPB))
end;


procedure BiosX (Fn, Ax : byte; BCx, DEx, HLx : integer);
begin
   inline
      (
       $3A/Fn/          { ld      a,(Fn)     }
       $4F/             { ld      c,a        }
       $87/             { add     a,a        }
       $81/             { add     a,c        }
       $06/$00/         { ld      b,0        }
       $4F/             { ld      c,a        }
       $2A/$01/$00/     { ld      hl,(0001h) }
       $09/             { add     hl,bc      }
       $22/* + 17/      { ld      (zzzz),hl  }
       $3A/Ax/          { ld      a,(Ax)     }
       $ED/$4B/BCx/     { ld      bc,(BCx)   }
       $ED/$5B/DEx/     { ld      de,(DEx)   }
       $2A/HLx/         { ld      hl,(HLx)   }
       $CD/$00/$00      { call    zzzz       }
      )
end;


procedure GetBIOSdata (AdrSor, AdrDst, Amount : integer);
begin
   BiosX(28, 0, $0100, 0, 0);       { set xmove banks, bank #0 to bank #1 }
   BiosX(24, 0, Amount, AdrSor, AdrDst)                     { move memory }
end;


function GetBIOSword (AdrSor : integer) : integer;
const
   WrdDst : integer = 0;
begin
   GetBIOSdata(AdrSor, addr(WrdDst), 2);
   GetBIOSword := WrdDst
end;


function GetBIOSbyte (AdrSor : integer) : byte;
const
   BytDst : byte = 0;
begin
   GetBIOSdata(AdrSor, addr(BytDst), 1);
   GetBIOSbyte := BytDst
end;

{
*==========================================================================
* Return the number of command line parameters.
*
*   pass: void
* return: integer                       number of parameters
*==========================================================================
}
function ParamCount : integer;
var
   i, PrmCnt, PrmLen : integer;
begin
   i := 1;
   PrmCnt := 0;
   PrmLen := length(ComLne);
   while i <= PrmLen do
      begin
         while (i <= PrmLen) and (ComLne[i] = ' ') do
            i := i + 1;
         if i <= PrmLen then
            PrmCnt := PrmCnt + 1;
         if ComLne[i] = '/' then
            i := i + 1;
         while (i <= PrmLen) and (not(ComLne[i] in [' ', '/'])) do
            i := i + 1
      end;
   ParamCount := PrmCnt
end;

{
*==========================================================================
* Return the specified command line parameter.
*
*   pass: PrmNmb : integer
* return: String127                     parameter
*==========================================================================
}
function ParamStr (PrmNmb : integer) : String127;
var
   i, PrmCnt, PrmLen : integer;
   PrmStr            : string[127];
begin
   i := 1;
   PrmCnt := 0;
   PrmStr := '';
   PrmLen := length(ComLne);
   while (i <= PrmLen) and (PrmCnt < PrmNmb) do
      begin
         while (i <= PrmLen) AND (ComLne[i] = ' ') do
            i := i + 1;
         if i <= PrmLen then
            PrmCnt := PrmCnt + 1;
         if ComLne[i] = '/' then
            begin
               PrmStr := '/';
               i := i + 1
            end
         else
            PrmStr := '';
         while (i <= PrmLen) and (not (ComLne[i] in [' ', '/'])) do
            begin
               PrmStr := PrmStr + ComLne[i];
               i := i + 1
            end
      end;
   ParamStr := PrmStr
end;

{
*==========================================================================
* Write disk sector
*
*   pass: track : integer
*         sector : integer
* return: byte                          0 if no error
*==========================================================================
}
function WriteDiskSector ( track : integer;
                          sector : integer) : byte;
var
   res_a : byte;
begin
   BiosCall(10, 0, track, 0, 0);                     { set physical track }
   BiosCall(11, 0, sector, 0, 0);                   { set physical sector }
   BiosCall(12, 0, addr(buffer), 0, 0);                 { set DMA address }
   BiosCall(28, 1, 0, 0, 0);                     { set data bank #1 (TPA) }
   res_a := BiosFunc(14, 0, 0, 0, 0);                    { write 1 sector }
   WriteDiskSector := res_a;
end;

{
*==========================================================================
* Write disk track.
*
* Skewing is used to speed things up.
*
*   pass: track : integer
* return: byte                          0 if no error
*==========================================================================
}
function WriteDiskTrack (track : integer) : byte;
var
   s : integer;
   res : byte;
begin
   s := 1;
   repeat
      res := WriteDiskSector(track, s);
      s := s + 1;
   until ((res <> 0) or (s > SecTrk));
   WriteDiskTrack := res;
end;

{
*==========================================================================
* Format all tracks.
*
*   pass: drive : char              drive char (A-B)
* return: void
*==========================================================================
}
procedure FormatAllTracks;
var
   t : integer;
   res : integer;
   completed : real;
begin
   BiosCall(9, 0, DrvCde, 1, 0);

   fillchar(buffer, sizeof(buffer), FORMAT_BYTE);
   t := 0;
   writeln;
   write('  0 percent completed', ^M);
   repeat
      res := WriteDiskTrack(t);
      t := t + 1;
      completed := (t / (DiskTracks * DiskSides)) * 100.0;
      write(completed:3:0, ^M);
   until ((res <> 0) or (t = (DiskTracks * DiskSides)));
   bdos(37, 1 shl DrvCde);    { reset the formatted drive }
   writeln;
   writeln;
   if (res = 0) then
      writeln('Format completed')
   else
      writeln('Format failed');
end;

{
*==========================================================================
* Format disk
*
*   pass: drive : char              drive char (A-B)
* return: void
*==========================================================================
}
procedure FormatDisk (drive : char);
var
   TblAdr             : integer;
   DPH, DPB, SPT, PHM : integer;
   res_hl             : integer;
   def_drive          : byte;
   response           : string[1];
begin
   DrvCde := ord(drive) - ord('A');
   TblAdr := bioshl(21) + DrvCde * 2;
   DPH := GetBIOSword(TblAdr);
   if (DPH <> 0) then
      begin
         DPB := GetBIOSword(DPH+12);
         SPT := GetBIOSword(DPB);                         { sectors/track }
         PHM := GetBIOSbyte(DPB+16);               { physical sector mask }
         SecTrk := SPT DIV (PHM + 1);

         writeln('WARNING - All data on ', drive,
         ': will be lost forever !');

         write('Are you sure [y/n] ?: ');
         buflen := 1;
         readln(response);
         if ((response = 'y') or (response = 'Y')) then
            FormatAllTracks
         else
            writeln('Format aborted');
      end
   else
      writeln('The drive specified does not exist')
end;

{
*==========================================================================
* Usage Information
*
*   pass: void
* return: void
*==========================================================================
}
procedure UsageInformation;
begin
   writeln(APPNAME, ' v', APPVERS, '  (c) Copyright S.J.Kay  ', APPDATE);
   writeln;
   writeln('!!! WARNING !!!');
   writeln('This program performs a high level disk format by writing E5 hex');
   writeln('to all locations on the disk,  this is just as destructive as a');
   writeln('low level format,  you will lose all data on the disk!');
   writeln;
   writeln('Usage:- ', APPNAME, ' SS/DS 40/80 drive:');
   writeln;
   writeln(' SS/DS = SS is single sided, DS is double sided disk');
   writeln(' 40/80 = 40 or 80 tracks per disk side');
   writeln('drive: = CP/M drive A: or B:');
end;

var
   parm  : String127;
   Error : integer;
begin
   Version := bdoshl(12);
   if (hi(Version) = $00) and (lo(Version) >= $30) then
      begin
         Error := ord(ParamCount <> 3);
         if (Error = 0) then
            begin
               parm := ParamStr(1);
               if (parm = 'SS') then
                  DiskSides := 1
               else
                  if (parm = 'DS') then
                     DiskSides := 2
                  else
                     Error := 1;

               if (Error = 0) then
                  begin
                     parm := ParamStr(2);
                     if (parm = '40') then
                        DiskTracks := 40
                     else
                        if (parm = '80') then
                           DiskTracks := 80
                        else
                           Error := Error + 1;
                  end;

               if (Error = 0) then
                  begin
                     parm := ParamStr(3);
                     if (length(parm) = 2) then
                        parm[1] := upcase(parm[1]);
                     if ((parm[2] = ':') and (parm[1] in ['A'..'B'])) then
                        FormatDisk(parm[1])
                     else
                        Error := Error + 1;
                  end;
            end;
         if (Error <> 0) then
            UsageInformation
      end
   else
      begin
         writeln;
         writeln('Wrong SYSTEM,  requires CP/M Plus ver 3.0 up')
      end
end.
