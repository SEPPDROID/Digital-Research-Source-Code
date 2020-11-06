(*************************************************************************)
(*                                                                       *)
(*          DOSDIR v1.00 (c) Copyright S.J.Kay  24th April 1995          *)
(*                                                                       *)
(*                 Displays a directory of DOS disks                     *)
(*                                                                       *)
(*************************************************************************)

{    WARNING - Make sure the END address is lowered to say about $5000    }
{    before compiling to disk otherwise it will crash when used from      }
{    within a SUBMIT file.  This happens to any TURBO v2.00a compiled     }
{    program because it does not check the TPA size !.                    }

{    As TURBO v2.00a compiled programs overwrite part of the command      }
{    line parameters (only 1st 31 characters are intact) make sure that   }
{    FIXTURBO.COM is run on this compiled program to allow full access    }
{    to command line parameters.                                          }

{$C-}                                       { turn off ^C and ^S checking }

const
   ComLne : string[127] = 'PARAMETERS';         { filled in by patch code }

type
   registers = record
                  case boolean of
                     true  : (AL, AH, BL, BH, CL, CH, DL, DH : byte);
                     false : (AX,     BX,     CX,     DX,
                              BP, SI, DI, DS, ES, FLAGS      : integer)
               end;

   String14  = string[14];
   String127 = string[127];

var
   R      : registers;
   DTA    : array [0..42] of byte;           { Data Transfer Area Buffer }
   DirSpc : array [0..126] of byte;
   DirPrm : String127;
   DirPth : String127;
   DirMsk : String127;
   DirDrv : byte;
   DirChr : char;
   Error  : integer;


procedure ProcZ80 (Fn, Ax : byte; BCx, DEx, HLx : integer);
begin
   inline
      (
       $3A/Fn/          { LD      A,(Fn)     }
       $32/* + 17/      { LD      (FNCNMB),A }
       $3A/Ax/          { LD      A,(Ax)     }
       $ED/$4B/BCx/     { LD      BC,(BCx)   }
       $ED/$5B/DEx/     { LD      DE,(DEx)   }
       $2A/HLx/         { LD      HL,(HLx)   }
       $D3/$FF          { OUT     (FNCNMB),A }
      )
end;


procedure Intr (Int : byte; var R : registers);
begin
   ProcZ80($A1, Int, $AA55, $55AA, addr(R))
end;


procedure Msdos (var R : registers);
begin
   Intr($21, R)
end;


function Seg (var Dummy) : integer;
const
   SegAdr : integer = 0;
begin
   ProcZ80($A0, 0, 0, 0, addr(SegAdr));
   Seg := SegAdr
end;


function Ofs (var VarTyp) : integer;
begin
   Ofs := addr(VarTyp)
end;


procedure TestExtendedError;
begin
   if odd(R.FLAGS) then
      begin
         R.AH := $59;
         R.BX := $00;
         Msdos(R);
         if (R.AL <> 0) and (R.AL <> 18) then
            begin
               Error := R.AL;
               writeln;
               case Error of
                   3 : writeln('Path not found');
                  15 : writeln('Invalid drive specification');
                  83 : writeln('Failed on DOS int 24H (critical error)')
                    else
                       writeln('Error occurred, code: ', Error, ' (dec)')
               end
            end
      end
end;


function EntryFound (FstDir : boolean; AtrTyp : byte) : boolean;
begin
   if FstDir then
      R.AH := $4E                             { search for first function }
   else
      R.AH := $4F;                             { search for next function }
   R.CX := AtrTyp;                                       { attribute type }
   R.DS := seg(DirSpc);
   R.DX := ofs(DirSpc);
   Msdos(R);
   EntryFound := not odd(R.Flags);
   TestExtendedError
end;


procedure ExtractName (var FleNme : String14);
var
   SubTyp, VolTyp : boolean;
   i              : integer;
begin
   SubTyp := (DTA[$15] and $10) <> 0;
   VolTyp := (DTA[$15] and $08) <> 0;
   i := 0;
   if not SubTyp then
      FleNme := ''
   else
      FleNme := '[';
   while (i < 12) and (DTA[$1E+i] <> 0) do
      begin
         if (chr(DTA[$1E+i]) = '.') and VolTyp then
            i := i + 1
         else
            begin
               FleNme := FleNme + chr(DTA[$1E+i]);
               i := i + 1
            end
      end;
   if SubTyp then
      FleNme := FleNme + ']'
end;


procedure VolumeLabel;
var
   FleNme : String14;
   TmpStr : String127;
begin
   writeln;
   TmpStr := DirChr + ':\*.*';               { look in the root directory }
   move(TmpStr[1], DirSpc, length(TmpStr));
   DirSpc[length(TmpStr)] := 0;
   if (EntryFound(true, $08)) and (Error = 0) then
      begin
         ExtractName(FleNme);
         writeln(' Volume in drive ', DirChr, ' is ', FleNme);
         writeln
      end
   else
      if Error = 0 then
         begin
            writeln(' Volume in drive ', DirChr, ' has no label ');
            writeln
         end
end;


procedure CheckFileOrSubdir;
begin
   if (pos('*', DirMsk) = 0) and (pos('?', DirMsk) = 0) then
      begin
         move(DirPrm[1], DirSpc, length(DirPrm));
         DirSpc[length(DirPrm)] := 0;
         if EntryFound(true, $10) and (Error = 0) then
            begin
               if (DTA[$15] and $10) <> 0 then
                  DirPrm := DirPrm + '\*.*'
            end
      end
end;


procedure DiskDetails (FleCnt : integer);
var
   Free   : real;
   FreStr : string[20];
   i, x   : integer;
begin
   R.AH := $36;                                     { get disk free space }
   R.DL := DirDrv;                           { drive number A=1, B=2, etc }
   Msdos(R);                                              { get disk info }
   Free := (R.AX * 1.0) * (R.CX * 1.0) * (R.BX * 1.0);
   str(Free:11:0, FreStr);
   i := length(FreStr);
   x := 0;
   while (i > 0) and (FreStr[i] <> ' ') do
      begin
         x := x + 1;
         if ((x mod 3) = 0) and (FreStr[i-1] <> ' ') then
            insert(',', FreStr, i);
         i := i - 1
      end;
   writeln;
   writeln(FleCnt:9, ' file(s)', '':15-length(FreStr),
   FreStr, ' bytes free')
end;


procedure DosDir;
var
   FstDir : boolean;
   FleCnt : integer;
   FleNme : String14;
begin
   VolumeLabel;
   if Error = 0 then
      CheckFileOrSubdir;
   if Error = 0 then
      begin
         move(DirPrm[1], DirSpc, length(DirPrm));
         DirSpc[length(DirPrm)] := 0;
         writeln(' Directory of ', DirPrm);
         writeln;
         FleCnt := 0;
         FstDir := true;
         while EntryFound(FstDir, $10) and (Error = 0) do
            begin
               FstDir := false;
               FleCnt := FleCnt + 1;
               ExtractName(FleNme);
               write(FleNme, '':16-length(FleNme))
            end
      end;
   if Error = 0 then
      begin
         if (FleCnt mod 5) <> 0 then
            writeln;
         if FstDir then
            writeln(' No files found');
         DiskDetails(FleCnt)
      end
end;


var
   i : integer;
begin
   Error := 0;
   DirPrm := ComLne;
   while pos(' ', DirPrm) = 1 DO
      delete(DirPrm, 1, 1);
   DirMsk := DirPrm;
   DirPth := DirPrm;
   i := length(DirPth);
   while (i > 0) and (not (DirPth[i] in [':', '\'])) do
      i := i - 1;
   DirPth[0] := chr(i);
   delete(DirMsk, 1, i);
   if pos(':', DirPth) = 2 then
      DirDrv := ord(upcase(DirPth[1])) - 64
   else
      begin
         R.AH := $19;                          { get current drive number }
         Msdos(R);                                    { get default drive }
         DirDrv := R.AL + 1;
         DirPth := chr(DirDrv + 64) + ':' + DirPth
      end;
   if pos('\', DirPth) = 0 then
      begin
         R.AH := $47;
         R.DL := DirDrv;
         R.DS := seg(DirPth[3]);
         R.SI := ofs(DirPth[3]);
         Msdos(R);
         TestExtendedError;
         if Error = 0 then
            begin
               i := 1;
               while DirPth[i] <> #0 do
                  i := i + 1;
               DirPth[0] := chr(i-1);
               if pos('\', DirPth) <> 3 then
                  insert('\', DirPth, 3)
            end
      end;
   if Error = 0 then
      begin
         DirChr := chr(DirDrv + 64);
         if DirPth[length(DirPth)] <> '\' then
            DirPth := DirPth + '\';
         if DirMsk = '' then
            DirMsk := '*.*';
         DirPrm := DirPth + DirMsk;
         R.AX := $1A00;                    { function used to set the DTA }
         R.DS := seg(DTA);            { store the parameter segment in DS }
         R.DX := ofs(DTA);              { "    "      "      offset in DX }
         Msdos(R);                                     { set DTA location }
         DosDir
      end
end.
