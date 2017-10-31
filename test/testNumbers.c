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

bool test_u8_findNextPowerOf2() {
    assert(u8_findNextPowerOf2(1) == 1);
    assert(u8_findNextPowerOf2(3) == 4);
    assert(u8_findNextPowerOf2(97) == 128);
    assert(u8_findNextPowerOf2(129) == 0);

    return true;
}

bool test_u16_findNextPowerOf2() {
    assert(u16_findNextPowerOf2(8) == 8);
    assert(u16_findNextPowerOf2(746) == 1024);
    assert(u16_findNextPowerOf2(31532) == 32768);
    assert(u16_findNextPowerOf2(33768) == 0);

    return true;
}

bool test_u32_findNextPowerOf2() {
    assert(u32_findNextPowerOf2(512) == 512);
    assert(u32_findNextPowerOf2(1234) == 2048);
    assert(u32_findNextPowerOf2(2147481244) == 2147483648);
    assert(u32_findNextPowerOf2(2147491231) == 0);

    return true;
}

bool test_u64_findNextPowerOf2() {
    assert(u64_findNextPowerOf2(2147483648) == 2147483648);
    assert(u64_findNextPowerOf2(137123416546) == 137438953472);
    assert(u64_findNextPowerOf2(9220145818951489954U) == 9223372036854775808U);
    assert(u64_findNextPowerOf2(9223372036854775809U) == 0);

    return true;
}



//
// Run Tests
//

void test_numbers(int * failures, int * successes) {
    test(max);
    test(min);

    test(u8_findNextPowerOf2);
    test(u16_findNextPowerOf2);
    test(u32_findNextPowerOf2);
    test(u64_findNextPowerOf2);
}