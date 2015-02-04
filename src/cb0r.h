#include <stdint.h>
#include <stdlib.h>

// these are the possible types
#define CB0R_ERR    0
#define CB0R_INT    1   // (int64_t)val
#define CB0R_UINT   2   // val
#define CB0R_BYTE   3   // start+val
#define CB0R_TEXT   4   // start+val
#define CB0R_ARRAY  5   // val is items, start == first
#define CB0R_MAP    6   // val is pairs, start == first
#define CB0R_TAG    7   // val
#define CB0R_FLOAT  8   // (double)val
#define CB0R_TRUE   9   // n/a
#define CB0R_FALSE  10  // n/a
#define CB0R_NULL   11  // n/a
#define CB0R_UNDEF  12  // n/a
#define CB0R_END    13  // n/a

size_t cb0r(uint8_t *raw, size_t len, uint8_t *type, uint64_t *val, size_t *start);
