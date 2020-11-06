_clenf (s) /* counts length of string */
register char *s;
{
register int n;
for (n=0; *s++ != '\0'; n++);
return (n);}
