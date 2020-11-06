(*************************************************************************)
(*                                                                       *)
(*             CPMJOB v1.00 (c) Copyright 1992-2009, S.J.Kay             *)
(*                                                                       *)
(*              Support utility for IBM Z80 Emulator CP/M 3              *)
(*                                                                       *)
(*************************************************************************)

{    WARNING - Make sure the END address is lowered to say about $8000    }
{    before compiling to disk otherwise it will crash when used from      }
{    within a SUBMIT file.  This happens to any TURBO v2.00a compiled     }
{    program because it does not check the TPA size !.                    }

{
 ChangeLog
 ---------
 20 February 2009, SJK
 - In EmulatorParameter procedure remove any options passed to z80em86.

 27 April 1995, SJK
 - Undocumented changes.
}

{$C-}                                       { turn off ^C and ^S checking }

type
   registers = record
                  case boolean of
                     true  : (AL, AH, BL, BH, CL, CH, DL, DH : byte);
                     false : (AX,     BX,     CX,     DX,
                              BP, SI, DI, DS, ES, FLAGS      : integer)
               end;

   String80 = string[80];

const
   ENDBUF = $1FFF;
   FLENME = 'CPMJOB.SUB';

var
   R      : registers;
   ComLne : string[80] absolute $0080;
   CpmPth : string[80];
   DosPth : string[128];
   DosSpc : array [0..79] of byte;
   Buffer : array [0..ENDBUF] of char;


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


function FuncZ80 (Fn, Ax : byte; BCx, DEx, HLx : integer) : byte;
const
   BytVal : byte = 0;
begin
   inline
      (
       $3A/Fn/          { ld      a,(Fn)     }
       $32/* + 17/      { ld      (FNCNMB),a }
       $3A/Ax/          { ld      a,(Ax)     }
       $ED/$4B/BCx/     { ld      bc,(BCx)   }
       $ED/$5B/DEx/     { ld      de,(DEx)   }
       $2A/HLx/         { ld      hl,(HLx)   }
       $D3/$FF/         { out     (FNCNMB),a }
       $32/BytVal       { ld      (BYTVAL),a }
      );
   FuncZ80 := BytVal
end;


function GetByt (Seg, Off : integer) : byte;
begin
   GetByt := FuncZ80($B0, 0, 0, Seg, Off)
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


procedure ReportError (ErrStr : String80);
begin
   ProcZ80($FB, 0, 0, 0, 0);                              { turn video on }
   writeln('CPMJOB.COM Error, ', ErrStr)
end;


procedure WriteSubmitFile (FleSze : integer);
var
   F        : text;
   i, Error : integer;
begin
   assign(F, CpmPth);
{$I-}
   rewrite(F);                               { open CP/M file for writing }
   Error := ioresult;
   if Error = 0 then
      begin
         i := 0;
         if FleSze <> 0 then
            begin
               writeln(F, 'video on');
               Error := ioresult
            end;
         while (i < FleSze) and (Error = 0) do
            begin
               write(F, Buffer[i]);
               Error := ioresult;
               i := i + 1
            end;
         if FleSze <> 0 then
            begin
               writeln(F, 'exit');
               Error := ioresult
            end;
         if Error = 0 then
            begin
               close(F);                                { close CP/M file }
               Error := ioresult;
               if Error <> 0 then
                  ReportError('Could not close destination file')
            end
         else
            ReportError('Destination disk full error')
      end
   else
      ReportError('Could not create destination file')
{$I-}
end;


procedure MakeSubmitFile;
begin
   if DosPth <> '' then
      begin
         DosPth := DosPth + ^M^J;
         move(DosPth[1], Buffer, length(DosPth))
      end;
   WriteSubmitFile(length(DosPth))
end;


procedure CopySubmitFile;
var
   FleHnd, FleSze, Error : integer;
begin
   move(DosPth[1], DosSpc[0], length(DosPth));
   DosSpc[length(DosPth)] := 0;
   R.AH := $3D;                                      { open file function }
   R.AL := $00;                                             { read access }
   R.DS := seg(DosSpc);
   R.DX := ofs(DosSpc);
   Msdos(R);                                  { open DOS file for reading }
   FleHnd := R.AX;                                          { file handle }
   Error := R.Flags and 1;
   if Error = 0 then
      begin
         R.AH := $3F;                      { read file or device function }
         R.BX := FleHnd;                                    { file handle }
         R.CX := ENDBUF+1;                                { bytes to read }
         R.DS := seg(Buffer);
         R.DX := ofs(Buffer);
         Msdos(R);                              { read data from DOS file }
         FleSze := R.AX;                 { number of bytes read from file }
         if R.AX > ENDBUF then
            Error := 2
         else
            Error := (R.Flags and 1) * 3
      end;
   if Error = 0 then
      begin
         R.AH := $3E;                               { close file function }
         R.BX := FleHnd;                                    { file handle }
         Msdos(R);                                       { close DOS file }
         Error := (R.Flags and 1) * 4
      end;
   if Error <> 0 then
      begin
         case Error of
            1 : ReportError('Could not open source submit file');
            2 : ReportError('Source submit file is too big');
            3 : ReportError('Reading source submit file');
            4 : ReportError('Could not close source submit file')
         end;
         DosPth := 'exit';
         MakeSubmitFile
      end
   else
      WriteSubmitFile(FleSze)
end;


procedure EmulatorParameter;
const
   SegOff : array [0..1] of integer = (0, 0);
var
   CpyPth  : string[80];
   Seg     : integer;
   Off     : integer;
   i       : integer;
   options : boolean;
begin
   CpyPth := '';
   ProcZ80($F9, 0, 0, 0, addr(SegOff));            { seg:off of parameter }
   Seg := SegOff[0];
   Off := SegOff[1];
   for i := 1 to GetByt(Seg, Off) do
      DosPth := DosPth + chr(GetByt(Seg, Off + i));

   repeat
      while pos(' ', DosPth) = 1 do               { delete leading spaces }
         delete(DosPth, 1, 1);
      options := pos('-', DosPth) = 1; { any emulator options specified ? }
      if options then                        { deleted any leading option }
         begin
            delete(DosPth, 1, 1);
            while (DosPth <> '') and (DosPth[1] <> ' ') do
               delete(DosPth, 1, 1);
         end
   until not options;

   while pos(' ', DosPth) = length(DosPth) do
      DosPth[0] := chr(length(DosPth) - 1);
   for i := 1 to length(DosPth) do
      CpyPth := CpyPth + upcase(DosPth[i]);
   if pos(' ', CpyPth) = 0 then                     { possibly a SUB file }
      begin
         i := pos('.SUB', CpyPth);
         if (i > 0) and (i = (length(CpyPth) - 3)) then
            CopySubmitFile
         else
            MakeSubmitFile
      end
   else
      MakeSubmitFile
end;


function CpmParameter : boolean;
var
   GotPrm : boolean;
begin
   while pos(' ', CpmPth) = 1 do
      delete(CpmPth, 1, 1);
   GotPrm := (CpmPth <> '') and (length(CpmPth) <= 2);
   if GotPrm then
      begin
         if length(CpmPth) = 2 then
            GotPrm := CpmPth[2] = ':'
         else
            CpmPth := CpmPth + ':'
      end;
   CpmPth := CpmPth + FLENME;
   CpmParameter := GotPrm
end;


begin
   CpmPth := ComLne;
   DosPth := '';
   if CpmParameter then
      begin
         if FuncZ80($F8, 0, 0, 0, 0) <> 0 then   { parameter to Z80.EXE ? }
            EmulatorParameter
         else
            MakeSubmitFile
      end
   else
      ReportError('CP/M path error')
end.
