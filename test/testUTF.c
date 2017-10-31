#include "test.h"
#include "testUTF.h"



//
// Tests
//

bool test_utf8_toCodepoint() {
    return false;
}

bool test_utf8_fromCodepoint() {
    String utf = str_createUninitialised(1);
    String fromCodepoint;
    {
        assertStrValid(utf);

        str_set(utf, 0, (char) 0b00100100);

        fromCodepoint = utf8_fromCodepoint(0b0100100);

        assert(str_equals(utf, fromCodepoint));
    }
    str_destroy(&utf);
    str_destroy(&fromCodepoint);

    utf = str_createUninitialised(2);
    {
        assertStrValid(utf);

        str_set(utf, 0, (char) 0b11000010);
        str_set(utf, 1, (char) 0b10100010);

        fromCodepoint = utf8_fromCodepoint(0b00010100010);

        assert(str_equals(utf, fromCodepoint));
    }
    str_destroy(&utf);
    str_destroy(&fromCodepoint);

    utf = str_createUninitialised(3);
    {
        assertStrValid(utf);

        str_set(utf, 0, (char) 0b11100010);
        str_set(utf, 1, (char) 0b10000010);
        str_set(utf, 2, (char) 0b10101100);

        fromCodepoint = utf8_fromCodepoint(0b0010000010101100);

        assert(str_equals(utf, fromCodepoint));
    }
    str_destroy(&utf);
    str_destroy(&fromCodepoint);

    utf = str_createUninitialised(4);
    {
        assertStrValid(utf);

        str_set(utf, 0, (char) 0b11110000);
        str_set(utf, 1, (char) 0b10010000);
        str_set(utf, 2, (char) 0b10001101);
        str_set(utf, 3, (char) 0b10001000);

        fromCodepoint = utf8_fromCodepoint(0b000010000001101001000);

        assert(str_equals(utf, fromCodepoint));
    }
    str_destroy(&utf);
    str_destroy(&fromCodepoint);

    return true;
}

bool test_utf8_appendCodepoint() {
    return false;
}

bool test_utf16le_toCodepoint() {
    return false;
}

bool test_utf16le_fromCodepoint() {
    return false;
}

bool test_utf16le_appendCodepoint() {
    return false;
}



//
// Run Tests
//

void test_UTF(int *failures, int *successes) {
    test(utf8_toCodepoint);
    test(utf8_fromCodepoint);
    test(utf8_appendCodepoint);

    test(utf16le_toCodepoint);
    test(utf16le_fromCodepoint);
    test(utf16le_appendCodepoint);
}