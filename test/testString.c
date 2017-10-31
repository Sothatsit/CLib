#include "test.h"
#include "testString.h"



//
// Tests
//

bool test_str_create() {
    char * data = "Test String Create";

    String string = str_create(data);
    {
        assertStrValid(string);

        assert(string.length == 18);
        assert(string.data == data);
    }

    return true;
}

bool test_str_createCopy() {
    char * data = "John Doe";

    String johnDoe = str_create(data);
    String johnDoeCopy = str_createCopy(data);
    {
        assertStrValid(johnDoe);
        assertStrValid(johnDoeCopy);

        assert(str_equals(johnDoe, johnDoeCopy));
        assert(johnDoe.data != johnDoeCopy.data);
    }
    str_destroy(&johnDoeCopy);

    return true;
}

bool test_str_createOfLength() {
    String string1 = str_createOfLength("Test String Create Of Length", 11);
    String string2 = str_create("Test String");
    {
        assertStrValid(string1);
        assertStrValid(string2);

        assert(string1.length == 11);
        assert(str_equals(string1, string2));
    }

    return true;
}

bool test_str_createCopyOfLength() {
    char * data = "Test String Create Copy Of Length";

    String string1 = str_createOfLength(data, 11);
    String string2 = str_createCopyOfLength(data, 11);
    {
        assertStrValid(string1);
        assertStrValid(string2);

        assert(str_equals(string1, string2));
        assert(string1.data != string2.data);
    }
    str_destroy(&string2);

    return true;
}

bool test_str_createUninitialised() {
    String string = str_createUninitialised(10);
    String expected = str_createCopy("ABCDEFGHIJ");
    {
        assertStrValid(string);
        assertStrValid(expected);
        assert(string.length == 10);

        for(int index = 0; index < string.length; index++) {
            string.data[index] = (char) ('A' + index);
        }

        assert(str_equals(string, expected));
    }
    str_destroy(&string);
    str_destroy(&expected);

    return true;
}

bool test_str_createEmpty() {
    String empty = str_createEmpty();
    {
        assert(empty.length == 0);
    }
    str_destroy(&empty);

    return true;
}

bool test_str_isEmpty() {
    String empty = str_createEmpty();
    String nonEmpty = str_createCopy("Some random string");
    {
        assertStrValid(nonEmpty);

        assert(str_isEmpty(empty));
        assert(!str_isEmpty(nonEmpty));
    }
    str_destroy(&empty);
    str_destroy(&nonEmpty);

    return true;
}

bool test_str_destroy() {
    return true;
}

bool test_str_toCString() {
    String johnDoe = str_createCopy("John Doe");
    char * nullTerminated = str_toCString(johnDoe);
    {
        assertStrValid(johnDoe);

        assert(strcmp("John Doe", nullTerminated) == 0);
    }
    str_destroy(&johnDoe);
    free(nullTerminated);

    return true;
}

bool test_str_format() {
    String formatted = str_format("%s is %d%% good for you", "Apple Pie", 50);
    String expected = str_createCopy("Apple Pie is 50% good for you");
    {
        assertStrValid(formatted);
        assertStrValid(expected);

        assert(str_equals(formatted, expected));
    }
    str_destroy(&formatted);
    str_destroy(&expected);

    return true;
}

String call_str_vformat(char * format, ...) {
    va_list args;
    va_start(args, format);

    String returnValue = str_vformat(format, args);

    va_end(args);

    return returnValue;
}

bool test_str_vformat() {
    String formatted = call_str_vformat("%s is %d%% good for you", "Apple Pie", 50);
    String expected = str_createCopy("Apple Pie is 50% good for you");
    {
        assertStrValid(formatted);
        assertStrValid(expected);

        assert(str_equals(formatted, expected));
    }
    str_destroy(&formatted);
    str_destroy(&expected);

    return true;
}

bool test_str_copy() {
    String johnDoe = str_create("John Doe");
    String johnDoeCopy = str_copy(johnDoe);
    {
        assertStrValid(johnDoe);
        assertStrValid(johnDoeCopy);

        assert(str_equals(johnDoe, johnDoeCopy));
    }
    str_destroy(&johnDoeCopy);

    return true;
}

bool test_str_equals() {
    String bob = str_createCopy("Bob");
    String jeff = str_createCopy("Jeff");
    String johnDoe = str_createCopyOfLength("John\0Doe", 8);
    String johnPaul = str_createCopyOfLength("John\0Paul", 9);
    {
        assertStrValid(bob);
        assertStrValid(jeff);
        assertStrValid(johnDoe);
        assertStrValid(johnPaul);

        assert(!str_equals(bob, jeff));
        assert(str_equals(bob, bob));
        assert(!str_equals(johnDoe, johnPaul));
        assert(!str_equals(johnPaul, johnDoe));
    }
    str_destroy(&bob);
    str_destroy(&jeff);
    str_destroy(&johnDoe);
    str_destroy(&johnPaul);

    return true;
}

bool test_str_startsWith() {
    String bob = str_createCopy("Bob");
    String jeff = str_createCopy("Bob Jeff");
    String johnDoe = str_createCopyOfLength("John\0Doe", 8);
    String johnPaul = str_createCopyOfLength("John\0Paul", 9);
    {
        assertStrValid(bob);
        assertStrValid(jeff);
        assertStrValid(johnDoe);
        assertStrValid(johnPaul);

        assert(!str_startsWith(bob, jeff));
        assert(str_startsWith(jeff, bob));
        assert(!str_startsWith(johnDoe, johnPaul));
        assert(!str_startsWith(johnPaul, johnDoe));
    }
    str_destroy(&bob);
    str_destroy(&jeff);
    str_destroy(&johnDoe);
    str_destroy(&johnPaul);

    return true;
}

bool test_str_endsWith() {
    String bob = str_createCopy("Bob");
    String jeff = str_createCopy("Jeff Bob");
    String johnDoe = str_createCopyOfLength("John\0Doe", 8);
    String johnPaul = str_createCopyOfLength("John\0Paul", 9);
    {
        assertStrValid(bob);
        assertStrValid(jeff);
        assertStrValid(johnDoe);
        assertStrValid(johnPaul);

        assert(!str_endsWith(bob, jeff));
        assert(str_endsWith(jeff, bob));
        assert(!str_endsWith(johnDoe, johnPaul));
        assert(!str_endsWith(johnPaul, johnDoe));

        memcpy(johnDoe.data, "Doe\0John", 8);
        memcpy(johnPaul.data, "Paul\0John", 9);

        assert(!str_endsWith(johnDoe, johnPaul));
        assert(!str_endsWith(johnPaul, johnDoe));
    }
    str_destroy(&bob);
    str_destroy(&jeff);
    str_destroy(&johnDoe);
    str_destroy(&johnPaul);

    return true;
}

bool test_str_get() {
    String johnDoe = str_createCopyOfLength("John\0Doe", 8);
    {
        assertStrValid(johnDoe);

        assert(str_get(johnDoe, 0) == 'J');
        assert(str_get(johnDoe, 3) == 'n');
        assert(str_get(johnDoe, 4) == '\0');
        assert(str_get(johnDoe, 5) == 'D');
        assert(str_get(johnDoe, 7) == 'e');
    }
    str_destroy(&johnDoe);

    return true;
}

bool test_str_indexOfChar() {
    String names = str_createCopyOfLength("Little Finger\0Mance\0Jon\0Tyrrion\0Sansa\0Arya", 42);
    {
        assertStrValid(names);

        assert(str_indexOfChar(names, ' ') == 6);
        assert(str_indexOfChar(names, 'a') == 15);
        assert(str_indexOfChar(names, '\0') == 13);
    }
    str_destroy(&names);

    return true;
}

bool test_str_indexOfString() {
    String names = str_createCopyOfLength("Little Finger\0Mance\0Jon\0Tyrrion\0Sansa\0Arya", 42);
    String littleFinger = str_createCopy("Little Finger");
    String jon = str_createCopy("Jon");
    String arya = str_createCopyOfLength("\0Arya", 5);
    {
        assertStrValid(names);
        assertStrValid(littleFinger);
        assertStrValid(jon);
        assertStrValid(arya);

        assert(str_indexOfString(names, littleFinger) == 0);
        assert(str_indexOfString(names, jon) == 20);
        assert(str_indexOfString(names, arya) == 37);
    }
    str_destroy(&names);
    str_destroy(&littleFinger);
    str_destroy(&jon);
    str_destroy(&arya);

    return true;
}

bool test_str_indexOfCharAfterIndex() {
    String names = str_createCopyOfLength("Little Finger\0Mance\0Jon\0Tyrrion\0Sansa\0Arya", 42);
    {
        assertStrValid(names);

        assert(str_indexOfCharAfterIndex(names, ' ', 6) == 6);
        assert(str_indexOfCharAfterIndex(names, 'a', 16) == 33);
        assert(str_indexOfCharAfterIndex(names, '\0', 15) == 19);
    }
    str_destroy(&names);

    return true;
}

bool test_str_indexOfStringAfterIndex() {
    String names = str_createCopyOfLength("Little Finger\0Arya\0Little Finger\0Tyrrion\0Sansa\0Arya", 51);
    String littleFinger = str_createCopy("Little Finger");
    String arya = str_createCopyOfLength("\0Arya", 5);
    {
        assertStrValid(names);
        assertStrValid(littleFinger);
        assertStrValid(arya);

        assert(str_indexOfStringAfterIndex(arya, littleFinger, 0) == -1);

        assert(str_indexOfStringAfterIndex(names, littleFinger, 0) == 0);
        assert(str_indexOfStringAfterIndex(names, littleFinger, 1) == 19);
        assert(str_indexOfStringAfterIndex(names, arya, 15) == 46);

        assert(str_indexOfStringAfterIndex(names, str_createEmpty(), 5) == 6);
    }
    str_destroy(&names);
    str_destroy(&littleFinger);
    str_destroy(&arya);

    return true;
}

bool test_str_lastIndexOfChar() {
    String names = str_createCopyOfLength("Little Finger\0Mance\0Jon\0Tyrrion\0Sansa\0Arya", 42);
    {
        assertStrValid(names);

        assert(str_lastIndexOfChar(names, ' ') == 6);
        assert(str_lastIndexOfChar(names, 'a') == 41);
        assert(str_lastIndexOfChar(names, '\0') == 37);
    }
    str_destroy(&names);

    return true;
}

bool test_str_lastIndexOfString() {
    String names = str_createCopyOfLength("Little Finger\0Arya\0Little Finger\0Tyrrion\0Sansa\0Arya", 51);
    String littleFinger = str_createCopy("Little Finger");
    String tyrrion = str_createCopy("Tyrrion");
    String arya = str_createCopyOfLength("\0Arya", 5);
    {
        assertStrValid(names);
        assertStrValid(littleFinger);
        assertStrValid(tyrrion);
        assertStrValid(arya);

        assert(str_lastIndexOfString(names, littleFinger) == 19);
        assert(str_lastIndexOfString(names, tyrrion) == 33);
        assert(str_lastIndexOfString(names, arya) == 46);
    }
    str_destroy(&names);
    str_destroy(&littleFinger);
    str_destroy(&tyrrion);
    str_destroy(&arya);

    return true;
}

bool test_str_containsChar() {
    String jeff = str_createCopyOfLength("Jeff\0 Bob", 8);
    {
        assertStrValid(jeff);

        assert(!str_containsChar(jeff, 'z'));
        assert(str_containsChar(jeff, 'f'));
        assert(str_containsChar(jeff, ' '));
        assert(!str_containsChar(jeff, 'F'));
        assert(str_containsChar(jeff, '\0'));
    }
    str_destroy(&jeff);

    return true;
}

bool test_str_containsString() {
    String jeff = str_createCopyOfLength("Jeff\0Bob", 8);
    String bob = str_createCopy("Bob");
    String jess = str_createCopy("Jess");
    String withNull = str_createCopyOfLength("ff\0B", 4);
    String differentAfterNull = str_createCopyOfLength("ff\0A", 4);
    {
        assertStrValid(jeff);
        assertStrValid(bob);
        assertStrValid(jess);
        assertStrValid(withNull);
        assertStrValid(differentAfterNull);

        assert(!str_containsString(jeff, jess));
        assert(str_containsString(jeff, bob));
        assert(!str_containsString(bob, jeff));
        assert(str_containsString(jeff, withNull));
        assert(!str_containsString(jeff, differentAfterNull));
    }
    str_destroy(&jeff);
    str_destroy(&bob);
    str_destroy(&jess);
    str_destroy(&withNull);
    str_destroy(&differentAfterNull);

    return true;
}

bool test_str_toUppercase() {
    String string = str_createCopyOfLength("Everybody likes \0 NULL characters! \\o/", 36);
    String expected = str_createCopyOfLength("EVERYBODY LIKES \0 NULL CHARACTERS! \\O/", 36);
    {
        assertStrValid(string);
        assertStrValid(expected);

        str_toUppercase(string);

        assert(str_equals(string, expected));
    }
    str_destroy(&string);
    str_destroy(&expected);

    return true;
}

bool test_str_toLowercase() {
    String string = str_createCopyOfLength("Everybody likes \0 NULL characters! \\o/", 1);
    String expected = str_createCopyOfLength("everybody likes \0 null characters! \\o/", 1);
    {
        assertStrValid(string);
        assertStrValid(expected);

        str_toLowercase(string);

        assert(str_equals(string, expected));
    }
    str_destroy(&string);
    str_destroy(&expected);

    return true;
}

bool test_str_set() {
    String billy = str_createCopyOfLength("Billy\0Kimberley", 15);
    String expected = str_createCopy("JillySOimberlNy");
    {
        assertStrValid(billy);
        assertStrValid(expected);

        str_set(billy, 0, 'J');
        str_set(billy, 5, 'S');
        str_set(billy, 6, 'O');
        str_set(billy, 13, 'N');

        assert(str_get(billy, 0) == 'J');
        assert(str_get(billy, 5) == 'S');
        assert(str_get(billy, 6) == 'O');
        assert(str_get(billy, 13) == 'N');

        assert(str_equals(billy, expected));
    }
    str_destroy(&billy);
    str_destroy(&expected);

    return true;
}

bool test_str_setChars() {
    String string = str_createCopy("Everybody likes strings");
    String jeff = str_createCopy("jeff");
    String bob = str_createCopy("bob");
    String expected = str_createCopy("Ejeffbody likes bobings");
    {
        assertStrValid(string);
        assertStrValid(jeff);
        assertStrValid(bob);
        assertStrValid(expected);

        str_setChars(string, 1, jeff);
        str_setChars(string, 16, bob);

        assert(str_equals(string, expected));
    }
    str_destroy(&string);
    str_destroy(&jeff);
    str_destroy(&bob);
    str_destroy(&expected);

    return true;
}

bool test_str_replaceChar() {
    String mississippi = str_createCopy("Mississippi");
    String expected = str_createCopy("Mirrirrippi");
    {
        assertStrValid(mississippi);
        assertStrValid(expected);

        str_replaceChar(mississippi, 's', 'r');

        assert(str_equals(mississippi, expected));
    }
    str_destroy(&mississippi);
    str_destroy(&expected);

    return true;
}

bool test_str_replaceString() {
    String apples = str_createCopy("Everyone likes apples, apples are the best, yay apples");
    String pineapples = str_createCopy("Everyone likes pineapples, pineapples are the best, yay pineapples");
    String replaced;
    {
        assertStrValid(apples);
        assertStrValid(pineapples);

        replaced = str_replaceString(apples, str_create("apple"), str_create("pineapple"));

        assertStrValid(replaced);
        assert(str_equals(replaced, pineapples));
    }
    str_destroy(&apples);
    str_destroy(&pineapples);
    str_destroy(&replaced);

    return true;
}

bool test_str_replaceCString() {
    String apples = str_createCopy("Everyone likes apples, apples are the best, yay apples");
    String pineapples = str_createCopy("Everyone likes pineapples, pineapples are the best, yay pineapples");
    String replaced;
    {
        assertStrValid(apples);
        assertStrValid(pineapples);

        replaced = str_replaceCString(apples, "apple", "pineapple");

        assertStrValid(replaced);
        assert(str_equals(replaced, pineapples));
    }
    str_destroy(&apples);
    str_destroy(&pineapples);
    str_destroy(&replaced);

    return true;
}

bool test_str_replaceStringInPlace() {
    String pineapples = str_createCopy("Everyone likes pineapples, pineapples are the best, yay pineapples");
    String apples = str_createCopy("Everyone likes apples, apples are the best, yay apples");
    {
        assertStrValid(pineapples);
        assertStrValid(apples);

        pineapples = str_replaceStringInPlace(pineapples, str_create("pineapple"), str_create("apple"));

        assert(str_equals(pineapples, apples));
    }
    str_destroy(&apples);
    str_destroy(&pineapples);

    return true;
}

bool test_str_replaceCStringInPlace() {
    String pineapples = str_createCopy("Everyone likes pineapples, pineapples are the best, yay pineapples");
    String apples = str_createCopy("Everyone likes apples, apples are the best, yay apples");
    {
        assertStrValid(pineapples);
        assertStrValid(apples);

        pineapples = str_replaceCStringInPlace(pineapples, "pineapple", "apple");

        assert(str_equals(pineapples, apples));
    }
    str_destroy(&apples);
    str_destroy(&pineapples);

    return true;
}

bool test_str_concat() {
    String a = str_createCopy("Apple Pies & ");
    String b = str_createCopy("TV Remotes");
    String added;
    String expected = str_createCopy("Apple Pies & TV Remotes");
    {
        assertStrValid(a);
        assertStrValid(b);
        assertStrValid(expected);

        added = str_concat(a, b);

        assertStrValid(added);
        assert(str_equals(added, expected));
    }
    str_destroy(&a);
    str_destroy(&b);
    str_destroy(&added);
    str_destroy(&expected);

    return true;
}

bool test_str_substring() {
    String string = str_createCopy("Everyobdy likes apple pie, or else they're not a real person");
    String expected = str_createCopy(" likes apple pie,");
    {
        assertStrValid(string);
        assertStrValid(expected);

        String substring = str_substring(string, 9, 26);

        assert(str_equals(substring, expected));
    }
    str_destroy(&string);
    str_destroy(&expected);

    return true;
}

bool test_str_trim() {
    String string = str_createCopy(" \n  \n\r\t Apple Pie\n\n\n\t\t\r");
    String expected = str_createCopy("Apple Pie");
    {
        assertStrValid(string);
        assertStrValid(expected);

        String trimmed = str_trim(string);

        assert(str_equals(trimmed, expected));
    }
    str_destroy(&string);
    str_destroy(&expected);

    return true;
}

bool test_str_trimLeading() {
    String string = str_createCopy(" \n  \n\r\t Apple Pie\n\n\n\t\t\r");
    String expected = str_createCopy("Apple Pie\n\n\n\t\t\r");
    {
        assertStrValid(string);
        assertStrValid(expected);

        String trimmed = str_trimLeading(string);

        assert(str_equals(trimmed, expected));
    }
    str_destroy(&string);
    str_destroy(&expected);

    return true;
}

bool test_str_trimTrailing() {
    String string = str_createCopy(" \n  \n\r\t Apple Pie\n\n\n\t\t\r");
    String expected = str_createCopy(" \n  \n\r\t Apple Pie");
    {
        assertStrValid(string);
        assertStrValid(expected);

        String trimmed = str_trimTrailing(string);

        assertStrValid(trimmed);
        assert(str_equals(trimmed, expected));
    }
    str_destroy(&string);
    str_destroy(&expected);

    return true;
}

bool test_str_splitAt() {
    String originalString = str_createCopy("Apple, dream, bottle, paper, phone");
    String expectedFirst = str_createCopy("Apple, dream");
    String expectedSecond = str_createCopy("bottle");
    String expectedThird = str_createCopy("phone");
    {
        assertStrValid(originalString);
        assertStrValid(expectedFirst);
        assertStrValid(expectedSecond);

        String third = originalString;
        String first = str_splitAt(&third, 12, 2);
        String second = str_splitAt(&third, 6, 9);

        assertStrValid(first);
        assertStrValid(second);
        assertStrValid(third);

        assert(str_equals(expectedFirst, first));
        assert(str_equals(expectedSecond, second));
        assert(str_equals(expectedThird, third));
    }
    str_destroy(&originalString);
    str_destroy(&expectedFirst);
    str_destroy(&expectedSecond);

    return true;
}

bool test_str_splitAtChar() {
    String originalString = str_createCopy("Apple, dream, bottle, paper, phone");
    String expectedFirst = str_createCopy("Apple");
    String expectedSecond = str_createCopy(" dream");
    String expectedThird = str_createCopy(" bottle, paper, phone");
    {
        assertStrValid(originalString);
        assertStrValid(expectedFirst);
        assertStrValid(expectedSecond);

        String third = originalString;
        String first = str_splitAtChar(&third, ',');
        String second = str_splitAtChar(&third, ',');

        assertStrValid(first);
        assertStrValid(second);
        assertStrValid(third);

        assert(str_equals(expectedFirst, first));
        assert(str_equals(expectedSecond, second));
        assert(str_equals(expectedThird, third));
    }
    str_destroy(&originalString);
    str_destroy(&expectedFirst);
    str_destroy(&expectedSecond);
    str_destroy(&expectedThird);

    return true;
}

bool test_str_splitAtString() {
    String originalString = str_createCopy("Apple, dream, bottle, paper, phone");
    String delimiter = str_createCopy(", ");
    String expectedFirst = str_createCopy("Apple");
    String expectedSecond = str_createCopy("dream");
    String expectedThird = str_createCopy("bottle, paper, phone");
    {
        assertStrValid(originalString);
        assertStrValid(expectedFirst);
        assertStrValid(expectedSecond);

        String third = originalString;
        String first = str_splitAtString(&third, delimiter);
        String second = str_splitAtString(&third, delimiter);

        assertStrValid(first);
        assertStrValid(second);
        assertStrValid(third);

        assert(str_equals(expectedFirst, first));
        assert(str_equals(expectedSecond, second));
        assert(str_equals(expectedThird, third));
    }
    str_destroy(&originalString);
    str_destroy(&delimiter);
    str_destroy(&expectedFirst);
    str_destroy(&expectedSecond);
    str_destroy(&expectedThird);

    return true;
}

bool test_str_splitAtCString() {
    String originalString = str_createCopy("Apple, dream, bottle, paper, phone");
    String expectedFirst = str_createCopy("Apple");
    String expectedSecond = str_createCopy("dream");
    String expectedThird = str_createCopy("bottle, paper, phone");
    {
        assertStrValid(originalString);
        assertStrValid(expectedFirst);
        assertStrValid(expectedSecond);

        String third = originalString;
        String first = str_splitAtCString(&third, ", ");
        String second = str_splitAtCString(&third, ", ");

        assertStrValid(first);
        assertStrValid(second);
        assertStrValid(third);

        assert(str_equals(expectedFirst, first));
        assert(str_equals(expectedSecond, second));
        assert(str_equals(expectedThird, third));
    }
    str_destroy(&originalString);
    str_destroy(&expectedFirst);
    str_destroy(&expectedSecond);
    str_destroy(&expectedThird);

    return true;
}



//
// Run Tests
//

void test_String(int * failures, int * successes) {
    test(str_create);
    test(str_createCopy);
    test(str_createOfLength);
    test(str_createCopyOfLength);
    test(str_createUninitialised);
    test(str_createEmpty);
    test(str_isEmpty);
    test(str_destroy);

    test(str_toCString);
    test(str_format);
    test(str_vformat);
    test(str_copy);

    test(str_equals);
    test(str_startsWith);
    test(str_endsWith);

    test(str_get);
    test(str_indexOfChar);
    test(str_indexOfString);
    test(str_indexOfCharAfterIndex);
    test(str_indexOfStringAfterIndex);
    test(str_lastIndexOfChar);
    test(str_lastIndexOfString);
    test(str_containsChar);
    test(str_containsString);

    test(str_toUppercase);
    test(str_toLowercase);
    test(str_set);
    test(str_setChars);
    test(str_replaceChar);
    test(str_replaceString);
    test(str_replaceCString);
    test(str_replaceStringInPlace);
    test(str_replaceCStringInPlace);
    test(str_concat);
    test(str_substring);
    test(str_trim);
    test(str_trimLeading);
    test(str_trimTrailing);
    test(str_splitAt);
    test(str_splitAtChar);
    test(str_splitAtString);
    test(str_splitAtCString);
}