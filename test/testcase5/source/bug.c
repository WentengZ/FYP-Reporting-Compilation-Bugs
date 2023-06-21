// test.c
#include <stdio.h>

int x;

__attribute__((noinline))
int test(int * restrict ptr) {
    *ptr = 1;
    if (ptr == &x) {
        *ptr = 2;
    }
    return *ptr;
}

int main() {
    printf("%d\n", test(&x));
}