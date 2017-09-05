#include "string.h"

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

//
// Strings
//

String str_create(char * data) {
    return str_createOfLength(data, strlen(data));
}

String str_createCopy(char * data) {
    return str_copy(str_createOfLength(data, strlen(data)));
}

String str_createOfLength(char * data, u64 length) {
    if(length == 0) {
        data = NULL;
    }

    String string;

    string.data = data;
    string.length = length;

    return string;
}

String str_createCopyOfLength(char * data, u64 length) {
    return str_copy(str_createOfLength(data, length));
}

String str_createUninitialised(u64 length) {
    if(length == 0)
        return str_createEmpty();

    String string;

    string.data = malloc(length);
    string.length = length;

    if(string.data == NULL)
        return str_createEmpty();

    return string;
}

void str_destroy(String string) {
    if(str_isEmpty(string) || string.data == NULL)
        return;

    free(string.data);
}

char * str_toNullTerminated(String string) {
    char * data = malloc(string.length + 1);

    if(data == NULL)
        return NULL;

    if(!str_isEmpty(string)) {
        memcpy(data, string.data, string.length);
    }

    data[string.length] = '\0';

    return data;
}

String str_format(char * format, ...) {
    va_list argList;

    va_start(argList, format);

    String formattedString = str_vformat(format, argList);

    va_end(argList);

    return formattedString;
}

String str_vformat(char * format, va_list arguments) {
    va_list argumentsCopy = {};
    va_copy(argumentsCopy, arguments);

    u64 length = (u64) vsnprintf(NULL, 0, format, arguments);

    if(length == (u64) -1)
        return str_createEmpty();

    char * data = malloc(length + 1);

    if(data == NULL)
        return str_createEmpty();

    vsprintf(data, format, argumentsCopy);

    return str_createOfLength(data, length);
}

String str_copy(String string) {
    if(str_isEmpty(string))
        return string;

    String copy = str_createUninitialised(string.length);

    if(str_isEmpty(copy))
        return copy;

    memcpy(copy.data, string.data, string.length);

    return copy;
}

bool str_equals(String string1, String string2) {
    if(string1.length != string2.length)
        return false;

    if(string1.length == 0)
        return true;

    return memcmp(string1.data, string2.data, string1.length) == 0;
}

bool str_startsWith(String string, String prefix) {
    if(prefix.length > string.length)
        return false;

    if(prefix.length == 0)
        return true;

    return memcmp(string.data, prefix.data, prefix.length) == 0;
}

bool str_endsWith(String string, String suffix) {
    if(suffix.length > string.length)
        return false;

    if(suffix.length == 0)
        return true;

    return memcmp(&string.data[string.length - suffix.length], suffix.data, suffix.length) == 0;
}

void str_toUppercase(String string) {
    for(u64 index = 0; index < string.length; index++) {
        if(char_isLowercase(string.data[index])) {
            string.data[index] -= ('a' - 'A');
        }
    }
}

void str_toLowercase(String string) {
    for(u64 index = 0; index < string.length; index++) {
        if(char_isUppercase(string.data[index])) {
            string.data[index] += ('a' - 'A');
        }
    }
}

void str_setChars(String string, u64 index, String replacement) {
    memcpy(&string.data[index], replacement.data, replacement.length);
}

void str_replaceChar(String string, char find, char replacement) {
    for(u64 index = 0; index < string.length; index++) {
        if(string.data[index] == find) {
            string.data[index] = replacement;
        }
    }
}

String str_replaceString(String string, String find, String replacement) {
    if(find.length == 0)
        return str_createEmpty();

    StringBuilder stringBuilder = strbuilder_create(string.length);

    if(stringBuilder.capacity == 0)
        return str_createEmpty();

    u64 lastIndex = 0;
    s64 index;

    while((index = str_indexOfStringAfterIndex(string, find, lastIndex)) != -1) {
        strbuilder_appendSubstring(&stringBuilder, string, lastIndex, (u64) index);

        strbuilder_appendString(&stringBuilder, replacement);

        lastIndex = (u64) index + find.length;
    }

    strbuilder_appendSubstring(&stringBuilder, string, lastIndex, string.length);

    strbuilder_trimToLength(&stringBuilder);

    if(stringBuilder.capacity == 0)
        return str_createEmpty();

    return stringBuilder.string;
}

String str_replaceStringInPlace(String string, String find, String replacement) {
    if(find.length == 0)
        return str_createEmpty();

    if(replacement.length > find.length)
        return str_createEmpty();

    if(find.length == replacement.length) {
        s64 index = 0;

        while((index = str_indexOfStringAfterIndex(string, find, (u64) index)) != -1) {
            str_setChars(string, (u64) index, replacement);

            index += replacement.length;
        }

        return string;
    } else {
        String modified;

        modified.data = string.data;
        modified.length = 0;

        u64 copyFromIndex = 0;
        s64 findIndex;

        while((findIndex = str_indexOfStringAfterIndex(string, find, copyFromIndex)) != -1) {
            if(copyFromIndex) {
                String copyBack = str_substring(string, copyFromIndex, (u64) findIndex);

                str_setChars(modified, modified.length, copyBack);
                modified.length += copyBack.length;
            } else {
                modified.length = (u64) findIndex;
            }

            str_setChars(modified, modified.length, replacement);
            modified.length += replacement.length;

            copyFromIndex = findIndex + find.length;
        }

        String copyBack = str_substring(string, copyFromIndex, string.length);

        str_setChars(modified, modified.length, copyBack);
        modified.length += copyBack.length;

        return modified;
    }
}

String str_concat(String string1, String string2) {
    if(str_isEmpty(string1))
        return string2;

    if(str_isEmpty(string2))
        return string1;

    String newString = str_createUninitialised(string1.length + string2.length);

    if(str_isEmpty(newString))
        return newString;

    memcpy(newString.data, string1.data, string1.length);
    memcpy(&newString.data[string1.length], string2.data, string2.length);

    return newString;
}

String str_substring(String string, u64 start, u64 end) {
    end = min(end, string.length);

    if(end <= start)
        return str_createEmpty();

    String substring;

    substring.length = end - start;
    substring.data = &string.data[start];

    return substring;
}

String str_trim(String string) {
    u64 start = 0;
    u64 end = string.length;

    while(end > 0) {
        if(!char_isWhitespace(str_get(string, end - 1)))
            break;

        end--;
    }

    while(start < string.length) {
        if(!char_isWhitespace(str_get(string, start)))
            break;

        start++;
    }

    return str_substring(string, start, end);
}

String str_trimLeading(String string) {
    u64 start = 0;

    while(start < string.length) {
        if(!char_isWhitespace(str_get(string, start)))
            break;

        start++;
    }

    return str_substring(string, start, string.length);
}

String str_trimTrailing(String string) {
    u64 end = string.length;

    while(end > 0) {
        if(!char_isWhitespace(str_get(string, end - 1)))
            break;

        end--;
    }

    return str_substring(string, 0, end);
}

s64 str_indexOfChar(String string, char find) {
    return str_indexOfCharAfterIndex(string, find, 0);
}

s64 str_indexOfString(String string, String find) {
    return str_indexOfStringAfterIndex(string, find, 0);
}

s64 str_indexOfCharAfterIndex(String string, char find, u64 index) {
    for(; index < string.length; index++) {
        if(str_get(string, index) == find)
            return (s64) index;
    }
    return -1;
}

s64 str_indexOfStringAfterIndex(String string, String find, u64 index) {
    if(find.length > string.length)
        return -1;

    String checkString;

    checkString.length = find.length;

    u64 maxIndex = string.length - find.length;

    for(; index <= maxIndex; index++) {
        checkString.data = &string.data[index];

        if(str_equals(checkString, find))
            return (s64) index;
    }

    return -1;
}

s64 str_lastIndexOfChar(String string, char find) {
    u64 index = string.length;

    do {
        index--;

        if(str_get(string, index) == find)
            return (s64) index;
    } while(index > 0);

    return -1;
}

s64 str_lastIndexOfString(String string, String find) {
    if(find.length > string.length)
        return -1;

    String checkString;

    checkString.length = find.length;

    u64 index = string.length - find.length + 1;

    do {
        index--;

        checkString.data = &string.data[index];

        if(str_equals(checkString, find))
            return (s64) index;
    } while(index > 0);

    return -1;
}

bool str_containsChar(String string, char find) {
    return str_indexOfChar(string, find) != -1;
}

bool str_containsString(String string, String find) {
    return str_indexOfString(string, find) != -1;
}

String str_splitAt(String * remaining, s64 index, u64 delimiterLength) {
    if(index == -1) {
        String splitPart = *remaining;

        *remaining = str_createEmpty();

        return splitPart;
    }

    String splitPart = str_substring(*remaining, 0, (u64) index);

    *remaining = str_substring(*remaining, (u64) index + delimiterLength, remaining->length);

    return splitPart;
}

String str_splitAtFirstChar(String * remaining, char find) {
    s64 index = str_indexOfChar(*remaining, find);

    return str_splitAt(remaining, index, 1);
}

String str_splitAtFirstString(String * remaining, String find) {
    s64 index = str_indexOfString(*remaining, find);

    return str_splitAt(remaining, index, find.length);
}

String str_UCSCodepointToUTF8(u32 codepoint) {
    if((codepoint & 0x80000000) != 0) {
        // Invalid codepoint.
        return str_createEmpty();
    }

    // If the codepoint can fit into 7 bits.
    if(codepoint <= 0x007F) {
        // 0xxxxxxx

        String string = str_createUninitialised(1);

        if(!str_isEmpty(string)) {
            str_set(string, 0, (char) codepoint);
        }

        return string;
    }

    // If the codepoint can fit into 11 bits.
    if(codepoint <= 0x07FF) {
        // 110xxxxx 10xxxxxx

        String string = str_createUninitialised(2);

        if(!str_isEmpty(string)) {
            str_set(string, 0, (char) (0xC0 | (codepoint >> 6)));
            str_set(string, 1, (char) (0x80 | (codepoint & 0x3F)));
        }

        return string;
    }

    // If the codepoint can fit into 16 bits.
    if(codepoint <= 0xFFFF) {
        // 1110xxxx 10xxxxxx 10xxxxxx

        String string = str_createUninitialised(3);

        if(!str_isEmpty(string)) {
            str_set(string, 0, (char) (0xE0 | (codepoint >> 12)));
            str_set(string, 1, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
            str_set(string, 2, (char) (0x80 | (codepoint & 0x3F)));
        }

        return string;
    }

    // If the codepoint can fit into 21 bits.
    if(codepoint <= 0x001FFFFF) {
        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

        String string = str_createUninitialised(4);

        if(!str_isEmpty(string)) {
            str_set(string, 0, (char) (0xF0 | (codepoint >> 18)));
            str_set(string, 1, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
            str_set(string, 2, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
            str_set(string, 3, (char) (0x80 | (codepoint & 0x3F)));
        }

        return string;
    }

    // If the codepoint can fit into 26 bits.
    if(codepoint <= 0x03FFFFFF) {
        // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

        String string = str_createUninitialised(5);

        if(!str_isEmpty(string)) {
            str_set(string, 0, (char) (0xF8 | (codepoint >> 24)));
            str_set(string, 1, (char) (0x80 | ((codepoint >> 18) & 0x3F)));
            str_set(string, 2, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
            str_set(string, 3, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
            str_set(string, 4, (char) (0x80 | (codepoint & 0x3F)));
        }

        return string;
    }

    // If the codepoint can fit into 31 bits.
    if(codepoint <= 0x7FFFFFFF) {
        // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

        String string = str_createUninitialised(6);

        if(!str_isEmpty(string)) {
            str_set(string, 0, (char) (0xFC | (codepoint >> 30)));
            str_set(string, 1, (char) (0x80 | ((codepoint >> 24) & 0x3F)));
            str_set(string, 2, (char) (0x80 | ((codepoint >> 18) & 0x3F)));
            str_set(string, 3, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
            str_set(string, 4, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
            str_set(string, 5, (char) (0x80 | (codepoint & 0x3F)));
        }

        return string;
    }

    // Invalid codepoint.
    return str_createEmpty();
}



//
// String Builder
//

StringBuilder strbuilder_create(u64 initialSize) {
    if(initialSize == 0){
        initialSize = 32;
    }

    StringBuilder stringBuilder;

    stringBuilder.string = str_createUninitialised(initialSize);

    if(str_isEmpty(stringBuilder.string)) {
        stringBuilder.capacity = 0;

        return stringBuilder;
    }

    stringBuilder.string.length = 0;
    stringBuilder.capacity = initialSize;

    return stringBuilder;
}

void strbuilder_destroy(StringBuilder stringBuilder) {
    if(stringBuilder.capacity == 0)
        return;

    str_destroy(stringBuilder.string);
}

String strbuilder_getStringCopy(StringBuilder stringBuilder) {
    return str_copy(stringBuilder.string);
}

bool strbuilder_setCapacity(StringBuilder * stringBuilder, u64 capacity) {
    if(stringBuilder->capacity == 0)
        return false;

    if(stringBuilder->string.length > capacity)
        return false;

    if(capacity == 0)
        return false;

    if(stringBuilder->capacity == capacity)
        return true;

    stringBuilder->string.data = realloc(stringBuilder->string.data, capacity);
    stringBuilder->capacity = capacity;

    if(stringBuilder->string.data == NULL) {
        stringBuilder->capacity = 0;
        return false;
    }

    return true;
}

bool strbuilder_ensureCapacity(StringBuilder * stringBuilder, u64 requiredCapacity) {
    if(stringBuilder->capacity == 0)
        return false;

    if(requiredCapacity <= stringBuilder->capacity)
        return true;

    u64 newCapacity = stringBuilder->capacity;

    while(newCapacity < requiredCapacity) {
        newCapacity *= 2;
    }

    return strbuilder_setCapacity(stringBuilder, newCapacity);
}

bool strbuilder_trimToLength(StringBuilder * stringBuilder) {
    return strbuilder_setCapacity(stringBuilder, stringBuilder->string.length);
}

bool strbuilder_appendChar(StringBuilder * stringBuilder, char character) {
    if(!strbuilder_ensureCapacity(stringBuilder, stringBuilder->string.length + 1))
        return false;

    u64 endIndex = stringBuilder->string.length;

    stringBuilder->string.length += 1;

    str_set(stringBuilder->string, endIndex, character);

    return true;
}

bool strbuilder_appendString(StringBuilder * stringBuilder, String string) {
    if(!strbuilder_ensureCapacity(stringBuilder, stringBuilder->string.length + string.length))
        return false;

    u64 endIndex = stringBuilder->string.length;

    stringBuilder->string.length += string.length;

    str_setChars(stringBuilder->string, endIndex, string);

    return true;
}

bool strbuilder_appendSubstring(StringBuilder * stringBuilder, String string, u64 start, u64 end) {
    return strbuilder_appendString(stringBuilder, str_substring(string, start, end));
}