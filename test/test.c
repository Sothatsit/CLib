#include "test.h"
#include "testNumbers.h"
#include "testSorting.h"
#include "testString.h"
#include "testStringBuilder.h"
#include "testUTF.h"
#include "testBuffer.h"
#include "testErrors.h"
#include "testExamples.h"

void test_all(int * failures, int * successes) {
    test_numbers(failures, successes);
    test_sorting(failures, successes);
    test_String(failures, successes);
    test_StringBuilder(failures, successes);
    test_UTF(failures, successes);
    test_Buffer(failures, successes);
    test_errors(failures, successes);
    test_examples(failures, successes);
}

int main(int argc, char *argv[]) {
    int failures = 0;
    int successes = 0;

    printf(BLUE BOLD "Running tests..." RESET "\n\n");

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
    if(DEBUG) {
        printf(GREEN BOLD "Running" RESET " : " MAGENTA "%s" RESET "\n", name);
    }

    clock_t begin = clock();

    bool success = testFunction();

    clock_t end = clock();

    if(success) {
        ++(*successes);
    } else {
        double duration = (double) (end - begin) / (CLOCKS_PER_SEC / 1e3);

        printf(FAILURE " : " MAGENTA "%s" RESET " (" YELLOW "%0.3f ms" RESET ")\n\n",
               name, duration);

        ++(*failures);
    }
}
