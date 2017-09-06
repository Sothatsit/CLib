#ifndef __CLIB_string_h
#define __CLIB_string_h

#include "numbers.h"

#include <stdbool.h>
#include <stdarg.h>

//
// Characters
//

/*
 * Returns whether {c} is whitespace.
 *
 * A character is considered whitespace if it is one of the following:
 * - Horizontal Tab '\t'
 * - New Line '\n'
 * - Carriage Return '\r'
 * - Space ' '
 */
#define char_isWhitespace(c) (c == '\t' || c == '\n' || c == '\r' || c == ' ')

/*
 * Returns whether {c} is a control character.
 */
#define char_isControlCharacter(c) (c >= 0 && c <= 31)

/*
 * Returns whether {c} is a digit.
 */
#define char_isDigit(c) (c >= '0' && c <= '9')

/*
 * Returns whether {c} is a standard English letter (a-z A-Z).
 */
#define char_isLetter(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))

/*
 * Returns whether {c} is lowercase. If {c} is not a letter as defined by char_isLetter, false will be returned.
 */
#define char_isLowercase(c) (c >= 'a' && c <= 'z')

/*
 * Returns whether {c} is uppercase. If {c} is not a letter as defined by char_isLetter, false will be returned.
 */
#define char_isUppercase(c) (c >= 'A' && c <= 'Z')

/*
 * Converts {c} to lowercase. If {c} is not uppercase as defined by
 * char_isUppercase, the returned character will be the same.
 */
#define char_toLowercase(c) (char_isUppercase(c) ? c + ('a' - 'A') : c)

/*
 * Converts {c} to uppercase. If {c} is not lowercase as defined by
 * char_isLowercase, the returned character will be the same.
 */
#define char_toUppercase(c) (char_isLowercase(c) ? c - ('a' - 'A') : c)



//
// Strings
//

/*
 * A blob of data of a set length of chars.
 */
typedef struct String String;

struct String {
    /*
     * The pointer to the start of the blob of data.
     */
    char * data;

    /*
     * The length, in chars, of the blob of data.
     */
    u64 length;
};

/*
 * Returns the character at {index} in {string}.
 *
 * This does not perform a bounds check.
 */
#define str_get(string, index) (string.data[index])

/*
 * Set the character at {index} in {string} to {character}.
 *
 * This does not perform a bounds check.
 */
#define str_set(string, index, character) (string.data[index] = character)

/*
 * Returns a String with length 0.
 */
#define str_createEmpty() (str_createOfLength(NULL, 0))

/*
 * Returns whether {string} is empty.
 */
#define str_isEmpty(string) (string.length == 0)

/*
 * Create a String from the C string {data}.
 *
 * This will use the characters in {data}, if {data} is freed the returned String
 * will be invalid. Use str_createCopy to create a copy when creating a String.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_create(char * data);

/*
 * Create a String from the C string {data}, copying {data} instead of using it.
 *
 * str_destroy should be used on the returned String once it is no longer being used.
 */
String str_createCopy(char * data);

/*
 * Create a String from the characters in {data} with the length {length}.
 *
 * This will use the characters in {data}, if {data} is freed the returned
 * String will be invalid. Use str_createCopyOfLength to create a copy of the data.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_createOfLength(char * data, u64 length);

/*
 * Create a String from the characters in {data} with the length {length}, copying {data} instead of using it.
 *
 * str_destroy may be used on the returned String and will free {data}.
 * str_destroy should not be used on this String if {data} should not be freed.
 */
String str_createCopyOfLength(char * data, u64 length);

/*
 * Create a String filled with uninitialised data of length {length}.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_createUninitialised(u64 length);

/*
 * Destroy {string}.
 *
 * Any Strings derived from {string} (e.g. substrings) will be
 * invalid after this operation unless copied using str_copy.
 *
 * If {string} is a substring of another String, the result of this operation is
 * undefined and may error. The original String should be destroyed instead.
 */
void str_destroy(String string);

/*
 * Creates a null terminated version of {string}.
 *
 * The returned null terminated string should be freed.
 */
char * str_toCString(String string);

/*
 * Creates a String from the printf format string {format}.
 *
 * Will return an empty String on failure.
 */
String str_format(char * format, ...);

/*
 * Creates a String from the printf format string {format} and the arguments {arguments}.
 *
 * Will return an empty String on failure.
 */
String str_vformat(char * format, va_list arguments);

/*
 * Create a copy of {string}.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 */
String str_copy(String string);

/*
 * Check if {string1} and {string2} contain the same data.
 */
bool str_equals(String string1, String string2);

/*
 * Check if {string} starts with {prefix}.
 */
bool str_startsWith(String string, String prefix);

/*
 * Check if {string} ends with {suffix}.
 */
bool str_endsWith(String string, String suffix);

/*
 * Find the index of the first occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_indexOfChar(String string, char find);

/*
 * Find the index of the first occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_indexOfString(String string, String find);

/*
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if {find} is not found after or at {index}.
 */
s64 str_indexOfCharAfterIndex(String string, char find, u64 index);

/*
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if {find} is not found after or at {index}.
 */
s64 str_indexOfStringAfterIndex(String string, String find, u64 index);

/*
 * Find the index of the last occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_lastIndexOfChar(String string, char find);

/*
 * Find the index of the last occurence of {find} in {string}. Will return -1 if {find} is not found.
 */
s64 str_lastIndexOfString(String string, String find);

/*
 * Check whether {string} contains {find}.
 */
bool str_containsChar(String string, char find);

/*
 * Check whether {string} contains {find}.
 */
bool str_containsString(String string, String find);

/*
 * Modifies all the characters in {string} to make them uppercase as defined by char_toUppercase.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 */
void str_toUppercase(String string);

/*
 * Modifies all the characters in {string} to make them lowercase as defined by char_toLowercase.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 */
void str_toLowercase(String string);

/*
 * Set the characters in {string} at {index} to the characters in {replacement}.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 *
 * Will not perform a bounds check to make sure replacement will fit within string.
 */
void str_setChars(String string, u64 index, String replacement);

/*
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will modify {string}, if this is unwanted use str_copy beforehand.
 */
void str_replaceChar(String string, char find, char replacement);

/*
 * Replace all occurences of {find} in {string} with {replacement}.
 *
 * This will return a new String. This new String should be destroyed when it's no longer in use.
 *
 * Will return an empty String on failure.
 */
String str_replaceString(String string, String find, String replacement);

/*
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * Requires that the length of {find} is greater than or equal to the length of {replacement}.
 *
 * Will return an empty String on failure.
 */
String str_replaceStringInPlace(String string, String find, String replacement);

/*
 * Concatenate {string1} and {string2} into a new String.
 *
 * str_destroy should be used on the returned String when it is no longer being used.
 *
 * Will return an empty String on failure.
 */
String str_concat(String string1, String string2);

/*
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

/*
 * Get a substring of {string} with the leading and trailing whitespace omitted.
 *
 * str_destroy should not be used on the returned String, as it uses the data from {string}.
 * As the returned String uses the data from {string}, if {string} is destroyed the returned
 * String will be invalid. Use str_copy on the returned String if this is unwanted.
 */
String str_trim(String string);

/*
 * Get a substring of {string} with the leading whitespace omitted.
 *
 * str_destroy should not be used on the returned String, as it uses the data from {string}.
 * As the returned String uses the data from {string}, if {string} is destroyed the returned
 * String will be invalid. Use str_copy on the returned String if this is unwanted.
 */
String str_trimLeading(String string);

/*
 * Get a substring of {string} with the trailing whitespace omitted.
 *
 * str_destroy should not be used on the returned String, as it uses the data from {string}.
 * As the returned String uses the data from {string}, if {string} is destroyed the returned
 * String will be invalid. Use str_copy on the returned String if this is unwanted.
 */
String str_trimTrailing(String string);

/*
 * Returns a substring of {remaining} inclusive from its start to {index} - 1.
 * Sets {remaining} to a substring of {remaining} from {index} + {delimiterLength}.
 *
 * If {index} is -1, {remaining} will be returned and {remaining} will be set to an empty String.
 */
String str_splitAt(String * remaining, s64 index, u64 delimiterLength);

/*
 * Returns a substring of {remaining} from its start to the first occurence of {find}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {find}.
 *
 * If {find} is not found, {remaining} will be returned and {remaining} will be set to an empty String.
 * Therefore, if {remaining} is empty, there are no more occurences of {find} in the String.
 */
String str_splitAtFirstChar(String * remaining, char find);

/*
 * Returns a substring of {remaining} from its start to the first occurence of {find}.
 * Sets {remaining} to a substring of {remaining} after the first occurence of {find}.
 *
 * If {find} is not found, {remaining} will be returned and {remaining} will be set to an empty String.
 * Therefore, if {remaining} is empty, there are no more occurences of {find} in the String.
 */
String str_splitAtFirstString(String * remaining, String find);

/*
 * Construct a UTF-8 String from the character in the UCS codepoint.
 *
 * Will return an empty String on failure.
 */
String str_UCSCodepointToUTF8(u32 codepoint);



//
// String Builder
//

/*
 * A utility for creating Strings of unknown length.
 */
struct StringBuilder {
    /*
     * The underlying String that is being built.
     *
     * This String can be used from the StringBuilder at any time, and it or
     * the StringBuilder should be destroyed after they are both not in use.
     *
     * If this String is taken from the StringBuilder, any
     * appendages to the StringBuilder will not affect its value.
     */
    String string;

    /*
     * The current capacity that the StringBuilder can hold before it has to expand.
     */
    u64 capacity;
};

typedef struct StringBuilder StringBuilder;

/*
 * Creates a new StringBuilder with initial capacity {initialSize}.
 *
 * Will return a StringBuilder with capacity 0 on error or if {initialSize} is 0.
 */
StringBuilder strbuilder_create(u64 initialSize);

/*
 * Destroy {stringBuilder}.
 *
 * As {stringBuilder}.string and {stringBuilder} share the same data, only one of these should be destroyed.
 */
void strbuilder_destroy(StringBuilder stringBuilder);

/*
 * Returns a copy of the String in {stringBuilder}.
 */
String strbuilder_getStringCopy(StringBuilder stringBuilder);

/*
 * Sets the capacity of {stringBuilder} to {capacity}.
 *
 * Returns whether setting the capacity was successful.
 */
bool strbuilder_setCapacity(StringBuilder * stringBuilder, u64 capacity);

/*
 * If {requiredCapacity} is greater than the current capacity of {stringBuilder}, the
 * capacity of {stringBuilder} will be attempted to be increased to the next power of 2.
 *
 * Returns whether it was able to ensure that {stringBuilder} has a capacity of at least {requiredCapacity}.
 */
bool strbuilder_ensureCapacity(StringBuilder * stringBuilder, u64 requiredCapacity);

/*
 * Attempts to reduce the capacity of {stringBuilder} so it is equal to the length of its contents.
 *
 * Returns whether reducing the capacity to the length of its was successful.
 */
bool strbuilder_trimToLength(StringBuilder * stringBuilder);

/*
 * Will append {character} to {stringBuilder}.
 *
 * Returns whether appending {character} was succesful.
 */
bool strbuilder_appendChar(StringBuilder * stringBuilder, char character);

/*
 * Will append {string} to {stringBuilder}.
 *
 * Returns whether appending {string} was successful.
 */
bool strbuilder_appendString(StringBuilder * stringBuilder, String string);

/*
 * Will append {string} to {stringBuilder}.
 *
 * Returns whether appending {string} was successful.
 */
bool strbuilder_appendCString(StringBuilder * stringBuilder, char * string);

/*
 * Will append a substring of {string} between {start} and {end} to {stringBuilder}.
 */
bool strbuilder_appendSubstring(StringBuilder * stringBuilder, String string, u64 start, u64 end);

#endif