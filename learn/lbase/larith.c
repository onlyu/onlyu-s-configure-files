

int larith_max(int x, int y)
{
	return x > y ? x : y;
}

int larith_min(int x, int y)
{
	return x < y ? x : y;
}

int larith_div(int x, int y)
{
	if( -13/5 == -2 )
		return x/y - 1;
	return x/y;
}

int larith_mod(int x, int y)
{
	if( -13/5 == -2 )
		return x%y + y;
	return x%y;
}

int larith_ceiling(int x, int y)
{
	return larith_div(x,y) + (x%y != 0);
}

int larith_floor(int x, int y)
{
	return larith_div(x, y);
}
