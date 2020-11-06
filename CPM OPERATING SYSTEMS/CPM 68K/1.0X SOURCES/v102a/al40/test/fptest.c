#include <math.h>

float *f1();
float f2();

struct type {
	float ft;
	double dt;
	float *pft;
} fstruc;

char *fstr = "-2.3E-4";
char *astr = "8.6324";

main()
{
	double d;
	float f, *fp;
	char *s, buf[20];
	long l;
	int i;

	if ((fp = f1()) != fstruc.pft) error(0xf1);
	if ((f = f2()) != 2.0) error(0xf2);
	d = f;
	if (d != f) error(0xf3);
	f = atof(fstr);
	s = ftoa(f,buf,6);
	if (strcmp(s,"-0.000230") != 0) error(0xf4);
	f = 3.4;
	l = f;
	if (l != 3L) error(0xf5);
	i = f;
	if (i != 3) error(0xf6);
	f = l;
	if (f < 3.0) error(0xf7);
	f = i;
	if (f < 3.0) error(0xf8);
	f = atof(astr);
	printf("%s ==> ~ %f or %e\n",astr,f,f);
	printf("Far Freaking Out....\n");
}

float *
f1()
{
	return(fstruc.pft);
}

float
f2()
{
	return(2.0);
}

error(enum)
int enum;
{
	printf("%x\n",enum);
}
