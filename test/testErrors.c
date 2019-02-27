#include "test.h"
#include "testErrors.h"

//
// Tests
//

bool test_errtype_c() {
    char * unknown = errtype_c(ERROR_UNKNOWN);

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        char * string = errtype_c(errorType);

        if (errorType == ERROR_UNKNOWN) {
            assert(strcmp(string, unknown) == 0);
        } else {
            assert(strcmp(string, unknown) != 0);
        }
    }

    assert(strcmp(errtype_c(ERROR_COUNT), unknown) == 0);
    assert(strcmp(errtype_c(ERROR_COUNT + 5), unknown) == 0);

    return true;
}

bool test_errtype_str() {
    String unknown = errtype_str(ERROR_UNKNOWN);

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        String string = errtype_str(errorType);

        if (errorType == ERROR_UNKNOWN) {
            assert(str_equals(string, unknown));
        } else {
            assert(!str_equals(string, unknown));
        }
    }

    assert(str_equals(errtype_str(ERROR_COUNT), unknown));
    assert(str_equals(errtype_str(ERROR_COUNT + 5), unknown));

    return true;
}

bool test_err_create() {
    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            assert(err_create(errorType, errnum) <= -1);
        }
    }

    return true;
}

bool test_err_reason() {
    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            String reason = err_reason(err_create(errorType, errnum));

            assert(str_containsStr(reason, errtype_str(errorType)));
            if (errnum != 0) {
                assert(str_containsC(reason, strerror(errnum)));
            }
        }
    }

    return true;
}

bool test_err_type() {
    assert(err_type(-ERROR_COUNT) == ERROR_INVALID);
    assert(err_type(0) == ERROR_NONE);
    assert(err_type(5) == ERROR_NONE);
    assert(err_type(234) == ERROR_NONE);
    assert(err_type(1532) == ERROR_NONE);

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            assert(err_type(err_create(errorType, errnum)) == errorType);
        }
    }

    return true;
}

bool test_err_num() {
    assert(err_num(-1) == 0);
    assert(err_num(0) == 0);
    assert(err_num(5) == 0);
    assert(err_num(234) == 0);
    assert(err_num(525323) == 0);

    for (CLibErrorType errorType = ERROR_FIRST; errorType <= ERROR_LAST; ++errorType) {
        for (int errnum = 0; errnum < sys_nerr; ++errnum) {
            assert(err_num(err_create(errorType, errnum)) == errnum);
        }
    }

    return true;
}



//
// Run Tests
//

void test_errors(int * failures, int * successes) {
    test(errtype_c);
    test(errtype_str);
    test(err_create);
    test(err_reason);
    test(err_type);
    test(err_num);
}
