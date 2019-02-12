#include "test.h"
#include "testSorting.h"

//
// Tests
//

/*!
 * Arguments
 *  1: array
 *  2: length
 *
 * Returns whether sorting was successful.
 */
typedef bool (*SortFn_u64)(u64 *, u64);

/*!
 * Test a u64 sorting method with a given test case.
 */
bool test_u64_sortCase(char * sortName, SortFn_u64 sortFn, u64 * source, u64 * expected, u64 length) {
    u64 * sorted = malloc(length * sizeof(u64)); {
        assertNonNull(sorted);
        memcpy(sorted, source, length * sizeof(u64));
    }

    assertOrError((*sortFn)(sorted, length), "Error performing %s", sortName);

    bool matchesExpected = (memcmp(sorted, expected, length * sizeof(u64)) == 0);

    if(matchesExpected)
        return true;

    char * sourceString = str_c(arr_u64_toString(source, length));
    char * sortedString = str_c(arr_u64_toString(sorted, length));
    char * expectedString = str_c(arr_u64_toString(expected, length));

    error("%s did not produce the expected output\n"
          BOLD "  Source:   " RESET "%s\n"
          BOLD "  Sorted:   " RESET "%s\n"
          BOLD "  Expected: " RESET "%s",
          sortName, sourceString, sortedString, expectedString);

    return false;
}

// Test case 0
u64 u64_sortLength0 = 0;
u64 u64_sortSource0[] = {};
u64 u64_sortExpected0[] = {};

// Test case 1
u64 u64_sortLength1 = 15;
u64 u64_sortSource1[]   = { 5, 3, 7, 4, 6, 2, 5, 7, 2, 4, 3, 5, 0, 9, 5 };
u64 u64_sortExpected1[] = { 0, 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 6, 7, 7, 9 };

// Test case 2
u64 u64_sortLength2 = 62;
u64 u64_sortSource2[] = {
    1,2,5,9,34,342,123,124,67,5,23,4,7,5689,5,8957,234,2,523,462,36,3457,456,859,
    65,243,3452,34,3457,345,62,345,234,5,563,4,52,345,236,676,48,567,34,1234,234,
    61,23,52356232344353,245,532,623,45,2345,234,523,5734,6425,1,1234,342,45,65
};
u64 u64_sortExpected2[] = {
    1,1,2,2,4,4,5,5,5,5,7,9,23,23,34,34,34,36,45,45,48,52,61,62,65,65,67,123,124,
    234,234,234,234,236,243,245,342,342,345,345,345,456,462,523,523,532,563,567,
    623,676,859,1234,1234,2345,3452,3457,3457,5689,5734,6425,8957,52356232344353
};

#define test_u64_sortCase(function, caseNumber)      \
    assert(test_u64_sortCase(#function, &function,   \
        u64_sortSource##caseNumber, u64_sortExpected##caseNumber, u64_sortLength##caseNumber))

#define test_u64_sort(function)       \
    test_u64_sortCase(function, 0);   \
    test_u64_sortCase(function, 1);   \
    test_u64_sortCase(function, 2);

// Required to make u64_insertionSort conform to SortFn_u64.
bool u64_insertionSort_boolReturn(u64 * source, u64 length) {
    u64_insertionSort(source, length);
    return true;
}

bool test_u64_insertionSort() {
    test_u64_sort(u64_insertionSort_boolReturn);
    return true;
}

// Required to make u64_quickSort conform to SortFn_u64.
bool u64_quickSort_boolReturn(u64 * source, u64 length) {
    u64_quickSort(source, length);
    return true;
}

bool test_u64_quickSort() {
    test_u64_sort(u64_quickSort_boolReturn);
    return true;
}

bool test_u64_mergeSort() {
    test_u64_sort(u64_mergeSort);
    return true;
}

// Required to make u64_mergeSort_withBuffer conform to SortFn_u64.
bool u64_mergeSort_withBuffer_curried(u64 * array, u64 length) {
    u64 buffer[length];
    u64_mergeSort_withBuffer(array, buffer, length);
    return true;
}

bool test_u64_mergeSort_withBuffer() {
    test_u64_sort(u64_mergeSort_withBuffer_curried);
    return true;
}



//
// Run Tests
//

void test_sorting(int * failures, int * successes) {
    test(u64_insertionSort);
    test(u64_quickSort);
    test(u64_mergeSort_withBuffer);
    test(u64_mergeSort);
}
