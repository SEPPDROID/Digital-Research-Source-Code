#include <math.h>

float *f1();
float f2();
float ret_0();

struct type {
	float ft;
	double dt;
	float *pft;
} fstruc;

char *fstr = "-2.3E-4";
char *astr = "8.6324";
float ar[10];
float global_f;

int err = 0;

main()
{
	double d;
	float f, fp2, *fp;
	char *s, buf[20];
	long l;
	int i;

	if ((fp = f1()) != fstruc.pft) error(0xf1);
	if ((f = f2()) != 2.0) error(0xf2);
	d = f;
	if (d != f) error(0xf3);
	f = atof(fstr);
	s = ftoa(f,buf,6);
	printf("%s => %f => %s\n",fstr,f,s);
	f = 3.4;
	l = f;
	if (l != 3L) { error(0xf4); printf("%f => %ld != 3L\n",f,l); }
	i = f;
	if (i != 3)  { error(0xf5); printf("%f => %d != 3\n",f,i); }
	l = 7L;
	f = l;
	if (f < 6.8 || f > 7.2) { error(0xf6); printf("%ld => %f != 7.0\n",l,f); }
	i = 7;
	f = i;
	if (f < 6.8 || f > 7.2) { error(0xf6); printf("%d => %f != 7.0\n",i,f); }
	f = atof(astr);
	printf("%s ==> ~ %f or %e\n",astr,f,f);
	f = 0;
	if (f != 0.0) error(0xf7);
	i = 3;
	ar[i] = 3.0;
	f = 4.0;
	fp2 = 10.0;
	printf("%f += %f * %f ==> ",ar[i],f,fp2);
	ar[i] += f * fp2;
	printf("%f\n",ar[i]);
	global_f = 0.0;
	f = 0.0;
	if_test(f);
	if (!err)
		printf("Far Freaking Out....\n");
	else
		printf("Tough Luck...\n");
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

float
ret_0()
{
	return(0.0);
}

if_test(pf)
float pf;
{
	float af, *ptr;

	ar[2] = 0.0;
	af = 0.0;
	ptr = &af;
	if (global_f) error(0x10);
	if (ar[2]) error(0x11);
	if (af) error(0x12);
	if (pf) error(0x13);
	if (*ptr) error(0x14);
	if (ret_0()) error(0x14);
}

error(enum)
int enum;
{
	printf("error: %x\n",enum);
	err++;
}
