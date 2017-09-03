#include "array.h"

#include <stdlib.h>
#include <memory.h>

//
// Arrays
//

Array arr_create(void * data, u64 length, u64 elementSize) {
    Array array;

    array.data = data;
    array.length = length;
    array.elementSize = elementSize;

    return array;
}

Array arr_createCopy(void * data, u64 length, u64 elementSize) {
    return arr_copy(arr_create(data, length, elementSize));
}

Array arr_createUninitialised(u64 length, u64 elementSize) {
    if(length * elementSize == 0)
        return arr_createEmpty();

    void * data = malloc(length * elementSize);

    if(data == NULL)
        return arr_createEmpty();

    return arr_create(data, length, elementSize);
}

Array arr_copy(Array array) {
    if(arr_isEmpty(array))
        return array;

    Array copy;

    copy.data = malloc(array.length * array.elementSize);
    copy.length = array.length;
    copy.elementSize = array.elementSize;

    if(copy.data == NULL)
        return arr_createEmpty();

    memcpy(copy.data, array.data, array.length * array.elementSize);

    return copy;
}

void arr_destroy(Array array) {
    free(array.data);
}

bool arr_equals(Array array1, Array array2) {
    if(array1.length != array2.length)
        return false;

    if(array1.length == 0)
        return true;

    if(array1.elementSize != array2.elementSize)
        return false;

    return memcmp(array1.data, array2.data, array1.length * array1.elementSize) == 0;
}

Array arr_convertLeftPadded(Array array, u64 newElementSize) {
    if(array.elementSize == newElementSize)
        return array;

    Array newArray = arr_createUninitialised(array.length, newElementSize);

    if(arr_isEmpty(newArray))
        return newArray;

    if(newElementSize > array.elementSize) {
        u64 diff = newElementSize - array.elementSize;

        for(u64 index = 0; index < array.length; index++) {
            memset(arr_getPointer(newArray, index), 0, diff);
            memcpy(arr_getPointer(newArray, index) + diff, arr_getPointer(array, index), array.elementSize);
        }
    } else {
        u64 diff = array.elementSize - newElementSize;

        for(u64 index = 0; index < array.length; index++) {
            memcpy(arr_getPointer(newArray, index), arr_getPointer(array, index) + diff, newElementSize);
        }
    }

    return newArray;
}

Array arr_convertRightPadded(Array array, u64 newElementSize) {
    if(array.elementSize == newElementSize)
        return array;

    Array newArray = arr_createUninitialised(array.length, newElementSize);

    if(arr_isEmpty(newArray))
        return newArray;

    if(newElementSize > array.elementSize) {
        u64 diff = newElementSize - array.elementSize;

        for(u64 index = 0; index < array.length; index++) {
            memcpy(arr_getPointer(newArray, index), arr_getPointer(array, index), array.elementSize);
            memset(arr_getPointer(newArray, index) + diff, 0, diff);
        }
    } else {
        for(u64 index = 0; index < array.length; index++) {
            memcpy(arr_getPointer(newArray, index), arr_getPointer(array, index), newElementSize);
        }
    }

    return newArray;
}

bool arr_setElements(Array array, u64 index, Array replacements) {
    if(array.elementSize != replacements.elementSize)
        return false;

    if(index + replacements.length > array.length)
        return false;

    memcpy(arr_getPointer(array, index), replacements.data, replacements.length * array.elementSize);

    return true;
}

Array arr_subarray(Array array, u64 start, u64 end) {
    end = min(end, array.length);

    if(end <= start)
        return arr_createEmpty();

    Array subarray;

    subarray.data = arr_getPointer(array, start);
    subarray.length = end - start;

    return subarray;
}

Array arr_concat(Array array1, Array array2) {
    if(arr_isEmpty(array1))
        return array2;

    if(arr_isEmpty(array2))
        return array1;

    if(array1.elementSize != array2.elementSize)
        return arr_createEmpty();

    Array newArray = arr_createUninitialised(array1.length + array2.length, array1.elementSize);

    if(arr_isEmpty(newArray))
        return newArray;

    memcpy(newArray.data, array1.data, array1.length * array1.elementSize);
    memcpy(&newArray.data[array1.length * array1.elementSize], array2.data, array2.length * array2.elementSize);

    return newArray;
}



//
// Lists
//

List list_create(u64 initialSize, u64 elementSize) {
    if(initialSize == 0){
        initialSize = 8;
    }

    List list;

    list.array = arr_createUninitialised(initialSize, elementSize);

    if(arr_isEmpty(list.array)) {
        list.capacity = 0;

        return list;
    }

    list.array.length = 0;
    list.capacity = initialSize;

    return list;
}

void list_destroy(List list) {
    if(list.capacity == 0)
        return;

    arr_destroy(list.array);
}

bool list_setCapacity(List * list, u64 capacity) {
    if(list->capacity == 0)
        return false;

    if(list->array.length > capacity)
        return false;

    if(capacity == 0)
        return false;

    if(list->capacity == capacity)
        return true;

    list->array.data = realloc(list->array.data, capacity * list->array.elementSize);
    list->capacity = capacity;

    if(list->array.data == NULL) {
        list->capacity = 0;
        return false;
    }

    return true;
}

bool list_ensureCapacity(List * list, u64 requiredCapacity) {
    if(list->capacity == 0)
        return false;

    if(requiredCapacity <= list->capacity)
        return true;

    u64 newCapacity = list->capacity;

    while(newCapacity < requiredCapacity) {
        newCapacity *= 2;
    }

    return list_setCapacity(list, newCapacity);
}

bool list_trimToLength(List * list) {
    return list_setCapacity(list, list->array.length);
}

void * list_append(List * list) {
    if(!list_ensureCapacity(list, list->array.length + 1))
        return NULL;

    u64 endIndex = list->array.length;

    list->array.length += 1;

    return arr_getPointer(list->array, endIndex);
}

bool list_appendElement(List * list, void * element) {
    void * elementPointer = list_append(list);

    if(elementPointer == NULL)
        return false;

    memcpy(elementPointer, element, list->array.elementSize);

    return true;
}

void * list_insert(List * list, u64 index) {
    if(index > list->array.length)
        return NULL;

    if(!list_ensureCapacity(list, list->array.length + 1))
        return NULL;

    void * insert = arr_getPointer(list->array, index);
    void * elementAfter = arr_getPointer(list->array, index + 1);

    memmove(elementAfter, insert, list->array.length - index);

    list->array.length += 1;

    return insert;
}

bool list_insertValue(List * list, u64 index, void * element) {
    void * elementPointer = list_insert(list, index);

    if(elementPointer == NULL)
        return false;

    memcpy(elementPointer, element, list->array.elementSize);

    return true;
}

bool list_remove(List * list, u64 index) {
    if(index >= list->array.length)
        return false;

    list->array.length -= 1;

    if(index == list->array.length)
        return true;

    void * remove = arr_getPointer(list->array, index);
    void * last = arr_getPointer(list->array, list->array.length);

    memcpy(remove, last, list->array.elementSize);

    return true;
}

bool list_removeOrdered(List * list, u64 index) {
    if(index >= list->array.length)
        return false;

    list->array.length -= 1;

    if(index == list->array.length)
        return true;

    void * remove = arr_getPointer(list->array, index);
    void * elementAfter = arr_getPointer(list->array, index + 1);

    memmove(remove, elementAfter, list->array.length - index);

    return true;
}

bool list_appendArray(List * list, Array array) {
    if(list->array.elementSize != array.elementSize)
        return false;

    if(!list_ensureCapacity(list, list->array.length + array.length))
        return false;

    u64 endIndex = list->array.length;

    list->array.length += array.length;

    arr_setElements(list->array, endIndex, array);

    return true;
}

bool list_appendSubarray(List * list, Array array, u64 start, u64 end) {
    return list_appendArray(list, arr_subarray(array, start, end));
}



//
// Data Type Specific Functions
//

#define defineQuickSort(DATA_TYPE)                                                                        \
    void arr_##DATA_TYPE##QuickSort(Array array) {                                                        \
        arr_##DATA_TYPE##QuickSortBounded(array, 0, array.length - 1);                                    \
    }

#define defineQuickSortBounded(DATA_TYPE)                                                                 \
    void arr_##DATA_TYPE##QuickSortBounded(Array array, u64 first, u64 last) {                            \
        u64 pivotIndex, i, j;                                                                             \
        DATA_TYPE pivot, temp;                                                                            \
        DATA_TYPE * data;                                                                                 \
                                                                                                          \
        if(first < last) {                                                                                \
            data = (DATA_TYPE *) array.data;                                                              \
                                                                                                          \
            pivotIndex = first;                                                                           \
            pivot = data[pivotIndex];                                                                     \
                                                                                                          \
            i = first;                                                                                    \
            j = last;                                                                                     \
                                                                                                          \
            while(i < j) {                                                                                \
                while(data[i] <= pivot && i < last) {                                                     \
                    i++;                                                                                  \
                }                                                                                         \
                                                                                                          \
                while(data[j] > pivot) {                                                                  \
                    j--;                                                                                  \
                }                                                                                         \
                                                                                                          \
                if(i < j){                                                                                \
                    temp = data[i];                                                                       \
                    data[i] = data[j];                                                                    \
                    data[j] = temp;                                                                       \
                }                                                                                         \
            }                                                                                             \
                                                                                                          \
            temp = data[pivotIndex];                                                                      \
            data[pivotIndex] = data[j];                                                                   \
            data[j] = temp;                                                                               \
                                                                                                          \
            arr_##DATA_TYPE##QuickSortBounded(array, first, j-1);                                         \
            arr_##DATA_TYPE##QuickSortBounded(array, j+1, last);                                          \
        }                                                                                                 \
    }

#define defineBinarySearch(DATA_TYPE)                                                                     \
    s64 arr_##DATA_TYPE##BinarySearch(Array array, DATA_TYPE value) {                                     \
        return arr_##DATA_TYPE##BinarySearchBounded(array, 0, array.length - 1, value);                   \
    }

#define defineBinarySearchBounded(DATA_TYPE)                                                              \
    s64 arr_##DATA_TYPE##BinarySearchBounded(Array array, u64 left, u64 right, DATA_TYPE value) {         \
        right = min(right, array.length - 1);                                                             \
                                                                                                          \
        u64 index = right;                                                                                \
                                                                                                          \
        while(right >= left) {                                                                            \
            index = (right - left) / 2;                                                                   \
                                                                                                          \
            DATA_TYPE element = arr_##DATA_TYPE##Get(array, index);                                       \
                                                                                                          \
            if(element == value)                                                                          \
                return (s64) index;                                                                       \
                                                                                                          \
            if(element < value)                                                                           \
                left = index + 1;                                                                         \
            else {                                                                                        \
                right = index - 1;                                                                        \
            }                                                                                             \
        }                                                                                                 \
                                                                                                          \
        return -((s64) index) - 1;                                                                        \
    }

#define defineListAppend(DATA_TYPE)                                                                       \
    bool list_##DATA_TYPE##Append(List * list, DATA_TYPE value) {                                         \
        DATA_TYPE * pointer = list_append(list);                                                          \
                                                                                                          \
        if(pointer == NULL)                                                                               \
            return false;                                                                                 \
                                                                                                          \
        *pointer = value;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }

#define defineListInsert(DATA_TYPE)                                                                       \
    bool list_##DATA_TYPE##Insert(List * list, u64 index, DATA_TYPE value) {                              \
        DATA_TYPE * pointer = list_insert(list, index);                                                   \
                                                                                                          \
        if(pointer == NULL)                                                                               \
            return false;                                                                                 \
                                                                                                          \
        *pointer = value;                                                                                 \
                                                                                                          \
        return true;                                                                                      \
    }

#define defineDataSpecificFunctions(DATA_TYPE)                                                            \
    defineQuickSort(DATA_TYPE)                                                                            \
    defineQuickSortBounded(DATA_TYPE)                                                                     \
    defineBinarySearch(DATA_TYPE)                                                                         \
    defineBinarySearchBounded(DATA_TYPE)                                                                  \
    defineListAppend(DATA_TYPE)                                                                           \
    defineListInsert(DATA_TYPE)

defineDataSpecificFunctions(char);
defineDataSpecificFunctions(u8);
defineDataSpecificFunctions(u16);
defineDataSpecificFunctions(u32);
defineDataSpecificFunctions(u64);
defineDataSpecificFunctions(s8);
defineDataSpecificFunctions(s16);
defineDataSpecificFunctions(s32);
defineDataSpecificFunctions(s64);
defineDataSpecificFunctions(f32);
defineDataSpecificFunctions(f64);