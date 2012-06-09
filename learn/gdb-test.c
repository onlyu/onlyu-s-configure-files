#include <stdio.h>
#include <string.h>

int foo(int val);

int test(int val)
{
	printf("%d\n",val);
	return val * val;
}
struct test_s{
    int test_int;
	const char *test_string;
};

int main(int argc, char *argv[])
{
	int i;
	int b = 10;
	int attach = 1;
	struct test_s te;
	te
	for (i=0;i<b;i++){
		int a = test(i);
		printf("%d\n", a);
		a = foo(a);
		if(i=5){
			strcpy(0, "test crash");
		}
		printf("%d\n", a);
	}
	return 0;
}
