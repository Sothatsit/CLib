#ifndef __CLIB_json_h
#define __CLIB_json_h

#include "datatypes.h"

/*
 * The possible value types in Json.
 */
typedef enum JsonType {
    JSON_NONE,

    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL,

    JSON_STRING,
    JSON_NUMBER,

    JSON_OBJECT,
    JSON_ARRAY
} JsonType;

/*
 * JsonType values as strings.
 */
extern const char * JsonTypeNames[];

/*
 * A Json object or array.
 */
typedef struct JsonObject {
    /*
     * The children JsonNodes of the object.
     */
    struct JsonNode * children;

    /*
     * The number of children this object contains.
     */
    u32 size;
} JsonObject;

/*
 * A Json value.
 */
typedef union JsonValue {
    /*
     * Used for types JSON_STRING or JSON_NUMBER.
     */
    String string;

    /*
     * Used for types JSON_OBJECT or JSON_ARRAY.
     */
    JsonObject object;
} JsonValue;

/*
 * A key to a value in a JsonObject.
 */
typedef union JsonKey {
    /*
     * A String key in an object.
     */
    String name;

    /*
     * An index in an array.
     */
    u32 index;
} JsonKey;

/*
 * Flags describing a JsonNode.
 */
typedef struct JsonNodeFlags {
    /*
     * The type of the JsonNode.
     */
    JsonType type:3;

    /*
     * The type of the JsonNode's parent.
     */
    JsonType parentType:3;

    /*
     * Whether this is the root JsonNode.
     */
    bool isRoot:1;
} JsonNodeFlags;

/*
 * A value in a JsonObject.
 */
typedef struct JsonNode {
    /*
     * Flags describing the node.
     */
    JsonNodeFlags flags;

    /*
     * The key of the value in its object.
     */
    JsonKey key;

    /*
     * The value for an object, array, string or number.
     */
    JsonValue value;
} JsonNode;

/*
 * The result of parsing a json string.
 */
typedef struct JsonParseResult {
    /*
     * The input that was parsed.
     */
    String input;

    /*
     * The root JsonNode.
     */
    JsonNode * root;

    /*
     * Whether there was an error in parsing.
     */
    bool isError;

    /*
     * The error that occurred.
     */
    String error;

    /*
     * The index that the error occured around.
     */
    u64 errorIndex;
} JsonParseResult;

/*
 * Parse {string} into a JsonNode.
 */
JsonParseResult json_parse(String string);

/*
 * Print an error report with the error in {result}.
 */
void json_parse_printError(JsonParseResult result);

/*
 * Quote {unquoted} so that it can be placed into a json string.
 */
String json_quoteString(String unquoted);

/*
 * Append the quoted version of {unquoted} into {builder}.
 *
 * Returns whether it was successful.
 */
bool json_quoteStringInto(String unquoted, StringBuilder * builder);

/*
 * Stringify {node} into a String.
 */
String json_stringify(JsonNode * node);

/*
 * Append the String version of {value} into {builder}.
 */
void json_stringifyInto(JsonNode * value, StringBuilder * builder);

/*
 * A String that is used when indenting values.
 */
#ifndef JSON_PRETTIFY_INDENTATION
#define JSON_PRETTIFY_INDENTATION str_create("   ")
#endif

/*
 * Prettify {node} into a String.
 */
String json_prettify(JsonNode * node);

/*
 * Append the prettified String version of {value} into {builder}.
 */
void json_prettifyInto(JsonNode * value, StringBuilder * builder, u64 depth);

#endif