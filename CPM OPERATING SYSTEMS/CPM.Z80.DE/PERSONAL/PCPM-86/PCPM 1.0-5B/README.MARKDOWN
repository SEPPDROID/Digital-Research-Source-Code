This archive represents a German issue of Digital Research's Personal CP/M-86 1.0/5b for the Siemens Simatic S5 programming device PG685. Sources are in Assembler, and have been disassembled with Sourcer v. 7.00.

/README.TXT


  Last edit: 2002-05-24 MPAUL


            Digital Research Personal CP/M-86 1.0/5b (1985)
            ===============================================
                                 for
                                 ===
                      Siemens Simatic S5 / PG685
                      ==========================


  Summary
  -------

  This archive represents a German issue of Digital Research's
  Personal CP/M-86 1.0/5b for the Siemens Simatic S5 programming
  device PG685.

  This release of PCP/M-86 is dated 1985-10-09 (or 1985-09-10?),
  and contains Digital Research copyright strings going back to
  1981, 1982, and for the most part 1983.

  Many of the .CMD files contain "CP/M-86 Plus" strings, leading
  to the assumption that Personal CP/M-86 1.x and 2.x represent
  nothing but this rumored product CP/M-86 Plus or that they are
  at least very close relatives, with Personal CP/M-86 being derived
  from CP/M-86 Plus. There are also hints that the single-user
  Personal CP/M-86 1.x and 2.x issues in some way correspond with
  the multi-user Concurrent CP/M-86 3.1.


  License
  -------

  Since 1999-07, Lineo, Inc., Utah, USA, is the copyright owner of all
  original Digital Research CP/M operating system technology by way of
  Caldera Thin Clients, Inc. (since mid 1998), Caldera, Inc. (since
  1996-07), Novell, Inc. (since 1991-07) and Digital Research, Inc.

  On 2001-10-19, Lineo's (then-times) Chairman and CEO Bryan Sparks
  issued a public CP/M distribution and usage license as detailed
  in the file LICENSE.TXT accompaning this distribution archive.

  This historic issue of CP/M is expressively made available "AS IS"
  under the terms of this license, and without warranties or support
  of any kind. Use the files and information solely at your own risk.

  However, while the meaning is pretty clear in "human language",
  the actual wording is in another sense so vague that I suggest you
  still better check with Lineo in case you plan to utilize this
  stuff for anything beyond the sole purpose of studying the CP/M
  and DOS history or personal non-commercial use.

  (Disclaimer: I am no spokesperson of Lineo, and I have no direct
  affiliation with Lineo at the time of this writing.)


  Features
  --------

  According to the documentation on the disk, this issue of PCP/M-86
  supports one 96 tpi floppy disk drive and one hard disk drive (at
  least up to 85 Mb), apparently up to 512 Kb RAM, and up to three
  background processes.

  It can read and write these floppy formats:

   PG695 / PC16-11 : 80 track floppies formatted under CCP/M-86 version 2.0/3
                     or CCP/M-86 version 3.1/1 or PCP/M-86 version 1.0/x

  It can read, but not write these formats:

   PG675           : 40 track floppies formatted under CP/M-86 version 1.1/2
                     and PCP/M-86 version 1.0/x

   IBM-PC XT       : 40 track floppies formatted under CP/M-86 since
                     version 1.1, CCP/M-86 all versions if written
                     with single- or dual-sided CP/M formats.

  I have no idea what kind of machine this "Siemens Simatic S5
  Programmierger„t PG685" was, but I assume it was some sort of
  programmable industrial control. It must have been small enough
  to be portable (whatever this means). The documentation mentions
  an optional external graphics monitor "BMG" in addition to
  the internal display. The video was capable of both, text
  and graphics mode and the documentation mentions GSX-86.


  Documentation
  -------------

  All this information provided here was leaked from reading the READ.ME
  file on the floppy disk and some examination of the binaries.

  Apparently there was a small German Siemens publication:

   "Simatic S5 - Personal CP/M-86 Betriebssystem - Tabellenheft",
   93 pages, Order No. C79000-B8500-C352-01

  And there was a comprehensive manual for Personal CP/M-86 as well,
  but I have no further info about it, unfortunately.


  File versions
  -------------

   File name:  File size:  CRC:  Date strings:            Version strings:

   BACK.CMD        15.232  EDBF  1983-11-16
   CCP.CMD          7.168  E06F  1983-11-11
   DATE.CMD         3.840  F1EC  1983-11-16
   DDT86.CMD       14.336  9DB9  1981                     1.2
   DEVICE.CMD      19.840  FEF9  1983-11-16
   DIR.CMD         11.648  4984  1983-11-16
   DSKMAINT.CMD    31.104  91BC  1983-01-26,"09.10.1985"  2.3 for PG685 on
                                                          PCP/M-86 1.0
                                                          or CCP/M-86 3.1 with
                                                          CCP/M-86 XIOS 3.1
   DUMP86.CMD       2.944  4EAF  1983-10-03               3.0,3.1
   ED.CMD           9.728  E763  1983-11-16
   ERASE.CMD        3.968  DC89  1983-11-16
   EXTERN.CMD         640  9BC7  N/A
   GENRSX.CMD      18.688  FD67  1983-10-04,1983-11-16
   GET.CMD          9.088  9E70  1983-11-16
   GETRSX.RSX       3.328  3A82  N/A
   HDPARK.CMD       3.328  257C  N/A                      1.0 for PG685
   HDPARTY.CMD     30.336  6713  1983-01-26               2.5/3 on PCP/M-86 1.0
                                                          or CCP/M-86 3.1
   HELP.CMD         7.680  DBC2  1983-11-16               1.1
   HELP.HLP        54.016  4448  (1982-08-14)
   INITDIR.CMD     35.840  BEA9  N/A
   INTERN.CMD         640  1BC7  N/A
   PATCH86.CMD      3.328  0A90  1983-10-03               3.1
   PCPM.SYS        47.488  3166  1983-11-11,1983-11-16
   PIP.CMD          9.472  845E  1983-11-16,1983-09-02    3.1
   PUT.CMD          8.832  1E84  1983-11-16
   PUTRSX.RSX       3.584  8C7B  N/A
   READ.ME          4.480  6ABF  N/A
   RENAME.CMD       3.200  15D1  1983-11-16
   SET.CMD          9.344  737C  1982,1983-11-16
   SETDEF.CMD       5.120  D248  1983-11-16
   SHOW.CMD         8.704  AFDE  1983-11-16
   STOP.CMD         2.304  C8C4  1983-11-16
   SUBMIT.CMD      11.008  97A5  1983-11-16
   TYPE.CMD         4.096  35A4  1983-11-16

  The CRC checksums were calculated with DR-DOS XDIR /C.

  The date strings found in the binaries do not necessarily match with
  the file date stamps (which might be stored on the PCP/M-86 floppy image -
  I simply don't know, as Sydex 22DISK has no options to read them).

  To avoid further confusion all dates were converted to the international
  date format as per ISO 8601 ("ccyy-mm-dd") unless it was not possible
  for me to track down the actual date due to the ambiguity between the
  US ("mmddccyy") and the (old) European date format ("ddmmccyy"), which
  were for sure both used on the floppy. The uncertain date is given in
  quotes. (Please note that both separators '/' and '.' are in use for
  both the US and the European format, only the ISO 8601 format makes
  the '-' mandantory in case a separator is used.)


  A little attempt in Personal CP/M-86 geneology
  ----------------------------------------------

  Within this file, PCP/M-86 and Personal CP/M-86 are used as synonyms,
  just like CCP/M-86 is used as an abbreviation to Concurrent CP/M-86.

  This issue of PCP/M-86 1.0/5b contains these date stamps:

   "01/26/83"               ->  1983-01-26
   "09/02/83"               ->  1983-09-02
   "031083"                 ->  1983-10-03
   "10/4/83"                ->  1983-10-04
   "111183"                 ->  1983-11-11
   "11/16/83" and "161183"  ->  1983-11-16
   "09.10.85"               ->  1985-10-09? (probably in European format)

  Most probably the date 1982-08-14 was also some special date in the
  CP/M-86 history since the HELP DATE text uses this date as an example.

  Mentionings of a Digital Research product named CP/M-86 Plus can
  be found in Usenet discussions like news:comp.os.cpm and various
  private discussions, as well as in the literature, for example:

  Herwig Feichtinger "Arbeitsbuch Mikrocomputer", 2nd edition, 1987,
  Franzis Verlag, ISBN 3-7723-8022-0 (presumably the first edition
  was published in 1986 or earlier):

  On page 466 the author mentions that CP/M-86 would featurewise
  correspond with CP/M(-80) 2.2, while a newer CP/M(-80) 3.0 alias
  CP/M Plus existed for the 8080/8085/Z80 CPU family, but not for
  the x86 family. He states that while the book went to press
  (1st or 2nd edition?), Digital Research was working on a new
  product named CP/M-86 Plus which would overcome most of the
  limitations of a CP/M(-80) 2.2 style implementation, presumably
  raising the compatibility level to a 3.0 implementation (just
  like for the CP/M-80 series).

  However, so far (2002) noone seems to have found an "offical"
  copy of CP/M-86 Plus, leading to the assumption that Personal
  CP/M-86 1.x and 2.x, which contain many "CP/M-86 Plus" strings
  in the binaries, either are in fact CP/M-86 Plus or at least
  a further derivation of it.

  The author also mentions that Concurrent CP/M-86 (CCP/M) was
  developed out of CP/M-86 (not as I assumed the other way around),
  and that CCP/M-86 would support up to four processes (what a
  coincidence with Personal CP/M-86, which supports three background
  processes, while MP/M-86 supports many more!), but that these
  processes can be utilized by a single user as well as getting
  assigned to different users on up to four serial terminals (not
  possible with Personal CP/M-86). He further states that CCP/M-86
  supports multiple screen windows, and that the system files
  XIOS.SYS, CCP/M.SYS and BDOSPC.COM together would consume 140 Kb.
  Unfortunately he does not mention the exact CCP/M-86 version number
  he is talking about in his book.

  Taking into account that Personal CP/M-86 1.0/5b DSKMAINT.CMD
  2.3 contains strings that it requires "Personal CP/M 1.0" or
  "Concurrent CP/M 3.1" and "Concurrent CP/M XIOS 3.1",
  Personal CP/M-86 1.x looks much like some kind of a single-user
  edition of Concurrent CP/M-86 3.1 to me... (If CP/M-86 Plus
  actually existed, it could have been a single user version
  of CCP/M-86 3.0 then, but this is only guesswork on my part.)

  More interesting is that he states CCP/M-86 3.1 would be able
  to emulate MS-DOS and that it would automatically determine the
  floppy disk format (CP/M or DOS), and that CCP/M-86 3.1 would
  be 30% faster than MS-DOS 1.0 when writing to floppy disks,
  but that CCP/M-86 3.1 would only support the old MS-DOS 1.0
  8 sectors/track floppies, not the newer 9 sectors/track floppies
  introduced with MS-DOS 2.0+, and that support for sub-directories
  was still lacking as well.

  Well, neither Personal CP/M-86 1.0/5b nor 2.1/1 do support
  any kind of DOS emulation, and I cannot make any statement
  in regard to CCP/M-86 3.1 (which I have never seen), but knowing
  that Digital Research's successor DOS Plus 1.2 (as, for example,
  found on the Amstrad PC1512 and - judging from its internal
  strings "CPCDOS 4.1" - corresponding with Concurrent PC-DOS 4.1)
  or DOS Plus 2.1 (as used on the Acorn BBC Master 512) do both
  come with a MS-DOS 2.11 emulation layer, there is some coincidence
  here as well.

  The only contradiction is the version number 3.1 or 4.1;
  if CCP/M-86 3.1 already supported an early form of DOS emulation,
  than PCP/M-86 1.x is *less* than a single-user edition of CCP/M-86 3.1.
  On the other hand, it could well be, that, what Herwig Feichtinger
  still called CCP/M-86, was already marketted as Concurrent PC-DOS 3.1
  (I know that at least a Concurrent PC-DOS 3.2 existed), so there
  is sort of coincidence again... Another unverified little theory is
  that Concurrent CP/M-86 3.x and Concurrent PC-DOS 3.x were in fact
  two flavours of basically the same product, with the latter having
  added an (optional) DOS emulation layer. Does someone know for sure?

  If you have corrections, additions, or can shed some better light on
  these earliest pre-decessors of the later single user DR DOS and
  Multiuser DOS families (with their latest representants at the time
  of this writing being Lineo DR-DOS 7.03 and OEM DR-DOS 7.05 and
  ITERA/IMS REAL/32 7.93 and ITERA/IMS REAL NG now), I would very
  much appreciate your feedback.

  Thanks and Enjoy!

  Matthias Paul <mpaul@drdos.org>

  EOF
