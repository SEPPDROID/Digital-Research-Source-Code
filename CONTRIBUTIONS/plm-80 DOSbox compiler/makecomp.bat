Rem compile using Borland's freely downloadable compiler
Rem This setup created by mr.collis@physics.org 31/03/2008
bcc32 -O2 plm81.c bccf2c.lib
bcc32 -O2 plm82.c bccf2c.lib
del *.tds
del *.obj
