C***********************************************************************
C
C                               MASSAGE.FOR
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
C    This module of the PL/M-VAX compiler 'massages' a code tree by
C    (1) resolving signed/unsigned context of nodes, (2) coercing
C    context if needed, (3) folding constant operations, (4) merging
C    common subnodes, and (5) computing reference counts for the nodes.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    08SEP81  Alex Hunter  1. Compute reference counts. (V5.1)
C
C***********************************************************************
      INTEGER*2 FUNCTION MASSAGE(CODE,DEFAULT_CONTEXT)
      INCLUDE 'PLMCOM.FOR/NOLIST'
  
      CALL RESOLVE_CONTEXT(CODE)
      IF (NODE_CONTEXT(CODE).EQ.0) 
     #    CALL SET_CONTEXT(CODE,DEFAULT_CONTEXT)
      CALL COERCE_TYPES(CODE)
      MASSAGE=FOLD_CONSTANTS(CODE)
      MASSAGE=MERGE(MASSAGE)
      CALL COMPUTE_REFERENCE_COUNTS(MASSAGE)
      RETURN
      END
