main()
{	float x,y,z;

	x = 1.0;
	y = 2.0;
	z = x * y;

	printf ("%0lx\n", *((int *)&z));
}
