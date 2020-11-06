C***********************************************************************
C
C                               JPI.FOR
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
C    This module of the PL/M-VAX compiler is used to obtain job
C    statistics for compiler performance measurement and reporting.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    18OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C
C-----------------------------------------------------------------------
      SUBROUTINE JPI(NTICKS,NFAULTS)
      IMPLICIT INTEGER*4 (A-Z)
      INCLUDE 'SYS$LIBRARY:JPIDEF.FOR/NOLIST'
      INTEGER*4 IL(7)
      INTEGER*2 IW(14)
      EQUIVALENCE (IL,IW)
 
      DATA IW/4,JPI$_CPUTIM,0,0,0,0,4,JPI$_PAGEFLTS,0,0,0,0,0,0/
 
      IL(2)=%LOC(NTICKS)
      IL(5)=%LOC(NFAULTS)
      ISS=SYS$GETJPI(,,,IL,,,)
      RETURN
      END
