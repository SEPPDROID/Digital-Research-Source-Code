(*************************************************************************)
(*                                                                       *)
(*              Functions common to most support programs                *)
(*                      (c) Copyright S.J.Kay                            *)
(*                                                                       *)
(*************************************************************************)

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


type
   registers = record
                  case boolean of
                     true  : (AL, AH, BL, BH, CL, CH, DL, DH : byte);
                     false : (AX,     BX,     CX,     DX,
                              BP, SI, DI, DS, ES, FLAGS      : integer)
               end;



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
