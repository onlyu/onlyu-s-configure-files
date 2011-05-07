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
  int i;
  int b = 10;
  int attach = 1;
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
