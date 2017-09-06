#ifndef __CLIB_numbers_h
#define __CLIB_numbers_h

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int32_t s64;

typedef float f32;
typedef double f64;

/*
 * Get the maximum of num1 and num2.
 */
#define max(num1, num2) (num1 > num2 ? num1 : num2)

/*
 * Get the minimum of num1 and num2.
 */
#define min(num1, num2) (num1 < num2 ? num1 : num2)

/*
 * Round to the nearest power of 2.
 */
u8 u8_findNextPowerOf2(u8 number);

/*
 * Round to the nearest power of 2.
 */
u16 u16_findNextPowerOf2(u16 number);

/*
 * Round to the nearest power of 2.
 */
u32 u32_findNextPowerOf2(u32 number);

/*
 * Round to the nearest power of 2.
 */
u64 u64_findNextPowerOf2(u64 number);

#endif