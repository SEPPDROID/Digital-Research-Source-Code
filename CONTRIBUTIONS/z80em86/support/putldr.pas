{*************************************************************************}
{*                                                                       *}
{*           PUTLDR v1.00 (c) Copyright S.J.Kay 18th April 1995          *}
{*                                                                       *}
{*           Places S.J.Kay's CP/M 3.0 loader on system stracks          *}
{*                                                                       *}
{*************************************************************************}

{                       LAYOUT OF CPMLDR.SYS FILE                         }
{                                                                         }
{ FILE OFFSET   R/W/C   DESCRIPTION OF ITEM                               }
{   0,  1   2     c     JP XXXX jumps over data                           }
{   3   4         r     if AA55 hex value here then assume correct file   }
{   5             r     CPMLDR.SYS version                                }
{   6,  7         r     CPMLDR.SYS execute address (calculate offsets)    }
{   8,  9         r     DPH table address                                 }
{  10, 11         r     DPB table address                                 }
{  12, 13         r     XLT sector translate table address                }
{  14, 15         w     bytes in physical sector                          }
{  16             w     physical sectors per track                        }
{  17             w     RDRV (udf, density, type, physical drive)         }
{  18             w     tracks to be loaded                               }
{  19             w     flag for loader to initialize banked system       }

{$C-}                                       { turn off ^C and ^S checking }


type
   String80 = string[80];

const
   MAXBUF = 127;

var
   Version    : integer;
   DrvChr     : char;
   DrvCde     : integer;
   BnkFlg     : byte;
   Parameters : String80;
   ComLne     : String80 absolute $0080;
   FleBuf     : array[0..MAXBUF, 0..127] of byte;
   VerBuf     : array[0..1023] of byte;

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


FUNCTION LoadSystemFile (VAR FleRec : integer) : boolean;
VAR
   Error : integer;
   F     : FILE;
BEGIN
   assign(F, 'CPMLDR.SYS');
   FleRec := 0;
{$I-}
   reset(F);
   Error := ioresult;
{$I+}
   IF Error = 0 THEN
      BEGIN
         IF NOT eof(F) THEN
            BEGIN
               WHILE NOT eof(F) AND (FleRec <= MAXBUF) AND (Error = 0) DO
                  BEGIN
{$I-}
                     blockread(F, FleBuf[FleRec, 0], 1);
                     Error := ioresult;
{$I+}
                     FleRec := FleRec + 1
                  END;
               close(F);
               IF Error <> 0 THEN
                  writeln('Error reading CPMLDR.SYS')
               ELSE
                  IF NOT eof(F) THEN
                     BEGIN
                        writeln('Error, CPMLDR.SYS to big too load');
                        Error := 1
                     END
                  ELSE
                     BEGIN
                        IF (FleBuf[0,3] <> $55) AND (FleBuf[0,4] <> $AA) THEN
                           BEGIN
                              writeln('Error, not the correct file');
                              Error := 1
                           END
                     END
            END
         ELSE
            BEGIN
               writeln('Error, CPMLDR.SYS is an empty file');
               Error := 1
            END
      END
   ELSE
      writeln('CPMLDR.SYS not found');
   LoadSystemFile := Error = 0
END;


PROCEDURE WriteLoaderFile (    FleRec : integer;
                               SecTrk : integer;
                               SecCnt : integer;
                               BufOff : integer;
                           VAR Error  : integer);
VAR
   Track, Sector, BufPos : integer;
BEGIN
   BiosCall(9, 0, DrvCde, 1, 0);
   writeln('Placing CPMLDR.SYS onto systems tracks of drive ',
           chr(DrvCde + ord('A')), ':');
   BufPos := 0;
   Sector := 1;
   Track := 0;
   REPEAT
      IF Sector > SecTrk THEN
         BEGIN
            Track := Track + 1;
            Sector := 1
         END;
      BiosCall(10, 0, Track, 0, 0);                  { set physical track }
      BiosCall(11, 0, Sector, 0, 0);                { set physical sector }
      BiosCall(12, 0, addr(FleBuf[BufPos, 0]), 0, 0);   { set DMA address }
      BiosCall(28, 1, 0, 0, 0);                  { set data bank #1 (TPA) }
      Error := BiosFunc(14, 0, 0, 0, 0);                 { write 1 sector }
      BufPos := BufPos + BufOff;
      Sector := Sector + 1;
      SecCnt := SecCnt - 1
   UNTIL (SecCnt = 0) OR (Error <> 0);
   IF Error > 0 THEN
      writeln('Error writing system tracks')
END;


PROCEDURE VerifyLoaderTracks (    FleRec : integer;
                                  SecTrk : integer;
                                  SecCnt : integer;
                              VAR Error  : integer);
VAR
   Track, Sector : integer;
BEGIN
   writeln('Verifying...');
   Sector := 1;
   Track := 0;
   REPEAT
      IF Sector > SecTrk THEN
         BEGIN
            Track := Track + 1;
            Sector := 1
         END;
      BiosCall(10, 0, Track, 0, 0);                  { set physical track }
      BiosCall(11, 0, Sector, 0, 0);                { set physical sector }
      BiosCall(12, 0, addr(VerBuf), 0, 0);              { set DMA address }
      BiosCall(28, 1, 0, 0, 0);                  { set data bank #1 (TPA) }
      Error := BiosFunc(13, 0, 0, 0, 0);                  { read 1 sector }
      Sector := Sector + 1;
      SecCnt := SecCnt - 1
   UNTIL (SecCnt = 0) OR (Error <> 0);
   IF Error > 0 THEN
      writeln('Error, CRC error detected')
END;


PROCEDURE WriteSystemTracks (FleRec : integer;
                             SecTrk : integer;
                             SecCnt : integer;
                             BufOff : integer);
VAR
   Error : integer;
BEGIN
   WriteLoaderFile(FleRec, SecTrk, SecCnt, BufOff, Error);
   IF Error = 0 THEN
      VerifyLoaderTracks(FleRec, SecTrk, SecCnt, Error)
END;


PROCEDURE SetDskInf (DPH, XLT, DPB, PHM, SecTrk, TrkCnt : integer);
VAR
   FleBse, ExeAdr, OffSet, SecLen : integer;
   DPBPos, XLTPos                 : integer;
BEGIN
   FleBse := addr(FleBuf);                  { base address of file buffer }
   ExeAdr := mem[FleBse + 6] + mem[FleBse + 7] SHL 8; { code exec address }
   OffSet := FleBse - ExeAdr;
   SecLen := (PHM + 1) * 128;                   { calculate sector length }
   DPBPos := (mem[FleBse + 10] + mem[FleBse + 11] SHL 8) + OffSet;
   GetBIOSdata(DPB, DPBPos, 17);
   XLTPos := (mem[FleBse + 12] + mem[FleBse + 13] SHL 8) + OffSet;
   GetBIOSdata(XLT, XLTPos, 64);
   mem[FleBse + 14] := lo(SecLen);                 { LSB of sector length }
   mem[FleBse + 15] := hi(SecLen);                 { MSB of sector length }
   mem[FleBse + 16] := SecTrk;                   { physical sectors/track }
   mem[FleBse + 17] := GetBIOSbyte(DPH-2);       { get UNIT byte from DPH }
   mem[FleBse + 18] := TrkCnt;                      { tracks to be loaded }
   mem[FleBse + 19] := BnkFlg          { banked loader configuration flag }
END;


FUNCTION DestinationTracks (    FleRec : integer;
                            VAR SecTrk : integer;
                            VAR SecCnt : integer;
                            VAR BufOff : integer) : boolean;
VAR
   TblAdr, TrkCnt, Error        : integer;
   DPH, XLT, DPB, SPT, PHM, OFF : integer;
BEGIN
   Error := 0;
   DrvCde := ord(DrvChr) - ord('A');
   TblAdr := bioshl(21) + DrvCde * 2;
   DPH := GetBIOSword(TblAdr);
   IF DPH <> 0 THEN
      BEGIN
         XLT := GetBIOSword(DPH);
         DPB := GetBIOSword(DPH+12);
         SPT := GetBIOSword(DPB);                         { sectors/track }
         OFF := GetBIOSword(DPB+13);                       { track offset }
         PHM := GetBIOSbyte(DPB+16);               { physical sector mask }
         IF OFF = 0 THEN
            BEGIN
               writeln('Error, drive ', DrvChr, ': has no system tracks');
               Error := 1
            END
         ELSE
            BEGIN
               IF FleRec > (SPT * OFF) THEN
                  BEGIN
                     writeln('Error, system too big for system tracks');
                     Error := 1
                  END
               ELSE
                  BEGIN
                     SecTrk := SPT DIV (PHM + 1);
                     SecCnt := (FleRec + PHM) DIV (PHM + 1);
                     BufOff := PHM + 1;
                     TrkCnt := (FleRec + (SPT - 1)) DIV SPT;
                     SetDskInf(DPH, XLT, DPB, PHM, SecTrk, TrkCnt)
                  END
            END
      END
   ELSE
      BEGIN
         writeln('Error, drive ', DrvChr, ': does not exist');
         Error := 1
      END;
   DestinationTracks := Error = 0
END;


PROCEDURE PlaceLoader;
VAR
   FleRec, SecTrk, SecCnt, BufOff : integer;
BEGIN
   IF LoadSystemFile(FleRec) THEN
      BEGIN
         if DestinationTracks(FleRec, SecTrk, SecCnt, BufOff) then
            WriteSystemTracks(FleRec, SecTrk, SecCnt, BufOff)
      END
END;


FUNCTION ParmCount (VAR Parameters : String80) : integer;
VAR
   Index, PrmCnt, PrmLen : integer;
BEGIN
   Index := 1;
   PrmCnt := 0;
   PrmLen := length(Parameters);
   WHILE Index <= PrmLen DO
      BEGIN
         WHILE (Index <= PrmLen) AND (Parameters[Index] = ' ') DO
            Index := Index + 1;
         IF Index <= PrmLen
         THEN
            PrmCnt := PrmCnt + 1;
         WHILE (Index <= PrmLen) AND (Parameters[Index] <> ' ') DO
            Index := Index + 1
      END;
   ParmCount := PrmCnt
END;


PROCEDURE RetParmStr (    PrmNmb     : integer;
                      VAR Parameters : String80;
                      VAR PrmStr     : String80);
VAR
   Index, PrmCnt, PrmLen : integer;
BEGIN
   Index := 1;
   PrmCnt := 0;
   PrmLen := length(Parameters);
   WHILE (Index <= PrmLen) AND (PrmCnt < PrmNmb) DO
      BEGIN
         PrmStr := '';
         WHILE (Index <= PrmLen) AND (Parameters[Index] = ' ') DO
            Index := Index + 1;
         IF Index <= PrmLen
         THEN
            PrmCnt := PrmCnt + 1;
         WHILE (Index <= PrmLen) AND (Parameters[Index] <> ' ') DO
            BEGIN
               PrmStr := concat(PrmStr, Parameters[Index]);
               Index := Index + 1
            END
      END
END;


FUNCTION ExtractParameters : boolean;
VAR
   PrmStr : String80;
   Error  : boolean;
BEGIN
   Error := ParmCount(Parameters) <> 2;
   IF NOT Error THEN
      BEGIN
         RetParmStr(1, Parameters, PrmStr);
         PrmStr[1] := upcase(PrmStr[1]);
         Error := (PrmStr <> 'B') and (PrmStr <> 'N');
         IF NOT Error THEN
            BEGIN
               IF PrmStr = 'B' THEN
                  BnkFlg := $ff
               ELSE
                  BnkFlg := $00;
               RetParmStr(2, Parameters, PrmStr);
               PrmStr[1] := upcase(PrmStr[1]);
               Error := length(PrmStr) > 2;
               IF NOT Error AND (length(PrmStr) = 2) THEN
                  Error := PrmStr[2] <> ':';
               DrvChr := PrmStr[1]
            END
      END;
   ExtractParameters := Error = false
END;


procedure ShowUsage;
begin
   writeln;
   writeln('PUTLDR v1.00 (c) Copyright  S.J.Kay  18th April 1995');
   writeln;
   writeln('Places CPMLDR.SYS file on disk system tracks');
   writeln;
   writeln('Use:- putldr x d:');
   writeln;
   writeln(' x = B for a banked loader configuration');
   writeln(' x = N for a non banked loader configuration');
   writeln('d: = destination drive')
end;


begin
   Parameters := ComLne;
   Version := bdoshl(12);
   if (hi(Version) = $00) and (lo(Version) >= $30) then
      begin
         if ExtractParameters then
            PlaceLoader
         else
            ShowUsage
      end
   else
      begin
         writeln;
         writeln('Wrong SYSTEM,  requires CP/M Plus ver 3.0 up')
      end
end.
