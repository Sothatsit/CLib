#include "test.h"
#include "testBuffer.h"

//
// Tests
//

#define assertWritable(buffer)                                   \
    do{ for(u64 index = 0; index < buffer.capacity; ++index) {   \
        buffer.start[index] = (u8) index;                        \
    }} while(0)

bool test_buffer_create() {
    Buffer buffer = buffer_create(64);
    {
        assertNonNull(buffer.start);
        assert(buffer.capacity == 64);
        assertWritable(buffer);
    }
    buffer_destroy(&buffer);

    return true;
}

bool test_buffer_createUsing() {
    char * data = "ABCDEFGHIJ";

    Buffer buffer = buffer_createUsing(data, 10);
    {
        assert(buffer.start == data);
        assert(buffer.capacity == 10);
    }

    return true;
}

bool test_buffer_createEmpty() {
    Buffer buffer = buffer_createEmpty();
    {
        assert(buffer.start == NULL);
        assert(buffer.capacity == 0);
    }
    buffer_destroy(&buffer);

    return true;
}

bool test_buffer_copy() {
    char * data = "ABCDEFGHIJ";

    Buffer buffer = buffer_createUsing(data, 10);
    Buffer copy = buffer_copy(buffer);
    {
        assertNonNull(copy.start);
        assert(copy.start != data);
        assert(buffer_equals(buffer, copy));
    }
    buffer_destroy(&copy);

    return true;
}

bool test_buffer_copyInto() {
    Buffer copyFrom = buffer_createUsing("ABCDEFGHIJ", 10);
    Buffer copyTo = buffer_create(10);
    {
        assert(buffer_copyInto(copyFrom, copyTo));
        assert(buffer_equals(copyFrom, copyTo));
    }
    buffer_destroy(&copyTo);

    copyTo = buffer_create(20);
    {
        assert(buffer_copyInto(copyFrom, copyTo));
        assert(memcmp(copyFrom.start, copyTo.start, 10) == 0);
    }
    buffer_destroy(&copyTo);

    copyTo = buffer_create(5);
    {
        assert(!buffer_copyInto(copyFrom, copyTo));
    }
    buffer_destroy(&copyTo);

    return true;
}

bool test_buffer_isEmpty() {
    Buffer empty = buffer_createEmpty();
    Buffer nonEmpty = buffer_createUsing("ABC", 3);
    {
        assert(buffer_isEmpty(empty));
        assert(!buffer_isEmpty(nonEmpty));
    }

    return true;
}

bool test_buffer_destroy() {
    Buffer buffer = buffer_create(64);
    {
        assertWritable(buffer);
    }
    buffer_destroy(&buffer);

    return true;
}

bool test_buffer_setCapacity() {
    Buffer buffer = buffer_create(24);
    {
        assert(buffer.capacity == 24);
        assertWritable(buffer);

        assert(buffer_setCapacity(&buffer, 16));
        assert(buffer.capacity == 16);
        assertWritable(buffer);

        assert(buffer_setCapacity(&buffer, 80));
        assert(buffer.capacity == 80);
        assertWritable(buffer);
    }
    buffer_destroy(&buffer);

    return true;
}

bool test_buffer_ensureCapacity() {
    Buffer buffer = buffer_create(24);
    {
        assert(buffer.capacity == 24);
        assertWritable(buffer);

        assert(buffer_ensureCapacity(&buffer, 16));
        assert(buffer.capacity == 24);
        assertWritable(buffer);

        assert(buffer_ensureCapacity(&buffer, 80));
        assert(buffer.capacity == 128);
        assertWritable(buffer);
    }
    buffer_destroy(&buffer);

    return true;
}

bool test_buffer_equals() {
    Buffer buffer = buffer_createUsing("ABCDEFGHIJK", 11);
    Buffer backwards = buffer_createUsing("KJIHGFEDCBA", 11);
    Buffer longer = buffer_createUsing("ABCDEFGHIKLMNOP", 15);
    Buffer shorter = buffer_createUsing("ABCDEFGH", 8);
    Buffer constructed = buffer_create(11);
    {
        for(u64 index = 0; index < 11; ++index) {
            constructed.start[index] = (char) ('A' + index);
        }

        assert(buffer_equals(buffer, constructed));
        assert(!buffer_equals(buffer, backwards));
        assert(!buffer_equals(buffer, longer));
        assert(!buffer_equals(buffer, shorter));
    }
    buffer_destroy(&constructed);

    return true;
}



//
// Run Tests
//

void test_Buffer(int * failures, int * successes) {
    test(buffer_create);
    test(buffer_createUsing);
    test(buffer_createEmpty);
    test(buffer_copy);
    test(buffer_copyInto);
    test(buffer_isEmpty);
    test(buffer_destroy);
    test(buffer_setCapacity);
    test(buffer_ensureCapacity);
    test(buffer_equals);
}