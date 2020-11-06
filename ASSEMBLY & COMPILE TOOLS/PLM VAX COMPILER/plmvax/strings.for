C***********************************************************************
C
C                               STRINGS.FOR
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
C    This module of the PL/M-VAX compiler contains several character
C    string utility functions.
C
C-----------------------------------------------------------------------
C		R E V I S I O N   H I S T O R Y
C
C    18OCT81  Alex Hunter  1. Added disclaimer notice. (V5.4)
C                          2. Re-coded LNB.
C
C-----------------------------------------------------------------------
	INTEGER*4 FUNCTION LNB(STRING)
C-----------------------------------------------------------------------
C
C    Function to locate the last non-blank character of a character
C    string.
C
C   Return with LNB = index of last non-blank character in STRING
C   (or LNB = 1 if STRING contains only blanks).
C
C-----------------------------------------------------------------------
	IMPLICIT INTEGER*4 (A-Z)
	CHARACTER*(*) STRING
 
     	LNB = LEN(STRING)
        DO WHILE (LNB.GT.1 .AND. STRING(LNB:LNB).EQ.' ')
	   LNB = LNB - 1
     	ENDDO
	RETURN
	END
C-----------------------------------------------------------------------
C
C    Function to convert a signed integer*4 value into an ASCII
C    character*10 string representation.
C
C    The character string will be right-justified with leading blanks.
C    The index of the first significant (i.e., non-blank) character
C    in the string will be returned in IFSD.
C
C-----------------------------------------------------------------------
	CHARACTER*10 FUNCTION STRING10 (N, IFSD)
	IMPLICIT INTEGER*4 (A-Z)
 
        IF (N.EQ.'80000000'X) THEN
                STRING10 = '2147483648'
		IFSD = 1
		RETURN
	ENDIF
 
	STRING10 = ' '
	M = ABS(N)
	I = 11
100	I = I - 1
	STRING10(I:I) = CHAR(ICHAR('0')+MOD(M,10))
	M = M/10
	IF (M.NE.0) GO TO 100
	IF (N.LT.0) THEN
		I = I - 1
		STRING10(I:I) = '-'
	ENDIF
	IFSD = I
	RETURN
	END
C-----------------------------------------------------------------------
C
C    Function to convert a REAL*4 value into an ASCII character*14
C    string representation.
C
C    The index of the first significant character in the string will be
C    returned in IFSD, and the index of the last significant character
C    will be returned in ILSD.
C
C-----------------------------------------------------------------------
      CHARACTER*14 FUNCTION STRINGG(RVAL,IFSD,ILSD)
      INTEGER*4 IFSD, ILSD
      REAL*4 RVAL
 
      ENCODE(14,1000,STRINGG) RVAL
1000  FORMAT(G14.7)
 
      IF (STRINGG.EQ.' 0.0000000E+00') THEN
         IFSD=2
         ILSD=4
         RETURN
      ENDIF
 
      DO 10 IFSD=1,13
      IF (STRINGG(IFSD:IFSD).NE.' ') GO TO 20
10    CONTINUE
 
20    IF (STRINGG(11:11).EQ.'E') THEN
         ILSD=14
         RETURN
      ENDIF
 
      DO 30 ILSD=14,2,-1
      IF (STRINGG(ILSD:ILSD).NE.' '.AND.STRINGG(ILSD:ILSD).NE.'0') 
     #      GO TO 40
30    CONTINUE
 
40    IF (STRINGG(ILSD:ILSD+1).EQ.'.0') ILSD=ILSD+1
      RETURN
      END
C-----------------------------------------------------------------------
C
C    Function to convert a UDI-style byte string into a FORTRAN character
C    string.
C
C    The byte string in STRING is copied into the character string CHARS
C    and the length of the string is returned as the value of the 
C    function.
C
C-----------------------------------------------------------------------
      INTEGER*4 FUNCTION MAKE_CHARS(CHARS,STRING)
      CHARACTER*(*) CHARS
      BYTE STRING(0:0)
 
      IF (STRING(0).GT.0) THEN
         DO 10 I=1,STRING(0)
         CHARS(I:I)=CHAR(STRING(I))
10       CONTINUE
         MAKE_CHARS=STRING(0)
      ELSE
         CHARS=' '
         MAKE_CHARS=1
      ENDIF
      RETURN
      END
