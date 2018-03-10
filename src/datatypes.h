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
// Numbers
//

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int32_t s64;

/*!
 * Get the maximum of {num1} and {num2}.
 */
#define max(num1, num2) (num1 > num2 ? num1 : num2)

/*!
 * Get the minimum of {num1} and {num2}.
 */
#define min(num1, num2) (num1 < num2 ? num1 : num2)

/*!
 * Find the next power of 2 greater than or equal to {number}.
 */
u8 u8_nextPowerOf2(u8 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 */
u16 u16_nextPowerOf2(u16 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 */
u32 u32_nextPowerOf2(u32 number);

/*!
 * Find the next power of 2 greater than or equal to {number}.
 */
u64 u64_nextPowerOf2(u64 number);



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
    u64 length;
} String;

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
String str_createOfLength(char * data, u64 length);

/*!
 * Create a String from the characters in {data} with the length {length}, copying {data} instead of using it.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_createCopyOfLength(char * data, u64 length);

/*!
 * Create a String filled with uninitialised data of length {length}.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_createUninitialised(u64 length);

/*!
 * Returns a String with length 0.
 */
String str_createEmpty();

/*!
 * Returns whether {string} is empty.
 */
bool str_isEmpty(String string);

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
 * The returned null terminated string should be freed.
 */
char * str_toCString(String string);

/*!
 * Creates a String from the printf format string {format}.
 *
 * Will return an empty String on failure.
 */
String str_format(char * format, ...);

/*!
 * Creates a String from the printf format string {format} and the arguments {arguments}.
 *
 * Will return an empty String on failure.
 */
String str_vformat(char * format, va_list arguments);

/*!
 * Create a copy of {string}.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_copy(String string);

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
char str_get(String string, u64 index);

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
 */
s64 str_indexOfString(String string, String find);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if {find} is not found after or at {index}.
 */
s64 str_indexOfCharAfterIndex(String string, char find, u64 index);

/*!
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * If {find} is empty will return {index} + 1 unless {string} is also empty, in which case it will return -1.
 *
 * Will return -1 if {find} is not found after or at {index}.
 */
s64 str_indexOfStringAfterIndex(String string, String find, u64 index);

/*!
 * Find the index of the last occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_lastIndexOfChar(String string, char find);

/*!
 * Find the index of the last occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_lastIndexOfString(String string, String find);

/*!
 * Check whether {string} contains {find}.
 */
bool str_containsChar(String string, char find);

/*!
 * Check whether {string} contains {find}.
 */
bool str_containsString(String string, String find);

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
 * Returns true if {index} is within the bounds of {string} and the character is set.
 */
bool str_set(String string, u64 index, char character);

/*!
 * Set the characters in {string} at {index} to the characters in {replacement}.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 *
 * Will perform a bounds check to ensure that {replacement} will fit completely into {string}.
 * Returns whether the operation was successful.
 */
bool str_setChars(String string, u64 index, String replacement);

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
String str_replaceString(String string, String find, String replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will return a new String. This new String should be destroyed when it's no longer in use.
 *
 * Will return an empty String on failure.
 */
String str_replaceCString(String string, char * find, char * replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * Requires that the length of {find} is greater than or equal to the length of {replacement}.
 *
 * Will return an empty String on failure.
 */
String str_replaceStringInPlace(String string, String find, String replacement);

/*!
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * Requires that the length of {find} is greater than or equal to the length of {replacement}.
 *
 * Will return an empty String on failure.
 */
String str_replaceCStringInPlace(String string, char * find, char * replacement);

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
String str_substring(String string, u64 start, u64 end);

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
String str_splitAt(String * remaining, s64 index, u64 delimiterLength);

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
String str_splitAtString(String *remaining, String delimiter);

/*!
 * Returns a substring of {remaining} from its start to the first occurence of {delimiter}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {delimiter}.
 *
 * If {delimiter} is not found, {remaining} will be returned and {remaining} will be set to an empty String.
 * Therefore, if {remaining} is empty, there are no more occurences of {delimiter} in the String.
 */
String str_splitAtCString(String * remaining, char * delimiter);



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
    u64 capacity;
} StringBuilder;

/*!
 * Creates a new StringBuilder with initial capacity {initialSize}.
 *
 * Will return a StringBuilder with capacity 0 on error or if {initialSize} is 0.
 */
StringBuilder strbuilder_create(u64 initialSize);

/*!
 * Destroy {stringBuilder}.
 *
 * As {stringBuilder}->string and {stringBuilder} share the same data, only one of these should be destroyed.
 */
void strbuilder_destroy(StringBuilder * stringBuilder);

/*!
 * Returns a copy of the String in {stringBuilder}.
 */
String strbuilder_getStringCopy(StringBuilder stringBuilder);

/*!
 * Sets the capacity of {stringBuilder} to {capacity}.
 *
 * Returns whether setting the capacity was successful.
 */
bool strbuilder_setCapacity(StringBuilder * stringBuilder, u64 capacity);

/*!
 * If {requiredCapacity} is greater than the current capacity of {stringBuilder}, the
 * capacity of {stringBuilder} will be attempted to be increased to the next power of 2.
 *
 * Returns whether it was able to ensure that {stringBuilder} has a capacity of at least {requiredCapacity}.
 */
bool strbuilder_ensureCapacity(StringBuilder * stringBuilder, u64 requiredCapacity);

/*!
 * Attempts to reduce the capacity of {stringBuilder} so it is equal to the length of its contents.
 *
 * Returns whether reducing the capacity to its length was successful.
 */
bool strbuilder_trimToLength(StringBuilder * stringBuilder);

/*!
 * Will append {character} to {stringBuilder}.
 *
 * Returns whether appending {character} was succesful.
 */
bool strbuilder_appendChar(StringBuilder * stringBuilder, char character);

/*!
 * Will append {string} to {stringBuilder}.
 *
 * Returns whether appending {string} was successful.
 */
bool strbuilder_appendString(StringBuilder * stringBuilder, String string);

/*!
 * Will append {string} to {stringBuilder}.
 *
 * Returns whether appending {string} was successful.
 */
bool strbuilder_appendCString(StringBuilder * stringBuilder, char * string);

/*!
 * Will append a substring of {string} between {start} and {end} to {stringBuilder}.
 */
bool strbuilder_appendSubstring(StringBuilder * stringBuilder, String string, u64 start, u64 end);



//
// String conversions
//

/*!
 *
 */
String u64_arrayToString(u64 * array, u64 length);



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
bool utf8_appendCodepoint(StringBuilder * builder, u32 codepoint);

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
bool utf16le_appendCodepoint(StringBuilder * builder, u32 codepoint);



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
    u64 capacity;
} Buffer;

/*!
 * Allocates a new Buffer with capacity {capacity}.
 *
 * Will return an empty Buffer on error or if {capacity} is 0.
 */
Buffer buffer_create(u64 capacity);

/*!
 * Create a new Buffer using the data at {start} with capacity {capacity}.
 *
 * Will return an empty Buffer if {start} is NULL or if {capacity} is 0.
 */
Buffer buffer_createUsing(char * start, u64 capacity);

/*!
 * Create an empty Buffer.
 */
Buffer buffer_createEmpty();

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
 * Check whether {buffer} is empty.
 */
bool buffer_isEmpty(Buffer buffer);

/*!
 * Destroy {buffer} and free its contents.
 */
void buffer_destroy(Buffer * buffer);

/*!
 * Sets the capacity of {buffer} to {capacity}.
 *
 * Returns whether setting the capacity was successful.
 */
bool buffer_setCapacity(Buffer * buffer, u64 capacity);

/*!
 * If {requiredCapacity} is greater than the current capacity of {buffer}, the
 * capacity of {buffer} will be attempted to be increased to the next power of 2.
 *
 * Returns whether it was able to ensure that {buffer} has a capacity of at least {requiredCapacity}.
 */
bool buffer_ensureCapacity(Buffer * buffer, u64 requiredCapacity);

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
    u64 used;
} Stack;

/*!
 * Creates a new Stack with an initial capacity of {initialCapacity} chars.
 *
 * Will return a Stack with capacity 0 on error or if {initialCapacity} is 0.
 */
Stack stack_create(u64 initialCapacity);

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
bool stack_setCapacity(Stack * stack, u64 capacity);

/*!
 * If {requiredCapacity} is greater than the current capacity of {stack}, the
 * capacity of {stack} will be attempted to be increased to the next power of 2.
 *
 * Returns whether it was able to ensure that {stack} has a capacity of at least {requiredCapacity}.
 */
bool stack_ensureCapacity(Stack * stack, u64 requiredCapacity);

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
char * stack_reserve(Stack * stack, u64 length);

/*!
 * Append {length} chars from {data} to {stack}.
 *
 * Returns a pointer to the start of the appended data.
 *
 * Will return NULL if it is unable to append {data}.
 */
char * stack_appendData(Stack * stack, char * data, u64 length);

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
 *    static Stack * stack_createOf{type}(u64 initialCapacity)
 *
 * e.g. For Buffer
 *    static Stack * stack_createOfBuffer(u64 initialCapacity)
 *
 * Creates a new Stack with an inital capacity of {intialCapacity} elements of type {type}.
 *
 * Will return a Stack with capacity 0 on error or if {intialCapacity} is 0.
 */
#define stack_defineTypedCreate(type)                          \
    static Stack stack_createOf##type(u64 initialCapacity) {   \
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
 *    static {type} * stack_appendMany{type}(Stack * stack, {type} * data, u64 count)
 *
 * e.g. For Buffer
 *    static Buffer * stack_appendManyBuffer(Stack * stack, Buffer * data, u64 count)
 *
 * Returns a pointer to the bottom most appended element.
 *
 * Will return NULL if it is unable to append the values.
 */
#define stack_defineTypedAppendMany(type)                                               \
    static type * stack_appendMany##type(Stack * stack, type * data, u64 count) {       \
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
 *    static {type} * stack_popMany{type}(Stack * stack, u64 count)
 *
 * e.g. For Buffer
 *    static Buffer * stack_popManyBuffer(Stack * stack, u64 count)
 *
 * Returns a pointer to the bottom-most element that was popped from {stack}.
 *
 * Will return NULL if there are not {count} values on {stack}.
 */
#define stack_defineTypedPopMany(type)                              \
    static type * stack_popMany##type(Stack * stack, u64 count) {   \
        if(stack->used < count * sizeof(type))                      \
            return NULL;                                            \
                                                                    \
        stack->used -= count * sizeof(type);                        \
                                                                    \
        return (type *) stack_getEnd(*stack);                       \
    }

#endif