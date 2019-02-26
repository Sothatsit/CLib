#include "test.h"
#include "testBuffer.h"

//
// Tests
//

#define assertWritable(buffer)                                   \
    do{ for(u64 index = 0; index < buffer.capacity; ++index) {   \
        buffer.start[index] = (u8) index;                        \
    }} while(0)

bool test_buf_create() {
    Buffer buffer = buf_create(64);
    {
        assertNonNull(buffer.start);
        assert(buffer.capacity == 64);
        assertWritable(buffer);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_createUsing() {
    char * data = "ABCDEFGHIJ";

    Buffer buffer = buf_createUsing(data, 10);
    {
        assert(buffer.start == data);
        assert(buffer.capacity == 10);
    }

    return true;
}

bool test_buf_createEmpty() {
    Buffer buffer = buf_createEmpty();
    {
        assert(buffer.start == NULL);
        assert(buffer.capacity == 0);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_copy() {
    char * data = "ABCDEFGHIJ";

    Buffer buffer = buf_createUsing(data, 10);
    Buffer copy = buf_copy(buffer);
    {
        assertNonNull(copy.start);
        assert(copy.start != data);
        assert(buf_equals(buffer, copy));
    }
    buf_destroy(&copy);

    return true;
}

bool test_buf_copyInto() {
    Buffer copyFrom = buf_createUsing("ABCDEFGHIJ", 10);
    Buffer copyTo = buf_create(10);
    {
        assert(buf_copyInto(copyFrom, copyTo));
        assert(buf_equals(copyFrom, copyTo));
    }
    buf_destroy(&copyTo);

    copyTo = buf_create(20);
    {
        assert(buf_copyInto(copyFrom, copyTo));
        assert(memcmp(copyFrom.start, copyTo.start, 10) == 0);
    }
    buf_destroy(&copyTo);

    copyTo = buf_create(5);
    {
        assert(!buf_copyInto(copyFrom, copyTo));
    }
    buf_destroy(&copyTo);

    return true;
}

bool test_buf_isEmpty() {
    Buffer empty = buf_createEmpty();
    Buffer nonEmpty = buf_createUsing("ABC", 3);
    {
        assert(buf_isEmpty(empty));
        assert(!buf_isEmpty(nonEmpty));
    }

    return true;
}

bool test_buf_destroy() {
    Buffer buffer = buf_create(64);
    {
        assertWritable(buffer);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_setCapacity() {
    Buffer buffer = buf_create(24);
    {
        assert(buffer.capacity == 24);
        assertWritable(buffer);

        assert(buf_setCapacity(&buffer, 16));
        assert(buffer.capacity == 16);
        assertWritable(buffer);

        assert(buf_setCapacity(&buffer, 80));
        assert(buffer.capacity == 80);
        assertWritable(buffer);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_ensureCapacity() {
    Buffer buffer = buf_create(24);
    {
        assert(buffer.capacity == 24);
        assertWritable(buffer);

        assert(buf_ensureCapacity(&buffer, 16));
        assert(buffer.capacity == 24);
        assertWritable(buffer);

        assert(buf_ensureCapacity(&buffer, 80));
        assert(buffer.capacity == 128);
        assertWritable(buffer);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_equals() {
    Buffer buffer = buf_createUsing("ABCDEFGHIJK", 11);
    Buffer backwards = buf_createUsing("KJIHGFEDCBA", 11);
    Buffer longer = buf_createUsing("ABCDEFGHIKLMNOP", 15);
    Buffer shorter = buf_createUsing("ABCDEFGH", 8);
    Buffer constructed = buf_create(11);
    {
        for(u64 index = 0; index < 11; ++index) {
            constructed.start[index] = (char) ('A' + index);
        }

        assert(buf_equals(buffer, constructed));
        assert(!buf_equals(buffer, backwards));
        assert(!buf_equals(buffer, longer));
        assert(!buf_equals(buffer, shorter));
    }
    buf_destroy(&constructed);

    return true;
}



//
// Run Tests
//

void test_Buffer(int * failures, int * successes) {
    test(buf_create);
    test(buf_createUsing);
    test(buf_createEmpty);
    test(buf_copy);
    test(buf_copyInto);
    test(buf_isEmpty);
    test(buf_destroy);
    test(buf_setCapacity);
    test(buf_ensureCapacity);
    test(buf_equals);
}
