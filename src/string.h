#ifndef JSON_util_string_h
#define JSON_util_string_h

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

typedef struct String String;

struct String {
    char * data;
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
 * This will use the characters in {data}, if {data} is free'd the returned String
 * will be invalid. Use str_createCopy to create a copy when creating a String.
 *
 * str_destroy may be used on the returned String and will free data.
 */
String str_create(char * data);

/*
 * Create a String from the C string {data}, copying {data} instead of using it.
 *
 * str_destroy should be used on the returned String once it is no longer being used.
 */
String str_createCopy(char * data);

/*
 * Create a String with the characters in {data} with the length {length}.
 *
 * This will use the characters in {data}, if {data} is free'd the String
 * will be invalid. Use str_copy to create a copy of the data.
 *
 * str_destroy may be used on the returned String and will free the passed in data.
 */
String str_createOfLength(char * data, u64 length);

/*
 * Create a copy of the String with the characters in {data} with the length {length}.
 *
 * str_destroy should be used on the returned String once it is no longer being used.
 */
String str_createCopyOfLength(char * data, u64 length);

/*
 * Create a String with the length {length}.
 *
 * str_destroy should be used on the returned String when no longer in use.
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
 */
char * str_toCString(String string);

/*
 * Creates a String from the printf format string {format}.
 *
 * Will return an empty string on failure.
 */
String str_format(char * format, ...);

/*
 * Creates a String from the printf format string {format}.
 *
 * Will return an empty string on failure.
 */
String str_vformat(char * format, va_list arguments);

/*
 * Create a copy of {string}.
 */
String str_copy(String string);

/*
 * Check if {string1} and {string2} match.
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
 * Find the index of the first occurence of {find} in {string}. Will return -1 if find is not found.
 */
s64 str_indexOfChar(String string, char find);

/*
 * Find the index of the first occurence of {find} in {string}. Will return -1 if find is not found.
 */
s64 str_indexOfString(String string, String find);

/*
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if find is not found after or at {index}.
 */
s64 str_indexOfCharAfterIndex(String string, char find, u64 index);

/*
 * Find the index of the first occurence of {find} after or at {index} in {string}.
 *
 * Will return -1 if find is not found after or at {index}.
 */
s64 str_indexOfStringAfterIndex(String string, String find, u64 index);

/*
 * Find the index of the last occurence of {find} in {string}. Will return -1 if find is not found.
 */
s64 str_lastIndexOfChar(String string, char find);

/*
 * Find the index of the last occurence of {find} in {string}. Will return -1 if find is not found.
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
 * Set the characters in {string} from {index} to the characters in {replacement}.
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
 * This will return a new String. This new String
 * must be destroyed when it's no longer in use.
 *
 * Will return an empty String on failure.
 */
String str_replaceString(String string, String find, String replacement);

/*
 * Replace all occurences of {find} in {string} with {replacement}, modifying {string}.
 *
 * The length of {find} must be greater than or equal
 * to the length of {replacement} for this operation.
 *
 * Will return an empty String on failure.
 */
String str_replaceStringInPlace(String string, String find, String replacement);

/*
 * Concatenate string1 and string2 into a new String.
 *
 * Will return an empty String on failure.
 */
String str_concat(String string1, String string2);

/*
 * Get a substring of string inclusive from the index start to end - 1.
 *
 * The length of the returned String will be end - start.
 *
 * The returned String will use the data of string. Therefore the returned String
 * does not need to be destroyed and destroying string will make the returned
 * String invalid. Use str_copy on the returned String if this is not wanted.
 *
 * If end <= start an empty String will be returned. Will do a bounds check.
 */
String str_substring(String string, u64 start, u64 end);

/*
 * Get a substring of string with the leading and trailing whitespace omitted.
 *
 * The returned String will use the data of the String passed in. Therefore the returned String
 * does not need to be destroyed and destroying the String passed in will make the returned
 * String invalid. Use str_copy on the returned String if this is not wanted.
 */
String str_trim(String string);

/*
 * Get a substring of string with the leading whitespace omitted.
 *
 * The returned String will use the data of the String passed in. Therefore the returned String
 * does not need to be destroyed and destroying the String passed in will make the returned
 * String invalid. Use str_copy on the returned String if this is not wanted.
 */
String str_trimLeading(String string);

/*
 * Get a substring of string with the trailing whitespace omitted.
 *
 * The returned String will use the data of the String passed in. Therefore the returned String
 * does not need to be destroyed and destroying the String passed in will make the returned
 * String invalid. Use str_copy on the returned String if this is not wanted.
 */
String str_trimTrailing(String string);

/*
 * Returns a substring of remaining inclusive from the start of remaining to index - 1.
 * Sets remaining to a substring of remaining from index + delimiterLength.
 *
 * If index is -1 the returned String will be equal to remaining and remaining will be set to an empty string.
 */
String str_splitAt(String * remaining, s64 index, u64 delimiterLength);

/*
 * Returns a substring of remaining from the start to the first occurence of find.
 * Sets remaining to a substring of remaining after the first occurence of find.
 *
 * If find is not found the returned String will be equal to
 * remaining and remaining will be set to an empty string.
 *
 * Therefore, if remaining is empty, the returned String is the last element of the split string.
 */
String str_splitAtFirstChar(String * remaining, char find);

/*
 * Returns a substring of remaining from the start to the first occurence of find.
 * Sets remaining to a substring of remaining after the first occurence of find.
 *
 * If find is not found the returned String will be equal to
 * remaining and remaining will be set to an empty string.
 *
 * Therefore, if remaining is empty, the returned String is the last element of the split string.
 */
String str_splitAtFirstString(String * remaining, String find);

/*
 * Construct a UTF-8 String with the character in the UCS codepoint.
 *
 * Will return an empty string on failure.
 */
String str_UCSCodepointToUTF8(u32 codepoint);



//
// String Builder
//

struct StringBuilder {
    String string;
    u64 capacity;
};

typedef struct StringBuilder StringBuilder;

/*
 * Creates a new StringBuilder for constructing Strings.
 *
 * If initialSize is 0, the initial size of the StringBuilder will be defaulted to 32 characters.
 *
 * Will return a StringBuilder with capacity 0 on error.
 */
StringBuilder strbuilder_create(u64 initialSize);

/*
 * Destroys stringBuilder.
 *
 * This will destroy the String recieved from StringBuilder.string as well. Only the StringBuilder or
 * the String recieved from StringBuilder.string need to be destroyed, they share the same data.
 */
void strbuilder_destroy(StringBuilder stringBuilder);

/*
 * Returns a String with a copy of the String in stringBuilder.
 */
String strbuilder_getStringCopy(StringBuilder stringBuilder);

/*
 * Sets the capacity of stringBuilder to capacity.
 *
 * Returns whether setting the capacity was successful.
 */
bool strbuilder_setCapacity(StringBuilder * stringBuilder, u64 capacity);

/*
 * Will increase the capacity of stringBuilder until it's capacity is at least requiredCapacity.
 */
bool strbuilder_ensureCapacity(StringBuilder * stringBuilder, u64 requiredCapacity);

/*
 * Attempts to reduce the capacity of stringBuilder so it is equal to the length of it's contents.
 */
bool strbuilder_trimToLength(StringBuilder * stringBuilder);

/*
 * Will append character to stringBuilder.
 *
 * Will return true if successful.
 */
bool strbuilder_appendChar(StringBuilder * stringBuilder, char character);

/*
 * Will append string to stringBuilder.
 *
 * Will return true if successful.
 */
bool strbuilder_appendString(StringBuilder * stringBuilder, String string);

/*
 * Will append string to stringBuilder.
 *
 * Will return true if successful.
 */
bool strbuilder_appendCString(StringBuilder * stringBuilder, char * string);
/*
 * Append a substring of string inclusive from the index start to end - 1 to stringBuilder.
 */
bool strbuilder_appendSubstring(StringBuilder * stringBuilder, String string, u64 start, u64 end);

#endif