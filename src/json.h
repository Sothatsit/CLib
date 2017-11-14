#ifndef __CLIB_json_h
#define __CLIB_json_h

#include "datatypes.h"

/*!
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

/*!
 * JsonType values as strings.
 */
extern const char * JsonTypeNames[];

/*!
 * A Json object or array.
 */
typedef struct JsonObject {
    /*!
     * The offset of the children JsonNodes of the object relative to the root node.
     */
    u32 childIndex;

    /*!
     * The number of children this object contains.
     */
    u32 size;
} JsonObject;

/*!
 * A Json value.
 */
typedef union JsonValue {
    /*!
     * Used for types JSON_STRING or JSON_NUMBER.
     */
    String string;

    /*!
     * Used for types JSON_OBJECT or JSON_ARRAY.
     */
    JsonObject object;
} JsonValue;

/*!
 * A key to a value in a JsonObject.
 */
typedef union JsonKey {
    /*!
     * A String key in an object.
     */
    String name;

    /*!
     * An index in an array.
     */
    u32 index;
} JsonKey;

/*!
 * A value in a JsonObject.
 */
typedef struct JsonNode {
    /*!
     * The type of the JsonNode.
     */
    JsonType type:4;

    /*!
     * The type of the JsonNode's parent.
     */
    JsonType parentType:4;

    /*!
     * The key of the value in its object.
     */
    JsonKey key;

    /*!
     * The value for an object, array, string or number.
     */
    JsonValue value;
} JsonNode;

/*!
 * The root of a parsed json tree.
 */
typedef struct JsonRoot {
    /*!
     * The nodes in this tree.
     *
     * The first element in this array, nodes[0], should
     * be the root JsonNode if this is a valid JsonRoot.
     */
    JsonNode * nodes;

    /*!
     * The number of nodes in the tree.
     */
    u32 size;
} JsonRoot;

/*!
 * The result of parsing a json string.
 */
typedef struct JsonParseResult {
    /*!
     * The input that was parsed.
     */
    String input;

    /*!
     * The root of the json tree.
     */
    JsonRoot root;

    /*!
     * Whether there was an error in parsing.
     */
    bool isError;

    /*!
     * The error that occurred.
     */
    String error;

    /*!
     * The index that the error occured around.
     */
    u64 errorIndex;
} JsonParseResult;

/*!
 * A re-usable context for parsing json Strings.
 */
typedef struct JsonParseContext JsonParseContext;

/*!
 * Create a JsonParseContext to allow re-using resources when parsing json.
 */
JsonParseContext json_parseContext_create();

/*!
 * Destroy {context} to free its data.
 */
void json_parseContext_destroy(JsonParseContext * context);

/*!
 * Use {context} to parse {string} into a JsonNode.
 *
 * This will override data used from previous parses using {context}, invalidating them.
 */
JsonParseResult json_parseContext_parse(JsonParseContext * context, String string);

/*!
 * Parse {string} into a JsonNode.
 *
 * This will modify and use the data from {string} to store parsed strings and numbers.
 */
JsonParseResult json_parse(String string);

/*!
 * Copy {result} so that it can be kept while the JsonParseContext
 * used to create it can be re-used without overwriting it.
 */
JsonParseResult json_parseResult_copy(JsonParseResult result);

/*!
 * Create an empty JsonNode.
 */
JsonNode json_node_createEmpty();

/*!
 * Check whether a JsonNode is empty.
 *
 * This is not for checking whether an object has no children.
 */
bool json_node_isEmpty(JsonNode node);

/*!
 * Get the JsonNode at the index {index} in {root}.
 *
 * Will return an empty JsonNode on error.
 */
JsonNode json_root_get(JsonRoot root, u32 index);

/*!
 * Get the JsonNode at the index {index} in the object or array node {node}.
 *
 * Will return a JsonNode with type JSON_NONE on error.
 */
JsonNode json_obj_get(JsonRoot root, JsonNode node, u32 index);

/*!
 * Find a child with the key {key} in the object node {node}.
 *
 * Will return a JsonNode with type JSON_NONE if a child with key {key} cannot be found or on error.
 */
JsonNode json_obj_find(JsonRoot root, JsonNode node, String key);

/*!
 * Find the index in root of a child with the key {key} in the object node {node}.
 *
 * Will return -1 if an object with the key {key} cannot be found or on error.
 */
s32 json_obj_findIndex(JsonRoot root, JsonNode node, String key);

/*!
 * Destroy {result} to free its data.
 *
 * This should only be used from results obtained from json_parse or json_parseResult_copy.
 */
void json_parseResult_destroy(JsonParseResult * result);

/*!
 * Print an error report with the error in {result}.
 */
void json_parseResult_printError(JsonParseResult result);

/*!
 * Quote {unquoted} so that it can be placed into a json string.
 */
String json_quoteString(String unquoted);

/*!
 * Append the quoted version of {unquoted} into {builder}.
 *
 * Returns whether it was successful.
 */
bool json_quoteStringInto(String unquoted, StringBuilder * builder);

/*!
 * Stringify {node} into a String.
 */
String json_stringify(JsonRoot root, JsonNode node);

/*!
 * Append the String version of {value} into {builder}.
 */
void json_stringifyInto(JsonRoot root, JsonNode node, StringBuilder * builder);

#endif