void reset_handler(void)
{
	int a = 3;
	int b = 6;
	int c = 15;
	int d = 4;
	int e = 23;
	sum(a, b);
	func(a, b, c, d, e);
	while (1)
		;
}

int sum(int d, int e)
{
	int f = d + e;
	return f;
}

int func(int aa, int bb, int cc, int dd, int ee)
{
	int ff = aa + bb + cc + dd + ee;
	return ff;
}


