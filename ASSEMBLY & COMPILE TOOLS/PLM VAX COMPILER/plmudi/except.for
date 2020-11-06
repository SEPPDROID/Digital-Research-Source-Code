C***********************************************************************
C
C		D I S C L A I M E R   N O T I C E		      
C		-------------------   -----------
C
C  This document and/or portions of the material and data furnished
C  herewith, was developed under sponsorship of the U. S. Government.
C  Neither the U.S. nor the U.S.D.O.E., nor the Leland Stanford Junior
C  University, nor their employees, nor their respective contractors,
C  subcontractors, or their employees, makes any warranty, express or
C  implied, or assumes any liability or responsibility for accuracy,
C  completeness or usefulness of any information, apparatus, product
C  or process disclosed, or represents that its use will not infringe
C  privately-owned rights.  Mention of any product, its manufacturer,
C  or suppliers shall not, nor is it intended to, imply approval, dis-
C  approval, or fitness for any particular use.  The U. S. and the
C  University at all times retain the right to use and disseminate same
C  for any purpose whatsoever.  Such distribution shall be made by the
C  National Energy Software Center at the Argonne National Laboratory
C  and only subject to the distributee furnishing satisfactory proof
C  that he has a valid license from the Intel Corporation in effect.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    14JAN82  Alex Hunter  1. Change value of E$SUPPORT per Series III
C			      Programmer's Reference Manual Rev. B.
C
C***********************************************************************
      PARAMETER
     # E$OK           = '0000'X,
     # E$CONTEXT      = '0101'X,
     # E$CROSSFS      = '0102'X,
     # E$EXIST        = '0103'X,
     # E$FACCESS      = '0026'X,
     # E$FEXIST       = '0020'X,
     # E$FNEXIST      = '0021'X,
     # E$MEM          = '0002'X,
     # E$NOPEN        = '0104'X,
     # E$OPEN         = '0105'X,
     # E$OREAD        = '0106'X,
     # E$OWRITE       = '0107'X,
     # E$PARAM        = '0108'X,
     # E$PTR          = '0109'X,
     # E$SHARE        = '0028'X,
     # E$SIX          = '010A'X,
     # E$SPACE        = '0029'X,
     # E$STRING$BUF   = '0081'X,
     # E$SUPPORT      = '0023'X,
     # E$SYNTAX       = '010C'X,
     # E$UNSAT        = '010E'X,
     # E$ADDRESS      = '010F'X,
     # E$BAD$FILE     = '0110'X,
     # E$ZERO$DIVIDE  = '8000'X,
     # E$OVERFLOW     = '8001'X,
     # E$8087         = '8007'X
 
