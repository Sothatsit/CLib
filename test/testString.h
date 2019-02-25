#ifndef __CLIB_testString_h
#define __CLIB_testString_h

/*
 * Assert {string} is not empty and contains data.
 */
#define assertStrValid(string)                                                       \
    do {                                                                             \
        assertOrError(str_isValid(string), #string " is unexpectedly invalid");      \
        assertOrError(!str_isErrored(string), #string " is unexpectedly errored");   \
        assertNonNull(string.data);                                                  \
    } while(0)

/*
 * Test the String type.
 */
void test_String(int * failures, int * successes);

#endif
