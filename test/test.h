#ifndef __CLIB_test_h
#define __CLIB_test_h

#include <printf.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>



//
// Print Constants
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

#define ERROR    RED BOLD   "Error"   RESET
#define FAILURE  RED BOLD   "Failure" RESET
#define SUCCESS  GREEN BOLD "Success" RESET
#define LINE     "-------------------------------------"



//
// Asserts
//

/*
 * Assert {condition} is true, returning false from the current function if it is false.
 */
#define assert(condition)                                                                           \
    do {if(!(condition)) {                                                                          \
         printf(ERROR " : " YELLOW "line %d" RESET " in " YELLOW __FILE__ RESET "\n", __LINE__);    \
         return false;                                                                              \
    }} while(0)

/*
 * Assert {condition} is true, returning false from the current function and printing an error if it is false.
 */
#define assertOrError(condition, error, ...)                                                              \
    do {if(!(condition)) {                                                                                \
         printf(ERROR " : " YELLOW "line %d" RESET " in " YELLOW __FILE__ RESET " : \n  " #error "\n",    \
                  __LINE__, ## __VA_ARGS__);                                                              \
         return false;                                                                                    \
    }} while(0)

/*
 * Assert {pointer} is not NULL.
 */
#define assertNonNull(pointer) assertOrError(pointer != NULL, #pointer " is unexpectedly null")

/*
 * Assert {string} is not empty and contains data.
 */
#define assertStrValid(string)                                                    \
    do {                                                                          \
        assertOrError(!str_isEmpty(string), #string " is unexpectedly empty");    \
        assertNonNull(string.data);                                               \
    } while(0)



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