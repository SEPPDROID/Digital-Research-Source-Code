#define	OUTSTR	9
#define	EXIT	0

char	mesg[] = {"Hello, Steve\r\n$"};
_main()
{
	__BDOS(OUTSTR,&mesg[0]);
}
exit()
{
	__BDOS(EXIT,0L);
}
