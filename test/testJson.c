#include "test.h"
#include "testJson.h"
#include "../src/json.h"

//
// Tests
//

bool test_json_quoteString() {
    return false;
}

bool test_json_quoteStringInto() {
    return false;
}

bool test_json_parse() {
    String input = str_createCopy("[true, -0.05134e+70, \"\\uDBFF\\uDC00\", false, null, \n{\"a\\npple\": true,\"!!\" :\"wat\"}]");

    printf("Input:\n%s\n\n", str_toCString(input));

    JsonParseResult result = json_parse(input);

    if(result.isError) {
        json_parse_printError(result);
        return false;
    }

    printf("Result:\n%s\n", str_toCString(json_prettify(result.root)));

    return true;
}

bool test_json_parseInto() {
    return false;
}

bool test_json_stringify() {
    return false;
}

bool test_json_stringifyInto() {
    return false;
}

bool test_json_prettify() {
    return false;
}

bool test_json_prettifyInto() {
    return false;
}

bool test_json_failFile(char * filename, char * expectedError) {
    String file = str_readFile(filename);

    JsonParseResult result = json_parse(file);

    if(!result.isError)
        return false;

    String expected = str_create(expectedError);

    if(!str_equals(result.error, expected)) {
        json_parse_printError(result);
        return false;
    }

    return true;
}

#define fail(number, expectedError)                                                   \
    bool test_json_fail##number() {                                                   \
        return test_json_failFile("jsonTests/fail" #number ".json", expectedError);   \
    }

fail(2, "Unexpected eof while parsing array")
fail(3, "Expected \" for start of key while parsing object")
fail(4, "Unexpected character while expecting value")
fail(5, "Unexpected character while expecting value")
fail(6, "Unexpected character while expecting value")
fail(7, "Remaining characters after completion of parsing")
fail(8, "Remaining characters after completion of parsing")
fail(9, "Expected \" for start of key while parsing object")
fail(10, "Remaining characters after completion of parsing")
fail(11, "Expected , or } while parsing object")
fail(12, "Unexpected character while expecting value")
fail(13, "Unexpected digit after 0 in number")
fail(14, "Expected , or } while parsing object")
fail(15, "Invalid escape sequence")
fail(16, "Unexpected character while expecting value")
fail(17, "Invalid escape sequence")
fail(19, "Expected : while parsing object")
fail(20, "Unexpected character while expecting value")
fail(21, "Expected : while parsing object")
fail(22, "Expected , or ] while parsing array")
fail(23, "Expected true")
fail(24, "Unexpected character while expecting value")
fail(25, "Control character in string")
fail(26, "Invalid escape sequence")
fail(27, "Control character in string")
fail(29, "Expected digit in exponent of number")
fail(30, "Expected digit in exponent of number")
fail(31, "Expected digit in exponent of number")
fail(32, "Unexpected eof while parsing object")
fail(33, "Expected , or ] while parsing array")

bool test_json_passFile(char * filename) {
    String file = str_readFile(filename);

    JsonParseResult result = json_parse(file);

    if(result.isError) {
        json_parse_printError(result);
    }

    return !result.isError;
}

#define pass(number)                                         \
    bool test_json_pass##number() {                          \
        return test_json_passFile("jsonTests/pass" #number ".json");   \
    }

pass(1) pass(2) pass(3)



//
// Run Tests
//

#define f(number) test(json_fail##number)
#define p(number) test(json_pass##number)

void test_json(int * failures, int * successes) {
    test(json_quoteString);
    test(json_quoteStringInto);
    test(json_parse);
    test(json_parseInto);
    test(json_stringify);
    test(json_stringifyInto);
    test(json_prettify);
    test(json_prettifyInto);

    // Test jsonTests/fail{number}.json files
    f(2);  f(3);  f(4);  f(5);  f(6);  f(7);  f(8);  f(9);  f(10); f(11);
    f(12); f(13); f(14); f(15); f(16); f(17); f(19); f(20); f(21); f(22);
    f(23); f(24); f(25); f(26); f(27); f(29); f(30); f(31); f(32); f(33);

    // Test jsonTests/pass{number}.json files
    p(1); p(2); p(3);
}