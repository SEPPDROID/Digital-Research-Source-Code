(*************************************************************************)
(*                                                                       *)
(*        DOS-TO-CPM v1.00 (c) Copyright S.J.Kay  18th April 1995        *)
(*                                                                       *)
(*                       Copys DOS files to CP/M                         *)
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
   TMPFLE = 'DOSTOCPM.$$$';

type
   String13  = string[13];
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
   DTA    : array [0..42] of byte;
   DirSpc : array [0..79] of byte;
   FleSpc : array [0..79] of byte;
   DirMsk : String127;
   DOSPth : String127;
   CPMPth : String127;

   Quiet  : boolean;
   ChkWrt : boolean;
   OvrWrt : boolean;
   FleHnd : integer;
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


function FileFound (FstDir : boolean) : boolean;
begin
   R.AH := $1A;                                        { set DTA function }
   R.DS := seg(DTA);
   R.DX := ofs(DTA);
   Msdos(R);
   if FstDir then
      R.AH := $4E                             { search for first function }
   else
      R.AH := $4F;                             { search for next function }
   R.CX := 0;
   R.DS := seg(DirSpc);
   R.DX := ofs(DirSpc);
   Msdos(R);
   FileFound := not odd(R.Flags);
   if FstDir and odd(R.Flags) then
      writeln('No files found to match: ', DirMsk)
end;


procedure OpenFiles (FleNme : String13);
var
   TmpStr : String127;
   UsrRes : string[1];
   Result : integer;
begin
   if not Quiet then
      writeln('Copying: ', FleNme);
   TmpStr := DOSPth + FleNme;
   move(TmpStr[1], FleSpc, length(TmpStr));
   FleSpc[length(TmpStr)] := 0;
   R.AH := $3D;                                      { open file function }
   R.AL := $00;                                             { read access }
   R.DS := seg(FleSpc);
   R.DX := ofs(FleSpc);
   Msdos(R);                                  { open DOS file for reading }
   Error := R.Flags and 1;
   if Error = 0 then
      begin
         FleHnd := R.AX;                                    { file handle }
         OvrWrt := true;
{$I-}
         if ChkWrt then
            begin
               assign(F, CPMPth + FleNme);
               reset(F);                              { does file exist ? }
               if ioresult = 0 then
                  begin
                     close(F);
                     Result := ioresult;
                     write('Overwrite ', CPMPth, FleNme, ' ? (y/n): ');
                     readln(UsrRes);
                     OvrWrt := (UsrRes = 'y') or (UsrRes = 'Y')
                  end
            end;
         if OvrWrt then
            begin
               assign(F, CPMPth + TMPFLE);
               rewrite(F);         { open temporary CP/M file for writing }
               Error := ioresult;
               if Error <> 0 then
                  writeln('Error CP/M disk directory full')
            end
{$I+}
      end
   else
      writeln('Error opening DOS file for reading')
end;


procedure CloseFiles (FleNme : String13);
var
   Result : integer;
begin
   R.AH := $3E;                                     { close file function }
   R.BX := FleHnd;                                          { file handle }
   Msdos(R);                                             { close DOS file }
   Error := R.Flags and 1;
   if Error <> 0 then
      writeln('Error closing DOS source file');
   if OvrWrt and (Error = 0) then
      begin
{$I-}
         close(F);                            { close temporary CP/M file }
         Error := ioresult;
         if Error = 0 then
            begin
               assign(F, CPMPth + FleNme);
               erase(F);
               Result := ioresult;
               assign(F, CPMPth + TMPFLE);
               rename(F, CPMPth + FleNme);
               Error := ioresult;
               if Error <> 0 then
                  writeln('Error renaming temporary CP/M file')
            end
         else
            writeln('Error closing temporary CP/M file')
      end
{$I+}
end;


procedure CopyFile (FleNme : String13);
var
   EndFle : boolean;
   Blocks : integer;
begin
   repeat
      R.AH := $3F;                         { read file or device function }
      R.BX := FleHnd;                                       { file handle }
      R.CX := ENDBUF+1;                                   { bytes to read }
      R.DS := seg(Buffer);
      R.DX := ofs(Buffer);
      Msdos(R);                                 { read data from DOS file }
      Error := R.Flags and 1;
      if Error = 0 then
         begin
            Blocks := (R.AX + 127) div 128;
            EndFle := R.AX < (ENDBUF + 1);
            if EndFle then
               Buffer[R.AX] := ord(^Z);
{$I-}
            blockwrite(F, Buffer, Blocks);           { write to CP/M file }
            Error := ioresult;
            if Error <> 0 then
               writeln('Error CP/M disk full')
{$I+}
         end
      else
         writeln('Error reading DOS file')
   until EndFle or (Error <> 0)
end;


procedure TransferFiles;
var
   FstDir, Found : boolean;
   FleNme        : String13;
   i             : integer;
begin
   DOSPth := DirMsk;
   i := length(DOSPth);
   while (i > 0) and (not (DOSPth[i] in ['\', ':'])) do
      i := i - 1;
   DOSPth[0] := chr(i);
   move(DirMsk[1], DirSpc, length(DirMsk));
   DirSpc[length(DirMsk)] := 0;
   FstDir := true;
   while FileFound(FstDir) and (Error = 0) do
      begin
         FstDir := false;
         i := 0;
         FleNme := '';
         while (i < 13) and (DTA[$1E+i] <> 0) do
            begin
               FleNme := FleNme + chr(DTA[$1E+i]);
               i := i + 1
            end;
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
   writeln('DOS-TO-CPM v1.00  (c) Copyright  S.J.Kay  18th April 1995');
   writeln;
   writeln('Use:- dostocpm (d:\path\files) (d:\path\files)... c: [/f/q]');
   writeln;
   writeln('d:\path\files = DOS drive, path and files');
   writeln('           c: = CP/M drive path');
   writeln('           /f = force overwriting of existing file(s)');
   writeln('           /q = quiet, no display of file names')
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
   CpmPth := ParamStr(PrmTot);
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
