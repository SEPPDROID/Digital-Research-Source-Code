C***********************************************************************
C
C			XQCOMMON.FOR
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
C-----------------------------------------------------------------------
C
C    This include-file contains global definitions for XQIO.FOR,
C    which consists of UDI-to-VMS I/O interface routines for the
C    PL/M-VAX runtime library.
C
C-----------------------------------------------------------------------
C
C		R E V I S I O N   H I S T O R Y
C
C    14OCT81  Alex Hunter  1. Added disclaimer notice.
C    12JAN82  Alex Hunter  1. Allocate core dynamically.
C    03FEB82  Alex Hunter  1. Change name of common blocks.
C
C***********************************************************************
      IMPLICIT INTEGER*2 (A-Z)
 
      PARAMETER  CONN_MIN=20,CONN_MAX=31  ! 12 connections max.
      PARAMETER  CHUNK_SIZE=8192          ! Core file basic allocation
					  ! unit in bytes (should be a
					  ! multiple of 512).
      PARAMETER  MAX_CHUNKS=1000          ! Max chunks per core file.
      PARAMETER  MAX_CORE_FILE_SIZE = CHUNK_SIZE * MAX_CHUNKS
      PARAMETER  OUTPUT_RECL=510          ! 1 block - 2 control bytes.
      PARAMETER  MAX_INPUT=512            ! Max bytes expected on input.
      PARAMETER  CR='0D'X,LF='0A'X
 
      CHARACTER*45 FILENAME(CONN_MIN:CONN_MAX)
      CHARACTER*2 CRLF
      CHARACTER*1 XCR,XLF
      EQUIVALENCE (XCR,CRLF(1:1)),(XLF,CRLF(2:2))
      COMMON /XQ_CHARS/ CRLF,FILENAME
      DATA XCR,XLF/CR,LF/
 
      INTEGER*4 CHUNK_ADDRESS(0:MAX_CHUNKS-1,CONN_MIN:CONN_MAX)
      BYTE TEMPORARY_BUFFER(MAX_INPUT)
      COMMON /XQ_CORE/ CHUNK_ADDRESS,TEMPORARY_BUFFER
      PARAMETER CHUNK_ADDRESS_DIMS = MAX_CHUNKS*(CONN_MIN-CONN_MAX+1)
!!!!  DATA CHUNK_ADDRESS /12000*0/
!!!!  Note: by letting the linker create demand-zero pages for
!!!!  the chunk address table (instead of explicitly initializing
!!!!  the table to zeroes), we save almost 100 blocks in the
!!!!  image file.  (But make sure the DATA statement gets re-instated
!!!!  if you use this code on some other system!)
 
      BYTE STATE(CONN_MIN:CONN_MAX)
      PARAMETER STATE_UNATTACHED=0,STATE_ATTACHED=1,STATE_OPEN=2
 
      INTEGER*4 MARKER(CONN_MIN:CONN_MAX),LENGTH(CONN_MIN:CONN_MAX)
 
      INTEGER*2 ACCESS_RIGHTS(CONN_MIN:CONN_MAX)
      PARAMETER AR_DELETE=1,AR_READ=2,AR_WRITE=4,AR_UPDATE=8
   
      INTEGER*2 SEEK_CAPABILITY(CONN_MIN:CONN_MAX)
      PARAMETER SC_FORWARD=1,SC_BACKWARD=2
 
      BYTE ACCESS_MODE(CONN_MIN:CONN_MAX)
      PARAMETER AM_READ=1,AM_WRITE=2,AM_UPDATE=3
 
      BYTE TYPE(CONN_MIN:CONN_MAX)
      PARAMETER NORMAL=0,WORK_FILE=1,INTERACTIVE=2,BYTE_BUCKET=3
 
      LOGICAL*1 MODIFIED(CONN_MIN:CONN_MAX)
 
      BYTE SPECIAL_MODE(CONN_MIN:CONN_MAX)
      PARAMETER TRANSPARENT=1,LINE_EDITED=2,TRANSPARENT_NOWAIT=3
 
      LOGICAL*4 TT_CHANNEL_ASSIGNED
      INTEGER*2 TT_CHANNEL
 
      COMMON /XQ_COMMON/ TT_CHANNEL_ASSIGNED,
     #                  MARKER,LENGTH,ACCESS_RIGHTS,SEEK_CAPABILITY,
     #                  TT_CHANNEL,ACCESS_MODE,TYPE,MODIFIED,STATE,
     #                  SPECIAL_MODE
 
      DATA TT_CHANNEL_ASSIGNED/.FALSE./
 
