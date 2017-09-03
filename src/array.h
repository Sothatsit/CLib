#ifndef JSON_util_array_h
#define JSON_util_array_h

#include "types.h"

#include <stdbool.h>

//
// Arrays
//

struct Array {
    void * data;
    u64 length;
    u64 elementSize;
};

typedef struct Array Array;

/*
 * Returns a pointer to the element at {index} in {array}.
 */
#define arr_getPointer(array, index) (&array.data[index * array.elementSize])

/*
 * Returns an Array with length 0.
 */
#define arr_createEmpty() (arr_create(NULL, 0, 0))

/*
 * Returns whether {array} is empty.
 */
#define arr_isEmpty(array) (array.length == 0)

/*
 * Create an Array with {length} elements of size {elementSize} bytes stored sequentially at {data}.
 *
 * This will use {data}, if {data} is free'd the returned Array will
 * be invalid. Use arr_createCopy to create a copy of the data.
 *
 * arr_destroy may be used on the returned Array and will free {data}.
 */
Array arr_create(void * data, u64 length, u64 elementSize);

/*
 * Create an Array with {length} elements of size {elementSize} bytes
 * stored sequentially with the data copied from the data at {data}.
 *
 * arr_destroy should be used on the returned Array after it is no longer in use.
 */
Array arr_createCopy(void * data, u64 length, u64 elementSize);

/*
 * Create an Array with {length} elements of size {elementSize} bytes.
 *
 * arr_destroy should be used on the returned Array when no longer in use.
 */
Array arr_createUninitialised(u64 length, u64 elementSize);

/*
 * Create a copy of array.
 *
 * Will return an empty Array on fail.
 */
Array arr_copy(Array array);

/*
 * Destroy array.
 *
 * Any Arrays derived from array (e.g. sub-arrays) will become
 * invalid after this operation unless copied using array_copy.
 *
 * If array is a sub-array of another Array, this operation is undefined and may error.
 */
void arr_destroy(Array array);

/*
 * Check if the contents of array1 equal the contents of array2.
 */
bool arr_equals(Array array1, Array array2);

/*
 * Convert array to a new Array where all the elements are of size newElementSize.
 *
 * If array's elementSize is less than newElementSize, this will left pad each element with 0s.
 *
 * If array's elementSize is equal to newElementSize, array will be returned.
 *
 * If array's elementSize is greater than newElementSize, this will copy the rightmost bits in each element.
 *
 * Will return an empty array on failure.
 */
Array arr_convertLeftPadded(Array array, u64 newElementSize);

/*
 * Convert array to a new Array where all the elements are of size newElementSize.
 *
 * If array's elementSize is less than newElementSize, this will right pad each element with 0s.
 *
 * If array's elementSize is equal to newElementSize, array will be returned.
 *
 * If array's elementSize is greater than newElementSize, this will copy the leftmost bits in each element.
 *
 * Will return an empty array on failure.
 */
Array arr_convertRightPadded(Array array, u64 newElementSize);

/*
 * Set the elements in array from index to the elements in replacements.
 *
 * Returns whether setting the elements was successful.
 */
bool arr_setElements(Array array, u64 index, Array replacements);

/*
 * Get a subarray of array inclusive from the index start to end - 1.
 *
 * The length of the returned Array will be end - start.
 *
 * The returned Array will use the data of array. Therefore the returned Array
 * does not need to be destroyed and destroying array will make the returned
 * Array invalid. Use arr_copy on the returned Array if this is not wanted.
 */
Array arr_subarray(Array array, u64 start, u64 end);

/*
 * Concatenate array1 and array2 into a new Array.
 *
 * array1 and array2 must have the same elementSize.
 * You can convert array1 or array2's elementSize using arr_convertLeftPadded or arr_convertRightPadded.
 *
 * Will return an empty array on failure.
 */
Array arr_concat(Array array1, Array array2);



//
// Lists
//

struct List {
    Array array;

    u64 capacity;
};

typedef struct List List;

/*
 * Creates a new List for constructing Arrays.
 *
 * If initialSize is 0, the initial size of the List will be defaulted to 8 elements.
 *
 * Will return an List with capacity 0 on error.
 */
List list_create(u64 initialSize, u64 elementSize);

/*
 * Destroys list.
 *
 * This will destroy the Array recieved from List.array as well. Only the List or
 * the Array recieved from List.array need to be destroyed, they share the same data.
 */
void list_destroy(List list);

/*
 * Returns an Array with a copy of the Array in list.
 */
#define list_getArrayCopy(list) (arr_copy(list.array))

/*
 * Sets the capacity of list to capacity.
 *
 * Returns whether setting the capacity was successful.
 */
bool list_setCapacity(List * list, u64 capacity);

/*
 * Will increase the capacity of list so it's capacity is at least requiredCapacity.
 *
 * Returns whether it was successful in increasing the capacity of list to at least requiredCapacity.
 */
bool list_ensureCapacity(List * list, u64 requiredCapacity);

/*
 * Attempts to reduce the capacity of list so it is equal to the length of it's contents.
 *
 * Returns whether reducing the capacity was successful.
 */
bool list_trimToLength(List * list);

/*
 * Will increase the capacity of list and return a pointer to the new element so it can be set.
 *
 * Will return NULL on failure.
 */
void * list_append(List * list);

/*
 * Alias of list_append that sets the element automatically.
 *
 * Returns whether appending element was successful.
 */
bool list_appendElement(List * list, void * element);

/*
 * Will increase the capacity of list and return a pointer to the new element so it can be set.
 *
 * Will return NULL on failure.
 */
void * list_insert(List * list, u64 index);

/*
 * Alias of list_insert that sets the element automatically.
 *
 * Returns whether inserting the element was successful.
 */
bool list_insertValue(List * list, u64 index, void * element);

/*
 * Remove the element at index of list.
 *
 * Returns whether removing the element was successful.
 */
bool list_remove(List * list, u64 index);

/*
 * Remove the element at index of list, maintaining the order of list.
 *
 * Returns whether removing the element was successful.
 */
bool list_removeOrdered(List * list, u64 index);

/*
 * Append array to list.
 *
 * array must have the same element size as the array in list.
 *
 * Will return true if successful.
 */
bool list_appendArray(List * list, Array array);

/*
 * Append a subset of array inclusive from the index start to end - 1 to list.
 */
bool list_appendSubarray(List * list, Array array, u64 start, u64 end);



//
// char specific functions
//

/*
 * Alias of arr_create for the char data type.
 */
#define arr_charCreate(data, length) (arr_create(data, length, sizeof(char)))

/*
 * Alias of arr_createOfLength for the char data type.
 */
#define arr_charCreateOfLength(length) (arr_createOfLength(length, sizeof(char)))

/*
 * Get the char at index in array.
 */
#define arr_charGet(array, index) (*((char *) arr_getPointer(array, index)))

/*
 * Set the char at index in array to value.
 */
#define arr_charSet(array, index, value) (*((char *) arr_getPointer(array, index)) = value)

/*
 * Create a new array with all the elements converted to chars.
 *
 * This treats all the elements in array as unsigned numbers.
 */
#define arr_toCharArray(array) (arr_convertLeftPadded(array, sizeof(char)))

/*
 * Perform a quick sort on the whole array. See arr_charQuickSortBounded for details.
 */
void arr_charQuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_charQuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_charBinarySearchBounded for details.
 */
s64 arr_charBinarySearch(Array array, char value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_charBinarySearchBounded(Array array, u64 left, u64 right, char value);

/*
 * Alias of list_create for the char data type.
 */
#define list_charCreate(initialSize) (list_create(length, sizeof(char)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_charAppend(List * list, char value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_charInsert(List * list, u64 index, char value);



//
// u8 specific functions
//

/*
 * Alias of arr_create for the u8 data type.
 */
#define arr_u8Create(data, length) (arr_create(data, length, sizeof(u8)))

/*
 * Alias of arr_createOfLength for the u8 data type.
 */
#define arr_u8CreateOfLength(length) (arr_createOfLength(length, sizeof(u8)))

/*
 * Get the u8 at index in array.
 */
#define arr_u8Get(array, index) (*((u8 *) arr_getPointer(array, index)))

/*
 * Set the u8 at index in array to value.
 */
#define arr_u8Set(array, index, value) (*((u8 *) arr_getPointer(array, index)) = value)

/*
 * Create a new array with all the elements converted to u8s.
 *
 * This treats all the elements in array as unsigned numbers.
 */
#define arr_u8ToArray(array) (arr_convertLeftPadded(array, sizeof(u8)))

/*
 * Perform a quick sort on the whole array. See arr_u8QuickSortBounded for details.
 */
void arr_u8QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_u8QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_u8BinarySearchBounded for details.
 */
s64 arr_u8BinarySearch(Array array, u8 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_u8BinarySearchBounded(Array array, u64 left, u64 right, u8 value);

/*
 * Alias of list_create for the u8 data type.
 */
#define list_u8Create(initialSize) (list_create(length, sizeof(u8)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_u8Append(List * list, u8 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_u8Insert(List * list, u64 index, u8 value);



//
// u16 specific functions
//

/*
 * Alias of arr_create for the u16 data type.
 */
#define arr_u16Create(data, length) (arr_create(data, length, sizeof(u16)))

/*
 * Alias of arr_createOfLength for the u16 data type.
 */
#define arr_u16CreateOfLength(length) (arr_createOfLength(length, sizeof(u16)))

/*
 * Get the u16 at index in array.
 */
#define arr_u16Get(array, index) (*((u16 *) arr_getPointer(array, index)))

/*
 * Set the u16 at index in array to value.
 */
#define arr_u16Set(array, index, value) (*((u16 *) arr_getPointer(array, index)) = value)

/*
 * Create a new array with all the elements converted to u16s.
 *
 * This treats all the elements in array as unsigned numbers.
 */
#define arr_u16ToArray(array) (arr_convertLeftPadded(array, sizeof(u16)))

/*
 * Perform a quick sort on the whole array. See arr_u16QuickSortBounded for details.
 */
void arr_u16QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_u16QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_u16BinarySearchBounded for details.
 */
s64 arr_u16BinarySearch(Array array, u16 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_u16BinarySearchBounded(Array array, u64 left, u64 right, u16 value);

/*
 * Alias of list_create for the u16 data type.
 */
#define list_u16Create(initialSize) (list_create(length, sizeof(u16)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_u16Append(List * list, u16 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_u16Insert(List * list, u64 index, u16 value);



//
// u32 specific functions
//

/*
 * Alias of arr_create for the u32 data type.
 */
#define arr_u32Create(data, length) (arr_create(data, length, sizeof(u32)))

/*
 * Alias of arr_createOfLength for the u32 data type.
 */
#define arr_u32CreateOfLength(length) (arr_createOfLength(length, sizeof(u32)))

/*
 * Get the u32 at index in array.
 */
#define arr_u32Get(array, index) (*((u32 *) arr_getPointer(array, index)))

/*
 * Set the u32 at index in array to value.
 */
#define arr_u32Set(array, index, value) (*((u32 *) arr_getPointer(array, index)) = value)

/*
 * Create a new array with all the elements converted to u32s.
 *
 * This treats all the elements in array as unsigned numbers.
 */
#define arr_u32ToArray(array) (arr_convertLeftPadded(array, sizeof(u32)))

/*
 * Perform a quick sort on the whole array. See arr_u32QuickSortBounded for details.
 */
void arr_u32QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_u32QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_u32BinarySearchBounded for details.
 */
s64 arr_u32BinarySearch(Array array, u32 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_u32BinarySearchBounded(Array array, u64 left, u64 right, u32 value);

/*
 * Alias of list_create for the u32 data type.
 */
#define list_u32Create(initialSize) (list_create(length, sizeof(u32)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_u32Append(List * list, u32 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_u32Insert(List * list, u64 index, u32 value);



//
// u64 specific functions
//

/*
 * Alias of arr_create for the u64 data type.
 */
#define arr_u64Create(data, length) (arr_create(data, length, sizeof(u64)))

/*
 * Alias of arr_createOfLength for the u64 data type.
 */
#define arr_u64CreateOfLength(length) (arr_createOfLength(length, sizeof(u64)))

/*
 * Get the u64 at index in array.
 */
#define arr_u64Get(array, index) (*((u64 *) arr_getPointer(array, index)))

/*
 * Set the u64 at index in array to value.
 */
#define arr_u64Set(array, index, value) (*((u64 *) arr_getPointer(array, index)) = value)

/*
 * Create a new array with all the elements converted to u64s.
 *
 * This treats all the elements in array as unsigned numbers.
 */
#define arr_u64ToArray(array) (arr_convertLeftPadded(array, sizeof(u64)))

/*
 * Perform a quick sort on the whole array. See arr_u64QuickSortBounded for details.
 */
void arr_u64QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_u64QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_u64BinarySearchBounded for details.
 */
s64 arr_u64BinarySearch(Array array, u64 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_u64BinarySearchBounded(Array array, u64 left, u64 right, u64 value);

/*
 * Alias of list_create for the u64 data type.
 */
#define list_u64Create(initialSize) (list_create(length, sizeof(u64)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_u64Append(List * list, u64 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_u64Insert(List * list, u64 index, u64 value);



//
// s8 specific functions
//

/*
 * Alias of arr_create for the s8 data type.
 */
#define arr_s8Create(data, length) (arr_create(data, length, sizeof(s8)))

/*
 * Alias of arr_createOfLength for the s8 data type.
 */
#define arr_s8CreateOfLength(length) (arr_createOfLength(length, sizeof(s8)))

/*
 * Get the s8 at index in array.
 */
#define arr_s8Get(array, index) (*((s8 *) arr_getPointer(array, index)))

/*
 * Set the s8 at index in array to value.
 */
#define arr_s8Set(array, index, value) (*((s8 *) arr_getPointer(array, index)) = value)

/*
 * Perform a quick sort on the whole array. See arr_s8QuickSortBounded for details.
 */
void arr_s8QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_s8QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_s8BinarySearchBounded for details.
 */
s64 arr_s8BinarySearch(Array array, s8 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_s8BinarySearchBounded(Array array, u64 left, u64 right, s8 value);

/*
 * Alias of list_create for the s8 data type.
 */
#define list_s8Create(initialSize) (list_create(length, sizeof(s8)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_s8Append(List * list, s8 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_s8Insert(List * list, u64 index, s8 value);



//
// s16 specific functions
//

/*
 * Alias of arr_create for the s16 data type.
 */
#define arr_s16Create(data, length) (arr_create(data, length, sizeof(s16)))

/*
 * Alias of arr_createOfLength for the s16 data type.
 */
#define arr_s16CreateOfLength(length) (arr_createOfLength(length, sizeof(s16)))

/*
 * Get the s16 at index in array.
 */
#define arr_s16Get(array, index) (*((s16 *) arr_getPointer(array, index)))

/*
 * Set the s16 at index in array to value.
 */
#define arr_s16Set(array, index, value) (*((s16 *) arr_getPointer(array, index)) = value)

/*
 * Perform a quick sort on the whole array. See arr_s16QuickSortBounded for details.
 */
void arr_s16QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_s16QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_s16BinarySearchBounded for details.
 */
s64 arr_s16BinarySearch(Array array, s16 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_s16BinarySearchBounded(Array array, u64 left, u64 right, s16 value);

/*
 * Alias of list_create for the s16 data type.
 */
#define list_s16Create(initialSize) (list_create(length, sizeof(s16)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_s16Append(List * list, s16 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_s16Insert(List * list, u64 index, s16 value);



//
// s32 specific functions
//

/*
 * Alias of arr_create for the s32 data type.
 */
#define arr_s32Create(data, length) (arr_create(data, length, sizeof(s32)))

/*
 * Alias of arr_createOfLength for the s32 data type.
 */
#define arr_s32CreateOfLength(length) (arr_createOfLength(length, sizeof(s32)))

/*
 * Get the s32 at index in array.
 */
#define arr_s32Get(array, index) (*((s32 *) arr_getPointer(array, index)))

/*
 * Set the s32 at index in array to value.
 */
#define arr_s32Set(array, index, value) (*((s32 *) arr_getPointer(array, index)) = value)

/*
 * Perform a quick sort on the whole array. See arr_s32QuickSortBounded for details.
 */
void arr_s32QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_s32QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_s32BinarySearchBounded for details.
 */
s64 arr_s32BinarySearch(Array array, s32 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_s32BinarySearchBounded(Array array, u64 left, u64 right, s32 value);

/*
 * Alias of list_create for the s32 data type.
 */
#define list_s32Create(initialSize) (list_create(length, sizeof(s32)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_s32Append(List * list, s32 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_s32Insert(List * list, u64 index, s32 value);



//
// s64 specific functions
//

/*
 * Alias of arr_create for the s64 data type.
 */
#define arr_s64Create(data, length) (arr_create(data, length, sizeof(s64)))

/*
 * Alias of arr_createOfLength for the s64 data type.
 */
#define arr_s64CreateOfLength(length) (arr_createOfLength(length, sizeof(s64)))

/*
 * Get the s64 at index in array.
 */
#define arr_s64Get(array, index) (*((s64 *) arr_getPointer(array, index)))

/*
 * Set the s64 at index in array to value.
 */
#define arr_s64Set(array, index, value) (*((s64 *) arr_getPointer(array, index)) = value)

/*
 * Perform a quick sort on the whole array. See arr_s64QuickSortBounded for details.
 */
void arr_s64QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_s64QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_s64BinarySearchBounded for details.
 */
s64 arr_s64BinarySearch(Array array, s64 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_s64BinarySearchBounded(Array array, u64 left, u64 right, s64 value);

/*
 * Alias of list_create for the s64 data type.
 */
#define list_s64Create(initialSize) (list_create(length, sizeof(s64)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_s64Append(List * list, s64 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_s64Insert(List * list, u64 index, s64 value);



//
// f32 specific functions
//

/*
 * Alias of arr_create for the f32 data type.
 */
#define arr_f32Create(data, length) (arr_create(data, length, sizeof(f32)))

/*
 * Alias of arr_createOfLength for the f32 data type.
 */
#define arr_f32CreateOfLength(length) (arr_createOfLength(length, sizeof(f32)))

/*
 * Get the f32 at index in array.
 */
#define arr_f32Get(array, index) (*((f32 *) arr_getPointer(array, index)))

/*
 * Set the f32 at index in array to value.
 */
#define arr_f32Set(array, index, value) (*((f32 *) arr_getPointer(array, index)) = value)

/*
 * Perform a quick sort on the whole array. See arr_f32QuickSortBounded for details.
 */
void arr_f32QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_f32QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_f32BinarySearchBounded for details.
 */
s64 arr_f32BinarySearch(Array array, f32 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_f32BinarySearchBounded(Array array, u64 left, u64 right, f32 value);

/*
 * Alias of list_create for the f32 data type.
 */
#define list_f32Create(initialSize) (list_create(length, sizeof(f32)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_f32Append(List * list, f32 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_f32Insert(List * list, u64 index, f32 value);



//
// f64 specific functions
//

/*
 * Alias of arr_create for the f64 data type.
 */
#define arr_f64Create(data, length) (arr_create(data, length, sizeof(f64)))

/*
 * Alias of arr_createOfLength for the f64 data type.
 */
#define arr_f64CreateOfLength(length) (arr_createOfLength(length, sizeof(f64)))

/*
 * Get the f64 at index in array.
 */
#define arr_f64Get(array, index) (*((f64 *) arr_getPointer(array, index)))

/*
 * Set the f64 at index in array to value.
 */
#define arr_f64Set(array, index, value) (*((f64 *) arr_getPointer(array, index)) = value)

/*
 * Perform a quick sort on the whole array. See arr_f64QuickSortBounded for details.
 */
void arr_f64QuickSort(Array array);

/*
 * Perform a quick sort over the bounds first to last inclusive of array.
 *
 * Will sort in ascending order.
 */
void arr_f64QuickSortBounded(Array array, u64 first, u64 last);

/*
 * Perform a binary search on the whole array. See arr_f64BinarySearchBounded for details.
 */
s64 arr_f64BinarySearch(Array array, f64 value);

/*
 * Perform a binary search over the bounds left to right inclusive of array.
 *
 * Will return the index of value in array if found, otherwise will return -(insertion point) - 1,
 * where insertion point is the point in the array where it would be found.
 */
s64 arr_f64BinarySearchBounded(Array array, u64 left, u64 right, f64 value);

/*
 * Alias of list_create for the f64 data type.
 */
#define list_f64Create(initialSize) (list_create(length, sizeof(f64)))

/*
 * Appends value to list, returning whether it was successful.
 */
bool list_f64Append(List * list, f64 value);

/*
 * Inserts value into list at index, returning whether it was successful.
 */
bool list_f64Insert(List * list, u64 index, f64 value);

#endif