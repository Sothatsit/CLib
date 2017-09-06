#include "numbers.h"

u8 u8_findNextPowerOf2(u8 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    return (u8) (n + 1);
}

u16 u16_findNextPowerOf2(u16 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    return (u16) (n + 1);
}

u32 u32_findNextPowerOf2(u32 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

u64 u64_findNextPowerOf2(u64 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
}