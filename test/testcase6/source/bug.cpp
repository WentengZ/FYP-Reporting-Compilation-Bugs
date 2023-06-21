#include <optional>
#include <string>
#include <string_view>
#include <iostream>

__attribute__((always_inline)) static inline unsigned char constant_time_ge_8(unsigned int a, unsigned int b) {
  return ~(((unsigned int)(a - b)) >> 15);
}

__attribute__((noinline)) unsigned char CheckPadding(unsigned char *data, unsigned int length)
{
    unsigned int padding_length = data[length - 1];
    unsigned char res = 0;

    for (unsigned int i = 0; i < length - 1; i++) {
        unsigned char mask = constant_time_ge_8(padding_length, i);
        unsigned char b = data[length - 1 - i];
        res |= mask & (padding_length ^ b);
    }

    return res;
}

__attribute__((noinline)) unsigned char CheckPaddingNoVec(unsigned char *data, unsigned int length)
{
    unsigned int padding_length = data[length - 1];
    unsigned char res = 0;

#pragma clang loop vectorize(disable)
    for (unsigned int i = 0; i < length - 1; i++) {
        unsigned char mask = constant_time_ge_8(padding_length, i);
        unsigned char b = data[length - 1 - i];
        res |= mask & (padding_length ^ b);
    }

    return res;
}

__attribute__((aligned(16))) unsigned char data_length41[41] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03};

__attribute__((aligned(16))) unsigned char data_length40[40] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x03};

int main() {
    // std::cout << (int)CheckPadding(data_length40, sizeof(data_length40)) << std::endl;
    std::cout << (int)CheckPadding(data_length41, sizeof(data_length41)) << std::endl;
    // std::cout << (int)CheckPaddingNoVec(data_length40, sizeof(data_length40)) << std::endl;
    // std::cout << (int)CheckPaddingNoVec(data_length41, sizeof(data_length41)) << std::endl;
}