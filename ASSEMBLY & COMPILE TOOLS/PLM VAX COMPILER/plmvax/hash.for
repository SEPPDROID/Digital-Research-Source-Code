C***********************************************************************
C
C                               HASH.FOR
C
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
C
C-----------------------------------------------------------------------
C
C    This module of the PL/M-VAX compiler contains the symbol table
C    hash code routine, which maps a 32 character identifier into
C    an integer in the range [0..210].
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    18OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C
C-----------------------------------------------------------------------
      INTEGER*4 FUNCTION HASH (ID)
      IMPLICIT INTEGER*4 (A-Z)
      CHARACTER*32 ID
 
      HASH= ((
     #            (ICHAR(ID(1:1))+ICHAR(ID(5:5))+ICHAR(ID(9:9)))*256
     #          + (ICHAR(ID(2:2))+ICHAR(ID(6:6))+ICHAR(ID(10:10))))*256
     #          + (ICHAR(ID(3:3))+ICHAR(ID(7:7))+ICHAR(ID(11:11))))*256
     #          + (ICHAR(ID(4:4))+ICHAR(ID(8:8))+ICHAR(ID(12:12)))
      HASH=MOD(IABS(HASH),211)
      RETURN
      END
