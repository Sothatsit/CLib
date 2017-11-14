#include "test.h"
#include "testJson.h"
#include "../src/json.h"

//
// Tests
//

bool test_json_parseContext_create() {
    return false;
}

bool test_json_parseContext_destroy() {
    return true;
}

bool test_json_parseContext_parse() {
    return false;
}

bool test_json_parse() {
    return false;
}

bool test_json_parseResult_copy() {
    return false;
}

bool test_json_parseResult_destroy() {
    return true;
}

bool test_json_parseResult_printError() {
    return true;
}

bool test_json_quoteString() {
    return false;
}

bool test_json_quoteStringInto() {
    return false;
}

bool test_json_stringify() {
    return false;
}

bool test_json_stringifyInto() {
    return false;
}

bool test_json_failFile(char * filename, char * expectedError) {
    String file = str_readFile(filename);

    JsonParseResult result = json_parse(file);

    if(!result.isError) {
        json_parseResult_destroy(&result);
        return false;
    }

    String expected = str_create(expectedError);

    if(!str_equals(result.error, expected)) {
        json_parseResult_printError(result);
        return false;
    }

    return true;
}

#define fail(number, expectedError)                                                   \
    bool test_json_fail##number() {                                                   \
        return test_json_failFile("jsonTests/fail/fail" #number ".json", expectedError);   \
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

#define assertNode(node, expectedType, expectedParentType) \
    do {                                                   \
        assert((node).type == expectedType);               \
        assert((node).parentType == expectedParentType);   \
    } while(0);

#define assertChildNode(root, object, index, expectedType)   \
    assertNode(json_obj_get(root, object, index), expectedType, (object).type);

#define assertChildString(root, object, index, expectedString)                                            \
    do {                                                                                                  \
        assertChildNode(root, object, index, JSON_STRING);                                                \
        assert(str_equals(json_obj_get(root, object, index).value.string, str_create(expectedString)));   \
    } while(0);

#define assertChildNumber(root, object, index, expectedNumber)                                            \
    do {                                                                                                  \
        assertChildNode(root, object, index, JSON_NUMBER);                                                \
        assert(str_equals(json_obj_get(root, object, index).value.string, str_create(expectedNumber)));   \
    } while(0);

#define assertChildObject(root, array, index, objectType, expectedSize)               \
    do {                                                                              \
        assertChildNode(root, array, index, objectType);                              \
        assert(json_obj_get(root, array, index).value.object.size == expectedSize);   \
    } while(0);

#define assertObjectNode(root, object, key, expectedType)   \
    assertChildNode(root, object, json_obj_findIndex(root, object, str_create(key)), expectedType);

#define assertObjectString(root, object, key, expectedString)   \
    assertChildString(root, object, json_obj_findIndex(root, object, str_create(key)), expectedString);

#define assertObjectNumber(root, object, key, expectedNumber)   \
    assertChildNumber(root, object, json_obj_findIndex(root, object, str_create(key)), expectedNumber);

#define assertObjectObject(root, object, key, objectType, expectedSize)   \
    assertChildObject(root, object, json_obj_findIndex(root, object, str_create(key)), objectType, expectedSize);

bool test_json_pass1() {
    String file = str_readFile("jsonTests/pass/pass1.json");

    JsonParseResult result = json_parse(file);

    if(result.isError) {
        json_parseResult_printError(result);
        return false;
    }

    JsonRoot root = result.root;
    JsonNode array = result.root.nodes[0];

    assertNode(array, JSON_ARRAY, JSON_NONE);
    assert(array.key.index == 0);
    assert(array.value.object.size == 20);
    {
        assertChildString(root, array, 0, "JSON Test Pattern pass1");
        assertChildObject(root, array, 1, JSON_OBJECT, 1);
        {
            JsonNode object = json_obj_get(root, array, 1);
            JsonNode child = json_obj_find(root, object, str_create("object with 1 member"));

            assertNode(child, JSON_ARRAY, JSON_OBJECT);

            JsonNode innerString = json_obj_get(root, child, 0);

            assertNode(innerString, JSON_STRING, JSON_ARRAY);
            assert(str_equals(innerString.value.string, str_create("array with 1 element")));
        }
        assertChildObject(root, array, 2, JSON_OBJECT, 0);
        assertChildObject(root, array, 3, JSON_ARRAY, 0);
        assertChildNumber(root, array, 4, "-42");
        assertChildNode(root, array, 5, JSON_TRUE);
        assertChildNode(root, array, 6, JSON_FALSE);
        assertChildNode(root, array, 7, JSON_NULL);
        assertChildObject(root, array, 8, JSON_OBJECT, 32);
        {
            JsonNode object = json_obj_get(root, array, 8);

            assertObjectNumber(root, object, "integer", "1234567890");
            assertObjectNumber(root, object, "real", "-9876.543210");
            assertObjectNumber(root, object, "e", "0.123456789e-12");
            assertObjectNumber(root, object, "E", "1.234567890E+34");
            assertObjectNumber(root, object, "", "23456789012E66");
            assertObjectNumber(root, object, "zero", "0");
            assertObjectNumber(root, object, "one", "1");
            assertObjectString(root, object, "space", " ");
            assertObjectString(root, object, "quote", "\"");
            assertObjectString(root, object, "controls", "\b\f\n\r\t");
            assertObjectString(root, object, "slash", "/ & /");
            assertObjectString(root, object, "alpha", "abcdefghijklmnopqrstuvwyz");
            assertObjectString(root, object, "ALPHA", "ABCDEFGHIJKLMNOPQRSTUVWYZ");
            assertObjectString(root, object, "digit", "0123456789");
            assertObjectString(root, object, "0123456789", "digit");
            assertObjectString(root, object, "special", "`1~!@#$%^&*()_+-={':[,]}|;.</>?");
            assertObjectString(root, object, "hex", "\u0123\u4567\u89AB\uCDEF\uabcd\uef4A");
            assertObjectNode(root, object, "true", JSON_TRUE);
            assertObjectNode(root, object, "false", JSON_FALSE);
            assertObjectNode(root, object, "null", JSON_NULL);
            assertObjectObject(root, object, "array", JSON_ARRAY, 0);
            assertObjectObject(root, object, "object", JSON_OBJECT, 0);
            assertObjectString(root, object, "address", "50 St. James Street");
            assertObjectString(root, object, "url", "http://www.JSON.org/");
            assertObjectObject(root, object, " s p a c e d ", JSON_ARRAY, 7);
            assertObjectObject(root, object, "compact", JSON_ARRAY, 7);
            {
                JsonNode spaced = json_obj_find(root, object, str_create(" s p a c e d "));
                JsonNode compact = json_obj_find(root, object, str_create("compact"));

                for(u32 index = 0; index < 7; ++index) {
                    assertChildNode(root, spaced, index, JSON_NUMBER);
                    assertChildNode(root, compact, index, JSON_NUMBER);

                    JsonNode spacedChild = json_obj_get(root, spaced, index);
                    JsonNode compactChild = json_obj_get(root, compact, index);

                    String numberString = str_format("%d", index + 1);

                    assert(str_equals(spacedChild.value.string, numberString));
                    assert(str_equals(compactChild.value.string, numberString));
                }
            }
            assertObjectString(root, object, "jsontext", "{\"object with 1 member\":[\"array with 1 element\"]}");
            assertObjectString(root, object, "quotes", "&#34; \" %22 0x22 034 &#x22;");
            assertObjectString(root, object,
                "/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?",
                "A key can be any string"
            );
        }
        assertChildNumber(root, array, 9, "0.5");
        assertChildNumber(root, array, 10, "98.6");
        assertChildNumber(root, array, 11, "99.44");
        assertChildNumber(root, array, 12, "1066");
        assertChildNumber(root, array, 13, "1e1");
        assertChildNumber(root, array, 14, "0.1e1");
        assertChildNumber(root, array, 15, "1e-1");
        assertChildNumber(root, array, 16, "1e00");
        assertChildNumber(root, array, 17, "2e+00");
        assertChildNumber(root, array, 18, "2e-00");
        assertChildString(root, array, 19, "rosebud");
    }

    json_parseResult_destroy(&result);
    return true;
}

bool test_json_pass2() {
    String file = str_readFile("jsonTests/pass/pass2.json");

    JsonParseResult result = json_parse(file);

    if(result.isError) {
        json_parseResult_printError(result);
        return false;
    }

    JsonRoot root = result.root;
    JsonNode array = result.root.nodes[0];

    assertNode(array, JSON_ARRAY, JSON_NONE);
    assert(array.key.index == 0);
    assert(array.value.object.size == 1);

    for(u32 index = 0; index < 18; ++index) {
        assertChildNode(root, array, 0, JSON_ARRAY);

        array = json_obj_get(root, array, 0);
    }

    assertChildString(root, array, 0, "Not too deep");

    json_parseResult_destroy(&result);
    return true;
}

bool test_json_pass3() {
    String file = str_readFile("jsonTests/pass/pass3.json");

    JsonParseResult result = json_parse(file);

    if(result.isError) {
        json_parseResult_printError(result);
        return false;
    }

    JsonNode string = result.root.nodes[0];

    assertNode(string, JSON_STRING, JSON_NONE);
    assert(string.key.index == 0);
    assert(str_equals(string.value.string, str_create("The root object does not have to be an object or an array")));

    json_parseResult_destroy(&result);
    return true;
}



//
// Run Tests
//

#define f(number) test(json_fail##number)

void test_json(int * failures, int * successes) {
    test(json_parseContext_create);
    test(json_parseContext_destroy);
    test(json_parseContext_parse);
    test(json_parse);
    test(json_parseResult_copy);
    test(json_parseResult_destroy);
    test(json_parseResult_printError);
    test(json_quoteString);
    test(json_quoteStringInto);
    test(json_stringify);
    test(json_stringifyInto);

    // Test jsonTests/fail/fail{number}.json files
    f(2);  f(3);  f(4);  f(5);  f(6);  f(7);  f(8);  f(9);  f(10); f(11);
    f(12); f(13); f(14); f(15); f(16); f(17); f(19); f(20); f(21); f(22);
    f(23); f(24); f(25); f(26); f(27); f(29); f(30); f(31); f(32); f(33);

    test(json_pass1);
    test(json_pass2);
    test(json_pass3);
}