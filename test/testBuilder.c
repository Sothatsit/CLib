#include "test.h"
#include "testBuilder.h"



//
// Tests
//

bool test_builder_create() {
    Builder builder = builder_create(32);
    {
        assert(buf_isValid(builder.buffer));
        assert(builder.buffer.size == 32);
        assert(builder.length == 0);
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_destroy() {
    return true;
}

bool test_builder_str() {
    Builder builder = builder_create(32);
    {
        builder_appendC(&builder, "Apples");
        assert(str_equalsC(builder_str(builder), "Apples"));

        builder_appendC(&builder, " are");
        assert(str_equalsC(builder_str(builder), "Apples are"));

        builder_appendStr(&builder, str_create(" delicious"));
        assert(str_equalsC(builder_str(builder), "Apples are delicious"));
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_buf() {
    Builder builder = builder_create(32);
    {
        builder_appendC(&builder, "Apples");
        assert(buf_equals(
            builder_buf(builder),
            buf_createUsingC("Apples")
        ));

        builder_appendC(&builder, " are");
        assert(buf_equals(
            builder_buf(builder),
            buf_createUsingC("Apples are")
        ));

        builder_appendStr(&builder, str_create(" delicious"));
        assert(buf_equals(
            builder_buf(builder),
            buf_createUsingC("Apples are delicious")
        ));
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_strCopy() {
    String expected = str_create("Apple pie is super nice man");
    Builder builder = builder_create(0);
    String builtString;
    {
        builder_appendC(&builder, "Apple");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "pie");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "is");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "super");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "nice");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "man");

        builtString = builder_strCopy(builder);

        assert(str_equals(builtString, expected));
        assert(str_equals(builder_str(builder), builtString));
        assert(builder_str(builder).data != builtString.data);
    }
    builder_destroy(&builder);
    str_destroy(&builtString);

    return true;
}

bool test_builder_bufCopy() {
    Buffer expected = buf_createUsingC("Apple pie is super nice man");
    Builder builder = builder_create(0);
    Buffer builtBuffer;
    {
        builder_appendC(&builder, "Apple");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "pie");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "is");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "super");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "nice");
        builder_appendChar(&builder, ' ');
        builder_appendC(&builder, "man");

        builtBuffer = builder_bufCopy(builder);

        assert(buf_equals(builtBuffer, expected));
        assert(buf_equals(builder_buf(builder), builtBuffer));
        assert(builder_buf(builder).start != builtBuffer.start);
    }
    builder_destroy(&builder);
    buf_destroy(&builtBuffer);

    return true;
}

bool test_builder_setCapacity() {
    Builder builder = builder_create(32);
    builder_appendC(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.buffer.size == 32);

        assertSuccess(builder_setCapacity(&builder, 24));
        assert(builder.buffer.size == 24);

        assertSuccess(builder_setCapacity(&builder, 80));
        assert(builder.buffer.size == 80);

        assert(builder_setCapacity(&builder, 20) != ERROR_SUCCESS);
        assert(builder_isErrored(builder));
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_ensureCapacity() {
    Builder builder = builder_create(32);
    builder_appendC(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.buffer.size == 32);

        assertSuccess(builder_ensureCapacity(&builder, 24));
        assert(builder.buffer.size >= 24);

        assertSuccess(builder_ensureCapacity(&builder, 80));
        assert(builder.buffer.size >= 80);

        assertSuccess(builder_ensureCapacity(&builder, 20));
        assert(builder.buffer.size >= 20);
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_trimToLength() {
    Builder builder = builder_create(32);
    builder_appendC(&builder, "Testing testing, 1, 2, 3");
    {
        assert(builder.buffer.size == 32);

        assertSuccess(builder_trimToLength(&builder));
        assert(builder.buffer.size == 24);

        builder_appendC(&builder, "...");

        assertSuccess(builder_trimToLength(&builder));
        assert(builder.buffer.size == 27);
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_appendChar() {
    String expected = str_create("aBc123-_\\[ ");
    Builder builder = builder_create(0);
    {
        builder_appendChar(&builder, 'a');
        builder_appendChar(&builder, 'B');
        builder_appendChar(&builder, 'c');
        builder_appendChar(&builder, '1');
        builder_appendChar(&builder, '2');
        builder_appendChar(&builder, '3');
        builder_appendChar(&builder, '-');
        builder_appendChar(&builder, '_');
        builder_appendChar(&builder, '\\');
        builder_appendChar(&builder, '[');
        builder_appendChar(&builder, ' ');

        assert(str_equals(builder_str(builder), expected));
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_appendStr() {
    String expected = str_create("Apple pie is super nice man");
    Builder builder = builder_create(0);
    {
        builder_appendStr(&builder, str_create("Apple "));
        builder_appendStr(&builder, str_create("pie "));
        builder_appendStr(&builder, str_create("is "));
        builder_appendStr(&builder, str_create("super "));
        builder_appendStr(&builder, str_create("nice "));
        builder_appendStr(&builder, str_create("man"));

        assert(str_equals(builder_str(builder), expected));
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_appendBuf() {
    Buffer expected = buf_createUsingC("Apple pie is super nice man");
    Builder builder = builder_create(0);
    {
        builder_appendBuf(&builder, buf_createUsingC("Apple "));
        builder_appendBuf(&builder, buf_createUsingC("pie "));
        builder_appendBuf(&builder, buf_createUsingC("is "));
        builder_appendBuf(&builder, buf_createUsingC("super "));
        builder_appendBuf(&builder, buf_createUsingC("nice "));
        builder_appendBuf(&builder, buf_createUsingC("man"));

        assert(buf_equals(builder_buf(builder), expected));
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_appendC() {
    String expected = str_create("Apple pie is super nice man");
    Builder builder = builder_create(0);
    {
        builder_appendC(&builder, "Apple ");
        builder_appendC(&builder, "pie ");
        builder_appendC(&builder, "is ");
        builder_appendC(&builder, "super ");
        builder_appendC(&builder, "nice ");
        builder_appendC(&builder, "man");

        assert(str_equals(builder_str(builder), expected));
    }
    builder_destroy(&builder);

    return true;
}

bool test_builder_appendSubstring() {
    String expected = str_create("Apple pie is super nice man");
    Builder builder = builder_create(0);
    {
        builder_appendSubstring(&builder, str_create("Apple apple "), 0, 6);
        builder_appendSubstring(&builder, str_create("pie pie "), 0, 4);
        builder_appendSubstring(&builder, str_create("is is "), 3, 6);
        builder_appendSubstring(&builder, str_create("super super "), 0, 6);
        builder_appendSubstring(&builder, str_create("nice nice "), 5, 10);
        builder_appendSubstring(&builder, str_create("man man"), 0, 3);

        assert(str_equals(builder_str(builder), expected));
    }
    builder_destroy(&builder);

    return true;
}



//
// Run Tests
//

void test_Builder(int * failures, int * successes) {
    test(builder_create);
    test(builder_destroy);
    test(builder_str);
    test(builder_buf);
    test(builder_strCopy);
    test(builder_bufCopy);
    test(builder_setCapacity);
    test(builder_ensureCapacity);
    test(builder_trimToLength);

    test(builder_appendChar);
    test(builder_appendStr);
    test(builder_appendBuf);
    test(builder_appendC);
    test(builder_appendSubstring);
}
