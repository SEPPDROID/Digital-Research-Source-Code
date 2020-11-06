Rem This setup created by mr.collis@physics.org 31/03/2008
copy %1.plm fort.2
echo rubbish  > fort.1
plm81

rename fort.16 fort.4
rename fort.17 fort.7
plm82
copy fort.17 %1.hex

