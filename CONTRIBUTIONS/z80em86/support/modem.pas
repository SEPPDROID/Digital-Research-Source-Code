(*************************************************************************)
(*                                                                       *)
(*     SIMPLE MODEM v1.00  (c) Copyright  S.J.Kay  18th April 1995       *)
(*          Using the Ward Christensen file transfer protocol            *)
(*                                                                       *)
(*                Uses CP/M 3.0 AUXIN and AUXOUT routines                *)
(*                                                                       *)
(*************************************************************************)

type
   registers = record
                  case boolean of
                     true  : (AL, AH, BL, BH, CL, CH, DL, DH : byte);
                     false : (AX,     BX,     CX,     DX,
                              BP, SI, DI, DS, ES, FLAGS      : integer)
               end;


const
   PthNme = '';
   ENDBUF = $1FFF;        { must be a multiple of 128 -1 }

   SOH    = $01;          { Start Of Header }
   EOT    = $04;          { End Of Transmission }
   ACK    = $06;          { Acknowledge }
   NAK    = $15;          { Negative Acknowledge }
   CAN    = $18;          { Cancel }


var
   R      : registers;
   F      : file;
   ExtCde : byte;
   Check  : byte;
   BufPos : integer;
   Abort  : boolean;
   TmeOut : boolean;
   FleOpn : boolean;
   Buffer : array [$0000..ENDBUF] of byte;


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


function SerialOutputStatus : byte;
begin
   SerialOutputStatus := bios(18)     { test if AUXOUT is ready }
end;

function SerialInputStatus : byte;
begin
   SerialInputStatus := bios(17)     { test if AUXIN has a character }
end;

procedure SerialOutput (x : byte);
begin
   bios(5, x);
end;

function SerialInput : byte;
begin
   SerialInput := bios(6);
end;


function TickCount : integer;
begin
   R.AH := $00;
   intr($1A, R);
   TickCount := R.DX
end;


procedure TestAbort;
var
   Key : char;
begin
   while keypressed do
      begin
         read(kbd, Key);
         if (Key = ^[) or (Key = ^X) then
            begin
               Abort := true;
               ExtCde := 1
            end
      end
end;


procedure WriteByte (BytVal : byte; TckVal : integer);
var
   BytOut : boolean;
   T      : integer;
begin
   TmeOut := false;
   if SerialOutputStatus <> 0 then
      SerialOutput(BytVal)
   else
      begin
         BytOut := false;
         T := TickCount;
         repeat
            if SerialOutputStatus <> 0 then
               begin
                  SerialOutput(BytVal);
                  BytOut := true
               end
            else
               begin
                  TmeOut := (TickCount - T) >= TckVal;
                  if keypressed then
                     TestAbort
               end
         until BytOut or TmeOut or Abort
      end
end;


function ReadByte (TckVal : integer) : byte;
var
   BytInp : boolean;
   T      : integer;
begin
   TmeOut := false;
   if SerialInputStatus <> 0 then
      ReadByte := SerialInput
   else
      begin
         BytInp := false;
         T := TickCount;
         repeat
            if SerialInputStatus <> 0 then
               begin
                  ReadByte := SerialInput;
                  BytInp := true
               end
            else
               begin
                  ReadByte := $FF;
                  TmeOut := (TickCount - T) >= TckVal;
                  if keypressed then
                     TestAbort
               end
         until BytInp or TmeOut or Abort
      end
end;


procedure Purge;
var
   Dummy : byte;
begin
   repeat
      Dummy := ReadByte(19)                              { 1 sec time out }
   until TmeOut or Abort
end;


procedure SendCancel;
var
   Dummy : byte;
begin
   repeat
      Abort := false;
      Dummy := ReadByte(19)                              { 1 sec time out }
   until TmeOut;
   WriteByte(CAN, 91)                                    { 5 sec time out }
end;


procedure StartNAK;
var
   BytVal, Retry : byte;
begin
   Retry := 15;
   repeat
      Purge;                                       { 1 sec min time taken }
      if not Abort then
         begin
            WriteByte(NAK, 1820);                      { 100 sec time out }
            if TmeOut then
               begin
                  Abort := true;
                  ExtCde := 2
               end;
            if not Abort then
               begin
                  BytVal := ReadByte(56);    { 3 sec time out waiting ACK }
                  if Tmeout then
                     Retry := Retry - 1;
                  if Retry = 0 then
                     begin
                        Abort := true;
                        ExtCde := 3
                     end
               end
         end
   until (BytVal = ACK) or Abort
end;


procedure BlockNAK;
begin
   Purge;
   if not Abort then
      begin
         WriteByte(NAK, 1820);                         { 100 sec time out }
         if TmeOut then
            begin
               Abort := true;
               ExtCde := 4
            end
      end
end;


procedure BlockACK;
begin
   if not Abort then
      begin
         WriteByte(ACK, 1820);                         { 100 sec time out }
         if TmeOut then
            begin
               Abort := true;
               ExtCde := 5
            end
      end
end;


procedure OpenFile;
var
   i      : integer;
   FleNme : string[12];
begin
   BufPos := 0;
   FleNme := '';
   i := 0;
   while (i < 12) and (Buffer[i] <> 0) do
      begin
         FleNme := FleNme + chr(Buffer[i]);
         i := i + 1
      end;
   gotoxy(42, 17);
   write(FleNme, '':12);
   assign(F, PthNme + FleNme);
   rewrite(F);
   FleOpn := true
end;


procedure FlushBuffer;
begin
   blockwrite(F, Buffer, BufPos div 128);
   BufPos := 0
end;


procedure CloseFile;
begin
   if FleOpn then
      begin
         FlushBuffer;
         close(F);
         FleOpn := false
      end
end;


function ReadBlockByte (TckVal : integer) : byte;
var
   BytVal : byte;
begin
   BytVal := ReadByte(TckVal);
   Check := Check + BytVal;
   ReadBlockByte := BytVal
end;


function CheckHeader : boolean;
var
   BytVal : byte;
begin
   BytVal := ReadBlockByte(182);
   if (BytVal = EOT) and (not TmeOut) and (not Abort) then
      begin
         CloseFile;
         WriteByte(ACK, 1820);
         if TmeOut then
            Abort := true;
         if not Abort then
            WriteByte(ACK, 1820);
         if TmeOut then
            Abort := true;
         if Abort then
            ExtCde := 6
      end;
   CheckHeader := BytVal = EOT
end;


procedure ReadBlocks;
var
   Finish                         : boolean;
   i, SveBuf                      : integer;
   GetBlk, BlkNmb, BlkCpl, ChkSum : byte;
   BytVal                         : byte;
begin
   BufPos := 0;
   GetBlk := 0;
   FleOpn := false;
   repeat
      i := 0;
      Check := 0;
      SveBuf := BufPos;
      Finish := CheckHeader;
      if not Finish then
         begin
            if (not TmeOut) and (not Abort) then
               BlkNmb := ReadBlockByte(19);
            if (not TmeOut) and (not Abort) then
               BlkCpl := ReadBlockByte(19);
            while (not TmeOut) and (not Abort) and (i < 128) do
               begin
                  BytVal := ReadBlockByte(19);
                  Buffer[BufPos] := BytVal;
                  BufPos := BufPos + 1;
                  i := i + 1
               end;
            if (not TmeOut) and (not Abort) then
               ChkSum := ReadByte(19);
            if (not TmeOut) and (not Abort) then
               if ChkSum = Check then
                  begin
                     if not FleOpn and (BlkNmb = 0) then
                        begin
                           OpenFile;
                           gotoxy(43, 18);
                           write(BlkNmb, '': 2);
                           BlockACK
                        end;
                     if BlkNmb = GetBlk then
                        begin
                           gotoxy(43, 18);
                           write(BlkNmb, '': 2);
                           GetBlk := GetBlk + 1;
                           if BufPos > ENDBUF then
                              FlushBuffer;
                           BlockACK
                        end
                     else
                        begin
                           BufPos := SveBuf;
                           if BlkNmb = (GetBlk - 1) then
                              BlockACK
                           else
                              begin
                                 Abort := true;
                                 ExtCde := 7
                              end
                        end
                  end;
            if TmeOut or (Check <> ChkSum) then
               begin
                  BlockNAK;
                  BufPos := SveBuf
               end
         end
   until Abort or Finish
end;


procedure Receive;
var
   Retry, Finish : boolean;
   BytVal        : byte;
begin
   Abort := false;
   Finish := false;
   ExtCde := 0;
   repeat
      StartNAK;                             { send NAK, then wait for ACK }
      if not Abort then
         begin
            Retry := true;
            while Retry and not Abort do
               begin
                  BytVal := ReadByte(19); { 1st letter of filename or EOT }
                  Retry := TmeOut;
                  if not Retry then
                     Finish := BytVal = EOT
                  else
                     BlockNAK
               end;
            if (not Finish) and (not Abort) then
               ReadBlocks
         end
   until Finish or Abort;
   if Abort then
      SendCancel;
   gotoxy(1, 23);
   case ExtCde of
      1 : writeln('User cancelled by pressing abort key');
      2 : writeln('Unable to send initial NAK to sender');
      3 : writeln('No ACK response for initial NAK sent');
      4 : writeln('Unable to send NAK for bad block');
      5 : writeln('Unable to send ACK for block received');
      6 : writeln('Unable to send ACK for file end (EOT)');
      7 : writeln('Fatal loss of sync receiving blocks')
   end
end;


begin
   clrscr;
   writeln('             MODEM v1.00  (c) S.J.Kay  1st April 1995');
   writeln;
   writeln('         Uses the Ward Christensen file transfer protocol');
   writeln('                    with file name support.');
   writeln;
   writeln('         NOTE: Files will be written to the default drive.');
   writeln;
   writeln('    Uses AUXIN and AUXOUT devices, set device for correct values');
   writeln('                    before using this program');
   writeln;
   writeln('          To abort press the ESC or ^X keys at any time');
   writeln;
   gotoxy(31, 17);
   write(  'Receiving:');
   gotoxy(31, 18);
   write(  '    Block: #');
   Receive
end.
