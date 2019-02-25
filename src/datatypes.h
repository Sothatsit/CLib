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

    ERROR_SUCCESS,
    ERROR_NONE,
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

    ERROR_FILE_OPEN,
    ERROR_FILE_SEEK,
    ERROR_FILE_TELL,
    ERROR_FILE_READ,
    ERROR_FILE_CLOSE,

    ERROR_INVALID_CODEPOINT,

    ERROR_COUNT

} CLibErrorType;

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
// Strings
//

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
} String;

/*!
 * Create a copy of {string}.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_copy(String string);

/*!
 * Create a String from the C string {data}.
 *
 * This will use the characters in {data}, if {data} is freed the returned String
 * will be invalid. Use str_createCopy to create a copy when creating a String.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_create(char * data);

/*!
 * Create a String from the C string {data}, copying {data} instead of using it.
 *
 * str_destroy should be used on the returned String once it is no longer being used.
 */
String str_createCopy(char * data);

/*!
 * Create a String from the characters in {data} with the length {length}.
 *
 * This will use the characters in {data}, if {data} is freed the returned
 * String will be invalid. Use str_createCopyOfLength to create a copy of the data.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_createOfLength(char * data, s64 length);

/*!
 * Create a String from the characters in {data} with the length {length}, copying {data} instead of using it.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_createCopyOfLength(char * data, s64 length);

/*!
 * Create a String filled with uninitialised data of length {length}.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_createUninitialised(s64 length);

/*!
 * Returns a String that represents an error.
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
 */
String str_getErrorReason(String string);

/*!
 * Destroy {string}.
 *
 * Any Strings derived from {string} (e.g. substrings) will be
 * invalid after this operation unless copied using str_copy.
 *
 * If {string} is a substring of another String, the result of this operation is
 * undefined and may error. The original String should be destroyed instead.
 */
void str_destroy(String * string);

/*!
 * Creates a null terminated version of {string}.
 *
 * The returned null terminated string should be free'd.
 */
char * str_c(String string);

/*!
 * An alias of str_c.
 */
char * str_toCString(String string);

/*!
 * Creates a String from the printf format string
 * {format} and the arguments {arguments}.
 */
String str_format(char * format, ...);

/*!
 * Creates a null-terminated string from the printf format
 * string {format} and the arguments {arguments}.
 *
 * Will return NULL on failure.
 */
char * str_formatC(char * format, ...);

/*!
 * Creates a String from the printf format string {format} and the arguments {arguments}.
 */
String str_vformat(char * format, va_list arguments);

/*!
 * Creates a null-terminated string from the printf
 * format string {format} and the arguments {arguments}.
 */
char * str_vformatC(char * format, va_list arguments);

/*!
 * Check if {string1} and {string2} contain the same data.
 */
bool str_equals(String string1, String string2);

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
 * If {index} is out of the bounds of {string}, the null character will be returned.
 */
char str_get(String string, s64 index);

/*!
 * Find the index of the first occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_indexOfChar(String string, char find);

/*!
 * Find the index of the first occurence of {find} in {string}.
 *
 * If {find} is empty will return 1 unless {string} is also empty, in which case it will return -1.
 *
 * Will return -1 if {find} is not found.
 * Will return -2 on error.
 */
s64 str_indexOfStr(String string, String find);

/*!
 * Find the index of the first occurence of {find} in {string}.
 *
 * If {find} is empty will return 1 unless {string} is also empty, in which case it will return -1.
 *
 * Will return -1 if {find} is not found.
 * Will return -2 on error.
 */
s64 str_indexOfC(String string, char * find);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if {find} is not found after or at {index}.
 * Will return -2 on error.
 */
s64 str_indexOfCharAfter(String string, char find, s64 index);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if {find} is not found after or at {index}, or if {find} is empty.
 * Will return -2 on error.
 */
s64 str_indexOfStrAfter(String string, String find, s64 index);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if {find} is not found after or at {index}, or if {find} is empty.
 * Will return -2 on error.
 */
s64 str_indexOfCAfter(String string, char * find, s64 index);

/*!
 * Find the index of the last occurence of {find} in {string}.
 *
 * Will return -1 if {find} is not found.
 * Will return -2 on error.
 */
s64 str_lastIndexOfChar(String string, char find);

/*!
 * Find the index of the last occurence of {find} in {string}.
 *
 * Will return -1 if {find} is not found.
 * Will return -2 on error.
 */
s64 str_lastIndexOfStr(String string, String find);

/*!
 * Find the index of the last occurence of {find} in {string}.
 *
 * Will return -1 if {find} is not found.
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
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 */
void str_toUppercase(String string);

/*!
 * Modifies all the characters in {string} to make them lowercase as defined by char_toLowercase.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 */
void str_toLowercase(String string);

/*!
 * Set the character at {index} in {string} to {character}.
 *
 * Returns true if {index} is within the bounds of {string} and the character was set.
 */
CLibErrorType str_set(String string, s64 index, char character);

/*!
 * Set the characters in {string} at {index} to the characters in {replacement}.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 *
 * Will perform a bounds check to ensure that {replacement} will fit completely into {string}.
 * Returns whether the operation was successful.
 */
CLibErrorType str_setChars(String string, s64 index, String replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 */
void str_replaceChar(String string, char find, char replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will return a new String. This new String should be destroyed when it's no longer in use.
 *
 * Will return an empty String on failure.
 */
String str_replaceStr(String string, String find, String replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will return a new String. This new String should be destroyed when it's no longer in use.
 *
 * Will return an empty String on failure.
 */
String str_replaceC(String string, char * find, char * replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * Requires that the length of {find} is greater than or equal to the length of {replacement}.
 *
 * Will return an empty String on failure.
 */
String str_replaceStrInPlace(String string, String find, String replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * Requires that the length of {find} is greater than or equal to the length of {replacement}.
 *
 * Will return an empty String on failure.
 */
String str_replaceCInPlace(String string, char * find, char * replacement);

/*!
 * Reverse the characters in {string} in place.
 *
 * Will return an empty String on failure.
 */
void str_reverse(String string);

/*!
 * Concatenate {string1} and {string2} into a new String.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 *
 * Will return an empty String on failure.
 */
String str_concat(String string1, String string2);

/*!
 * Get a substring of {string} inclusive from the index {start} to {end} - 1.
 *
 * The length of the returned String will be {end} - {start}, barring error.
 * If start or end is out of the bounds of {string}, an empty String will be returned.
 * If end <= start an empty String will be returned.
 *
 * str_destroy should not be used on the returned String, as it uses the data from {string}.
 * As the returned String uses the data from {string}, if {string} is destroyed the returned
 * String will be invalid. Use str_copy on the returned String if this is unwanted.
 */
String str_substring(String string, s64 start, s64 end);

/*!
 * Get a substring of {string} with the leading and trailing whitespace omitted.
 *
 * str_destroy should not be used on the returned String, as it uses the data from {string}.
 * As the returned String uses the data from {string}, if {string} is destroyed the returned
 * String will be invalid. Use str_copy on the returned String if this is unwanted.
 */
String str_trim(String string);

/*!
 * Get a substring of {string} with the leading whitespace omitted.
 *
 * str_destroy should not be used on the returned String, as it uses the data from {string}.
 * As the returned String uses the data from {string}, if {string} is destroyed the returned
 * String will be invalid. Use str_copy on the returned String if this is unwanted.
 */
String str_trimLeading(String string);

/*!
 * Get a substring of {string} with the trailing whitespace omitted.
 *
 * str_destroy should not be used on the returned String, as it uses the data from {string}.
 * As the returned String uses the data from {string}, if {string} is destroyed the returned
 * String will be invalid. Use str_copy on the returned String if this is unwanted.
 */
String str_trimTrailing(String string);

/*!
 * Returns a substring of {remaining} inclusive from its start to {index} - 1.
 * Sets {remaining} to a substring of {remaining} from {index} + {delimiterLength}.
 *
 * If {index} is -1, {remaining} will be returned and {remaining} will be set to an empty String.
 */
String str_splitAt(String * remaining, s64 index, s64 delimiterLength);

/*!
 * Returns a substring of {remaining} from its start to the first occurence of {find}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {find}.
 *
 * If {find} is not found, {remaining} will be returned and {remaining} will be set to an empty String.
 * Therefore, if {remaining} is empty, there are no more occurences of {find} in the String.
 */
String str_splitAtChar(String *remaining, char delimiter);

/*!
 * Returns a substring of {remaining} from its start to the first occurence of {delimiter}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {delimiter}.
 *
 * If {delimiter} is not found, {remaining} will be returned and {remaining} will be set to an empty String.
 * Therefore, if {remaining} is empty, there are no more occurences of {delimiter} in the String.
 */
String str_splitAtStr(String *remaining, String delimiter);

/*!
 * Returns a substring of {remaining} from its start to the first occurence of {delimiter}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {delimiter}.
 *
 * If {delimiter} is not found, {remaining} will be returned and {remaining} will be set to an empty String.
 * Therefore, if {remaining} is empty, there are no more occurences of {delimiter} in the String.
 */
String str_splitAtC(String * remaining, char * delimiter);



//
// File IO
//

/*!
 * Load a file into a String.
 *
 * Will return an empty String on failure.
 */
String str_readFile(char *filename);



//
// String Builder
//

/*!
 * A utility for creating Strings of unknown length.
 */
typedef struct StringBuilder {
    /*!
     * The underlying String that is being built.
     *
     * This String can be used from the StringBuilder at any time, and it or
     * the StringBuilder should be destroyed after they are both not in use.
     *
     * If this String is taken from the StringBuilder, any
     * appendages to the StringBuilder will not affect its value.
     */
    String string;

    /*!
     * The current capacity that the StringBuilder can hold before it has to expand.
     */
    s64 capacity;
} StringBuilder;

/*!
 * Creates a new StringBuilder with initial capacity {initialSize}.
 *
 * Will return a StringBuilder with capacity 0 on error or if {initialSize} is 0.
 */
StringBuilder strbuilder_create(s64 initialSize);

/*!
 * Creates a StringBuilder that is in an errored state.
 */
StringBuilder strbuilder_createErrored(CLibErrorType errorType, int errnum);

/*!
 * Returns whether {builder} is in an errored state.
 */
bool strbuilder_isErrored(StringBuilder builder);

/*!
 * Returns whether {builder} is a valid StringBuilder.
 */
bool strbuilder_isValid(StringBuilder builder);

/*!
 * Destroy {builder}.
 *
 * As {builder}->string and {builder} share the same data, only one of these should be destroyed.
 */
void strbuilder_destroy(StringBuilder * builder);

/*!
 * Returns a copy of the String in {builder}.
 */
String strbuilder_getCopy(StringBuilder builder);

/*!
 * Sets the capacity of {builder} to {capacity}.
 *
 * Returns whether setting the capacity was successful.
 */
CLibErrorType strbuilder_setCapacity(StringBuilder * builder, s64 capacity);

/*!
 * If {requiredCapacity} is greater than the current capacity of {builder}, the
 * capacity of {builder} will be attempted to be increased to the next power of 2.
 *
 * Returns whether it was able to ensure that {builder} has a capacity of at least {requiredCapacity}.
 */
CLibErrorType strbuilder_ensureCapacity(StringBuilder * builder, s64 requiredCapacity);

/*!
 * Attempts to reduce the capacity of {builder} so it is equal to the length of its contents.
 *
 * Returns whether reducing the capacity to its length was successful.
 */
CLibErrorType strbuilder_trimToLength(StringBuilder * builder);

/*!
 * Will append {character} to {builder}.
 *
 * Returns whether appending {character} was succesful.
 */
CLibErrorType strbuilder_appendChar(StringBuilder * builder, char character);

/*!
 * Will append {string} to {builder}.
 *
 * Returns whether appending {string} was successful.
 */
CLibErrorType strbuilder_append(StringBuilder * builder, String string);

/*!
 * Will append {string} to {builder}.
 *
 * Returns whether appending {string} was successful.
 */
CLibErrorType strbuilder_appendC(StringBuilder * builder, char * string);

/*!
 * Will append a substring of {string} between {start} and {end} to {builder}.
 */
CLibErrorType strbuilder_appendSubstring(StringBuilder * builder, String string, s64 start, s64 end);



//
// String conversions
//

/*!
 * Convert a u64 array into a String.
 */
String arr_u64_toString(u64 * array, s64 length);



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
 *
 * Will return an empty String on failure.
 */
String utf8_fromCodepoint(u32 codepoint);

/*!
 * Append the Unicode codepoint {codepoint} in UTF-8 into {builder}.
 *
 * Returns whether it was successful.
 */
CLibErrorType utf8_appendCodepoint(StringBuilder * builder, u32 codepoint);

/*!
 * Read a Unicode codepoint from the start of the UTF-16LE String {remaining}, modifiying
 * {remaining} to be a substring not including the character that was read.
 *
 * Will return 0 on failure or if the first character is the null character.
 */
u32 utf16le_toCodepoint(String * remaining);

/*!
 * Construct a UTF-16LE String containing the codepoint {codepoint}.
 *
 * Will return an empty String on failure.
 */
String utf16le_fromCodepoint(u32 codepoint);

/*!
 * Append the Unicode codepoint {codepoint} in UTF-16LE into {builder}.
 *
 * Returns whether it was successful.
 */
CLibErrorType utf16le_appendCodepoint(StringBuilder * builder, u32 codepoint);



//
// Buffers
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
     * The capacity in chars of the buffer.
     */
    s64 capacity;
} Buffer;

/*!
 * Allocates a new Buffer with capacity {capacity}.
 *
 * Will return an empty Buffer on error or if {capacity} is 0.
 */
Buffer buffer_create(s64 capacity);

/*!
 * Create an empty Buffer.
 */
Buffer buffer_createEmpty();

/*!
 * Create a Buffer in an errored state.
 */
Buffer buffer_createInvalid();

/*!
 * Create a new Buffer using the data at {start} with capacity {capacity}.
 *
 * Will return an empty Buffer if {start} is NULL or if {capacity} is 0.
 */
Buffer buffer_createUsing(char * start, s64 capacity);

/*!
 * Check whether {buffer} is empty.
 */
bool buffer_isEmpty(Buffer buffer);

/*!
 * Check whether {buffer} is usable and not in an errored state.
 */
bool buffer_isValid(Buffer buffer);

/*!
 * Check whether {buffer} is in an errored state.
 */
bool buffer_isInvalid(Buffer buffer);

/*!
 * Create a copy of {buffer}.
 *
 * Will return an empty Buffer on failure or if {buffer} is empty.
 */
Buffer buffer_copy(Buffer buffer);

/*!
 * Copy the contents of {from} into the start of {to}.
 *
 * The Buffer {to} must have a capacity at least as large as {buffer}.
 *
 * Will return whether it was successful.
 */
bool buffer_copyInto(Buffer from, Buffer to);

/*!
 * Destroy {buffer} and free its contents.
 */
void buffer_destroy(Buffer * buffer);

/*!
 * Sets the capacity of {buffer} to {capacity}.
 *
 * Returns whether setting the capacity was successful.
 */
bool buffer_setCapacity(Buffer * buffer, s64 capacity);

/*!
 * If {requiredCapacity} is greater than the current capacity of {buffer}, the
 * capacity of {buffer} will be attempted to be increased to the next power of 2.
 *
 * Returns whether it was able to ensure that {buffer} has a capacity of at least {requiredCapacity}.
 */
bool buffer_ensureCapacity(Buffer * buffer, s64 requiredCapacity);

/*!
 * Check that the contents of the buffers {buffer1} and {buffer2} are the same.
 */
bool buffer_equals(Buffer buffer1, Buffer buffer2);



//
// Stacks
//

/*!
 * A stack of elements that can dynamically grow to accommodate more elements.
 *
 * Elements can be appended or popped from the stack.
 */
typedef struct Stack {
    /*!
     * The buffer used to store the elements on the stack.
     */
    Buffer buffer;

    /*!
     * The number of chars currently being used in the buffer.
     */
    s64 used;
} Stack;

/*!
 * Creates a new Stack with an initial capacity of {initialCapacity} chars.
 *
 * Will return a Stack with capacity 0 on error or if {initialCapacity} is 0.
 */
Stack stack_create(s64 initialCapacity);

/*!
 * Check whether the underlying Buffer of {stack} is empty.
 */
bool stack_isValid(Stack stack);

/*!
 * Destroy {stack} and free its contents.
 */
void stack_destroy(Stack * stack);

/*!
 * Sets the capacity of {stack} to {capacity}.
 *
 * Will fail if {stack->used} is greater than {capacity}.
 *
 * Returns whether setting the capacity was successful.
 */
bool stack_setCapacity(Stack * stack, s64 capacity);

/*!
 * If {requiredCapacity} is greater than the current capacity of {stack}, the
 * capacity of {stack} will be attempted to be increased to the next power of 2.
 *
 * Returns whether it was able to ensure that {stack} has a capacity of at least {requiredCapacity}.
 */
bool stack_ensureCapacity(Stack * stack, s64 requiredCapacity);

/*!
 * Attempts to reduce the capacity of {stack} so it is equal to {stack->used}.
 *
 * Returns whether reducing the capacity to the used space was successful.
 */
bool stack_trimToUsed(Stack * stack);

/*!
 * Get a pointer to the end of {stack}.
 */
#define stack_getEnd(stack) (&((stack).buffer.start)[(stack).used])

/*!
 * Get a pointer to the last element of {stack}, or NULL if there are no elements on {stack}.
 */
#define stack_peek(type, stack) \
    ((type *) (stack.used >= sizeof(type) ? (void *) &((stack).buffer.start)[(stack).used - sizeof(type)] : NULL))

/*!
 * Reserve {length} chars at the end of {stack}.
 *
 * Returns a pointer to the start of the reserved space.
 *
 * Will return NULL if it is unable to reserve {length} chars.
 */
char * stack_reserve(Stack * stack, s64 length);

/*!
 * Append {length} chars from {data} to {stack}.
 *
 * Returns a pointer to the start of the appended data.
 *
 * Will return NULL if it is unable to append {data}.
 */
char * stack_appendData(Stack * stack, char * data, s64 length);

/*!
 * Define functions to modify a Stack which contains data of type {type}.
 *
 * Append: {type} * stack_append{type}(Stack * stack, {type} value)
 *   Append a value of type {type} to {stack}.
 *
 * Pop: {type} * stack_pop{type}(Stack * stack)
 *   Pop a value of type {type} off the top of the stack.
 *
 *
 * to append {value} of type {type} to {stack}
 */
#define stack_defineTypedFunctions(type)   \
    stack_defineTypedCreate(type)          \
    stack_defineTypedAppend(type);         \
    stack_defineTypedAppendMany(type);     \
    stack_defineTypedPopMany(type);        \
    stack_defineTypedPop(type)

/*!
 * Define a function to create a stack of type {type} with
 * initial capacity {initialCapacity} elements of type {type}.
 *
 * The signature of the function is:
 *    static Stack * stack_createOf{type}(s64 initialCapacity)
 *
 * e.g. For Buffer
 *    static Stack * stack_createOfBuffer(s64 initialCapacity)
 *
 * Creates a new Stack with an inital capacity of {intialCapacity} elements of type {type}.
 *
 * Will return a Stack with capacity 0 on error or if {intialCapacity} is 0.
 */
#define stack_defineTypedCreate(type)                          \
    static Stack stack_createOf##type(s64 initialCapacity) {   \
        return stack_create(initialCapacity * sizeof(type));   \
    }

/*!
 * Define a function to append the value {value} of type {type} to {stack}.
 *
 * The signature of the function is:
 *    static {type} * stack_append{type}(Stack * stack, {type} value)
 *
 * e.g. For Buffer
 *    static Buffer * stack_appendBuffer(Stack * stack, Buffer value)
 *
 * Returns a pointer to the appended element.
 *
 * Will return NULL if it is unable to append the value.
 */
#define stack_defineTypedAppend(type)                                  \
    static type * stack_append##type(Stack * stack, type value) {      \
        if(!stack_ensureCapacity(stack, stack->used + sizeof(type)))   \
            return NULL;                                               \
                                                                       \
        type * end = (type *) stack_getEnd(*stack);                    \
        *end = value;                                                  \
                                                                       \
        stack->used += sizeof(type);                                   \
                                                                       \
        return end;                                                    \
    }

/*!
 * Define a function to append {count} values from {data} of type {type} to {stack}.
 *
 * The signature of the function is:
 *    static {type} * stack_appendMany{type}(Stack * stack, {type} * data, s64 count)
 *
 * e.g. For Buffer
 *    static Buffer * stack_appendManyBuffer(Stack * stack, Buffer * data, s64 count)
 *
 * Returns a pointer to the bottom most appended element.
 *
 * Will return NULL if it is unable to append the values.
 */
#define stack_defineTypedAppendMany(type)                                               \
    static type * stack_appendMany##type(Stack * stack, type * data, s64 count) {       \
        return (type *) stack_appendData(stack, (char *) data, count * sizeof(type));   \
    }

/*!
 * Define a function to pop the top value of type {type} from {stack}.
 *
 * The signature of the function is:
 *    static {type} * stack_pop{type}(Stack * stack)
 *
 *  e.g. For Buffer
 *     static Buffer * stack_popBuffer(Stack * stack)
 *
 *  Returns a pointer to the element that was popped from {stack}.
 *
 *  Will return NULL if there are no elements on {stack}.
 */
#define stack_defineTypedPop(type)                     \
    static type * stack_pop##type(Stack * stack) {     \
        return stack_popMany##type(stack, 1);          \
    }

/*!
 * Define a function to pop {count} values of type {type} from {stack}.
 *
 * The signature of the function is:
 *    static {type} * stack_popMany{type}(Stack * stack, s64 count)
 *
 * e.g. For Buffer
 *    static Buffer * stack_popManyBuffer(Stack * stack, s64 count)
 *
 * Returns a pointer to the bottom-most element that was popped from {stack}.
 *
 * Will return NULL if there are not {count} values on {stack}.
 */
#define stack_defineTypedPopMany(type)                              \
    static type * stack_popMany##type(Stack * stack, s64 count) {   \
        if(stack->used < count * sizeof(type))                      \
            return NULL;                                            \
                                                                    \
        stack->used -= count * sizeof(type);                        \
                                                                    \
        return (type *) stack_getEnd(*stack);                       \
    }



//
// Errors
//

/*!
 * Returns a C string representation of {error}.
 *
 * The resulting string should not be free'd.
 */
char * errtype_c(CLibErrorType errorType);

/*!
 * Returns a String representation of {error}.
 *
 * The resultant String should not be free'd.
 */
String errtype_str(CLibErrorType errorType);

/*!
 * Returns an s64 number <= -1, which contains the error and errnum.
 */
s64 err_create(CLibErrorType errorType, int errnum);

/*!
 * Convert the s64 error given by err_create into a String.
 *
 * The resultant String should be free'd after use.
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
