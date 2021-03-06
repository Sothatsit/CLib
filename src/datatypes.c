#include <memory.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include "datatypes.h"

//
// Error Types
//

char * CLibErrorTypeStrings[ERROR_COUNT] = {
    "ERROR_NONE: No error",

    "ERROR_SUCCESS: Success",
    "ERROR_UNKNOWN: Unknown error",
    "ERROR_INVALID: Invalid error type",

    "ERROR_FREED: Resource has been free'd",
    "ERROR_STRING_EXHAUSTED: String has been exhausted",

    "ERROR_ARG_NULL: Argument unexpectedly NULL",
    "ERROR_ARG_INVALID: Invalid arguments",
    "ERROR_NEG_LENGTH: Invalid negative length",
    "ERROR_ALLOC: Error allocating memory",
    "ERROR_FORMAT: Error formatting string",
    "ERROR_CAST: Cannot cast number",
    "ERROR_OVERFLOW: Number overflowed",

    "ERROR_FILE_OPEN: Error opening file",
    "ERROR_FILE_SEEK: Error seeking in file",
    "ERROR_FILE_TELL: Error telling the location in file",
    "ERROR_FILE_READ: Error reading file",
    "ERROR_FILE_CLOSE: Error closing file",

    "ERROR_INVALID_CODEPOINT: Invalid codepoint"
};



//
// Numbers
//

bool can_cast_s64_to_sizet(s64 num) {
    return num >= 0 && num <= SIZE_MAX;
}

bool can_cast_sizet_to_s64(size_t num) {
    return num <= S64_MAX;
}

bool can_cast_s64_to_u64(s64 num) {
    return num >= 0;
}

bool can_cast_u64_to_s64(u64 num) {
    return num <= S64_MAX;
}

bool can_cast_long_to_s64(long num) {
    return num >= S64_MIN && num <= S64_MAX;
}

bool can_cast_s64_to_long(s64 num) {
    return num >= LONG_MIN && num <= LONG_MAX;
}

u8 u8_nextPowerOf2(u8 n) {
    if(n == 0)
        return 1;

    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    return (u8) (n + 1);
}

u16 u16_nextPowerOf2(u16 n) {
    if(n == 0)
        return 1;

    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    return (u16) (n + 1);
}

u32 u32_nextPowerOf2(u32 n) {
    if(n == 0)
        return 1;

    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

u64 u64_nextPowerOf2(u64 n) {
    if(n == 0)
        return 1;

    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
}

s8 s8_nextPowerOf2(s8 n) {
    if (n <= 0)
        return 1;

    n = (s8) u8_nextPowerOf2((u8) n);
    return max(n, (s8) 0);
}

s16 s16_nextPowerOf2(s16 n) {
    if (n <= 0)
        return 1;

    n = (s16) u16_nextPowerOf2((u16) n);
    return max(n, (s16) 0);
}

s32 s32_nextPowerOf2(s32 n) {
    if (n <= 0)
        return 1;

    n = (s32) u32_nextPowerOf2((u32) n);
    return max(n, (s32) 0);
}

s64 s64_nextPowerOf2(s64 n) {
    if (n <= 0)
        return 1;

    n = (s64) u64_nextPowerOf2((u64) n);
    return max(n, (s64) 0);
}



//
// Sorting
//

void u64_insertionSort(u64 * source, u64 length) {
    for(u64 index = 1; index < length; ++index) {
        u64 value = source[index];
        u64 lookBackIndex = index;

        while(lookBackIndex > 0 && source[lookBackIndex - 1] > value) {
            source[lookBackIndex] = source[lookBackIndex - 1];

            lookBackIndex -= 1;
        }

        source[lookBackIndex] = value;
    }
}



/*!
 * Perform a quick sort over the array {array} of length {length} from {left} to {right} inclusive.
 *
 * Will sort {array} from {left} to {right} inclusive in ascending order.
 */
static void u64_quickSort_overRange(u64 * array, u64 left, u64 right);

/*
 * Using the rightmost element as the pivot, place all elements
 * greater than the pivot to the right of the pivot and all
 * elements less than the pivot to its left.
 *
 * Returns the index of the pivot after partitioning.
 */
static u64 u64_quickSort_partition(u64 * array, u64 left, u64 right);

void u64_quickSort(u64 * array, u64 length) {
    u64_quickSort_overRange(array, 0, length - 1);
}

static void u64_quickSort_overRange(u64 * array, u64 left, u64 right) {
    if(left >= right || right == (u64) -1)
        return;

    u64 partition = u64_quickSort_partition(array, left, right);
    u64_quickSort_overRange(array, partition + 1, right);
    u64_quickSort_overRange(array, left, partition - 1);
}

static u64 u64_quickSort_partition(u64 * array, u64 left, u64 right) {
    u64 pivot = array[right];

    u64 smallerIndex = left;

    for(u64 index = left; index < right; ++index) {
        if(array[index] > pivot)
            continue;

        u64 temp = array[smallerIndex];
        array[smallerIndex] = array[index];
        array[index] = temp;

        smallerIndex += 1;
    }

    array[right] = array[smallerIndex];
    array[smallerIndex] = pivot;

    return smallerIndex;
}



/*!
 * Merge the two sorted arrays {array} from {left} inclusive to {middle} exclusive and {source}
 * from {middle} inclusive to {right} inclusive, with {buffer} used as intermediate storage.
 *
 * The array {buffer} must have a length of at least {right} + 1.
 */
static void u64_merge(u64 * array, u64 * buffer, u64 left, u64 middle, u64 right);

bool u64_mergeSort(u64 * array, u64 length) {
    u64 * buffer = malloc(length * sizeof(u64));
    if(buffer == NULL)
        return false;

    u64_mergeSort_withBuffer(array, buffer, length);

    free(buffer);
    return true;
}

void u64_mergeSort_withBuffer(u64 * array, u64 * buffer, u64 length) {
    u64 index = 0;

    while(index + 1 < length) {
        u64 first = array[index];
        u64 second = array[index + 1];

        if(first > second) {
            array[index] = second;
            array[index + 1] = first;
        }

        index += 2;
    }

    u64 groupSize = 2;
    index = 0;

    while(groupSize < length) {
        while(index + groupSize < length) {
            u64 left = index;
            u64 middle = index + groupSize;
            u64 right = min(index + 2 * groupSize, length) - 1;

            u64_merge(array, buffer, left, middle, right);

            index += 2 * groupSize;
        }

        index = 0;
        groupSize *= 2;
    }
}

static void u64_merge(u64 * array, u64 * buffer, u64 left, u64 middle, u64 right) {
    memcpy(&buffer[left], &array[left], (right - left + 1) * sizeof(u64));

    u64 leftIndex = left;
    u64 rightIndex = middle;

    u64 index = left;

    while(leftIndex < middle && rightIndex <= right) {
        if(buffer[leftIndex] < buffer[rightIndex]) {
            array[index] = buffer[leftIndex];
            leftIndex += 1;
            index += 1;
        } else {
            array[index] = buffer[rightIndex];
            rightIndex += 1;
            index += 1;
        }
    }

    while(leftIndex < middle) {
        array[index] = buffer[leftIndex];
        leftIndex += 1;
        index += 1;
    }

    while(rightIndex <= right) {
        array[index] = buffer[rightIndex];
        rightIndex += 1;
        index += 1;
    }
}



//
// Buffers
//

Buffer buf_create(s64 size) {
    Buffer buffer;

    if(size < 0) {
        buffer.start = NULL;
        buffer.size = size;

        return buffer;
    }

    buffer.start = NULL;
    buffer.size = 0;

    buf_setCapacity(&buffer, size);

    return buffer;
}

Buffer buf_createEmpty() {
    return buf_create(0);
}

Buffer buf_createErrored(CLibErrorType errorType, int errnum) {
    if(errorType == ERROR_NONE)
        return buf_createErrored(ERROR_ARG_INVALID, 0);

    return buf_create(err_create(errorType, errnum));
}

Buffer buf_createUsing(char * data, s64 size) {
    if(size < 0)
        return buf_createErrored(ERROR_NEG_LENGTH, 0);
    if(size > 0 && data == NULL)
        return buf_createErrored(ERROR_ARG_NULL, 0);

    if(size == 0) {
        data = NULL;
    }

    return (Buffer) {
        .start = data,
        .size = size
    };
}

Buffer buf_createUsingC(char * string) {
    if(string == NULL)
        return buf_createErrored(ERROR_ARG_NULL, 0);

    size_t length = strlen(string);
    if(!can_cast_sizet_to_s64(length))
        return buf_createErrored(ERROR_CAST, 0);

    return buf_createUsing(string, (s64) length);
}

bool buf_isEmpty(Buffer buffer) {
    return buffer.size == 0;
}

bool buf_isValid(Buffer buffer) {
    if (buffer.size > 0)
        return buffer.start != NULL;

    return buffer.size == 0;
}

bool buf_isErrored(Buffer buffer) {
    return !buf_isValid(buffer);
}

CLibErrorType buf_getErrorType(Buffer buffer) {
    if(!buf_isErrored(buffer))
        return ERROR_NONE;

    return err_type(buffer.size);
}

int buf_getErrorNum(Buffer buffer) {
    if(!buf_isErrored(buffer))
        return 0;

    return err_num(buffer.size);
}

String buf_getErrorReason(Buffer buffer) {
    return err_reason(buffer.size);
}

Buffer buf_copy(Buffer buffer) {
    if(buf_isErrored(buffer))
        return buffer;

    if(buffer.size == 0)
        return buf_createEmpty();

    Buffer copy = buf_create(buffer.size);
    if(buf_isErrored(copy))
        return copy;

    errno = 0;
    CLibErrorType errorType = buf_copyInto(copy, buffer, 0);
    if(errorType != ERROR_SUCCESS)
        return buf_createErrored(errorType, errno);

    return copy;
}

CLibErrorType buf_copyInto(Buffer destination, Buffer data, s64 index) {
    if(buf_isErrored(destination) || buf_isErrored(data))
        return ERROR_ARG_INVALID;
    if(index < 0 || index + data.size > destination.size)
        return ERROR_ARG_INVALID;

    if(data.size == 0)
        return ERROR_SUCCESS;

    memmove(destination.start, data.start, data.size);
    return ERROR_SUCCESS;
}

void buf_destroy(Buffer * buffer) {
    if(buf_isErrored(*buffer))
        return;

    if(buffer->start != NULL) {
        free(buffer->start);
    }

    *buffer = buf_createErrored(ERROR_FREED, 0);
}

CLibErrorType buf_setCapacity(Buffer * buffer, s64 size) {
    if(buf_isErrored(*buffer))
        return ERROR_ARG_INVALID;
    if(buffer->size == size)
        return ERROR_SUCCESS;

    if(size == 0) {
        buf_destroy(buffer);
        *buffer = buf_createEmpty();
        return ERROR_SUCCESS;
    }

    if(!can_cast_s64_to_sizet(size)) {
        *buffer = buf_createErrored(ERROR_CAST, 0);
        return ERROR_CAST;
    }

    if(buffer->size == 0) {
        buffer->start = malloc((size_t) size);
    } else {
        buffer->start = realloc(buffer->start, (size_t) size);
    }

    if(buffer->start == NULL) {
        *buffer = buf_createErrored(ERROR_ALLOC, errno);
        return ERROR_ALLOC;
    }

    buffer->size = size;
    return ERROR_SUCCESS;
}

CLibErrorType buf_ensureCapacity(Buffer * buffer, s64 requiredCapacity) {
    if(buf_isErrored(*buffer))
        return ERROR_ARG_INVALID;
    if(requiredCapacity <= buffer->size)
        return ERROR_SUCCESS;

    s64 newCapacity = s64_nextPowerOf2(requiredCapacity);
    if(newCapacity == 0) {
        *buffer = buf_createErrored(ERROR_OVERFLOW, 0);
        return ERROR_OVERFLOW;
    }

    return buf_setCapacity(buffer, newCapacity);
}

bool buf_equals(Buffer buffer1, Buffer buffer2) {
    if(buf_isErrored(buffer1) || buf_isErrored(buffer2))
        return false;

    if(buffer1.size != buffer2.size)
        return false;
    if(buffer1.size == 0)
        return true;

    if(!can_cast_s64_to_sizet(buffer1.size))
        return false;

    return memcmp(buffer1.start, buffer2.start, (size_t) buffer1.size) == 0;
}



//
// Strings
//

/*!
 * Set the value of {flag} to {value} in {string}.
 */
void str_setFlag(String * string, u8 flag, bool value);

String str_copy(String string) {
    if(str_isErrored(string) || str_isEmpty(string))
        return string;

    String copy = str_createUninitialised(string.length);
    if(str_isErrored(copy))
        return copy;

    memcpy(copy.data, string.data, string.length);

    return copy;
}

String str_create(char * data) {
    if(data == NULL)
        return str_createErrored(ERROR_ARG_NULL, 0);

    size_t length = strlen(data);
    if(!can_cast_sizet_to_s64(length))
        return str_createErrored(ERROR_CAST, 0);

    String string = str_createOfLength(data, (s64) length);

    str_setFlag(&string, STRING_FLAG_IS_NULL_TERMINATED, true);

    return string;
}

String str_createCopy(char * data) {
    return str_copy(str_create(data));
}

String str_createOfLength(char * data, s64 length) {
    if(length <= 0) {
        data = NULL;
    } else if (data == NULL) {
        return str_createErrored(ERROR_ARG_NULL, 0);
    }

    String string;

    string.data = data;
    string.length = length;
    string.flags = 0;

    if(data != NULL) {
        str_setFlag(&string, STRING_FLAG_IS_OWN_ALLOCATION, true);
    }

    return string;
}

String str_createCopyOfLength(char * data, s64 length) {
    return str_copy(str_createOfLength(data, length));
}

String str_createUninitialised(s64 length) {
    if(length < 0)
        return str_createErrored(ERROR_NEG_LENGTH, 0);
    if(length == 0)
        return str_createEmpty();
    if(!can_cast_s64_to_sizet(length + 1))
        return str_createErrored(ERROR_CAST, 0);

    String string;

    string.data = malloc((size_t) (length + 1));
    string.length = length;
    string.flags = 0;

    if(string.data == NULL)
        return str_createErrored(ERROR_ALLOC, errno);

    string.data[length] = '\0';

    str_setFlag(&string, STRING_FLAG_IS_NULL_TERMINATED, true);
    str_setFlag(&string, STRING_FLAG_IS_OWN_ALLOCATION, true);

    return string;
}

String str_createErrored(CLibErrorType errorType, int errnum) {
    if(errorType == ERROR_NONE)
        return str_createErrored(ERROR_ARG_INVALID, 0);

    return str_createOfLength(NULL, err_create(errorType, errnum));
}

String str_createEmpty() {
    return str_createOfLength(NULL, 0);
}

bool str_isEmpty(String string) {
    return string.length == 0;
}

bool str_isValid(String string) {
    if (string.length > 0)
        return string.data != NULL;

    return string.length == 0;
}

bool str_isErrored(String string) {
    return !str_isValid(string);
}

CLibErrorType str_getErrorType(String string) {
    if (!str_isErrored(string))
        return ERROR_NONE;

    return err_type(string.length);
}

int str_getErrorNum(String string) {
    if (!str_isErrored(string))
        return 0;

    return err_num(string.length);
}

String str_getErrorReason(String string) {
    return err_reason(string.length);
}

void str_setFlag(String * string, u8 flag, bool value) {
    if(value) {
        string->flags |= flag;
    } else {
        string->flags &= (~flag);
    }
}

bool str_isFlagSet(String string, u8 flag) {
    return (string.flags & flag) == flag;
}

bool str_isNullTerminated(String string) {
    return str_isFlagSet(string, STRING_FLAG_IS_NULL_TERMINATED);
}

bool str_isOwnAllocation(String string) {
    return str_isFlagSet(string, STRING_FLAG_IS_OWN_ALLOCATION);
}

void str_destroy(String * string) {
    if(str_isErrored(*string))
        return;

    if(string->data != NULL && str_isOwnAllocation(*string)) {
        free(string->data);
    }

    *string = str_createErrored(ERROR_FREED, 0);
}

char * str_c(String string) {
    if(str_isErrored(string))
        return NULL;

    s64 new_length = string.length + 1;
    if(!can_cast_s64_to_sizet(new_length))
        return NULL;

    char * data = malloc((size_t) new_length);
    if(data == NULL)
        return NULL;

    if(string.length > 0) {
        memcpy(data, string.data, string.length);
    }

    data[string.length] = '\0';

    return data;
}

char * str_c_destroy(String * string) {
    // If the string is already null-terminated, we can just re-use it.
    if(str_isNullTerminated(*string) && str_isOwnAllocation(*string)) {
        char * data = string->data;
        string->data = NULL;

        str_destroy(string);

        return data;
    }

    // Otherwise we'll have to convert it.
    char * cStr = str_c(*string);

    str_destroy(string);

    return cStr;
}

String str_format(char * format, ...) {
    va_list argList;
    va_start(argList, format);

    String formattedString = str_vformat(format, argList);

    va_end(argList);

    return formattedString;
}

char * str_formatC(char * format, ...) {
    va_list argList;
    va_start(argList, format);

    char * formattedString = str_vformatC(format, argList);

    va_end(argList);

    return formattedString;
}

String str_vformat(char * format, va_list arguments) {
    va_list argumentsCopy = {};
    va_copy(argumentsCopy, arguments);

    s64 length = vsnprintf(NULL, 0, format, arguments);
    if(length < 0)
        return str_createErrored(ERROR_FORMAT, errno);

    // For the null character
    length += 1;
    if(!can_cast_s64_to_sizet(length))
        return str_createErrored(ERROR_CAST, 0);

    char * data = malloc((size_t) length);
    if(data == NULL)
        return str_createErrored(ERROR_ALLOC, errno);

    vsprintf(data, format, argumentsCopy);

    String string = str_createOfLength(data, length - 1);
    str_setFlag(&string, STRING_FLAG_IS_NULL_TERMINATED, true);

    return string;
}

char * str_vformatC(char * format, va_list arguments) {
    String formatted = str_vformat(format, arguments);
    return str_c_destroy(&formatted);
}

bool str_equals(String string1, String string2) {
    if(str_isErrored(string1) || str_isErrored(string2))
        return false;

    if(string1.length != string2.length)
        return false;
    if(string1.length == 0)
        return true;

    // This shouldn't happen... but just in case
    if(!can_cast_s64_to_sizet(string1.length))
        return false;

    return memcmp(string1.data, string2.data, (size_t) string1.length) == 0;
}

bool str_equalsC(String string1, char * string2) {
    return str_equals(string1, str_create(string2));
}

bool str_startsWith(String string, String prefix) {
    if(str_isErrored(string) || str_isErrored(prefix))
        return false;

    if(prefix.length == 0)
        return true;
    if(prefix.length > string.length)
        return false;

    // This shouldn't happen... but just in case
    if(!can_cast_s64_to_sizet(prefix.length))
        return false;

    return memcmp(string.data, prefix.data, (size_t) prefix.length) == 0;
}

bool str_endsWith(String string, String suffix) {
    if(str_isErrored(string) || str_isErrored(suffix))
        return false;

    if(suffix.length == 0)
        return true;
    if(suffix.length > string.length)
        return false;

    // This shouldn't happen... but just in case
    if(!can_cast_s64_to_sizet(suffix.length))
        return false;

    return memcmp(&string.data[string.length - suffix.length], suffix.data, (size_t) suffix.length) == 0;
}

char str_get(String string, s64 index) {
    return index >= 0 && index < string.length ? string.data[index] : (char) '\0';
}

s64 str_indexOfChar(String string, char find) {
    return str_indexOfCharAfter(string, find, 0);
}

s64 str_indexOfStr(String string, String find) {
    return str_indexOfStrAfter(string, find, 0);
}

s64 str_indexOfC(String string, char * find) {
    return str_indexOfStrAfter(string, str_create(find), 0);
}

s64 str_indexOfCharAfter(String string, char find, s64 index) {
    if(str_isErrored(string) || index < 0)
        return -2;

    for(; index < string.length; index++) {
        if(string.data[index] == find)
            return (s64) index;
    }
    return -1;
}

s64 str_indexOfStrAfter(String string, String find, s64 index) {
    if(str_isErrored(string) || str_isErrored(find) || index < 0)
        return -2;

    if(find.length > string.length || string.length == 0)
        return -1;
    if(find.length == 0)
        return index + 1;

    String checkString;
    checkString.length = find.length;
    checkString.flags = 0;

    s64 maxIndex = string.length - find.length;
    for(; index <= maxIndex; index++) {
        checkString.data = &string.data[index];

        if(str_equals(checkString, find))
            return index;
    }

    return -1;
}

s64 str_indexOfCAfter(String string, char * find, s64 index) {
    return str_indexOfStrAfter(string, str_create(find), index);
}

s64 str_lastIndexOfChar(String string, char find) {
    if(str_isErrored(string))
        return -2;

    s64 index = string.length;
    while(index > 0) {
        index -= 1;
        if(string.data[index] == find)
            return (s64) index;
    };

    return -1;
}

s64 str_lastIndexOfStr(String string, String find) {
    if(str_isErrored(string) || str_isErrored(find))
        return -2;

    if(find.length > string.length)
        return -1;
    if(find.length == 0)
        return -1;

    String checkString;
    checkString.length = find.length;
    checkString.flags = 0;

    s64 index = string.length - find.length + 1;

    while(index > 0) {
        index -= 1;
        checkString.data = &string.data[index];

        if(str_equals(checkString, find))
            return (s64) index;
    }

    return -1;
}

s64 str_lastIndexOfC(String string, char * find) {
    return str_lastIndexOfStr(string, str_create(find));
}

bool str_containsChar(String string, char find) {
    return str_indexOfChar(string, find) != -1;
}

bool str_containsStr(String string, String find) {
    if(find.length == 0)
        return false;

    return str_indexOfStr(string, find) != -1;
}

bool str_containsC(String string, char * find) {
    return str_containsStr(string, str_create(find));
}

void str_toUppercase(String string) {
    if(str_isErrored(string))
        return;

    for(u64 index = 0; index < string.length; index++) {
        string.data[index] = char_toUppercase(string.data[index]);
    }
}

void str_toLowercase(String string) {
    if(str_isErrored(string))
        return;

    for(u64 index = 0; index < string.length; index++) {
        string.data[index] = char_toLowercase(string.data[index]);
    }
}

CLibErrorType str_set(String string, s64 index, char character) {
    if(str_isErrored(string))
        return ERROR_ARG_INVALID;
    if(index < 0 || index >= string.length)
        return ERROR_ARG_INVALID;

    // Temp variable to remove broken CLion unused value warning
    char * data = string.data;

    data[index] = character;
    return ERROR_SUCCESS;
}

CLibErrorType str_setChars(String string, s64 index, String replacement) {
    if(str_isErrored(string) || str_isErrored(replacement))
        return ERROR_ARG_INVALID;
    if(index < 0 || index + replacement.length > string.length)
        return ERROR_ARG_INVALID;

    if(replacement.length == 0)
        return ERROR_SUCCESS;

    memmove(&string.data[index], replacement.data, replacement.length);
    return ERROR_SUCCESS;
}

CLibErrorType str_setCharsC(String string, s64 index, char * replacement) {
    return str_setChars(string, index, str_create(replacement));
}

void str_replaceChar(String string, char find, char replacement) {
    if(str_isErrored(string))
        return;

    for(s64 index = 0; index < string.length; index++) {
        if(string.data[index] == find) {
            string.data[index] = replacement;
        }
    }
}

String str_replaceStr(String string, String find, String replacement) {
    if(str_isErrored(string) || str_isErrored(find) || str_isErrored(replacement))
        return str_createErrored(ERROR_ARG_INVALID, 0);

    Builder builder = builder_create(string.length);

    s64 lastIndex = 0;
    s64 index;

    while((index = str_indexOfStrAfter(string, find, lastIndex)) >= 0) {
        builder_appendSubstring(&builder, string, lastIndex, index);
        builder_appendStr(&builder, replacement);

        lastIndex = index + find.length;
    }

    builder_appendSubstring(&builder, string, lastIndex, string.length);
    builder_trimToLength(&builder);

    return builder_str(builder);
}

String str_replaceC(String string, char * find, char * replacement) {
    return str_replaceStr(string, str_create(find), str_create(replacement));
}

String str_replaceStrInPlace(String string, String find, String replacement) {
    if(str_isErrored(string) || str_isErrored(find) || str_isErrored(replacement))
        return str_createErrored(ERROR_ARG_INVALID, 0);
    if(replacement.length > find.length)
        return str_createErrored(ERROR_ARG_INVALID, 0);

    if(find.length == replacement.length) {
        s64 index = 0;

        while((index = str_indexOfStrAfter(string, find, index)) >= 0) {
            str_setChars(string, index, replacement);
            index += replacement.length;
        }

        return string;
    } else {
        String modified;

        modified.data = string.data;
        modified.length = 0;
        modified.flags = string.flags;

        s64 copyFromIndex = 0;
        s64 findIndex;

        while((findIndex = str_indexOfStrAfter(string, find, copyFromIndex)) >= 0) {
            if(copyFromIndex > 0) {
                String copyBack = str_substring(string, copyFromIndex, findIndex);

                s64 end = modified.length;
                modified.length += copyBack.length;
                str_setChars(modified, end, copyBack);
            } else {
                modified.length = findIndex;
            }

            s64 end = modified.length;
            modified.length += replacement.length;
            str_setChars(modified, end, replacement);

            copyFromIndex = findIndex + find.length;
        }

        if(copyFromIndex < string.length) {
            String copyBack = str_substring(string, copyFromIndex, string.length);

            s64 end = modified.length;
            modified.length += copyBack.length;

            str_setChars(modified, end, copyBack);
            modified.data[modified.length] = '\0';

            str_setFlag(&modified, STRING_FLAG_IS_NULL_TERMINATED, true);
        }

        return modified;
    }
}

String str_replaceCInPlace(String string, char * find, char * replacement) {
    return str_replaceStrInPlace(string, str_create(find), str_create(replacement));
}

void str_reverse(String string) {
    if(str_isErrored(string))
        return;
    if(string.length <= 1)
        return;

    s64 fromStart = 0;
    s64 fromEnd = string.length - 1;

    while (fromStart < fromEnd) {
        // Swap fromStart and fromEnd
        char startChar = string.data[fromStart];
        char endChar = string.data[fromEnd];
        str_set(string, fromStart, endChar);
        str_set(string, fromEnd, startChar);

        fromStart += 1;
        fromEnd -= 1;
    }
}

String str_concat(String string1, String string2) {
    if(str_isErrored(string1) || str_isErrored(string2))
        return str_createErrored(ERROR_ARG_INVALID, 0);

    String newString = str_createUninitialised(string1.length + string2.length);
    if(str_isErrored(newString))
        return newString;

    CLibErrorType result = str_setChars(newString, 0, string1);
    if (result != ERROR_SUCCESS) {
        str_destroy(&newString);
        return str_createErrored(result, 0);
    }

    result = str_setChars(newString, string1.length, string2);
    if (result != ERROR_SUCCESS) {
        str_destroy(&newString);
        return str_createErrored(result, 0);
    }

    return newString;
}

String str_substring(String string, s64 start, s64 end) {
    if(str_isErrored(string))
        return string;
    if(start < 0 || start >= string.length || end < start || end > string.length)
        return str_createErrored(ERROR_ARG_INVALID, 0);

    if(start == end)
        return str_createEmpty();

    String substring;

    substring.length = end - start;
    substring.data = &string.data[start];
    substring.flags = 0;

    if(end == string.length && str_isNullTerminated(string)) {
        str_setFlag(&substring, STRING_FLAG_IS_NULL_TERMINATED, true);
    }

    return substring;
}

String str_trim(String string) {
    return str_trimLeading(str_trimTrailing(string));
}

String str_trimLeading(String string) {
    if(str_isErrored(string))
        return string;

    s64 start = 0;
    while(start < string.length && char_isWhitespace(string.data[start])) {
        start += 1;
    }

    return str_substring(string, start, string.length);
}

String str_trimTrailing(String string) {
    if(str_isErrored(string))
        return string;

    s64 end = string.length;
    while(end > 0 && char_isWhitespace(string.data[end - 1])) {
        end -= 1;
    }

    return str_substring(string, 0, end);
}

String str_splitAt(String * remaining, s64 index, s64 delimiterLength) {
    if(str_isErrored(*remaining))
        return *remaining;

    if(index < -1 || delimiterLength < 0 || index + delimiterLength >= remaining->length) {
        *remaining = str_createErrored(ERROR_ARG_INVALID, 0);
        return *remaining;
    }

    if(index == -1) {
        String splitPart = *remaining;
        *remaining = str_createErrored(ERROR_STRING_EXHAUSTED, 0);

        return splitPart;
    }

    String splitPart = str_substring(*remaining, 0, index);
    *remaining = str_substring(*remaining, index + delimiterLength, remaining->length);

    return splitPart;
}

String str_splitAtChar(String * remaining, char find) {
    if(str_isErrored(*remaining))
        return *remaining;

    s64 index = str_indexOfChar(*remaining, find);

    return str_splitAt(remaining, index, 1);
}

String str_splitAtStr(String * remaining, String delimiter) {
    if(str_isErrored(*remaining))
        return *remaining;

    s64 index = str_indexOfStr(*remaining, delimiter);

    return str_splitAt(remaining, index, delimiter.length);
}

String str_splitAtC(String * remaining, char * delimiter) {
    return str_splitAtStr(remaining, str_create(delimiter));
}



//
// File IO
//

String str_readFile(char * filename) {
    FILE * file = fopen(filename, "rb");
    if(file == NULL)
        return str_createErrored(ERROR_FILE_OPEN, errno);

    if(fseek(file, 0, SEEK_END) != 0)
        return str_createErrored(ERROR_FILE_SEEK, errno);

    long position = ftell(file);
    if(position < 0)
        return str_createErrored(ERROR_FILE_TELL, errno);
    if(position == 0)
        return str_createEmpty();

    if(!can_cast_long_to_s64(position))
        return str_createErrored(ERROR_CAST, 0);

    s64 length = (s64) position;
    String buffer = str_createUninitialised(length);
    if(str_isErrored(buffer))
        return buffer;
    if(fseek(file, 0, SEEK_SET) != 0)
        return str_createErrored(ERROR_FILE_SEEK, errno);
    if(!can_cast_s64_to_sizet(length))
        return str_createErrored(ERROR_CAST, 0);

    size_t read = fread(buffer.data, 1, (size_t) length, file);

    if(read != length) {
        str_destroy(&buffer);
        return str_createErrored(ERROR_FILE_READ, errno);
    }

    if(fclose(file) != 0) {
        str_destroy(&buffer);
        return str_createErrored(ERROR_FILE_CLOSE, errno);
    }

    return buffer;
}



//
// Builder
//

Builder builder_create(s64 initialSize) {
    Builder builder;

    builder.buffer = buf_create(initialSize);
    builder.length = 0;

    return builder;
}

Builder builder_createErrored(CLibErrorType errorType, int errnum) {
    return builder_create(err_create(errorType, errnum));
}

bool builder_isErrored(Builder builder) {
    return buf_isErrored(builder.buffer);
}

bool builder_isValid(Builder builder) {
    return !builder_isErrored(builder);
}

void builder_destroy(Builder * builder) {
    if(builder_isErrored(*builder))
        return;

    buf_destroy(&builder->buffer);
    *builder = builder_createErrored(ERROR_FREED, 0);
}

String builder_str(Builder builder) {
    if(builder_isErrored(builder)) {
        CLibErrorType errorType = buf_getErrorType(builder.buffer);
        int errnum = buf_getErrorNum(builder.buffer);
        return str_createErrored(errorType, errnum);
    }

    return str_createOfLength(builder.buffer.start, builder.length);
}

Buffer builder_buf(Builder builder) {
    if(builder_isErrored(builder)) {
        CLibErrorType errorType = buf_getErrorType(builder.buffer);
        int errnum = buf_getErrorNum(builder.buffer);
        return buf_createErrored(errorType, errnum);
    }

    return buf_createUsing(builder.buffer.start, builder.length);
}

String builder_strCopy(Builder builder) {
    return str_copy(builder_str(builder));
}

Buffer builder_bufCopy(Builder builder) {
    return buf_copy(builder_buf(builder));
}

CLibErrorType builder_setCapacity(Builder * builder, s64 capacity) {
    if(capacity < builder->length) {
        builder->buffer = buf_createErrored(ERROR_ARG_INVALID, 0);
        return ERROR_ARG_INVALID;
    }

    return buf_setCapacity(&builder->buffer, capacity);
}

CLibErrorType builder_ensureCapacity(Builder * builder, s64 requiredCapacity) {
    return buf_ensureCapacity(&builder->buffer, requiredCapacity);
}

CLibErrorType builder_trimToLength(Builder * builder) {
    if(builder_isErrored(*builder))
        return ERROR_ARG_INVALID;

    return builder_setCapacity(builder, builder->length);
}

CLibErrorType builder_appendChar(Builder * builder, char character) {
    if(builder_isErrored(*builder))
        return ERROR_ARG_INVALID;

    s64 requiredCapacity = builder->length + 1;
    CLibErrorType result = builder_ensureCapacity(builder, requiredCapacity);
    if(result != ERROR_SUCCESS)
        return result;

    s64 end = builder->length;
    builder->length += 1;
    builder->buffer.start[end] = character;

    return ERROR_SUCCESS;
}

CLibErrorType builder_appendStr(Builder * builder, String string) {
    if(builder_isErrored(*builder))
        return ERROR_ARG_INVALID;
    if(str_isErrored(string))
        return err_type(string.length);

    if(string.length == 0)
        return ERROR_SUCCESS;

    s64 requiredCapacity = builder->length + string.length;
    CLibErrorType result = builder_ensureCapacity(builder, requiredCapacity);
    if(result != ERROR_SUCCESS)
        return result;

    s64 end = builder->length;
    builder->length += string.length;
    memcpy(&builder->buffer.start[end], string.data, string.length);

    return ERROR_SUCCESS;
}

CLibErrorType builder_appendBuf(Builder * builder, Buffer buffer) {
    return builder_appendStr(builder, str_createOfLength(buffer.start, buffer.size));
}

CLibErrorType builder_appendC(Builder * builder, char * string) {
    return builder_appendStr(builder, str_create(string));
}

CLibErrorType builder_appendSubstring(Builder * builder, String string, s64 start, s64 end) {
    return builder_appendStr(builder, str_substring(string, start, end));
}



//
// Unicode
//

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
    Builder builder = builder_create(4);

    utf8_appendCodepoint(&builder, codepoint);

    return builder_str(builder);
}

CLibErrorType utf8_appendCodepoint(Builder *builder, u32 codepoint) {
    if((codepoint & 0x80000000) != 0)
        return ERROR_INVALID_CODEPOINT;

    // If the codepoint can fit into 7 bits.
    if(codepoint <= 0x007F) {
        // 0xxxxxxx
        return builder_appendChar(builder, (char) codepoint);
    }

    // If the codepoint can fit into 11 bits.
    if(codepoint <= 0x07FF) {
        // 110xxxxx 10xxxxxx

        builder_appendChar(builder, (char) (0xC0 | (codepoint >> 6)));
        return builder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));
    }

    // If the codepoint can fit into 16 bits.
    if(codepoint <= 0xFFFF) {
        // 1110xxxx 10xxxxxx 10xxxxxx

        builder_appendChar(builder, (char) (0xE0 | (codepoint >> 12)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        return builder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));
    }

    // If the codepoint can fit into 21 bits.
    if(codepoint <= 0x001FFFFF) {
        // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

        builder_appendChar(builder, (char) (0xF0 | (codepoint >> 18)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        return builder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));
    }

    // If the codepoint can fit into 26 bits.
    if(codepoint <= 0x03FFFFFF) {
        // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

        builder_appendChar(builder, (char) (0xF8 | (codepoint >> 24)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 18) & 0x3F)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        return builder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));
    }

    // If the codepoint can fit into 31 bits.
    if(codepoint <= 0x7FFFFFFF) {
        // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx

        builder_appendChar(builder, (char) (0xFC | (codepoint >> 30)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 24) & 0x3F)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 18) & 0x3F)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 12) & 0x3F)));
        builder_appendChar(builder, (char) (0x80 | ((codepoint >> 6) & 0x3F)));
        return builder_appendChar(builder, (char) (0x80 | (codepoint & 0x3F)));
    }

    // Invalid codepoint.
    return ERROR_INVALID_CODEPOINT;
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
    Builder builder = builder_create(4);

    utf16le_appendCodepoint(&builder, codepoint);

    return builder_str(builder);
}

#define __utf16leAppendCodepoint_append(builder, value, res)                        \
    do {                                                                            \
        const u8 low8bits = (1 << 8) - 1;                                           \
        res = builder_appendChar(builder, (char) ((value) & low8bits));          \
        res = builder_appendChar(builder, (char) (((value) >> 8) & low8bits));   \
    } while(0);

CLibErrorType utf16le_appendCodepoint(Builder * builder, u32 codepoint) {
    CLibErrorType res;

    if(codepoint < 0x10000) {
        __utf16leAppendCodepoint_append(builder, (u16) codepoint, res);
        return res;
    }

    const u16 low10bits = (1 << 10) - 1;

    codepoint -= 0x10000;

    u16 lo = (u16) 0xD800 | (u16) ((codepoint >> 10) & low10bits);
    u16 hi = (u16) 0xDC00 | (u16) (codepoint & low10bits);

    __utf16leAppendCodepoint_append(builder, lo, res);
    __utf16leAppendCodepoint_append(builder, hi, res);
    return res;
}

#undef __utf16leAppendCodepoint_append



//
// Errors
//

char * errtype_c(CLibErrorType errorType) {
    if (errorType >= ERROR_COUNT) {
        errorType = ERROR_UNKNOWN;
    }

    return CLibErrorTypeStrings[errorType];
}

String errtype_str(CLibErrorType errorType) {
    return str_create(errtype_c(errorType));
}

s64 err_create(CLibErrorType errorType, int errnum) {
    // 16 bits for the CLibError, 32 bits for the errnum
    if ((u64) errorType > 0xFFFF || errnum < 0 || errnum >= 0xFFFFFFFF)
        return -1 - ERROR_INVALID;

    u64 num = errorType | (errnum << 16);

    return (s64) (-num);
}

String err_reason(s64 error_s64) {
    CLibErrorType errorType = err_type(error_s64);
    int errnum = err_num(error_s64);

    String message;
    if (errorType == ERROR_INVALID) {
        message = str_format("%s %d", errtype_c(ERROR_INVALID), error_s64);
    } else {
        message = str_copy(errtype_str(errorType));
    }

    if (errnum == 0)
        return message;

    char * cstr = str_c(message);
    String detailed = str_format("%s (%s)", cstr, strerror(errnum));

    str_destroy(&message);
    free(cstr);

    return detailed;
}

CLibErrorType err_type(s64 error_s64) {
    if (error_s64 >= 0)
        return ERROR_NONE;
    if (!can_cast_s64_to_u64(-error_s64))
        return ERROR_INVALID;

    u64 error_bits = (u64) (-error_s64);

    CLibErrorType errorType = (CLibErrorType) (error_bits & 0xFFFF);
    if (errorType >= ERROR_COUNT)
        return ERROR_INVALID;

    return errorType;
}

int err_num(s64 error_s64) {
    if (error_s64 >= 0 || !can_cast_s64_to_u64(-error_s64))
        return 0;

    u64 error_bits = (u64) (-error_s64);

    return (int) ((error_bits >> 16) & 0xFFFFFFFF);
}
