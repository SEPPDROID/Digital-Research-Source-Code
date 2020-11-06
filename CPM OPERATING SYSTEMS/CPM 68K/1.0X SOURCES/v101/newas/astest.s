* BEGIN
**
**INSTCHK.A68              v0.2 82/07/19
**
* 
* 
* PROGREFS.R
* DATAREFS.L
* 
* 
*CODES$ SECTION
* 
*=1000

 
*
*CONSTANTS
*
D02          EQU     2
D04          EQU     4
D7E          EQU     $7E
D7FFC        EQU     $7FFC
D7FFE        EQU     $7FFE
D80          EQU     $80
D82          EQU     $82
D8000        EQU     $8000
D8002        EQU     $8002
D1002        EQU     $1002
DIR15        EQU     $3333
DIR16        EQU     $7FFE
DIR23        EQU     $333333
DIR24        EQU     $7FFFFE
IM3          EQU     $7
IM5          EQU     $1F
IM7          EQU     $33
IM8          EQU     $7E
IM16         EQU     $7FFE
IM32         EQU     $7FFFFFFE
OFF7         EQU     $33
OFF8         EQU     $7E
OFF15        EQU     $3333
OFF16        EQU     $7FFE
RLIST        REG    D0,D1,D2,D3,D5,A5,A6,A7
 
*
*EFFECTIVE ADDRESS TESTS
*
 
*MODE 0
 
 ADDA.W  D0,A0                      ;D0C0
 ADDA.W  D1,A0                      ;D0C1
 ADDA.W  D2,A0                      ;D0C2
 ADDA.W  D3,A0                      ;D0C3
 ADDA.W  D4,A0                      ;D0C4
 ADDA.W  D5,A0                      ;D0C5
 ADDA.W  D6,A0                      ;D0C6
 ADDA.W  D7,A0                      ;D0C7
 
 ADDA.W  D8                         ;*UNDEF ERROR
 
*MODE 1
 
 ADDA.W  A0,A0                      ;D0C8
 ADDA.W  A1,A0                      ;D0C9
 ADDA.W  A2,A0                      ;D0CA
 ADDA.W  A3,A0                      ;D0CB
 ADDA.W  A4,A0                      ;D0CC
 ADDA.W  A5,A0                      ;D0CD
 ADDA.W  A6,A0                      ;D0CE
 ADDA.W  A7,A0                      ;D0CF
 
 ADDA.W  A8,A0                      ;*UNDEF ERROR
 
*MODE 2
 
 ADDA.W  (A1),A0                    ;D0D1
 ADDA.W  0(A1),A0                   ;D0D1
 
 
*MODE 3
 
 ADDA.W  (A1)+,A0                   ;D0D9
 
 ADDA.W  (D1)+,A0                   ;*OPRD ERROR
 ADDA.W  OFF8(A1)+,A0               ;*? OPRD ERROR          !!!
 
*MODE 4
 
 ADDA.W  -(A1),A0                   ;D0E1
 
*MODE 5
 
 ADDA.W  1(A1),A0                   ;D0E9 0001
 ADDA.W  -1(A1),A0                  ;D0E9 FFFF
 ADDA.W  $7FFF(A1),A0               ;D0E9 7FFF
 
 ADDA.W  $10000(A1),A0              ;*OPRD ERROR
 
*MODE 6
 
 ADDA.W  (A1,D2),A0                 ;D0F1 2000
 ADDA.W  0(A1,D2.W),A0              ;D0F1 2000
 ADDA.W  -1(A1,D2.L),A0             ;D0F1 28FF
 ADDA.W  $7F(A1,A3),A0              ;D0F1 B07F
 ADDA.W  -$80(A1,A3.W),A0           ;D0F1 B080
 
 ADDA.W  (D1,D2),A0                 ;*OPRD ERROR
 ADDA.W  (A1,D2.B),A0               ;*OPRD ERROR
 ADDA.W  $D80(A1,D2),A0             ;*OPRD ERROR
 
 
*MODE 7, REGISTER 0
 
 ADDA.W  0,A0                    ;D0F8 0000
 ADDA.W  $0FE,A0                 ;D0F8 00FE
 ADDA.W  -2,A0                   ;D0F8 FFFE
 ADDA.W  DIR16,A0                ;D0F8 7FFE
 
 ADDA.W  DIR16,A0                ;D0F8 7FFE
 ADDA.W  DIR24,A0                ;*OPRD ERROR
 
*MODE 7, REGISTER 1
 
 ADDA.W  $10000,A0                  ;D0F9 0001 0000
 
 ADDA.W  DIR16,A0                ;D0F9 0000 7FFE
 
*MODE 7, REGISTER 2
 
 ADDA.W  *+D02(PC),A0               ;D0FA 0000
 ADDA.W  *+D8000(PC),A0             ;D0FA 7FFE
 ADDA.W  *-D7FFE(PC),A0             ;D0FA 8000
 
 ADDA.W  *+D8002(PC),A0             ;*OPRD ERROR
 ADDA.W  *-D8000(PC),A0             ;*OPRD ERROR
 
*MODE 7, REGISTER 3
 
 ADDA.W  *+D04(PC,D2),A0            ;D0FB 2002
 ADDA.W  *+D80(PC,D2.W),A0          ;D0FB 207E
 ADDA.W  *+D04(PC,D2.L),A0          ;D0FB 2802
 ADDA.W  *+D04(PC,A3),A0            ;D0FB B002
 ADDA.W  *+D04(PC,A3.W),A0          ;D0FB B002
 ADDA.W  *+D04(PC,A3.L),A0          ;D0FB B802
 
 ADDA.W  *+$100(PC,D2)              ;*OPRD ERROR
 
*MODE 7, REGISTER 4
 
 ADDA.W  #DIR16,A0               ;D0FC 7FFE
 
 ADDA.W  #$10000,A0                 ;*OPRD ERROR
 
 ADDA.L  #$10000,A0                 ;D1FC 0001 0000
 
*
*Instruction Tests
*
 
 ABCD    D2,D1                      ;C302
 ABCD    -(A4),-(A3)                ;C70C
 
 ABCD.B  D2,D1                      ;C302
 ABCD.B  -(A4),-(A3)                ;C70C
 
 ABCD.W  D2,D1                      ;*MNEM ERROR
 ABCD.L  D2,D1                      ;*MNEM ERROR
 
 
 ADD     D5,D1                      ;D245
 ADD     A6,D1                      ;D24E
 ADD     (A6),D1                    ;D256
 ADD     (A6)+,D1                   ;D25E
 ADD     -(A6),D1                   ;D266
 ADD     OFF16(A6),D1               ;D26E 7FFE
 ADD     OFF8(A6,D5),D1             ;D276 507E
 ADD     DIR16,D1                ;D278 7FFE
 ADD     DIR24,D1                   ;D279 007F FFFE
 ADD     *+D1002(PC),D1             ;D27A 1000
 ADD     *+D80(PC,D5),D1            ;D27B 507E
 ADD     #IM8,D1                    ;D27C 007E
 
 ADD     D1,D5                      ;DA41
 ADD     D1,A6                      ;DCC1
 ADD     D1,(A6)                    ;D356
 ADD     D1,(A6)+                   ;D35E
 ADD     D1,-(A6)                   ;D366
 ADD     D1,OFF16(A6)               ;D36E 7FFE
 ADD     D1,OFF8(A6,D5)             ;D376 507E
 ADD     D1,DIR16                ;D378 7FFE
 ADD     D1,DIR24                   ;D379 007F FFFE
 ADD     D1,*+D1002(PC)             ;*OPRD ERROR
 ADD     D1,*+D80(PC,D5)            ;*OPRD ERROR
 ADD     D1,#IM8                    ;*OPRD ERROR
 
 ADD.B   D5,D1                      ;D205
 ADD.B   A6,D1                      ;*OPRD ERROR
 ADD.B   (A6),D1                    ;D216
 ADD.B   (A6)+,D1                   ;D21E
 ADD.B   -(A6),D1                   ;D226
 ADD.B   OFF16(A6),D1               ;D22E 7FFE
 ADD.B   OFF8(A6,D5),D1             ;D236 507E
 ADD.B   DIR16,D1                ;D238 7FFE
 ADD.B   DIR24,D1                   ;D239 007F FFFE
 ADD.B   *+D1002(PC),D1             ;D23A 1000
 ADD.B   *+D80(PC,D5),D1            ;D23B 507E
 ADD.B   #IM8,D1                    ;0601 (D23C) 007E
 
 ADD.B   D1,D5                      ;DA01
 ADD.B   D1,A6                      ;*OPRD ERROR
 ADD.B   D1,(A6)                    ;D316
 ADD.B   D1,(A6)+                   ;D31E
 ADD.B   D1,-(A6)                   ;D326
 ADD.B   D1,OFF16(A6)               ;D32E 7FFE
 ADD.B   D1,OFF8(A6,D5)             ;D336 507E
 ADD.B   D1,DIR16                ;D338 7FFE
 ADD.B   D1,DIR24                   ;D339 007F FFFE
 ADD.B   D1,*+D1002(PC)             ;*OPRD ERROR
 ADD.B   D1,*+D80(PC,D5)            ;*OPRD ERROR
 ADD.B   D1,#IM8                    ;*OPRD ERROR
 
 ADD.W   D5,D1                      ;D245
 ADD.W   A6,D1                      ;D24E
 ADD.W   (A6),D1                    ;D256
 ADD.W   (A6)+,D1                   ;D25E
 ADD.W   -(A6),D1                   ;D266
 ADD.W   OFF16(A6),D1               ;D26E 7FFE
 ADD.W   OFF8(A6,D5),D1             ;D276 507E
 ADD.W   DIR16,D1                ;D278 7FFE
 ADD.W   DIR24,D1                   ;D279 007F FFFE
 ADD.W   *+D1002(PC),D1             ;D27A 1000
 ADD.W   *+D80(PC,D5),D1            ;D27B 507E
 ADD.W   #IM8,D1                    ;0641 (D27C) 007E
 
 ADD.W   D1,D5                      ;DA41
 ADD.W   D1,A6                      ;DCC1
 ADD.W   D1,(A6)                    ;D356
 ADD.W   D1,(A6)+                   ;D35E
 ADD.W   D1,-(A6)                   ;D366
 ADD.W   D1,OFF16(A6)               ;D36E 7FFE
 ADD.W   D1,OFF8(A6,D5)             ;D376 507E
 ADD.W   D1,DIR16                ;D378 7FFE
 ADD.W   D1,DIR24                   ;D379 007F FFFE
 ADD.W   D1,*+D1002(PC)             ;*OPRD ERROR
 ADD.W   D1,*+D80(PC,D5)            ;*OPRD ERROR
 ADD.W   D1,#IM8                    ;*OPRD ERROR
 
 
 ADD.L   D5,D1                      ;D285
 ADD.L   A6,D1                      ;D28E
 ADD.L   (A6),D1                    ;D296
 ADD.L   (A6)+,D1                   ;D29E
 ADD.L   -(A6),D1                   ;D2A6
 ADD.L   OFF16(A6),D1               ;D2AE 7FFE
 ADD.L   OFF8(A6,D5),D1             ;D2B6 507E
 ADD.L   DIR16,D1                ;D2B8 7FFE
 ADD.L   DIR24,D1                   ;D2B9 007F FFFE
 ADD.L   *+D1002(PC),D1             ;D2BA 1000
 ADD.L   *+D80(PC,D5),D1            ;D2BB 507E
 ADD.L   #IM8,D1                    ;0681 (D2BC) 0000 007E
 
 ADD.L   D1,D5                      ;DA81
 ADD.L   D1,A6                      ;DDC1
 ADD.L   D1,(A6)                    ;D396
 ADD.L   D1,(A6)+                   ;D39E
 ADD.L   D1,-(A6)                   ;D3A6
 ADD.L   D1,OFF16(A6)               ;D3AE 7FFE
 ADD.L   D1,OFF8(A6,D5)             ;D3B6 507E
 ADD.L   D1,DIR16                ;D3B8 7FFE
 ADD.L   D1,DIR24                   ;D3B9 007F FFFE
 ADD.L   D1,*+D1002(PC)             ;*OPRD ERROR
 ADD.L   D1,*+D80(PC,D5)            ;*OPRD ERROR
 ADD.L   D1,#IM8                    ;*OPRD ERROR
 
 
 ADDA    D5,A3                      ;D6C5
 ADDA    A6,A3                      ;D6CE
 ADDA    (A6),A3                    ;D6D6
 ADDA    (A6)+,A3                   ;D6DE
 ADDA    -(A6),A3                   ;D6E6
 ADDA    OFF16(A6),A3               ;D6EE 7FFE
 ADDA    OFF8(A6,D5),A3             ;D6F6 507E
 ADDA    DIR16,A3                ;D6F8 7FFE
 ADDA    DIR24,A3                   ;D6F9 007F FFFE
 ADDA    *+D1002(PC),A3             ;D6FA 1000
 ADDA    *+D80(PC,D5),A3            ;D6FB 507E
 ADDA    #IM8,A3                    ;D6FC 007E
 
 ADDA.W  D5,A3                      ;D6C5
 ADDA.W  A6,A3                      ;D6CE
 ADDA.W  (A6),A3                    ;D6D6
 ADDA.W  (A6)+,A3                   ;D6DE
 ADDA.W  -(A6),A3                   ;D6E6
 ADDA.W  OFF16(A6),A3               ;D6EE 7FFE
 ADDA.W  OFF8(A6,D5),A3             ;D6F6 507E
 ADDA.W  DIR16,A3                ;D6F8 7FFE
 ADDA.W  DIR24,A3                   ;D6F9 007F FFFE
 ADDA.W  *+D1002(PC),A3             ;D6FA 1000
 ADDA.W  *+D80(PC,D5),A3            ;D6FB 507E
 ADDA.W  #IM8,A3                    ;D6FC 007E
 
 ADDA.L  D5,A3                      ;D7C5
 ADDA.L  A6,A3                      ;D7CE
 ADDA.L  (A6),A3                    ;D7D6
 ADDA.L  (A6)+,A3                   ;D7DE
 ADDA.L  -(A6),A3                   ;D7E6
 ADDA.L  OFF16(A6),A3               ;D7EE 7FFE
 ADDA.L  OFF8(A6,D5),A3             ;D7F6 507E
 ADDA.L  DIR16,A3                ;D7F8 7FFE
 ADDA.L  DIR24,A3                   ;D7F9 007F FFFE
 ADDA.L  *+D1002(PC),A3             ;D7FA 1000
 ADDA.L  *+D80(PC,D5),A3            ;D7FB 507E
 ADDA.L  #IM8,A3                    ;D7FC 0000 007E
 
 ADDA.B  D5,A3                      ;*MNEM ERROR
 
 
 ADD     D5,A3                      ;D6C5
 ADD     A6,A3                      ;D6CE
 ADD     (A6),A3                    ;D6D6
 ADD     (A6)+,A3                   ;D6DE
 ADD     -(A6),A3                   ;D6E6
 ADD     OFF16(A6),A3               ;D6EE 7FFE
 ADD     OFF8(A6,D5),A3             ;D6F6 507E
 ADD     DIR16,A3                ;D6F8 7FFE
 ADD     DIR24,A3                   ;D6F9 007F FFFE
 ADD     *+D1002(PC),A3             ;D6FA 1000
 ADD     *+D80(PC,D5),A3            ;D6FB 507E
 ADD     #IM8,A3                    ;D6FC 007E
 
 ADD.W   D5,A3                      ;D6C5
 ADD.W   A6,A3                      ;D6CE
 ADD.W   (A6),A3                    ;D6D6
 ADD.W   (A6)+,A3                   ;D6DE
 ADD.W   -(A6),A3                   ;D6E6
 ADD.W   OFF16(A6),A3               ;D6EE 7FFE
 ADD.W   OFF8(A6,D5),A3             ;D6F6 507E
 ADD.W   DIR16,A3                ;D6F8 7FFE
 ADD.W   DIR24,A3                   ;D6F9 007F FFFE
 ADD.W   *+D1002(PC),A3             ;D6FA 1000
 ADD.W   *+D80(PC,D5),A3            ;D6FB 507E
 ADD.W   #IM8,A3                    ;D6FC 007E
 
 ADD.L   D5,A3                      ;D7C5
 ADD.L   A6,A3                      ;D7CE
 ADD.L   (A6),A3                    ;D7D6
 ADD.L   (A6)+,A3                   ;D7DE
 ADD.L   -(A6),A3                   ;D7E6
 ADD.L   OFF16(A6),A3               ;D7EE 7FFE
 ADD.L   OFF8(A6,D5),A3             ;D7F6 507E
 ADD.L   DIR16,A3                ;D7F8 7FFE
 ADD.L   DIR24,A3                   ;D7F9 007F FFFE
 ADD.L   *+D1002(PC),A3             ;D7FA 1000
 ADD.L   *+D80(PC,D5),A3            ;D7FB 507E
 ADD.L   #IM8,A3                    ;D7FC 0000 007E
 
 
 ADDI    #IM8,D5                    ;0645 007E
 ADDI    #IM8,A6                    ;*OPRD ERROR
 ADDI    #IM8,(A6)                  ;0656 007E
 ADDI    #IM8,(A6)+                 ;065E 007E
 ADDI    #IM8,-(A6)                 ;0666 007E
 ADDI    #IM8,OFF16(A6)             ;066E 007E 7FFE
 ADDI    #IM8,OFF8(A6,D5)           ;0676 007E 507E
 ADDI    #IM8,DIR16              ;0678 007E 7FFE
 ADDI    #IM8,DIR24                 ;0679 007E 007F FFFE
 ADDI    #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADDI    #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADDI    #IM8,#IM8                  ;*OPRD ERROR
 
 ADDI.B  #IM8,D5                    ;0605 007E
 ADDI.B  #IM8,A6                    ;*OPRD ERROR
 ADDI.B  #IM8,(A6)                  ;0616 007E
 ADDI.B  #IM8,(A6)+                 ;061E 007E
 ADDI.B  #IM8,-(A6)                 ;0626 007E
 ADDI.B  #IM8,OFF16(A6)             ;062E 007E 7FFE
 ADDI.B  #IM8,OFF8(A6,D5)           ;0636 007E 507E
 ADDI.B  #IM8,DIR16              ;0638 007E 7FFE
 ADDI.B  #IM8,DIR24                 ;0639 007E 007F FFFE
 ADDI.B  #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADDI.B  #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADDI.B  #IM8,#IM8                  ;*OPRD ERROR
 
 ADDI.W  #IM8,D5                    ;0645 007E
 ADDI.W  #IM8,A6                    ;*OPRD ERROR
 ADDI.W  #IM8,(A6)                  ;0656 007E
 ADDI.W  #IM8,(A6)+                 ;065E 007E
 ADDI.W  #IM8,-(A6)                 ;0666 007E
 ADDI.W  #IM8,OFF16(A6)             ;066E 007E 7FFE
 ADDI.W  #IM8,OFF8(A6,D5)           ;0676 007E 507E
 ADDI.W  #IM8,DIR16              ;0678 007E 7FFE
 ADDI.W  #IM8,DIR24                 ;0679 007E 007F FFFE
 ADDI.W  #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADDI.W  #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADDI.W  #IM8,#IM8                  ;*OPRD ERROR
 
 ADDI.L  #IM8,D5                    ;0685 0000 007E
 ADDI.L  #IM8,A6                    ;*OPRD ERROR
 ADDI.L  #IM8,(A6)                  ;0696 0000 007E
 ADDI.L  #IM8,(A6)+                 ;069E 0000 007E
 ADDI.L  #IM8,-(A6)                 ;06A6 0000 007E
 ADDI.L  #IM8,OFF16(A6)             ;06AE 0000 007E 7FFE
 ADDI.L  #IM8,OFF8(A6,D5)           ;06B6 0000 007E 507E
 ADDI.L  #IM8,DIR16              ;06B8 0000 007E 7FFE
 ADDI.L  #IM8,DIR24                 ;06B9 0000 007E 007F FFFE
 ADDI.L  #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADDI.L  #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADDI.L  #IM8,#IM8                  ;*OPRD ERROR
 
 
 ADD     #IM8,D5                    ;0645 (DA7C) 007E
 ADD     #IM8,A6                    ;DCFC 007E
 ADD     #IM8,(A6)                  ;0656 007E
 ADD     #IM8,(A6)+                 ;065E 007E
 ADD     #IM8,-(A6)                 ;0666 007E
 ADD     #IM8,OFF16(A6)             ;066E 007E 7FFE
 ADD     #IM8,OFF8(A6,D5)           ;0676 007E 507E
 ADD     #IM8,DIR16              ;0678 007E 7FFE
 ADD     #IM8,DIR24                 ;0679 007E 007F FFFE
 ADD     #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADD     #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADD     #IM8,#IM8                  ;*OPRD ERROR
 
 ADD.B   #IM8,D5                    ;0605 (DA3C) 007E
 ADD.B   #IM8,A6                    ;*OPRD ERROR
 ADD.B   #IM8,(A6)                  ;0616 007E
 ADD.B   #IM8,(A6)+                 ;061E 007E
 ADD.B   #IM8,-(A6)                 ;0626 007E
 ADD.B   #IM8,OFF16(A6)             ;062E 007E 7FFE
 ADD.B   #IM8,OFF8(A6,D5)           ;0636 007E 507E
 ADD.B   #IM8,DIR16              ;0638 007E 7FFE
 ADD.B   #IM8,DIR24                 ;0639 007E 007F FFFE
 ADD.B   #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADD.B   #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADD.B   #IM8,#IM8                  ;*OPRD ERROR
 
 ADD.W   #IM8,D5                    ;0645 (DA7C) 007E
 ADD.W   #IM8,A6                    ;DCFC 007E
 ADD.W   #IM8,(A6)                  ;0656 007E
 ADD.W   #IM8,(A6)+                 ;065E 007E
 ADD.W   #IM8,-(A6)                 ;0666 007E
 ADD.W   #IM8,OFF16(A6)             ;066E 007E 7FFE
 ADD.W   #IM8,OFF8(A6,D5)           ;0676 007E 507E
 ADD.W   #IM8,DIR16              ;0678 007E 7FFE
 ADD.W   #IM8,DIR24                 ;0679 007E 007F FFFE
 ADD.W   #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADD.W   #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADD.W   #IM8,#IM8                  ;*OPRD ERROR
 
 ADD.L   #IM8,D5                    ;0685 (DABC) 0000 007E
 ADD.L   #IM8,A6                    ;DDFC 0000 007E
 ADD.L   #IM8,(A6)                  ;0696 0000 007E
 ADD.L   #IM8,(A6)+                 ;069E 0000 007E
 ADD.L   #IM8,-(A6)                 ;06A6 0000 007E
 ADD.L   #IM8,OFF16(A6)             ;06AE 0000 007E 7FFE
 ADD.L   #IM8,OFF8(A6,D5)           ;06B6 0000 007E 507E
 ADD.L   #IM8,DIR16              ;06B8 0000 007E 7FFE
 ADD.L   #IM8,DIR24                 ;06B9 0000 007E 007F FFFE
 ADD.L   #IM8,*+D1002(PC)           ;*OPRD ERROR
 ADD.L   #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 ADD.L   #IM8,#IM8                  ;*OPRD ERROR
 
 
 ADDQ    #IM3,D5                    ;5E45
 ADDQ    #IM3,A6                    ;5E4E
 ADDQ    #IM3,(A6)                  ;5E56
 ADDQ    #IM3,(A6)+                 ;5E5E
 ADDQ    #IM3,-(A6)                 ;5E66
 ADDQ    #IM3,OFF16(A6)             ;5E6E 7FFE
 ADDQ    #IM3,OFF8(A6,D5)           ;5E76 507E
 ADDQ    #IM3,DIR16              ;5E78 7FFE
 ADDQ    #IM3,DIR24                 ;5E79 007F FFFE
 ADDQ    #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADDQ    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADDQ    #IM3,#IM8                  ;*OPRD ERROR
 
 ADDQ.B  #IM3,D5                    ;5E05
 ADDQ.B  #IM3,A6                    ;*? OPRD ERROR          !!!
 ADDQ.B  #IM3,(A6)                  ;5E16
 ADDQ.B  #IM3,(A6)+                 ;5E1E
 ADDQ.B  #IM3,-(A6)                 ;5E26
 ADDQ.B  #IM3,OFF16(A6)             ;5E2E 7FFE
 ADDQ.B  #IM3,OFF8(A6,D5)           ;5E36 507E
 ADDQ.B  #IM3,DIR16              ;5E38 7FFE
 ADDQ.B  #IM3,DIR24                 ;5E39 007F FFFE
 ADDQ.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADDQ.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADDQ.B  #IM3,#IM8                  ;*OPRD ERROR
 
 ADDQ.W  #IM3,D5                    ;5E45
 ADDQ.W  #IM3,A6                    ;5E4E
 ADDQ.W  #IM3,(A6)                  ;5E56
 ADDQ.W  #IM3,(A6)+                 ;5E5E
 ADDQ.W  #IM3,-(A6)                 ;5E66
 ADDQ.W  #IM3,OFF16(A6)             ;5E6E 7FFE
 ADDQ.W  #IM3,OFF8(A6,D5)           ;5E76 507E
 ADDQ.W  #IM3,DIR16              ;5E78 7FFE
 ADDQ.W  #IM3,DIR24                 ;5E79 007F FFFE
 ADDQ.W  #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADDQ.W  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADDQ.W  #IM3,#IM8                  ;*OPRD ERROR
 
 ADDQ.L  #IM3,D5                    ;5E85
 ADDQ.L  #IM3,A6                    ;5E8E
 ADDQ.L  #IM3,(A6)                  ;5E96
 ADDQ.L  #IM3,(A6)+                 ;5E9E
 ADDQ.L  #IM3,-(A6)                 ;5EA6
 ADDQ.L  #IM3,OFF16(A6)             ;5EAE 7FFE
 ADDQ.L  #IM3,OFF8(A6,D5)           ;5EB6 507E
 ADDQ.L  #IM3,DIR16              ;5EB8 7FFE
 ADDQ.L  #IM3,DIR24                 ;5EB9 007F FFFE
 ADDQ.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADDQ.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADDQ.L  #IM3,#IM8                  ;*OPRD ERROR
 
 ADDQ    #0,D5                      ;*OPRD ERROR
 ADDQ    #9,D5                      ;*OPRD ERROR
 
 
 ADD     #IM3,D5                    ;5E45
 ADD     #IM3,A6                    ;5E4E
 ADD     #IM3,(A6)                  ;5E56
 ADD     #IM3,(A6)+                 ;5E5E
 ADD     #IM3,-(A6)                 ;5E66
 ADD     #IM3,OFF16(A6)             ;5E6E 7FFE
 ADD     #IM3,OFF8(A6,D5)           ;5E76 507E
 ADD     #IM3,DIR16              ;5E78 7FFE
 ADD     #IM3,DIR24                 ;5E79 007F FFFE
 ADD     #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADD     #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADD     #IM3,#IM8                  ;*OPRD ERROR
 
 ADD.B   #IM3,D5                    ;5E05
 ADD.B   #IM3,A6                    ;*? OPRD ERROR          !!!
 ADD.B   #IM3,(A6)                  ;5E16
 ADD.B   #IM3,(A6)+                 ;5E1E
 ADD.B   #IM3,-(A6)                 ;5E26
 ADD.B   #IM3,OFF16(A6)             ;5E2E 7FFE
 ADD.B   #IM3,OFF8(A6,D5)           ;5E36 507E
 ADD.B   #IM3,DIR16              ;5E38 7FFE
 ADD.B   #IM3,DIR24                 ;5E39 007F FFFE
 ADD.B   #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADD.B   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADD.B   #IM3,#IM8                  ;*OPRD ERROR
 
 ADD.W   #IM3,D5                    ;5E45
 ADD.W   #IM3,A6                    ;5E4E
 ADD.W   #IM3,(A6)                  ;5E56
 ADD.W   #IM3,(A6)+                 ;5E5E
 ADD.W   #IM3,-(A6)                 ;5E66
 ADD.W   #IM3,OFF16(A6)             ;5E6E 7FFE
 ADD.W   #IM3,OFF8(A6,D5)           ;5E76 507E
 ADD.W   #IM3,DIR16              ;5E78 7FFE
 ADD.W   #IM3,DIR24                 ;5E79 007F FFFE
 ADD.W   #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADD.W   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADD.W   #IM3,#IM8                  ;*OPRD ERROR
 
 ADD.L   #IM3,D5                    ;5E85
 ADD.L   #IM3,A6                    ;5E8E
 ADD.L   #IM3,(A6)                  ;5E96
 ADD.L   #IM3,(A6)+                 ;5E9E
 ADD.L   #IM3,-(A6)                 ;5EA6
 ADD.L   #IM3,OFF16(A6)             ;5EAE 7FFE
 ADD.L   #IM3,OFF8(A6,D5)           ;5EB6 507E
 ADD.L   #IM3,DIR16              ;5EB8 7FFE
 ADD.L   #IM3,DIR24                 ;5EB9 007F FFFE
 ADD.L   #IM3,*+D1002(PC)           ;*OPRD ERROR
 ADD.L   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ADD.L   #IM3,#IM8                  ;*OPRD ERROR
 
 
 ADDX    D2,D1                      ;D342
 ADDX    -(A4),-(A3)                ;D74C
 
 ADDX    (A6),D1                    ;*OPRD ERROR
 
 ADDX.B  D2,D1                      ;D302
 ADDX.B  -(A4),-(A3)                ;D70C
 
 ADDX.W  D2,D1                      ;D342
 ADDX.W  -(A4),-(A3)                ;D74C
 
 ADDX.L  D2,D1                      ;D382
 ADDX.L  -(A4),-(A3)                ;D78C
 
 
 AND     D5,D1                      ;C245
 AND     A6,D1                      ;*OPRD ERROR
 AND     (A6),D1                    ;C256
 AND     (A6)+,D1                   ;C25E
 AND     -(A6),D1                   ;C266
 AND     OFF16(A6),D1               ;C26E 7FFE
 AND     OFF8(A6,D5),D1             ;C276 507E
 AND     DIR16,D1                ;C278 7FFE
 AND     DIR24,D1                   ;C279 007F FFFE
 AND     *+D1002(PC),D1             ;C27A 1000
 AND     *+D80(PC,D5),D1            ;C27B 507E
 AND     #IM8,D1                    ;0241 (C27C) 007E
 
 AND     D1,D5                      ;CA41
 AND     D1,A6                      ;*OPRD ERROR
 AND     D1,(A6)                    ;C356
 AND     D1,(A6)+                   ;C35E
 AND     D1,-(A6)                   ;C366
 AND     D1,OFF16(A6)               ;C36E 7FFE
 AND     D1,OFF8(A6,D5)             ;C376 507E
 AND     D1,DIR16                ;C378 7FFE
 AND     D1,DIR24                   ;C379 007F FFFE
 AND     D1,*+D1002(PC)             ;*OPRD ERROR
 AND     D1,*+D80(PC,D5)            ;*OPRD ERROR
 AND     D1,#IM8                    ;*OPRD ERROR
 
 AND.B   D5,D1                      ;C205
 AND.B   A6,D1                      ;*OPRD ERROR
 AND.B   (A6),D1                    ;C216
 AND.B   (A6)+,D1                   ;C21E
 AND.B   -(A6),D1                   ;C226
 AND.B   OFF16(A6),D1               ;C22E 7FFE
 AND.B   OFF8(A6,D5),D1             ;C236 507E
 AND.B   DIR16,D1                ;C238 7FFE
 AND.B   DIR24,D1                   ;C239 007F FFFE
 AND.B   *+D1002(PC),D1             ;C23A 1000
 AND.B   *+D80(PC,D5),D1            ;C23B 507E
 AND.B   #IM8,D1                    ;0201 (C23C) 007E
 
 AND.B   D1,D5                      ;CA01
 AND.B   D1,A6                      ;*OPRD ERROR
 AND.B   D1,(A6)                    ;C316
 AND.B   D1,(A6)+                   ;C31E
 AND.B   D1,-(A6)                   ;C326
 AND.B   D1,OFF16(A6)               ;C32E 7FFE
 AND.B   D1,OFF8(A6,D5)             ;C336 507E
 AND.B   D1,DIR16                ;C338 7FFE
 AND.B   D1,DIR24                   ;C339 007F FFFE
 AND.B   D1,*+D1002(PC)             ;*OPRD ERROR
 AND.B   D1,*+D80(PC,D5)            ;*OPRD ERROR
 AND.B   D1,#IM8                    ;*OPRD ERROR
 
 AND.W   D5,D1                      ;C245
 AND.W   A6,D1                      ;*OPRD ERROR
 AND.W   (A6),D1                    ;C256
 AND.W   (A6)+,D1                   ;C25E
 AND.W   -(A6),D1                   ;C266
 AND.W   OFF16(A6),D1               ;C26E 7FFE
 AND.W   OFF8(A6,D5),D1             ;C276 507E
 AND.W   DIR16,D1                ;C278 7FFE
 AND.W   DIR24,D1                   ;C279 007F FFFE
 AND.W   *+D1002(PC),D1             ;C27A 1000
 AND.W   *+D80(PC,D5),D1            ;C27B 507E
 AND.W   #IM8,D1                    ;0241 (C27C) 007E
 
 AND.W   D1,D5                      ;CA41
 AND.W   D1,A6                      ;*OPRD ERROR
 AND.W   D1,(A6)                    ;C356
 AND.W   D1,(A6)+                   ;C35E
 AND.W   D1,-(A6)                   ;C366
 AND.W   D1,OFF16(A6)               ;C36E 7FFE
 AND.W   D1,OFF8(A6,D5)             ;C376 507E
 AND.W   D1,DIR16                ;C378 7FFE
 AND.W   D1,DIR24                   ;C379 007F FFFE
 AND.W   D1,*+D1002(PC)             ;*OPRD ERROR
 AND.W   D1,*+D80(PC,D5)            ;*OPRD ERROR
 AND.W   D1,#IM8                    ;*OPRD ERROR
 
 
 AND.L   D5,D1                      ;C285
 AND.L   A6,D1                      ;*OPRD ERROR
 AND.L   (A6),D1                    ;C296
 AND.L   (A6)+,D1                   ;C29E
 AND.L   -(A6),D1                   ;C2A6
 AND.L   OFF16(A6),D1               ;C2AE 7FFE
 AND.L   OFF8(A6,D5),D1             ;C2B6 507E
 AND.L   DIR16,D1                ;C2B8 7FFE
 AND.L   DIR24,D1                   ;C2B9 007F FFFE
 AND.L   *+D1002(PC),D1             ;C2BA 1000
 AND.L   *+D80(PC,D5),D1            ;C2BB 507E
 AND.L   #IM8,D1                    ;0281 (C2BC) 0000 007E
 
 AND.L   D1,D5                      ;CA81
 AND.L   D1,A6                      ;*OPRD ERROR
 AND.L   D1,(A6)                    ;C396
 AND.L   D1,(A6)+                   ;C39E
 AND.L   D1,-(A6)                   ;C3A6
 AND.L   D1,OFF16(A6)               ;C3AE 7FFE
 AND.L   D1,OFF8(A6,D5)             ;C3B6 507E
 AND.L   D1,DIR16                ;C3B8 7FFE
 AND.L   D1,DIR24                   ;C3B9 007F FFFE
 AND.L   D1,*+D1002(PC)             ;*OPRD ERROR
 AND.L   D1,*+D80(PC,D5)            ;*OPRD ERROR
 AND.L   D1,#IM8                    ;*OPRD ERROR
 
 
 ANDI    #IM3,D5                    ;0245 0007
 ANDI    #IM3,A6                    ;*OPRD ERROR
 ANDI    #IM3,(A6)                  ;0256 0007
 ANDI    #IM3,(A6)+                 ;025E 0007
 ANDI    #IM3,-(A6)                 ;0266 0007
 ANDI    #IM3,OFF16(A6)             ;026E 0007 7FFE
 ANDI    #IM3,OFF8(A6,D5)           ;0276 0007 507E
 ANDI    #IM3,DIR16              ;0278 0007 7FFE
 ANDI    #IM3,DIR24                 ;0279 0007 007F FFFE
 ANDI    #IM3,*+D1002(PC)           ;*OPRD ERROR
 ANDI    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ANDI    #IM3,#IM8                  ;*OPRD ERROR
 
 ANDI.B  #IM3,D5                    ;0205 0007
 ANDI.B  #IM3,A6                    ;*OPRD ERROR
 ANDI.B  #IM3,(A6)                  ;0216 0007
 ANDI.B  #IM3,(A6)+                 ;021E 0007
 ANDI.B  #IM3,-(A6)                 ;0226 0007
 ANDI.B  #IM3,OFF16(A6)             ;022E 0007 7FFE
 ANDI.B  #IM3,OFF8(A6,D5)           ;0236 0007 507E
 ANDI.B  #IM3,DIR16              ;0238 0007 7FFE
 ANDI.B  #IM3,DIR24                 ;0239 0007 007F FFFE
 ANDI.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 ANDI.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ANDI.B  #IM3,#IM8                  ;*OPRD ERROR
 
 ANDI.W  #IM3,D5                    ;0245 0007
 ANDI.W  #IM3,A6                    ;*OPRD ERROR
 ANDI.W  #IM3,(A6)                  ;0256 0007
 ANDI.W  #IM3,(A6)+                 ;025E 0007
 ANDI.W  #IM3,-(A6)                 ;0266 0007
 ANDI.W  #IM3,OFF16(A6)             ;026E 0007 7FFE
 ANDI.W  #IM3,OFF8(A6,D5)           ;0276 0007 507E
 ANDI.W  #IM3,DIR16              ;0278 0007 7FFE
 ANDI.W  #IM3,DIR24                 ;0279 0007 007F FFFE
 ANDI.W  #IM3,*+D1002(PC)           ;*OPRD ERROR
 ANDI.W  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ANDI.W  #IM3,#IM8                  ;*OPRD ERROR
 
 ANDI.L  #IM3,D5                    ;0285 0000 0007
 ANDI.L  #IM3,A6                    ;*OPRD ERROR
 ANDI.L  #IM3,(A6)                  ;0296 0000 0007
 ANDI.L  #IM3,(A6)+                 ;029E 0000 0007
 ANDI.L  #IM3,-(A6)                 ;02A6 0000 0007
 ANDI.L  #IM3,OFF16(A6)             ;02AE 0000 0007 7FFE
 ANDI.L  #IM3,OFF8(A6,D5)           ;02B6 0000 0007 507E
 ANDI.L  #IM3,DIR16              ;02B8 0000 0007 7FFE
 ANDI.L  #IM3,DIR24                 ;02B9 0000 0007 007F FFFE
 ANDI.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 ANDI.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ANDI.L  #IM3,#IM8                  ;*OPRD ERROR
 
 
 AND     #IM3,D5                    ;0245 (CA7C) 0007
 AND     #IM3,A6                    ;*OPRD ERROR
 AND     #IM3,(A6)                  ;0256 0007
 AND     #IM3,(A6)+                 ;025E 0007
 AND     #IM3,-(A6)                 ;0266 0007
 AND     #IM3,OFF16(A6)             ;026E 0007 7FFE
 AND     #IM3,OFF8(A6,D5)           ;0276 0007 507E
 AND     #IM3,DIR16              ;0278 0007 7FFE
 AND     #IM3,DIR24                 ;0279 0007 007F FFFE
 AND     #IM3,*+D1002(PC)           ;*OPRD ERROR
 AND     #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 AND     #IM3,#IM8                  ;*OPRD ERROR
 
 AND.B   #IM3,D5                    ;0205 (CA3C) 0007
 AND.B   #IM3,A6                    ;*OPRD ERROR
 AND.B   #IM3,(A6)                  ;0216 0007
 AND.B   #IM3,(A6)+                 ;021E 0007
 AND.B   #IM3,-(A6)                 ;0226 0007
 AND.B   #IM3,OFF16(A6)             ;022E 0007 7FFE
 AND.B   #IM3,OFF8(A6,D5)           ;0236 0007 507E
 AND.B   #IM3,DIR16              ;0238 0007 7FFE
 AND.B   #IM3,DIR24                 ;0239 0007 007F FFFE
 AND.B   #IM3,*+D1002(PC)           ;*OPRD ERROR
 AND.B   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 AND.B   #IM3,#IM8                  ;*OPRD ERROR
 
 AND.W   #IM3,D5                    ;0245 (CA7C) 0007
 AND.W   #IM3,A6                    ;*OPRD ERROR
 AND.W   #IM3,(A6)                  ;0256 0007
 AND.W   #IM3,(A6)+                 ;025E 0007
 AND.W   #IM3,-(A6)                 ;0266 0007
 AND.W   #IM3,OFF16(A6)             ;026E 0007 7FFE
 AND.W   #IM3,OFF8(A6,D5)           ;0276 0007 507E
 AND.W   #IM3,DIR16              ;0278 0007 7FFE
 AND.W   #IM3,DIR24                 ;0279 0007 007F FFFE
 AND.W   #IM3,*+D1002(PC)           ;*OPRD ERROR
 AND.W   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 AND.W   #IM3,#IM8                  ;*OPRD ERROR
 
 AND.L   #IM3,D5                    ;0285 (CABC) 0000 0007
 AND.L   #IM3,A6                    ;*OPRD ERROR
 AND.L   #IM3,(A6)                  ;0296 0000 0007
 AND.L   #IM3,(A6)+                 ;029E 0000 0007
 AND.L   #IM3,-(A6)                 ;02A6 0000 0007
 AND.L   #IM3,OFF16(A6)             ;02AE 0000 0007 7FFE
 AND.L   #IM3,OFF8(A6,D5)           ;02B6 0000 0007 507E
 AND.L   #IM3,DIR16              ;02B8 0000 0007 7FFE
 AND.L   #IM3,DIR24                 ;02B9 0000 0007 007F FFFE
 AND.L   #IM3,*+D1002(PC)           ;*OPRD ERROR
 AND.L   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 AND.L   #IM3,#IM8                  ;*OPRD ERROR
 
 
 ANDI   #IM8,CCR                    ;*O 023C 007E          !!!
 
 ANDI.B #IM8,CCR                    ;*O 023C 007E          !!!
 
 AND    #IM8,CCR                    ;023C 007E
 
 AND.B  #IM8,CCR                    ;023C 007E
 
 ANDI   #IM16,CCR                   ;*OPRD ERROR
 
 ANDI.W #IM8,CCR                    ;*OPRD ERROR
 
 
 ANDI   #IM8,SR                     ;*O 027C 007E          !!!
 
 ANDI.W #IM8,SR                     ;*O 027C 007E          !!!
 
 AND    #IM8,SR                     ;027C 007E
 
 AND.W  #IM8,SR                     ;027C 007E
 
 ANDI   #IM32,SR                    ;*OPRD ERROR
 
 ANDI.B #IM8,SR                     ;*OPRD ERROR
 
 
 ASL     D1,D2                      ;E362
 
 ASL     #IM3,D2                    ;EF42
 
 ASL     #0,D2                      ;*OPRD ERROR
 ASL     #9,D2                      ;*OPRD ERROR
 
 ASL     D1,(A6)                    ;*OPRD ERROR
 ASL     #IM3,(A6)                  ;*OPRD ERROR
 
 ASL     D5                         ;*OPRD ERROR
 ASL     A6                         ;*OPRD ERROR
 ASL     (A6)                       ;E1D6
 ASL     (A6)+                      ;E1DE
 ASL     -(A6)                      ;E1E6
 ASL     OFF16(A6)                  ;E1EE 7FFE
 ASL     OFF8(A6,D5)                ;E1F6 507E
 ASL     DIR16                   ;E1F8 7FFE
 ASL     DIR24                      ;E1F9 007F FFFE
 ASL     *+D1002(PC)                ;*OPRD ERROR
 ASL     *+D80(PC,D5)               ;*OPRD ERROR
 ASL     #IM8                       ;*OPRD ERROR
 
 ASL.B   D1,D2                      ;E322
 
 ASL.B   #IM3,D2                    ;EF02
 
 ASL.B   (A6)                       ;*OPRD ERROR
 
 ASL.W   D1,D2                      ;E362
 
 ASL.W   #IM3,D2                    ;EF42
 
 ASL.W   D5                         ;*OPRD ERROR
 ASL.W   A6                         ;*OPRD ERROR
 ASL.W   (A6)                       ;E1D6
 ASL.W   (A6)+                      ;E1DE
 ASL.W   -(A6)                      ;E1E6
 ASL.W   OFF16(A6)                  ;E1EE 7FFE
 ASL.W   OFF8(A6,D5)                ;E1F6 507E
 ASL.W   DIR16                   ;E1F8 7FFE
 ASL.W   DIR24                      ;E1F9 007F FFFE
 ASL.W   *+D1002(PC)                ;*OPRD ERROR
 ASL.W   *+D80(PC,D5)               ;*OPRD ERROR
 ASL.W   #IM8                       ;*OPRD ERROR
 
 ASL.L   D1,D2                      ;E3A2
 
 ASL.L   #IM3,D2                    ;EF82
 
 ASL.L   (A6)                       ;*OPRD ERROR
 
 
 ASR     D1,D2                      ;E262
 
 ASR     #IM3,D2                    ;EE42
 
 ASR     #0,D2                      ;*OPRD ERROR
 ASR     #9,D2                      ;*OPRD ERROR
 
 ASR     D1,(A6)                    ;*OPRD ERROR
 ASR     #IM3,(A6)                  ;*OPRD ERROR
 
 ASR     D5                         ;*OPRD ERROR
 ASR     A6                         ;*OPRD ERROR
 ASR     (A6)                       ;E0D6
 ASR     (A6)+                      ;E0DE
 ASR     -(A6)                      ;E0E6
 ASR     OFF16(A6)                  ;E0EE 7FFE
 ASR     OFF8(A6,D5)                ;E0F6 507E
 ASR     DIR16                   ;E0F8 7FFE
 ASR     DIR24                      ;E0F9 007F FFFE
 ASR     *+D1002(PC)                ;*OPRD ERROR
 ASR     *+D80(PC,D5)               ;*OPRD ERROR
 ASR     #IM8                       ;*OPRD ERROR
 
 ASR.B   D1,D2                      ;E222
 
 ASR.B   #IM3,D2                    ;EE02
 
 ASR.B   (A6)                       ;*OPRD ERROR
 
 ASR.W   D1,D2                      ;E262
 
 ASR.W   #IM3,D2                    ;EE42
 
 ASR.W   D5                         ;*OPRD ERROR
 ASR.W   A6                         ;*OPRD ERROR
 ASR.W   (A6)                       ;E0D6
 ASR.W   (A6)+                      ;E0DE
 ASR.W   -(A6)                      ;E0E6
 ASR.W   OFF16(A6)                  ;E0EE 7FFE
 ASR.W   OFF8(A6,D5)                ;E0F6 507E
 ASR.W   DIR16                   ;E0F8 7FFE
 ASR.W   DIR24                      ;E0F9 007F FFFE
 ASR.W   *+D1002(PC)                ;*OPRD ERROR
 ASR.W   *+D80(PC,D5)               ;*OPRD ERROR
 ASR.W   #IM8                       ;*OPRD ERROR
 
 ASR.L   D1,D2                      ;E2A2
 
 ASR.L   #IM3,D2                    ;EE82
 
 ASR.L   (A6)                       ;*OPRD ERROR
 
 
 BRA     *+D04(PC)                  ;6002
 
 BRA     *+D04                      ;6002

 BT      *+D04                      ;6002
 BSR     *+D04                      ;6102
 BHI     *+D04                      ;6202
 BLS     *+D04                      ;6302
 BCC     *+D04                      ;6402
 BHS     *+D04                      ;6402
 BCS     *+D04                      ;6502
 BLO     *+D04                      ;6502
 BNE     *+D04                      ;6602
 BNZ     *+D04                      ;6602
 BEQ     *+D04                      ;6702
 BZE     *+D04                      ;6702
 BVC     *+D04                      ;6802
 BVS     *+D04                      ;6902
 BPL     *+D04                      ;6A02
 BMI     *+D04                      ;6B02
 BGE     *+D04                      ;6C02
 BLT     *+D04                      ;6D04
 BGT     *+D04                      ;6E02
 BLE     *+D04                      ;6F02
 
 BF      *+D04                      ;*MNEM ERROR
 
 BRA     *+D80                      ;607E
 BRA     *+D82                      ;6000 0080
 
 BRA     *-D7E                      ;6080
 BRA     *-D80                      ;6000 FF7E
 
 BRA     *+D8000                    ;6000 7FFE
 BRA     *+D8002                    ;*OPRD ERROR
 
 BRA     *-D7FFC                    ;6000 8002
 BRA     *-D7FFE                    ;6000 8000
 BRA     *-D8000                    ;*OPRD ERROR
 
 
 BRA.S   *+D80                      ;607E
 BRA.S   *+D82                      ;*OPRD ERROR
 
 BRA.S   *-D7E                      ;6080
 BRA.S   *-D80                      ;*OPRD ERROR
 
 BRA.S   *+D8000                    ;*OPRD ERROR
 BRA.S   *+D8002                    ;*OPRD ERROR
 
 BRA.L   *+D80                      ;*M 6000 007E          !!!
 BRA.L   *+D82                      ;*M 6000 0080          !!!
 
 BRA.L   *-D7E                      ;*M 6000 FF80          !!!
 BRA.L   *-D80                      ;*M 6000 FF7E          !!!
 
 BRA.L   *+D8000                    ;*M 6000 7FFE          !!!
 BRA.L   *-D7FFE                    ;*M 6000 8000          !!!
 
 BRA.L   *+D8002                    ;*M OPRD ERROR          !!!
 BRA.L   *-D8000                    ;*M OPRD ERROR          !!!
 
 
 BCHG    D1,D5                      ;0345
 BCHG    D1,A6                      ;*OPRD ERROR
 BCHG    D1,(A6)                    ;0356
 BCHG    D1,(A6)+                   ;035E
 BCHG    D1,-(A6)                   ;0366
 BCHG    D1,OFF16(A6)               ;036E 7FFE
 BCHG    D1,OFF8(A6,D5)             ;0376 507E
 BCHG    D1,DIR16                ;0378 7FFE
 BCHG    D1,DIR24                   ;0379 007F FFFE
 BCHG    D1,*+D1002(PC)             ;*OPRD ERROR
 BCHG    D1,*+D80(PC,D5)            ;*OPRD ERROR
 BCHG    D1,#IM8                    ;*OPRD ERROR
 
 BCHG    #IM3,D5                    ;0845 0007
 BCHG    #IM3,A6                    ;*OPRD ERROR
 BCHG    #IM3,(A6)                  ;0856 0007
 BCHG    #IM3,(A6)+                 ;085E 0007
 BCHG    #IM3,-(A6)                 ;0866 0007
 BCHG    #IM3,OFF16(A6)             ;086E 0007 7FFE
 BCHG    #IM3,OFF8(A6,D5)           ;0876 0007 507E
 BCHG    #IM3,DIR16              ;0878 0007 7FFE
 BCHG    #IM3,DIR24                 ;0879 0007 007F FFFE
 BCHG    #IM3,*+D1002(PC)           ;*OPRD ERROR
 BCHG    #IM3,D7E(PC,D5)             ;*OPRD ERROR
 BCHG    #IM3,#IM8                  ;*OPRD ERROR
 
 
 BCHG    #0,D5                      ;0845 0000
 BCHG    #IM5,D5                    ;0845 001F
 BCHG    #0,(A6)                    ;0856 0000
 
 BCHG    #-1,D5                     ;*? OPRD ERROR          !!!
 BCHG    #-1,(A6)                   ;*? OPRD ERROR          !!!
 BCHG    #$20,D5                    ;*? OPRD ERROR          !!!
 BCHG    #8,(A6)                    ;*? OPRD ERROR          !!!
 
 BCHG.B  D1,D5                      ;*OPRD ERROR
 BCHG.B  D1,A6                      ;*OPRD ERROR
 BCHG.B  D1,(A6)                    ;0356
 BCHG.B  D1,(A6)+                   ;035E
 BCHG.B  D1,-(A6)                   ;0366
 BCHG.B  D1,OFF16(A6)               ;036E 7FFE
 BCHG.B  D1,OFF8(A6,D5)             ;0376 507E
 BCHG.B  D1,DIR16                ;0378 7FFE
 BCHG.B  D1,DIR24                   ;0379 007F FFFE
 BCHG.B  D1,*+D1002(PC)             ;*OPRD ERROR
 BCHG.B  D1,*+D80(PC,D5)            ;*OPRD ERROR
 BCHG.B  D1,#IM8                    ;*OPRD ERROR
 
 BCHG.B  #IM3,D5                    ;*OPRD ERROR
 BCHG.B  #IM3,A6                    ;*OPRD ERROR
 BCHG.B  #IM3,(A6)                  ;0856 0007
 BCHG.B  #IM3,(A6)+                 ;085E 0007
 BCHG.B  #IM3,-(A6)                 ;0866 0007
 BCHG.B  #IM3,OFF16(A6)             ;086E 0007 7FFE
 BCHG.B  #IM3,OFF8(A6,D5)           ;0876 0007 507E
 BCHG.B  #IM3,DIR16              ;0878 0007 7FFE
 BCHG.B  #IM3,DIR24                 ;0879 0007 007F FFFE
 BCHG.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 BCHG.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BCHG.B  #IM3,#IM8                  ;*OPRD ERROR
 
 BCHG.B  A6,D5                      ;*OPRD ERROR
 BCHG.B  (A6),D5                    ;*OPRD ERROR
 
 BCHG.W  D1,D5                      ;*MNEM ERROR
 BCHG.W  #IM3,D5                    ;*MNEM ERROR
 BCHG.W  D1,(A6)                    ;*MNEM ERROR
 BCHG.W  #IM3,(A6)                  ;*MNEM ERROR
 
 BCHG.L  D1,D5                      ;0345
 BCHG.L  D1,A6                      ;*OPRD ERROR
 BCHG.L  D1,(A6)                    ;*OPRD ERROR
 BCHG.L  D1,(A6)+                   ;*OPRD ERROR
 BCHG.L  D1,-(A6)                   ;*OPRD ERROR
 BCHG.L  D1,OFF16(A6)               ;*OPRD ERROR
 BCHG.L  D1,OFF8(A6,D5)             ;*OPRD ERROR
 BCHG.L  D1,DIR16                ;*OPRD ERROR
 BCHG.L  D1,DIR24                   ;*OPRD ERROR
 BCHG.L  D1,*+D1002(PC)             ;*OPRD ERROR
 BCHG.L  D1,*+D80(PC,D5)            ;*OPRD ERROR
 BCHG.L  D1,#IM8                    ;*OPRD ERROR
 
 BCHG.L  #IM3,D5                    ;0845 0007
 BCHG.L  #IM3,A6                    ;*OPRD ERROR
 BCHG.L  #IM3,(A6)                  ;*OPRD ERROR
 BCHG.L  #IM3,(A6)+                 ;*OPRD ERROR
 BCHG.L  #IM3,-(A6)                 ;*OPRD ERROR
 BCHG.L  #IM3,OFF16(A6)             ;*OPRD ERROR
 BCHG.L  #IM3,OFF8(A6,D5)           ;*OPRD ERROR
 BCHG.L  #IM3,DIR16              ;*OPRD ERROR
 BCHG.L  #IM3,DIR24                 ;*OPRD ERROR
 BCHG.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 BCHG.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BCHG.L  #IM3,#IM8                  ;*OPRD ERROR
 
 BCHG.L  A6,(A6)                    ;*OPRD ERROR
 BCHG.L  (A6),(A6)                  ;*OPRD ERROR
 
 BCLR    D1,D5                      ;0385
 BCLR    D1,A6                      ;*OPRD ERROR
 BCLR    D1,(A6)                    ;0396
 BCLR    D1,(A6)+                   ;039E
 BCLR    D1,-(A6)                   ;03A6
 BCLR    D1,OFF16(A6)               ;03AE 7FFE
 BCLR    D1,OFF8(A6,D5)             ;03B6 507E
 BCLR    D1,DIR16                ;03B8 7FFE
 BCLR    D1,DIR24                   ;03B9 007F FFFE
 BCLR    D1,*+D1002(PC)             ;*OPRD ERROR
 BCLR    D1,*+D80(PC,D5)            ;*OPRD ERROR
 BCLR    D1,#IM8                    ;*OPRD ERROR
 
 BCLR    #IM3,D5                    ;0885 0007
 BCLR    #IM3,A6                    ;*OPRD ERROR
 BCLR    #IM3,(A6)                  ;0896 0007
 BCLR    #IM3,(A6)+                 ;089E 0007
 BCLR    #IM3,-(A6)                 ;08A6 0007
 BCLR    #IM3,OFF16(A6)             ;08AE 0007 7FFE
 BCLR    #IM3,OFF8(A6,D5)           ;08B6 0007 507E
 BCLR    #IM3,DIR16              ;08B8 0007 7FFE
 BCLR    #IM3,DIR24                 ;08B9 0007 007F FFFE
 BCLR    #IM3,*+D1002(PC)           ;*OPRD ERROR
 BCLR    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BCLR    #IM3,#IM8                  ;*OPRD ERROR
 
 
 BCLR    #0,D5                      ;0885 0000
 BCLR    #IM5,D5                    ;0885 001F
 BCLR    #0,(A6)                    ;0896 0000
 
 BCLR    #-1,D5                     ;*? OPRD ERROR          !!!
 BCLR    #-1,(A6)                   ;*? OPRD ERROR          !!!
 BCLR    #$20,D5                    ;*? OPRD ERROR          !!!
 BCLR    #8,(A6)                    ;*? OPRD ERROR          !!!
 
 BCLR.B  D1,D5                      ;*OPRD ERROR
 BCLR.B  D1,A6                      ;*OPRD ERROR
 BCLR.B  D1,(A6)                    ;0396
 BCLR.B  D1,(A6)+                   ;039E
 BCLR.B  D1,-(A6)                   ;03A6
 BCLR.B  D1,OFF16(A6)               ;03AE 7FFE
 BCLR.B  D1,OFF8(A6,D5)             ;03B6 507E
 BCLR.B  D1,DIR16                ;03B8 7FFE
 BCLR.B  D1,DIR24                   ;03B9 007F FFFE
 BCLR.B  D1,*+D1002(PC)             ;*OPRD ERROR
 BCLR.B  D1,*+D80(PC,D5)            ;*OPRD ERROR
 BCLR.B  D1,#IM8                    ;*OPRD ERROR
 
 BCLR.B  #IM3,D5                    ;*OPRD ERROR
 BCLR.B  #IM3,A6                    ;*OPRD ERROR
 BCLR.B  #IM3,(A6)                  ;0896 0007
 BCLR.B  #IM3,(A6)+                 ;089E 0007
 BCLR.B  #IM3,-(A6)                 ;08A6 0007
 BCLR.B  #IM3,OFF16(A6)             ;08AE 0007 7FFE
 BCLR.B  #IM3,OFF8(A6,D5)           ;08B6 0007 507E
 BCLR.B  #IM3,DIR16              ;08B8 0007 7FFE
 BCLR.B  #IM3,DIR24                 ;08B9 0007 007F FFFE
 BCLR.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 BCLR.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BCLR.B  #IM3,#IM8                  ;*OPRD ERROR
 
 BCLR.B  A6,D5                      ;*OPRD ERROR
 BCLR.B  (A6),D5                    ;*OPRD ERROR
 
 BCLR.W  D1,D5                      ;*MNEM ERROR
 BCLR.W  #IM3,D5                    ;*MNEM ERROR
 BCLR.W  D1,(A6)                    ;*MNEM ERROR
 BCLR.W  #IM3,(A6)                  ;*MNEM ERROR
 
 
 BCLR.L  D1,D5                      ;0385
 BCLR.L  D1,A6                      ;*OPRD ERROR
 BCLR.L  D1,(A6)                    ;*OPRD ERROR
 BCLR.L  D1,(A6)+                   ;*OPRD ERROR
 BCLR.L  D1,-(A6)                   ;*OPRD ERROR
 BCLR.L  D1,OFF16(A6)               ;*OPRD ERROR
 BCLR.L  D1,OFF8(A6,D5)             ;*OPRD ERROR
 BCLR.L  D1,DIR16                ;*OPRD ERROR
 BCLR.L  D1,DIR24                   ;*OPRD ERROR
 BCLR.L  D1,*+D1002(PC)             ;*OPRD ERROR
 BCLR.L  D1,*+D80(PC,D5)            ;*OPRD ERROR
 BCLR.L  D1,#IM8                    ;*OPRD ERROR
 
 BCLR.L  #IM3,D5                    ;0885 0007
 BCLR.L  #IM3,A6                    ;*OPRD ERROR
 BCLR.L  #IM3,(A6)                  ;*OPRD ERROR
 BCLR.L  #IM3,(A6)+                 ;*OPRD ERROR
 BCLR.L  #IM3,-(A6)                 ;*OPRD ERROR
 BCLR.L  #IM3,OFF16(A6)             ;*OPRD ERROR
 BCLR.L  #IM3,OFF8(A6,D5)           ;*OPRD ERROR
 BCLR.L  #IM3,DIR16              ;*OPRD ERROR
 BCLR.L  #IM3,DIR24                 ;*OPRD ERROR
 BCLR.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 BCLR.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BCLR.L  #IM3,#IM8                  ;*OPRD ERROR
 
 BCLR.L  A6,(A6)                    ;*OPRD ERROR
 BCLR.L  (A6),(A6)                  ;*OPRD ERROR
 
 BSET    D1,D5                      ;03C5
 BSET    D1,A6                      ;*OPRD ERROR
 BSET    D1,(A6)                    ;03D6
 BSET    D1,(A6)+                   ;03DE
 BSET    D1,-(A6)                   ;03E6
 BSET    D1,OFF16(A6)               ;03EE 7FFE
 BSET    D1,OFF8(A6,D5)             ;03F6 507E
 BSET    D1,DIR16                ;03F8 7FFE
 BSET    D1,DIR24                   ;03F9 007F FFFE
 BSET    D1,*+D1002(PC)             ;*OPRD ERROR
 BSET    D1,*+D80(PC,D5)            ;*OPRD ERROR
 BSET    D1,#IM8                    ;*OPRD ERROR
 
 BSET    #IM3,D5                    ;08C5 0007
 BSET    #IM3,A6                    ;*OPRD ERROR
 BSET    #IM3,(A6)                  ;08D6 0007
 BSET    #IM3,(A6)+                 ;08DE 0007
 BSET    #IM3,-(A6)                 ;08E6 0007
 BSET    #IM3,OFF16(A6)             ;08EE 0007 7FFE
 BSET    #IM3,OFF8(A6,D5)           ;08F6 0007 507E
 BSET    #IM3,DIR16              ;08F8 0007 7FFE
 BSET    #IM3,DIR24                 ;08F9 0007 007F FFFE
 BSET    #IM3,*+D1002(PC)           ;*OPRD ERROR
 BSET    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BSET    #IM3,#IM8                  ;*OPRD ERROR
 
 
 BSET    #0,D5                      ;08C5 0000
 BSET    #IM5,D5                    ;08C5 001F
 BSET    #0,(A6)                    ;08D6 0000
 
 BSET    #-1,D5                     ;*? OPRD ERROR          !!!
 BSET    #-1,(A6)                   ;*? OPRD ERROR          !!!
 BSET    #$20,D5                    ;*? OPRD ERROR          !!!
 BSET    #8,(A6)                    ;*? OPRD ERROR          !!!
 
 BSET.B  D1,D5                      ;*OPRD ERROR
 BSET.B  D1,A6                      ;*OPRD ERROR
 BSET.B  D1,(A6)                    ;03D6
 BSET.B  D1,(A6)+                   ;03DE
 BSET.B  D1,-(A6)                   ;03E6
 BSET.B  D1,OFF16(A6)               ;03EE 7FFE
 BSET.B  D1,OFF8(A6,D5)             ;03F6 507E
 BSET.B  D1,DIR16                ;03F8 7FFE
 BSET.B  D1,DIR24                   ;03F9 007F FFFE
 BSET.B  D1,*+D1002(PC)             ;*OPRD ERROR
 BSET.B  D1,*+D80(PC,D5)            ;*OPRD ERROR
 BSET.B  D1,#IM8                    ;*OPRD ERROR
 
 BSET.B  #IM3,D5                    ;*OPRD ERROR
 BSET.B  #IM3,A6                    ;*OPRD ERROR
 BSET.B  #IM3,(A6)                  ;08D6 0007
 BSET.B  #IM3,(A6)+                 ;08DE 0007
 BSET.B  #IM3,-(A6)                 ;08E6 0007
 BSET.B  #IM3,OFF16(A6)             ;08EE 0007 7FFE
 BSET.B  #IM3,OFF8(A6,D5)           ;08F6 0007 507E
 BSET.B  #IM3,DIR16              ;08F8 0007 7FFE
 BSET.B  #IM3,DIR24                 ;08F9 0007 007F FFFE
 BSET.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 BSET.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BSET.B  #IM3,#IM8                  ;*OPRD ERROR
 
 BSET.B  A6,D5                      ;*OPRD ERROR
 BSET.B  (A6),D5                    ;*OPRD ERROR
 
 BSET.W  D1,D5                      ;*MNEM ERROR
 BSET.W  #IM3,D5                    ;*MNEM ERROR
 BSET.W  D1,(A6)                    ;*MNEM ERROR
 BSET.W  #IM3,(A6)                  ;*MNEM ERROR
 
 
 BSET.L  D1,D5                      ;03C5
 BSET.L  D1,A6                      ;*OPRD ERROR
 BSET.L  D1,(A6)                    ;*OPRD ERROR
 BSET.L  D1,(A6)+                   ;*OPRD ERROR
 BSET.L  D1,-(A6)                   ;*OPRD ERROR
 BSET.L  D1,OFF16(A6)               ;*OPRD ERROR
 BSET.L  D1,OFF8(A6,D5)             ;*OPRD ERROR
 BSET.L  D1,DIR16                ;*OPRD ERROR
 BSET.L  D1,DIR24                   ;*OPRD ERROR
 BSET.L  D1,*+D1002(PC)             ;*OPRD ERROR
 BSET.L  D1,*+D80(PC,D5)            ;*OPRD ERROR
 BSET.L  D1,#IM8                    ;*OPRD ERROR
 
 BSET.L  #IM3,D5                    ;08C5 0007
 BSET.L  #IM3,A6                    ;*OPRD ERROR
 BSET.L  #IM3,(A6)                  ;*OPRD ERROR
 BSET.L  #IM3,(A6)+                 ;*OPRD ERROR
 BSET.L  #IM3,-(A6)                 ;*OPRD ERROR
 BSET.L  #IM3,OFF16(A6)             ;*OPRD ERROR
 BSET.L  #IM3,OFF8(A6,D5)           ;*OPRD ERROR
 BSET.L  #IM3,DIR16              ;*OPRD ERROR
 BSET.L  #IM3,DIR24                 ;*OPRD ERROR
 BSET.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 BSET.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BSET.L  #IM3,#IM8                  ;*OPRD ERROR
 
 BSET.L  A6,(A6)                    ;*OPRD ERROR
 BSET.L  (A6),(A6)                  ;*OPRD ERROR
 
 
 
 BTST    D1,D5                      ;0305
 BTST    D1,A6                      ;*OPRD ERROR
 BTST    D1,(A6)                    ;0316
 BTST    D1,(A6)+                   ;031E
 BTST    D1,-(A6)                   ;0326
 BTST    D1,OFF16(A6)               ;032E 7FFE
 BTST    D1,OFF8(A6,D5)             ;0336 507E
 BTST    D1,DIR16                ;0338 7FFE
 BTST    D1,DIR24                   ;0339 007F FFFE
 BTST    D1,*+D1002(PC)             ;033A 1000
 BTST    D1,*+D80(PC,D5)            ;033B 507E
 BTST    D1,#IM8                    ;*OPRD ERROR
 
 BTST    #IM3,D5                    ;0805 0007
 BTST    #IM3,A6                    ;*OPRD ERROR
 BTST    #IM3,(A6)                  ;0816 0007
 BTST    #IM3,(A6)+                 ;081E 0007
 BTST    #IM3,-(A6)                 ;0826 0007
 BTST    #IM3,OFF16(A6)             ;082E 0007 7FFE
 BTST    #IM3,OFF8(A6,D5)           ;0836 0007 507E
 BTST    #IM3,DIR16              ;0838 0007 7FFE
 BTST    #IM3,DIR24                 ;0839 0007 007F FFFE
 BTST    #IM3,*+D1002(PC)           ;083A 0007 0FFE
 BTST    #IM3,*+D80(PC,D5)          ;083B 0007 507C
 BTST    #IM3,#IM8                  ;*OPRD ERROR
 
 BTST.B  D1,D5                      ;*OPRD ERROR
 
 BTST    #0,D5                      ;0805 0000
 BTST    #IM5,D5                    ;0805 001F
 BTST    #0,(A6)                    ;0816 0000
 
 BTST    #-1,D5                     ;*? OPRD ERROR          !!!
 BTST    #-1,(A6)                   ;*? OPRD ERROR          !!!
 BTST    #$20,D5                    ;*? OPRD ERROR          !!!
 BTST    #8,(A6)                    ;*? OPRD ERROR          !!!
 
 BTST.B  D1,A6                      ;*OPRD ERROR
 BTST.B  D1,(A6)                    ;0316
 BTST.B  D1,(A6)+                   ;031E
 BTST.B  D1,-(A6)                   ;0326
 BTST.B  D1,OFF16(A6)               ;032E 7FFE
 BTST.B  D1,OFF8(A6,D5)             ;0336 507E
 BTST.B  D1,DIR16                ;0338 7FFE
 BTST.B  D1,DIR24                   ;0339 007F FFFE
 BTST.B  D1,*+D1002(PC)             ;033A 1000
 BTST.B  D1,*+D80(PC,D5)            ;033B 507E
 BTST.B  D1,#IM8                    ;*OPRD ERROR
 
 BTST.B  #IM3,D5                    ;*OPRD ERROR
 BTST.B  #IM3,A6                    ;*OPRD ERROR
 BTST.B  #IM3,(A6)                  ;0816 0007
 BTST.B  #IM3,(A6)+                 ;081E 0007
 BTST.B  #IM3,-(A6)                 ;0826 0007
 BTST.B  #IM3,OFF16(A6)             ;082E 0007 7FFE
 BTST.B  #IM3,OFF8(A6,D5)           ;0836 0007 507E
 BTST.B  #IM3,DIR16              ;0838 0007 7FFE
 BTST.B  #IM3,DIR24                 ;0839 0007 007F FFFE
 BTST.B  #IM3,*+D1002(PC)           ;083A 0007 0FFE
 BTST.B  #IM3,*+D80(PC,D5)          ;083B 0007 507C
 BTST.B  #IM3,#IM8                  ;*OPRD ERROR
 
 BTST.B  A6,D5                      ;*OPRD ERROR
 BTST.B  (A6),D5                    ;*OPRD ERROR
 
 BTST.W  D1,D5                      ;*MNEM ERROR
 BTST.W  #IM3,D5                    ;*MNEM ERROR
 BTST.W  D1,(A6)                    ;*MNEM ERROR
 BTST.W  #IM3,(A6)                  ;*MNEM ERROR
 
 
 BTST.L  D1,D5                      ;0305
 BTST.L  D1,A6                      ;*OPRD ERROR
 BTST.L  D1,(A6)                    ;*OPRD ERROR
 BTST.L  D1,(A6)+                   ;*OPRD ERROR
 BTST.L  D1,-(A6)                   ;*OPRD ERROR
 BTST.L  D1,OFF16(A6)               ;*OPRD ERROR
 BTST.L  D1,OFF8(A6,D5)             ;*OPRD ERROR
 BTST.L  D1,DIR16                ;*OPRD ERROR
 BTST.L  D1,DIR24                   ;*OPRD ERROR
 BTST.L  D1,*+D1002(PC)             ;*OPRD ERROR
 BTST.L  D1,*+D80(PC,D5)            ;*OPRD ERROR
 BTST.L  D1,#IM8                    ;*OPRD ERROR
 
 BTST.L  #IM3,D5                    ;0805 0007
 BTST.L  #IM3,A6                    ;*OPRD ERROR
 BTST.L  #IM3,(A6)                  ;*OPRD ERROR
 BTST.L  #IM3,(A6)+                 ;*OPRD ERROR
 BTST.L  #IM3,-(A6)                 ;*OPRD ERROR
 BTST.L  #IM3,OFF16(A6)             ;*OPRD ERROR
 BTST.L  #IM3,OFF8(A6,D5)           ;*OPRD ERROR
 BTST.L  #IM3,DIR16              ;*OPRD ERROR
 BTST.L  #IM3,DIR24                 ;*OPRD ERROR
 BTST.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 BTST.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 BTST.L  #IM3,#IM8                  ;*OPRD ERROR
 
 BTST.L  A6,(A6)                    ;*OPRD ERROR
 BTST.L  (A6),(A6)                  ;*OPRD ERROR
 
 
 CHK     D5,D1                      ;4385
 CHK     A6,D1                      ;*OPRD ERROR
 CHK     (A6),D1                    ;4396
 CHK     (A6)+,D1                   ;439E
 CHK     -(A6),D1                   ;43A6
 CHK     OFF16(A6),D1               ;43AE 7FFE
 CHK     OFF8(A6,D5),D1             ;43B6 507E
 CHK     DIR16,D1                ;43B8 7FFE
 CHK     DIR24,D1                   ;43B9 007F FFFE
 CHK     *+D1002(PC),D1             ;43BA 1000
 CHK     *+D80(PC,D5),D1            ;43BB 507E
 CHK     #IM8,D1                    ;43BC 007E
 
 CHK     D5,(A6)                    ;*OPRD ERROR
 
 CHK.W   D5,D1                      ;4385
 CHK.W   A6,D1                      ;*OPRD ERROR
 CHK.W   (A6),D1                    ;4396
 CHK.W   (A6)+,D1                   ;439E
 CHK.W   -(A6),D1                   ;43A6
 CHK.W   OFF16(A6),D1               ;43AE 7FFE
 CHK.W   OFF8(A6,D5),D1             ;43B6 507E
 CHK.W   DIR16,D1                ;43B8 7FFE
 CHK.W   DIR24,D1                   ;43B9 007F FFFE
 CHK.W   *+D1002(PC),D1             ;43BA 1000
 CHK.W   *+D80(PC,D5),D1            ;43BB 507E
 CHK.W   #IM8,D1                    ;43BC 007E
 
 
 CLR     D5                         ;4245
 CLR     A6                         ;*OPRD ERROR
 CLR     (A6)                       ;4256
 CLR     (A6)+                      ;425E
 CLR     -(A6)                      ;4266
 CLR     OFF16(A6)                  ;426E 7FFE
 CLR     OFF8(A6,D5)                ;4276 507E
 CLR     DIR16                   ;4278 7FFE
 CLR     DIR24                      ;4279 007F FFFE
 CLR     *+D1002(PC)                ;*OPRD ERROR
 CLR     *+D80(PC,D5)               ;*OPRD ERROR
 CLR     #IM8                       ;*OPRD ERROR
 
 CLR.B   D5                         ;4205
 CLR.B   A6                         ;*OPRD ERROR
 CLR.B   (A6)                       ;4216
 CLR.B   (A6)+                      ;421E
 CLR.B   -(A6)                      ;4226
 CLR.B   OFF16(A6)                  ;422E 7FFE
 CLR.B   OFF8(A6,D5)                ;4236 507E
 CLR.B   DIR16                   ;4238 7FFE
 CLR.B   DIR24                      ;4239 007F FFFE
 CLR.B   *+D1002(PC)                ;*OPRD ERROR
 CLR.B   *+D80(PC,D5)               ;*OPRD ERROR
 CLR.B   #IM8                       ;*OPRD ERROR
 
 CLR.W   D5                         ;4245
 CLR.W   A6                         ;*OPRD ERROR
 CLR.W   (A6)                       ;4256
 CLR.W   (A6)+                      ;425E
 CLR.W   -(A6)                      ;4266
 CLR.W   OFF16(A6)                  ;426E 7FFE
 CLR.W   OFF8(A6,D5)                ;4276 507E
 CLR.W   DIR16                   ;4278 7FFE
 CLR.W   DIR24                      ;4279 007F FFFE
 CLR.W   *+D1002(PC)                ;*OPRD ERROR
 CLR.W   *+D80(PC,D5)               ;*OPRD ERROR
 CLR.W   #IM8                       ;*OPRD ERROR
 
 CLR.L   D5                         ;4285
 CLR.L   A6                         ;*OPRD ERROR
 CLR.L   (A6)                       ;4296
 CLR.L   (A6)+                      ;429E
 CLR.L   -(A6)                      ;42A6
 CLR.L   OFF16(A6)                  ;42AE 7FFE
 CLR.L   OFF8(A6,D5)                ;42B6 507E
 CLR.L   DIR16                   ;42B8 7FFE
 CLR.L   DIR24                      ;42B9 007F FFFE
 CLR.L   *+D1002(PC)                ;*OPRD ERROR
 CLR.L   *+D80(PC,D5)               ;*OPRD ERROR
 CLR.L   #IM8                       ;*OPRD ERROR
 
 
 CMP     D5,D1                      ;B245
 CMP     A6,D1                      ;B24E
 CMP     (A6),D1                    ;B256
 CMP     (A6)+,D1                   ;B25E
 CMP     -(A6),D1                   ;B266
 CMP     OFF16(A6),D1               ;B26E 7FFE
 CMP     OFF8(A6,D5),D1             ;B276 507E
 CMP     DIR16,D1                ;B278 7FFE
 CMP     DIR24,D1                   ;B279 007F FFFE
 CMP     *+D1002(PC),D1             ;B27A 1000
 CMP     *+D80(PC,D5),D1            ;B27B 507E
 CMP     #IM8,D1                    ;0C41 (B27C) 007E
 
 CMP     D5,(A6)                    ;*OPRD ERROR
 
 CMP.B   D5,D1                      ;B205
 CMP.B   A6,D1                      ;*OPRD ERROR
 CMP.B   (A6),D1                    ;B216
 CMP.B   (A6)+,D1                   ;B21E
 CMP.B   -(A6),D1                   ;B226
 CMP.B   OFF16(A6),D1               ;B22E 7FFE
 CMP.B   OFF8(A6,D5),D1             ;B236 507E
 CMP.B   DIR16,D1                ;B238 7FFE
 CMP.B   DIR24,D1                   ;B239 007F FFFE
 CMP.B   *+D1002(PC),D1             ;B23A 1000
 CMP.B   *+D80(PC,D5),D1            ;B23B 507E
 CMP.B   #IM8,D1                    ;0C01 (B23C) 007E
 
 CMP.W   D5,D1                      ;B245
 CMP.W   A6,D1                      ;B24E
 CMP.W   (A6),D1                    ;B256
 CMP.W   (A6)+,D1                   ;B25E
 CMP.W   -(A6),D1                   ;B266
 CMP.W   OFF16(A6),D1               ;B26E 7FFE
 CMP.W   OFF8(A6,D5),D1             ;B276 507E
 CMP.W   DIR16,D1                ;B278 7FFE
 CMP.W   DIR24,D1                   ;B279 007F FFFE
 CMP.W   *+D1002(PC),D1             ;B27A 1000
 CMP.W   *+D80(PC,D5),D1            ;B27B 507E
 CMP.W   #IM8,D1                    ;0C41 (B27C) 007E
 
 CMP.L   D5,D1                      ;B285
 CMP.L   A6,D1                      ;B28E
 CMP.L   (A6),D1                    ;B296
 CMP.L   (A6)+,D1                   ;B29E
 CMP.L   -(A6),D1                   ;B2A6
 CMP.L   OFF16(A6),D1               ;B2AE 7FFE
 CMP.L   OFF8(A6,D5),D1             ;B2B6 507E
 CMP.L   DIR16,D1                ;B2B8 7FFE
 CMP.L   DIR24,D1                   ;B2B9 007F FFFE
 CMP.L   *+D1002(PC),D1             ;B2BA 1000
 CMP.L   *+D80(PC,D5),D1            ;B2BB 507E
 CMP.L   #IM8,D1                    ;0C81 (B2BC) 0000 007E
 
 
 CMPA    D5,A3                      ;B6C5
 CMPA    A6,A3                      ;B6CE
 CMPA    (A6),A3                    ;B6D6
 CMPA    (A6)+,A3                   ;B6DE
 CMPA    -(A6),A3                   ;B6E6
 CMPA    OFF16(A6),A3               ;B6EE 7FFE
 CMPA    OFF8(A6,D5),A3             ;B6F6 507E
 CMPA    DIR16,A3                ;B6F8 7FFE
 CMPA    DIR24,A3                   ;B6F9 007F FFFE
 CMPA    *+D1002(PC),A3             ;B6FA 1000
 CMPA    *+D80(PC,D5),A3            ;B6FB 507E
 CMPA    #IM8,A3                    ;B6FC 007E
 
 CMPA.B  D5,A3                      ;*MNEM ERROR
 
 CMPA.W  D5,A3                      ;B6C5
 CMPA.W  A6,A3                      ;B6CE
 CMPA.W  (A6),A3                    ;B6D6
 CMPA.W  (A6)+,A3                   ;B6DE
 CMPA.W  -(A6),A3                   ;B6E6
 CMPA.W  OFF16(A6),A3               ;B6EE 7FFE
 CMPA.W  OFF8(A6,D5),A3             ;B6F6 507E
 CMPA.W  DIR16,A3                ;B6F8 7FFE
 CMPA.W  DIR24,A3                   ;B6F9 007F FFFE
 CMPA.W  *+D1002(PC),A3             ;B6FA 1000
 CMPA.W  *+D80(PC,D5),A3            ;B6FB 507E
 CMPA.W  #IM8,A3                    ;B6FC 007E
 
 CMPA.L  D5,A3                      ;B7C5
 CMPA.L  A6,A3                      ;B7CE
 CMPA.L  (A6),A3                    ;B7D6
 CMPA.L  (A6)+,A3                   ;B7DE
 CMPA.L  -(A6),A3                   ;B7E6
 CMPA.L  OFF16(A6),A3               ;B7EE 7FFE
 CMPA.L  OFF8(A6,D5),A3             ;B7F6 507E
 CMPA.L  DIR16,A3                ;B7F8 7FFE
 CMPA.L  DIR24,A3                   ;B7F9 007F FFFE
 CMPA.L  *+D1002(PC),A3             ;B7FA 1000
 CMPA.L  *+D80(PC,D5),A3            ;B7FB 507E
 CMPA.L  #IM8,A3                    ;B7FC 0000 007E
 
 
 CMP     D5,A3                      ;B6C5
 CMP     A6,A3                      ;B6CE
 CMP     (A6),A3                    ;B6D6
 CMP     (A6)+,A3                   ;B6DE
 CMP     -(A6),A3                   ;B6E6
 CMP     OFF16(A6),A3               ;B6EE 7FFE
 CMP     OFF8(A6,D5),A3             ;B6F6 507E
 CMP     DIR16,A3                ;B6F8 7FFE
 CMP     DIR24,A3                   ;B6F9 007F FFFE
 CMP     *+D1002(PC),A3             ;B6FA 1000
 CMP     *+D80(PC,D5),A3            ;B6FB 507E
 CMP     #IM8,A3                    ;B6FC 007E
 
 CMP.B   D5,A3                      ;*OPRD ERROR
 
 CMP.W   D5,A3                      ;B6C5
 CMP.W   A6,A3                      ;B6CE
 CMP.W   (A6),A3                    ;B6D6
 CMP.W   (A6)+,A3                   ;B6DE
 CMP.W   -(A6),A3                   ;B6E6
 CMP.W   OFF16(A6),A3               ;B6EE 7FFE
 CMP.W   OFF8(A6,D5),A3             ;B6F6 507E
 CMP.W   DIR16,A3                ;B6F8 7FFE
 CMP.W   DIR24,A3                   ;B6F9 007F FFFE
 CMP.W   *+D1002(PC),A3             ;B6FA 1000
 CMP.W   *+D80(PC,D5),A3            ;B6FB 507E
 CMP.W   #IM8,A3                    ;B6FC 007E
 
 CMP.L   D5,A3                      ;B7C5
 CMP.L   A6,A3                      ;B7CE
 CMP.L   (A6),A3                    ;B7D6
 CMP.L   (A6)+,A3                   ;B7DE
 CMP.L   -(A6),A3                   ;B7E6
 CMP.L   OFF16(A6),A3               ;B7EE 7FFE
 CMP.L   OFF8(A6,D5),A3             ;B7F6 507E
 CMP.L   DIR16,A3                ;B7F8 7FFE
 CMP.L   DIR24,A3                   ;B7F9 007F FFFE
 CMP.L   *+D1002(PC),A3             ;B7FA 1000
 CMP.L   *+D80(PC,D5),A3            ;B7FB 507E
 CMP.L   #IM8,A3                    ;B7FC 0000 007E
 
 
 CMPI    #IM3,D5                    ;0C45 0007
 CMPI    #IM3,A6                    ;*OPRD ERROR
 CMPI    #IM3,(A6)                  ;0C56 0007
 CMPI    #IM3,(A6)+                 ;0C5E 0007
 CMPI    #IM3,-(A6)                 ;0C66 0007
 CMPI    #IM3,OFF16(A6)             ;0C6E 0007 7FFE
 CMPI    #IM3,OFF8(A6,D5)           ;0C76 0007 507E
 CMPI    #IM3,DIR16              ;0C78 0007 7FFE
 CMPI    #IM3,DIR24                 ;0C79 0007 007F FFFE
 CMPI    #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMPI    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMPI    #IM3,#IM8                  ;*OPRD ERROR
 
 CMPI.B  #IM3,D5                    ;0C05 0007
 CMPI.B  #IM3,A6                    ;*OPRD ERROR
 CMPI.B  #IM3,(A6)                  ;0C16 0007
 CMPI.B  #IM3,(A6)+                 ;0C1E 0007
 CMPI.B  #IM3,-(A6)                 ;0C26 0007
 CMPI.B  #IM3,OFF16(A6)             ;0C2E 0007 7FFE
 CMPI.B  #IM3,OFF8(A6,D5)           ;0C36 0007 507E
 CMPI.B  #IM3,DIR16              ;0C38 0007 7FFE
 CMPI.B  #IM3,DIR24                 ;0C39 0007 007F FFFE
 CMPI.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMPI.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMPI.B  #IM3,#IM8                  ;*OPRD ERROR
 
 CMPI.W  #IM3,D5                    ;0C45 0007
 CMPI.W  #IM3,A6                    ;*OPRD ERROR
 CMPI.W  #IM3,(A6)                  ;0C56 0007
 CMPI.W  #IM3,(A6)+                 ;0C5E 0007
 CMPI.W  #IM3,-(A6)                 ;0C66 0007
 CMPI.W  #IM3,OFF16(A6)             ;0C6E 0007 7FFE
 CMPI.W  #IM3,OFF8(A6,D5)           ;0C76 0007 507E
 CMPI.W  #IM3,DIR16              ;0C78 0007 7FFE
 CMPI.W  #IM3,DIR24                 ;0C79 0007 007F FFFE
 CMPI.W  #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMPI.W  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMPI.W  #IM3,#IM8                  ;*OPRD ERROR
 
 CMPI.L  #IM3,D5                    ;0C85 0000 0007
 CMPI.L  #IM3,A6                    ;*OPRD ERROR
 CMPI.L  #IM3,(A6)                  ;0C96 0000 0007
 CMPI.L  #IM3,(A6)+                 ;0C9E 0000 0007
 CMPI.L  #IM3,-(A6)                 ;0CA6 0000 0007
 CMPI.L  #IM3,OFF16(A6)             ;0CAE 0000 0007 7FFE
 CMPI.L  #IM3,OFF8(A6,D5)           ;0CB6 0000 0007 507E
 CMPI.L  #IM3,DIR16              ;0CB8 0000 0007 7FFE
 CMPI.L  #IM3,DIR24                 ;0CB9 0000 0007 007F FFFE
 CMPI.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMPI.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMPI.L  #IM3,#IM8                  ;*OPRD ERROR
 
 
 CMP     #IM3,D5                    ;0C45 (BA7C) 0007
 CMP     #IM3,A6                    ;BCFC 0007
 CMP     #IM3,(A6)                  ;0C56 0007
 CMP     #IM3,(A6)+                 ;0C5E 0007
 CMP     #IM3,-(A6)                 ;0C66 0007
 CMP     #IM3,OFF16(A6)             ;0C6E 0007 7FFE
 CMP     #IM3,OFF8(A6,D5)           ;0C76 0007 507E
 CMP     #IM3,DIR16              ;0C78 0007 7FFE
 CMP     #IM3,DIR24                 ;0C79 0007 007F FFFE
 CMP     #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMP     #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMP     #IM3,#IM8                  ;*OPRD ERROR
 
 CMP.B   #IM3,D5                    ;0C05 (BA3C) 0007
 CMP.B   #IM3,A6                    ;*OPRD ERROR
 CMP.B   #IM3,(A6)                  ;0C16 0007
 CMP.B   #IM3,(A6)+                 ;0C1E 0007
 CMP.B   #IM3,-(A6)                 ;0C26 0007
 CMP.B   #IM3,OFF16(A6)             ;0C2E 0007 7FFE
 CMP.B   #IM3,OFF8(A6,D5)           ;0C36 0007 507E
 CMP.B   #IM3,DIR16              ;0C38 0007 7FFE
 CMP.B   #IM3,DIR24                 ;0C39 0007 007F FFFE
 CMP.B   #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMP.B   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMP.B   #IM3,#IM8                  ;*OPRD ERROR
 
 CMP.W   #IM3,D5                    ;0C45 (BA7C) 0007
 CMP.W   #IM3,A6                    ;BCFC 0007
 CMP.W   #IM3,(A6)                  ;0C56 0007
 CMP.W   #IM3,(A6)+                 ;0C5E 0007
 CMP.W   #IM3,-(A6)                 ;0C66 0007
 CMP.W   #IM3,OFF16(A6)             ;0C6E 0007 7FFE
 CMP.W   #IM3,OFF8(A6,D5)           ;0C76 0007 507E
 CMP.W   #IM3,DIR16              ;0C78 0007 7FFE
 CMP.W   #IM3,DIR24                 ;0C79 0007 007F FFFE
 CMP.W   #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMP.W   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMP.W   #IM3,#IM8                  ;*OPRD ERROR
 
 CMP.L   #IM3,D5                    ;0C85 (BABC) 0000 0007
 CMP.L   #IM3,A6                    ;BDFC 0000 0007
 CMP.L   #IM3,(A6)                  ;0C96 0000 0007
 CMP.L   #IM3,(A6)+                 ;0C9E 0000 0007
 CMP.L   #IM3,-(A6)                 ;0CA6 0000 0007
 CMP.L   #IM3,OFF16(A6)             ;0CAE 0000 0007 7FFE
 CMP.L   #IM3,OFF8(A6,D5)           ;0CB6 0000 0007 507E
 CMP.L   #IM3,DIR16              ;0CB8 0000 0007 7FFE
 CMP.L   #IM3,DIR24                 ;0CB9 0000 0007 007F FFFE
 CMP.L   #IM3,*+D1002(PC)           ;*OPRD ERROR
 CMP.L   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 CMP.L   #IM3,#IM8                  ;*OPRD ERROR
 
 
 CMPM    (A4)+,(A3)+                ;B74C
 
 CMPM.B  (A4)+,(A3)+                ;B70C
 
 CMPM.W  (A4)+,(A3)+                ;B74C
 
 CMPM.L  (A4)+,(A3)+                ;B78C
 
 
 CMP     (A4)+,(A3)+                ;B74C
 
 CMP.B   (A4)+,(A3)+                ;B70C
 
 CMP.W   (A4)+,(A3)+                ;B74C
 
 CMP.L   (A4)+,(A3)+                ;B78C
 
 CMPM    -(A4),-(A4)                ;*OPRD ERROR
 
 CMP     -(A4),-(A4)                ;*OPRD ERROR
 
 DBT     D1,*+D04(PC)               ;50C9 0002
 
 
 DBT     D1,*+D04                   ;50C9 0002
 DBF     D1,*+D04                   ;51C9 0002
 DBHI    D1,*+D04                   ;52C9 0002
 DBLS    D1,*+D04                   ;53C9 0002
 DBCC    D1,*+D04                   ;54C9 0002
 DBHS    D1,*+D04                   ;54C9 0002
 DBCS    D1,*+D04                   ;55C9 0002
 DBLO    D1,*+D04                   ;55C9 0002
 DBNE    D1,*+D04                   ;56C9 0002
 DBNZ    D1,*+D04                   ;56C9 0002
 DBEQ    D1,*+D04                   ;57C9 0002
 DBZE    D1,*+D04                   ;57C9 0002
 DBVC    D1,*+D04                   ;58C9 0002
 DBVC    D1,*+D04                   ;58C9 0002
 DBVS    D1,*+D04                   ;59C9 0002
 DBPL    D1,*+D04                   ;5AC9 0002
 DBMI    D1,*+D04                   ;5BC9 0002
 DBGE    D1,*+D04                   ;5CC9 0002
 DBLT    D1,*+D04                   ;5DC9 0002
 DBGT    D1,*+D04                   ;5EC9 0002
 DBLE    D1,*+D04                   ;5FC9 0002
 
 DBT     D1,*+D8000                 ;50C9 7FFE
 DBT     *+D8002                    ;*OPRD ERROR
 
 DBT     D1,*-D7FFE                 ;50C9 8000
 DBT     D1,*-D8000                ;*OPRD ERROR
 
 DBT.B   D1,*+D04                   ;*MNEM ERROR
 
 DBT.W   D1,*+D04                   ;*MNEM ERROR (MOTOROLA)          !!!
 
 DBT.L   D1,*+D04                   ;*MNEM ERROR
 
 
 DIVS    D5,D1                      ;83C5
 DIVS    A6,D1                      ;*OPRD ERROR
 DIVS    (A6),D1                    ;83D6
 DIVS    (A6)+,D1                   ;83DE
 DIVS    -(A6),D1                   ;83E6
 DIVS    OFF16(A6),D1               ;83EE 7FFE
 DIVS    OFF8(A6,D5),D1             ;83F6 507E
 DIVS    DIR16,D1                ;83F8 7FFE
 DIVS    DIR24,D1                   ;83F9 007F FFFE
 DIVS    *+D1002(PC),D1             ;83FA 1000
 DIVS    *+D80(PC,D5),D1            ;83FB 507E
 DIVS    #IM8,D1                    ;83FC 007E
 
 DIVS.B  D5,D1                      ;*MNEM ERROR
 
 DIVS.W  D5,D1                      ;83C5
 DIVS.W  A6,D1                      ;*OPRD ERROR
 DIVS.W  (A6),D1                    ;83D6
 DIVS.W  (A6)+,D1                   ;83DE
 DIVS.W  -(A6),D1                   ;83E6
 DIVS.W  OFF16(A6),D1               ;83EE 7FFE
 DIVS.W  OFF8(A6,D5),D1             ;83F6 507E
 DIVS.W  DIR16,D1                ;83F8 7FFE
 DIVS.W  DIR24,D1                   ;83F9 007F FFFE
 DIVS.W  *+D1002(PC),D1             ;83FA 1000
 DIVS.W  *+D80(PC,D5),D1            ;83FB 507E
 DIVS.W  #IM8,D1                    ;83FC 007E
 
 DIVS.L  D5,D1                      ;*MNEM ERROR
 
 
 DIVU    D5,D1                      ;82C5
 DIVU    A6,D1                      ;*OPRD ERROR
 DIVU    (A6),D1                    ;82D6
 DIVU    (A6)+,D1                   ;82DE
 DIVU    -(A6),D1                   ;82E6
 DIVU    OFF16(A6),D1               ;82EE 7FFE
 DIVU    OFF8(A6,D5),D1             ;82F6 507E
 DIVU    DIR16,D1                ;82F8 7FFE
 DIVU    DIR24,D1                   ;82F9 007F FFFE
 DIVU    *+D1002(PC),D1             ;82FA 1000
 DIVU    *+D80(PC,D5),D1            ;82FB 507E
 DIVU    #IM8,D1                    ;82FC 007E
 
 DIVU.B  D5,D1                      ;*MNEM ERROR
 
 DIVU.W  D5,D1                      ;82C5
 DIVU.W  A6,D1                      ;*OPRD ERROR
 DIVU.W  (A6),D1                    ;82D6
 DIVU.W  (A6)+,D1                   ;82DE
 DIVU.W  -(A6),D1                   ;82E6
 DIVU.W  OFF16(A6),D1               ;82EE 7FFE
 DIVU.W  OFF8(A6,D5),D1             ;82F6 507E
 DIVU.W  DIR16,D1                ;82F8 7FFE
 DIVU.W  DIR24,D1                   ;82F9 007F FFFE
 DIVU.W  *+D1002(PC),D1             ;82FA 1000
 DIVU.W  *+D80(PC,D5),D1            ;82FB 507E
 DIVU.W  #IM8,D1                    ;82FC 007E
 
 DIVU.L  D5,D1                      ;*MNEM ERROR
 
 
 EOR     D1,D5                      ;B345
 EOR     D1,A6                      ;*OPRD ERROR
 EOR     D1,(A6)                    ;B356
 EOR     D1,(A6)+                   ;B35E
 EOR     D1,-(A6)                   ;B366
 EOR     D1,OFF16(A6)               ;B36E 7FFE
 EOR     D1,OFF8(A6,D5)             ;B376 507E
 EOR     D1,DIR16                ;B378 7FFE
 EOR     D1,DIR24                   ;B379 007F FFFE
 EOR     D1,*+D1002(PC)             ;*OPRD ERROR
 EOR     D1,*+D80(PC,D5)            ;*OPRD ERROR
 EOR     D1,#IM8                    ;*OPRD ERROR
 
 EOR     (A6),D5                    ;*OPRD ERROR
 
 EOR.B   D1,D5                      ;B305
 EOR.B   D1,A6                      ;*OPRD ERROR
 EOR.B   D1,(A6)                    ;B316
 EOR.B   D1,(A6)+                   ;B31E
 EOR.B   D1,-(A6)                   ;B326
 EOR.B   D1,OFF16(A6)               ;B32E 7FFE
 EOR.B   D1,OFF8(A6,D5)             ;B336 507E
 EOR.B   D1,DIR16                ;B338 7FFE
 EOR.B   D1,DIR24                   ;B339 007F FFFE
 EOR.B   D1,*+D1002(PC)             ;*OPRD ERROR
 EOR.B   D1,*+D80(PC,D5)            ;*OPRD ERROR
 EOR.B   D1,#IM8                    ;*OPRD ERROR
 
 EOR.W   D1,D5                      ;B345
 EOR.W   D1,A6                      ;*OPRD ERROR
 EOR.W   D1,(A6)                    ;B356
 EOR.W   D1,(A6)+                   ;B35E
 EOR.W   D1,-(A6)                   ;B366
 EOR.W   D1,OFF16(A6)               ;B36E 7FFE
 EOR.W   D1,OFF8(A6,D5)             ;B376 507E
 EOR.W   D1,DIR16                ;B378 7FFE
 EOR.W   D1,DIR24                   ;B379 007F FFFE
 EOR.W   D1,*+D1002(PC)             ;*OPRD ERROR
 EOR.W   D1,*+D80(PC,D5)            ;*OPRD ERROR
 EOR.W   D1,#IM8                    ;*OPRD ERROR
 
 EOR.L   D1,D5                      ;B385
 EOR.L   D1,A6                      ;*OPRD ERROR
 EOR.L   D1,(A6)                    ;B396
 EOR.L   D1,(A6)+                   ;B39E
 EOR.L   D1,-(A6)                   ;B3A6
 EOR.L   D1,OFF16(A6)               ;B3AE 7FFE
 EOR.L   D1,OFF8(A6,D5)             ;B3B6 507E
 EOR.L   D1,DIR16                ;B3B8 7FFE
 EOR.L   D1,DIR24                   ;B3B9 007F FFFE
 EOR.L   D1,*+D1002(PC)             ;*OPRD ERROR
 EOR.L   D1,*+D80(PC,D5)            ;*OPRD ERROR
 EOR.L   D1,#IM8                    ;*OPRD ERROR
 
 
 EORI    #IM3,D5                    ;0A45 0007
 EORI    #IM3,A6                    ;*OPRD ERROR
 EORI    #IM3,(A6)                  ;0A56 0007
 EORI    #IM3,(A6)+                 ;0A5E 0007
 EORI    #IM3,-(A6)                 ;0A66 0007
 EORI    #IM3,OFF16(A6)             ;0A6E 0007 7FFE
 EORI    #IM3,OFF8(A6,D5)           ;0A76 0007 507E
 EORI    #IM3,DIR16              ;0A78 0007 7FFE
 EORI    #IM3,DIR24                 ;0A79 0007 007F FFFE
 EORI    #IM3,*+D1002(PC)           ;*OPRD ERROR
 EORI    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EORI    #IM3,#IM8                  ;*OPRD ERROR
 
 EORI.B  #IM3,D5                    ;0A05 0007
 EORI.B  #IM3,A6                    ;*OPRD ERROR
 EORI.B  #IM3,(A6)                  ;0A16 0007
 EORI.B  #IM3,(A6)+                 ;0A1E 0007
 EORI.B  #IM3,-(A6)                 ;0A26 0007
 EORI.B  #IM3,OFF16(A6)             ;0A2E 0007 7FFE
 EORI.B  #IM3,OFF8(A6,D5)           ;0A36 0007 507E
 EORI.B  #IM3,DIR16              ;0A38 0007 7FFE
 EORI.B  #IM3,DIR24                 ;0A39 0007 007F FFFE
 EORI.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 EORI.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EORI.B  #IM3,#IM8                  ;*OPRD ERROR
 
 EORI.W  #IM3,D5                    ;0A45 0007
 EORI.W  #IM3,A6                    ;*OPRD ERROR
 EORI.W  #IM3,(A6)                  ;0A56 0007
 EORI.W  #IM3,(A6)+                 ;0A5E 0007
 EORI.W  #IM3,-(A6)                 ;0A66 0007
 EORI.W  #IM3,OFF16(A6)             ;0A6E 0007 7FFE
 EORI.W  #IM3,OFF8(A6,D5)           ;0A76 0007 507E
 EORI.W  #IM3,DIR16              ;0A78 0007 7FFE
 EORI.W  #IM3,DIR24                 ;0A79 0007 007F FFFE
 EORI.W  #IM3,*+D1002(PC)           ;*OPRD ERROR
 EORI.W  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EORI.W  #IM3,#IM8                  ;*OPRD ERROR
 
 EORI.L  #IM3,D5                    ;0A85 0000 0007
 EORI.L  #IM3,A6                    ;*OPRD ERROR
 EORI.L  #IM3,(A6)                  ;0A96 0000 0007
 EORI.L  #IM3,(A6)+                 ;0A9E 0000 0007
 EORI.L  #IM3,-(A6)                 ;0AA6 0000 0007
 EORI.L  #IM3,OFF16(A6)             ;0AAE 0000 0007 7FFE
 EORI.L  #IM3,OFF8(A6,D5)           ;0AB6 0000 0007 507E
 EORI.L  #IM3,DIR16              ;0AB8 0000 0007 7FFE
 EORI.L  #IM3,DIR24                 ;0AB9 0000 0007 007F FFFE
 EORI.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 EORI.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EORI.L  #IM3,#IM8                  ;*OPRD ERROR
 
 
 EOR     #IM3,D5                    ;0A45 0007
 EOR     #IM3,A6                    ;*OPRD ERROR
 EOR     #IM3,(A6)                  ;0A56 0007
 EOR     #IM3,(A6)+                 ;0A5E 0007
 EOR     #IM3,-(A6)                 ;0A66 0007
 EOR     #IM3,OFF16(A6)             ;0A6E 0007 7FFE
 EOR     #IM3,OFF8(A6,D5)           ;0A76 0007 507E
 EOR     #IM3,DIR16              ;0A78 0007 7FFE
 EOR     #IM3,DIR24                 ;0A79 0007 007F FFFE
 EOR     #IM3,*+D1002(PC)           ;*OPRD ERROR
 EOR     #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EOR     #IM3,#IM8                  ;*OPRD ERROR
 
 EOR.B   #IM3,D5                    ;0A05 0007
 EOR.B   #IM3,A6                    ;*OPRD ERROR
 EOR.B   #IM3,(A6)                  ;0A16 0007
 EOR.B   #IM3,(A6)+                 ;0A1E 0007
 EOR.B   #IM3,-(A6)                 ;0A26 0007
 EOR.B   #IM3,OFF16(A6)             ;0A2E 0007 7FFE
 EOR.B   #IM3,OFF8(A6,D5)           ;0A36 0007 507E
 EOR.B   #IM3,DIR16              ;0A38 0007 7FFE
 EOR.B   #IM3,DIR24                 ;0A39 0007 007F FFFE
 EOR.B   #IM3,*+D1002(PC)           ;*OPRD ERROR
 EOR.B   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EOR.B   #IM3,#IM8                  ;*OPRD ERROR
 
 EOR.W   #IM3,D5                    ;0A45 0007
 EOR.W   #IM3,A6                    ;*OPRD ERROR
 EOR.W   #IM3,(A6)                  ;0A56 0007
 EOR.W   #IM3,(A6)+                 ;0A5E 0007
 EOR.W   #IM3,-(A6)                 ;0A66 0007
 EOR.W   #IM3,OFF16(A6)             ;0A6E 0007 7FFE
 EOR.W   #IM3,OFF8(A6,D5)           ;0A76 0007 507E
 EOR.W   #IM3,DIR16              ;0A78 0007 7FFE
 EOR.W   #IM3,DIR24                 ;0A79 0007 007F FFFE
 EOR.W   #IM3,*+D1002(PC)           ;*OPRD ERROR
 EOR.W   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EOR.W   #IM3,#IM8                  ;*OPRD ERROR
 
 EOR.L   #IM3,D5                    ;0A85 0000 0007
 EOR.L   #IM3,A6                    ;*OPRD ERROR
 EOR.L   #IM3,(A6)                  ;0A96 0000 0007
 EOR.L   #IM3,(A6)+                 ;0A9E 0000 0007
 EOR.L   #IM3,-(A6)                 ;0AA6 0000 0007
 EOR.L   #IM3,OFF16(A6)             ;0AAE 0000 0007 7FFE
 EOR.L   #IM3,OFF8(A6,D5)           ;0AB6 0000 0007 507E
 EOR.L   #IM3,DIR16              ;0AB8 0000 0007 7FFE
 EOR.L   #IM3,DIR24                 ;0AB9 0000 0007 007F FFFE
 EOR.L   #IM3,*+D1002(PC)           ;*OPRD ERROR
 EOR.L   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 EOR.L   #IM3,#IM8                  ;*OPRD ERROR
 
 
 EORI   #IM8,CCR                    ;*O 0A3C 007E          !!!
 
 EORI.B #IM8,CCR                    ;*O 0A3C 007E          !!!
 
 EOR    #IM8,CCR                    ;0A3C 007E
 
 EOR.B  #IM8,CCR                    ;0A3C 007E
 
 EORI   #IM16,CCR                   ;*OPRD ERROR
 
 EORI.W #IM8,CCR                    ;*OPRD ERROR
 
 
 EORI   #IM8,SR                     ;*O 0A7C 007E          !!!
 
 EORI.W #IM8,SR                     ;*O 0A7C 007E          !!!
 
 EOR    #IM8,SR                     ;0A7C 007E
 
 EOR.W  #IM8,SR                     ;0A7C 007E
 
 EORI.B #IM8,SR                     ;*OPRD ERROR
 
 EORI   #IM32,SR                    ;*OPRD ERROR
 
 
 EXG     D1,D2                      ;C342
 EXG     D1,A3                      ;C38B
 EXG     A3,D1                      ;C38B
 EXG     A3,A4                      ;C74C
 
 EXG     (A6),D5                    ;*OPRD ERROR
 EXG     D5,(A6)                    ;*OPRD ERROR
 EXG.B   D1,D2                      ;*MNEM ERROR
 EXG.W   D1,D2                      ;*MNEM ERROR
 
 EXG.L   D1,D2                      ;C342
 EXG.L   D1,A3                      ;C38B
 EXG.L   A3,D1                      ;C38B
 EXG.L   A3,A4                      ;C74C
 
 EXT     D1                         ;4881
 
 EXT     (A6)                       ;*OPRD ERROR
 EXT.B   D1                         ;*MNEM ERROR
 
 EXT.W   D1                         ;4881
 
 EXT.L   D1                         ;48C1
 
 
 ILLEGAL                            ;4AFC
 
 
 JMP     D5                         ;*OPRD ERROR
 JMP     A6                         ;*OPRD ERROR
 JMP     (A6)                       ;4ED6
 JMP     (A6)+                      ;*OPRD ERROR
 JMP     -(A6)                      ;*OPRD ERROR
 JMP     OFF16(A6)                  ;4EEE 7FFE
 JMP     OFF8(A6,D5)                ;4EF6 507E
 JMP     DIR16                   ;4EF8 7FFE
 JMP     DIR24                   ;4EF9 007F FFFE
 JMP     *+D1002(PC)                ;4EFA 1000
 JMP     *+D80(PC,D5)               ;4EFB 507E
 JMP     #IM8                       ;*OPRD ERROR
 
 
 JMP.S   DIR16                   ;4EF8 7FFE
 JMP.L   DIR16                   ;*OPRD ERROR
 JMP.L   DIR24                      ;4EF9 007F FFFE

 JMP     DIR16                   ;4EF8 7FFE
 JMP     DIR16                   ;4EF9 0000 7FFE
 
 JMP.S   DIR24                      ;*OPRD ERROR
 
 JMP.S   DIR16                   ;*OPRD ERROR
 JMP.L   DIR16                   ;*OPRD ERROR
 
 
 JSR     D5                         ;*OPRD ERROR
 JSR     A6                         ;*OPRD ERROR
 JSR     (A6)                       ;4E96
 JSR     (A6)+                      ;*OPRD ERROR
 JSR     -(A6)                      ;*OPRD ERROR
 JSR     OFF16(A6)                  ;4EAE 7FFE
 JSR     OFF8(A6,D5)                ;4EB6 507E
 JSR     DIR16                   ;4EB8 7FFE
 JSR     DIR24                   ;4EB9 007F FFFE
 JSR     *+D1002(PC)                ;4EBA 1000
 JSR     *+D80(PC,D5)               ;4EBB 507E
 JSR     #IM8                       ;*OPRD ERROR
 
 
 LEA     D5,A3                      ;*OPRD ERROR
 LEA     A6,A3                      ;*OPRD ERROR
 LEA     (A6),A3                    ;47D6
 LEA     (A6)+,A3                   ;*OPRD ERROR
 LEA     -(A6),A3                   ;*OPRD ERROR
 LEA     OFF16(A6),A3               ;47EE 7FFE
 LEA     OFF8(A6,D5),A3             ;47F6 507E
 LEA     DIR16,A3                ;47F8 7FFE
 LEA     DIR24,A3                   ;47F9 007F FFFE
 LEA     *+D1002(PC),A3             ;47FA 1000
 LEA     *+D80(PC,D5),A3            ;47FB 507E
 LEA     #IM8,A3                    ;*OPRD ERROR
 
 LEA     (A6),D5                    ;*OPRD ERROR
 
 LEA.L   D5,A3                      ;*OPRD ERROR
 LEA.L   A6,A3                      ;*OPRD ERROR
 LEA.L   (A6),A3                    ;47D6
 LEA.L   (A6)+,A3                   ;*OPRD ERROR
 LEA.L   -(A6),A3                   ;*OPRD ERROR
 LEA.L   OFF16(A6),A3               ;47EE 7FFE
 LEA.L   OFF8(A6,D5),A3             ;47F6 507E
 LEA.L   DIR16,A3                ;47F8 7FFE
 LEA.L   DIR24,A3                   ;47F9 007F FFFE
 LEA.L   *+D1002(PC),A3             ;47FA 1000
 LEA.L   *+D80(PC,D5),A3            ;47FB 507E
 LEA.L   #IM8,A3                    ;*OPRD ERROR
 
 LEA.B   D5,A3                      ;*MNEM ERROR
 LEA.W   D5,A3                      ;*MNEM ERROR
 LINK    A3,#-IM3                   ;4E53 FFF9
 
 
 LINK    A3,#IM3                    ;*O 4E53 0007          !!!
 
 LINK    A3,#IM16                   ;*O 4E53 7FFF          !!!
 
 LINK    D1,#IM3                    ;*OPRD ERROR
 
 LINK    D1,#IM32                   ;*OPRD ERROR
 
 LSL     D1,D2                      ;E36A
 
 LSL     #IM3,D2                    ;EF4A
 
 LSL     #0,D2                      ;*OPRD ERROR
 LSL     #9,D2                      ;*OPRD ERROR
 
 LSL     D1,(A6)                    ;*OPRD ERROR
 LSL     #IM3,(A6)                  ;*OPRD ERROR
 
 LSL     D5                         ;*OPRD ERROR
 LSL     A6                         ;*OPRD ERROR
 LSL     (A6)                       ;E3D6
 LSL     (A6)+                      ;E3DE
 LSL     -(A6)                      ;E3E6
 LSL     OFF16(A6)                  ;E3EE 7FFE
 LSL     OFF8(A6,D5)                ;E3F6 507E
 LSL     DIR16                   ;E3F8 7FFE
 LSL     DIR24                      ;E3F9 007F FFFE
 LSL     *+D1002(PC)                ;*OPRD ERROR
 LSL     *+D80(PC,D5)               ;*OPRD ERROR
 LSL     #IM8                       ;*OPRD ERROR
 
 LSL.B   D1,D2                      ;E32A
 
 LSL.B   #IM3,D2                    ;EF0A
 
 LSL.B   (A6)                       ;*OPRD ERROR
 
 LSL.W   D1,D2                      ;E36A
 
 LSL.W   #IM3,D2                    ;EF4A
 
 LSL.W   D5                         ;*OPRD ERROR
 LSL.W   A6                         ;*OPRD ERROR
 LSL.W   (A6)                       ;E3D6
 LSL.W   (A6)+                      ;E3DE
 LSL.W   -(A6)                      ;E3E6
 LSL.W   OFF16(A6)                  ;E3EE 7FFE
 LSL.W   OFF8(A6,D5)                ;E3F6 507E
 LSL.W   DIR16                   ;E3F8 7FFE
 LSL.W   DIR24                      ;E3F9 007F FFFE
 LSL.W   *+D1002(PC)                ;*OPRD ERROR
 LSL.W   *+D80(PC,D5)               ;*OPRD ERROR
 LSL.W   #IM8                       ;*OPRD ERROR
 
 LSL.L   D1,D2                      ;E3AA
 
 LSL.L   #IM3,D2                    ;EF8A
 
 LSL.L   (A6)                       ;*OPRD ERROR
 
 
 LSR     D1,D2                      ;E26A
 
 LSR     #IM3,D2                    ;EE4A
 
 LSR     #0,D2                      ;*OPRD ERROR
 LSR     #9,D2                      ;*OPRD ERROR
 
 LSR     D1,(A6)                    ;*OPRD ERROR
 LSR     #IM3,(A6)                  ;*OPRD ERROR
 
 LSR     D5                         ;*OPRD ERROR
 LSR     A6                         ;*OPRD ERROR
 LSR     (A6)                       ;E2D6
 LSR     (A6)+                      ;E2DE
 LSR     -(A6)                      ;E2E6
 LSR     OFF16(A6)                  ;E2EE 7FFE
 LSR     OFF8(A6,D5)                ;E2F6 507E
 LSR     DIR16                   ;E2F8 7FFE
 LSR     DIR24                      ;E2F9 007F FFFE
 LSR     *+D1002(PC)                ;*OPRD ERROR
 LSR     *+D80(PC,D5)               ;*OPRD ERROR
 LSR     #IM8                       ;*OPRD ERROR
 
 LSR.B   D1,D2                      ;E22A
 
 LSR.B   #IM3,D2                    ;EE0A
 
 LSR.B   (A6)                       ;*OPRD ERROR
 
 LSR.W   D1,D2                      ;E26A
 
 LSR.W   #IM3,D2                    ;EE4A
 
 LSR.W   D5                         ;*OPRD ERROR
 LSR.W   A6                         ;*OPRD ERROR
 LSR.W   (A6)                       ;E2D6
 LSR.W   (A6)+                      ;E2DE
 LSR.W   -(A6)                      ;E2E6
 LSR.W   OFF16(A6)                  ;E2EE 7FFE
 LSR.W   OFF8(A6,D5)                ;E2F6 507E
 LSR.W   DIR16                   ;E2F8 7FFE
 LSR.W   DIR24                      ;E2F9 007F FFFE
 LSR.W   *+D1002(PC)                ;*OPRD ERROR
 LSR.W   *+D80(PC,D5)               ;*OPRD ERROR
 LSR.W   #IM8                       ;*OPRD ERROR
 
 LSR.L   D1,D2                      ;E2AA
 
 LSR.L   #IM3,D2                    ;EE8A
 
 LSR.L   (A6)                       ;*OPRD ERROR
 
 
 MOVE    D5,D3                      ;3605
 MOVE    D5,A4                      ;3845
 MOVE    D5,(A4)                    ;3885
 MOVE    D5,(A4)+                   ;38C5
 MOVE    D5,-(A4)                   ;3905
 MOVE    D5,OFF15(A4)               ;3945 3333
 MOVE    D5,OFF7(A4,D3)             ;3985 3033
 MOVE    D5,DIR15                ;31C5 3333
 MOVE    D5,DIR23                   ;33C5 0033 3333
 MOVE    D5,*+D1002(PC)             ;*OPRD ERROR
 MOVE    D5,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE    D5,#IM7                    ;*OPRD ERROR
 
 
 MOVE    A6,D3                      ;360E
 MOVE    A6,A4                      ;384E
 MOVE    A6,(A4)                    ;388E
 MOVE    A6,(A4)+                   ;38CE
 MOVE    A6,-(A4)                   ;390E
 MOVE    A6,OFF15(A4)               ;394E 3333
 MOVE    A6,OFF7(A4,D3)             ;398E 3033
 MOVE    A6,DIR15                ;31CE 3333
 MOVE    A6,DIR23                   ;33CE 0033 3333
 MOVE    A6,*+D1002(PC)             ;*OPRD ERROR
 MOVE    A6,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE    A6,#IM7                    ;*OPRD ERROR
 
 
 MOVE    (A6),D3                    ;3616
 MOVE    (A6),A4                    ;3856
 MOVE    (A6),(A4)                  ;3896
 MOVE    (A6),(A4)+                 ;38D6
 MOVE    (A6),-(A4)                 ;3916
 MOVE    (A6),OFF15(A4)             ;3956 3333
 MOVE    (A6),OFF7(A4,D3)           ;3996 3033
 MOVE    (A6),DIR15              ;31D6 3333
 MOVE    (A6),DIR23                 ;33D6 0033 3333
 MOVE    (A6),*+D1002(PC)           ;*OPRD ERROR
 MOVE    (A6),*+D04(PC,D3)          ;*OPRD ERROR
 MOVE    (A6),#IM7                  ;*OPRD ERROR
 
 
 MOVE    (A6)+,D3                   ;361E
 MOVE    (A6)+,A4                   ;385E
 MOVE    (A6)+,(A4)                 ;389E
 MOVE    (A6)+,(A4)+                ;38DE
 MOVE    (A6)+,-(A4)                ;391E
 MOVE    (A6)+,OFF15(A4)            ;395E 3333
 MOVE    (A6)+,OFF7(A4,D3)          ;399E 3033
 MOVE    (A6)+,DIR15             ;31DE 3333
 MOVE    (A6)+,DIR23                ;33DE 0033 3333
 MOVE    (A6)+,*+D1002(PC)          ;*OPRD ERROR
 MOVE    (A6)+,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE    (A6)+,#IM7                 ;*OPRD ERROR
 
 
 MOVE    -(A6),D3                   ;3626
 MOVE    -(A6),A4                   ;3866
 MOVE    -(A6),(A4)                 ;38A6
 MOVE    -(A6),(A4)+                ;38E6
 MOVE    -(A6),-(A4)                ;3926
 MOVE    -(A6),OFF15(A4)            ;3966 3333
 MOVE    -(A6),OFF7(A4,D3)          ;39A6 3033
 MOVE    -(A6),DIR15             ;31E6 3333
 MOVE    -(A6),DIR23                ;33E6 0033 3333
 MOVE    -(A6),*+D1002(PC)          ;*OPRD ERROR
 MOVE    -(A6),*+D04(PC,D3)         ;*OPRD ERROR
 MOVE    -(A6),#IM7                 ;*OPRD ERROR
 
 
 MOVE    OFF16(A6),D3               ;362E 7FFE
 MOVE    OFF16(A6),A4               ;386E 7FFE
 MOVE    OFF16(A6),(A4)             ;38AE 7FFE
 MOVE    OFF16(A6),(A4)+            ;38EE 7FFE
 MOVE    OFF16(A6),-(A4)            ;392E 7FFE
 MOVE    OFF16(A6),OFF15(A4)        ;396E 7FFE 3333
 MOVE    OFF16(A6),OFF7(A4,D3)      ;39AE 7FFE 3033
 MOVE    OFF16(A6),DIR15         ;31EE 7FFE 3333
 MOVE    OFF16(A6),DIR23            ;33EE 7FFE 0033
 MOVE    OFF16(A6),*+D1002(PC)      ;*OPRD ERROR
 MOVE    OFF16(A6),*+D04(PC,D3)     ;*OPRD ERROR
 MOVE    OFF16(A6),#IM7             ;*OPRD ERROR
 
 
 MOVE    OFF8(A6,D5),D3             ;3636 507E
 MOVE    OFF8(A6,D5),A4             ;3876 507E
 MOVE    OFF8(A6,D5),(A4)           ;38B6 507E
 MOVE    OFF8(A6,D5),(A4)+          ;38F6 507E
 MOVE    OFF8(A6,D5),-(A4)          ;3936 507E
 MOVE    OFF8(A6,D5),OFF15(A4)      ;3976 507E 3333
 MOVE    OFF8(A6,D5),OFF7(A4,D3)    ;39B6 507E 3033
 MOVE    OFF8(A6,D5),DIR15       ;31F6 507E 3333
 MOVE    OFF8(A6,D5),DIR23          ;33F6 507E 0033 3333
 MOVE    OFF8(A6,D5),*+D1002(PC)    ;*OPRD ERROR
 MOVE    OFF8(A6,D5),*+D04(PC,D3)   ;*OPRD ERROR
 MOVE    OFF8(A6,D5),#IM7           ;*OPRD ERROR
 
 
 MOVE    DIR16,D3                ;3638 7FFE
 MOVE    DIR16,A4                ;3878 7FFE
 MOVE    DIR16,(A4)              ;38B8 7FFE
 MOVE    DIR16,(A4)+             ;38F8 7FFE
 MOVE    DIR16,-(A4)             ;3938 7FFE
 MOVE    DIR16,OFF15(A4)         ;3978 7FFE 3333
 MOVE    DIR16,OFF7(A4,D3)       ;39B8 7FFE 3033
 MOVE    DIR16,DIR15^.W          ;31F8 7FFE 3333
 MOVE    DIR16,DIR23             ;33F8 7FFE 0033 3333
 MOVE    DIR16,*+D1002(PC)       ;*OPRD ERROR
 MOVE    DIR16,*+D04(PC,D3)      ;*OPRD ERROR
 MOVE    DIR16,#IM7              ;*OPRD ERROR
 
 
 MOVE    DIR24,D3                   ;3639 007F FFFE
 MOVE    DIR24,A4                   ;3879 007F FFFE
 MOVE    DIR24,(A4)                 ;38B9 007F FFFE
 MOVE    DIR24,(A4)+                ;38F9 007F FFFE
 MOVE    DIR24,-(A4)                ;3939 007F FFFE
 MOVE    DIR24,OFF15(A4)            ;3979 007F FFFE 3333
 MOVE    DIR24,OFF7(A4,D3)          ;39B9 007F FFFE 3033
 MOVE    DIR24,DIR15             ;31F9 007F FFFE 3333
 MOVE    DIR24,DIR23                ;33F9 007F FFFE 0033 3333
 MOVE    DIR24,*+D1002(PC)          ;*OPRD ERROR
 MOVE    DIR24,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE    DIR24,#IM7                 ;*OPRD ERROR
 
 
 MOVE    *+D1002(PC),D3             ;363A 1000
 MOVE    *+D1002(PC),A4             ;387A 1000
 MOVE    *+D1002(PC),(A4)           ;38BA 1000
 MOVE    *+D1002(PC),(A4)+          ;38FA 1000
 MOVE    *+D1002(PC),-(A4)          ;393A 1000
 MOVE    *+D1002(PC),OFF15(A4)      ;397A 1000 3333
 MOVE    *+D1002(PC),OFF7(A4,D3)    ;39BA 1000 3033
 MOVE    *+D1002(PC),DIR15       ;31FA 1000 3333
 MOVE    *+D1002(PC),DIR23          ;33FA 1000 0033 3333
 MOVE    *+D1002(PC),$+D1002(PC)    ;*OPRD ERROR
 MOVE    *+D1002(PC),$+D04(PC,D3)   ;*OPRD ERROR
 MOVE    *+D1002(PC),#IM7           ;*OPRD ERROR
 
 
 MOVE    *+D80(PC,D5),D3            ;363B 507E
 MOVE    *+D80(PC,D5),A4            ;387B 507E
 MOVE    *+D80(PC,D5),(A4)          ;38BB 507E
 MOVE    *+D80(PC,D5),(A4)+         ;38FB 507E
 MOVE    *+D80(PC,D5),-(A4)         ;393B 507E
 MOVE    *+D80(PC,D5),OFF15(A4)     ;397B 507E 3333
 MOVE    *+D80(PC,D5),OFF7(A4,D3)   ;39BB 507E 3033
 MOVE    *+D80(PC,D5),DIR15      ;31FB 507E 3333
 MOVE    *+D80(PC,D5),DIR23         ;33FB 507E 0033 3333
 MOVE    *+D80(PC,D5),$+D1002(PC)   ;*OPRD ERROR
 MOVE    *+D80(PC,D5),$+D04(PC,D3)  ;*OPRD ERROR
 MOVE    *+D80(PC,D5),#IM7          ;*OPRD ERROR
 
 
 MOVE    #IM8,D3                    ;363C 007E
 MOVE    #IM8,A4                    ;387C 007E
 MOVE    #IM8,(A4)                  ;38BC 007E
 MOVE    #IM8,(A4)+                 ;38FC 007E
 MOVE    #IM8,-(A4)                 ;393C 007E
 MOVE    #IM8,OFF15(A4)             ;397C 007E 3333
 MOVE    #IM8,OFF7(A4,D3)           ;39BC 007E 3033
 MOVE    #IM8,DIR15              ;31FC 007E 3333
 MOVE    #IM8,DIR23                 ;33FC 007E 0033 3333
 MOVE    #IM8,*+D1002(PC)           ;*OPRD ERROR
 MOVE    #IM8,*+D04(PC,D3)          ;*OPRD ERROR
 MOVE    #IM8,#IM7                  ;*OPRD ERROR
 
 
 MOVE.B  D5,D3                      ;1605
 MOVE.B  D5,A4                      ;*OPRD ERROR
 MOVE.B  D5,(A4)                    ;1885
 MOVE.B  D5,(A4)+                   ;18C5
 MOVE.B  D5,-(A4)                   ;1905
 MOVE.B  D5,OFF15(A4)               ;1945 3333
 MOVE.B  D5,OFF7(A4,D3)             ;1985 3033
 MOVE.B  D5,DIR15                ;11C5 3333
 MOVE.B  D5,DIR23                   ;13C5 0033 3333
 MOVE.B  D5,*+D1002(PC)             ;*OPRD ERROR
 MOVE.B  D5,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE.B  D5,#IM7                    ;*OPRD ERROR
 
 
 MOVE.B  A6,D3                      ;*OPRD ERROR
 MOVE.B  A6,A4                      ;*OPRD ERROR
 MOVE.B  A6,(A4)                    ;*OPRD ERROR
 MOVE.B  A6,(A4)+                   ;*OPRD ERROR
 MOVE.B  A6,-(A4)                   ;*OPRD ERROR
 MOVE.B  A6,OFF15(A4)               ;*OPRD ERROR
 MOVE.B  A6,OFF7(A4,D3)             ;*OPRD ERROR
 MOVE.B  A6,DIR15                ;*OPRD ERROR
 MOVE.B  A6,DIR23                   ;*OPRD ERROR
 MOVE.B  A6,*+D1002(PC)             ;*OPRD ERROR
 MOVE.B  A6,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE.B  A6,#IM7                    ;*OPRD ERROR
 
 
 MOVE.B  (A6),D3                    ;1616
 MOVE.B  (A6),A4                    ;*OPRD ERROR
 MOVE.B  (A6),(A4)                  ;1896
 MOVE.B  (A6),(A4)+                 ;18D6
 MOVE.B  (A6),-(A4)                 ;1916
 MOVE.B  (A6),OFF15(A4)             ;1956 3333
 MOVE.B  (A6),OFF7(A4,D3)           ;1996 3033
 MOVE.B  (A6),DIR15              ;11D6 3333
 MOVE.B  (A6),DIR23                 ;13D6 0033 3333
 MOVE.B  (A6),*+D1002(PC)           ;*OPRD ERROR
 MOVE.B  (A6),*+D04(PC,D3)          ;*OPRD ERROR
 MOVE.B  (A6),#IM7                  ;*OPRD ERROR
 
 
 MOVE.B  (A6)+,D3                   ;161E
 MOVE.B  (A6)+,A4                   ;*OPRD ERROR
 MOVE.B  (A6)+,(A4)                 ;189E
 MOVE.B  (A6)+,(A4)+                ;18DE
 MOVE.B  (A6)+,-(A4)                ;191E
 MOVE.B  (A6)+,OFF15(A4)            ;195E 3333
 MOVE.B  (A6)+,OFF7(A4,D3)          ;199E 3033
 MOVE.B  (A6)+,DIR15             ;11DE 3333
 MOVE.B  (A6)+,DIR23                ;13DE 0033 3333
 MOVE.B  (A6)+,*+D1002(PC)          ;*OPRD ERROR
 MOVE.B  (A6)+,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.B  (A6)+,#IM7                 ;*OPRD ERROR
 
 
 MOVE.B  -(A6),D3                   ;1626
 MOVE.B  -(A6),A4                   ;*OPRD ERROR
 MOVE.B  -(A6),(A4)                 ;18A6
 MOVE.B  -(A6),(A4)+                ;18E6
 MOVE.B  -(A6),-(A4)                ;1926
 MOVE.B  -(A6),OFF15(A4)            ;1966 3333
 MOVE.B  -(A6),OFF7(A4,D3)          ;19A6 3033
 MOVE.B  -(A6),DIR15             ;11E6 3333
 MOVE.B  -(A6),DIR23                ;13E6 0033 3333
 MOVE.B  -(A6),*+D1002(PC)          ;*OPRD ERROR
 MOVE.B  -(A6),*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.B  -(A6),#IM7                 ;*OPRD ERROR
 
 
 MOVE.B  OFF16(A6),D3               ;162E 7FFE
 MOVE.B  OFF16(A6),A4               ;*OPRD ERROR
 MOVE.B  OFF16(A6),(A4)             ;18AE 7FFE
 MOVE.B  OFF16(A6),(A4)+            ;18EE 7FFE
 MOVE.B  OFF16(A6),-(A4)            ;192E 7FFE
 MOVE.B  OFF16(A6),OFF15(A4)        ;196E 7FFE 3333
 MOVE.B  OFF16(A6),OFF7(A4,D3)      ;19AE 7FFE 3033
 MOVE.B  OFF16(A6),DIR15         ;11EE 7FFE 3333
 MOVE.B  OFF16(A6),DIR23            ;13EE 7FFE 0033 3333
 MOVE.B  OFF16(A6),*+D1002(PC)      ;*OPRD ERROR
 MOVE.B  OFF16(A6),*+D04(PC,D3)     ;*OPRD ERROR
 MOVE.B  OFF16(A6),#IM7             ;*OPRD ERROR
 
 
 MOVE.B  OFF8(A6,D5),D3             ;1636 507E
 MOVE.B  OFF8(A6,D5),A4             ;*OPRD ERROR
 MOVE.B  OFF8(A6,D5),(A4)           ;18B6 507E
 MOVE.B  OFF8(A6,D5),(A4)+          ;18F6 507E
 MOVE.B  OFF8(A6,D5),-(A4)          ;1936 507E
 MOVE.B  OFF8(A6,D5),OFF15(A4)      ;1976 507E 3333
 MOVE.B  OFF8(A6,D5),OFF7(A4,D3)    ;19B6 507E 3033
 MOVE.B  OFF8(A6,D5),DIR15       ;11F6 507E 3333
 MOVE.B  OFF8(A6,D5),DIR23          ;13F6 507E 0033 3333
 MOVE.B  OFF8(A6,D5),*+D1002(PC)    ;*OPRD ERROR
 MOVE.B  OFF8(A6,D5),*+D04(PC,D3)   ;*OPRD ERROR
 MOVE.B  OFF8(A6,D5),#IM7           ;*OPRD ERROR
 
 
 MOVE.B  DIR16,D3                ;1638 7FFE
 MOVE.B  DIR16,A4                ;*OPRD ERROR
 MOVE.B  DIR16,(A4)              ;18B8 7FFE
 MOVE.B  DIR16,(A4)+             ;18F8 7FFE
 MOVE.B  DIR16,-(A4)             ;1938 7FFE
 MOVE.B  DIR16,OFF15(A4)         ;1978 7FFE 3333
 MOVE.B  DIR16,OFF7(A4,D3)       ;19B8 7FFE 3033
 MOVE.B  DIR16,DIR15^.W          ;11F8 7FFE 3333
 MOVE.B  DIR16,DIR23             ;13F8 7FFE 0033 3333
 MOVE.B  DIR16,*+D1002(PC)       ;*OPRD ERROR
 MOVE.B  DIR16,*+D04(PC,D3)      ;*OPRD ERROR
 MOVE.B  DIR16,#IM7              ;*OPRD ERROR
 
 
 MOVE.B  DIR24,D3                   ;1639 007F FFFE
 MOVE.B  DIR24,A4                   ;*OPRD ERROR
 MOVE.B  DIR24,(A4)                 ;18B9 007F FFFE
 MOVE.B  DIR24,(A4)+                ;18F9 007F FFFE
 MOVE.B  DIR24,-(A4)                ;1939 007F FFFE
 MOVE.B  DIR24,OFF15(A4)            ;1979 007F FFFE 3333
 MOVE.B  DIR24,OFF7(A4,D3)          ;19B9 007F FFFE 3033
 MOVE.B  DIR24,DIR15             ;11F9 007F FFFE 3333
 MOVE.B  DIR24,DIR23                ;13F9 007F FFFE 0033 3333
 MOVE.B  DIR24,*+D1002(PC)          ;*OPRD ERROR
 MOVE.B  DIR24,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.B  DIR24,#IM7                 ;*OPRD ERROR
 
 
 MOVE.B  *+D1002(PC),D3             ;163A 1000
 MOVE.B  *+D1002(PC),A4             ;*OPRD ERROR
 MOVE.B  *+D1002(PC),(A4)           ;18BA 1000
 MOVE.B  *+D1002(PC),(A4)+          ;18FA 1000
 MOVE.B  *+D1002(PC),-(A4)          ;193A 1000
 MOVE.B  *+D1002(PC),OFF15(A4)      ;197A 1000 3333
 MOVE.B  *+D1002(PC),OFF7(A4,D3)    ;19BA 1000 3033
 MOVE.B  *+D1002(PC),DIR15       ;11FA 1000 3333
 MOVE.B  *+D1002(PC),DIR23          ;13FA 1000 0033 3333
 MOVE.B  *+D1002(PC),$+D1002(PC)    ;*OPRD ERROR
 MOVE.B  *+D1002(PC),$+D04(PC,D3)   ;*OPRD ERROR
 MOVE.B  *+D1002(PC),#IM7           ;*OPRD ERROR
 
 
 MOVE.B  *+D80(PC,D5),D3            ;163B 507E
 MOVE.B  *+D80(PC,D5),A4            ;*OPRD ERROR
 MOVE.B  *+D80(PC,D5),(A4)          ;18BB 507E
 MOVE.B  *+D80(PC,D5),(A4)+         ;18FB 507E
 MOVE.B  *+D80(PC,D5),-(A4)         ;193B 507E
 MOVE.B  *+D80(PC,D5),OFF15(A4)     ;197B 507E 3333
 MOVE.B  *+D80(PC,D5),OFF7(A4,D3)   ;19BB 507E 3033
 MOVE.B  *+D80(PC,D5),DIR15      ;11FB 507E 3333
 MOVE.B  *+D80(PC,D5),DIR23         ;13FB 507E 0033 3333
 MOVE.B  *+D80(PC,D5),$+D1002(PC)   ;*OPRD ERROR
 MOVE.B  *+D80(PC,D5),$+D04(PC,D3)  ;*OPRD ERROR
 MOVE.B  *+D80(PC,D5),#IM7          ;*OPRD ERROR
 
 
 MOVE.B  #IM8,D3                    ;163C 007E
 MOVE.B  #IM8,A4                    ;*OPRD ERROR
 MOVE.B  #IM8,(A4)                  ;18BC 007E
 MOVE.B  #IM8,(A4)+                 ;18FC 007E
 MOVE.B  #IM8,-(A4)                 ;193C 007E
 MOVE.B  #IM8,OFF15(A4)             ;197C 007E 3333
 MOVE.B  #IM8,OFF7(A4,D3)           ;19BC 007E 3033
 MOVE.B  #IM8,DIR15              ;11FC 007E 3333
 MOVE.B  #IM8,DIR23                 ;13FC 007E 0033 3333
 MOVE.B  #IM8,*+D1002(PC)           ;*OPRD ERROR
 MOVE.B  #IM8,*+D04(PC,D3)          ;*OPRD ERROR
 MOVE.B  #IM8,#IM7                  ;*OPRD ERROR
 
 
 MOVE.W  D5,D3                      ;3605
 MOVE.W  D5,A4                      ;3845
 MOVE.W  D5,(A4)                    ;3885
 MOVE.W  D5,(A4)+                   ;38C5
 MOVE.W  D5,-(A4)                   ;3905
 MOVE.W  D5,OFF15(A4)               ;3945 3333
 MOVE.W  D5,OFF7(A4,D3)             ;3985 3033
 MOVE.W  D5,DIR15                ;31C5 3333
 MOVE.W  D5,DIR23                   ;33C5 0033 3333
 MOVE.W  D5,*+D1002(PC)             ;*OPRD ERROR
 MOVE.W  D5,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE.W  D5,#IM7                    ;*OPRD ERROR
 
 
 MOVE.W  A6,D3                      ;360E
 MOVE.W  A6,A4                      ;384E
 MOVE.W  A6,(A4)                    ;388E
 MOVE.W  A6,(A4)+                   ;38CE
 MOVE.W  A6,-(A4)                   ;390E
 MOVE.W  A6,OFF15(A4)               ;394E 3333
 MOVE.W  A6,OFF7(A4,D3)             ;398E 3033
 MOVE.W  A6,DIR15                ;31CE 3333
 MOVE.W  A6,DIR23                   ;33CE 0033 3333
 MOVE.W  A6,*+D1002(PC)             ;*OPRD ERROR
 MOVE.W  A6,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE.W  A6,#IM7                    ;*OPRD ERROR
 
 
 MOVE.W  (A6),D3                    ;3616
 MOVE.W  (A6),A4                    ;3856
 MOVE.W  (A6),(A4)                  ;3896
 MOVE.W  (A6),(A4)+                 ;38D6
 MOVE.W  (A6),-(A4)                 ;3916
 MOVE.W  (A6),OFF15(A4)             ;3956 3333
 MOVE.W  (A6),OFF7(A4,D3)           ;3996 3033
 MOVE.W  (A6),DIR15              ;31D6 3333
 MOVE.W  (A6),DIR23                 ;33D6 0033 3333
 MOVE.W  (A6),*+D1002(PC)           ;*OPRD ERROR
 MOVE.W  (A6),*+D04(PC,D3)          ;*OPRD ERROR
 MOVE.W  (A6),#IM7                  ;*OPRD ERROR
 
 
 MOVE.W  (A6)+,D3                   ;361E
 MOVE.W  (A6)+,A4                   ;385E
 MOVE.W  (A6)+,(A4)                 ;389E
 MOVE.W  (A6)+,(A4)+                ;38DE
 MOVE.W  (A6)+,-(A4)                ;391E
 MOVE.W  (A6)+,OFF15(A4)            ;395E 3333
 MOVE.W  (A6)+,OFF7(A4,D3)          ;399E 3033
 MOVE.W  (A6)+,DIR15             ;31DE 3333
 MOVE.W  (A6)+,DIR23                ;33DE 0033 3333
 MOVE.W  (A6)+,*+D1002(PC)          ;*OPRD ERROR
 MOVE.W  (A6)+,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.W  (A6)+,#IM7                 ;*OPRD ERROR
 
 
 MOVE.W  -(A6),D3                   ;3626
 MOVE.W  -(A6),A4                   ;3866
 MOVE.W  -(A6),(A4)                 ;38A6
 MOVE.W  -(A6),(A4)+                ;38E6
 MOVE.W  -(A6),-(A4)                ;3926
 MOVE.W  -(A6),OFF15(A4)            ;3966 3333
 MOVE.W  -(A6),OFF7(A4,D3)          ;39A6 3033
 MOVE.W  -(A6),DIR15             ;31E6 3333
 MOVE.W  -(A6),DIR23                ;33E6 0033 3333
 MOVE.W  -(A6),*+D1002(PC)          ;*OPRD ERROR
 MOVE.W  -(A6),*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.W  -(A6),#IM7                 ;*OPRD ERROR
 
 
 MOVE.W  OFF16(A6),D3               ;362E 7FFE
 MOVE.W  OFF16(A6),A4               ;386E 7FFE
 MOVE.W  OFF16(A6),(A4)             ;38AE 7FFE
 MOVE.W  OFF16(A6),(A4)+            ;38EE 7FFE
 MOVE.W  OFF16(A6),-(A4)            ;392E 7FFE
 MOVE.W  OFF16(A6),OFF15(A4)        ;396E 7FFE 3333
 MOVE.W  OFF16(A6),OFF7(A4,D3)      ;39AE 7FFE 3033
 MOVE.W  OFF16(A6),DIR15         ;31EE 7FFE 3333
 MOVE.W  OFF16(A6),DIR23            ;33EE 7FFE 0033 3333
 MOVE.W  OFF16(A6),*+D1002(PC)      ;*OPRD ERROR
 MOVE.W  OFF16(A6),*+D04(PC,D3)     ;*OPRD ERROR
 MOVE.W  OFF16(A6),#IM7             ;*OPRD ERROR
 
 
 MOVE.W  OFF8(A6,D5),D3             ;3636 507E
 MOVE.W  OFF8(A6,D5),A4             ;3876 507E
 MOVE.W  OFF8(A6,D5),(A4)           ;38B6 507E
 MOVE.W  OFF8(A6,D5),(A4)+          ;38F6 507E
 MOVE.W  OFF8(A6,D5),-(A4)          ;3936 507E
 MOVE.W  OFF8(A6,D5),OFF15(A4)      ;3976 507E 3333
 MOVE.W  OFF8(A6,D5),OFF7(A4,D3)    ;39B6 507E 3033
 MOVE.W  OFF8(A6,D5),DIR15       ;31F6 507E 3333
 MOVE.W  OFF8(A6,D5),DIR23          ;33F6 507E 0033 3333
 MOVE.W  OFF8(A6,D5),*+D1002(PC)    ;*OPRD ERROR
 MOVE.W  OFF8(A6,D5),*+D04(PC,D3)   ;*OPRD ERROR
 MOVE.W  OFF8(A6,D5),#IM7           ;*OPRD ERROR
 
 
 MOVE.W  DIR16,D3                ;3638 7FFE
 MOVE.W  DIR16,A4                ;3878 7FFE
 MOVE.W  DIR16,(A4)              ;38B8 7FFE
 MOVE.W  DIR16,(A4)+             ;38F8 7FFE
 MOVE.W  DIR16,-(A4)             ;3938 7FFE
 MOVE.W  DIR16,OFF15(A4)         ;3978 7FFE 3333
 MOVE.W  DIR16,OFF7(A4,D3)       ;39B8 7FFE 3033
 MOVE.W  DIR16,DIR15^.W          ;31F8 7FFE 3333
 MOVE.W  DIR16,DIR23             ;33F8 7FFE 0033 3333
 MOVE.W  DIR16,*+D1002(PC)       ;*OPRD ERROR
 MOVE.W  DIR16,*+D04(PC,D3)      ;*OPRD ERROR
 MOVE.W  DIR16,#IM7              ;*OPRD ERROR
 
 
 MOVE.W  DIR24,D3                   ;3639 007F FFFE
 MOVE.W  DIR24,A4                   ;3879 007F FFFE
 MOVE.W  DIR24,(A4)                 ;38B9 007F FFFE
 MOVE.W  DIR24,(A4)+                ;38F9 007F FFFE
 MOVE.W  DIR24,-(A4)                ;3939 007F FFFE
 MOVE.W  DIR24,OFF15(A4)            ;3979 007F FFFE 3333
 MOVE.W  DIR24,OFF7(A4,D3)          ;39B9 007F FFFE 3033
 MOVE.W  DIR24,DIR15             ;31F9 007F FFFE 3333
 MOVE.W  DIR24,DIR23                ;33F9 007F FFFE 0033 3333
 MOVE.W  DIR24,*+D1002(PC)          ;*OPRD ERROR
 MOVE.W  DIR24,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.W  DIR24,#IM7                 ;*OPRD ERROR
 
 
 MOVE.W  *+D1002(PC),D3             ;363A 1000
 MOVE.W  *+D1002(PC),A4             ;387A 1000
 MOVE.W  *+D1002(PC),(A4)           ;38BA 1000
 MOVE.W  *+D1002(PC),(A4)+          ;38FA 1000
 MOVE.W  *+D1002(PC),-(A4)          ;393A 1000
 MOVE.W  *+D1002(PC),OFF15(A4)      ;397A 1000 3333
 MOVE.W  *+D1002(PC),OFF7(A4,D3)    ;39BA 1000 3033
 MOVE.W  *+D1002(PC),DIR15       ;31FA 1000 3333
 MOVE.W  *+D1002(PC),DIR23          ;33FA 1000 0033 3333
 MOVE.W  *+D1002(PC),$+D1002(PC)    ;*OPRD ERROR
 MOVE.W  *+D1002(PC),$+D04(PC,D3)   ;*OPRD ERROR
 MOVE.W  *+D1002(PC),#IM7           ;*OPRD ERROR
 
 
 MOVE.W  *+D80(PC,D5),D3            ;363B 507E
 MOVE.W  *+D80(PC,D5),A4            ;387B 507E
 MOVE.W  *+D80(PC,D5),(A4)          ;38BB 507E
 MOVE.W  *+D80(PC,D5),(A4)+         ;38FB 507E
 MOVE.W  *+D80(PC,D5),-(A4)         ;393B 507E
 MOVE.W  *+D80(PC,D5),OFF15(A4)     ;397B 507E 3333
 MOVE.W  *+D80(PC,D5),OFF7(A4,D3)   ;39BB 507E 3033
 MOVE.W  *+D80(PC,D5),DIR15      ;31FB 507E 3333
 MOVE.W  *+D80(PC,D5),DIR23         ;33FB 507E 0033 3333
 MOVE.W  *+D80(PC,D5),$+D1002(PC)   ;*OPRD ERROR
 MOVE.W  *+D80(PC,D5),$+D04(PC,D3)  ;*OPRD ERROR
 MOVE.W  *+D80(PC,D5),#IM7          ;*OPRD ERROR
 
 
 MOVE.W  #IM8,D3                    ;363C 007E
 MOVE.W  #IM8,A4                    ;387C 007E
 MOVE.W  #IM8,(A4)                  ;38BC 007E
 MOVE.W  #IM8,(A4)+                 ;38FC 007E
 MOVE.W  #IM8,-(A4)                 ;393C 007E
 MOVE.W  #IM8,OFF15(A4)             ;397C 007E 3333
 MOVE.W  #IM8,OFF7(A4,D3)           ;39BC 007E 3033
 MOVE.W  #IM8,DIR15              ;31FC 007E 3333
 MOVE.W  #IM8,DIR23                 ;33FC 007E 0033 3333
 MOVE.W  #IM8,*+D1002(PC)           ;*OPRD ERROR
 MOVE.W  #IM8,*+D04(PC,D3)          ;*OPRD ERROR
 MOVE.W  #IM8,#IM7                  ;*OPRD ERROR
 
 
 MOVE.L  D5,D3                      ;2605
 MOVE.L  D5,A4                      ;2845
 MOVE.L  D5,(A4)                    ;2885
 MOVE.L  D5,(A4)+                   ;28C5
 MOVE.L  D5,-(A4)                   ;2905
 MOVE.L  D5,OFF15(A4)               ;2945 3333
 MOVE.L  D5,OFF7(A4,D3)             ;2985 3033
 MOVE.L  D5,DIR15                ;21C5 3333
 MOVE.L  D5,DIR23                   ;23C5 0033 3333
 MOVE.L  D5,*+D1002(PC)             ;*OPRD ERROR
 MOVE.L  D5,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE.L  D5,#IM7                    ;*OPRD ERROR
 
 
 MOVE.L  A6,D3                      ;260E
 MOVE.L  A6,A4                      ;284E
 MOVE.L  A6,(A4)                    ;288E
 MOVE.L  A6,(A4)+                   ;28CE
 MOVE.L  A6,-(A4)                   ;290E
 MOVE.L  A6,OFF15(A4)               ;294E 3333
 MOVE.L  A6,OFF7(A4,D3)             ;298E 3033
 MOVE.L  A6,DIR15                ;21CE 3333
 MOVE.L  A6,DIR23                   ;23CE 0033 3333
 MOVE.L  A6,*+D1002(PC)             ;*OPRD ERROR
 MOVE.L  A6,*+D04(PC,D3)            ;*OPRD ERROR
 MOVE.L  A6,#IM7                    ;*OPRD ERROR
 
 
 MOVE.L  (A6),D3                    ;2616
 MOVE.L  (A6),A4                    ;2856
 MOVE.L  (A6),(A4)                  ;2896
 MOVE.L  (A6),(A4)+                 ;28D6
 MOVE.L  (A6),-(A4)                 ;2916
 MOVE.L  (A6),OFF15(A4)             ;2956 3333
 MOVE.L  (A6),OFF7(A4,D3)           ;2966 3033
 MOVE.L  (A6),DIR15              ;21D6 3333
 MOVE.L  (A6),DIR23                 ;23D6 0033 3333
 MOVE.L  (A6),*+D1002(PC)           ;*OPRD ERROR
 MOVE.L  (A6),*+D04(PC,D3)          ;*OPRD ERROR
 MOVE.L  (A6),#IM7                  ;*OPRD ERROR
 
 
 MOVE.L  (A6)+,D3                   ;261E
 MOVE.L  (A6)+,A4                   ;285E
 MOVE.L  (A6)+,(A4)                 ;289E
 MOVE.L  (A6)+,(A4)+                ;28DE
 MOVE.L  (A6)+,-(A4)                ;291E
 MOVE.L  (A6)+,OFF15(A4)            ;295E 3333
 MOVE.L  (A6)+,OFF7(A4,D3)          ;299E 3033
 MOVE.L  (A6)+,DIR15             ;21DE 3333
 MOVE.L  (A6)+,DIR23                ;23DE 0033 3333
 MOVE.L  (A6)+,*+D1002(PC)          ;*OPRD ERROR
 MOVE.L  (A6)+,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.L  (A6)+,#IM7                 ;*OPRD ERROR
 
 
 MOVE.L  -(A6),D3                   ;2626
 MOVE.L  -(A6),A4                   ;2866
 MOVE.L  -(A6),(A4)                 ;28A6
 MOVE.L  -(A6),(A4)+                ;28E6
 MOVE.L  -(A6),-(A4)                ;2926
 MOVE.L  -(A6),OFF15(A4)            ;2966 3333
 MOVE.L  -(A6),OFF7(A4,D3)          ;29A6 3033
 MOVE.L  -(A6),DIR15             ;21E6 3333
 MOVE.L  -(A6),DIR23                ;23E6 0033 3333
 MOVE.L  -(A6),*+D1002(PC)          ;*OPRD ERROR
 MOVE.L  -(A6),*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.L  -(A6),#IM7                 ;*OPRD ERROR
 
 
 MOVE.L  OFF16(A6),D3               ;262E 7FFE
 MOVE.L  OFF16(A6),A4               ;286E 7FFE
 MOVE.L  OFF16(A6),(A4)             ;28AE 7FFE
 MOVE.L  OFF16(A6),(A4)+            ;28EE 7FFE
 MOVE.L  OFF16(A6),-(A4)            ;292E 7FFE
 MOVE.L  OFF16(A6),OFF15(A4)        ;296E 7FFE 3333
 MOVE.L  OFF16(A6),OFF7(A4,D3)      ;29AE 7FFE 3033
 MOVE.L  OFF16(A6),DIR15         ;21EE 7FFE 3333
 MOVE.L  OFF16(A6),DIR23            ;23EE 7FFE 0033 3333
 MOVE.L  OFF16(A6),*+D1002(PC)      ;*OPRD ERROR
 MOVE.L  OFF16(A6),*+D04(PC,D3)     ;*OPRD ERROR
 MOVE.L  OFF16(A6),#IM7             ;*OPRD ERROR
 
 
 MOVE.L  OFF8(A6,D5),D3             ;2636 507E
 MOVE.L  OFF8(A6,D5),A4             ;2876 507E
 MOVE.L  OFF8(A6,D5),(A4)           ;28B6 507E
 MOVE.L  OFF8(A6,D5),(A4)+          ;28F6 507E
 MOVE.L  OFF8(A6,D5),-(A4)          ;2936 507E
 MOVE.L  OFF8(A6,D5),OFF15(A4)      ;2976 507E 3333
 MOVE.L  OFF8(A6,D5),OFF7(A4,D3)    ;29B6 507E 3033
 MOVE.L  OFF8(A6,D5),DIR15       ;21F6 507E 3033
 MOVE.L  OFF8(A6,D5),DIR23          ;23F6 507E 0033 3333
 MOVE.L  OFF8(A6,D5),*+D1002(PC)    ;*OPRD ERROR
 MOVE.L  OFF8(A6,D5),*+D04(PC,D3)   ;*OPRD ERROR
 MOVE.L  OFF8(A6,D5),#IM7           ;*OPRD ERROR
 
 
 MOVE.L  DIR16,D3                ;2638 7FFE
 MOVE.L  DIR16,A4                ;2878 7FFE
 MOVE.L  DIR16,(A4)              ;28B8 7FFE
 MOVE.L  DIR16,(A4)+             ;28F8 7FFE
 MOVE.L  DIR16,-(A4)             ;2938 7FFE
 MOVE.L  DIR16,OFF15(A4)         ;2978 7FFE 3333
 MOVE.L  DIR16,OFF7(A4,D3)       ;29B8 7FFE 3033
 MOVE.L  DIR16,DIR15^.W          ;21F8 7FFE 3333
 MOVE.L  DIR16,DIR23             ;23F8 7FFE 0033
 MOVE.L  DIR16,*+D1002(PC)       ;*OPRD ERROR
 MOVE.L  DIR16,*+D04(PC,D3)      ;*OPRD ERROR
 MOVE.L  DIR16,#IM7              ;*OPRD ERROR
 
 
 MOVE.L  DIR24,D3                   ;2639 007F FFFE
 MOVE.L  DIR24,A4                   ;2879 007F FFFE
 MOVE.L  DIR24,(A4)                 ;28B9 007F FFFE
 MOVE.L  DIR24,(A4)+                ;28F9 007F FFFE
 MOVE.L  DIR24,-(A4)                ;2939 007F FFFE
 MOVE.L  DIR24,OFF15(A4)            ;2979 007F FFFE 3333
 MOVE.L  DIR24,OFF7(A4,D3)          ;29B9 007F FFFE 3033
 MOVE.L  DIR24,DIR15             ;21F9 007F FFFE 3333
 MOVE.L  DIR24,DIR23                ;23F9 007F FFFE 0033 3333
 MOVE.L  DIR24,*+D1002(PC)          ;*OPRD ERROR
 MOVE.L  DIR24,*+D04(PC,D3)         ;*OPRD ERROR
 MOVE.L  DIR24,#IM7                 ;*OPRD ERROR
 
 
 MOVE.L  *+D1002(PC),D3             ;263A 1000
 MOVE.L  *+D1002(PC),A4             ;287A 1000
 MOVE.L  *+D1002(PC),(A4)           ;28BA 1000
 MOVE.L  *+D1002(PC),(A4)+          ;28FA 1000
 MOVE.L  *+D1002(PC),-(A4)          ;293A 1000
 MOVE.L  *+D1002(PC),OFF15(A4)      ;297A 1000 3333
 MOVE.L  *+D1002(PC),OFF7(A4,D3)    ;29BA 1000 3033
 MOVE.L  *+D1002(PC),DIR15       ;21FA 1000 3333
 MOVE.L  *+D1002(PC),DIR23          ;23FA 1000 0033 3333
 MOVE.L  *+D1002(PC),$+D1002(PC)    ;*OPRD ERROR
 MOVE.L  *+D1002(PC),$+D04(PC,D3)   ;*OPRD ERROR
 MOVE.L  *+D1002(PC),#IM7           ;*OPRD ERROR
 
 
 MOVE.L  *+D80(PC,D5),D3            ;263B 507E
 MOVE.L  *+D80(PC,D5),A4            ;287B 507E
 MOVE.L  *+D80(PC,D5),(A4)          ;28BB 507E
 MOVE.L  *+D80(PC,D5),(A4)+         ;28FB 507E
 MOVE.L  *+D80(PC,D5),-(A4)         ;293B 507E
 MOVE.L  *+D80(PC,D5),OFF15(A4)     ;297B 507E 3333
 MOVE.L  *+D80(PC,D5),OFF7(A4,D3)   ;29BB 507E 3033
 MOVE.L  *+D80(PC,D5),DIR15      ;21FB 507E 3333
 MOVE.L  *+D80(PC,D5),DIR23         ;23FB 507E 0033 3333
 MOVE.L  *+D80(PC,D5),$+D1002(PC)   ;*OPRD ERROR
 MOVE.L  *+D80(PC,D5),$+D04(PC,D3)  ;*OPRD ERROR
 MOVE.L  *+D80(PC,D5),#IM7          ;*OPRD ERROR
 
 
 MOVE.L  #IM8,D3                    ;767E
 MOVE.L  #IM8,A4                    ;287C 0000 007E
 MOVE.L  #IM8,(A4)                  ;28BC 0000 007E
 MOVE.L  #IM8,(A4)+                 ;28FC 0000 007E
 MOVE.L  #IM8,-(A4)                 ;293C 0000 007E
 MOVE.L  #IM8,OFF15(A4)             ;297C 0000 007E 3333
 MOVE.L  #IM8,OFF7(A4,D3)           ;29BC 0000 007E 3033
 MOVE.L  #IM8,DIR15              ;21FC 0000 007E 3333
 MOVE.L  #IM8,DIR23                 ;23FC 0000 007E 0033 3333
 MOVE.L  #IM8,*+D1002(PC)           ;*OPRD ERROR
 MOVE.L  #IM8,*+D04(PC,D3)          ;*OPRD ERROR
 MOVE.L  #IM8,#IM7                  ;*OPRD ERROR
 
 
 MOVEQ   #IM8,D1                    ;727E
 MOVE    #IM8,D1                    ;323C 007E
 MOVEQ.L #IM8,D1                    ;727E
 
 MOVEQ   #IM16,D1                   ;*OPRD ERROR
 
 
 MOVE    D5,CCR                     ;*O 44C5          !!!
 MOVE    A6,CCR                     ;*OPRD ERROR   
 MOVE    (A6),CCR                   ;*O 44D6          !!!
 MOVE    (A6)+,CCR                  ;*O 44DE          !!!
 MOVE    -(A6),CCR                  ;*O 44E6          !!!
 MOVE    OFF16(A6),CCR              ;*O 44EE 7FFE          !!!
 MOVE    OFF8(A6,D5),CCR            ;*O 44F6 507E          !!!
 MOVE    DIR16,CCR               ;*O 44F8 7FFE          !!!
 MOVE    DIR24,CCR                  ;*O 44F9 007F FFFE          !!!
 MOVE    *+D1002(PC),CCR            ;*O 44FA 1000          !!!
 MOVE    *+D80(PC,D5),CCR           ;*O 44FB 507E          !!!
 MOVE    #IM8,CCR                   ;*O 44FC 007E          !!!
 
 MOVE.W  D5,CCR                     ;*O 44C5          !!!
 MOVE.W  A6,CCR                     ;*OPRD ERROR
 MOVE.W  (A6),CCR                   ;*O 44D6          !!!
 MOVE.W  (A6)+,CCR                  ;*O 44DE          !!!
 MOVE.W  -(A6),CCR                  ;*O 44E6          !!!
 MOVE.W  OFF16(A6),CCR              ;*O 44EE 7FFE          !!!
 MOVE.W  OFF8(A6,D5),CCR            ;*O 44F6 507E          !!!
 MOVE.W  DIR16,CCR               ;*O 44F8 7FFE          !!!
 MOVE.W  DIR24,CCR                  ;*O 44F9 007F FFFE          !!!
 MOVE.W  *+D1002(PC),CCR            ;*O 44FA 1000          !!!
 MOVE.W  *+D80(PC,D5),CCR           ;*O 44FB 507E          !!!
 MOVE.W  #IM8,CCR                   ;*O 44FC 007E          !!!
 
 MOVE.B  D5,CCR                     ;*? OPRD ERROR          !!!
 MOVE.L  D5,CCR                     ;*OPRD ERROR
 
 
 MOVE    D5,SR                      ;46C5
 MOVE    A6,SR                      ;*OPRD ERROR
 MOVE    (A6),SR                    ;46D6
 MOVE    (A6)+,SR                   ;46DE
 MOVE    -(A6),SR                   ;46E6
 MOVE    OFF16(A6),SR               ;46EE 7FFE
 MOVE    OFF8(A6,D5),SR             ;46F6 507E
 MOVE    DIR16,SR                ;46F8 7FFE
 MOVE    DIR24,SR                   ;46F9 007F FFFE
 MOVE    *+D1002(PC),SR             ;46FA 1000
 MOVE    *+D80(PC,D5),SR            ;46FB 507E
 MOVE    #IM8,SR                    ;46FC 007E
 
 MOVE.W  D5,SR                      ;46C5
 MOVE.W  A6,SR                      ;*OPRD ERROR
 MOVE.W  (A6),SR                    ;46D6
 MOVE.W  (A6)+,SR                   ;46DE
 MOVE.W  -(A6),SR                   ;46E6
 MOVE.W  OFF16(A6),SR               ;46EE 7FFE
 MOVE.W  OFF8(A6,D5),SR             ;46F6 507E
 MOVE.W  DIR16,SR                ;46F8 7FFE
 MOVE.W  DIR24,SR                   ;46F9 007F FFFE
 MOVE.W  *+D1002(PC),SR             ;46FA 1000
 MOVE.W  *+D80(PC,D5),SR            ;46FB 507E
 MOVE.W  #IM8,SR                    ;46FC 007E
 
 
 MOVE    SR,D5                      ;40C5
 MOVE    SR,A6                      ;*OPRD ERROR
 MOVE    SR,(A6)                    ;40D6
 MOVE    SR,(A6)+                   ;40DE
 MOVE    SR,-(A6)                   ;40E6
 MOVE    SR,OFF16(A6)               ;40EE 7FFE
 MOVE    SR,OFF8(A6,D5)             ;40F6 507E
 MOVE    SR,DIR16                ;40F8 7FFE
 MOVE    SR,DIR24                   ;40F9 007F FFFE
 MOVE    SR,*+D1002(PC)             ;*OPRD ERROR
 MOVE    SR,*+D80(PC,D5)            ;*OPRD ERROR
 MOVE    SR,#IM8                    ;*OPRD ERROR
 
 MOVE.W  SR,D5                      ;40C5
 
 MOVE.B  SR,D5                      ;*OPRD ERROR
 MOVE.L  SR,D5                      ;*OPRD ERROR
 
 
 MOVE    USP,A3                     ;*O 4E6B          !!!
 MOVE    A3,USP                     ;*O 4E63          !!!
 
 MOVE    USP,D1                     ;*OPRD ERROR
 MOVE    D1,USP                     ;*OPRD ERROR
 
 MOVE.B  USP,A3                     ;*OPRD ERROR
 
 MOVE.W  USP,A3                     ;*OPRD ERROR
 
 MOVE.L  USP,A3                     ;4E6B
 MOVE.L  A3,USP                     ;4E63
 
 
 MOVEA   D5,A3                      ;3645
 MOVEA   A6,A3                      ;364E
 MOVEA   (A6),A3                    ;3656
 MOVEA   (A6)+,A3                   ;365E
 MOVEA   -(A6),A3                   ;3666
 MOVEA   OFF16(A6),A3               ;366E 7FFE
 MOVEA   OFF8(A6,D5),A3             ;3676 507E
 MOVEA   DIR16,A3                ;3678 7FFE
 MOVEA   DIR24,A3                   ;3679 007F FFFE
 MOVEA   *+D1002(PC),A3             ;367A 1000
 MOVEA   *+D80(PC,D5),A3            ;367B 507E
 MOVEA   #IM8,A3                    ;367C 007E
 
 MOVEA   D5,D5                      ;*OPRD ERROR
 
 MOVEA.B D5,A3                      ;*MNEM ERROR
 
 MOVEA.W D5,A3                      ;3645
 MOVEA.W A6,A3                      ;364E
 MOVEA.W (A6),A3                    ;3656
 MOVEA.W (A6)+,A3                   ;365E
 MOVEA.W -(A6),A3                   ;3666
 MOVEA.W OFF16(A6),A3               ;366E 7FFE
 MOVEA.W OFF8(A6,D5),A3             ;3676 507E
 MOVEA.W DIR16,A3                ;3678 7FFE
 MOVEA.W DIR24,A3                   ;3679 007F FFFE
 MOVEA.W *+D1002(PC),A3             ;367A 1000
 MOVEA.W *+D80(PC,D5),A3            ;367B 507E
 MOVEA.W #IM8,A3                    ;367C 007E
 
 MOVEA.L D5,A3                      ;2645
 MOVEA.L A6,A3                      ;264E
 MOVEA.L (A6),A3                    ;2656
 MOVEA.L (A6)+,A3                   ;265E
 MOVEA.L -(A6),A3                   ;2666
 MOVEA.L OFF16(A6),A3               ;266E 7FFE
 MOVEA.L OFF8(A6,D5),A3             ;2676 507E
 MOVEA.L DIR16,A3                ;2678 7FFE
 MOVEA.L DIR24,A3                   ;2679 007F FFFE
 MOVEA.L *+D1002(PC),A3             ;267A 1000
 MOVEA.L *+D80(PC,D5),A3            ;267B 507E
 MOVEA.L #IM8,A3                    ;267C 0000 007E
 
 
 MOVE    D5,A3                      ;3645
 MOVE    A6,A3                      ;364E
 MOVE    (A6),A3                    ;3656
 MOVE    (A6)+,A3                   ;365E
 MOVE    -(A6),A3                   ;3666
 MOVE    OFF16(A6),A3               ;366E 7FFE
 MOVE    OFF8(A6,D5),A3             ;3676 507E
 MOVE    DIR16,A3                ;3678 7FFE
 MOVE    DIR24,A3                   ;3679 007F FFFE
 MOVE    *+D1002(PC),A3             ;367A 1000
 MOVE    *+D80(PC,D5),A3            ;367B 507E
 MOVE    #IM8,A3                    ;367C 007E
 
 MOVE.W  D5,A3                      ;3645
 MOVE.W  A6,A3                      ;364E
 MOVE.W  (A6),A3                    ;3656
 MOVE.W  (A6)+,A3                   ;365E
 MOVE.W  -(A6),A3                   ;3666
 MOVE.W  OFF16(A6),A3               ;366E 7FFE
 MOVE.W  OFF8(A6,D5),A3             ;3676 507E
 MOVE.W  DIR16,A3                ;3678 7FFE
 MOVE.W  DIR24,A3                   ;3679 007F FFFE
 MOVE.W  *+D1002(PC),A3             ;367A 1000
 MOVE.W  *+D80(PC,D5),A3            ;367B 507E
 MOVE.W  #IM8,A3                    ;367C 007E
 
 MOVE.L  D5,A3                      ;2645
 MOVE.L  A6,A3                      ;264E
 MOVE.L  (A6),A3                    ;2656
 MOVE.L  (A6)+,A3                   ;265E
 MOVE.L  -(A6),A3                   ;2666
 MOVE.L  OFF16(A6),A3               ;266E 7FFE
 MOVE.L  OFF8(A6,D5),A3             ;2676 507E
 MOVE.L  DIR16,A3                ;2678 7FFE
 MOVE.L  DIR24,A3                   ;2679 007F FFFE
 MOVE.L  *+D1002(PC),A3             ;267A 1000
 MOVE.L  *+D80(PC,D5),A3            ;267B 507E
 MOVE.L  #IM8,A3                    ;267C 0000 007E
 
 
 MOVEM   RLIST,D5                   ;*OPRD ERROR
 MOVEM   RLIST,A6                   ;*OPRD ERROR
 MOVEM   RLIST,(A6)                 ;4896 E02F
 MOVEM   RLIST,(A6)+                ;*OPRD ERROR
 MOVEM   RLIST,-(A6)                ;48A6 F407
 MOVEM   RLIST,OFF16(A6)            ;48AE E02F 7FFE
 MOVEM   RLIST,OFF8(A6,D5)          ;48B6 E02F 507E
 MOVEM   RLIST,DIR16             ;48B8 E02F 7FFE
 MOVEM   RLIST,DIR24                ;48B9 E02F 007F FFFE
 MOVEM   RLIST,*+D1002(PC)          ;*OPRD ERROR
 MOVEM   RLIST,*+D80(PC,D5)         ;*OPRD ERROR
 MOVEM   RLIST,#IM8                 ;*OPRD ERROR
 
 MOVEM   D5,RLIST                   ;*OPRD ERROR
 MOVEM   A6,RLIST                   ;*OPRD ERROR
 MOVEM   (A6),RLIST                 ;4C96 E02F
 MOVEM   (A6)+,RLIST                ;4C9E E02F
 MOVEM   -(A6),RLIST                ;*OPRD ERROR
 MOVEM   OFF16(A6),RLIST            ;4CAE E02F 7FFE
 MOVEM   OFF8(A6,D5),RLIST          ;4CB6 E02F 507E
 MOVEM   DIR16,RLIST             ;4CB8 E02F 7FFE
 MOVEM   DIR24,RLIST                ;4CB9 E02F 007F FFFE
 MOVEM   *+D1002(PC),RLIST          ;4CBA E02F 0FFE
 MOVEM   *+D80(PC,D5),RLIST         ;4CBB E02F 507C
 MOVEM   #IM8,RLIST                 ;*OPRD ERROR
 
 MOVEM.W RLIST,D5                   ;*OPRD ERROR
 MOVEM.W RLIST,A6                   ;*OPRD ERROR
 MOVEM.W RLIST,(A6)                 ;4896 E02F
 MOVEM.W RLIST,(A6)+                ;*OPRD ERROR
 MOVEM.W RLIST,-(A6)                ;48A6 F407
 MOVEM.W RLIST,OFF16(A6)            ;48AE E02F 7FFE
 MOVEM.W RLIST,OFF8(A6,D5)          ;48B6 E02F 507E
 MOVEM.W RLIST,DIR16             ;48B8 E02F 7FFE
 MOVEM.W RLIST,DIR24                ;48B9 E02F 007F FFFE
 MOVEM.W RLIST,*+D1002(PC)          ;*OPRD ERROR
 MOVEM.W RLIST,*+D80(PC,D5)         ;*OPRD ERROR
 MOVEM.W RLIST,#IM8                 ;*OPRD ERROR
 
 MOVEM.W D5,RLIST                   ;*OPRD ERROR
 MOVEM.W A6,RLIST                   ;*OPRD ERROR
 MOVEM.W (A6),RLIST                 ;4C96 E02F
 MOVEM.W (A6)+,RLIST                ;4C9E E02F
 MOVEM.W -(A6),RLIST                ;*OPRD ERROR
 MOVEM.W OFF16(A6),RLIST            ;4CAE E02F 7FFE
 MOVEM.W OFF8(A6,D5),RLIST          ;4CB6 E02F 507E
 MOVEM.W DIR16,RLIST             ;4CB8 E02F 7FFE
 MOVEM.W DIR24,RLIST                ;4CB9 E02F 007F FFFE
 MOVEM.W *+D1002(PC),RLIST          ;4CBA E02F 0FFE
 MOVEM.W *+D80(PC,D5),RLIST         ;4CBB E02F 507C
 MOVEM.W #IM8,RLIST                 ;*OPRD ERROR
 
 MOVEM.L RLIST,D5                   ;*OPRD ERROR
 MOVEM.L RLIST,A6                   ;*OPRD ERROR
 MOVEM.L RLIST,(A6)                 ;48D6 E02F
 MOVEM.L RLIST,(A6)+                ;*OPRD ERROR
 MOVEM.L RLIST,-(A6)                ;48E6 F407
 MOVEM.L RLIST,OFF16(A6)            ;48EE E02F 7FFE
 MOVEM.L RLIST,OFF8(A6,D5)          ;48F6 E02F 507E
 MOVEM.L RLIST,DIR16             ;48F8 E02F 7FFE
 MOVEM.L RLIST,DIR24                ;48F9 E02F 007F FFFE
 MOVEM.L RLIST,*+D1002(PC)          ;*OPRD ERROR
 MOVEM.L RLIST,*+D80(PC,D5)         ;*OPRD ERROR
 MOVEM.L RLIST,#IM8                 ;*OPRD ERROR
 
 MOVEM.L D5,RLIST                   ;*OPRD ERROR
 MOVEM.L A6,RLIST                   ;*OPRD ERROR
 MOVEM.L (A6),RLIST                 ;4CD6 E02F
 MOVEM.L (A6)+,RLIST                ;4CDE E02F
 MOVEM.L -(A6),RLIST                ;*OPRD ERROR
 MOVEM.L OFF16(A6),RLIST            ;4CEE E02F 7FFE
 MOVEM.L OFF8(A6,D5),RLIST          ;4CF6 E02F 507E
 MOVEM.L DIR16,RLIST             ;4CF8 E02F 7FFE
 MOVEM.L DIR24,RLIST                ;4CF9 E02F 007F FFFE
 MOVEM.L *+D1002(PC),RLIST          ;4CFA E02F 0FFE
 MOVEM.L *+D80(PC,D5),RLIST         ;4CFB E02F 507C
 MOVEM.L #IM8,RLIST                 ;*OPRD ERROR
 
 MOVEM.B D5,D0-D3/D5/A6-A7          ;*MNEM ERROR
 
 
 
 MOVEP   D1,DIR16(A4)               ;038C 7FFE
 MOVEP   DIR16(A4),D1               ;030C 7FFE
 
 MOVEP.W D1,DIR16(A4)               ;038C 7FFE
 MOVEP.W DIR16(A4),D1               ;030C 7FFE
 
 MOVEP.L D1,DIR16(A4)               ;03CC 7FFE
 MOVEP.L DIR16(A4),D1               ;034C 7FFE
 
 MOVEP.B D1,DIR16(A4)               ;*MNEM ERROR
 
 
 MOVEQ   #IM3,D1                    ;7207
 
 MOVEQ.B #IM3,D1                    ;*MNEM ERROR
 MOVEQ.W #IM3,D1                    ;*MNEM ERROR
 
 MOVEQ.L #IM3,D1                    ;7207
 
 
 MOVE    #IM3,D1                    ;323C 0007
 
 MOVE.L  #IM3,D1                    ;7207
 
 
 MULS    D5,D1                      ;C3C5
 MULS    A6,D1                      ;*OPRD ERROR
 MULS    (A6),D1                    ;C3D6
 MULS    (A6)+,D1                   ;C3DE
 MULS    -(A6),D1                   ;C3E6
 MULS    OFF16(A6),D1               ;C3EE 7FFE
 MULS    OFF8(A6,D5),D1             ;C3F6 507E
 MULS    DIR16,D1                ;C3F8 7FFE
 MULS    DIR24,D1                   ;C3F9 007F FFFE
 MULS    *+D1002(PC),D1             ;C3FA 1000
 MULS    *+D80(PC,D5),D1            ;C3FB 507E
 MULS    #IM8,D1                    ;C3FC 007E
 
 MULS.B  D5,D1                      ;*MNEM ERROR
 
 MULS.W  D5,D1                      ;C3C5
 MULS.W  A6,D1                      ;*OPRD ERROR
 MULS.W  (A6),D1                    ;C3D6
 MULS.W  (A6)+,D1                   ;C3DE
 MULS.W  -(A6),D1                   ;C3E6
 MULS.W  OFF16(A6),D1               ;C3EE 7FFE
 MULS.W  OFF8(A6,D5),D1             ;C3F6 507E
 MULS.W  DIR16,D1                ;C3F8 7FFE
 MULS.W  DIR24,D1                   ;C3F9 007F FFFE
 MULS.W  *+D1002(PC),D1             ;C3FA 1000
 MULS.W  *+D80(PC,D5),D1            ;C3FB 507E
 MULS.W  #IM8,D1                    ;C3FC 007E
 
 MULS.L  D5,D1                      ;*MNEM ERROR
 
 
 MULU    D5,D1                      ;C2C5
 MULU    A6,D1                      ;*OPRD ERROR
 MULU    (A6),D1                    ;C2D6
 MULU    (A6)+,D1                   ;C2DE
 MULU    -(A6),D1                   ;C2E6
 MULU    OFF16(A6),D1               ;C2EE 7FFE
 MULU    OFF8(A6,D5),D1             ;C2F6 507E
 MULU    DIR16,D1                ;C2F8 7FFE
 MULU    DIR24,D1                   ;C2F9 007F FFFE
 MULU    *+D1002(PC),D1             ;C2FA 1000
 MULU    *+D80(PC,D5),D1            ;C2FB 507E
 MULU    #IM8,D1                    ;C2FC 007E
 
 MULU.B  D5,D1                      ;*MNEM ERROR
 
 MULU.W  D5,D1                      ;C2C5
 MULU.W  A6,D1                      ;*OPRD ERROR
 MULU.W  (A6),D1                    ;C2D6
 MULU.W  (A6)+,D1                   ;C2DE
 MULU.W  -(A6),D1                   ;C2E6
 MULU.W  OFF16(A6),D1               ;C2EE 7FFE
 MULU.W  OFF8(A6,D5),D1             ;C2F6 507E
 MULU.W  DIR16,D1                ;C2F8 7FFE
 MULU.W  DIR24,D1                   ;C2F9 007F FFFE
 MULU.W  *+D1002(PC),D1             ;C2FA 1000
 MULU.W  *+D80(PC,D5),D1            ;C2FB 507E
 MULU.W  #IM8,D1                    ;C2FC 007E
 
 MULU.L  D5,D1                      ;*MNEM ERROR
 
 
 NBCD    D5                         ;4805
 NBCD    A6                         ;*OPRD ERROR
 NBCD    (A6)                       ;4816
 NBCD    (A6)+                      ;481E
 NBCD    -(A6)                      ;4826
 NBCD    OFF16(A6)                  ;482E 7FFE
 NBCD    OFF8(A6,D5)                ;4836 507E
 NBCD    DIR16                   ;4838 7FFE
 NBCD    DIR24                      ;4839 007F FFFE
 NBCD    *+D1002(PC)                ;*OPRD ERROR
 NBCD    *+D80(PC,D5)               ;*OPRD ERROR
 NBCD    #IM8                       ;*OPRD ERROR
 
 NBCD.B  D5                         ;4805
 NBCD.B  A6                         ;*OPRD ERROR
 NBCD.B  (A6)                       ;4816
 NBCD.B  (A6)+                      ;481E
 NBCD.B  -(A6)                      ;4826
 NBCD.B  OFF16(A6)                  ;482E 7FFE
 NBCD.B  OFF8(A6,D5)                ;4836 507E
 NBCD.B  DIR16                   ;4838 7FFE
 NBCD.B  DIR24                      ;4839 007F FFFE
 NBCD.B  *+D1002(PC)                ;*OPRD ERROR
 NBCD.B  *+D80(PC,D5)               ;*OPRD ERROR
 NBCD.B  #IM8                       ;*OPRD ERROR
 
 NBCD.W  D5                         ;*MNEM ERROR
 
 NBCD.L  D5                         ;*MNEM ERROR
 
 
 NEG     D5                         ;4445
 NEG     A6                         ;*OPRD ERROR
 NEG     (A6)                       ;4456
 NEG     (A6)+                      ;445E
 NEG     -(A6)                      ;4466
 NEG     OFF16(A6)                  ;446E 7FFE
 NEG     OFF8(A6,D5)                ;4476 507E
 NEG     DIR16                   ;4478 7FFE
 NEG     DIR24                      ;4479 007F FFFE
 NEG     *+D1002(PC)                ;*OPRD ERROR
 NEG     *+D80(PC,D5)               ;*OPRD ERROR
 NEG     #IM8                       ;*OPRD ERROR
 
 NEG.B   D5                         ;4405
 NEG.B   A6                         ;*OPRD ERROR
 NEG.B   (A6)                       ;4416
 NEG.B   (A6)+                      ;441E
 NEG.B   -(A6)                      ;4426
 NEG.B   OFF16(A6)                  ;442E 7FFE
 NEG.B   OFF8(A6,D5)                ;4436 507E
 NEG.B   DIR16                   ;4438 7FFE
 NEG.B   DIR24                      ;4439 007F FFFE
 NEG.B   *+D1002(PC)                ;*OPRD ERROR
 NEG.B   *+D80(PC,D5)               ;*OPRD ERROR
 NEG.B   #IM8                       ;*OPRD ERROR
 
 NEG.W   D5                         ;4445
 NEG.W   A6                         ;*OPRD ERROR
 NEG.W   (A6)                       ;4456
 NEG.W   (A6)+                      ;445E
 NEG.W   -(A6)                      ;4466
 NEG.W   OFF16(A6)                  ;446E 7FFE
 NEG.W   OFF8(A6,D5)                ;4476 507E
 NEG.W   DIR16                   ;4478 7FFE
 NEG.W   DIR24                      ;4479 007F FFFE
 NEG.W   *+D1002(PC)                ;*OPRD ERROR
 NEG.W   *+D80(PC,D5)               ;*OPRD ERROR
 NEG.W   #IM8                       ;*OPRD ERROR
 
 NEG.L   D5                         ;4485
 NEG.L   A6                         ;*OPRD ERROR
 NEG.L   (A6)                       ;4496
 NEG.L   (A6)+                      ;449E
 NEG.L   -(A6)                      ;44A6
 NEG.L   OFF16(A6)                  ;44AE 7FFE
 NEG.L   OFF8(A6,D5)                ;44B6 507E
 NEG.L   DIR16                   ;44B8 7FFE
 NEG.L   DIR24                      ;44B9 007F FFFE
 NEG.L   *+D1002(PC)                ;*OPRD ERROR
 NEG.L   *+D80(PC,D5)               ;*OPRD ERROR
 NEG.L   #IM8                       ;*OPRD ERROR
 
 
 NEGX    D5                         ;4045
 NEGX    A6                         ;*OPRD ERROR
 NEGX    (A6)                       ;4056
 NEGX    (A6)+                      ;405E
 NEGX    -(A6)                      ;4066
 NEGX    OFF16(A6)                  ;406E 7FFE
 NEGX    OFF8(A6,D5)                ;4076 507E
 NEGX    DIR16                   ;4078 7FFE
 NEGX    DIR24                      ;4079 007F FFFE
 NEGX    *+D1002(PC)                ;*OPRD ERROR
 NEGX    *+D80(PC,D5)               ;*OPRD ERROR
 NEGX    #IM8                       ;*OPRD ERROR
 
 NEGX.B  D5                         ;4005
 NEGX.B  A6                         ;*OPRD ERROR
 NEGX.B  (A6)                       ;4016
 NEGX.B  (A6)+                      ;401E
 NEGX.B  -(A6)                      ;4026
 NEGX.B  OFF16(A6)                  ;402E 7FFE
 NEGX.B  OFF8(A6,D5)                ;4036 507E
 NEGX.B  DIR16                   ;4038 7FFE
 NEGX.B  DIR24                      ;4039 007F FFFE
 NEGX.B  *+D1002(PC)                ;*OPRD ERROR
 NEGX.B  *+D80(PC,D5)               ;*OPRD ERROR
 NEGX.B  #IM8                       ;*OPRD ERROR
 
 NEGX.W  D5                         ;4045
 NEGX.W  A6                         ;*OPRD ERROR
 NEGX.W  (A6)                       ;4056
 NEGX.W  (A6)+                      ;405E
 NEGX.W  -(A6)                      ;4066
 NEGX.W  OFF16(A6)                  ;406E 7FFE
 NEGX.W  OFF8(A6,D5)                ;4076 507E
 NEGX.W  DIR16                   ;4078 7FFE
 NEGX.W  DIR24                      ;4079 007F FFFE
 NEGX.W  *+D1002(PC)                ;*OPRD ERROR
 NEGX.W  *+D80(PC,D5)               ;*OPRD ERROR
 NEGX.W  #IM8                       ;*OPRD ERROR
 
 NEGX.L  D5                         ;4085
 NEGX.L  A6                         ;*OPRD ERROR
 NEGX.L  (A6)                       ;4096
 NEGX.L  (A6)+                      ;409E
 NEGX.L  -(A6)                      ;40A6
 NEGX.L  OFF16(A6)                  ;40AE 7FFE
 NEGX.L  OFF8(A6,D5)                ;40B6 507E
 NEGX.L  DIR16                   ;40B8 7FFE
 NEGX.L  DIR24                      ;40B9 007F FFFE
 NEGX.L  *+D1002(PC)                ;*OPRD ERROR
 NEGX.L  *+D80(PC,D5)               ;*OPRD ERROR
 NEGX.L  #IM8                       ;*OPRD ERROR
 
 
 NOP                                ;4E71
 
 
 NOT     D5                         ;4645
 NOT     A6                         ;*OPRD ERROR
 NOT     (A6)                       ;4656
 NOT     (A6)+                      ;465E
 NOT     -(A6)                      ;4666
 NOT     OFF16(A6)                  ;466E 7FFE
 NOT     OFF8(A6,D5)                ;4676 507E
 NOT     DIR16                   ;4678 7FFE
 NOT     DIR24                      ;4679 007F FFFE
 NOT     *+D1002(PC)                ;*OPRD ERROR
 NOT     *+D80(PC,D5)               ;*OPRD ERROR
 NOT     #IM8                       ;*OPRD ERROR
 
 NOT.B   D5                         ;4605
 NOT.B   A6                         ;*OPRD ERROR
 NOT.B   (A6)                       ;4616
 NOT.B   (A6)+                      ;461E
 NOT.B   -(A6)                      ;4626
 NOT.B   OFF16(A6)                  ;462E 7FFE
 NOT.B   OFF8(A6,D5)                ;4636 507E
 NOT.B   DIR16                   ;4638 7FFE
 NOT.B   DIR24                      ;4639 007F FFFE
 NOT.B   *+D1002(PC)                ;*OPRD ERROR
 NOT.B   *+D80(PC,D5)               ;*OPRD ERROR
 NOT.B   #IM8                       ;*OPRD ERROR
 
 NOT.W   D5                         ;4645
 NOT.W   A6                         ;*OPRD ERROR
 NOT.W   (A6)                       ;4656
 NOT.W   (A6)+                      ;465E
 NOT.W   -(A6)                      ;4666
 NOT.W   OFF16(A6)                  ;466E 7FFE
 NOT.W   OFF8(A6,D5)                ;4676 507E
 NOT.W   DIR16                   ;4678 7FFE
 NOT.W   DIR24                      ;4679 007F FFFE
 NOT.W   *+D1002(PC)                ;*OPRD ERROR
 NOT.W   *+D80(PC,D5)               ;*OPRD ERROR
 NOT.W   #IM8                       ;*OPRD ERROR
 
 NOT.L   D5                         ;4685
 NOT.L   A6                         ;*OPRD ERROR
 NOT.L   (A6)                       ;4696
 NOT.L   (A6)+                      ;469E
 NOT.L   -(A6)                      ;46A6
 NOT.L   OFF16(A6)                  ;46AE 7FFE
 NOT.L   OFF8(A6,D5)                ;46B6 507E
 NOT.L   DIR16                   ;46B8 7FFE
 NOT.L   DIR24                      ;46B9 007F FFFE
 NOT.L   *+D1002(PC)                ;*OPRD ERROR
 NOT.L   *+D80(PC,D5)               ;*OPRD ERROR
 NOT.L   #IM8                       ;*OPRD ERROR
 
 
 OR      D5,D1                      ;8245
 OR      A6,D1                      ;*OPRD ERROR
 OR      (A6),D1                    ;8256
 OR      (A6)+,D1                   ;825E
 OR      -(A6),D1                   ;8266
 OR      OFF16(A6),D1               ;826E 7FFE
 OR      OFF8(A6,D5),D1             ;8276 507E
 OR      DIR16,D1                ;8278 7FFE
 OR      DIR24,D1                   ;8279 007F FFFE
 OR      *+D1002(PC),D1             ;827A 1000
 OR      *+D80(PC,D5),D1            ;827B 507E
 OR      #IM8,D1                    ;0041 (827C) 007E
 
 OR      D1,D5                      ;8A41
 OR      D1,A6                      ;*OPRD ERROR
 OR      D1,(A6)                    ;8356
 OR      D1,(A6)+                   ;835E
 OR      D1,-(A6)                   ;8366
 OR      D1,OFF16(A6)               ;836E 7FFE
 OR      D1,OFF8(A6,D5)             ;8376 507E
 OR      D1,DIR16                ;8378 7FFE
 OR      D1,DIR24                   ;8379 007F FFFE
 OR      D1,*+D1002(PC)             ;*OPRD ERROR
 OR      D1,*+D80(PC,D5)            ;*OPRD ERROR
 OR      D1,#IM8                    ;*OPRD ERROR
 
 OR.B    D5,D1                      ;8205
 OR.B    A6,D1                      ;*OPRD ERROR
 OR.B    (A6),D1                    ;8216
 OR.B    (A6)+,D1                   ;821E
 OR.B    -(A6),D1                   ;8226
 OR.B    OFF16(A6),D1               ;822E 7FFE
 OR.B    OFF8(A6,D5),D1             ;8236 507E
 OR.B    DIR16,D1                ;8238 7FFE
 OR.B    DIR24,D1                   ;8239 007F FFFE
 OR.B    *+D1002(PC),D1             ;823A 1000
 OR.B    *+D80(PC,D5),D1            ;823B 507E
 OR.B    #IM8,D1                    ;0001 (823C) 007E
 
 OR.B    D1,D5                      ;8A01
 OR.B    D1,A6                      ;*OPRD ERROR
 OR.B    D1,(A6)                    ;8316
 OR.B    D1,(A6)+                   ;831E
 OR.B    D1,-(A6)                   ;8326
 OR.B    D1,OFF16(A6)               ;832E 7FFE
 OR.B    D1,OFF8(A6,D5)             ;8336 507E
 OR.B    D1,DIR16                ;8338 7FFE
 OR.B    D1,DIR24                   ;8339 007F FFFE
 OR.B    D1,*+D1002(PC)             ;*OPRD ERROR
 OR.B    D1,*+D80(PC,D5)            ;*OPRD ERROR
 OR.B    D1,#IM8                    ;*OPRD ERROR
 
 OR.W    D5,D1                      ;8245
 OR.W    A6,D1                      ;*OPRD ERROR
 OR.W    (A6),D1                    ;8256
 OR.W    (A6)+,D1                   ;825E
 OR.W    -(A6),D1                   ;8266
 OR.W    OFF16(A6),D1               ;826E 7FFE
 OR.W    OFF8(A6,D5),D1             ;8276 507E
 OR.W    DIR16,D1                   ;8278 7FFE
 OR.W    DIR24,D1                   ;8279 007F FFFE
 OR.W    *+D1002(PC),D1             ;827A 1000
 OR.W    *+D80(PC,D5),D1            ;827B 507E
 OR.W    #IM8,D1                    ;0041 (827C) 007E
 
 OR.W    D1,D5                      ;8A41
 OR.W    D1,A6                      ;*OPRD ERROR
 OR.W    D1,(A6)                    ;8356
 OR.W    D1,(A6)+                   ;835E
 OR.W    D1,-(A6)                   ;8366
 OR.W    D1,OFF16(A6)               ;836E 7FFE
 OR.W    D1,OFF8(A6,D5)             ;8376 507E
 OR.W    D1,DIR16                ;8378 7FFE
 OR.W    D1,DIR24                   ;8379 007F FFFE
 OR.W    D1,*+D1002(PC)             ;*OPRD ERROR
 OR.W    D1,*+D80(PC,D5)            ;*OPRD ERROR
 OR.W    D1,#IM8                    ;*OPRD ERROR
 
 
 OR.L    D5,D1                      ;8285
 OR.L    A6,D1                      ;*OPRD ERROR
 OR.L    (A6),D1                    ;8296
 OR.L    (A6)+,D1                   ;829E
 OR.L    -(A6),D1                   ;82A6
 OR.L    OFF16(A6),D1               ;82AE 7FFE
 OR.L    OFF8(A6,D5),D1             ;82B6 507E
 OR.L    DIR16,D1                ;82B8 7FFE
 OR.L    DIR24,D1                   ;82B9 007F FFFE
 OR.L    *+D1002(PC),D1             ;82BA 1000
 OR.L    *+D80(PC,D5),D1            ;82BB 507E
 OR.L    #IM8,D1                    ;0081 (82BC) 0000 007E
 
 OR.L    D1,D5                      ;8A81
 OR.L    D1,A6                      ;*OPRD ERROR
 OR.L    D1,(A6)                    ;8396
 OR.L    D1,(A6)+                   ;839E
 OR.L    D1,-(A6)                   ;83A6
 OR.L    D1,OFF16(A6)               ;83AE 7FFE
 OR.L    D1,OFF8(A6,D5)             ;83B6 507E
 OR.L    D1,DIR16                ;83B8 7FFE
 OR.L    D1,DIR24                   ;83B9 007F FFFE
 OR.L    D1,*+D1002(PC)             ;*OPRD ERROR
 OR.L    D1,*+D80(PC,D5)            ;*OPRD ERROR
 OR.L    D1,#IM8                    ;*OPRD ERROR
 
 ORI     #IM3,D5                    ;0045 0007
 ORI     #IM3,A6                    ;*OPRD ERROR
 ORI     #IM3,(A6)                  ;0056 0007
 ORI     #IM3,(A6)+                 ;005E 0007
 ORI     #IM3,-(A6)                 ;0066 0007
 ORI     #IM3,OFF16(A6)             ;006E 0007 7FFE
 ORI     #IM3,OFF8(A6,D5)           ;0076 0007 507E
 ORI     #IM3,DIR16              ;0078 0007 7FFE
 ORI     #IM3,DIR24                 ;0079 0007 007F FFFE
 ORI     #IM3,*+D1002(PC)           ;*OPRD ERROR
 ORI     #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ORI     #IM3,#IM8                  ;*OPRD ERROR
 
 ORI.B   #IM3,D5                    ;0005 0007
 ORI.B   #IM3,A6                    ;*OPRD ERROR
 ORI.B   #IM3,(A6)                  ;0016 0007
 ORI.B   #IM3,(A6)+                 ;001E 0007
 ORI.B   #IM3,-(A6)                 ;0026 0007
 ORI.B   #IM3,OFF16(A6)             ;002E 0007 7FFE
 ORI.B   #IM3,OFF8(A6,D5)           ;0036 0007 507E
 ORI.B   #IM3,DIR16              ;0038 0007 7FFE
 ORI.B   #IM3,DIR24                 ;0039 0007 007F FFFE
 ORI.B   #IM3,*+D1002(PC)           ;*OPRD ERROR
 ORI.B   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ORI.B   #IM3,#IM8                  ;*OPRD ERROR
 
 ORI.W   #IM3,D5                    ;0045 0007
 ORI.W   #IM3,A6                    ;*OPRD ERROR
 ORI.W   #IM3,(A6)                  ;0056 0007
 ORI.W   #IM3,(A6)+                 ;005E 0007
 ORI.W   #IM3,-(A6)                 ;0066 0007
 ORI.W   #IM3,OFF16(A6)             ;006E 0007 7FFE
 ORI.W   #IM3,OFF8(A6,D5)           ;0076 0007 507E
 ORI.W   #IM3,DIR16              ;0078 0007 7FFE
 ORI.W   #IM3,DIR24                 ;0079 0007 007F FFFE
 ORI.W   #IM3,*+D1002(PC)           ;*OPRD ERROR
 ORI.W   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ORI.W   #IM3,#IM8                  ;*OPRD ERROR
 
 ORI.L   #IM3,D5                    ;0085 0000 0007
 ORI.L   #IM3,A6                    ;*OPRD ERROR
 ORI.L   #IM3,(A6)                  ;0096 0000 0007
 ORI.L   #IM3,(A6)+                 ;009E 0000 0007
 ORI.L   #IM3,-(A6)                 ;00A6 0000 0007
 ORI.L   #IM3,OFF16(A6)             ;00AE 0000 0007 7FFE
 ORI.L   #IM3,OFF8(A6,D5)           ;00B6 0000 0007 507E
 ORI.L   #IM3,DIR16              ;00B8 0000 0007 7FFE
 ORI.L   #IM3,DIR24                 ;00B9 0000 0007 007F FFFE
 ORI.L   #IM3,*+D1002(PC)           ;*OPRD ERROR
 ORI.L   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 ORI.L   #IM3,#IM8                  ;*OPRD ERROR
 
 
 OR      #IM3,D5                    ;0045 (8A7C) 0007
 OR      #IM3,A6                    ;*OPRD ERROR
 OR      #IM3,(A6)                  ;0056 0007
 OR      #IM3,(A6)+                 ;005E 0007
 OR      #IM3,-(A6)                 ;0066 0007
 OR      #IM3,OFF16(A6)             ;006E 0007 7FFE
 OR      #IM3,OFF8(A6,D5)           ;0076 0007 507E
 OR      #IM3,DIR16              ;0078 0007 7FFE
 OR      #IM3,DIR24                 ;0079 0007 007F FFFE
 OR      #IM3,*+D1002(PC)           ;*OPRD ERROR
 OR      #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 OR      #IM3,#IM8                  ;*OPRD ERROR
 
 OR.B    #IM3,D5                    ;0005 (8A3C) 0007
 OR.B    #IM3,A6                    ;*OPRD ERROR
 OR.B    #IM3,(A6)                  ;0016 0007
 OR.B    #IM3,(A6)+                 ;001E 0007
 OR.B    #IM3,-(A6)                 ;0026 0007
 OR.B    #IM3,OFF16(A6)             ;002E 0007 7FFE
 OR.B    #IM3,OFF8(A6,D5)           ;0036 0007 507E
 OR.B    #IM3,DIR16              ;0038 0007 7FFE
 OR.B    #IM3,DIR24                 ;0039 0007 007F FFFE
 OR.B    #IM3,*+D1002(PC)           ;*OPRD ERROR
 OR.B    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 OR.B    #IM3,#IM8                  ;*OPRD ERROR
 
 OR.W    #IM3,D5                    ;0045 (8A7C) 0007
 OR.W    #IM3,A6                    ;*OPRD ERROR
 OR.W    #IM3,(A6)                  ;0056 0007
 OR.W    #IM3,(A6)+                 ;005E 0007
 OR.W    #IM3,-(A6)                 ;0066 0007
 OR.W    #IM3,OFF16(A6)             ;006E 0007 7FFE
 OR.W    #IM3,OFF8(A6,D5)           ;0076 0007 507E
 OR.W    #IM3,DIR16              ;0078 0007 7FFE
 OR.W    #IM3,DIR24                 ;0079 0007 007F FFFE
 OR.W    #IM3,*+D1002(PC)           ;*OPRD ERROR
 OR.W    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 OR.W    #IM3,#IM8                  ;*OPRD ERROR
 
 OR.L    #IM3,D5                    ;0085 (8ABC) 0000 0007
 OR.L    #IM3,A6                    ;*OPRD ERROR
 OR.L    #IM3,(A6)                  ;0096 0000 0007
 OR.L    #IM3,(A6)+                 ;009E 0000 0007
 OR.L    #IM3,-(A6)                 ;00A6 0000 0007
 OR.L    #IM3,OFF16(A6)             ;00AE 0000 0007 7FFE
 OR.L    #IM3,OFF8(A6,D5)           ;00B6 0000 0007 507E
 OR.L    #IM3,DIR16              ;00B8 0000 0007 7FFE
 OR.L    #IM3,DIR24                 ;00B9 0000 0007 007F FFFE
 OR.L    #IM3,*+D1002(PC)           ;*OPRD ERROR
 OR.L    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 OR.L    #IM3,#IM8                  ;*OPRD ERROR
 
 
 ORI    #IM8,CCR                    ;*O 003C 007E          !!!
 
 ORI.B  #IM8,CCR                    ;*O 003C 007E          !!!
 
 OR     #IM8,CCR                    ;003C 007E
 
 OR.B   #IM8,CCR                    ;003C 007E
 
 ORI    #IM16,CCR                   ;*OPRD ERROR
 
 ORI.W  #IM8,CCR                    ;*OPRD ERROR
 
 ORI.L   #IM8,CCR                   ;*OPRD ERROR
 
 
 ORI    #IM8,SR                     ;*O 007C 007E          !!!
 
 ORI.W  #IM8,SR                     ;*O 007C 007E          !!!
 
 OR     #IM8,SR                     ;007C 007E
 
 OR.W   #IM8,SR                     ;007C 007E
 
 ORI    #IM32,SR                    ;*OPRD ERROR
 
 ORI.B  #IM8,SR                     ;*OPRD ERROR
 
 ORI.L   #IM8,SR                    ;*OPRD ERROR
 
 
 PEA     D5                         ;*OPRD ERROR
 PEA     A6                         ;*OPRD ERROR
 PEA     (A6)                       ;4856
 PEA     (A6)+                      ;*OPRD ERROR
 PEA     -(A6)                      ;*OPRD ERROR
 PEA     OFF16(A6)                  ;486E 7FFE
 PEA     OFF8(A6,D5)                ;4876 507E
 PEA     DIR16                   ;4878 7FFE
 PEA     DIR24                      ;4879 007F FFFE
 PEA     *+D1002(PC)                ;487A 1000
 PEA     *+D80(PC,D5)               ;487B 507E
 PEA     #IM8                       ;*OPRD ERROR
 
 PEA.B   (A6)                       ;*MNEM ERROR
 
 PEA.W   (A6)                       ;*MNEM ERROR
 
 PEA.L   D5                         ;*OPRD ERROR
 PEA.L   A6                         ;*OPRD ERROR
 PEA.L   (A6)                       ;4856
 PEA.L   (A6)+                      ;*OPRD ERROR
 PEA.L   -(A6)                      ;*OPRD ERROR
 PEA.L   OFF16(A6)                  ;486E 7FFE
 PEA.L   OFF8(A6,D5)                ;4876 507E
 PEA.L   DIR16                   ;4878 7FFE
 PEA.L   DIR24                      ;4879 007F FFFE
 PEA.L   *+D1002(PC)                ;487A 1000
 PEA.L   *+D80(PC,D5)               ;487B 507E
 PEA.L   #IM8                       ;*OPRD ERROR
 
 
 RESET                              ;4E70
 
 
 ROL     D1,D2                      ;E37A
 
 ROL     #IM3,D2                    ;EF5A
 
 ROL     #0,D2                      ;*OPRD ERROR
 ROL     #9,D2                      ;*OPRD ERROR
 
 ROL     D1,(A6)                    ;*OPRD ERROR
 ROL     #IM3,(A6)                  ;*OPRD ERROR
 
 ROL     D5                         ;*OPRD ERROR
 ROL     A6                         ;*OPRD ERROR
 ROL     (A6)                       ;E7D6
 ROL     (A6)+                      ;E7DE
 ROL     -(A6)                      ;E7E6
 ROL     OFF16(A6)                  ;E7EE 7FFE
 ROL     OFF8(A6,D5)                ;E7F6 507E
 ROL     DIR16                   ;E7F8 7FFE
 ROL     DIR24                      ;E7F9 007F FFFE
 ROL     *+D1002(PC)                ;*OPRD ERROR
 ROL     *+D80(PC,D5)               ;*OPRD ERROR
 ROL     #IM8                       ;*OPRD ERROR
 
 ROL.B   D1,D2                      ;E33A
 
 ROL.B   #IM3,D2                    ;EF1A
 
 ROL.B   (A6)                       ;*OPRD ERROR
 
 ROL.W   D1,D2                      ;E37A
 
 ROL.W   #IM3,D2                    ;EF5A
 
 ROL.W   D5                         ;*OPRD ERROR
 ROL.W   A6                         ;*OPRD ERROR
 ROL.W   (A6)                       ;E7D6
 ROL.W   (A6)+                      ;E7DE
 ROL.W   -(A6)                      ;E7E6
 ROL.W   OFF16(A6)                  ;E7EE 7FFE
 ROL.W   OFF8(A6,D5)                ;E7F6 507E
 ROL.W   DIR16                   ;E7F8 7FFE
 ROL.W   DIR24                      ;E7F9 007F FFFE
 ROL.W   *+D1002(PC)                ;*OPRD ERROR
 ROL.W   *+D80(PC,D5)               ;*OPRD ERROR
 ROL.W   #IM8                       ;*OPRD ERROR
 
 ROL.L   D1,D2                      ;E3BA
 
 ROL.L   #IM3,D2                    ;EF9A
 
 ROL.L   (A6)                       ;*OPRD ERROR
 
 
 ROR     D1,D2                      ;E27A
 
 ROR     #IM3,D2                    ;EE5A
 
 ROR     #0,D2                      ;*OPRD ERROR
 ROR     #9,D2                      ;*OPRD ERROR
 
 ROR     D1,(A6)                    ;*OPRD ERROR
 ROR     #IM3,(A6)                  ;*OPRD ERROR
 
 ROR     D5                         ;*OPRD ERROR
 ROR     A6                         ;*OPRD ERROR
 ROR     (A6)                       ;E6D6
 ROR     (A6)+                      ;E6DE
 ROR     -(A6)                      ;E6E6
 ROR     OFF16(A6)                  ;E6EE 7FFE
 ROR     OFF8(A6,D5)                ;E6F6 507E
 ROR     DIR16                   ;E6F8 7FFE
 ROR     DIR24                      ;E6F9 007F FFFE
 ROR     *+D1002(PC)                ;*OPRD ERROR
 ROR     *+D80(PC,D5)               ;*OPRD ERROR
 ROR     #IM8                       ;*OPRD ERROR
 
 ROR.B   D1,D2                      ;E23A
 
 ROR.B   #IM3,D2                    ;EE1A
 
 ROR.B   (A6)                       ;*OPRD ERROR
 
 ROR.W   D1,D2                      ;E27A
 
 ROR.W   #IM3,D2                    ;EE5A
 
 ROR.W   D5                         ;*OPRD ERROR
 ROR.W   A6                         ;*OPRD ERROR
 ROR.W   (A6)                       ;E6D6
 ROR.W   (A6)+                      ;E6DE
 ROR.W   -(A6)                      ;E6E6
 ROR.W   OFF16(A6)                  ;E6EE 7FFE
 ROR.W   OFF8(A6,D5)                ;E6F6 507E
 ROR.W   DIR16                   ;E6F8 7FFE
 ROR.W   DIR24                      ;E6F9 007F FFFE
 ROR.W   *+D1002(PC)                ;*OPRD ERROR
 ROR.W   *+D80(PC,D5)               ;*OPRD ERROR
 ROR.W   #IM8                       ;*OPRD ERROR
 
 ROR.L   D1,D2                      ;E2BA
 
 ROR.L   #IM3,D2                    ;EE9A
 
 ROR.L   (A6)                       ;*OPRD ERROR
 
 
 ROXL    D1,D2                      ;E372
 
 ROXL    #IM3,D2                    ;EF52
 
 ROXL    #0,D2                      ;*OPRD ERROR
 ROXL    #9,D2                      ;*OPRD ERROR
 
 ROXL    D1,(A6)                    ;*OPRD ERROR
 ROXL    #IM3,(A6)                  ;*OPRD ERROR
 
 ROXL    D5                         ;*OPRD ERROR
 ROXL    A6                         ;*OPRD ERROR
 ROXL    (A6)                       ;E5D6
 ROXL    (A6)+                      ;E5DE
 ROXL    -(A6)                      ;E5E6
 ROXL    OFF16(A6)                  ;E5EE 7FFE
 ROXL    OFF8(A6,D5)                ;E5F6 507E
 ROXL    DIR16                   ;E5F8 7FFE
 ROXL    DIR24                      ;E5F9 007F FFFE
 ROXL    *+D1002(PC)                ;*OPRD ERROR
 ROXL    *+D80(PC,D5)               ;*OPRD ERROR
 ROXL    #IM8                       ;*OPRD ERROR
 
 ROXL.B  D1,D2                      ;E332
 
 ROXL.B  #IM3,D2                    ;EF12
 
 ROXL.B  (A6)                       ;*OPRD ERROR
 
 ROXL.W  D1,D2                      ;E372
 
 ROXL.W  #IM3,D2                    ;EF52
 
 ROXL.W  D5                         ;*OPRD ERROR
 ROXL.W  A6                         ;*OPRD ERROR
 ROXL.W  (A6)                       ;E5D6
 ROXL.W  (A6)+                      ;E5DE
 ROXL.W  -(A6)                      ;E5E6
 ROXL.W  OFF16(A6)                  ;E5EE 7FFE
 ROXL.W  OFF8(A6,D5)                ;E5F6 507E
 ROXL.W  DIR16                   ;E5F8 7FFE
 ROXL.W  DIR24                      ;E5F9 007F FFFE
 ROXL.W  *+D1002(PC)                ;*OPRD ERROR
 ROXL.W  *+D80(PC,D5)               ;*OPRD ERROR
 ROXL.W  #IM8                       ;*OPRD ERROR
 
 ROXL.L  D1,D2                      ;E3B2
 
 ROXL.L  #IM3,D2                    ;EF92
 
 ROXL.L  (A6)                       ;*OPRD ERROR
 
 
 ROXR    D1,D2                      ;E272
 
 ROXR    #IM3,D2                    ;EE52
 
 ROXR    #0,D2                      ;*OPRD ERROR
 ROXR    #9,D2                      ;*OPRD ERROR
 
 ROXR    D1,(A6)                    ;*OPRD ERROR
 ROXR    #IM3,(A6)                  ;*OPRD ERROR
 
 ROXR    D5                         ;*OPRD ERROR
 ROXR    A6                         ;*OPRD ERROR
 ROXR    (A6)                       ;E4D6
 ROXR    (A6)+                      ;E4DE
 ROXR    -(A6)                      ;E4E6
 ROXR    OFF16(A6)                  ;E4EE 7FFE
 ROXR    OFF8(A6,D5)                ;E4F6 507E
 ROXR    DIR16                   ;E4F8 7FFE
 ROXR    DIR24                      ;E4F9 007F FFFE
 ROXR    *+D1002(PC)                ;*OPRD ERROR
 ROXR    *+D80(PC,D5)               ;*OPRD ERROR
 ROXR    #IM8                       ;*OPRD ERROR
 
 ROXR.B  D1,D2                      ;E232
 
 ROXR.B  #IM3,D2                    ;EE12
 
 ROXR.B  (A6)                       ;*OPRD ERROR
 
 ROXR.W  D1,D2                      ;E272
 
 ROXR.W  #IM3,D2                    ;EE52
 
 ROXR.W  D5                         ;*OPRD ERROR
 ROXR.W  A6                         ;*OPRD ERROR
 ROXR.W  (A6)                       ;E4D6
 ROXR.W  (A6)+                      ;E4DE
 ROXR.W  -(A6)                      ;E4E6
 ROXR.W  OFF16(A6)                  ;E4EE 7FFE
 ROXR.W  OFF8(A6,D5)                ;E4F6 507E
 ROXR.W  DIR16                   ;E4F8 7FFE
 ROXR.W  DIR24                      ;E4F9 007F FFFE
 ROXR.W  *+D1002(PC)                ;*OPRD ERROR
 ROXR.W  *+D80(PC,D5)               ;*OPRD ERROR
 ROXR.W  #IM8                       ;*OPRD ERROR
 
 ROXR.L  D1,D2                      ;E2B2
 
 ROXR.L  #IM3,D2                    ;EE92
 
 ROXR.L  (A6)                       ;*OPRD ERROR
 
 
 RTE                                ;4E73
 
 
 RTR                                ;4E77
 
 
 RTS                                ;4E75
 
 
 SBCD    D2,D1                      ;8302
 SBCD    -(A4),-(A3)                ;870C
 
 SBCD    (A6),D1                    ;*OPRD ERROR
 
 SBCD.B  D2,D1                      ;8302
 SBCD.B  -(A4),-(A3)                ;870C
 
 SBCD.W  D2,D1                      ;*MNEM ERROR
 SBCD.L  D2,D1                      ;*MNEM ERROR
 
 ST      D5                         ;50C5
 SF      D5                         ;51C5
 SHI     D5                         ;52C5
 SLS     D5                         ;53C5
 SCC     D5                         ;54C5
 SHS     D5                         ;54C5
 SCS     D5                         ;55C5
 SLO     D5                         ;55C5
 SNE     D5                         ;56C5
 SNZ     D5                         ;56C5
 SEQ     D5                         ;57C5
 SZE     D5                         ;57C5
 SVC     D5                         ;58C5
 SVS     D5                         ;59C5
 SPL     D5                         ;5AC5
 SMI     D5                         ;5BC5
 SGE     D5                         ;5CC5
 SLT     D5                         ;5DC5
 SGT     D5                         ;5EC5
 SLE     D5                         ;5FC5
 
 
 ST      D5                         ;50C5
 ST      A6                         ;*OPRD ERROR
 ST      (A6)                       ;50D6
 ST      (A6)+                      ;50DE
 ST      -(A6)                      ;50E6
 ST      OFF16(A6)                  ;50EE 7FFE
 ST      OFF8(A6,D5)                ;50F6 507E
 ST      DIR16                   ;50F8 7FFE
 ST      DIR24                      ;50F9 007F FFFE
 ST      *+D1002(PC)                ;*OPRD ERROR
 ST      *+D80(PC,D5)               ;*OPRD ERROR
 ST      #IM8                       ;*OPRD ERROR
 
 ST.B    D5                         ;50C5
 ST.B    A6                         ;*OPRD ERROR
 ST.B    (A6)                       ;50D6
 ST.B    (A6)+                      ;50DE
 ST.B    -(A6)                      ;50E6
 ST.B    OFF16(A6)                  ;50EE 7FFE
 ST.B    OFF8(A6,D5)                ;50F6 507E
 ST.B    DIR16                   ;50F8 7FFE
 ST.B    DIR24                      ;50F9 007F FFFE
 ST.B    *+D1002(PC)                ;*OPRD ERROR
 ST.B    *+D80(PC,D5)               ;*OPRD ERROR
 ST.B    #IM8                       ;*OPRD ERROR
 
 SF.W    D5                         ;*MNEM ERROR
 
 SF.L    D5                         ;*MNEM ERROR
 
 
 STOP     #DIR16                    ;4E72 7FFE
 
 STOP    #IM32                      ;*OPRD ERROR
 
 
 SUB     D5,D1                      ;9245
 SUB     A6,D1                      ;924E
 SUB     (A6),D1                    ;9256
 SUB     (A6)+,D1                   ;925E
 SUB     -(A6),D1                   ;9266
 SUB     OFF16(A6),D1               ;926E 7FFE
 SUB     OFF8(A6,D5),D1             ;9276 507E
 SUB     DIR16,D1                ;9278 7FFE
 SUB     DIR24,D1                   ;9279 007F FFFE
 SUB     *+D1002(PC),D1             ;927A 1000
 SUB     *+D80(PC,D5),D1            ;927B 507E
 SUB     #IM8,D1                    ;0441 (927C) 007E
 
 SUB     D1,D5                      ;9A41
 SUB     D1,A6                      ;9CC1
 SUB     D1,(A6)                    ;9356
 SUB     D1,(A6)+                   ;935E
 SUB     D1,-(A6)                   ;9366
 SUB     D1,OFF16(A6)               ;936E 7FFE
 SUB     D1,OFF8(A6,D5)             ;9376 507E
 SUB     D1,DIR16                ;9378 7FFE
 SUB     D1,DIR24                   ;9379 007F FFFE
 SUB     D1,*+D1002(PC)             ;*OPRD ERROR
 SUB     D1,*+D80(PC,D5)            ;*OPRD ERROR
 SUB     D1,#IM8                    ;*OPRD ERROR
 
 SUB.B   D5,D1                      ;9205
 SUB.B   A6,D1                      ;*OPRD ERROR
 SUB.B   (A6),D1                    ;9216
 SUB.B   (A6)+,D1                   ;921E
 SUB.B   -(A6),D1                   ;9226
 SUB.B   OFF16(A6),D1               ;922E 7FFE
 SUB.B   OFF8(A6,D5),D1             ;9236 507E
 SUB.B   DIR16,D1                ;9238 7FFE
 SUB.B   DIR24,D1                   ;9239 007F FFFE
 SUB.B   *+D1002(PC),D1             ;923A 1000
 SUB.B   *+D80(PC,D5),D1            ;923B 507E
 SUB.B   #IM8,D1                    ;0401 (923C) 007E
 
 SUB.B   D1,D5                      ;9A01
 SUB.B   D1,A6                      ;*OPRD ERROR
 SUB.B   D1,(A6)                    ;9316
 SUB.B   D1,(A6)+                   ;931E
 SUB.B   D1,-(A6)                   ;9326
 SUB.B   D1,OFF16(A6)               ;932E 7FFE
 SUB.B   D1,OFF8(A6,D5)             ;9336 507E
 SUB.B   D1,DIR16                ;9338 7FFE
 SUB.B   D1,DIR24                   ;9339 007F FFFE
 SUB.B   D1,*+D1002(PC)             ;*OPRD ERROR
 SUB.B   D1,*+D80(PC,D5)            ;*OPRD ERROR
 SUB.B   D1,#IM8                    ;*OPRD ERROR
 
 SUB.W   D5,D1                      ;9245
 SUB.W   A6,D1                      ;924E
 SUB.W   (A6),D1                    ;9256
 SUB.W   (A6)+,D1                   ;925E
 SUB.W   -(A6),D1                   ;9266
 SUB.W   OFF16(A6),D1               ;926E 7FFE
 SUB.W   OFF8(A6,D5),D1             ;9276 507E
 SUB.W   DIR16,D1                ;9278 7FFE
 SUB.W   DIR24,D1                   ;9279 007F FFFE
 SUB.W   *+D1002(PC),D1             ;927A 1000
 SUB.W   *+D80(PC,D5),D1            ;927B 507E
 SUB.W   #IM8,D1                    ;0441 (927C) 007E
 
 SUB.W   D1,D5                      ;9A41
 SUB.W   D1,A6                      ;9CC1
 SUB.W   D1,(A6)                    ;9356
 SUB.W   D1,(A6)+                   ;935E
 SUB.W   D1,-(A6)                   ;9366
 SUB.W   D1,OFF16(A6)               ;936E 7FFE
 SUB.W   D1,OFF8(A6,D5)             ;9376 507E
 SUB.W   D1,DIR16                ;9378 7FFE
 SUB.W   D1,DIR24                   ;9379 007F FFFE
 SUB.W   D1,*+D1002(PC)             ;*OPRD ERROR
 SUB.W   D1,*+D80(PC,D5)            ;*OPRD ERROR
 SUB.W   D1,#IM8                    ;*OPRD ERROR
 
 SUB.L   D5,D1                      ;9285
 SUB.L   A6,D1                      ;928E
 SUB.L   (A6),D1                    ;9296
 SUB.L   (A6)+,D1                   ;929E
 SUB.L   -(A6),D1                   ;92A6
 SUB.L   OFF16(A6),D1               ;92AE 7FFE
 SUB.L   OFF8(A6,D5),D1             ;92B6 507E
 SUB.L   DIR16,D1                ;92B8 7FFE
 SUB.L   DIR24,D1                   ;92B9 007F FFFE
 SUB.L   *+D1002(PC),D1             ;92BA 1000
 SUB.L   *+D80(PC,D5),D1            ;92BB 507E
 SUB.L   #IM8,D1                    ;0481 (92BC) 0000 007E
 
 SUB.L   D1,D5                      ;9A81
 SUB.L   D1,A6                      ;9DC1
 SUB.L   D1,(A6)                    ;9396
 SUB.L   D1,(A6)+                   ;939E
 SUB.L   D1,-(A6)                   ;93A6
 SUB.L   D1,OFF16(A6)               ;93AE 7FFE
 SUB.L   D1,OFF8(A6,D5)             ;93B6 507E
 SUB.L   D1,DIR16                ;93B8 7FFE
 SUB.L   D1,DIR24                   ;93B9 007F FFFE
 SUB.L   D1,*+D1002(PC)             ;*OPRD ERROR
 SUB.L   D1,*+D80(PC,D5)            ;*OPRD ERROR
 SUB.L   D1,#IM8                    ;*OPRD ERROR
 
 
 SUBA    D5,A3                      ;96C5
 SUBA    A6,A3                      ;96CE
 SUBA    (A6),A3                    ;96D6
 SUBA    (A6)+,A3                   ;96DE
 SUBA    -(A6),A3                   ;96E6
 SUBA    OFF16(A6),A3               ;96EE 7FFE
 SUBA    OFF8(A6,D5),A3             ;96F6 507E
 SUBA    DIR16,A3                ;96F8 7FFE
 SUBA    DIR24,A3                   ;96F9 007F FFFE
 SUBA    *+D1002(PC),A3             ;96FA 1000
 SUBA    *+D80(PC,D5),A3            ;96FB 507E
 SUBA    #IM8,A3                    ;96FC 007E
 
 SUBA.W  D5,A3                      ;96C5
 SUBA.W  A6,A3                      ;96CE
 SUBA.W  (A6),A3                    ;96D6
 SUBA.W  (A6)+,A3                   ;96DE
 SUBA.W  -(A6),A3                   ;96E6
 SUBA.W  OFF16(A6),A3               ;96EE 7FFE
 SUBA.W  OFF8(A6,D5),A3             ;96F6 507E
 SUBA.W  DIR16,A3                ;96F8 7FFE
 SUBA.W  DIR24,A3                   ;96F9 007F FFFE
 SUBA.W  *+D1002(PC),A3             ;96FA 1000
 SUBA.W  *+D80(PC,D5),A3            ;96FB 507E
 SUBA.W  #IM8,A3                    ;96FC 007E
 
 SUBA.L  D5,A3                      ;97C5
 SUBA.L  A6,A3                      ;97CE
 SUBA.L  (A6),A3                    ;97D6
 SUBA.L  (A6)+,A3                   ;97DE
 SUBA.L  -(A6),A3                   ;97E6
 SUBA.L  OFF16(A6),A3               ;97EE 7FFE
 SUBA.L  OFF8(A6,D5),A3             ;97F6 507E
 SUBA.L  DIR16,A3                ;97F8 7FFE
 SUBA.L  DIR24,A3                   ;97F9 007F FFFE
 SUBA.L  *+D1002(PC),A3             ;97FA 1000
 SUBA.L  *+D80(PC,D5),A3            ;97FB 507E
 SUBA.L  #IM8,A3                    ;97FC 0000 007E
 
 SUBA.B  D5,A3                      ;*MNEM ERROR
 
 
 SUB     D5,A3                      ;96C5
 SUB     A6,A3                      ;96CE
 SUB     (A6),A3                    ;96D6
 SUB     (A6)+,A3                   ;96DE
 SUB     -(A6),A3                   ;96E6
 SUB     OFF16(A6),A3               ;96EE 7FFE
 SUB     OFF8(A6,D5),A3             ;96F6 507E
 SUB     DIR16,A3                ;96F8 7FFE
 SUB     DIR24,A3                   ;96F9 007F FFFE
 SUB     *+D1002(PC),A3             ;96FA 1000
 SUB     *+D80(PC,D5),A3            ;96FB 507E
 SUB     #IM8,A3                    ;96FC 007E
 
 SUB.W   D5,A3                      ;96C5
 SUB.W   A6,A3                      ;96CE
 SUB.W   (A6),A3                    ;96D6
 SUB.W   (A6)+,A3                   ;96DE
 SUB.W   -(A6),A3                   ;96E6
 SUB.W   OFF16(A6),A3               ;96EE 7FFE
 SUB.W   OFF8(A6,D5),A3             ;96F6 507E
 SUB.W   DIR16,A3                ;96F8 7FFE
 SUB.W   DIR24,A3                   ;96F9 007F FFFE
 SUB.W   *+D1002(PC),A3             ;96FA 1000
 SUB.W   *+D80(PC,D5),A3            ;96FB 507E
 SUB.W   #IM8,A3                    ;96FC 007E
 
 SUB.L   D5,A3                      ;97C5
 SUB.L   A6,A3                      ;97CE
 SUB.L   (A6),A3                    ;97D6
 SUB.L   (A6)+,A3                   ;97DE
 SUB.L   -(A6),A3                   ;97E6
 SUB.L   OFF16(A6),A3               ;97EE 7FFE
 SUB.L   OFF8(A6,D5),A3             ;97F6 507E
 SUB.L   DIR16,A3                ;97F8 7FFE
 SUB.L   DIR24,A3                   ;97F9 007F FFFE
 SUB.L   *+D1002(PC),A3             ;97FA 1000
 SUB.L   *+D80(PC,D5),A3            ;97FB 507E
 SUB.L   #IM8,A3                    ;97FC 0000 007E
 
 SUB.B   D5,A3                      ;*OPRD ERROR
 
 
 SUBI    #IM8,D5                    ;0445 007E
 SUBI    #IM8,A6                    ;*OPRD ERROR
 SUBI    #IM8,(A6)                  ;0456 007E
 SUBI    #IM8,(A6)+                 ;045E 007E
 SUBI    #IM8,-(A6)                 ;0466 007E
 SUBI    #IM8,OFF16(A6)             ;046E 007E 7FFE
 SUBI    #IM8,OFF8(A6,D5)           ;0476 007E 507E
 SUBI    #IM8,DIR16              ;0478 007E 7FFE
 SUBI    #IM8,DIR24                 ;0479 007E 007F FFFE
 SUBI    #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUBI    #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUBI    #IM8,#IM8                  ;*OPRD ERROR
 
 SUBI.B  #IM8,D5                    ;0405 007E
 SUBI.B  #IM8,A6                    ;*OPRD ERROR
 SUBI.B  #IM8,(A6)                  ;0416 007E
 SUBI.B  #IM8,(A6)+                 ;041E 007E
 SUBI.B  #IM8,-(A6)                 ;0426 007E
 SUBI.B  #IM8,OFF16(A6)             ;042E 007E 7FFE
 SUBI.B  #IM8,OFF8(A6,D5)           ;0436 007E 507E
 SUBI.B  #IM8,DIR16              ;0438 007E 7FFE
 SUBI.B  #IM8,DIR24                 ;0439 007E 007F FFFE
 SUBI.B  #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUBI.B  #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUBI.B  #IM8,#IM8                  ;*OPRD ERROR
 
 SUBI.W  #IM8,D5                    ;0445 007E
 SUBI.W  #IM8,A6                    ;*OPRD ERROR
 SUBI.W  #IM8,(A6)                  ;0456 007E
 SUBI.W  #IM8,(A6)+                 ;045E 007E
 SUBI.W  #IM8,-(A6)                 ;0466 007E
 SUBI.W  #IM8,OFF16(A6)             ;046E 007E 7FFE
 SUBI.W  #IM8,OFF8(A6,D5)           ;0476 007E 507E
 SUBI.W  #IM8,DIR16              ;0478 007E 7FFE
 SUBI.W  #IM8,DIR24                 ;0479 007E 007F FFFE
 SUBI.W  #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUBI.W  #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUBI.W  #IM8,#IM8                  ;*OPRD ERROR
 
 SUBI.L  #IM8,D5                    ;0485 0000 007E
 SUBI.L  #IM8,A6                    ;*OPRD ERROR
 SUBI.L  #IM8,(A6)                  ;0496 0000 007E
 SUBI.L  #IM8,(A6)+                 ;049E 0000 007E
 SUBI.L  #IM8,-(A6)                 ;04A6 0000 007E
 SUBI.L  #IM8,OFF16(A6)             ;04AE 0000 007E 7FFE
 SUBI.L  #IM8,OFF8(A6,D5)           ;04B6 0000 007E 507E
 SUBI.L  #IM8,DIR16              ;04B8 0000 007E 7FFE
 SUBI.L  #IM8,DIR24                 ;04B9 0000 007E 007F FFFE
 SUBI.L  #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUBI.L  #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUBI.L  #IM8,#IM8                  ;*OPRD ERROR
 
 
 SUB     #IM8,D5                    ;0445 (9A7C) 007E
 SUB     #IM8,A6                    ;9CFC 007E
 SUB     #IM8,(A6)                  ;0456 007E
 SUB     #IM8,(A6)+                 ;045E 007E
 SUB     #IM8,-(A6)                 ;0466 007E
 SUB     #IM8,OFF16(A6)             ;046E 007E 7FFE
 SUB     #IM8,OFF8(A6,D5)           ;0476 007E 507E
 SUB     #IM8,DIR16              ;0478 007E 7FFE
 SUB     #IM8,DIR24                 ;0479 007E 007F FFFE
 SUB     #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUB     #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUB     #IM8,#IM8                  ;*OPRD ERROR
 
 SUB.B   #IM8,D5                    ;0405 (9A3C) 007E
 SUB.B   #IM8,A6                    ;*OPRD ERROR
 SUB.B   #IM8,(A6)                  ;0416 007E
 SUB.B   #IM8,(A6)+                 ;041E 007E
 SUB.B   #IM8,-(A6)                 ;0426 007E
 SUB.B   #IM8,OFF16(A6)             ;042E 007E 7FFE
 SUB.B   #IM8,OFF8(A6,D5)           ;0436 007E 507E
 SUB.B   #IM8,DIR16              ;0438 007E 7FFE
 SUB.B   #IM8,DIR24                 ;0439 007E 007F FFFE
 SUB.B   #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUB.B   #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUB.B   #IM8,#IM8                  ;*OPRD ERROR
 
 SUB.W   #IM8,D5                    ;0445 (9A7C) 007E
 SUB.W   #IM8,A6                    ;9CFC 007E
 SUB.W   #IM8,(A6)                  ;0456 007E
 SUB.W   #IM8,(A6)+                 ;045E 007E
 SUB.W   #IM8,-(A6)                 ;0466 007E
 SUB.W   #IM8,OFF16(A6)             ;046E 007E 7FFE
 SUB.W   #IM8,OFF8(A6,D5)           ;0476 007E 507E
 SUB.W   #IM8,DIR16              ;0478 007E 7FFE
 SUB.W   #IM8,DIR24                 ;0479 007E 007F FFFE
 SUB.W   #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUB.W   #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUB.W   #IM8,#IM8                  ;*OPRD ERROR
 
 SUB.L   #IM8,D5                    ;0485 (9ABC) 0000 007E
 SUB.L   #IM8,A6                    ;9DFC 0000 007E
 SUB.L   #IM8,(A6)                  ;0496 0000 007E
 SUB.L   #IM8,(A6)+                 ;049E 0000 007E
 SUB.L   #IM8,-(A6)                 ;04A6 0000 007E
 SUB.L   #IM8,OFF16(A6)             ;04AE 0000 007E 7FFE
 SUB.L   #IM8,OFF8(A6,D5)           ;04B6 0000 007E 507E
 SUB.L   #IM8,DIR16              ;04B8 0000 007E 7FFE
 SUB.L   #IM8,DIR24                 ;04B9 0000 007E 007F FFFE
 SUB.L   #IM8,*+D1002(PC)           ;*OPRD ERROR
 SUB.L   #IM8,*+D80(PC,D5)          ;*OPRD ERROR
 SUB.L   #IM8,#IM8                  ;*OPRD ERROR
 
 SUBQ    #IM3,D5                    ;5F45
 SUBQ    #IM3,A6                    ;5F4E
 SUBQ    #IM3,(A6)                  ;5F56
 SUBQ    #IM3,(A6)+                 ;5F5E
 SUBQ    #IM3,-(A6)                 ;5F66
 SUBQ    #IM3,OFF16(A6)             ;5F6E 7FFE
 SUBQ    #IM3,OFF8(A6,D5)           ;5F76 507E
 SUBQ    #IM3,DIR16              ;5F78 7FFE
 SUBQ    #IM3,DIR24                 ;5F79 007F FFFE
 SUBQ    #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUBQ    #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUBQ    #IM3,#IM8                  ;*OPRD ERROR
 
 SUBQ.B  #IM3,D5                    ;5F05
 SUBQ.B  #IM3,A6                    ;*? OPRD ERROR          !!!
 SUBQ.B  #IM3,(A6)                  ;5F16
 SUBQ.B  #IM3,(A6)+                 ;5F1E
 SUBQ.B  #IM3,-(A6)                 ;5F26
 SUBQ.B  #IM3,OFF16(A6)             ;5F2E 7FFE
 SUBQ.B  #IM3,OFF8(A6,D5)           ;5F36 507E
 SUBQ.B  #IM3,DIR16              ;5F38 7FFE
 SUBQ.B  #IM3,DIR24                 ;5F39 007F FFFE
 SUBQ.B  #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUBQ.B  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUBQ.B  #IM3,#IM8                  ;*OPRD ERROR
 
 SUBQ.W  #IM3,D5                    ;5F45
 SUBQ.W  #IM3,A6                    ;5F4E
 SUBQ.W  #IM3,(A6)                  ;5F56
 SUBQ.W  #IM3,(A6)+                 ;5F5E
 SUBQ.W  #IM3,-(A6)                 ;5F66
 SUBQ.W  #IM3,OFF16(A6)             ;5F6E 7FFE
 SUBQ.W  #IM3,OFF8(A6,D5)           ;5F76 507E
 SUBQ.W  #IM3,DIR16              ;5F78 7FFE
 SUBQ.W  #IM3,DIR24                 ;5F79 007F FFFE
 SUBQ.W  #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUBQ.W  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUBQ.W  #IM3,#IM8                  ;*OPRD ERROR
 
 SUBQ.L  #IM3,D5                    ;5F85
 SUBQ.L  #IM3,A6                    ;5F8E
 SUBQ.L  #IM3,(A6)                  ;5F96
 SUBQ.L  #IM3,(A6)+                 ;5F9E
 SUBQ.L  #IM3,-(A6)                 ;5FA6
 SUBQ.L  #IM3,OFF16(A6)             ;5FAE 7FFE
 SUBQ.L  #IM3,OFF8(A6,D5)           ;5FB6 507E
 SUBQ.L  #IM3,DIR16              ;5FB8 7FFE
 SUBQ.L  #IM3,DIR24                 ;5FB9 007F FFFE
 SUBQ.L  #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUBQ.L  #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUBQ.L  #IM3,#IM8                  ;*OPRD ERROR
 
 
 SUBQ    #0,D5                      ;*OPRD ERROR
 SUBQ    #9,D5                      ;*OPRD ERROR
 
 
 SUB     #IM3,D5                    ;5F45
 SUB     #IM3,A6                    ;5F4E
 SUB     #IM3,(A6)                  ;5F56
 SUB     #IM3,(A6)+                 ;5F5E
 SUB     #IM3,-(A6)                 ;5F66
 SUB     #IM3,OFF16(A6)             ;5F6E 7FFE
 SUB     #IM3,OFF8(A6,D5)           ;5F76 507E
 SUB     #IM3,DIR16              ;5F78 7FFE
 SUB     #IM3,DIR24                 ;5F79 007F FFFE
 SUB     #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUB     #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUB     #IM3,#IM8                  ;*OPRD ERROR
 
 SUB.B   #IM3,D5                    ;5F05
 SUB.B   #IM3,A6                    ;*? OPRD ERROR          !!!
 SUB.B   #IM3,(A6)                  ;5F16
 SUB.B   #IM3,(A6)+                 ;5F1E
 SUB.B   #IM3,-(A6)                 ;5F26
 SUB.B   #IM3,OFF16(A6)             ;5F2E 7FFE
 SUB.B   #IM3,OFF8(A6,D5)           ;5F36 507E
 SUB.B   #IM3,DIR16              ;5F38 7FFE
 SUB.B   #IM3,DIR24                 ;5F39 007F FFFE
 SUB.B   #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUB.B   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUB.B   #IM3,#IM8                  ;*OPRD ERROR
 
 SUB.W   #IM3,D5                    ;5F45
 SUB.W   #IM3,A6                    ;5F4E
 SUB.W   #IM3,(A6)                  ;5F56
 SUB.W   #IM3,(A6)+                 ;5F5E
 SUB.W   #IM3,-(A6)                 ;5F66
 SUB.W   #IM3,OFF16(A6)             ;5F6E 7FFE
 SUB.W   #IM3,OFF8(A6,D5)           ;5F76 507E
 SUB.W   #IM3,DIR16              ;5F78 7FFE
 SUB.W   #IM3,DIR24                 ;5F79 007F FFFE
 SUB.W   #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUB.W   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUB.W   #IM3,#IM8                  ;*OPRD ERROR
 
 SUB.L   #IM3,D5                    ;5F85
 SUB.L   #IM3,A6                    ;5F8E
 SUB.L   #IM3,(A6)                  ;5F96
 SUB.L   #IM3,(A6)+                 ;5F9E
 SUB.L   #IM3,-(A6)                 ;5FA6
 SUB.L   #IM3,OFF16(A6)             ;5FAE 7FFE
 SUB.L   #IM3,OFF8(A6,D5)           ;5FB6 507E
 SUB.L   #IM3,DIR16              ;5FB8 7FFE
 SUB.L   #IM3,DIR24                 ;5FB9 007F FFFE
 SUB.L   #IM3,*+D1002(PC)           ;*OPRD ERROR
 SUB.L   #IM3,*+D80(PC,D5)          ;*OPRD ERROR
 SUB.L   #IM3,#IM8                  ;*OPRD ERROR
 
 
 SUBX    D2,D1                      ;9342
 SUBX    -(A4),-(A3)                ;974C
 
 SUBX    (A6),D1                    ;*OPRD ERROR
 
 SUBX.B  D2,D1                      ;9302
 SUBX.B  -(A4),-(A3)                ;970C
 
 SUBX.W  D2,D1                      ;9342
 SUBX.W  -(A4),-(A3)                ;974C
 
 SUBX.L  D2,D1                      ;9382
 SUBX.L  -(A4),-(A3)                ;978C
 
 
 SWAP    D1                         ;4841
 
 SWAP    (A6)                       ;*OPRD ERROR
 
 SWAP.B  D1                         ;*MNEM ERROR
 
 SWAP.W  D1                         ;4841
 
 SWAP.L  D1                         ;*MNEM ERROR
 
 
 TAS     D5                         ;4AC5
 TAS     A6                         ;*OPRD ERROR
 TAS     (A6)                       ;4AD6
 TAS     (A6)+                      ;4ADE
 TAS     -(A6)                      ;4AE6
 TAS     OFF16(A6)                  ;4AEE 7FFE
 TAS     OFF8(A6,D5)                ;4AF6 507E
 TAS     DIR16                   ;4AF8 7FFE
 TAS     DIR24                      ;4AF9 007F FFFE
 TAS     *+D1002(PC)                ;*OPRD ERROR
 TAS     *+D80(PC,D5)               ;*OPRD ERROR
 TAS     #IM8                       ;*OPRD ERROR
 
 TAS.B   D5                         ;4AC5
 TAS.B   A6                         ;*OPRD ERROR
 TAS.B   (A6)                       ;4AD6
 TAS.B   (A6)+                      ;4ADE
 TAS.B   -(A6)                      ;4AE6
 TAS.B   OFF16(A6)                  ;4AEE 7FFE
 TAS.B   OFF8(A6,D5)                ;4AF6 507E
 TAS.B   DIR16                   ;4AF8 7FFE
 TAS.B   DIR24                      ;4AF9 007F FFFE
 TAS.B   *+D1002(PC)                ;*OPRD ERROR
 TAS.B   *+D80(PC,D5)               ;*OPRD ERROR
 TAS.B   #IM8                       ;*OPRD ERROR
 
 TAS.W   D5                         ;*MNEM ERROR
 
 TAS.L   D5                         ;*MNEM ERROR
 
 
 TRAP    #IM3                       ;4E47
 
 TRAP    #IM8                       ;*OPRD ERROR
 
 
 TRAPV                              ;4E76
 
 
 TST     D5                         ;4A45
 TST     A6                         ;*OPRD ERROR
 TST     (A6)                       ;4A56
 TST     (A6)+                      ;4A5E
 TST     -(A6)                      ;4A66
 TST     OFF16(A6)                  ;4A6E 7FFE
 TST     OFF8(A6,D5)                ;4A76 507E
 TST     DIR16                   ;4A78 7FFE
 TST     DIR24                      ;4A79 007F FFFE
 TST     *+D1002(PC)                ;*OPRD ERROR
 TST     *+D80(PC,D5)               ;*OPRD ERROR
 TST     #IM8                       ;*OPRD ERROR
 
 TST.B   D5                         ;4A05
 TST.B   A6                         ;*OPRD ERROR
 TST.B   (A6)                       ;4A16
 TST.B   (A6)+                      ;4A1E
 TST.B   -(A6)                      ;4A26
 TST.B   OFF16(A6)                  ;4A2E 7FFE
 TST.B   OFF8(A6,D5)                ;4A36 507E
 TST.B   DIR16                   ;4A38 7FFE
 TST.B   DIR24                      ;4A39 007F FFFE
 TST.B   *+D1002(PC)                ;*OPRD ERROR
 TST.B   *+D80(PC,D5)               ;*OPRD ERROR
 TST.B   #IM8                       ;*OPRD ERROR
 
 TST.W   D5                         ;4A45
 TST.W   A6                         ;*OPRD ERROR
 TST.W   (A6)                       ;4A56
 TST.W   (A6)+                      ;4A5E
 TST.W   -(A6)                      ;4A66
 TST.W   OFF16(A6)                  ;4A6E 7FFE
 TST.W   OFF8(A6,D5)                ;4A76 507E
 TST.W   DIR16                   ;4A78 7FFE
 TST.W   DIR24                      ;4A79 007F FFFE
 TST.W   *+D1002(PC)                ;*OPRD ERROR
 TST.W   *+D80(PC,D5)               ;*OPRD ERROR
 TST.W   #IM8                       ;*OPRD ERROR
 
 TST.L   D5                         ;4A85
 TST.L   A6                         ;*OPRD ERROR
 TST.L   (A6)                       ;4A96
 TST.L   (A6)+                      ;4A9E
 TST.L   -(A6)                      ;4AA6
 TST.L   OFF16(A6)                  ;4AAE 7FFE
 TST.L   OFF8(A6,D5)                ;4AB6 507E
 TST.L   DIR16                   ;4AB8 7FFE
 TST.L   DIR24                      ;4AB9 007F FFFE
 TST.L   *+D1002(PC)                ;*OPRD ERROR
 TST.L   *+D80(PC,D5)               ;*OPRD ERROR
 TST.L   #IM8                       ;*OPRD ERROR
 
 
 UNLK    A3                         ;4E5B
 
 UNLK    D5                         ;*OPRD ERROR
 
 
*
*DC AND NUMBER FORMS - IGNORE DISASSEMBLY ERRORS
*
 
 DC      0                          ;0000
 DC      0                          ;0000
 DC      0                          ;0000
 DC      1,1                        ;0001 0001
 DC      -1,-1                      ;FFFF FFFF
 DC      $7FFF,32767                ;7FFF 7FFF
 DC      $8000,-$8000               ;8000 8000
 DC      32768,-32768               ;8000 8000
 DC      $0FFFF,65535               ;FFFF FFFF
 
 DC.B    0,0                        ;0000
 DC.B    1,1                        ;0101
 DC.B    -1,-1                      ;FFFF
 DC.B    $7F,127                    ;7F7F
 DC.B    $80,-$80                   ;8080
 DC.B    128,-128                   ;8080
 DC.B    $0FF,255                   ;FFFF
 DC.B    1                          ;01
 
 DC.B    $100,256                   ;*? OPRD ERROR          !!!
 
 DC.W    $10000,65536               ;*? OPRD ERROR          !!!
 
 DC.L    0                          ;0000 0000
 DC.L    0                          ;0000 0000
 DC.L    1                          ;0000 0001
 DC.L    1                          ;0000 0001
 DC.L    -1                         ;FFFF FFFF
 DC.L    -1                         ;FFFF FFFF
 DC.L    $7FFFFFFF                  ;7FFF FFFF
 DC.L    2147483647                 ;7FFF FFFF
 
 DC.L    $8000000                   ;*? OPRD ERROR          !!!
 DC.L    2147483648                 ;*? OPRD ERROR          !!!
 
 DC.B    1,2,3,4,5,6,7,8,9,10       ;0102 0304 0506 0708 090A
 DC.W    1,2,3,4,5                  ;0001 0002 0003 0004 0005
 DC.L    1,2                        ;0000 0001 0000 0002
 
 DC.B    'ABCDEFGH'
 DC.W    'ABCDEFGH'
 DC.L    'ABCDEFGH'

 DC.B    'THIS IS A STRING'
 DC.W    'THIS IS A STRING'
 DC.L    'THIS IS A STRING' 
 
 
* ENDSEC
 
 END
