#ifndef JSON_types_h
#define JSON_types_h

#include <stdint.h>

/*
 * Get the maximum of num1 and num2.
 */
#define max(num1, num2) (num1 > num2 ? num1 : num2)

/*
 * Get the minimum of num1 and num2.
 */
#define min(num1, num2) (num1 < num2 ? num1 : num2)

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

#endif