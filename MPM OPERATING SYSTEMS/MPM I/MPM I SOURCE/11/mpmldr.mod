 MPMLDR  
�  S  LDMON1 LDMON2 K� �	@P0014 �	@P0015 �	@P0016 �	@P0017 �	@P0018 �	@P0019 �	@P0034 �	@P0035 �	@P0086 �	@P0087 �	@P0088 �	@P0091 �	@P0092 �	@P0093 �	@P0098 �	@P0099 �	@P0100 
@P0101 
@P0102 
@P0103 
@P0104 F MPMLDR   MEMORY �E $START   
JMPTOSTART  	COPYRIGHT  COMPANYNAME ( SERIALNUMBER �   	ERRMSGADR -" . DEFAULTERRMSG "SYSTEMRESET #  DUMMY N -WRITECONSOLE 
  CHAR � =PRINTBUFFER J  BUFFERADDRESS ! MRESETDISKSYSTEM VOPENFILE '  
FCBADDRESS � f
READRECORD � 	 
FCBADDRESS � vSETDMAADDRESS �  
DMAADDRESS � �CRLF �PRINTNIB !  N � �PRINTHEX �  B � �	PRINTADDR Y  A � �	PRINTNAME -  NADR  I F !
PRINTITEMS �  NADR  BASE  SIZE 	 NMATCHSERIAL R�  CPYRTADR  MEMADR  I  J   BASE " CURTOP $ PREVTOP & 	SYSDATADR ( NREC ) MASK * RET + OFFSET , NOTDONE - I / J 1 FCB R BANKEDBDOSFCB s 
ENTRYPOINT �    HEADER  CODESIZE  DATASIZE  !
SYSTEMDATA  !MEMTOP !NMBCNS !BRKPTRST !SYSCALLSTKS !BANKSWITCHED !Z80CPU !
BANKEDBDOS !BASEBANKEDBDOS !	NMBMEMSEG !	MEMSEGTBL P!
USERSTACKS p!
TMPMEMSEGS �!RSPL ] 
OPTIONTEMP � y OPTION �   "BUFFER   z 	BITMAPADR � �LOADPRL  | GETNAME } FCBADR ,& �LOADBANKEDBDOS �LOADSYSTEMDATA P
  CNTR (B IDISPLAYMEMMAP �	RESTARTINSTR �	XEQMPM > BRKPTADR �ERROR q   �!$  �  COPYRIGHT (C) 1980,T   DIGITAL RESEARCH "
 ( 654321�   . �$   � . Disk read error$ @ Synchronization$/% 1  MPM     SYS                     % R  BNKBDOS SPR                     i' P Bad first record of SPR/RSP header$�( s Bad second record of SPR/RSP header$A/ � Unexpected end of file, or disk read error$� � RSP; � MPM.SYS does not exist$U � Number of consoles =  $] � 
Breakpoint RST #   =  $f 
Z80 CPU$� 
Banked BDOS file manager$@ 1
Top of memory      =$� H

Memory Segment Table:
$� cSYSTEM  DAT� nCONSOLE DATo yUSERSYS STK   �-------------------------
$� �Memseg  Usr$L �  Bank $@ > �	$ > �+ �

MP/M 1.1 Loader
===============

$� �XIOS    SPR$1 �BDOS    SPR$@ �XDOS    SPR$ �BNKBDOS SPR$� 
MP/M Loader error: $c " � " ( ,  "!  " ! �ɛ$ )&{ - �
 1 < = -! q* & ���'$ 2.j =  �
 C" L#  =! p+q* �	�ɘ$ D>H M$ }
 M% U& � M  ��� V' q
 \) f* � V! p+q* ���u$ ]W f+ ]
 l- v. � f!
 p+q*	 ���\$ mg� v/ I
 |1 �2 � v! p+q* ���B$ }w� �3 5 �4 �5 �6 � ��-
�-�k$ ��� �7 & �9 �: �; �< [) �! q>	! �Ҭ: �A�
O�-õ: �0O�-ɢ$ ����!$ ����J �= � �? �@ �A � �! q: ��O͑: �O͑�y$ ��8$	 ���� �B � �D �E �F �G �H �I �* �! p+q �- �-* |OͶ* }OͶH�-�7$ �����9$	 ���' �J � �N O P  Q W. �! p+q! 6 >
! �� * & �* N�-! 4��
$	 	�$ �� !R z 2T :U BV JW MX �1 !! r+s+p+q+��p+q�* DM��* DM��* DM��͆��$ KH@8�$ C;3"� NY F6 X^ ^_ f` �b �c �d �e �g �h �i �j �k �l �z N! r+s+p+q>���! 6�#6�: <2 O * 	: <2 O �* 	�
��f>! �Җ�>  �
�����>#���H�ҹ!@ "  ��* #" �X�.$ ��������\,$ ������yuqkg_O� �� �� �� �� �� �� �� �� �� �� �� � � � � 3� 9� K� S� ]� m� s� v� v� �� �� �� �� �� �� �� �� �� �� �� �� � � � ,� ,� 9� >� E� E� R� Y� i� � �� �� �� �� P��!~ r+s+q  �v*} DM�f2* � ��:* ���> �!P "  ��� �v*} DM�f� ���2* �!s "  ��! ��	Ͷ	 ��	�!( s! ""  ! "- :( !- �
ڃ*  DM�v� *  "  *} DM�f2* � �v!� "  �� *- "- �?*" "$ * �* �  ���	$ �
"" :| ��*" DM��" $ ��	DM��*" |2+ *  "	"z !  "/ !) 6�!  "- * +�!- �
�R*/ �*z :) �� �*-  "	:+ �*- �*" w�,*-  "	�*- �*" �w:) �2) � �E!) 6�*/ #"/  *- "- ��:| ҄."� 
-�a"�" $ ��	�*" DM�!��y�͆��yɜ$S t����F,#�}g_WP7 �����tnkcQI��������$u xqnSMIC?:2-&"
������������������~zqf^[WLC@=70
������� �� �& �� �� �� �� �� �� �� �� �� g/ �  �vR �V�� ��R  ��/Ұ��*" "!�0$ �������?$	 ���� �� Z� �� �� �� �� �� �� �� �� �� �� �� �� �� � � � � � #� +� 9� <� C� C� S� Y� `� f� m� t� z� �� � ��������	
8HHu�� !�v1 �V����!� "  ��1 �f2* � �����!�v1 �f2* � ����: !� �C! �"" ��!, q:, �<*" 6Z*" ~�Z�+!, 6 �9 " ��	�+s#r�*" |2 !* !& ��	"& "" � �=*!M͑� �=*!M͑:!�z�=:!҇�=1�=� *" DM��H�=c�*" DM �! *!& ��	"$ " �
�+s#rn�*" DM*$ ��!:!�H! 6 :!=! ��@ * & ��	" �
�* & P!)	�q#p: <2 ��:!=��<o&  ��	"$ " �
�+s#ry�*" DM*$ ��!�.$i 1(����K2F9�������������xurkda^WT:)!��������������$E B=.+����������QN=/$������( I�� IPVY^el������ �!�"�#�$�%�&�'�(�*�+	,	-/	./	/I	0g	1n	3t	4�	5�	6�	7�	8�	9�NI:!�Y:!2( î!( 6��!, q:, Ү*( & p!	�*" |O��ڤ*" |*( & p!	w:( =2( :( 2!!, 6 ë:( <2( �e��=:( � ʒ	:( =2( ��=:!/�/	*( & q!	�*( & ))!	�w*( & r!	�*( & 	��*( & ))!	 	w*( & ))!	 	6 *( & ))!	 	6 *( & ))!	N `i��	DM��*( & ))!	 	N `i��	DM��:!Ҍ	��=*( & ))!	 	NͶH�-͆ô��$1 `	B	�	�	�	�	r	o	l	e	G	��������jWN�$9 u	J	0			����������������wmfbZT� �	:
 �	;�	<; �	�ɛ �	= �	@�	A�	C�	D�	E�	F2  �	!w �:y �B°	:!�����*> w͓	�|$	 �	�	�	�$ �	�	�� !G%H+I.J4K7LBMOORPRQ]RjTmUmVxW�Y�Z�[�\�]�^�_�`�a�b�c�e�f�g�h�i�j�klmnopq r�!1  �:] 2y �M��=ͷ!�"  DM�=1  ��/�R��!�"  DM�=1  ��/�m��!�"  DM�=1  ��/҈�� *" 	�*& ��s#r*" "w  ! �N!  "�!1 �����*" �*�!��s#r*" "�!í:!��!�"  DM�=͌�I! �*& � !�
+|���͕	1  �=*  DM�=>�� !  ���v��vG$K ��������������~vnkhc[SPMH@852/,�$% 	���������yq^VC;)�$  "� LDMONX  LDMON1 LDMON2 O �	�_ ��o�g�� �	_ {�oz�g�� �	DM!  >)�)���		=��	ɍ$ �	�	 �	^#V�)��	ɚ$ �	� �	^#V�|�g}o��	��$ �	� �	i`N#F�o�g�Z 
o& �o�g�1 
_ {�_z#�W�ɦ !� �