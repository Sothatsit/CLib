#include "test.h"
#include "testStack.h"

//
// Tests
//

#define assertWritable(stack)                                          \
    do{                                                                \
        assert(stack_isValid(stack));                                  \
        for(u64 index = 0; index < stack.buffer.capacity; ++index) {   \
            stack.buffer.start[index] = (u8) index;                    \
        }                                                              \
    } while(0)

/*
 * A dummy type used for testing the Stack typed functions.
 */
typedef struct TestStruct {
    u64 number;
    String string;
} TestStruct;

stack_defineTypedFunctions(TestStruct);

bool test_stack_create() {
    Stack stack = stack_create(32);
    {
        assertNonNull(stack.buffer.start);
        assert(stack.buffer.capacity == 32);
        assert(stack.used == 0);
        assertWritable(stack);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_isValid() {
    Stack stack = stack_create(32);
    Stack empty = stack_create(0);
    {
        assert(stack_isValid(stack));
        assert(!stack_isValid(empty));
    }
    stack_destroy(&stack);
    stack_destroy(&empty);

    return true;
}

bool test_stack_destroy() {
    Stack stack = stack_create(32);
    {
        assertWritable(stack);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_setCapacity() {
    Stack stack = stack_create(32);
    {
        assertWritable(stack);

        assert(stack_setCapacity(&stack, 16));
        assert(stack.buffer.capacity == 16);
        assertWritable(stack);

        assert(stack_setCapacity(&stack, 80));
        assert(stack.buffer.capacity == 80);
        assertWritable(stack);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_ensureCapacity() {
    Stack stack = stack_create(32);
    {
        assertWritable(stack);

        assert(stack_ensureCapacity(&stack, 16));
        assert(stack.buffer.capacity == 32);
        assertWritable(stack);

        assert(stack_ensureCapacity(&stack, 80));
        assert(stack.buffer.capacity == 128);
        assertWritable(stack);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_trimToUsed() {
    Stack stack = stack_create(32);
    {
        assertWritable(stack);

        assert(stack_trimToUsed(&stack));
        assert(stack.buffer.capacity == 0);
        assert(stack.used == 0);

        assert(stack_setCapacity(&stack, 32));
        stack.used = 32;
        assert(stack_trimToUsed(&stack));
        assert(stack.buffer.capacity == 32);
        assert(stack.used == 32);

        assert(stack_setCapacity(&stack, 64));
        assert(stack_trimToUsed(&stack));
        assert(stack.buffer.capacity == 32);
        assert(stack.used == 32);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_getEnd() {
    Stack stack = stack_create(32);
    {
        assertWritable(stack);

        assert(stack_getEnd(stack) == stack.buffer.start);

        stack.used = 5;
        assert(stack_getEnd(stack) == &stack.buffer.start[5]);

        stack.used = 27;
        assert(stack_getEnd(stack) == &stack.buffer.start[27]);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_peek() {
    Stack stack = stack_create(64);
    {
        assertWritable(stack);

        assert(stack_peek(char, stack) == NULL);

        stack.used = 5;
        assert(stack_peek(char, stack) == &stack.buffer.start[4]);

        stack.used = 32;
        assert(stack_peek(char, stack) == &stack.buffer.start[31]);

        assert(stack_appendTestStruct(&stack, (TestStruct) {
            .number = 639,
            .string = str_create("TestStruct")
        }));
        assert(stack_peek(TestStruct, stack) == (TestStruct *) (&stack.buffer.start[32]));
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_reserve() {
    Stack stack = stack_create(16);
    {
        assertWritable(stack);

        assert(stack_reserve(&stack, 15) == stack.buffer.start);
        assert(stack.used == 15);

        assert(stack_reserve(&stack, 17) == &stack.buffer.start[15]);
        assert(stack.used == 32);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_appendData() {
    Stack stack = stack_create(16);
    {
        assertWritable(stack);

#define theData "Let's just write random crap, it's a test, this data is whack."
        u64 dataLength = 62;

        char * data = theData;
        char * twoData = theData theData;

        assert(stack_appendData(&stack, data, dataLength));
        assert(memcmp(data, stack.buffer.start, dataLength) == 0);

        assert(stack_appendData(&stack, data, dataLength));
        assert(memcmp(twoData, stack.buffer.start, 2 * dataLength) == 0);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_typedFunctions() {
    Stack stack = stack_createOfTestStruct(3);
    {
        assertWritable(stack);

        TestStruct data[] = {
            (TestStruct) {
                .number = 460349,
                .string = str_create("First!")
            },
            (TestStruct) {
                .number = 9546724,
                .string = str_create("Numero Duece!")
            },
            (TestStruct) {
                .number = 123,
                .string = str_create("Tree")
            },
            (TestStruct) {
                .number = 5346214,
                .string = str_create("Forewarned")
            }
        };

        assert(stack_appendManyTestStruct(&stack, data, 4));
        assert(stack.used == 4  * sizeof(TestStruct));

        assert(memcmp(data, stack.buffer.start, 4 * sizeof(TestStruct)) == 0);

        assert(memcmp(&data[2], stack_popManyTestStruct(&stack, 2), 2 * sizeof(TestStruct)) == 0);
        assert(stack.used == 2  * sizeof(TestStruct));

        assert(memcmp(&data[1], stack_popTestStruct(&stack), sizeof(TestStruct)) == 0);
        assert(stack.used == sizeof(TestStruct));

        TestStruct test = (TestStruct) {
            .number = 92374927585718457,
            .string = str_create("Woopdee Doop. I am a crook")
        };

        assert(stack_appendTestStruct(&stack, test));
        assert(stack.used == 2 * sizeof(TestStruct));

        assert(memcmp(&test, stack_popTestStruct(&stack), sizeof(TestStruct)) == 0);
        assert(stack.used == sizeof(TestStruct));

        assert(memcmp(&data[0], stack_popTestStruct(&stack), sizeof(TestStruct)) == 0);
        assert(stack.used == 0);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_typedCreate() {
    Stack stack = stack_createOfTestStruct(16);
    {
        assertNonNull(stack.buffer.start);
        assert(stack.buffer.capacity == 16 * sizeof(TestStruct));
        assert(stack.used == 0);
        assertWritable(stack);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_typedAppend() {
    Stack stack = stack_createOfTestStruct(1);
    {
        assertWritable(stack);

        TestStruct first = (TestStruct) {
            .number = 5,
            .string = str_create("Apples")
        };

        assert(stack_appendTestStruct(&stack, first));
        assert(stack.used == sizeof(TestStruct));
        assert(memcmp(&first, stack.buffer.start, sizeof(TestStruct)) == 0);

        TestStruct second = (TestStruct) {
            .number = 38572834,
            .string = str_create("test_stack_typedAppend")
        };

        assert(stack_appendTestStruct(&stack, second));
        assert(stack.used == 2 * sizeof(TestStruct));
        assert(memcmp(&second, stack_peek(TestStruct, stack), sizeof(TestStruct)) == 0);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_typedAppendMany() {
    Stack stack = stack_createOfTestStruct(3);
    {
        assertWritable(stack);

        TestStruct data[] = {
            (TestStruct) {
                .number = 460349,
                .string = str_create("First!")
            },
            (TestStruct) {
                .number = 9546724,
                .string = str_create("Numero Duece!")
            },
            (TestStruct) {
                .number = 123,
                .string = str_create("Tree")
            },
            (TestStruct) {
                .number = 5346214,
                .string = str_create("Forewarned")
            }
        };

        assert(stack_appendManyTestStruct(&stack, data, 4));
        assert(stack.used == 4  * sizeof(TestStruct));
        assert(memcmp(data, stack.buffer.start, 4 * sizeof(TestStruct)) == 0);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_typedPop() {
    Stack stack = stack_createOfTestStruct(1);
    {
        assertWritable(stack);

        TestStruct first = (TestStruct) {
            .number = 5,
            .string = str_create("Apples")
        };

        TestStruct second = (TestStruct) {
            .number = 38572834,
            .string = str_create("test_stack_typedAppend")
        };

        assert(stack_appendTestStruct(&stack, first));
        assert(stack_appendTestStruct(&stack, second));
        assert(stack.used == 2 * sizeof(TestStruct));
        assert(memcmp(&second, stack_popTestStruct(&stack), sizeof(TestStruct)) == 0);
        assert(stack.used == sizeof(TestStruct));
        assert(memcmp(&first, stack_popTestStruct(&stack), sizeof(TestStruct)) == 0);
        assert(stack.used == 0);
    }
    stack_destroy(&stack);

    return true;
}

bool test_stack_typedPopMany() {
    Stack stack = stack_createOfTestStruct(3);
    {
        assertWritable(stack);

        TestStruct data[] = {
            (TestStruct) {
                .number = 460349,
                .string = str_create("First!")
            },
            (TestStruct) {
                .number = 9546724,
                .string = str_create("Numero Duece!")
            },
            (TestStruct) {
                .number = 123,
                .string = str_create("Tree")
            },
            (TestStruct) {
                .number = 5346214,
                .string = str_create("Forewarned")
            }
        };

        assert(stack_appendManyTestStruct(&stack, data, 4));
        assert(stack.used == 4  * sizeof(TestStruct));
        assert(memcmp(data, stack_popManyTestStruct(&stack, 4), 4 * sizeof(TestStruct)) == 0);
        assert(stack.used == 0);
    }
    stack_destroy(&stack);

    return true;
}



//
// Run Tests
//

void test_Stack(int * failures, int * successes) {
    test(stack_create);
    test(stack_isValid);
    test(stack_destroy);
    test(stack_setCapacity);
    test(stack_ensureCapacity);
    test(stack_trimToUsed);
    test(stack_getEnd);
    test(stack_peek);
    test(stack_reserve);
    test(stack_appendData);
    test(stack_typedFunctions);
    test(stack_typedCreate);
    test(stack_typedAppend);
    test(stack_typedAppendMany);
    test(stack_typedPop);
    test(stack_typedPopMany);
}