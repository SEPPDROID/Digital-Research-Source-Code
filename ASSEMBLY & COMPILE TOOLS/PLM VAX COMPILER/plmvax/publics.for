C***********************************************************************
C
C                               PUBLICS.FOR
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
C    This module of the PL/M-VAX compiler generates the PUBLICS file
C    at the end of a compilation.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    20OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C
C-----------------------------------------------------------------------
      SUBROUTINE OUTPUT_PUBLICS(MODULE_NAME)
      INCLUDE 'PLMCOM.FOR/NOLIST'
      CHARACTER*32 MODULE_NAME,PUBLIQUE
 
      CHARACTER*1 REF_MNEM(11)
      DATA REF_MNEM
     //'X','S','B','A','F','D','C','V','U','I','L'/
 
      CHARACTER*1 KIND_MNEM(6)
      DATA KIND_MNEM
     //'M','V','A','P','L','K'/
 
      CHARACTER*1 TYPE_MNEM(-1:S_QUAD)
      DATA TYPE_MNEM
     //'S',' ','B','W','I','P','R','L','D','Q'/
 
      CHARACTER*1 MREF,MKIND,MTYPE
 
      IF (.NOT.PUBLICS_FLAG) RETURN
 
      WRITE(PUB,1001) MODULE_NAME
1001  FORMAT(' *M* ',A)
 
      DO 100 I=SYMBOL_TOP(0)+1,SYMBOL_TOP(1)
 
         IF (((SYMBOL_REF(I).EQ.S_EXT.OR.
     #       (SYMBOL_FLAGS(I).AND.S_PUBLIC).NE.0)) .AND.
     #         (SYMBOL_FLAGS(I).AND.S_NOTPUBLIC).EQ.0) THEN
 
            TYPE=SYMBOL_TYPE(I)
            IF (TYPE.EQ.S_STRUC) TYPE=-1
            MTYPE=TYPE_MNEM(TYPE)
 
            KIND=SYMBOL_KIND(I)
            MKIND=KIND_MNEM(KIND)
            MREF=REF_MNEM(SYMBOL_REF(I))
 
            IF (KIND.EQ.S_PROC) THEN
               WRITE(PUB,1002) PUBLIQUE(SYMBOL_PLM_ID(I)),MREF,MTYPE,
     #                         MKIND,SYMBOL_LIST_SIZE(I)
1002           FORMAT(X,A,X,3A1:'(',I5,')')
 
            ELSEIF (KIND.EQ.S_ARRAY) THEN
               WRITE(PUB,1002) PUBLIQUE(SYMBOL_PLM_ID(I)),MREF,MTYPE,
     #                         MKIND,SYMBOL_NBR_ELEMENTS(I)
 
            ELSE
               WRITE(PUB,1002) PUBLIQUE(SYMBOL_PLM_ID(I)),MREF,MTYPE,
     #                         MKIND
            ENDIF
         ENDIF
100   CONTINUE
 
      CLOSE (UNIT=PUB)
 
      RETURN
      END
