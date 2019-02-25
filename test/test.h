#ifndef __CLIB_test_h
#define __CLIB_test_h

#include "../src/datatypes.h"

#include <printf.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>



//
// Options
//

#define DEBUG false



//
// Print Constants
//

#define ERROR    RED BOLD   "Error"   RESET
#define FAILURE  RED BOLD   "Failure" RESET
#define SUCCESS  GREEN BOLD "Success" RESET
#define LINE     "-------------------------------------"



//
// Asserts
//

/*!
 * Print the location of where this macro is used.
 */
#define printErrorLocation() \
    printf(ERROR " : " YELLOW "line %d" RESET " in " YELLOW __FILE__ RESET "\n", __LINE__)

/*!
 * Print an error to console.
 */
#define error(message, ...)                        \
    do {                                           \
        printErrorLocation();                      \
        printf(" " message "\n", ##__VA_ARGS__);   \
    } while(0)

/*!
 * Assert {condition} is true, returning false from the current function if it is false.
 */
#define assert(condition)        \
    do {if(!(condition)) {       \
         printErrorLocation();   \
         return false;           \
    }} while(0)

/*!
 * Assert {condition} is true, returning false from the current function and printing an error if it is false.
 */
#define assertOrError(condition, message, ...)   \
    do {if(!(condition)) {                       \
         error(message, ##__VA_ARGS__);          \
         return false;                           \
    }} while(0)

/*!
 * Assert {pointer} is not NULL.
 */
#define assertNonNull(pointer) assertOrError((pointer) != NULL, #pointer " is unexpectedly null")

/*!
 * Assert that {errorType} is ERROR_SUCCESS.
 */
#define assertSuccess(errorType) assert((errorType) == ERROR_SUCCESS)


//
// Test Structure
//

/*
 * A test function that returns a boolean with true indicating success and false indicating failure.
 */
typedef bool (*TestFunction)();

/*
 * Run {testFunction}, incrementing the value of {failures} or {successes} based on the outcome.
 */
void runTest(char * name, TestFunction testFunction, int * failures, int * successes);

/*
 * Shorthand to test a function.
 *
 * Requires the name of the test function to be {functionName} prefixed by "test_".
 * E.g. the test function for "str_create" should be named "test_str_create".
 *
 * Requires failures and successes to both be variables of type int *.
 *
 */
#define test(functionName) runTest(#functionName, &test_##functionName, failures, successes)

#endif
