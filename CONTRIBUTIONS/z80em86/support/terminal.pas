(*************************************************************************)
(*                                                                       *)
(*    SIMPLE TERMINAL v1.00  (c) Copyright  S.J.Kay  18th April 1995     *)
(*                                                                       *)
(*                Uses CP/M 3.0 AUXIN and AUXOUT routines                *)
(*                                                                       *)
(*************************************************************************)

{$C-}                                       { turn off ^C and ^S checking }

var
   ExtTrm : boolean;


function GetKey : char;
var
   Key : char;
begin
   read(kbd, Key);
   if Key = ^@ then
      begin
         ExtTrm := (Key = ^@);                               { exit key ? }
         GetKey := #0
      end
   else
      GetKey := Key
end;


procedure Terminal;
var
   Key, X : char;
begin
   ExtTrm := false;
   while not ExtTrm do
      begin
         while bios(17) <> 0 do           { test if AUXIN has a character }
            write(chr(bios(6)));
         if keypressed then
            begin
               Key := GetKey;
               if Key <> #0 then
                  if bios(18) <> 0 then
                     bios(5, ord(Key))
            end
      end;
   writeln;
   writeln;
   writeln(' *** TERMINAL EXITED BY USER ***')
end;


begin
   writeln('SIMPLE TERMINAL v1.00  (c) Copyright  S.J.Kay  18th April 1995');
   writeln;
   writeln('Uses CP/M 3.0 AUXIN, AUXOUT, CONIN, CONOUT devices');
   writeln('set these devices for appropriate values');
   writeln;
   writeln('Press ^@ key to exit to system');
   writeln;
   Terminal
end.
