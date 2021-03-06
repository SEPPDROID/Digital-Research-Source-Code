C***********************************************************************
C
C                               BREAK.FOR
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
C    This module of the PL/M-VAX compiler handles breaks between
C    basic blocks.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    15OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C    21OCT81  Alex Hunter  1. Basic block analysis. (V5.5)
C
C-----------------------------------------------------------------------
      SUBROUTINE BREAK
      INCLUDE 'PLMCOM.FOR/NOLIST'
 
      CALL MASSAGE(BASIC_BLOCK,0)
      CALL GET_SOMEWHERE(BASIC_BLOCK,ANY_WHERE)
      BASIC_BLOCK=NULL
      END_OF_BASIC_BLOCK=.FALSE.
      NEXT_NODE=NODE_MIN
      NEXT_ATOM=FIRST_FREE_ATOM
      NEXT_FIXED=FIX_MIN
      NEXT_FLOAT=FLT_MIN
      NEXT_CONSTANT=CON_MIN
      CALL FREE_REGS
      RETURN
      END
