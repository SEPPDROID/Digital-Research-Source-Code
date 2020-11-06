(*************************************************************************)
(*                                                                       *)
(*           FORMAT v1.00 (c) Copyright S.J.Kay   2nd May 1995           *)
(*                                                                       *)
(*     Support utility for IBM Z80 Emulator CP/M 3 to allow formating    *)
(*                   floppy disks to a CP/M format                       *)
(*                                                                       *)
(*************************************************************************)

{$C-}                                       { turn off ^C and ^S checking }

type
   registers = record
                  case boolean of
                     true  : (AL, AH, BL, BH, CL, CH, DL, DH : byte);
                     false : (AX,     BX,     CX,     DX,
                              BP, SI, DI, DS, ES, FLAGS      : integer)
               end;

const
   DrvDta : array [0..6, 0..3] of integer =
   (
    { Media EndTrk SecTrk TrkCap }
    (  1,    39,     9,    6250  ),     { 40T DD diskette in 40T DD 5.25" }
    (  2,    39,     9,    6250  ),     { 40T DD diskette in 80T HD 5.25" }
    (  1,    39,     9,    6250  ),     { 80T DD diskette in 80T HD 5.25" }
    (  3,    79,    15,   10416  ),     { 80T HD diskette in 80T HD 5.25" }
    (  4,    79,     9,    6250  ),     { 80T DD diskette in 80T DD  3.5" }
    (  4,    79,     9,    6250  ),     { 80T DD diskette in 80T HD  3.5" }
    (  4,    79,    18,   12500  )      { 80T HD diskette in 80T HD  3.5" }
   );

var
   R       : registers;
   ComLne  : string[127] absolute $0080;
   ComPrm  : string[127];
   DPT     : array[0..10] of byte;
   DrvCde  : integer;
   DPH     : integer;
   DrvInf  : char;


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


procedure SetByt (Seg, Off : integer ; BytPut : byte);
begin
   ProcZ80($B1, BytPut, 0, Seg, Off)
end;


function GetWrd (Seg, Off : integer) : integer;
begin
   GetWrd := FuncZ80($B0, 0, 0, Seg, Off) +
             FuncZ80($B0, 0, 0, Seg, Off + 1) shl 8
end;


procedure SetWrd (Seg, Off, WrdPut : integer);
begin
   ProcZ80($B1, lo(WrdPut), 0, Seg, Off);
   ProcZ80($B1, hi(WrdPut), 0, Seg, Off + 1)
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


procedure DiskReset;
begin
   R.AH := $00;
   R.DL := DrvCde;
   intr($13, R)
end;


procedure GetInterrupt (Int : byte; var S, O : integer);
begin
   R.AH := $35;
   R.AL := Int;
   Msdos(R);
   S := R.ES;
   O := R.BX
end;


procedure SetInterrupt (Int : byte; S, O : integer);
begin
   R.AH := $25;
   R.AL := Int;
   R.DS := S;
   R.DX := O;
   Msdos(R)
end;


procedure ReportError (ErrCde : byte);
begin
   writeln;
   writeln;
   case ErrCde of
      $01 : writeln('Bad command');
      $02 : writeln('Address mark not found');
      $03 : writeln('Disk is write protected');
      $04 : writeln('Sector not found');
      $06 : writeln('Diskette removed');
      $08 : writeln('DMA overrun');
      $09 : writeln('DMA across 64 KB boundary');
      $0C : writeln('Bad media type');
      $10 : writeln('Bad CRC or ECC');
      $20 : writeln('Controller failed');
      $40 : writeln('Seek failed');
      $80 : writeln('Drive not ready')
         else
            writeln('Unknown disk error')
   end
end;


procedure DataFromCPM (var TotSde : integer;
                       var CylTot : integer;
                       var EndTrk : integer;
                       var TotSec : integer;
                       var SecSze : integer);
var
   DPB, SPT, DSM, OFF : integer;
   UNT, BLM, PSH, PHM : byte;
begin
   UNT := GetBIOSbyte(DPH-2);
   DPB := GetBIOSword(DPH+12);
   SPT := GetBIOSword(DPB);
   BLM := GetBIOSbyte(DPB+3);
   DSM := GetBIOSword(DPB+5);
   OFF := GetBIOSword(DPB+13);
   PSH := GetBIOSbyte(DPB+15);
   PHM := GetBIOSbyte(DPB+16);
   TotSde := 2 - (UNT shr 7);
   CylTot := ((((DSM + 1) * (BLM + 1) + BLM) div SPT) + OFF);
   EndTrk := CylTot div TotSde - 1;
   TotSec := SPT div (PHM + 1);
   SecSze := PSH
end;


procedure CalcDiskValues (    TotSec, SecSze, EndTrk : integer;
                         var Gap0, Gap1              : integer);
var
   TrkCap, TrkLen : integer;
   x              : integer;
begin
   if DrvInf = #0 then
      begin
         if EndTrk > 60 then
            begin
               if (TotSec * (128 shl SecSze)) > 5120 then
                  DrvInf := '7'   { 80T HD diskette in 80T HD  3.5" drive }
               else
                  DrvInf := '6'   { 80T DD diskette in 80T HD  3.5" drive }
            end
         else
            DrvInf := '2'         { 40T DD diskette in 80T HD 5.25" drive }
      end;
   x := ord(DrvInf) - ord('1');
   TrkCap := DrvDta[x, 3];
   TrkLen := TrkCap - trunc(TrkCap * (3.5 / 100.0) + 96.0);
   Gap1 := (TrkLen - (TotSec * (62 + (128 shl SecSze)))) div TotSec;
   if Gap1 > 255 then
      Gap1 := 255;
   Gap0 := Gap1 div 3
end;


function SetupFloppy : boolean;
var
   x, Retry : integer;
   Error    : boolean;
begin
   Retry := 3;
   x := ord(DrvInf) - ord('1');
   repeat
      R.AH := $17;
      R.AL := DrvDta[x, 0];
      R.DL := DrvCde;
      intr($13, R);
      Retry := Retry - 1;
      Error := odd(R.FLAGS);
      if Error and (Retry <> 0) then
         DiskReset
   until (not Error) or (Retry = 0);
   Retry := 3;
   if not Error then
      repeat
         R.AH := $18;
         R.CH := DrvDta[x, 1];
         R.CL := DrvDta[x, 2];
         R.DL := DrvCde;
         intr($13, R);
         Retry := Retry - 1;
         Error := odd(R.FLAGS);
         if Error and (Retry <> 0) then
            DiskReset
      until (not Error) or (Retry = 0);
   if Error then
      ReportError(R.AH);
   SetupFloppy := not Error
end;


function VerifyFormat (TotSec, SdeNmb, CylNmb : integer) : boolean;
var
   Retry : integer;
   Error : boolean;
begin
   R.AH := $04;
   R.AL := TotSec;
   R.CH := CylNmb;
   R.CL := 1;
   R.DH := SdeNmb;
   R.DL := DrvCde;
   intr($13, R);
   Error := odd(R.FLAGS);
   if Error then
      ReportError(R.AH);
   VerifyFormat := Error
end;


function TrackFormat (SdeNmb, CylNmb, TotSec, SecSze : integer) : boolean;
var
   FmtTbl : array [0..255, 0..3] of byte;
   i      : integer;
   SecNmb : integer;
begin
   SecNmb := 1;
   for i := 0 to TotSec - 1 do
      begin
         FmtTbl[i, 0] := CylNmb;
         FmtTbl[i, 1] := SdeNmb;
         FmtTbl[i, 2] := SecNmb;
         FmtTbl[i, 3] := SecSze;
         SecNmb := SecNmb + 1
      end;
   R.AH := $05;
   R.AL := $01;
   R.CH := CylNmb;
   R.CL := $00;
   R.DH := SdeNmb;
   R.DL := DrvCde;
   R.ES := seg(FmtTbl);
   R.BX := ofs(FmtTbl);
   intr($13, R);
   if odd(R.FLAGS) then
      ReportError(R.AH);
   TrackFormat := odd(R.FLAGS)
end;


procedure FormatFloppy (TotSde, CylTot, EndTrk, TotSec, SecSze : integer);
var
   SdeNmb, CylNmb, CylCnt : integer;
   Finish, Error          : boolean;
   Key                    : char;
begin
   SdeNmb := 0;
   CylNmb := 0;
   CylCnt := 0;
   Finish := false;
   repeat
      if keypressed then
         begin
            read(kbd, Key);
            Finish := Key = ^[
         end;
      if not Finish then
         begin
            Error := TrackFormat(SdeNmb, CylNmb, TotSec, SecSze);
            if not Error then
               Error := VerifyFormat(TotSec, SdeNmb, CylNmb);
            SdeNmb := (SdeNmb + ord(TotSde = 2)) and $01;
            CylNmb := CylNmb + ord(SdeNmb = 0);
            CylCnt := CylCnt + 1;
            Finish := CylNmb > EndTrk;
            if not Error then
               write((CylCnt / CylTot) * 100.0:3:0, ^M)
         end
   until Error or Finish;
   bdos(13)
end;


procedure StartFormat;
var
   TotSde, CylTot, EndTrk, TotSec, SecSze : integer;
   Gap0,   Gap1                           : integer;
   DskPrmS, DskPrmO, i                    : integer;
begin
   DataFromCPM(TotSde, CylTot, EndTrk, TotSec, SecSze);
   CalcDiskValues(TotSec, SecSze, EndTrk, Gap0, Gap1);
   writeln('Formatting: Tracks=', EndTrk+1, '  Sides=', TotSde,
           '  Sec/Trk=', TotSec, '  Byt/Sec=', 128 shl SecSze,
           '  Gap=', Gap1);
   write('  0 percent completed', ^M);
   if SetupFloppy then
      begin
         GetInterrupt($1E, DskPrmS, DskPrmO);
         for i := 0 to 10 do
            DPT[i] := GetByt(DskPrmS, DskPrmO + i);
         DPT[3] := SecSze;                         { set sector size code }
         DPT[4] := TotSec;                     { set last sector on track }
         DPT[5] := Gap0;             { set intersector gap for read/write }
         DPT[7] := Gap1;                 { set intersector gap for format }
         DPT[8] := $E5;                               { data format value }
         SetInterrupt($1E, seg(DPT), ofs(DPT));
         DiskReset;
         FormatFloppy(TotSde, CylTot, EndTrk, TotSec, SecSze);
         SetInterrupt($1E, DskPrmS, DskPrmO);
         DiskReset
      end
end;


procedure PromptUser;
var
   FmtNxt : STRING[1];
   Key    : char;
begin
   repeat
      writeln;
      writeln('Insert a disk to be formatted in drive ',
      chr(ord('A') + DrvCde));
      writeln;
      writeln('Press ENTER when ready');
      repeat
         read(kbd, Key)
      until Key in [^M, ^[, ^C];
      if Key = ^M then
         begin
            writeln;
            StartFormat;
            writeln;
            writeln;
            write('Format another disk (y/n) ?: ');
            buflen := 1;
            readln(FmtNxt)
         end;
      writeln
   until ((FmtNxt <> 'Y') and (FmtNxt <> 'y')) or (Key = ^[)
end;


procedure CheckAndFormat;
begin
   DrvCde := ord(ComPrm[1]) - ord('A');
   DPH := GetBIOSword(bioshl(21) + DrvCde * 2);
   if (DPH <> 0) and (DrvCde in [0..1]) then
      PromptUser
   else
      writeln('Drive specified not supported')
end;


procedure ShowUsage;
begin
   writeln;
   writeln('FORMAT v1.00 (c) Copyright  S.J.Kay  2nd May 1995');
   writeln;
   writeln('Formats disk according to the data held in the CP/M 3 DPB');
   writeln;
   writeln('If more than 59 tracks:-');
   writeln('   Format will be 80T DD/HD diskette in 80T HD 3.5" drive');
   writeln('If less than 60 tracks:-');
   writeln('   Format will be 40T DD diskette in 80T HD 5.25" drive');
   writeln;
   writeln('Use an appropriate switch if other hardware is required.');
   writeln;
   writeln('Use:- FORMAT D: [/1/2/3/4/5/6/7]');
   writeln;
   writeln('D: = format diskette in drive A or B');
   writeln('/1 = 40T DD diskette in 40T DD 5.25" drive');
   writeln('/2 = 40T DD diskette in 80T HD 5.25" drive');
   writeln('/3 = 80T DD diskette in 80T HD 5.25" drive');
   writeln('/4 = 80T HD diskette in 80T HD 5.25" drive');
   writeln('/5 = 80T DD diskette in 80T DD  3.5" drive');
   writeln('/6 = 80T DD diskette in 80T HD  3.5" drive');
   writeln('/7 = 80T HD diskette in 80T HD  3.5" drive')
end;


var
   Error : boolean;
begin
   DrvInf := #0;
   ComPrm := ComLne;
   while pos(' ', ComPrm) <> 0 do
      delete(ComPrm, pos(' ', ComPrm), 1);
   Error := pos(':', ComPrm) <> 2;
   if not Error then
      begin
         if (pos('/', ComPrm) = 3) and (length(ComPrm) = 4) then
            begin
               Error := not (ComPrm[4] in ['1'..'7']);
               if not Error then
                  DrvInf := ComPrm[4]
            end
         else
            Error := (length(ComPrm) <> 2) and (DrvInf = #0);
         if not Error then
            CheckAndFormat
         else
            ShowUsage
      end
   else
      ShowUsage
end.
