#include "test.h"
#include "testStringBuilder.h"



//
// Tests
//

bool test_strbuilder_create() {
    StringBuilder builder = strbuilder_create(32);
    {
        assert(buf_isValid(builder.buffer));
        assert(builder.buffer.size == 32);
        assert(builder.length == 0);
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_destroy() {
    return true;
}

bool test_strbuilder_getStringCopy() {
    String expected = str_create("Apple pie is super nice man");
    StringBuilder builder = strbuilder_create(0);
    String builtString;
    {
        strbuilder_appendC(&builder, "Apple");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendC(&builder, "pie");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendC(&builder, "is");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendC(&builder, "super");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendC(&builder, "nice");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendC(&builder, "man");

        builtString = strbuilder_getCopy(builder);

        assert(str_equals(builtString, expected));
        assert(str_equals(strbuilder_get(builder), builtString));
        assert(strbuilder_get(builder).data != builtString.data);
    }
    strbuilder_destroy(&builder);
    str_destroy(&builtString);

    return true;
}

bool test_strbuilder_setCapacity() {
    StringBuilder builder = strbuilder_create(32);
    strbuilder_appendC(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.buffer.size == 32);

        assertSuccess(strbuilder_setCapacity(&builder, 24));
        assert(builder.buffer.size == 24);

        assertSuccess(strbuilder_setCapacity(&builder, 80));
        assert(builder.buffer.size == 80);

        assert(strbuilder_setCapacity(&builder, 20) != ERROR_SUCCESS);
        assert(strbuilder_isErrored(builder));
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_ensureCapacity() {
    StringBuilder builder = strbuilder_create(32);
    strbuilder_appendC(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.buffer.size == 32);

        assertSuccess(strbuilder_ensureCapacity(&builder, 24));
        assert(builder.buffer.size >= 24);

        assertSuccess(strbuilder_ensureCapacity(&builder, 80));
        assert(builder.buffer.size >= 80);

        assertSuccess(strbuilder_ensureCapacity(&builder, 20));
        assert(builder.buffer.size >= 20);
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_trimToLength() {
    StringBuilder builder = strbuilder_create(32);
    strbuilder_appendC(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.buffer.size == 32);

        assertSuccess(strbuilder_trimToLength(&builder));
        assert(builder.buffer.size == 24);

        strbuilder_appendC(&builder, "...");

        assertSuccess(strbuilder_trimToLength(&builder));
        assert(builder.buffer.size == 27);
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_appendChar() {
    String expected = str_create("aBc123-_\\[ ");
    StringBuilder builder = strbuilder_create(0);
    {
        strbuilder_appendChar(&builder, 'a');
        strbuilder_appendChar(&builder, 'B');
        strbuilder_appendChar(&builder, 'c');
        strbuilder_appendChar(&builder, '1');
        strbuilder_appendChar(&builder, '2');
        strbuilder_appendChar(&builder, '3');
        strbuilder_appendChar(&builder, '-');
        strbuilder_appendChar(&builder, '_');
        strbuilder_appendChar(&builder, '\\');
        strbuilder_appendChar(&builder, '[');
        strbuilder_appendChar(&builder, ' ');

        assert(str_equals(strbuilder_get(builder), expected));
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_append() {
    String expected = str_create("Apple pie is super nice man");
    StringBuilder builder = strbuilder_create(0);
    {
        strbuilder_append(&builder, str_create("Apple "));
        strbuilder_append(&builder, str_create("pie "));
        strbuilder_append(&builder, str_create("is "));
        strbuilder_append(&builder, str_create("super "));
        strbuilder_append(&builder, str_create("nice "));
        strbuilder_append(&builder, str_create("man"));

        assert(str_equals(strbuilder_get(builder), expected));
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_appendC() {
    String expected = str_create("Apple pie is super nice man");
    StringBuilder builder = strbuilder_create(0);
    {
        strbuilder_appendC(&builder, "Apple ");
        strbuilder_appendC(&builder, "pie ");
        strbuilder_appendC(&builder, "is ");
        strbuilder_appendC(&builder, "super ");
        strbuilder_appendC(&builder, "nice ");
        strbuilder_appendC(&builder, "man");

        assert(str_equals(strbuilder_get(builder), expected));
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_appendSubstring() {
    String expected = str_create("Apple pie is super nice man");
    StringBuilder builder = strbuilder_create(0);
    {
        strbuilder_appendSubstring(&builder, str_create("Apple apple "), 0, 6);
        strbuilder_appendSubstring(&builder, str_create("pie pie "), 0, 4);
        strbuilder_appendSubstring(&builder, str_create("is is "), 3, 6);
        strbuilder_appendSubstring(&builder, str_create("super super "), 0, 6);
        strbuilder_appendSubstring(&builder, str_create("nice nice "), 5, 10);
        strbuilder_appendSubstring(&builder, str_create("man man"), 0, 3);

        assert(str_equals(strbuilder_get(builder), expected));
    }
    strbuilder_destroy(&builder);

    return true;
}



//
// Run Tests
//

void test_StringBuilder(int * failures, int * successes) {
    test(strbuilder_create);
    test(strbuilder_destroy);
    test(strbuilder_getStringCopy);
    test(strbuilder_setCapacity);
    test(strbuilder_ensureCapacity);
    test(strbuilder_trimToLength);

    test(strbuilder_appendChar);
    test(strbuilder_append);
    test(strbuilder_appendC);
    test(strbuilder_appendSubstring);
}
