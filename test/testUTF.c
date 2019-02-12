#include "test.h"
#include "testUTF.h"



//
// Tests
//

bool test_utf8_toCodepoint() {
    StringBuilder utf8 = strbuilder_create(10);
    {
        assert(strbuilder_appendChar(&utf8, (char) 0b00100100));

        assert(strbuilder_appendChar(&utf8, (char) 0b11000010));
        assert(strbuilder_appendChar(&utf8, (char) 0b10100010));

        assert(strbuilder_appendChar(&utf8, (char) 0b11100010));
        assert(strbuilder_appendChar(&utf8, (char) 0b10000010));
        assert(strbuilder_appendChar(&utf8, (char) 0b10101100));

        assert(strbuilder_appendChar(&utf8, (char) 0b11110000));
        assert(strbuilder_appendChar(&utf8, (char) 0b10010000));
        assert(strbuilder_appendChar(&utf8, (char) 0b10001101));
        assert(strbuilder_appendChar(&utf8, (char) 0b10001000));
    }

    String remaining = utf8.string;

    assert(utf8_toCodepoint(&remaining) == 0b0100100);
    assert(utf8_toCodepoint(&remaining) == 0b00010100010);
    assert(utf8_toCodepoint(&remaining) == 0b0010000010101100);
    assert(utf8_toCodepoint(&remaining) == 0b000010000001101001000);
    assert(str_isEmpty(remaining));

    return true;
}

bool test_utf8_fromCodepoint() {
    String utf8 = str_createUninitialised(4);

    String fromCodepoint = utf8_fromCodepoint(0b0100100);
    {
        utf8.length = 1;
        str_set(utf8, 0, (char) 0b00100100);

        assert(str_equals(utf8, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    fromCodepoint = utf8_fromCodepoint(0b00010100010);
    {
        utf8.length = 2;
        str_set(utf8, 0, (char) 0b11000010);
        str_set(utf8, 1, (char) 0b10100010);

        assert(str_equals(utf8, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    fromCodepoint = utf8_fromCodepoint(0b0010000010101100);
    {
        utf8.length = 3;
        str_set(utf8, 0, (char) 0b11100010);
        str_set(utf8, 1, (char) 0b10000010);
        str_set(utf8, 2, (char) 0b10101100);

        assert(str_equals(utf8, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    fromCodepoint = utf8_fromCodepoint(0b000010000001101001000);
    {
        utf8.length = 4;
        str_set(utf8, 0, (char) 0b11110000);
        str_set(utf8, 1, (char) 0b10010000);
        str_set(utf8, 2, (char) 0b10001101);
        str_set(utf8, 3, (char) 0b10001000);

        assert(str_equals(utf8, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    str_destroy(&utf8);
    return true;
}

bool test_utf8_appendCodepoint() {
    StringBuilder expected = strbuilder_create(10);
    {
        assert(strbuilder_appendChar(&expected, (char) 0b00100100));

        assert(strbuilder_appendChar(&expected, (char) 0b11000010));
        assert(strbuilder_appendChar(&expected, (char) 0b10100010));

        assert(strbuilder_appendChar(&expected, (char) 0b11100010));
        assert(strbuilder_appendChar(&expected, (char) 0b10000010));
        assert(strbuilder_appendChar(&expected, (char) 0b10101100));

        assert(strbuilder_appendChar(&expected, (char) 0b11110000));
        assert(strbuilder_appendChar(&expected, (char) 0b10010000));
        assert(strbuilder_appendChar(&expected, (char) 0b10001101));
        assert(strbuilder_appendChar(&expected, (char) 0b10001000));
    }

    StringBuilder fromCodepoints = strbuilder_create(10);
    {
        assert(utf8_appendCodepoint(&fromCodepoints, 0b0100100));
        assert(utf8_appendCodepoint(&fromCodepoints, 0b00010100010));
        assert(utf8_appendCodepoint(&fromCodepoints, 0b0010000010101100));
        assert(utf8_appendCodepoint(&fromCodepoints, 0b000010000001101001000));
    }

    assert(str_equals(expected.string, fromCodepoints.string));

    return true;
}

bool test_utf16le_toCodepoint() {
    StringBuilder utf16 = strbuilder_create(16);
    {
        assert(strbuilder_appendChar(&utf16, (char) 0x7A));
        assert(strbuilder_appendChar(&utf16, (char) 0x00));

        assert(strbuilder_appendChar(&utf16, (char) 0x34));
        assert(strbuilder_appendChar(&utf16, (char) 0x6C));

        assert(strbuilder_appendChar(&utf16, (char) 0x00));
        assert(strbuilder_appendChar(&utf16, (char) 0xD8));
        assert(strbuilder_appendChar(&utf16, (char) 0x00));
        assert(strbuilder_appendChar(&utf16, (char) 0xDC));

        assert(strbuilder_appendChar(&utf16, (char) 0x34));
        assert(strbuilder_appendChar(&utf16, (char) 0xD8));
        assert(strbuilder_appendChar(&utf16, (char) 0x1E));
        assert(strbuilder_appendChar(&utf16, (char) 0xDD));

        assert(strbuilder_appendChar(&utf16, (char) 0xFF));
        assert(strbuilder_appendChar(&utf16, (char) 0xDB));
        assert(strbuilder_appendChar(&utf16, (char) 0xFD));
        assert(strbuilder_appendChar(&utf16, (char) 0xDF));
    }

    String remaining = utf16.string;

    assert(utf16le_toCodepoint(&remaining) == 0x007A);
    assert(utf16le_toCodepoint(&remaining) == 0x6C34);
    assert(utf16le_toCodepoint(&remaining) == 0x10000);
    assert(utf16le_toCodepoint(&remaining) == 0x1D11E);
    assert(utf16le_toCodepoint(&remaining) == 0x10FFFD);
    assert(str_isEmpty(remaining));

    return true;
}

bool test_utf16le_fromCodepoint() {
    String utf16 = str_createUninitialised(4);

    String fromCodepoint = utf16le_fromCodepoint(0x7A);
    {
        utf16.length = 2;
        str_set(utf16, 0, (char) 0x7A);
        str_set(utf16, 1, (char) 0x00);

        assert(str_equals(utf16, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    fromCodepoint = utf16le_fromCodepoint(0x6C34);
    {
        utf16.length = 2;
        str_set(utf16, 0, (char) 0x34);
        str_set(utf16, 1, (char) 0x6C);

        assert(str_equals(utf16, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    fromCodepoint = utf16le_fromCodepoint(0x10000);
    {
        utf16.length = 4;
        str_set(utf16, 0, (char) 0x00);
        str_set(utf16, 1, (char) 0xD8);
        str_set(utf16, 2, (char) 0x00);
        str_set(utf16, 3, (char) 0xDC);

        assert(str_equals(utf16, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    fromCodepoint = utf16le_fromCodepoint(0x1D11E);
    {
        utf16.length = 4;
        str_set(utf16, 0, (char) 0x34);
        str_set(utf16, 1, (char) 0xD8);
        str_set(utf16, 2, (char) 0x1E);
        str_set(utf16, 3, (char) 0xDD);

        assert(str_equals(utf16, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    fromCodepoint = utf16le_fromCodepoint(0x10FFFD);
    {
        utf16.length = 4;
        str_set(utf16, 0, (char) 0xFF);
        str_set(utf16, 1, (char) 0xDB);
        str_set(utf16, 2, (char) 0xFD);
        str_set(utf16, 3, (char) 0xDF);

        assert(str_equals(utf16, fromCodepoint));
    }
    str_destroy(&fromCodepoint);

    str_destroy(&utf16);
    return true;
}

bool test_utf16le_appendCodepoint() {
    StringBuilder expected = strbuilder_create(16);
    {
        assert(strbuilder_appendChar(&expected, (char) 0x7A));
        assert(strbuilder_appendChar(&expected, (char) 0x00));

        assert(strbuilder_appendChar(&expected, (char) 0x34));
        assert(strbuilder_appendChar(&expected, (char) 0x6C));

        assert(strbuilder_appendChar(&expected, (char) 0x00));
        assert(strbuilder_appendChar(&expected, (char) 0xD8));
        assert(strbuilder_appendChar(&expected, (char) 0x00));
        assert(strbuilder_appendChar(&expected, (char) 0xDC));

        assert(strbuilder_appendChar(&expected, (char) 0x34));
        assert(strbuilder_appendChar(&expected, (char) 0xD8));
        assert(strbuilder_appendChar(&expected, (char) 0x1E));
        assert(strbuilder_appendChar(&expected, (char) 0xDD));

        assert(strbuilder_appendChar(&expected, (char) 0xFF));
        assert(strbuilder_appendChar(&expected, (char) 0xDB));
        assert(strbuilder_appendChar(&expected, (char) 0xFD));
        assert(strbuilder_appendChar(&expected, (char) 0xDF));
    }

    StringBuilder fromCodepoints = strbuilder_create(16);
    {
        assert(utf16le_appendCodepoint(&fromCodepoints, 0x007A));
        assert(utf16le_appendCodepoint(&fromCodepoints, 0x6C34));
        assert(utf16le_appendCodepoint(&fromCodepoints, 0x10000));
        assert(utf16le_appendCodepoint(&fromCodepoints, 0x1D11E));
        assert(utf16le_appendCodepoint(&fromCodepoints, 0x10FFFD));
    }

    assert(str_equals(expected.string, fromCodepoints.string));

    return true;
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
