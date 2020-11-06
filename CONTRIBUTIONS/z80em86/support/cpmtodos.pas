(*************************************************************************)
(*                                                                       *)
(*        CPM-TO-DOS v1.00 (c) Copyright S.J.Kay  18th April 1995        *)
(*                                                                       *)
(*                       Copys CP/M files to DOS                         *)
(*                                                                       *)
(*************************************************************************)

{    WARNING - Make sure the END address is lowered to say about $8000    }
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

   ENDBUF = $3FFF;                                     { 16 K copy buffer }
   ENDDIR = $1F;                           { 32 filename directory buffer }
   TMPFLE = 'CPMTODOS.$$$';

type
   String12  = string[12];
   String127 = string[127];

   registers = record
                  case boolean of
                     true  : (AL, AH, BL, BH, CL, CH, DL, DH : byte);
                     false : (AX,     BX,     CX,     DX,
                              BP, SI, DI, DS, ES, FLAGS      : integer)
               end;

var
   R      : registers;
   F      : file;
   Buffer : array [0..ENDBUF] of byte;
   DirBuf : array [0..ENDDIR] of String12;
   DMA    : array [0..3, 0..31] of char;
   FCB    : array [0..35] of byte;

   CPMdrv : byte;
   DirMsk : String127;
   DOSPth : String127;
   CPMPth : String127;

   Quiet  : boolean;
   ChkWrt : boolean;
   OvrWrt : boolean;
   Error  : integer;


procedure ProcZ80 (Fn, Ax : byte; BCx, DEx, HLx : integer);
begin
   inline
      (
       $3A/Fn/          { ld      a,(Fn)     }
       $32/* + 17/      { ld      (FNCNMB),a }
       $3A/Ax/          { ld      a,(Ax)     }
       $ED/$4B/BCx/     { ld      bc,(BCx)   }
       $ED/$5B/DEx/     { ld      de,(DEx)   }
       $2A/HLx/         { ld      hl,(HLx)   }
       $D3/$FF          { out     (FNCNMB),a }
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


type
   DOSFle = record
                 Path : array [0..127] of byte;
               Handle : integer
            end;

var
   ioresultDOS : integer;
   D           : DOSFle;


procedure AssignDOS (var F : DOSFle; Name : String127);
begin
   move(Name[1], F.Path, length(Name));
   F.Path[length(Name)] := 0
end;


procedure ResetDOS (var F : DOSFle);
begin
   R.AH := $3D;                               { open file handle function }
   R.AL := $02;                                       { read/write access }
   R.DS := seg(F.Path);
   R.DX := ofs(F.Path);
   Msdos(R);                                              { open DOS file }
   if odd(R.FLAGS) then
      ioresultDOS := R.AX
   else
      begin
         ioresultDOS := 0;
         F.Handle := R.AX                                   { file handle }
      end
end;


procedure RewriteDOS (var F : DOSFle);
begin
   R.AH := $3C;                                    { create file function }
   R.CX := $00;                                 { file attribute (normal) }
   R.DS := seg(F.Path);
   R.DX := ofs(F.Path);
   Msdos(R);                                            { create DOS file }
   if odd(R.FLAGS) then
      ioresultDOS := R.AX
   else
      begin
         ioresultDOS := 0;
         F.Handle := R.AX                                   { file handle }
      end
end;


procedure CloseDOS (var F : DOSFle);
begin
   R.AH := $3E;                              { close file handle function }
   R.BX := F.Handle;                                        { file handle }
   Msdos(R);                                             { close DOS file }
   if odd(R.FLAGS) then
      ioresultDOS := R.AX
   else
      ioresultDOS := 0
end;


procedure EraseDOS (var F : DOSFle);
begin
   R.AH := $41;                                    { delete file function }
   R.DS := seg(F.Path);
   R.DX := ofs(F.Path);
   Msdos(R);                                            { delete DOS file }
   if odd(R.FLAGS) then
      ioresultDOS := R.AX
   else
      ioresultDOS := 0
end;


procedure RenameDOS (var F : DOSFle; Name : String127);
var
   X : DOSFle;
begin
   move(Name[1], X.Path, length(Name));
   X.Path[length(Name)] := 0;
   R.AH := $56;                                    { rename file function }
   R.DS := seg(F.Path);
   R.DX := ofs(F.Path);
   R.ES := seg(X.Path);
   R.DI := ofs(X.Path);
   Msdos(R);                                            { rename DOS file }
   if odd(R.FLAGS) then
      ioresultDOS := R.AX
   else
      ioresultDOS := 0
end;


procedure BlockWriteDOS (var F : DOSFle; var Source; RecCnt : integer);
begin
   R.AH := $40;                        { write to file or device function }
   R.BX := F.Handle;                                        { file handle }
   R.CX := 128 * RecCnt;
   R.DS := seg(Source);
   R.DX := ofs(Source);
   Msdos(R);                                              { open DOS file }
   if odd(R.FLAGS) then
      ioresultDOS := $FF
   else
      ioresultDOS := 0
end;


procedure SetupFCB;
var
   PosGet, PosPut : integer;
begin
   PosGet := 1;
   PosPut := 1;
   fillchar(FCB, sizeof(FCB), 0);
   fillchar(FCB[1], 11, ' ');
   FCB[0] := CPMdrv;
   repeat
      while (PosGet <= length(DirMsk)) and
      (DirMsk[PosGet] <> '.') and (PosPut < 12) do
         begin
            if DirMsk[PosGet] = '*' then
               repeat
                  FCB[PosPut] := ord('?');
                  PosPut := PosPut + 1
               until PosPut in [9, 12]
            else
               begin
                  FCB[PosPut] := ord(upcase(DirMsk[PosGet]));
                  PosPut := PosPut + 1
               end;
            PosGet := PosGet + 1
         end;
      if DirMsk[PosGet] = '.' then
         begin
            PosGet := PosGet + 1;
            PosPut := 9
         end
   until (PosGet > length(DirMsk)) or (PosPut = 12)
end;


procedure LoadDirectoryBuffer (var DirTot : integer);
var
   DirPos, x, i : integer;
   SchDir       : byte;
   FleNme       : String12;
begin
   bdos(26, addr(DMA));
   i := DirTot;
   SchDir := 17;                              { search for first function }
   while i <> 0 do                   { move to correct directory position }
      begin
         bdos(SchDir, addr(FCB));                      { search directory }
         i := i - 1;
         SchDir := 18
      end;
   DirPos := 0;
   repeat
      x := bdos(SchDir, addr(FCB));
      if x <> $FF then
         begin
            FleNme := '';
            for i := 1 to 11 do
               begin
                  if DMA[x, i] <> ' ' then
                     FleNme := FleNme + DMA[x, i];
                  if i = 8 then
                     FleNme := FleNme + '.'
               end;
            DirBuf[DirPos] := FleNme;
            DirPos := DirPos + 1;
            DirTot := DirTot + 1
         end;
      SchDir := 18                             { search for next function }
   until (x = $FF) or (DirPos > ENDDIR)
end;


function FileFound (FstDir : boolean; var FleNme : String12) : boolean;
const
   DirTot : integer = 0;
   DirNmb : integer = 0;
   GetPos : integer = 0;
begin
   if FstDir then
      begin
         DirNmb := 0;
         DirTot := 0;
         SetupFCB;
      end;
   if DirNmb = DirTot then
      begin
         LoadDirectoryBuffer(DirTot);
         GetPos := 0
      end;
   FileFound := DirNmb < DirTot;
   if DirNmb < DirTot then
      begin
         FleNme := DirBuf[GetPos];
         GetPos := GetPos + 1;
         DirNmb := DirNmb + 1
      end;
   if DirTot = 0 then
      writeln('No files found to match: ', CPMPth, DirMsk)
end;


procedure OpenFiles (FleNme : String12);
var
   TmpStr : String127;
   UsrRes : string[1];
   Result : integer;
begin
   if not Quiet then
      writeln('Copying: ', FleNme);
{$I-}
   assign(F, CPMPth + FleNme);
   reset(F);
   Error := ioresult;
{$I+}
   if Error = 0 then
      begin
         OvrWrt := true;
         if ChkWrt then
            begin
               AssignDOS(D, DOSPth + FleNme);
               ResetDOS(D);                           { does file exist ? }
               if ioresultDOS = 0 then
                  begin
                     CloseDOS(D);
                     Result := ioresultDOS;
                     write('Overwrite ', DOSPth, FleNme, ' ? (y/n): ');
                     readln(UsrRes);
                     OvrWrt := (UsrRes = 'y') or (UsrRes = 'Y')
                  end
            end;
         if OvrWrt then
            begin
               AssignDOS(D, DOSPth + TMPFLE);
               RewriteDOS(D);       { open temporary DOS file for writing }
               Error := ioresultDOS;
               if Error <> 0 then
                  writeln('Error can not create DOS file')
            end
      end
   else
      writeln('Error opening CP/M file for reading')
end;


procedure CloseFiles (FleNme : String12);
var
   Result : integer;
begin
{$I-}
   close(F);                                            { close CP/M file }
   Error := ioresult;
{$I+}
   if Error <> 0 then
      writeln('Error closing CP/M source file');
   if OvrWrt and (Error = 0) then
      begin
         CloseDOS(D);                          { close temporary DOS file }
         Error := ioresultDOS;
         if Error = 0 then
            begin
               AssignDOS(D, DOSPth + FleNme);
               EraseDOS(D);
               Result := ioresultDOS;
               AssignDOS(D, DOSPth + TMPFLE);
               RenameDOS(D, DOSPth + FleNme);
               Error := ioresultDOS;
               if Error <> 0 then
                  writeln('Error renaming temporary DOS file')
            end
         else
            writeln('Error closing temporary DOS file')
      end
end;


procedure CopyFile (FleNme : String12);
var
   BufPos : integer;
begin
   repeat
      BufPos := 0;
{$I-}
      while (not eof(F)) and (BufPos < ENDBUF) and (Error = 0) do
         begin
            blockread(F, Buffer[BufPos], 1);   { read data from CP/M file }
            Error := ioresult;
            BufPos := BufPos + 128
         end;
{$I+}
      if Error = 0 then
         begin
            BlockwriteDOS(D, Buffer, BufPos div 128); { write to DOS file }
            Error := ioresultDOS;
            if Error <> 0 then
               writeln('Error DOS disk full')
         end
      else
         writeln('Error reading CP/M file')
   until eof(F) or (Error <> 0)
end;


procedure TransferFiles;
var
   FstDir, Found : boolean;
   FleNme        : String12;
begin
   if pos(':', DirMsk) = 2 then
      begin
         CPMPth := DirMsk;
         CPMPth[0] := #2;
         CPMdrv := (ord(DirMsk[1]) - ord('A')) + 1;
         delete(DirMsk, 1, 2)
      end
   else
      begin
         CPMPth := '';
         CPMdrv := 0                                 { default CP/M drive }
      end;
   if not (DOSPth[length(DOSPth)] in [':', '\']) then
      DOSPth := DOSPth + '\';
   FstDir := true;
   while FileFound(FstDir, FleNme) and (Error = 0) do
      begin
         FstDir := false;
         OpenFiles(FleNme);
         if OvrWrt and (Error = 0) then
            CopyFile(FleNme);
         if Error = 0 then
            CloseFiles(FleNme)
      end
end;


function ParmCount : integer;
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
   ParmCount := PrmCnt
end;


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


function ScanForSwitches (PrmNmb : integer) : integer;
var
   SwtStr    : String127;
   SwtPos, i : integer;
begin
   repeat
      SwtStr := ParamStr(PrmNmb);
      SwtPos := pos('/', SwtStr);
      if Swtpos <> 0 then
         begin
            PrmNmb := PrmNmb - 1;
            delete(SwtStr, 1, 1);
            for i := 1 to length(SwtStr) do
               SwtStr[i] := upcase(SwtStr[i]);
            if SwtStr = 'Q' then
               Quiet := true;
            if SwtStr = 'F' then
               ChkWrt := false
         end
   until SwtPos = 0;
   ScanForSwitches := PrmNmb
end;


procedure ShowUsage;
begin
   writeln;
   writeln('CPM-TO-DOS v1.00  (c) Copyright  S.J.Kay  18th April 1995');
   writeln;
   writeln('Use:- cpmtodos (c:files) (c:files)... d:\path [/f/q]');
   writeln;
   writeln('c:files = CP/M drive and files');
   writeln('d:\path = DOS destination drive and path');
   writeln('     /f = force overwriting of existing file(s)');
   writeln('     /q = quiet, no display of file names')
end;


var
   PrmTot, PrmNmb : integer;
   SwtStr         : String127;
begin
   Quiet := false;
   ChkWrt := true;
   Error := 0;
   PrmNmb := 1;
   PrmTot := ScanForSwitches(ParmCount);
   DOSPth := ParamStr(PrmTot);
   if PrmTot > 1 then
      begin
         repeat
            DirMsk := ParamStr(PrmNmb);
            PrmNmb := PrmNmb + 1;
            TransferFiles
         until (PrmNmb >= PrmTot) or (Error <> 0)
      end
   else
      ShowUsage
end.
