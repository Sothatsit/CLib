#include "test.h"
#include "testNumbers.h"
#include "testString.h"
#include "testStringBuilder.h"
#include "testExamples.h"

void test_all(int * failures, int * successes) {
    test_numbers(failures, successes);
    test_String(failures, successes);
    test_StringBuilder(failures, successes);
    test_examples(failures, successes);
}

int main(int argc, char *argv[]) {
    int failures = 0;
    int successes = 0;

    printf(BLUE BOLD "Running tests...\n\n" RESET);

    test_all(&failures, &successes);

    if(failures == 0) {
        printf(GREEN BOLD LINE "\n               Success\n" LINE "\n" RESET);
    } else {
        printf(RED BOLD LINE "\n               Failure\n" LINE "\n" RESET);
    }

    if(successes > 0) {
        printf(GREEN "%d Tests Successful\n" RESET, successes);
    }

    if(failures > 0) {
        printf(RED "%d Tests Failed\n" RESET, failures);
    }

    printf("\n");
}

void runTest(char * name, TestFunction testFunction, int * failures, int * successes) {
    clock_t begin = clock();

    bool success = testFunction();

    if(success) {
        ++*successes;
    } else {
        double duration = (double) (clock() - begin) / (CLOCKS_PER_SEC / 1e3);

        printf(FAILURE " : " MAGENTA "%s" RESET " (" YELLOW "%0.3f ms" RESET ")\n\n",
               name, duration);

        ++*failures;
    }
}