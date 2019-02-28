#ifndef __CLIB_datatypes_h
#define __CLIB_datatypes_h

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

//
// Colours
//

#define RESET    "\x1B[0m"
#define BOLD     "\x1B[1m"
#define RED      "\x1B[31m"
#define GREEN    "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE     "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN     "\x1B[36m"
#define WHITE    "\x1B[37m"



//
// Error Types
//

/*!
 * Errors during the use of CLib.
 */
typedef enum {

    ERROR_NONE = 0,

    ERROR_SUCCESS = 1,
    ERROR_UNKNOWN,
    ERROR_INVALID,

    ERROR_FREED,
    ERROR_STRING_EXHAUSTED,

    ERROR_ARG_NULL,
    ERROR_ARG_INVALID,
    ERROR_NEG_LENGTH,
    ERROR_ALLOC,
    ERROR_FORMAT,
    ERROR_CAST,
    ERROR_OVERFLOW,

    ERROR_FILE_OPEN,
    ERROR_FILE_SEEK,
    ERROR_FILE_TELL,
    ERROR_FILE_READ,
    ERROR_FILE_CLOSE,

    ERROR_INVALID_CODEPOINT,

    ERROR_COUNT

} CLibErrorType;

#define ERROR_FIRST ((CLibErrorType) 1)
#define ERROR_LAST ((CLibErrorType) (ERROR_COUNT - 1))

/*!
 * Strings describing each CLibErrorType.
 */
extern char * CLibErrorTypeStrings[ERROR_COUNT];



//
// Numbers
//

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define S8_MIN INT8_MIN
#define S16_MIN INT16_MIN
#define S32_MIN INT32_MIN
#define S64_MIN INT64_MIN

#define S8_MAX INT8_MAX
#define S16_MAX INT16_MAX
#define S32_MAX INT32_MAX
#define S64_MAX INT64_MAX

#define U8_MIN 0
#define U16_MIN 0
#define U32_MIN 0
#define U64_MIN 0

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX


/*!
 * Get the maximum of {num1} and {num2}.
 */
#define max(num1, num2) (num1 > num2 ? num1 : num2)

/*!
 * Get the minimum of {num1} and {num2}.
 */
#define min(num1, num2) (num1 < num2 ? num1 : num2)

/*!
 * Returns whether it is safe to cast {num} from s64 to size_t.
 */
bool can_cast_s64_to_sizet(s64 num);

/*!
 * Returns whether it is safe to cast {num} from size_t to s64.
 */
bool can_cast_sizet_to_s64(size_t num);

/*!
 * Returns whether it is safe to cast {num} from s64 to u64.
 */
bool can_cast_s64_to_u64(s64 num);

/*!
 * Returns whether it is safe to cast {num} from u64 to s64.
 */
bool can_cast_u64_to_s64(u64 num);

/*!
 * Returns whether it is safe to cast {num} from long to s64.
 */
bool can_cast_long_to_s64(long num);

/*!
 * Returns whether it is safe to cast {num} from s64 to long.
 */
bool can_cast_s64_to_long(s64 num);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow u8, 0 will be returned.
 */
u8 u8_nextPowerOf2(u8 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow u16, 0 will be returned.
 */
u16 u16_nextPowerOf2(u16 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow u32, 0 will be returned.
 */
u32 u32_nextPowerOf2(u32 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow u64, 0 will be returned.
 */
u64 u64_nextPowerOf2(u64 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow s8, 0 will be returned.
 */
s8 s8_nextPowerOf2(s8 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow s16, 0 will be returned.
 */
s16 s16_nextPowerOf2(s16 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow s32, 0 will be returned.
 */
s32 s32_nextPowerOf2(s32 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 *
 * If the next power of 2 will overflow s64, 0 will be returned.
 */
s64 s64_nextPowerOf2(s64 number);



//
// Sorting
//

/*!
 * Perform an insertion sort over the array {source} of length {length}.
 *
 * Will sort {source} in ascending order.
 */
void u64_insertionSort(u64 * source, u64 length);

/*!
 * Perform a quick sort over the array {array} of length {length}.
 *
 * Will sort {array} in ascending order.
 */
void u64_quickSort(u64 * array, u64 length);

/*!
 * Perform a merge sort over the array {array} of length {length}.
 *
 * Must allocate a buffer of size {length} for intermediate storage
 * for use in sorting. If this is unwanted use u64_mergeSortNoAlloc.
 *
 * Will sort {array} in ascending order.
 *
 * Returns whether it was successful.
 */
bool u64_mergeSort(u64 * array, u64 length);

/*
 * Perform a merge sort over the array {array} of length {length},
 * using the buffer {buffer} for intermediate storage during the sort.
 *
 * Will sort {array} in ascending order.
 *
 * The buffer {buffer} must have a length of at least {length}.
 */
void u64_mergeSort_withBuffer(u64 * array, u64 * buffer, u64 length);



//
// Characters
//

/*!
 * Returns whether {c} is whitespace.
 *
 * A character is considered whitespace if it is one of the following:
 * - Horizontal Tab '\t'
 * - New Line '\n'
 * - Carriage Return '\r'
 * - Space ' '
 */
#define char_isWhitespace(c) (c == '\t' || c == '\n' || c == '\r' || c == ' ')

/*!
 * Returns whether {c} is a control character.
 */
#define char_isControlCharacter(c) (c >= 0 && c <= 31)

/*!
 * Returns whether {c} is a digit.
 */
#define char_isDigit(c) (c >= '0' && c <= '9')

/*!
 * Returns whether {c} is a standard English letter (a-z A-Z).
 */
#define char_isLetter(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))

/*!
 * Returns whether {c} is lowercase. If {c} is not a letter as defined by char_isLetter, false will be returned.
 */
#define char_isLowercase(c) (c >= 'a' && c <= 'z')

/*!
 * Returns whether {c} is uppercase. If {c} is not a letter as defined by char_isLetter, false will be returned.
 */
#define char_isUppercase(c) (c >= 'A' && c <= 'Z')

/*!
 * Converts {c} to lowercase. If {c} is not uppercase as defined by
 * char_isUppercase, the returned character will be the same.
 */
#define char_toLowercase(c) (char_isUppercase(c) ? (char) (c + ('a' - 'A')) : c)

/*!
 * Converts {c} to uppercase. If {c} is not lowercase as defined by
 * char_isLowercase, the returned character will be the same.
 */
#define char_toUppercase(c) (char_isLowercase(c) ? (char) (c - ('a' - 'A')) : c)



//
// Buffer and String data structures
//

/*!
 * A fixed size buffer for storing arbritary data.
 */
typedef struct Buffer {
    /*!
     * Pointer to the start of the buffer.
     */
    char * start;

    /*!
     * The total number of chars in the buffer.
     */
    s64 size;
} Buffer;

/*!
 * A blob of data of a set length of chars.
 */
typedef struct String {
    /*!
     * The pointer to the start of the blob of data.
     */
    char * data;

    /*!
     * The length, in chars, of the blob of data.
     */
    s64 length;

    /*!
     * Flags containing info about this String.
     */
    u8 flags;
} String;

#define STRING_FLAG_IS_NULL_TERMINATED ((u8) 1)
#define STRING_FLAG_IS_OWN_ALLOCATION ((u8) 2)



//
// Buffers
//

/*!
 * Allocates a new Buffer with capacity {capacity}.
 */
Buffer buf_create(s64 capacity);

/*!
 * Create a new Buffer using the data in {data} with capacity {size}.
 *
 * Will return an errored Buffer if {data} is NULL or if {size} is negative.
 */
Buffer buf_createUsing(char * data, s64 size);

/*!
 * Create a new Buffer using the data in the null-terminated string {string}.
 *
 * Will returned an errored Buffer if {string} is NULL.
 */
Buffer buf_createUsingC(char * string);

/*!
 * Create an empty Buffer.
 */
Buffer buf_createEmpty();

/*!
 * Create a Buffer in an errored state.
 */
Buffer buf_createErrored(CLibErrorType errorType, int errnum);

/*!
 * Check whether {buffer} is empty.
 */
bool buf_isEmpty(Buffer buffer);

/*!
 * Check whether {buffer} is usable and not in an errored state.
 */
bool buf_isValid(Buffer buffer);

/*!
 * Check whether {buffer} is in an errored state.
 */
bool buf_isErrored(Buffer buffer);

/*!
 * Get the CLibErrorType for the errored buffer {buffer}.
 *
 * Will return ERROR_NONE if {buffer} is not errored.
 */
CLibErrorType buf_getErrorType(Buffer buffer);

/*!
 * Get the errnum for the errored buffer {buffer}.
 *
 * Will return 0 if {buffer} is not errored.
 */
int buf_getErrorNum(Buffer buffer);

/*!
 * Returns a String containing the reason for the errored Buffer {buffer}.
 */
String buf_getErrorReason(Buffer buffer);

/*!
 * Allocate a copy of {buffer}.
 */
Buffer buf_copy(Buffer buffer);

/*!
 * Copy the contents of {data} into {destination} starting from {index}.
 */
CLibErrorType buf_copyInto(Buffer destination, Buffer data, s64 index);

/*!
 * Destroy {buffer} and free its contents.
 */
void buf_destroy(Buffer * buffer);

/*!
 * Sets the capacity of {buffer} to {capacity}.
 */
CLibErrorType buf_setCapacity(Buffer * buffer, s64 capacity);

/*!
 * b
 */
CLibErrorType buf_ensureCapacity(Buffer * buffer, s64 requiredCapacity);

/*!
 * Check that the contents of the buffers {buffer1} and {buffer2} are the same.
 */
bool buf_equals(Buffer buffer1, Buffer buffer2);



//
// Strings
//

/*!
 * Create a copy of {string}.
 */
String str_copy(String string);

/*!
 * Create a String from the null-terminated string {data}.
 *
 * This will use the characters in {data}. If {data} is freed then the returned
 * String will be invalid. Use str_createCopyOfLength to create a copy of the data.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_create(char * data);

/*!
 * Create a String from the C string {data}, copying {data} instead of using it directly.
 *
 * str_destroy should be used on the returned String once it is no longer being used.
 */
String str_createCopy(char * data);

/*!
 * Create a String from the characters in {data} with the length {length}.
 *
 * This will use the characters in {data}. If {data} is freed then the returned
 * String will be invalid. Use str_createCopyOfLength to create a copy of the data.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_createOfLength(char * data, s64 length);

/*!
 * Create a String from the characters in {data} with the length
 * {length}, copying {data} instead of using it directly.
 *
 * str_destroy should be used on the returned String once it is no longer being used.
 */
String str_createCopyOfLength(char * data, s64 length);

/*!
 * Create a String filled with uninitialised data of length {length}.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_createUninitialised(s64 length);

/*!
 * Returns a String that represents the error.
 */
String str_createErrored(CLibErrorType errorType, int errnum);

/*!
 * Returns a String with length 0.
 */
String str_createEmpty();

/*!
 * Returns whether {string} is empty.
 */
bool str_isEmpty(String string);

/*!
 * Returns whether {string} is a valid string.
 */
bool str_isValid(String string);

/*!
 * Returns whether {string} is errored.
 */
bool str_isErrored(String string);

/*!
 * Returns the CLibErrorType of the errored string {string}.
 *
 * If {string} is not errored, then ERROR_NONE will be returned.
 */
CLibErrorType str_getErrorType(String string);

/*!
 * Returns the errnum of the errored string {string}.
 *
 * If {string} is not errored, then 0 will be returned.
 */
int str_getErrorNum(String string);

/*!
 * Returns a String containing the reason for the errored String {string}.
 *
 * The returned String should be destroyed once it is no longer being used.
 */
String str_getErrorReason(String string);

/*!
 * Returns whether {flag} is set in {string}.
 */
bool str_isFlagSet(String string, u8 flag);

/*!
 * Returns whether the data in {string} is already null-terminated.
 */
bool str_isNullTerminated(String string);

/*!
 * Returns whether the data in {string} is its own allocation.
 */
bool str_isOwnAllocation(String string);

/*!
 * Destroy {string}.
 *
 * Any Strings derived from {string} (e.g. substrings) will be
 * invalid after this operation unless copied using str_copy.
 */
void str_destroy(String * string);

/*!
 * Creates a null-terminated version of {string}.
 *
 * The returned null-terminated string should be free'd.
 */
char * str_c(String string);

/*!
 * Creates a null terminated version of {string}, and destroys {string}.
 * This method will re-use the data in {string} if possible.
 *
 * The returned null terminated string should be free'd.
 */
char * str_c_destroy(String * string);

/*!
 * Creates a String from the printf format string {format} and the given arguments.
 *
 * Any String's should be converted to null-terminated strings before use in this function.
 */
String str_format(char * format, ...);

/*!
 * Creates a null-terminated string from the printf format string {format} and the given arguments.
 *
 * Any String's should be converted to null-terminated strings before use in this function.
 *
 * Will return NULL on failure.
 */
char * str_formatC(char * format, ...);

/*!
 * Creates a String from the printf format string {format} and the arguments {arguments}.
 *
 * Any String's should be converted to null-terminated strings before use in this function.
 */
String str_vformat(char * format, va_list arguments);

/*!
 * Creates a null-terminated string from the printf format string {format} and the arguments {arguments}.
 *
 * Any String's should be converted to null-terminated strings before use in this function.
 *
 * Will return NULL on failure.
 */
char * str_vformatC(char * format, va_list arguments);

/*!
 * Check if {string1} and {string2} contain the same data.
 */
bool str_equals(String string1, String string2);

/*!
 * Check if {string1} and {string2} contain the same data.
 */
bool str_equalsC(String string1, char * string2);

/*!
 * Check if {string} starts with {prefix}.
 */
bool str_startsWith(String string, String prefix);

/*!
 * Check if {string} ends with {suffix}.
 */
bool str_endsWith(String string, String suffix);

/*!
 * Returns the character at {index} in {string}.
 *
 * If {index} is out of the bounds of {string}, '\0' will be returned.
 */
char str_get(String string, s64 index);

/*!
 * Find the index of the first occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_indexOfChar(String string, char find);

/*!
 * Find the index of the first occurence of {find} in {string}.
 *
 * If {find} is empty then 1 will be returned, unless {string}
 * is also empty, in which case -1 will be returned.
 *
 * Will return -1 if {find} is not found. Will return -2 on error.
 */
s64 str_indexOfStr(String string, String find);

/*!
 * Find the index of the first occurence of {find} in {string}.
 *
 * If {find} is empty then 1 will be returned, unless {string}
 * is also empty, in which case -1 will be returned.
 *
 * Will return -1 if {find} is not found. Will return -2 on error.
 */
s64 str_indexOfC(String string, char * find);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if {find} is not found. Will return -2 on error.
 */
s64 str_indexOfCharAfter(String string, char find, s64 index);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * If {find} is empty then 1 will be returned, unless {string}
 * is also empty, in which case -1 will be returned.
 *
 * Will return -1 if {find} is not found. Will return -2 on error.
 */
s64 str_indexOfStrAfter(String string, String find, s64 index);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * If {find} is empty then 1 will be returned, unless {string}
 * is also empty, in which case -1 will be returned.
 *
 * Will return -1 if {find} is not found. Will return -2 on error.
 */
s64 str_indexOfCAfter(String string, char * find, s64 index);

/*!
 * Find the index of the last occurence of {find} in {string}.
 *
 * Will return -1 if {find} is not found. Will return -2 on error.
 */
s64 str_lastIndexOfChar(String string, char find);

/*!
 * Find the index of the last occurence of {find} in {string}.
 *
 * Will return -1 if {find} is not found, or if {find} is empty.
 * Will return -2 on error.
 */
s64 str_lastIndexOfStr(String string, String find);

/*!
 * Find the index of the last occurence of {find} in {string}.
 *
 * Will return -1 if {find} is not found, or if {find} is empty.
 * Will return -2 on error.
 */
s64 str_lastIndexOfC(String string, char * find);

/*!
 * Check whether {string} contains {find}.
 */
bool str_containsChar(String string, char find);

/*!
 * Check whether {string} contains {find}.
 */
bool str_containsStr(String string, String find);

/*!
 * Check whether {string} contains the null-terminated string {find}.
 */
bool str_containsC(String string, char * find);

/*!
 * Modifies all the characters in {string} to make them uppercase as defined by char_toUppercase.
 */
void str_toUppercase(String string);

/*!
 * Modifies all the characters in {string} to make them lowercase as defined by char_toLowercase.
 */
void str_toLowercase(String string);

/*!
 * Set the character at {index} in {string} to {character}.
 *
 * Will perform a bounds check to ensure that {index} exists within {string}.
 */
CLibErrorType str_set(String string, s64 index, char character);

/*!
 * Set the characters in {string} at {index} to the characters in {replacement}.
 *
 * Will perform a bounds check to ensure that {replacement} will fit completely into {string}.
 */
CLibErrorType str_setChars(String string, s64 index, String replacement);

/*!
 * Set the characters in {string} at {index} to the characters in {replacement}.
 *
 * Will perform a bounds check to ensure that {replacement} will fit completely into {string}.
 */
CLibErrorType str_setCharsC(String string, s64 index, char * replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}.
 */
void str_replaceChar(String string, char find, char replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will return a new String. This new String should be destroyed when it's no longer in use.
 * If allocating a new String is not wanted, then str_replaceStrInPlace should be used instead.
 */
String str_replaceStr(String string, String find, String replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will return a new String. This new String should be destroyed when it's no longer in use.
 * If allocating a new String is not wanted, then str_replaceCInPlace should be used instead.
 */
String str_replaceC(String string, char * find, char * replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * Requires that the length of {find} is greater than or equal to the length of {replacement}.
 */
String str_replaceStrInPlace(String string, String find, String replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * Requires that the length of {find} is greater than or equal to the length of {replacement}.
 */
String str_replaceCInPlace(String string, char * find, char * replacement);

/*!
 * Reverse the characters in {string} in place.
 */
void str_reverse(String string);

/*!
 * Concatenate {string1} and {string2} into a new String.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_concat(String string1, String string2);

/*!
 * Get a substring of {string} inclusive from the index {start} to {end} - 1.
 */
String str_substring(String string, s64 start, s64 end);

/*!
 * Get a substring of {string} with the leading and trailing whitespace omitted.
 */
String str_trim(String string);

/*!
 * Get a substring of {string} with the leading whitespace omitted.
 */
String str_trimLeading(String string);

/*!
 * Get a substring of {string} with the trailing whitespace omitted.
 */
String str_trimTrailing(String string);

/*!
 * Returns a substring of {remaining} inclusive from its start to {index} - 1.
 * Sets {remaining} to a substring of {remaining} from {index} + {delimiterLength}.
 *
 * If {index} is -1, {remaining} will be returned and {remaining} will be
 * set to an errored String with CLibErrorType ERROR_STRING_EXHAUSTED.
 */
String str_splitAt(String * remaining, s64 index, s64 delimiterLength);

/*!
 * Returns a substring of {remaining} from its start to the first occurence of {delimiter}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {delimiter}.
 *
 * If {delimiter} is not found, {remaining} will be returned and {remaining} will
 * be set to an errored String with CLibErrorType ERROR_STRING_EXHAUSTED.
 */
String str_splitAtChar(String *remaining, char delimiter);

/*!
 * Returns a substring of {remaining} from its start to the first occurence of {delimiter}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {delimiter}.
 *
 * If {delimiter} is not found, {remaining} will be returned and {remaining} will
 * be set to an errored String with CLibErrorType ERROR_STRING_EXHAUSTED.
 */
String str_splitAtStr(String *remaining, String delimiter);

/*!
 * Returns a substring of {remaining} from its start to the first occurence of {delimiter}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {delimiter}.
 *
 * If {delimiter} is not found, {remaining} will be returned and {remaining} will
 * be set to an errored String with CLibErrorType ERROR_STRING_EXHAUSTED.
 */
String str_splitAtC(String * remaining, char * delimiter);



//
// File IO
//

/*!
 * Load the contents of the file {filename} into a String.
 *
 * The returned String should be destroyed when it is no longer in use.
 */
String str_readFile(char *filename);



//
// Builder
//

/*!
 * A utility for constructing Strings or Buffers of unknown length.
 */
typedef struct Builder {
    /*!
     * The buffer to store the data as it is built.
     */
    Buffer buffer;

    /*!
     * The length of the built data.
     */
    s64 length;
} Builder;

/*!
 * Creates a new Builder with initial capacity {initialSize}.
 *
 * Only the returned Builder itself, OR any Buffer or String constructed
 * using the returned Builder should be free'd, as they share the same data.
 */
Builder builder_create(s64 initialSize);

/*!
 * Creates a Builder that is in an errored state.
 */
Builder builder_createErrored(CLibErrorType errorType, int errnum);

/*!
 * Returns whether {builder} is in an errored state.
 */
bool builder_isErrored(Builder builder);

/*!
 * Returns whether {builder} is a valid Builder.
 */
bool builder_isValid(Builder builder);

/*!
 * Destroy {builder}, and any data constructed using {builder}.
 *
 * Only the {builder} itself, OR any Buffer or String constructed
 * using {builder} should be free'd, as they share the same data.
 */
void builder_destroy(Builder * builder);

/*!
 * Returns the contents of {builder} as a String.
 */
String builder_str(Builder builder);

/*!
 * Returns the contents of {builder} as a Buffer.
 */
Buffer builder_buf(Builder builder);

/*!
 * Returns a copy of the contents of {builder} as a String.
 */
String builder_strCopy(Builder builder);

/*!
 * Returns a copy of the contents of {builder} as a Buffer.
 */
Buffer builder_bufCopy(Builder builder);

/*!
 * Sets the capacity of {builder} to {capacity}.
 *
 * If {builder} contains more than {capacity} characters set, then this operation will error.
 */
CLibErrorType builder_setCapacity(Builder * builder, s64 capacity);

/*!
 * If the capacity of {builder} is smaller than {requiredCapacity},
 * then the capacity of {builder} will be increased.
 */
CLibErrorType builder_ensureCapacity(Builder * builder, s64 requiredCapacity);

/*!
 * Attempts to reduce the capacity of {builder} so it is equal to the length of its contents.
 *
 * Returns whether reducing the capacity to its length was successful.
 */
CLibErrorType builder_trimToLength(Builder * builder);

/*!
 * Append {character} to {builder}.
 */
CLibErrorType builder_appendChar(Builder * builder, char character);

/*!
 * Append {string} to {builder}.
 */
CLibErrorType builder_appendStr(Builder * builder, String string);

/*!
 * Append {buffer} to {builder}.
 */
CLibErrorType builder_appendBuf(Builder * builder, Buffer buffer);

/*!
 * Append {string} to {builder}.
 */
CLibErrorType builder_appendC(Builder * builder, char * string);

/*!
 * Append a substring of {string} between {start} and {end} to {builder}.
 */
CLibErrorType builder_appendSubstring(Builder * builder, String string, s64 start, s64 end);



//
// Unicode
//

/*!
 * Read a Unicode codepoint from the start of the UTF-8 String {remaining}, modifiying
 * {remaining} to be a substring not including the character that was read.
 *
 * Will return 0 on failure or if the first character is the null character.
 */
u32 utf8_toCodepoint(String * remaining);

/*!
 * Construct a UTF-8 String from the character in the Unicode codepoint {codepoint}.
 */
String utf8_fromCodepoint(u32 codepoint);

/*!
 * Append the Unicode codepoint {codepoint} in UTF-8 into {builder}.
 */
CLibErrorType utf8_appendCodepoint(Builder * builder, u32 codepoint);

/*!
 * Read a Unicode codepoint from the start of the UTF-16LE String {remaining}, modifiying
 * {remaining} to be a substring not including the character that was read.
 *
 * Will return 0 on failure or if the first character is the null character.
 */
u32 utf16le_toCodepoint(String * remaining);

/*!
 * Construct a UTF-16LE String containing the codepoint {codepoint}.
 */
String utf16le_fromCodepoint(u32 codepoint);

/*!
 * Append the Unicode codepoint {codepoint} in UTF-16LE into {builder}.
 */
CLibErrorType utf16le_appendCodepoint(Builder * builder, u32 codepoint);



//
// Errors
//

/*!
 * Returns a null-terminated string explaining {error}.
 *
 * The resulting string should not be free'd.
 */
char * errtype_c(CLibErrorType errorType);

/*!
 * Returns a String explaining {error}.
 *
 * The resultant String should not be destroyed.
 */
String errtype_str(CLibErrorType errorType);

/*!
 * Returns an s64 number < 0 that contains the error and errnum,
 * unless ERROR_NONE is passed, in which case 0 will be returned.
 */
s64 err_create(CLibErrorType errorType, int errnum);

/*!
 * Get the reason String for the s64 error given by err_create.
 *
 * The returned String should be free'd after use.
 */
String err_reason(s64 error_s64);

/*!
 * Get the CLibErrorType of {error}.
 */
CLibErrorType err_type(s64 error_s64);

/*!
 * Get the errnum of {error}.
 */
int err_num(s64 error_s64);



// End __CLIB_datatypes_h
#endif
