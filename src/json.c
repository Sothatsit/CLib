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
struct JsonParseContext {
    /*
     * The original String being parsed.
     */
    String input;

    /*
     * The remaining part of the Json string being parsed.
     */
    String remaining;

    /*
     * Stores JsonNodes of objects currently being created.
     */
    Stack nodeStack;

    /*
     * Stores finalised JsonNodes.
     */
    Stack dataStack;

    /*
     * The result of parsing.
     */
    JsonParseResult result;
};

/*
 * Define functions used to manipulate the Stacks used to construct a JsonObject.
 */
define_stack_typedFunctions(JsonNode);

/*
 * Create an empty JsonParseResult.
 */
static JsonParseResult json_parseResult_createEmpty();

/*
 * Initialise the JsonParseContext {context} so that it can be used to parse {input}.
 *
 * Returns whether it was successful.
 */
static bool json_parseContext_init(JsonParseContext * context, String input);

/*
 * Complete the JsonParseContext {context} and clear it so it can be used again.
 *
 * Returns the result of the parsing from {context}.
 */
static JsonParseResult json_parseContext_complete(JsonParseContext * context);

/*
 * Destroy the temporary stacks used by {context} maintaining the stacks used for the result.
 */
static void json_parseContext_partialDestroy(JsonParseContext * context);

/*
 * Report an error that occured while parsing from {context}.
 *
 * Will always return false.
 */
static bool json_parseContext_error(JsonParseContext * context, char * message);

/*
 * Append the JsonNode {node} to the current object being constructed in {context}.
 */
static bool json_parseContext_appendNode(JsonParseContext * context, JsonNode node);

/*
 * Complete creation of a JsonParseObject and append its JsonNode value to its parent object.
 */
static bool json_parseContext_completeObject(JsonParseContext * context, JsonNode node);

/*
 * Remove all the leading whitespace from the remaining string in {context}.
 *
 * Returns whether there are remaining characters in {context}.
 */
static bool json_parseContext_trim(JsonParseContext * context);

/*
 * Get the next character in {context}.
 */
static char json_parseContext_next(JsonParseContext * context);

/*
 * Chew {characters} characters from the remaining string in {context}.
 */
static void json_parseContext_chew(JsonParseContext * context, u64 characters);

/*
 * Parse a json value.
 */
static bool json_parseContext_parseValue(JsonParseContext * context, JsonNode node);

/*
 * Parse a constant value.
 */
static bool json_parseContext_parseExpected(JsonParseContext * context, JsonNode node, String expected);

/*
 * Parse a String into {value}.
 */
static bool json_parseContext_parseString(JsonParseContext * context, String * value);

/*
 * Parse a number.
 */
static bool json_parseContext_parseNumber(JsonParseContext * context, JsonNode node);

/*
 * Parse an object.
 */
static bool json_parseContext_parseObject(JsonParseContext * context, JsonNode node);

/*
 * Parse an array.
 */
static bool json_parseContext_parseArray(JsonParseContext * context, JsonNode node);



//
// Parsing Implementation
//

static JsonParseResult json_parseResult_createEmpty() {
    return (JsonParseResult) {
            .input = str_createEmpty(),
            .root.nodes = NULL,
            .root.size = 0,
            .isError = false,
            .error = str_createEmpty(),
            .errorIndex = 0
    };
}

JsonParseContext json_parseContext_create() {
    JsonParseContext context;

    context.input = str_createEmpty();
    context.remaining = str_createEmpty();
    context.nodeStack = stack_createOfJsonNode(8);
    context.dataStack = stack_createOfJsonNode(8);
    context.result = json_parseResult_createEmpty();

    if(!stack_isValid(context.nodeStack)) {
        json_parseContext_error(&context, "Unable to create nodeStack");
        return context;
    }

    if(!stack_isValid(context.dataStack)) {
        json_parseContext_error(&context, "Unable to create dataStack");
        return context;
    }

    if(stack_reserve(&context.dataStack, sizeof(JsonNode)) == NULL) {
        json_parseContext_error(&context, "Unable to reserve space for root JsonNode on dataStack");
        return context;
    }

    return context;
}

static bool json_parseContext_init(JsonParseContext * context, String input) {
    if(context->result.isError)
        return false;

    context->input = input;
    context->remaining = input;
    context->nodeStack.used = 0;
    context->dataStack.used = 0;
    context->result = json_parseResult_createEmpty();
    context->result.input = input;

    if(stack_reserve(&context->dataStack, sizeof(JsonNode)) == NULL)
        return json_parseContext_error(context, "Unable to reserve space for root JsonNode on dataStack");

    return true;
}

static JsonParseResult json_parseContext_complete(JsonParseContext * context) {
    if(json_parseContext_trim(context)) {
        json_parseContext_error(context, "Remaining characters after completion of parsing");
        goto returnAndClearResult;
    }

    JsonNode * node = stack_peek(JsonNode, context->nodeStack);

    if(node == NULL) {
        json_parseContext_error(context, "Expected root node to be left on nodeStack");
        goto returnAndClearResult;
    }

    context->result.root.nodes = (JsonNode *) context->dataStack.buffer.start;
    *context->result.root.nodes = *node;
    context->result.root.size = (u32) (context->dataStack.used / sizeof(JsonNode));

    context->remaining = str_createEmpty();

    returnAndClearResult: {
        JsonParseResult result = context->result;
        context->result = json_parseResult_createEmpty();
        return result;
    };
}

static void json_parseContext_partialDestroy(JsonParseContext * context) {
    context->input = str_createEmpty();
    context->remaining = str_createEmpty();
    context->result = json_parseResult_createEmpty();
    context->result.isError = true;
    context->result.error = str_create("This JsonParseContext has been partially destroyed");
    context->result.errorIndex = 0;

    stack_destroy(&context->nodeStack);
}

void json_parseContext_destroy(JsonParseContext * context) {
    context->input = str_createEmpty();
    context->remaining = str_createEmpty();
    context->result = json_parseResult_createEmpty();
    context->result.isError = true;
    context->result.error = str_create("This JsonParseContext has been destroyed");
    context->result.errorIndex = 0;

    stack_destroy(&context->nodeStack);
    stack_destroy(&context->dataStack);
}

static bool json_parseContext_error(JsonParseContext * context, char * message) {
    String error = str_create(message);

    if(context->result.isError) {
        StringBuilder builder = strbuilder_create(context->result.error.length + 2 + error.length);

        strbuilder_appendString(&builder, context->result.error);
        strbuilder_appendCString(&builder, ", ");
        strbuilder_appendString(&builder, error);

        str_destroy(&context->result.error);

        context->result.error = builder.string;
    } else {
        context->result.root.nodes = NULL;
        context->result.root.size = 0;
        context->result.isError = true;
        context->result.error = str_copy(error);

        if(str_isEmpty(context->remaining)) {
            context->result.errorIndex = context->input.length;
        } else {
            context->result.errorIndex = context->remaining.data - context->input.data;
        }
    }

    return false;
}

void json_parseResult_printError(JsonParseResult result) {
    const u32 charsAround = 10;

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

static bool json_parseContext_appendNode(JsonParseContext *context, JsonNode node) {
    JsonNode * appended = stack_appendJsonNode(&context->nodeStack, node);

    if(appended == NULL)
        return json_parseContext_error(context, "Failure appending value node");

    return true;
}

static bool json_parseContext_completeObject(JsonParseContext * context, JsonNode node) {
    JsonNode * stackNodes = stack_popManyJsonNode(&context->nodeStack, node.value.object.size);

    if(stackNodes == NULL)
        return json_parseContext_error(context, "JsonParseObject's size greater than size of node stack");

    node.value.object.childIndex = (u32) (context->dataStack.used / sizeof(JsonNode));

    JsonNode * nodes = stack_appendManyJsonNode(&context->dataStack, stackNodes, node.value.object.size);

    if(nodes == NULL)
        return json_parseContext_error(context, "Unable to append JsonNodes to data stack");

    return json_parseContext_appendNode(context, node);
}

static bool json_parseContext_trim(JsonParseContext *context) {
    context->remaining = str_trimLeading(context->remaining);

    return context->remaining.length > 0;
}

static char json_parseContext_next(JsonParseContext *context) {
    return context->remaining.data[0];
}

static void json_parseContext_chew(JsonParseContext *context, u64 characters) {
    context->remaining = str_substring(context->remaining, characters, context->remaining.length);
}

JsonParseResult json_parseContext_parse(JsonParseContext * context, String input) {
    if(!json_parseContext_init(context, input))
        return context->result;

    if(!json_parseContext_trim(context)) {
        json_parseContext_error(context, "Empty input");
        return context->result;
    }

    JsonNode node = {
            .parentType = JSON_NONE,
            .key.index = 0
    };

    if(!json_parseContext_parseValue(context, node))
        return context->result;

    JsonParseResult result = json_parseContext_complete(context);

    return result;
}

JsonParseResult json_parse(String input) {
    JsonParseContext context = json_parseContext_create();

    JsonParseResult result = json_parseContext_parse(&context, input);

    if(result.isError) {
        json_parseContext_destroy(&context);
    } else {
        json_parseContext_partialDestroy(&context);
    }

    return result;
}

JsonParseResult json_parseResult_copy(JsonParseResult result) {
    if(result.isError)
        return result;

    Buffer copiedData = buffer_copy((Buffer) {
            .start = (char *) result.root.nodes,
            .capacity = result.root.size * sizeof(JsonNode)
    });

    if(buffer_isEmpty(copiedData)) {
        return (JsonParseResult) {
                .input = result.input,
                .root.nodes = NULL,
                .root.size = 0,
                .isError = true,
                .error = str_create("Unable to allocate Buffer to copy result into"),
                .errorIndex = 0
        };
    }

    return (JsonParseResult) {
            .input = result.input,
            .root.nodes = (JsonNode *) copiedData.start,
            .root.size = result.root.size,
            .isError = false,
            .error = str_createEmpty(),
            .errorIndex = 0
    };
}

JsonNode json_node_createEmpty() {
    return (JsonNode) {
        .type = JSON_NONE,
        .parentType = JSON_NONE,
        .key.index = 0
    };
}

bool json_node_isEmpty(JsonNode node) {
    return node.type == JSON_NONE;
}

JsonNode json_root_get(JsonRoot root, u32 index) {
    if(index >= root.size)
        return json_node_createEmpty();

    return root.nodes[index];
}

JsonNode json_obj_get(JsonRoot root, JsonNode node, u32 index) {
    if(index >= node.value.object.size)
        return json_node_createEmpty();

    return json_root_get(root, node.value.object.childIndex + index);
}

JsonNode json_obj_find(JsonRoot root, JsonNode node, String key) {
    s32 index = json_obj_findIndex(root, node, key);

    if(index == -1)
        return json_node_createEmpty();

    return json_root_get(root, node.value.object.childIndex + index);
}

s32 json_obj_findIndex(JsonRoot root, JsonNode node, String key) {
    if(node.type != JSON_OBJECT)
        return -1;

    for(u32 index = 0; index < node.value.object.size; ++index) {
        JsonNode child = json_obj_get(root, node, index);

        if(str_equals(child.key.name, key))
            return index;
    }

    return -1;
}

void json_parseResult_destroy(JsonParseResult * result) {
    if(result->isError || result->root.nodes == NULL)
        return;

    free(result->root.nodes);

    result->root.nodes = NULL;
    result->root.size = 0;
    result->isError = true;
    result->error = str_create("This JsonParseResult has been destroyed");
    result->errorIndex = 0;
}

bool json_parseContext_parseValue(JsonParseContext *context, JsonNode node) {
    char next = json_parseContext_next(context);

    switch(next) {
        case '{':
            json_parseContext_chew(context, 1);
            return json_parseContext_parseObject(context, node);
        case '[':
            json_parseContext_chew(context, 1);
            return json_parseContext_parseArray(context, node);

        case 't':
            node.type = JSON_TRUE;
            return json_parseContext_parseExpected(context, node, str_createOfLength("true", 4));
        case 'f':
            node.type = JSON_FALSE;
            return json_parseContext_parseExpected(context, node, str_createOfLength("false", 5));
        case 'n':
            node.type = JSON_NULL;
            return json_parseContext_parseExpected(context, node, str_createOfLength("null", 4));

        case '"': {
            json_parseContext_chew(context, 1);

            node.type = JSON_STRING;

            return json_parseContext_parseString(context, &node.value.string)
                   && json_parseContext_appendNode(context, node);
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
            return json_parseContext_parseNumber(context, node);

        default:
            return json_parseContext_error(context, "Unexpected character while expecting value");
    }
}

bool json_parseContext_parseExpected(JsonParseContext * context, JsonNode node, String expected) {
    if(!str_startsWith(context->remaining, expected)) {
        String message = str_createUninitialised(9 + expected.length);

        str_setChars(message, 0, str_create("Expected "));
        str_setChars(message, 9, expected);

        json_parseContext_error(context, str_toCString(message));

        str_destroy(&message);

        return false;
    }

    json_parseContext_chew(context, expected.length);

    return json_parseContext_appendNode(context, node);
}

#define __json_parseString_parseHex4(value)                                          \
    do {                                                                             \
        u8 c;                                                                        \
                                                                                     \
        for(u64 i = 0; i < 4; ++i) {                                                 \
            c = (u8) context->remaining.data[index++];                               \
                                                                                     \
            if(c >= '0' && c <= '9') {                                               \
                c -= '0';                                                            \
            } else if(c >= 'a' && c <= 'f') {                                        \
                c -= 'a' - 10;                                                       \
            } else if(c >= 'A' && c <= 'F') {                                        \
                c -= 'A' - 10;                                                       \
            } else {                                                                 \
                json_parseContext_chew(context, index - 1);                          \
                return json_parseContext_error(context, "Expected hex character");   \
            }                                                                        \
                                                                                     \
            value <<= 4;                                                             \
            value |= c;                                                              \
        }                                                                            \
    } while(0)

#define __json_parseString_expect(character)                                                            \
    do {                                                                                                \
        if(index >= context->remaining.length) {                                                        \
            json_parseContext_chew(context, index);                                                     \
            return json_parseContext_error(context, "Expected second half of UTF-16 surrogate pair");   \
        }                                                                                               \
        if(context->remaining.data[index++] != character) {                                             \
            json_parseContext_chew(context, index - 1);                                                 \
            return json_parseContext_error(context, "Expected second half of UTF-16 surrogate pair");   \
        }                                                                                               \
    } while (0)

bool json_parseContext_parseString(JsonParseContext * context, String * value) {
    value->length = 0;
    value->data = context->remaining.data;

    u64 copyFromIndex = 0;
    u64 index = 0;

    while(index < context->remaining.length) {
        char next = context->remaining.data[index++];

        if(char_isControlCharacter(next)) {
            json_parseContext_chew(context, index - 1);
            return json_parseContext_error(context, "Control character in string");
        }

        if(next == '"') {
            u64 copyBack = index - copyFromIndex - 1;
            memmove(&value->data[value->length], &context->remaining.data[copyFromIndex], copyBack);
            value->length += copyBack;

            json_parseContext_chew(context, index);
            return true;
        }

        if(next == '\\') {
            u64 copyBack = index - copyFromIndex - 1;
            memmove(&value->data[value->length], &context->remaining.data[copyFromIndex], copyBack);
            value->length += copyBack;

            if(index >= context->remaining.length) {
                json_parseContext_chew(context, index);
                return json_parseContext_error(context, "Unexpected eof while parsing string");
            }

            next = context->remaining.data[index++];

            if(next == 'u') {
                if(index + 3 >= context->remaining.length) {
                    json_parseContext_chew(context, context->remaining.length);
                    return json_parseContext_error(context, "Unexpected eof while parsing UCS codepoint");
                }

                u32 codepoint = 0; {
                    __json_parseString_parseHex4(codepoint);
                }

                // UTF-16 Surrogate pair
                if(codepoint >= 0xD800 && codepoint <= 0xDFFF) {
                    if(codepoint >= 0xDC00) {
                        json_parseContext_chew(context, index - 5);
                        return json_parseContext_error(context, "Unexpected second half of UTF-16 surrogate pair");
                    }

                    u16 lo = (u16) codepoint;
                    u16 hi = 0; {
                        __json_parseString_expect('\\');
                        __json_parseString_expect('u');
                        __json_parseString_parseHex4(hi);
                    }

                    if(hi <= 0xDBFF) {
                        json_parseContext_chew(context, index - 5);
                        return json_parseContext_error(context, "Expected second half of UTF-16 surrogate pair");
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
                    json_parseContext_chew(context, index - 1);
                    return json_parseContext_error(context, "Invalid escape sequence");
            }

            value->data[value->length] = escapedValue;
            value->length += 1;

            copyFromIndex = index;
        }
    }

    return json_parseContext_error(context, "Unexpected eof while parsing string");
}

#undef __json_parseString_parseHex4

#define __jsonParseNumber_consumeOne()            \
    do {                                          \
        json_parseContext_chew(context, 1);       \
        node.value.string.length += 1;            \
        if(context->remaining.length == 0) {      \
            goto finishNumber;                    \
        }                                         \
        next = json_parseContext_next(context);   \
    } while(0)

#define __jsonParseNumber_consumeOneRequireNext()                                      \
    do {                                                                               \
        json_parseContext_chew(context, 1);                                            \
        node.value.string.length += 1;                                                 \
        if(context->remaining.length == 0) {                                           \
            json_parseContext_error(context, "Unexpected eof while parsing number");   \
            return false;                                                              \
        }                                                                              \
        next = json_parseContext_next(context);                                        \
    } while(0)

bool json_parseContext_parseNumber(JsonParseContext * context, JsonNode node) {
    node.type = JSON_NUMBER;
    node.value.string = (String) {
        .data = context->remaining.data,
        .length = 0
    };

    char next = json_parseContext_next(context);

    if(next == '-') {
        __jsonParseNumber_consumeOneRequireNext();
    }

    if(next == '0') {
        __jsonParseNumber_consumeOne();

        if(char_isDigit(next)) {
            json_parseContext_error(context, "Unexpected digit after 0 in number");
            return false;
        }
    } else if(char_isDigit(next)) {
        do {
            __jsonParseNumber_consumeOne();
        } while(char_isDigit(next));
    } else {
        return json_parseContext_error(context, "Expected digit");
    }

    if(next == '.') {
        __jsonParseNumber_consumeOneRequireNext();

        if(!char_isDigit(next)) {
            json_parseContext_error(context, "Expected digit after .");
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
            return json_parseContext_error(context, "Expected digit in exponent of number");
        }
    }

    finishNumber: {
        return json_parseContext_appendNode(context, node);
    }
}

#undef __jsonParseNumber_consumeOne
#undef __jsonParseNumber_consumeOneRequireNext

bool json_parseContext_parseObject(JsonParseContext * context, JsonNode node) {
    node.type = JSON_OBJECT;
    node.value.object.size = 0;

    JsonNode childNode = {
            .parentType = JSON_OBJECT
    };

    while(true) {
        if(!json_parseContext_trim(context))
            return json_parseContext_error(context, "Unexpected eof while parsing object");

        char next = json_parseContext_next(context);

        if(next == '}') {
            json_parseContext_chew(context, 1);
            return json_parseContext_completeObject(context, node);
        }

        if(node.value.object.size != 0) {
            if(next != ',')
                return json_parseContext_error(context, "Expected , or } while parsing object");

            json_parseContext_chew(context, 1);

            if(!json_parseContext_trim(context))
                return json_parseContext_error(context, "Unexpected eof while parsing object");

            next = json_parseContext_next(context);
        }

        if(next != '"')
            return json_parseContext_error(context, "Expected \" for start of key while parsing object");

        json_parseContext_chew(context, 1);
        json_parseContext_parseString(context, &childNode.key.name);

        if(str_isEmpty(childNode.key.name) && context->result.isError)
            return false;

        if(!json_parseContext_trim(context))
            return json_parseContext_error(context, "Unexpected eof while parsing object");

        next = json_parseContext_next(context);

        if(next != ':')
            return json_parseContext_error(context, "Expected : while parsing object");

        json_parseContext_chew(context, 1);

        if(!json_parseContext_trim(context))
            return json_parseContext_error(context, "Unexpected eof while parsing object");

        if(!json_parseContext_parseValue(context, childNode))
            return false;

        node.value.object.size += 1;
    };

    return false;
}

static bool json_parseContext_parseArray(JsonParseContext * context, JsonNode node) {
    node.type = JSON_ARRAY;
    node.value.object.size = 0;

    JsonNode childNode = {
            .parentType = JSON_ARRAY
    };

    while(true) {
        if(!json_parseContext_trim(context))
            return json_parseContext_error(context, "Unexpected eof while parsing array");

        char next = json_parseContext_next(context);

        if(next == ']') {
            json_parseContext_chew(context, 1);
            return json_parseContext_completeObject(context, node);
        }

        if(node.value.object.size != 0) {
            if(next != ',')
                return json_parseContext_error(context, "Expected , or ] while parsing array");

            json_parseContext_chew(context, 1);

            if(!json_parseContext_trim(context))
                return json_parseContext_error(context, "Unexpected eof while parsing array");
        }

        childNode.key.index = node.value.object.size;

        if(!json_parseContext_parseValue(context, childNode))
            return false;

        node.value.object.size += 1;
    };

    return false;
}



//
// Stringify
//

/*!
 * Append whitespace to depth {depth} in {builder}.
 */
static void json_prettify_indent(StringBuilder * builder, u64 depth);



//
// Stringify Implementation
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

String json_stringify(JsonRoot root, JsonNode node) {
    StringBuilder stringBuilder = strbuilder_create(32);

    json_stringifyInto(root, node, &stringBuilder);

    return stringBuilder.string;
}

void json_stringifyInto(JsonRoot root, JsonNode node, StringBuilder * builder) {
    switch(node.type) {
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
            strbuilder_appendString(builder, node.value.string);
            break;
        case JSON_STRING:
            strbuilder_appendChar(builder, '"');
            json_quoteStringInto(node.value.string, builder);
            strbuilder_appendChar(builder, '"');
            break;

        case JSON_OBJECT: {
            strbuilder_appendChar(builder, '{');

            for(u32 index = 0; index < node.value.object.size; ++index) {
                if(index != 0) {
                    strbuilder_appendChar(builder, ',');
                }

                JsonNode child = json_obj_get(root, node, index);

                strbuilder_appendChar(builder, '"');
                json_quoteStringInto(child.key.name, builder);
                strbuilder_appendCString(builder, "\":");

                json_stringifyInto(root, child, builder);
            }

            strbuilder_appendChar(builder, '}');

            break;
        }

        case JSON_ARRAY: {
            strbuilder_appendChar(builder, '[');

            for(u32 index = 0; index < node.value.object.size; ++index) {
                if(index != 0) {
                    strbuilder_appendChar(builder, ',');
                }

                JsonNode child = json_obj_get(root, node, index);

                json_stringifyInto(root, child, builder);
            }

            strbuilder_appendChar(builder, ']');

            break;
        }

        default: {
            // TODO: Error reporting

            String string = str_format("Unknown type %d", node.type);

            strbuilder_appendString(builder, string);

            str_destroy(&string);
            break;
        }
    }
}