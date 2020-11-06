/* Compiler Test Program Part II - version 4.0 plus */

struct lbits {
	unsigned b1 : 4;
	unsigned b2 : 12;
} low;

struct hbits {
	unsigned b1 : 12;
	unsigned b2 : 4;
} high;

int err;
int globali = -32768;
unsigned globalu = 44000;

main()
{
	l_vs_i();
	rl_vs_i();
	l_vs_ri();
	rl_vs_ri();
	v41();
	if (!err)
		printf("Far Freaking Out\n");
	else
		printf("Tough Luck.....\n");
}

l_vs_i()		/* longs versus ints... v. 4.0 addition */
{
	long autol, autol1;
	int i;

	i = 3;
	autol = autol1 = 100;
	autol1 *= i;
	autol =autol * i;
	if (autol1 != autol) error(0x00);

	autol = autol1 = 100;
	autol1 /= i;
	autol =autol / i;
	if (autol1 != autol) error(0x01);

	autol = autol1 = 100;
	autol1 += i;
	autol =autol + i;
	if (autol1 != autol) error(0x02);

	autol = autol1 = 100;
	autol1 -= i;
	autol =autol - i;
	if (autol1 != autol) error(0x03);

	autol = autol1 = 100;
	autol1 %= i;
	autol =autol % i;
	if (autol1 != autol) error(0x04);

	autol = autol1 = 100;
	autol1 =<< i;
	autol =autol << i;
	if (autol1 != autol) error(0x05);

	autol = autol1 = 100;
	autol1 =>> i;
	autol =autol >> i;
	if (autol1 != autol) error(0x06);

	autol = autol1 = 100;
	autol1 &= i;
	autol =autol & i;
	if (autol1 != autol) error(0x07);

	autol = autol1 = 100;
	autol1 |= i;
	autol =autol | i;
	if (autol1 != autol) error(0x08);

	autol = autol1 = 100;
	autol1 ^= i;
	autol =autol ^ i;
	if (autol1 != autol) error(0x09);
}

rl_vs_i()		/* register longs versus ints... v. 4.0 addition */
{
	register long regl, regl1;
	int i;

	i = 3;
	regl = regl1 = 100;
	regl1 *= i;
	regl = regl * i;
	if (regl1 != regl) error(0x10);

	regl = regl1 = 100;
	regl1 /= i;
	regl = regl / i;
	if (regl1 != regl) error(0x11);

	regl = regl1 = 100;
	regl1 += i;
	regl = regl + i;
	if (regl1 != regl) error(0x12);

	regl = regl1 = 100;
	regl1 -= i;
	regl = regl - i;
	if (regl1 != regl) error(0x13);

	regl = regl1 = 100;
	regl1 %= i;
	regl = regl % i;
	if (regl1 != regl) error(0x14);

	regl = regl1 = 100;
	regl1 =<< i;
	regl = regl << i;
	if (regl1 != regl) error(0x15);

	regl = regl1 = 100;
	regl1 =>> i;
	regl = regl >> i;
	if (regl1 != regl) error(0x16);

	regl = regl1 = 100;
	regl1 &= i;
	regl = regl & i;
	if (regl1 != regl) error(0x17);

	regl = regl1 = 100;
	regl1 |= i;
	regl = regl | i;
	if (regl1 != regl) error(0x18);

	regl = regl1 = 100;
	regl1 ^= i;
	regl = regl ^ i;
	if (regl1 != regl) error(0x19);
}

l_vs_ri()		/* longs versus register ints... v. 4.0 addition */
{
	long autol, autol1;
	register int i;

	i = 2;
	autol = autol1 = 100;
	autol1 *= i;
	autol =autol * i;
	if (autol1 != autol) error(0x20);

	autol = autol1 = 100;
	autol1 /= i;
	autol =autol / i;
	if (autol1 != autol) error(0x21);

	autol = autol1 = 100;
	autol1 += i;
	autol =autol + i;
	if (autol1 != autol) error(0x22);

	autol = autol1 = 100;
	autol1 -= i;
	autol =autol - i;
	if (autol1 != autol) error(0x23);

	autol = autol1 = 100;
	autol1 %= i;
	autol =autol % i;
	if (autol1 != autol) error(0x24);

	autol = autol1 = 100;
	autol1 =<< i;
	autol =autol << i;
	if (autol1 != autol) error(0x25);

	autol = autol1 = 100;
	autol1 =>> i;
	autol =autol >> i;
	if (autol1 != autol) error(0x26);

	autol = autol1 = 100;
	autol1 &= i;
	autol =autol & i;
	if (autol1 != autol) error(0x27);

	autol = autol1 = 100;
	autol1 |= i;
	autol =autol | i;
	if (autol1 != autol) error(0x28);

	autol = autol1 = 100;
	autol1 ^= i;
	autol =autol ^ i;
	if (autol1 != autol) error(0x29);
}

rl_vs_ri()		/* reg longs versus reg ints... v. 4.0 addition */
{
	register long regl, regl1;
	register int i;

	i = 3;
	regl = regl1 = 100;
	regl1 *= i;
	regl = regl * i;
	if (regl1 != regl) error(0x30);

	regl = regl1 = 100;
	regl1 /= i;
	regl = regl / i;
	if (regl1 != regl) error(0x31);

	regl = regl1 = 100;
	regl1 += i;
	regl = regl + i;
	if (regl1 != regl) error(0x32);

	regl = regl1 = 100;
	regl1 -= i;
	regl = regl - i;
	if (regl1 != regl) error(0x33);

	regl = regl1 = 100;
	regl1 %= i;
	regl = regl % i;
	if (regl1 != regl) error(0x34);

	regl = regl1 = 100;
	regl1 =<< i;
	regl = regl << i;
	if (regl1 != regl) error(0x35);

	regl = regl1 = 100;
	regl1 =>> i;
	regl = regl >> i;
	if (regl1 != regl) error(0x36);

	regl = regl1 = 100;
	regl1 &= i;
	regl = regl & i;
	if (regl1 != regl) error(0x37);

	regl = regl1 = 100;
	regl1 |= i;
	regl = regl | i;
	if (regl1 != regl) error(0x38);

	regl = regl1 = 100;
	regl1 ^= i;
	regl = regl ^ i;
	if (regl1 != regl) error(0x39);
}

v41()	/* 4.1 fixed and tested */
{
	long l, l1;

	/* bit assignments */
	low.b1 = high.b2;
	high.b1 = low.b2;

	l = l1 = 45L;
	/* non-code-generating simple expressions */
	l =* 1L;
	if (l != l1) error(0x40);
	l =+ 0L;
	if (l != l1) error(0x41);
	l =- 0L;
	if (l != l1) error(0x42);
	l =* 0L;
	if (l != 0L) error(0x43);
	l1 =% 1L;
	if (l1 != 0L) error(0x44);
}

error(pc)
{
	printf("error %x\n",pc);
	err++;
}

