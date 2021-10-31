#include <stdio.h>
#include "foo.h"
int main(void)
{
    int a = 3;
    int b = 5;
    int c = foo(a, b);
    printf("%d\n", c);
    return 0;
}