(*************************************************************************)
(*                                                                       *)
(*         SETDRIVE v1.00 (c) Copyright S.J.Kay  7th April 1995          *)
(*                                                                       *)
(*     Support utility for IBM Z80 Emulator CP/M 3 to allow setting      *)
(*               floppy drives to different CP/M formats                 *)
(*                                                                       *)
(*************************************************************************)

TYPE
   String10  = STRING[10];
   String16  = STRING[16];
   String80  = STRING[80];
   String255 = STRING[255];

VAR
   F         : TEXT;
   CpmComLne : String80 ABSOLUTE $0080;
   ComLne    : String80;
   DskNme    : String255;
   DPH       : integer;
   DPB       : integer;
   XLT       : integer;
   DrvTbl    : integer;
   DrvCde    : byte;
   RDRV      : byte;
   DPBM      : ARRAY [0..5] OF integer;
   DPBD      : ARRAY [0..16] OF byte;
   SKWD      : ARRAY [0..255] OF byte;


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


procedure PutBIOSdata (AdrSor, AdrDst, Amount : integer);
begin
   BiosX(28, 0, $0001, 0, 0);       { set xmove banks, bank #1 to bank #0 }
   BiosX(24, 0, Amount, AdrSor, AdrDst)                     { move memory }
end;


procedure PutBIOSword (AdrDst, WrdPut : integer);
const
   WrdSor : integer = 0;
begin
   WrdSor := WrdPut;
   PutBIOSdata(addr(WrdSor), AdrDst, 2)
end;


procedure PutBIOSbyte (AdrDst : integer; BytPut : byte);
const
   BytSor : byte = 0;
begin
   BytSor := BytPut;
   PutBIOSdata(addr(BytSor), AdrDst, 1)
end;


FUNCTION OpenDataFile (FleNme : String80) : boolean;
BEGIN
{$I-}
   assign(F, FleNme);
   reset(F);
   OpenDataFile := ioresult = 0
{$I+}
END;


PROCEDURE CloseDataFile;
VAR
   Dummy : integer;
BEGIN
{$I-}
   close(F);
   Dummy := ioresult
{$I+}
END;


FUNCTION ReadDataFile (VAR Data : String255) : integer;
BEGIN
   ReadDataFile := ord(eof(F));
   IF NOT eof(F) THEN
      readln(F, Data)
END;


PROCEDURE ReadTextFile (VAR   Data : String255;
                        VAR      X : integer;
                        VAR      L : integer;
                        VAR Result : integer);
BEGIN
   Data := '';
   WHILE (Data = '') AND (Result = 0) DO
      BEGIN
         Result := ReadDataFile(Data);
         IF Data <> '' THEN
            BEGIN
               X := 1;
               L := length(Data);
               WHILE (X <= L) AND (Data[X] = ' ') DO
                  X := X + 1;
               IF X > L THEN
                  Data := ''
               ELSE
                  IF Data[X] = chr(39) THEN
                     Data := ''
            END
      END
END;


PROCEDURE StrgData (VAR   Data : String255;
                    VAR RetStr : String255;
                             X : integer;
                             L : integer;
                         UpStr : boolean);
BEGIN
   RetStr := '';
   WHILE L > 0 DO
      BEGIN
         IF UpStr THEN
            RetStr := RetStr + upcase(Data[X])
         ELSE
            RetStr := RetStr + Data[X];
         X := X + 1;
         L := L - 1
      END
END;


PROCEDURE FindLabel (VAR   Data : String255;
                     VAR LabNme : String16;
                     VAR      X : integer;
                     VAR      L : integer;
                     VAR Result : integer);
BEGIN
   REPEAT
      LabNme := '';
      ReadTextFile(Data, X, L, Result);
      IF Result = 0 THEN
         BEGIN
            WHILE (X <= L) AND (NOT (Data[X] IN [':', ' '])) DO
               BEGIN
                  LabNme := LabNme + upcase(Data[X]);
                  X := X + 1
               END;
            X := X + 1;
            WHILE (X <= L) AND (Data[X] = ' ') DO
               X := X + 1;
            L := (L + 1) - X
         END
   UNTIL (Result > 0) or (LabNme <> '')
END;


PROCEDURE ExtractData (VAR   Data : String255;
                       VAR InpDat : String10;
                       VAR      X : integer;
                       VAR      L : integer);
BEGIN
   InpDat := '';
   WHILE (X <= L) AND (Data[X] = ' ') DO
      X := X + 1;
   IF (X <= L) AND (Data[X] = ',') THEN
      X := X + 1;
   WHILE (X <= L) AND (Data[X] = ' ') DO
      X := X + 1;
   WHILE (X <= L) AND (NOT(Data[X] IN [' ', ','])) DO
      BEGIN
         InpDat := concat(InpDat, upcase(Data[X]));
         X := X + 1
      END
END;


PROCEDURE DiskHardwareLabel (VAR   Data : String255;
                             VAR      X : integer;
                             VAR Result : integer);
VAR
   InpDat : String10;
   Count  : integer;
   L      : integer;
BEGIN
   L := length(Data);
   Count := 0;
   RDRV := 0;
   WHILE (Result = 0) AND (Count < 3) DO
      BEGIN
         ExtractData(Data, InpDat, X, L);
         CASE Count OF
            0 : IF InpDat = 'SS' THEN
                   RDRV := $80
                ELSE
                   IF InpDat = 'DS' THEN
                      RDRV := $40
                   ELSE
                      IF InpDat = 'UD' THEN
                         RDRV := 0
                      ELSE
                         Result := 255;
            1 : IF InpDat = 'SD' THEN
                   BEGIN
                      { ignore this value }
                   END
                ELSE
                   IF InpDat = 'DD' THEN
                      BEGIN
                         { ignore this value }
                      END
                   ELSE
                      Result := 255;
            2 : IF InpDat = 'LO' THEN
                   BEGIN
                      { ignore this value }
                   END
                ELSE
                   IF InpDat = 'HI' THEN
                      BEGIN
                         { ignore this value }
                      END
                   ELSE
                      Result := 255
         END;
         Count := Count + 1
      END;
   RDRV := RDRV + DrvCde AND $03
END;
