#define begin {
#define end }
/********************************************************/
/*							*/
/*	strins(s1,s2,pos)				*/
/*	insert s1 into s2 at pos			*/
/*	(pos starts at 0)				*/
/*							*/
/********************************************************/

strins(s1,s2,pos)
char s1[],s2[];
int pos;
begin
/*
PROCEDURE INSERT(SOURCE:STRING; VAR DEST:STRING; INDEX:INTEGER);
BEGIN
  MOVERIGHT(DEST[INDEX],DEST[INDEX+LENGTH(SOURCE)],LENGTH(DEST)-INDEX+1);
  MOVE(SOURCE[1],DEST[INDEX],LENGTH(SOURCE));
  DEST[0] := CHR(LENGTH(DEST) + LENGTH(SOURCE))
END;
*/

  int ls1, ls2, lm;	/* length s1, length s2, length to move */
  int mp1, mp2;		/* move "pointers" (indexes) */
  int i,j,k;

  ls1 = strlen(s1);
  ls2 = strlen(s2);
  lm =  ls2 - pos;

  mp1 = pos + lm;
  mp2 = pos + ls1 + lm;

  s2[mp2] = '\0';
 
  for(i=0; i < lm; i++)
    s2[--mp2] = s2[--mp1];

  for(i=0; i < ls1; i++)
    s2[pos+i] = s1[i];

end	/* strins */


/****************************************************************
 *
 *	strdel(sp,len)
 *	remove len characters from string pointed to by sp
 *
 ***************************************************************/

strdel(sp,len)
register char *sp;
register int len;
begin
  register char *ssp;

  ssp = sp + len;

  while ((*sp++ = *ssp++) != 0)
    ;
end
