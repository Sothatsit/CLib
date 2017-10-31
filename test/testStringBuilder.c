#include "test.h"
#include "testStringBuilder.h"



//
// Tests
//

bool test_strbuilder_create() {
    StringBuilder builder = strbuilder_create(32);
    {
        assert(builder.capacity == 32);
        assert(str_isEmpty(builder.string));
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
        strbuilder_appendCString(&builder, "Apple");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendCString(&builder, "pie");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendCString(&builder, "is");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendCString(&builder, "super");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendCString(&builder, "nice");
        strbuilder_appendChar(&builder, ' ');
        strbuilder_appendCString(&builder, "man");

        builtString = strbuilder_getStringCopy(builder);

        assert(str_equals(builtString, expected));
        assert(str_equals(builder.string, builtString));
        assert(builder.string.data != builtString.data);
    }
    strbuilder_destroy(&builder);
    str_destroy(&builtString);

    return true;
}

bool test_strbuilder_setCapacity() {
    StringBuilder builder = strbuilder_create(32);
    strbuilder_appendCString(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.capacity == 32);

        assert(strbuilder_setCapacity(&builder, 24));
        assert(builder.capacity == 24);

        assert(strbuilder_setCapacity(&builder, 80));
        assert(builder.capacity == 80);

        assert(!strbuilder_setCapacity(&builder, 20));
        assert(builder.capacity == 80);
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_ensureCapacity() {
    StringBuilder builder = strbuilder_create(32);
    strbuilder_appendCString(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.capacity == 32);

        assert(strbuilder_ensureCapacity(&builder, 24));
        assert(builder.capacity >= 24);

        assert(strbuilder_ensureCapacity(&builder, 80));
        assert(builder.capacity >= 80);

        assert(strbuilder_ensureCapacity(&builder, 20));
        assert(builder.capacity >= 20);
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_trimToLength() {
    StringBuilder builder = strbuilder_create(32);
    strbuilder_appendCString(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.capacity == 32);

        assert(strbuilder_trimToLength(&builder));
        assert(builder.capacity == 24);

        strbuilder_appendCString(&builder, "...");

        assert(strbuilder_trimToLength(&builder));
        assert(builder.capacity == 27);
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

        assert(str_equals(builder.string, expected));
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_appendString() {
    String expected = str_create("Apple pie is super nice man");
    StringBuilder builder = strbuilder_create(0);
    {
        strbuilder_appendString(&builder, str_create("Apple "));
        strbuilder_appendString(&builder, str_create("pie "));
        strbuilder_appendString(&builder, str_create("is "));
        strbuilder_appendString(&builder, str_create("super "));
        strbuilder_appendString(&builder, str_create("nice "));
        strbuilder_appendString(&builder, str_create("man"));

        assert(str_equals(builder.string, expected));
    }
    strbuilder_destroy(&builder);

    return true;
}

bool test_strbuilder_appendCString() {
    String expected = str_create("Apple pie is super nice man");
    StringBuilder builder = strbuilder_create(0);
    {
        strbuilder_appendCString(&builder, "Apple ");
        strbuilder_appendCString(&builder, "pie ");
        strbuilder_appendCString(&builder, "is ");
        strbuilder_appendCString(&builder, "super ");
        strbuilder_appendCString(&builder, "nice ");
        strbuilder_appendCString(&builder, "man");

        assert(str_equals(builder.string, expected));
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

        assert(str_equals(builder.string, expected));
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
    test(strbuilder_appendString);
    test(strbuilder_appendCString);
    test(strbuilder_appendSubstring);
}