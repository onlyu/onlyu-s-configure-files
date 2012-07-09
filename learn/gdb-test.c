#include <stdio.h>
#include <string.h>

int foo(int val);

int test(int val)
{
	printf("%d\n",val);
	return val * val;
}

int main(int argc, char *argv[])
{
	int a = 10;
	a *= 0.2;

	float b = 0.2;
	b *= 10;
	printf ("a=%d\n b=%f",a, b);
	return 0;
}
