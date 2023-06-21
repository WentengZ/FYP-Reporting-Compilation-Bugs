#include <stdio.h>
//4
int c_test_comp;
int a(int b) {
    int t = 0;
    while (t < 7 && b) t++;
    return t;
}
int test_only_my_e() {
    int l;
    for (l = 0; l < 6; l = l + 1)
        for (c_test_comp = 0; c_test_comp < 6; c_test_comp = (a(6) ^ 7) + c_test_comp + 1);
}
int main() {
    test_only_my_e();
    printf("%d\n", c_test_comp);
}