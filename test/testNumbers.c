#include "test.h"
#include "testNumbers.h"

//
// Tests
//

bool test_max() {
    assert(max(0, 60) == 60);
    assert(max(-60, 0) == 0);
    assert(max(50, 50) == 50);

    return true;
}

bool test_min() {
    assert(min(0, 60) == 0);
    assert(min(-60, 0) == -60);
    assert(min(50, 50) == 50);

    return true;
}

bool test_u8_nextPowerOf2() {
    assert(u8_nextPowerOf2(0) == 1);
    assert(u8_nextPowerOf2(1) == 1);
    assert(u8_nextPowerOf2(3) == 4);
    assert(u8_nextPowerOf2(97) == 128);
    assert(u8_nextPowerOf2(129) == 0);

    return true;
}

bool test_u16_nextPowerOf2() {
    assert(u16_nextPowerOf2(0) == 1);
    assert(u16_nextPowerOf2(1) == 1);
    assert(u16_nextPowerOf2(8) == 8);
    assert(u16_nextPowerOf2(746) == 1024);
    assert(u16_nextPowerOf2(31532) == 32768);
    assert(u16_nextPowerOf2(33768) == 0);

    return true;
}

bool test_u32_nextPowerOf2() {
    assert(u32_nextPowerOf2(0) == 1);
    assert(u32_nextPowerOf2(1) == 1);
    assert(u32_nextPowerOf2(512) == 512);
    assert(u32_nextPowerOf2(1234) == 2048);
    assert(u32_nextPowerOf2(2147481244) == 2147483648);
    assert(u32_nextPowerOf2(2147491231) == 0);

    return true;
}

bool test_u64_nextPowerOf2() {
    assert(u64_nextPowerOf2(0) == 1);
    assert(u64_nextPowerOf2(1) == 1);
    assert(u64_nextPowerOf2(2147483648) == 2147483648);
    assert(u64_nextPowerOf2(137123416546) == 137438953472);
    assert(u64_nextPowerOf2(9220145818951489954U) == 9223372036854775808U);
    assert(u64_nextPowerOf2(9223372036854775809U) == 0);

    return true;
}

bool test_s8_nextPowerOf2() {
    assert(s8_nextPowerOf2(S8_MIN) == 1);
    assert(s8_nextPowerOf2(-63) == 1);
    assert(s8_nextPowerOf2(-1) == 1);
    assert(s8_nextPowerOf2(0) == 1);
    assert(s8_nextPowerOf2(1) == 1);
    assert(s8_nextPowerOf2(3) == 4);
    assert(s8_nextPowerOf2(97) == 0);
    assert(s8_nextPowerOf2(S8_MAX) == 0);

    return true;
}

bool test_s16_nextPowerOf2() {
    assert(s16_nextPowerOf2(S16_MIN) == 1);
    assert(s16_nextPowerOf2(-245) == 1);
    assert(s16_nextPowerOf2(-1) == 1);
    assert(s16_nextPowerOf2(0) == 1);
    assert(s16_nextPowerOf2(1) == 1);
    assert(s16_nextPowerOf2(8) == 8);
    assert(s16_nextPowerOf2(746) == 1024);
    assert(s16_nextPowerOf2(31532) == 0);
    assert(s16_nextPowerOf2(S16_MAX) == 0);

    return true;
}

bool test_s32_nextPowerOf2() {
    assert(s32_nextPowerOf2(S32_MIN) == 1);
    assert(s32_nextPowerOf2(-245) == 1);
    assert(s32_nextPowerOf2(-1) == 1);
    assert(s32_nextPowerOf2(0) == 1);
    assert(s32_nextPowerOf2(1) == 1);
    assert(s32_nextPowerOf2(512) == 512);
    assert(s32_nextPowerOf2(1234) == 2048);
    assert(s32_nextPowerOf2(2147481244) == 0);
    assert(s32_nextPowerOf2(S32_MAX) == 0);

    return true;
}

bool test_s64_nextPowerOf2() {
    assert(s64_nextPowerOf2(S64_MIN) == 1);
    assert(s64_nextPowerOf2(-245) == 1);
    assert(s64_nextPowerOf2(-1) == 1);
    assert(s64_nextPowerOf2(0) == 1);
    assert(s64_nextPowerOf2(1) == 1);
    assert(s64_nextPowerOf2(2147483648) == 2147483648);
    assert(s64_nextPowerOf2(137123416546) == 137438953472);
    assert(s64_nextPowerOf2(9220145818951489954U) == 0);
    assert(s64_nextPowerOf2(S64_MAX) == 0);

    return true;
}

bool test_can_cast_s64_to_sizet() {
    // Placed in variables to avoid CLion warnings...
    size_t sizet_bytes = sizeof(size_t);
    size_t s64_bytes = sizeof(s64);

    assert(can_cast_s64_to_sizet(0));
    assert(can_cast_s64_to_sizet(1));
    assert(can_cast_s64_to_sizet(1234));
    assert(!can_cast_s64_to_sizet(-1));
    assert(!can_cast_s64_to_sizet(-1234));

    if (sizet_bytes < s64_bytes) {
        assert(!can_cast_s64_to_sizet(S64_MAX));
    }

    return true;
}

bool test_can_cast_sizet_to_s64() {
    // Placed in variables to avoid CLion warnings...
    size_t sizet_bytes = sizeof(size_t);
    size_t s64_bytes = sizeof(s64);

    assert(can_cast_sizet_to_s64(0));
    assert(can_cast_sizet_to_s64(1234));

    if(sizet_bytes >= s64_bytes) {
        assert(!can_cast_sizet_to_s64(SIZE_MAX));
    }

    return true;
}

bool test_can_cast_s64_to_long() {
    // Placed in variables to avoid CLion warnings...
    size_t long_bytes = sizeof(long);
    size_t s64_bytes = sizeof(s64);

    assert(can_cast_s64_to_long(0));
    assert(can_cast_s64_to_long(1));
    assert(can_cast_s64_to_long(1234));
    assert(can_cast_s64_to_long(-1));
    assert(can_cast_s64_to_long(-1234));

    if(s64_bytes <= long_bytes) {
        assert(can_cast_s64_to_long(S64_MIN));
        assert(can_cast_s64_to_long(S64_MAX));
    } else {
        assert(!can_cast_s64_to_long(S64_MIN));
        assert(!can_cast_s64_to_long(S64_MAX));
    }

    return true;
}

bool test_can_cast_long_to_s64() {
    // Placed in variables to avoid CLion warnings...
    size_t long_bytes = sizeof(long);
    size_t s64_bytes = sizeof(s64);

    assert(can_cast_long_to_s64(0));
    assert(can_cast_long_to_s64(1));
    assert(can_cast_long_to_s64(1234));
    assert(can_cast_long_to_s64(-1));
    assert(can_cast_long_to_s64(-1234));

    if(s64_bytes >= long_bytes) {
        assert(can_cast_long_to_s64(S64_MIN));
        assert(can_cast_long_to_s64(S64_MAX));
    } else {
        assert(!can_cast_long_to_s64(S64_MIN));
        assert(!can_cast_long_to_s64(S64_MAX));
    }

    return true;
}


//
// Run Tests
//

void test_numbers(int * failures, int * successes) {
    test(max);
    test(min);

    test(can_cast_sizet_to_s64);
    test(can_cast_s64_to_sizet);

    test(u8_nextPowerOf2);
    test(u16_nextPowerOf2);
    test(u32_nextPowerOf2);
    test(u64_nextPowerOf2);

    test(s8_nextPowerOf2);
    test(s16_nextPowerOf2);
    test(s32_nextPowerOf2);
    test(s64_nextPowerOf2);

    test(can_cast_s64_to_sizet);
    test(can_cast_sizet_to_s64);
    test(can_cast_s64_to_long);
    test(can_cast_long_to_s64);
}
