#include <stdio.h>
int c;
int a(int b) {
    int t = 0;
    while (t < 7 && b) t++;
    return t;
}
int test_only_my_e() {
    int l;
    for (l = 0; l < 6; l = l + 1)
        for (c = 0; c < 6; c = (a(6) ^ 7) + c + 1);
}
int main() {
    test_only_my();
    printf("%d\n", c);
}