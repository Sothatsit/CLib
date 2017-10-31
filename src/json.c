#include <stdio.h>
#include <memory.h>
#include "json.h"

const char * JsonTypeNames[] = {
        "JSON_NONE",

        "JSON_TRUE",
        "JSON_FALSE",
        "JSON_NULL",

        "JSON_STRING",
        "JSON_NUMBER",

        "JSON_OBJECT",
        "JSON_ARRAY"
};



//
// Parsing
//

/*
 * Stores data used while constructing a tree of Json data.
 */
typedef struct JsonParseContext {
    /*
     * The original String being parsed.
     */
    String input;

    /*
     * The remaining part of the Json string being parsed.
     */
    String remaining;

    /*
     * Stores JsonNodes of objects currently being created
     */
    Stack nodeStack;

    /*
     * Stores finalised JsonNodes and Strings
     */
    Stack dataStack;

    /*
     * The result of parsing.
     */
    JsonParseResult result;
} JsonParseContext;

/*
 * Define functions used to manipulate the Stacks used to construct a JsonObject.
 */
define_stack_typedFunctions(JsonNode);

/*
 * Construct a JsonParseContext to parse {string}.
 */
JsonParseContext json_parse_create(String string);

/*
 * Complete the JsonParseContext {context} by destroying its temporary stacks.
 *
 * Returns a reference to the root JsonNode.
 */
bool json_parse_complete(JsonParseContext * context);

/*
 * Destroy the JsonParseContext completely if parsing fails and no valid result can be returned.
 */
void json_parse_destroy(JsonParseContext * context);

/*
 * Report an error that occured while parsing from {context}.
 *
 * Will always return false.
 */
bool json_parse_error(JsonParseContext * context, char * message);

/*
 * Append the JsonNode {node} to the current object being constructed in {context}.
 */
bool json_parse_appendNode(JsonParseContext * context, JsonNode node);

/*
 * Complete creation of a JsonParseObject and append its JsonNode value to its parent object.
 */
bool json_parse_completeObject(JsonParseContext * context, JsonNodeFlags flags, JsonKey key, u32 size);

/*
 * Remove all the leading whitespace from the remaining string in {context}.
 *
 * Returns whether there are remaining characters in {context}.
 */
bool json_parse_trim(JsonParseContext * context);

/*
 * Get the next character in {context}.
 */
char json_parse_next(JsonParseContext * context);

/*
 * Chew {characters} characters from the remaining string in {context}.
 */
void json_parse_chew(JsonParseContext * context, u64 characters);

/*
 * Parse a json value.
 */
bool json_parse_parseValue(JsonParseContext * context, JsonNodeFlags flags, JsonKey key);

/*
 * Parse a constant value.
 */
bool json_parse_parseExpected(JsonParseContext * context, JsonNodeFlags flags, JsonKey key, String expected);

/*
 * Parse a String into {value}.
 */
bool json_parse_parseString(JsonParseContext * context, String * value);

/*
 * Parse a number.
 */
bool json_parse_parseNumber(JsonParseContext * context, JsonNodeFlags flags, JsonKey key);

/*
 * Parse an object.
 */
bool json_parse_parseObject(JsonParseContext * context, JsonNodeFlags flags, JsonKey key);

/*
 * Parse an array.
 */
bool json_parse_parseArray(JsonParseContext * context, JsonNodeFlags flags, JsonKey key);



//
// Parsing Implementation
//

JsonParseContext json_parse_create(String string) {
    JsonParseContext context;

    context.input = string;
    context.remaining = string;
    context.nodeStack = stack_createOfJsonNode(8);
    context.dataStack = stack_createOfJsonNode(8);
    context.result.input = string;
    context.result.root = (JsonNode *) context.dataStack.buffer.start;
    context.result.isError = false;
    context.result.error = str_createEmpty();
    context.result.errorIndex = 0;

    // TODO: Create functions to check whether a Stack or Buffer is invalid
    if(context.nodeStack.buffer.start == NULL) {
        json_parse_error(&context, "Unable to create nodeStack");
        return context;
    }

    if(context.dataStack.buffer.start == NULL) {
        json_parse_error(&context, "Unable to create dataStack");
        return context;
    }

    if(stack_reserve(&context.dataStack, sizeof(JsonNode)) == NULL) {
        json_parse_error(&context, "Unable to reserve space for root JsonNode on dataStack");
        return context;
    }

    return context;
}

bool json_parse_complete(JsonParseContext * context) {
    if(json_parse_trim(context))
        return json_parse_error(context, "Remaining characters after completion of parsing");

    JsonNode * node = stack_peek(JsonNode, context->nodeStack);

    if(node == NULL)
        return json_parse_error(context, "Expected root node to be left on nodeStack");

    context->result.root = (JsonNode *) context->dataStack.buffer.start;
    *context->result.root = *node;

    context->remaining = str_createEmpty();
    stack_destroy(&context->nodeStack);

    return true;
}

void json_parse_destroy(JsonParseContext * context) {
    context->remaining = str_createEmpty();

    stack_destroy(&context->nodeStack);
    stack_destroy(&context->dataStack);
}

bool json_parse_error(JsonParseContext * context, char * message) {
    //va_list argList;
    //va_start(argList, format);

    //String error = str_vformat(format, argList);

    String error = str_create(message);

    if(context->result.isError) {
        StringBuilder builder = strbuilder_create(context->result.error.length + 2 + error.length);

        strbuilder_appendString(&builder, context->result.error);
        strbuilder_appendCString(&builder, ", ");
        strbuilder_appendString(&builder, error);

        str_destroy(&context->result.error);
        str_destroy(&error);

        context->result.error = builder.string;
    } else {
        context->result.root = NULL;
        context->result.isError = true;
        context->result.error = error;

        if(str_isEmpty(context->remaining)) {
            context->result.errorIndex = context->input.length;
        } else {
            context->result.errorIndex = context->remaining.data - context->input.data;
        }
    }

    //va_end(argList);

    json_parse_destroy(context);
    return false;
}

void json_parse_printError(JsonParseResult result) {
    json_parse_printDetailedError(result, 10);
}

void json_parse_printDetailedError(JsonParseResult result, u64 charsAround) {
    if(result.isError) {
        fprintf(stderr, "Error parsing Json at index %lld:\n", result.errorIndex);
        fprintf(stderr, "   %s\n", str_toCString(result.error));

        u64 from = (result.errorIndex <= charsAround ? 0 : result.errorIndex - charsAround);
        u64 errorIndex = result.errorIndex - from;
        u64 to = min(result.errorIndex + charsAround, result.input.length);

        bool aroundCopied = false;
        String around = str_substring(result.input, from, to);
        {
            if(str_isEmpty(around))
                return;

            for(u64 index = 0; index < around.length; ++index) {
                char character = str_get(around, index);

                if(!char_isControlCharacter(character))
                    continue;

                if(!aroundCopied) {
                    around = str_copy(around);
                    aroundCopied = true;
                }

                str_set(around, index, ' ');
            }
        }

        String pointer = str_createUninitialised(errorIndex + 1);
        {
            memset(pointer.data, ' ', errorIndex);
            str_set(pointer, errorIndex, '^');
        }

        fprintf(stderr, "%s\n%s\n", str_toCString(around), str_toCString(pointer));

        if(aroundCopied) {
            str_destroy(&around);
        }
    } else {
        printf("Json parsing was successful\n");
    }
}

void json_parse_updateNodeReferences(Stack stack, char * previousStart) {
    s64 pointerChange = (s64) stack.buffer.start - (s64) previousStart;

    JsonNode * nodes = (JsonNode *) stack.buffer.start;
    u64 nodeCount = stack.used / sizeof(JsonNode);

    for(u64 index = 0; index < nodeCount; ++index) {
        JsonNode * node = &nodes[index];

        if(node->flags.parentType == JSON_OBJECT) {
            node->key.name.data += pointerChange;
        }

        switch(node->flags.type) {
            case JSON_OBJECT:
            case JSON_ARRAY:
                *((char **) &node->value.object.children) += pointerChange;
                break;

            case JSON_NUMBER:
            case JSON_STRING:
                node->value.string.data += pointerChange;
                break;

            default:
                break;
        }
    }
}

bool json_parse_appendNode(JsonParseContext * context, JsonNode node) {
    char * previousStart = context->nodeStack.buffer.start;

    JsonNode * appended = stack_appendJsonNode(&context->nodeStack, node);

    if(appended == NULL)
        return json_parse_error(context, "Failure appending value node");

    if(previousStart != context->nodeStack.buffer.start) {
        json_parse_updateNodeReferences(context->nodeStack, previousStart);
    }

    return true;
}

bool json_parse_completeObject(JsonParseContext * context, JsonNodeFlags flags, JsonKey key, u32 size) {
    JsonNode * stackNodes = stack_popManyJsonNode(&context->nodeStack, size);

    if(stackNodes == NULL)
        return json_parse_error(context, "JsonParseObject's size greater than size of node stack");;

    JsonNode * nodes = stack_appendManyJsonNode(&context->dataStack, stackNodes, size);

    if(nodes == NULL)
        return json_parse_error(context, "Unable to append JsonNodes to data stack");

    JsonNode objectNode; {
        objectNode.flags = flags;
        objectNode.key = key;
        objectNode.value.object.size = size;
        objectNode.value.object.children = nodes;
    }

    return json_parse_appendNode(context, objectNode);
}

bool json_parse_trim(JsonParseContext * context) {
    context->remaining = str_trimLeading(context->remaining);

    return context->remaining.length > 0;
}

char json_parse_next(JsonParseContext * context) {
    return context->remaining.data[0];
}

void json_parse_chew(JsonParseContext * context, u64 characters) {
    context->remaining = str_substring(context->remaining, characters, context->remaining.length);
}

JsonParseResult json_parse(String input) {
    JsonParseContext context = json_parse_create(input);

    if(context.result.isError)
        return context.result;

    if(!json_parse_trim(&context)) {
        json_parse_error(&context, "Empty input");
        return context.result;
    }

    JsonKey key = {
            .index = 0
    };

    JsonNodeFlags flags = {
            .parentType = JSON_NONE,
            .isRoot = true
    };

    if(!json_parse_parseValue(&context, flags, key))
        return context.result;

    json_parse_complete(&context);

    return context.result;
}

bool json_parse_parseValue(JsonParseContext * context, JsonNodeFlags flags, JsonKey key) {
    char next = json_parse_next(context);

    switch(next) {
        case '{':
            json_parse_chew(context, 1);
            return json_parse_parseObject(context, flags, key);
        case '[':
            json_parse_chew(context, 1);
            return json_parse_parseArray(context, flags, key);

        case 't':
            flags.type = JSON_TRUE;
            return json_parse_parseExpected(context, flags, key, str_createOfLength("true", 4));
        case 'f':
            flags.type = JSON_FALSE;
            return json_parse_parseExpected(context, flags, key, str_createOfLength("false", 5));
        case 'n':
            flags.type = JSON_NULL;
            return json_parse_parseExpected(context, flags, key, str_createOfLength("null", 4));

        case '"': {
            json_parse_chew(context, 1);

            flags.type = JSON_STRING;
            JsonNode node = {
                .flags = flags,
                .key = key
            };

            return json_parse_parseString(context, &node.value.string) && json_parse_appendNode(context, node);
        }

        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return json_parse_parseNumber(context, flags, key);

        default:
            return json_parse_error(context, "Unexpected character while expecting value");
    }
}

bool json_parse_parseExpected(JsonParseContext * context, JsonNodeFlags flags, JsonKey key, String expected) {
    if(!str_startsWith(context->remaining, expected)) {
        String message = str_createUninitialised(9 + expected.length);

        str_setChars(message, 0, str_create("Expected "));
        str_setChars(message, 9, expected);

        json_parse_error(context, str_toCString(message));

        str_destroy(&message);

        return false;
    }

    json_parse_chew(context, expected.length);

    JsonNode node; {
        node.flags = flags;
        node.key = key;
    }

    return json_parse_appendNode(context, node);
}

#define __json_parseString_parseHex4(value)                                   \
    do {                                                                      \
        u8 c;                                                                 \
                                                                              \
        for(u64 i = 0; i < 4; ++i) {                                          \
            c = (u8) context->remaining.data[index++];                        \
                                                                              \
            if(c >= '0' && c <= '9') {                                        \
                c -= '0';                                                     \
            } else if(c >= 'a' && c <= 'f') {                                 \
                c -= 'a' - 10;                                                \
            } else if(c >= 'A' && c <= 'F') {                                 \
                c -= 'A' - 10;                                                \
            } else {                                                          \
                json_parse_chew(context, index - 1);                          \
                return json_parse_error(context, "Expected hex character");   \
            }                                                                 \
                                                                              \
            value <<= 4;                                                      \
            value |= c;                                                       \
        }                                                                     \
    } while(0)

#define __json_parseString_expect(character)                                                     \
    do {                                                                                         \
        if(index >= context->remaining.length) {                                                 \
            json_parse_chew(context, index);                                                     \
            return json_parse_error(context, "Expected second half of UTF-16 surrogate pair");   \
        }                                                                                        \
        if(context->remaining.data[index++] != character) {                                      \
            json_parse_chew(context, index - 1);                                                 \
            return json_parse_error(context, "Expected second half of UTF-16 surrogate pair");   \
        }                                                                                        \
    } while (0)

bool json_parse_parseString(JsonParseContext * context, String * value) {
    value->length = 0;
    value->data = context->remaining.data;

    u64 copyFromIndex = 0;
    u64 index = 0;

    while(index < context->remaining.length) {
        char next = context->remaining.data[index++];

        if(char_isControlCharacter(next)) {
            json_parse_chew(context, index - 1);
            return json_parse_error(context, "Control character in string");
        }

        if(next == '"') {
            u64 copyBack = index - copyFromIndex - 1;
            memmove(&value->data[value->length], &context->remaining.data[copyFromIndex], copyBack);
            value->length += copyBack;

            json_parse_chew(context, index);
            return true;
        }

        if(next == '\\') {
            u64 copyBack = index - copyFromIndex - 1;
            memmove(&value->data[value->length], &context->remaining.data[copyFromIndex], copyBack);
            value->length += copyBack;

            if(index >= context->remaining.length) {
                json_parse_chew(context, index);
                return json_parse_error(context, "Unexpected eof while parsing string");
            }

            next = context->remaining.data[index++];

            if(next == 'u') {
                if(index + 3 >= context->remaining.length) {
                    json_parse_chew(context, context->remaining.length);
                    return json_parse_error(context, "Unexpected eof while parsing UCS codepoint");
                }

                u32 codepoint = 0; {
                    __json_parseString_parseHex4(codepoint);
                }

                // UTF-16 Surrogate pair
                if(codepoint >= 0xD800 && codepoint <= 0xDFFF) {
                    if(codepoint >= 0xDC00) {
                        json_parse_chew(context, index - 5);
                        return json_parse_error(context, "Unexpected second half of UTF-16 surrogate pair");
                    }

                    u16 lo = (u16) codepoint;
                    u16 hi = 0; {
                        __json_parseString_expect('\\');
                        __json_parseString_expect('u');
                        __json_parseString_parseHex4(hi);
                    }

                    if(hi <= 0xDBFF) {
                        json_parse_chew(context, index - 5);
                        return json_parse_error(context, "Expected second half of UTF-16 surrogate pair");
                    }

                    u16 buffer[2];
                    buffer[0] = lo;
                    buffer[1] = hi;

                    String utf16String = {
                            .data = (char *) buffer,
                            .length = 4
                    };

                    codepoint = utf16le_toCodepoint(&utf16String);
                }

                StringBuilder fakeBuilder = {
                        .string.data = &value->data[value->length],
                        .string.length = 0,
                        .capacity = 6
                };

                utf8_appendCodepoint(&fakeBuilder, codepoint);
                value->length += fakeBuilder.string.length;
                copyFromIndex = index;
                continue;
            }

            char escapedValue;

            switch(next) {
                case '"':
                case '\\':
                case '/':
                    escapedValue = next;
                    break;
                case 'b':
                    escapedValue = '\b';
                    break;
                case 'f':
                    escapedValue = '\f';
                    break;
                case 'n':
                    escapedValue = '\n';
                    break;
                case 'r':
                    escapedValue = '\r';
                    break;
                case 't':
                    escapedValue = '\t';
                    break;
                default:
                    json_parse_chew(context, index - 1);
                    return json_parse_error(context, "Invalid escape sequence");
            }

            value->data[value->length] = escapedValue;
            value->length += 1;

            copyFromIndex = index;
        }
    }

    return json_parse_error(context, "Unexpected eof while parsing string");
}

#undef __json_parseString_parseHex4

#define __jsonParseNumber_consumeOne()                                          \
    do {                                                                        \
        json_parse_chew(context, 1);                                            \
        number.length += 1;                                                     \
        if(context->remaining.length == 0) {                                    \
            goto finishNumber;                                                  \
        }                                                                       \
        next = json_parse_next(context);                                        \
    } while(0)

#define __jsonParseNumber_consumeOneRequireNext()                               \
    do {                                                                        \
        json_parse_chew(context, 1);                                            \
        number.length += 1;                                                     \
        if(context->remaining.length == 0) {                                    \
            json_parse_error(context, "Unexpected eof while parsing number");   \
            return false;                                                       \
        }                                                                       \
        next = json_parse_next(context);                                        \
    } while(0)

bool json_parse_parseNumber(JsonParseContext * context, JsonNodeFlags flags, JsonKey key) {
    flags.type = JSON_NUMBER;

    String number; {
        number.data = context->remaining.data;
        number.length = 0;
    }

    char next = json_parse_next(context);

    if(next == '-') {
        __jsonParseNumber_consumeOneRequireNext();
    }

    if(next == '0') {
        __jsonParseNumber_consumeOne();

        if(char_isDigit(next)) {
            json_parse_error(context, "Unexpected digit after 0 in number");
            return false;
        }
    } else if(char_isDigit(next)) {
        do {
            __jsonParseNumber_consumeOne();
        } while(char_isDigit(next));
    } else {
        return json_parse_error(context, "Expected digit");
    }

    if(next == '.') {
        __jsonParseNumber_consumeOneRequireNext();

        if(!char_isDigit(next)) {
            json_parse_error(context, "Expected digit after .");
            return false;
        }

        do {
            __jsonParseNumber_consumeOne();
        } while(char_isDigit(next));
    }

    if(next == 'e' || next == 'E') {
        __jsonParseNumber_consumeOneRequireNext();

        if(next == '+' || next == '-') {
            __jsonParseNumber_consumeOneRequireNext();
        }

        if(char_isDigit(next)) {
            do {
                __jsonParseNumber_consumeOne();
            } while(char_isDigit(next));
        } else {
            return json_parse_error(context, "Expected digit in exponent of number");
        }
    }

    finishNumber: {
        JsonNode node; {
            node.flags = flags;
            node.key = key;
            node.value.string = number;
        }

        return json_parse_appendNode(context, node);
    }
}

#undef __jsonParseNumber_consumeOne
#undef __jsonParseNumber_consumeOneRequireNext

bool json_parse_parseObject(JsonParseContext * context, JsonNodeFlags flags, JsonKey objectKey) {
    flags.type = JSON_OBJECT;

    JsonNodeFlags childFlags = {
            .parentType = JSON_OBJECT,
            .isRoot = false
    };

    u32 size = 0;
    JsonKey key;
    char next;

    while(true) {
        if(!json_parse_trim(context))
            return json_parse_error(context, "Unexpected eof while parsing object");

        next = json_parse_next(context);

        if(next == '}') {
            json_parse_chew(context, 1);
            return json_parse_completeObject(context, flags, objectKey, size);
        }

        if(size != 0) {
            if(next != ',')
                return json_parse_error(context, "Expected , or } while parsing object");

            json_parse_chew(context, 1);

            if(!json_parse_trim(context))
                return json_parse_error(context, "Unexpected eof while parsing object");

            next = json_parse_next(context);
        }

        if(next != '"')
            return json_parse_error(context, "Expected \" for start of key while parsing object");

        json_parse_chew(context, 1);

        json_parse_parseString(context, &key.name);

        if(str_isEmpty(key.name) && context->result.isError)
            return false;

        if(!json_parse_trim(context))
            return json_parse_error(context, "Unexpected eof while parsing object");

        next = json_parse_next(context);

        if(next != ':')
            return json_parse_error(context, "Expected : while parsing object");

        json_parse_chew(context, 1);

        if(!json_parse_trim(context))
            return json_parse_error(context, "Unexpected eof while parsing object");

        if(!json_parse_parseValue(context, childFlags, key))
            return false;

        size += 1;
    };

    return false;
}

bool json_parse_parseArray(JsonParseContext * context, JsonNodeFlags flags, JsonKey arrayKey) {
    flags.type = JSON_ARRAY;

    JsonNodeFlags childFlags = {
            .parentType = JSON_ARRAY,
            .isRoot = false
    };

    u32 size = 0;
    JsonKey key;
    char next;

    while(true) {
        if(!json_parse_trim(context))
            return json_parse_error(context, "Unexpected eof while parsing array");

        next = json_parse_next(context);

        if(next == ']') {
            json_parse_chew(context, 1);
            return json_parse_completeObject(context, flags, arrayKey, size);
        }

        if(size != 0) {
            if(next != ',')
                return json_parse_error(context, "Expected , or ] while parsing array");

            json_parse_chew(context, 1);

            if(!json_parse_trim(context))
                return json_parse_error(context, "Unexpected eof while parsing array");
        }

        key.index = size;

        if(!json_parse_parseValue(context, childFlags, key))
            return false;

        size += 1;
    };

    return false;
}



//
// Stringify
//

String json_quoteString(String unquoted) {
    StringBuilder builder = strbuilder_create(unquoted.length);

    json_quoteStringInto(unquoted, &builder);

    return builder.string;
}

bool json_quoteStringInto(String unquoted, StringBuilder * builder) {
    // In case we are unable to encode it as a hex character.
    String fallback;

    while(unquoted.length > 0) {
        fallback = unquoted;

        u32 codepoint = utf8_toCodepoint(&unquoted);

        if(codepoint == 0 && fallback.data[0] != '\0')
            return false;

        // Special control character encodings.
        if(char_isControlCharacter(codepoint)) {
            if(codepoint == '\b') {
                strbuilder_appendCString(builder, "\\b");
                continue;
            } else if(codepoint == '\f') {
                strbuilder_appendCString(builder, "\\f");
                continue;
            } else if(codepoint == '\n') {
                strbuilder_appendCString(builder, "\\n");
                continue;
            } else if(codepoint == '\r') {
                strbuilder_appendCString(builder, "\\r");
                continue;
            } else if(codepoint == '\t') {
                strbuilder_appendCString(builder, "\\t");
                continue;
            }
        }

        // Encode as normal character.
        if(codepoint >= 32 && codepoint <= 126) {
            switch(codepoint) {
                case '\"':
                    strbuilder_appendCString(builder, "\\\"");
                    break;
                case '\\':
                    strbuilder_appendCString(builder, "\\\\");
                    break;
                default:
                    strbuilder_appendChar(builder, (char) codepoint);
                    break;
            }

            continue;
        }

        u16 buffer[2];
        StringBuilder fakeBuilder = {
                .string.data = (char *) buffer,
                .string.length = 0,
                .capacity = 4
        };

        utf16le_appendCodepoint(&fakeBuilder, codepoint);

        const char * hexCharacters = "0123456789abcdef";

        for(u32 i = 0; i < fakeBuilder.string.length / 2; ++i) {
            u16 value = buffer[i];

            strbuilder_appendCString(builder, "\\u");
            strbuilder_appendChar(builder, hexCharacters[(value & 0xf000) >> 12]);
            strbuilder_appendChar(builder, hexCharacters[(value & 0x0f00) >> 8]);
            strbuilder_appendChar(builder, hexCharacters[(value & 0x00f0) >> 4]);
            strbuilder_appendChar(builder, hexCharacters[value & 0x000f]);
        }
    }

    return true;
}

String json_stringify(JsonNode * node) {
    StringBuilder stringBuilder = strbuilder_create(32);

    json_stringifyInto(node, &stringBuilder);

    return stringBuilder.string;
}

void json_stringifyInto(JsonNode * value, StringBuilder * builder) {
    switch(value->flags.type) {
        case JSON_TRUE:
            strbuilder_appendString(builder, str_createOfLength("true", 4));
            break;
        case JSON_FALSE:
            strbuilder_appendString(builder, str_createOfLength("false", 5));
            break;
        case JSON_NULL:
            strbuilder_appendString(builder, str_createOfLength("null", 4));
            break;
        case JSON_NUMBER:
            strbuilder_appendString(builder, value->value.string);
            break;
        case JSON_STRING:
            strbuilder_appendChar(builder, '"');
            json_quoteStringInto(value->value.string, builder);
            strbuilder_appendChar(builder, '"');
            break;

        case JSON_OBJECT: {
            JsonObject object = value->value.object;

            strbuilder_appendChar(builder, '{');

            for(int index = 0; index < object.size; ++index) {
                if(index != 0) {
                    strbuilder_appendChar(builder, ',');
                }

                JsonNode * node = &object.children[index];

                strbuilder_appendChar(builder, '"');
                json_quoteStringInto(node->key.name, builder);
                strbuilder_appendCString(builder, "\":");
                json_stringifyInto(node, builder);
            }

            strbuilder_appendChar(builder, '}');

            break;
        }

        case JSON_ARRAY: {
            JsonObject object = value->value.object;

            strbuilder_appendChar(builder, '[');

            for(int index = 0; index < object.size; ++index) {
                if(index != 0) {
                    strbuilder_appendChar(builder, ',');
                }

                JsonNode * node = &object.children[index];

                json_stringifyInto(node, builder);
            }

            strbuilder_appendChar(builder, ']');

            break;
        }

        default: {
            // TODO: Error reporting

            String string = str_format("Unknown type %s", JsonTypeNames[value->flags.type]);

            strbuilder_appendString(builder, string);

            str_destroy(&string);
            break;
        }
    }
}

String json_prettify(JsonNode * value) {
    StringBuilder stringBuilder = strbuilder_create(32);

    json_prettifyInto(value, &stringBuilder, 0);

    return stringBuilder.string;
}

void json_prettify_indent(StringBuilder * builder, u64 depth) {
    String indentation = JSON_PRETTIFY_INDENTATION;

    for(u64 index = 0; index < depth; ++index) {
        strbuilder_appendString(builder, indentation);
    }
}

void json_prettifyInto(JsonNode * value, StringBuilder * builder, u64 depth) {
    switch(value->flags.type) {
        case JSON_TRUE:
            strbuilder_appendCString(builder, "true");
            break;
        case JSON_FALSE:
            strbuilder_appendCString(builder, "false");
            break;
        case JSON_NULL:
            strbuilder_appendCString(builder, "null");
            break;
        case JSON_NUMBER:
            strbuilder_appendString(builder, value->value.string);
            break;
        case JSON_STRING:
            strbuilder_appendChar(builder, '"');
            json_quoteStringInto(value->value.string, builder);
            strbuilder_appendChar(builder, '"');
            break;

        case JSON_OBJECT: {
            JsonObject object = value->value.object;

            if(object.size > 0) {
                strbuilder_appendCString(builder, "{");

                for(int index = 0; index < object.size; ++index) {
                    if(index != 0) {
                        strbuilder_appendCString(builder, ", ");
                    }

                    strbuilder_appendChar(builder, '\n');
                    json_prettify_indent(builder, depth + 1);

                    JsonNode * node = &object.children[index];

                    strbuilder_appendChar(builder, '"');
                    json_quoteStringInto(node->key.name, builder);
                    strbuilder_appendCString(builder, "\": ");
                    json_prettifyInto(node, builder, depth + 1);
                }

                strbuilder_appendChar(builder, '\n');
                json_prettify_indent(builder, depth);
                strbuilder_appendChar(builder, '}');
            } else {
                strbuilder_appendCString(builder, "{}");
            }

            break;
        }

        case JSON_ARRAY: {
            JsonObject object = value->value.object;

            if(object.size > 0) {
                strbuilder_appendCString(builder, "[");

                for(int index = 0; index < object.size; ++index) {
                    if(index != 0) {
                        strbuilder_appendCString(builder, ", ");
                    }

                    strbuilder_appendChar(builder, '\n');
                    json_prettify_indent(builder, depth + 1);

                    JsonNode * node = &object.children[index];

                    json_prettifyInto(node, builder, depth + 1);
                }

                strbuilder_appendChar(builder, '\n');
                json_prettify_indent(builder, depth);
                strbuilder_appendChar(builder, ']');
            } else {
                strbuilder_appendCString(builder, "[]");
            }

            break;
        }

        default: {
            // TODO: Error reporting

            String string = str_format("Unknown type %s", JsonTypeNames[value->flags.type]);

            strbuilder_appendString(builder, string);

            str_destroy(&string);
            break;
        }
    }
}