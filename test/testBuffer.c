#include "test.h"
#include "testBuffer.h"

//
// Tests
//

#define assertWritable(buffer)                               \
    do{ for(u64 index = 0; index < buffer.size; ++index) {   \
        buffer.start[index] = (u8) index;                    \
    }} while(0)

bool test_buf_create() {
    Buffer buffer = buf_create(64);
    {
        assertNonNull(buffer.start);
        assert(buffer.size == 64);
        assertWritable(buffer);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_createUsing() {
    char * data = "ABCDEFGHIJ";

    Buffer full = buf_createUsing(data, 10);
    Buffer shorter = buf_createUsing(data, 5);
    Buffer empty = buf_createUsing(data, 0);
    Buffer errored = buf_createUsing(NULL, 5);
    {
        assert(full.start == data);
        assert(full.size == 10);

        assert(shorter.start == data);
        assert(shorter.size == 5);

        assert(empty.start == NULL);
        assert(empty.size == 0);

        assert(buf_isErrored(errored));
    }

    return true;
}

bool test_buf_createUsingC() {
    char * data = "ABCDEFGHIJ";

    Buffer buffer = buf_createUsingC(data);
    Buffer errored = buf_createUsingC(NULL);
    {
        assert(buffer.start == data);
        assert(buffer.size == 10);

        assert(buf_isErrored(errored));
    }

    return true;
}

bool test_buf_createEmpty() {
    Buffer buffer = buf_createEmpty();
    {
        assert(buffer.start == NULL);
        assert(buffer.size == 0);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_createErrored() {
    Buffer no_error = buf_createErrored(ERROR_NONE, 4);
    assert(buf_getErrorType(no_error) == ERROR_ARG_INVALID);
    assert(buf_getErrorNum(no_error) == 0);

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            Buffer errored = buf_createErrored(errorType, errnum);

            assert(buf_getErrorType(errored) == errorType);
            assert(buf_getErrorNum(errored) == errnum);
            assert(buf_isErrored(errored));
            assert(!buf_isValid(errored));
        }
    }

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
        assertSuccess(buf_copyInto(copyTo, copyFrom, 0));
        assert(buf_equals(copyTo, copyFrom));
    }
    buf_destroy(&copyTo);

    copyTo = buf_create(20);
    {
        assertSuccess(buf_copyInto(copyTo, copyFrom, 0));
        assert(memcmp(copyFrom.start, copyTo.start, 10) == 0);
    }
    buf_destroy(&copyTo);

    copyTo = buf_create(5);
    {
        assert(buf_copyInto(copyTo, copyFrom, 0) != ERROR_SUCCESS);
    }
    buf_destroy(&copyTo);

    return true;
}

bool test_buf_isEmpty() {
    Buffer empty = buf_createEmpty();
    Buffer nonEmpty = buf_createUsing("ABC", 3);
    Buffer errored = buf_createErrored(ERROR_UNKNOWN, 0);
    {
        assert(buf_isEmpty(empty));
        assert(!buf_isEmpty(nonEmpty));
        assert(!buf_isEmpty(errored));
    }

    return true;
}

bool test_buf_isValid() {
    Buffer empty = buf_createEmpty();
    Buffer nonEmpty = buf_createUsing("ABC", 3);
    Buffer errored = buf_createErrored(ERROR_UNKNOWN, 0);
    {
        assert(buf_isValid(empty));
        assert(buf_isValid(nonEmpty));
        assert(!buf_isValid(errored));
    }

    return true;
}

bool test_buf_isErrored() {
    assert(!buf_isErrored(buf_createUsing("ABC", 3)));
    assert(!buf_isErrored(buf_createEmpty()));

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            Buffer errored = buf_createErrored(errorType, errnum);

            assert(buf_isErrored(errored));
        }
    }

    return true;
}

bool test_buf_getErrorType() {
    assert(buf_getErrorType(buf_createUsing("ABC", 3)) == ERROR_NONE);
    assert(buf_getErrorType(buf_createEmpty()) == ERROR_NONE);

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            Buffer errored = buf_createErrored(errorType, errnum);

            assert(buf_getErrorType(errored) == errorType);
        }
    }

    return true;
}

bool test_buf_getErrorNum() {
    assert(buf_getErrorNum(buf_createUsing("ABC", 3)) == 0);
    assert(buf_getErrorNum(buf_createEmpty()) == 0);

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            Buffer errored = buf_createErrored(errorType, errnum);

            assert(buf_getErrorNum(errored) == errnum);
        }
    }

    return true;
}

bool test_buf_getErrorReason() {
    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            Buffer errored = buf_createErrored(errorType, errnum);
            String reason = buf_getErrorReason(errored);
            {
                assert(str_containsStr(reason, errtype_str(errorType)));
                if (errnum != 0) {
                    assert(str_containsC(reason, strerror(errnum)));
                }
            }
            str_destroy(&reason);
        }
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
        assert(buffer.size == 24);
        assertWritable(buffer);

        assertSuccess(buf_setCapacity(&buffer, 16));
        assert(buffer.size == 16);
        assertWritable(buffer);

        assertSuccess(buf_setCapacity(&buffer, 80));
        assert(buffer.size == 80);
        assertWritable(buffer);
    }
    buf_destroy(&buffer);

    return true;
}

bool test_buf_ensureCapacity() {
    Buffer buffer = buf_create(24);
    {
        assert(buffer.size == 24);
        assertWritable(buffer);

        assertSuccess(buf_ensureCapacity(&buffer, 16));
        assert(buffer.size == 24);
        assertWritable(buffer);

        assertSuccess(buf_ensureCapacity(&buffer, 80));
        assert(buffer.size == 128);
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
    test(buf_createErrored);
    test(buf_createEmpty);
    test(buf_copy);
    test(buf_copyInto);
    test(buf_isEmpty);
    test(buf_isValid);
    test(buf_isErrored);
    test(buf_getErrorType);
    test(buf_getErrorNum);
    test(buf_getErrorReason);
    test(buf_destroy);
    test(buf_setCapacity);
    test(buf_ensureCapacity);
    test(buf_equals);
}
