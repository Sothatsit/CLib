#ifndef __CLIB_testString_h
#define __CLIB_testString_h

/*
 * Assert {string} is not empty and contains data.
 */
#define assertStrValid(string)                                                    \
    do {                                                                          \
        assertOrError(!str_isEmpty(string), #string " is unexpectedly empty");    \
        assertNonNull(string.data);                                               \
    } while(0)

/*
 * Test the String type.
 */
void test_String(int * failures, int * successes);

#endif