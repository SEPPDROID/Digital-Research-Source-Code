(*************************************************************************)
(*                                                                       *)
(*         SETDRIVE v1.00 (c) Copyright S.J.Kay  7th April 1995          *)
(*                                                                       *)
(*     Support utility for IBM Z80 Emulator CP/M 3 to allow setting      *)
(*               floppy drives to different CP/M formats                 *)
(*                                                                       *)
(*************************************************************************)

{$I SETDRV.INC }

FUNCTION Log2 (X : integer): integer;
VAR
   Y : integer;
BEGIN
   Y := 0;
   WHILE X > 1 DO
      BEGIN
         X := X DIV 2;
         Y := Y + 1
      END;
   Log2 := Y
END;


FUNCTION ALV01 (Blocks : integer): integer;
VAR
   X, BitPos : integer;
BEGIN
   X := 1024 * 32;
   BitPos := 0;
   WHILE Blocks > 0 DO
      BEGIN
         BitPos := BitPos + X;
         X := X shr 1;
         Blocks := Blocks - 1
      END;
   ALV01 := BitPos
END;


PROCEDURE SetDPBData;
VAR
   SPT, BSH, BLM, EXM, DSM, DRM, ALV, CKS, OFF, PSH, PHM : integer;
BEGIN
   SPT := (DPBM[0] DIV 128) * DPBM[1];
   DPBD[0] := lo(SPT);
   DPBD[1] := hi(SPT);
   BSH := Log2(DPBM[3] DIV 128);
   DPBD[2] := BSH;
   BLM := DPBM[3] DIV 128 - 1;
   DPBD[3] := BLM;
   DSM := (SPT * (DPBM[2] - DPBM[5])) DIV (BLM + 1) - 1;
   EXM := DPBM[3] DIV ((ord(DSM > 255) + 1) * 1024) - 1;
   DPBD[4] := EXM;
   DPBD[5] := lo(DSM);
   DPBD[6] := hi(DSM);
   DRM := DPBM[4] - 1;
   DPBD[7] := lo(DRM);
   DPBD[8] := hi(DRM);
   ALV := ALV01((DPBM[4] * 32 + (DPBM[3] - 32)) DIV DPBM[3]);
   DPBD[9] := hi(ALV);
   DPBD[10] := lo(ALV);
   CKS := DPBM[4] DIV 4;
   DPBD[11] := lo(CKS);
   DPBD[12] := hi(CKS);
   OFF := DPBM[5];
   DPBD[13] := lo(OFF);
   DPBD[14] := hi(OFF);
   PSH := Log2(DPBM[0] DIV 128);
   DPBD[15] := PSH;
   PHM := DPBM[0] DIV 128 - 1;
   DPBD[16] := PHM
END;


PROCEDURE DiskDPBLabel (VAR   Data : String255;
                        VAR      X : integer;
                        VAR Result : integer);
VAR
   InpDat           : String10;
   Count, Number, L : integer;
BEGIN
   L := length(Data);
   Count := 0;
   WHILE (Result = 0) AND (Count < 6) DO
      BEGIN
         ExtractData(Data, InpDat, X, L);
         IF InpDat = '' THEN
            Result := 255
         ELSE
            val(InpDat, Number, Result);
         DPBM[Count] := Number;
         Count := Count + 1
      END;
   SetDPBData
END;


PROCEDURE MakeSkewData (VAR   Data : String255;
                        VAR      X : integer;
                        VAR Result : integer);
VAR
   UseSec                               : ARRAY [0..255] OF boolean;
   FstSkw, SkwFac, FstPhy, TotSec       : byte;
   EndSec, NxtSec, Number, Count, SF, L : integer;
   InpDat                               : String10;
BEGIN
   L := length(Data);
   Count := 0;
   WHILE (Result = 0) AND (Count < 3) DO
      BEGIN
         ExtractData(Data, InpDat, X, L);
         IF InpDat = '' THEN
            Result := 255
         ELSE
            val(InpDat, Number, Result);
         IF Result = 0 THEN
            CASE Count OF
               0 : SkwFac := Number;
               1 : FstSkw := Number;
               2 : FstPhy := Number
            END;
         Count := Count + 1
      END;
   TotSec := DPBM[1];
   IF Result = 0 THEN
      BEGIN
         FOR Count := 0 TO 255 DO
            UseSec[Count] := false;
         Count := 1;
         EndSec := FstPhy + (TotSec - 1);
         NxtSec := FstSkw;
         UseSec[FstSkw] := true;
         SKWD[0] := FstSkw;
         WHILE (Count < TotSec) AND (Result = 0) DO
            BEGIN
               SF := SkwFac;
               WHILE SF > 0 DO
                  BEGIN
                     NxtSec := NxtSec + 1;
                     IF NxtSec > EndSec THEN
                        NxtSec := FstPhy;
                     SF := SF - 1
                  END;
               WHILE UseSec[NxtSec] DO
                  NxtSec := NxtSec + 1;
               UseSec[NxtSec] := true;
               SKWD[Count] := NxtSec;
               Count := Count + 1
            END
      END
   ELSE
      writeln('Error in skew definition')
END;


PROCEDURE ExtractSkewData (VAR   Data : String255;
                           VAR      X : integer;
                           VAR Result : integer);
VAR
   InpDat : String10;
   LabNme : String16;
   Count  : integer;
   Number : integer;
   L      : integer;
BEGIN
   L := length(Data);
   Count := 0;
   WHILE (Result = 0) AND (Count < DPBM[1]) DO
      BEGIN
         ExtractData(Data, InpDat, X, L);
         IF InpDat = '' THEN
            BEGIN
               FindLabel(Data, LabNme, X, L, Result);
               IF LabNme <> 'SKD' THEN
                  Result := 255
               ELSE
                  L := length(Data)
            END
         ELSE
            BEGIN
               val(InpDat, Number, Result);
               IF Result = 0 THEN
                  SKWD[Count] := Number;
               Count := Count + 1
            END
      END
END;


PROCEDURE ReadFormat (VAR LabNme : String16;
                      VAR Result : integer);
VAR
   Data   : String255;
   FulNme : String255;
   C      : integer;
   X      : integer;
   L      : integer;
BEGIN
   C := 0;
   REPEAT
      FindLabel(Data, LabNme, X, L, Result);
      IF Result = 0 THEN
         CASE C OF
            0 : BEGIN
                   Result := ord(LabNme <> 'NME');
                   IF Result = 0 THEN
                      StrgData(Data, DskNme, X, L, false);
                END;
            1 : BEGIN
                   Result := ord(LabNme <> 'DSK');
                   IF Result = 0 THEN
                      DiskHardwareLabel(Data, X, Result)
                END;
            2 : BEGIN
                   Result := ord(LabNme <> 'FMT');
                   { ignore this label }
                END;
            3 : BEGIN
                   Result := ord(LabNme <> 'DPB');
                   IF Result = 0 THEN
                      DiskDPBLabel(Data, X, Result)
                END;
            4 : BEGIN
                   Result := ord((LabNme <> 'SKW') AND (LabNme <> 'SKD'));
                   IF Result = 0 THEN
                      IF LabNme = 'SKW' THEN
                         MakeSkewData(Data, X, Result)
                      ELSE
                         ExtractSkewData(Data, X, Result)
                END
         END;
      C := C + 1
   UNTIL (Result > 0) OR (C > 4)
END;


PROCEDURE FindKeyName (VAR Result : integer);
VAR
   LabNme : String16;
   Data   : String255;
   KeyNme : String255;
   KeyFnd : boolean;
   X      : integer;
   L      : integer;
BEGIN
   KeyFnd := false;
   WHILE NOT (KeyFnd) AND (Result = 0) DO
      BEGIN
         LabNme := '';
         WHILE (LabNme <> 'KEY') AND (Result = 0) DO
            FindLabel(Data, LabNme, X, L, Result);
         IF LabNme = 'KEY' THEN
            BEGIN
               StrgData(Data, KeyNme, X, L, true);
               KeyFnd := KeyNme = ComLne
            END
      END;
   IF NOT KeyFnd THEN
      writeln('Error, Disk key not found')
END;



PROCEDURE InstallData;
BEGIN
   DPB := GetBIOSword(DPH+12);
   XLT := GetBIOSword(DPH);
   PutBIOSdata(addr(DPBD), DPB, 17);
   PutBIOSdata(addr(SKWD), XLT, 50);
   PutBIOSbyte(DPH-2, RDRV);
   writeln('Drive ', chr(DrvCde + ord('A')), ': ', DskNme);
   bdos(13)                                                { reset drives }
END;


PROCEDURE SetDrive;
VAR
   Result : integer;
   LabNme : String16;
BEGIN
   Result := 0;
   IF NOT OpenDataFile('CPMDPB.DAT') THEN
      writeln('Error, CP/M disk data file not found')
   ELSE
      BEGIN
         FindKeyName(Result);
         IF Result = 0 THEN
            BEGIN
               LabNme := '';
               WHILE Result = 0 DO
                  ReadFormat(LabNme, Result);
               CloseDataFile;
               InstallData
            END
      END
END;


BEGIN
   ComLne := CpmComLne;
   WHILE pos(' ', ComLne) = 1 do
      delete(ComLne, 1, 1);
   IF (pos(':', ComLne) = 2) THEN
      BEGIN
         DrvCde := ord(ComLne[1]) - ord('A');
         DrvTbl := bioshl(21) + (DrvCde * 2);
         DPH := mem[DrvTbl + 1] SHL 8 + mem[DrvTbl];
         IF (DPH <> 0) AND (DrvCde IN [0..1]) THEN
            BEGIN
               delete(ComLne, 1, 2);
               WHILE pos(' ', ComLne) = 1 do
                  delete(ComLne, 1, 1);
               SetDrive
            END
         ELSE
            writeln('Error, drive specified not supported')
      END
   ELSE
      BEGIN
         writeln('SETDRIVE v1.00 (c) Copyright S.J.Kay  7th April 1995');
         writeln;
         writeln('Use the format:- SETDRIVE D: DISKNAME')
      END
END.
