#include <memory.h>
#include <stdio.h>
#include "datatypes.h"

//
// Numbers
//

u8 u8_findNextPowerOf2(u8 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    return (u8) (n + 1);
}

u16 u16_findNextPowerOf2(u16 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    return (u16) (n + 1);
}

u32 u32_findNextPowerOf2(u32 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

u64 u64_findNextPowerOf2(u64 n) {
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
}



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

String str_createEmpty() {
    return str_createOfLength(NULL, 0);
}

bool str_isEmpty(String string) {
    return string.data == NULL || string.length == 0;
}

void str_destroy(String * string) {
    if(string->length == 0)
        return;

    free(string->data);

    string->length = 0;
    string->data = NULL;
}

char * str_toCString(String string) {
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

char str_get(String string, u64 index) {
    return index < string.length ? string.data[index] : (char) '\0';
}

s64 str_indexOfChar(String string, char find) {
    return str_indexOfCharAfterIndex(string, find, 0);
}

s64 str_indexOfString(String string, String find) {
    return str_indexOfStringAfterIndex(string, find, 0);
}

s64 str_indexOfCharAfterIndex(String string, char find, u64 index) {
    for(; index < string.length; index++) {
        if(string.data[index] == find)
            return (s64) index;
    }
    return -1;
}

s64 str_indexOfStringAfterIndex(String string, String find, u64 index) {
    if(find.length > string.length)
        return -1;

    if(find.length == 0)
        return string.length > 0 ? (s64) (index + 1) : -1;

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
        --index;

        if(string.data[index] == find)
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
        --index;

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

void str_toUppercase(String string) {
    for(u64 index = 0; index < string.length; index++) {
        string.data[index] = char_toUppercase(string.data[index]);
    }
}

void str_toLowercase(String string) {
    for(u64 index = 0; index < string.length; index++) {
        string.data[index] = char_toLowercase(string.data[index]);
    }
}

bool str_set(String string, u64 index, char character) {
    if(index >= string.length)
        return false;

    string.data[index] = character;
    return true;
}

bool str_setChars(String string, u64 index, String replacement) {
    if(index + replacement.length > string.length)
        return false;

    if(str_isEmpty(replacement))
        return true;

    memmove(&string.data[index], replacement.data, replacement.length);
    return true;
}

void str_replaceChar(String string, char find, char replacement) {
    for(u64 index = 0; index < string.length; index++) {
        if(string.data[index] == find) {
            string.data[index] = replacement;
        }
    }
}

String str_replaceString(String string, String find, String replacement) {
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

String str_replaceCString(String string, char * find, char * replacement) {
    return str_replaceString(string, str_create(find), str_create(replacement));
}

String str_replaceStringInPlace(String string, String find, String replacement) {
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

                u64 end = modified.length;
                modified.length += copyBack.length;
                str_setChars(modified, end, copyBack);
            } else {
                modified.length = (u64) findIndex;
            }

            u64 end = modified.length;
            modified.length += replacement.length;
            str_setChars(modified, end, replacement);

            copyFromIndex = findIndex + find.length;
        }

        String copyBack = str_substring(string, copyFromIndex, string.length);

        u64 end = modified.length;
        modified.length += copyBack.length;
        str_setChars(modified, end, copyBack);

        return modified;
    }
}

String str_replaceCStringInPlace(String string, char * find, char * replacement) {
    return str_replaceStringInPlace(string, str_create(find), str_create(replacement));
}

String str_concat(String string1, String string2) {
    String newString = str_createUninitialised(string1.length + string2.length);

    if(str_isEmpty(newString))
        return newString;

    str_setChars(newString, 0, string1);
    str_setChars(newString, string1.length, string2);

    return newString;
}

String str_substring(String string, u64 start, u64 end) {
    if(start >= string.length || end > string.length || end <= start)
        return str_createEmpty();

    String substring;

    substring.length = end - start;
    substring.data = &string.data[start];

    return substring;
}

String str_trim(String string) {
    return str_trimLeading(str_trimTrailing(string));
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

        --end;
    }

    return str_substring(string, 0, end);
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

String str_splitAtChar(String * remaining, char find) {
    s64 index = str_indexOfChar(*remaining, find);

    return str_splitAt(remaining, index, 1);
}

String str_splitAtString(String * remaining, String delimiter) {
    s64 index = str_indexOfString(*remaining, delimiter);

    return str_splitAt(remaining, index, delimiter.length);
}

String str_splitAtCString(String * remaining, char * delimiter) {
    return str_splitAtString(remaining, str_create(delimiter));
}

String str_readFile(char * filename) {
    FILE * file = fopen(filename, "rb");

    if(file == NULL)
        return str_createEmpty();

    if(fseek(file, 0, SEEK_END))
        return str_createEmpty();

    long position = ftell(file);

    if(position <= 0)
        return str_createEmpty();

    u64 length = (u64) position;
    String buffer = str_createUninitialised(length);

    if(str_isEmpty(buffer))
        return str_createEmpty();

    if(fseek(file, 0, SEEK_SET))
        return str_createEmpty();

    size_t read = fread(buffer.data, 1, length, file);

    if(read != length) {
        str_destroy(&buffer);
        return str_createEmpty();
    }

    if(fclose(file)) {
        str_destroy(&buffer);
        return str_createEmpty();
    }

    return buffer;
}



//
// String Builder
//

StringBuilder strbuilder_create(u64 initialSize) {
    StringBuilder stringBuilder;

    stringBuilder.string = str_createEmpty();
    stringBuilder.capacity = 0;

    strbuilder_setCapacity(&stringBuilder, initialSize);

    return stringBuilder;
}

void strbuilder_destroy(StringBuilder * stringBuilder) {
    if(stringBuilder->capacity == 0)
        return;

    str_destroy(&stringBuilder->string);

    stringBuilder->capacity = 0;
}

String strbuilder_getStringCopy(StringBuilder stringBuilder) {
    return str_copy(stringBuilder.string);
}

bool strbuilder_setCapacity(StringBuilder * stringBuilder, u64 capacity) {
    if(stringBuilder->string.length > capacity)
        return false;

    if(stringBuilder->capacity == capacity)
        return true;

    if(capacity == 0) {
        str_destroy(&stringBuilder->string);
        stringBuilder->string = str_createEmpty();
        stringBuilder->capacity = 0;
        return true;
    }

    if(stringBuilder->capacity == 0) {
        stringBuilder->string = str_createUninitialised(capacity);

        if(str_isEmpty(stringBuilder->string)) {
            stringBuilder->capacity = 0;
            return false;
        }

        stringBuilder->string.length = 0;
        stringBuilder->capacity = capacity;
        return true;
    }

    stringBuilder->string.data = realloc(stringBuilder->string.data, capacity);
    stringBuilder->capacity = capacity;

    if(stringBuilder->string.data == NULL) {
        stringBuilder->capacity = 0;
        return false;
    }

    return true;
}

bool strbuilder_ensureCapacity(StringBuilder * stringBuilder, u64 requiredCapacity) {
    if(requiredCapacity <= stringBuilder->capacity)
        return true;

    u64 newCapacity = u64_findNextPowerOf2(requiredCapacity);

    return strbuilder_setCapacity(stringBuilder, newCapacity);
}

bool strbuilder_trimToLength(StringBuilder * stringBuilder) {
    return strbuilder_setCapacity(stringBuilder, stringBuilder->string.length);
}

bool strbuilder_appendChar(StringBuilder * stringBuilder, char character) {
    u64 requiredCapacity = stringBuilder->string.length + 1;

    if(!strbuilder_ensureCapacity(stringBuilder, requiredCapacity))
        return false;

    u64 end = stringBuilder->string.length;
    stringBuilder->string.length += 1;
    str_set(stringBuilder->string, end, character);

    return true;
}

bool strbuilder_appendString(StringBuilder * stringBuilder, String string) {
    u64 requiredCapacity = stringBuilder->string.length + string.length;

    if(!strbuilder_ensureCapacity(stringBuilder, requiredCapacity))
        return false;

    u64 end = stringBuilder->string.length;
    stringBuilder->string.length += string.length;
    str_setChars(stringBuilder->string, end, string);

    return true;
}

bool strbuilder_appendCString(StringBuilder * stringBuilder, char * string) {
    return strbuilder_appendString(stringBuilder, str_create(string));
}

bool strbuilder_appendSubstring(StringBuilder * stringBuilder, String string, u64 start, u64 end) {
    return strbuilder_appendString(stringBuilder, str_substring(string, start, end));
}

#define __utf8ToCodepoint_nextChar(remaining, next)   \
    do {                                              \
        if(remaining->length == 0)                    \
            return 0;                                 \
                                                      \
        next = (u8) remaining->data[0];               \
                                                      \
        remaining->length -= 1;                       \
        remaining->data += 1;                         \
    } while(0)

#define __utf8ToCodepoint_appendChar(remaining, next, codepoint)   \
    do {                                                           \
        __utf8ToCodepoint_nextChar(remaining, next);               \
        codepoint <<= 6;                                           \
        codepoint |= (u32) (next & 0b00111111);                    \
    } while(0)

u32 utf8_toCodepoint(String * remaining) {
    u8 next;

    __utf8ToCodepoint_nextChar(remaining, next);

    // 0xxxxxxx
    if(next < 0b10000000)
        return (u32) next;

    // 10xxxxxx
    if(next < 0b11000000) {
        // This char is the middle of a codepoint.
        return 0;
    }

    // 110xxxxx 10xxxxxx
    if(next < 0b11100000) {
        u32 codepoint = (u32) (next & 0b00011111);

        __utf8ToCodepoint_appendChar(remaining, next, codepoint);

        return codepoint;
    }

    // 1110xxxx 10xxxxxx 10xxxxxx
    if(next < 0b11110000) {
        u32 codepoint = (u32) (next & 0b00001111);

        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);

        return codepoint;
    }

    // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    if(next < 0b11111000) {
        u32 codepoint = (u32) (next & 0b00000111);

        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);

        return codepoint;
    }

    // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    if(next < 0b11111100) {
        u32 codepoint = (u32) (next & 0b00000011);

        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);

        return codepoint;
    }

    // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
    if(next < 0b11111110) {
        u32 codepoint = (u32) (next & 0b00000001);

        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);
        __utf8ToCodepoint_appendChar(remaining, next, codepoint);

        return codepoint;
    }

    // Invalid character
    return 0;
}

#undef __utf8ToCodepoint_nextChar
#undef __utf8ToCodepoint_appendChar

String utf8_fromCodepoint(u32 codepoint) {
    StringBuilder builder = strbuilder_create(4);

    if(!utf8_appendCodepoint(&builder, codepoint)) {
        strbuilder_destroy(&builder);
        return str_createEmpty();
    }

    return builder.string;
}

bool utf8_appendCodepoint(StringBuilder *builder, u32 codepoint) {
    bool res = true;

    if((codepoint & 0x80000000) != 0) {
        // Invalid codepoint.
        return false;
    }

    // If the codepoint can fit into 7 bits.
    if(codepoint <= 0x007F) {
        // 0xxxxxxx

        res = res && strbuilder_appendChar(builder, (char) codepoint);

        return res;
    }

    // If the codepoint can fit into 11 bits.
    if(codepoint <= 0x07FF) {
        // 110xxxxx 10xxxxxx

        res = res && strbuilder_appendChar(builder, (char) (0xC0 | (codepoint >> 6)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));

        return res;
    }

    // If the codepoint can fit into 16 bits.
    if(codepoint <= 0xFFFF) {
        // 1110xxxx 10xxxxxx 10xxxxxx

        res = res && strbuilder_appendChar(builder, (char) (0xE0 | (codepoint >> 12)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));

        return res;
    }

    // If the codepoint can fit into 21 bits.
    if(codepoint <= 0x001FFFFF) {
        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

        res = res && strbuilder_appendChar(builder, (char) (0xF0 | (codepoint >> 18)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));

        return res;
    }

    // If the codepoint can fit into 26 bits.
    if(codepoint <= 0x03FFFFFF) {
        // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

        res = res && strbuilder_appendChar(builder, (char) (0xF8 | (codepoint >> 24)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 18) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));

        return res;
    }

    // If the codepoint can fit into 31 bits.
    if(codepoint <= 0x7FFFFFFF) {
        // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

        res = res && strbuilder_appendChar(builder, (char) (0xFC | (codepoint >> 30)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 24) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 18) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        res = res && strbuilder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));

        return res;
    }

    // Invalid codepoint.
    return false;
}

#define __utf16leToCodepoint_nextChar(remaining, next)                                   \
    do {                                                                                 \
        if(remaining->length < 2)                                                        \
            return 0;                                                                    \
                                                                                         \
        next = (remaining->data[0] & 0xff) | ((u16) (remaining->data[1] & 0xff) << 8);   \
                                                                                         \
        remaining->length -= 2;                                                          \
        remaining->data += 2;                                                            \
    } while(0)

u32 utf16le_toCodepoint(String * remaining) {
    u16 lo;
    u16 hi;

    __utf16leToCodepoint_nextChar(remaining, lo);

    // Single character
    if(lo < 0xD800 || lo > 0xDFFF)
        return (u32) lo;

    // lo is the second half of a surrogate pair.
    if(lo > 0xDBFF)
        return 0;

    __utf16leToCodepoint_nextChar(remaining, hi);

    // hi is not the second half of a surrogate pair.
    if(hi < 0xDC00 || hi > 0xDFFF)
        return 0;

    const u32 low10bits = (1 << 10) - 1;

    return (((u32) lo & low10bits) << 10) + (hi & low10bits) + 0x10000;
}

#undef __utf16leToCodepoint_nextChar

String utf16le_fromCodepoint(u32 codepoint) {
    StringBuilder builder = strbuilder_create(4);

    if(!utf16le_appendCodepoint(&builder, codepoint)) {
        strbuilder_destroy(&builder);
        return str_createEmpty();
    }

    return builder.string;
}

#define __utf16leAppendCodepoint_append(builder, value)                       \
    do {                                                                      \
        const u8 low8bits = (1 << 8) - 1;                                     \
        strbuilder_appendChar(builder, (char) ((value) & low8bits));          \
        strbuilder_appendChar(builder, (char) (((value) >> 8) & low8bits));   \
    } while(0);

bool utf16le_appendCodepoint(StringBuilder * builder, u32 codepoint) {
    if(codepoint < 0x10000) {
        __utf16leAppendCodepoint_append(builder, (u16) codepoint);
        return true;
    }

    const u16 low10bits = (1 << 10) - 1;

    codepoint -= 0x10000;

    u16 lo = (u16) 0xD800 | (u16) ((codepoint >> 10) & low10bits);
    u16 hi = (u16) 0xDC00 | (u16) (codepoint & low10bits);

    __utf16leAppendCodepoint_append(builder, lo);
    __utf16leAppendCodepoint_append(builder, hi);

    return false;
}



//
// Buffers
//

Buffer buffer_create(u64 initialSize) {
    Buffer buffer;

    buffer.start = NULL;
    buffer.capacity = 0;

    buffer_setCapacity(&buffer, initialSize);

    return buffer;
}

void buffer_destroy(Buffer * buffer) {
    if(buffer->capacity == 0)
        return;

    free(buffer->start);

    buffer->capacity = 0;
    buffer->start = NULL;
}

bool buffer_setCapacity(Buffer * buffer, u64 capacity) {
    if(buffer->capacity == capacity)
        return true;

    if(capacity == 0) {
        buffer_destroy(buffer);
        buffer->start = NULL;
        buffer->capacity = 0;
        return true;
    }

    if(buffer->capacity == 0) {
        buffer->start = malloc(capacity);
    } else {
        buffer->start = realloc(buffer->start, capacity);
    }

    if(buffer->start == NULL) {
        buffer->capacity = 0;
        return false;
    }

    buffer->capacity = capacity;
    return true;
}

bool buffer_ensureCapacity(Buffer * buffer, u64 requiredCapacity) {
    if(requiredCapacity <= buffer->capacity)
        return true;

    u64 newCapacity = u64_findNextPowerOf2(requiredCapacity);

    return buffer_setCapacity(buffer, newCapacity);
}

String buffer_toString(Buffer buffer) {
    String string;

    string.length = buffer.capacity;
    string.data = buffer.start;

    return string;
}



//
// Stacks
//

Stack stack_create(u64 initialCapacity) {
    Stack stack;

    stack.buffer = buffer_create(initialCapacity);
    stack.used = 0;

    return stack;
}

void stack_destroy(Stack * stack) {
    buffer_destroy(&stack->buffer);
}

bool stack_setCapacity(Stack * stack, u64 capacity) {
    return buffer_setCapacity(&stack->buffer, capacity);
}

bool stack_ensureCapacity(Stack * stack, u64 requiredCapacity) {
    return buffer_ensureCapacity(&stack->buffer, requiredCapacity);
}

bool stack_trimToUsed(Stack * stack) {
    return stack_setCapacity(stack, stack->used);
}

char * stack_reserve(Stack * stack, u64 length) {
    if(!stack_ensureCapacity(stack, stack->used + length))
        return NULL;

    char * end = stack_getEnd(*stack);

    stack->used += length;

    return end;
}

char * stack_appendData(Stack * stack, char * data, u64 length) {
    char * start = stack_reserve(stack, length);

    if(start == NULL)
        return NULL;

    memcpy(start, data, length);

    return start;
}