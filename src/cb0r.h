#include <stdint.h>
#include <stdlib.h>

// these are the possible types
enum {
  CB0R_ERR = 0,
  CB0R_INT  , // (int64_t)val
  CB0R_UINT , // val
  CB0R_BYTE , // start+val
  CB0R_TEXT , // start+val
  CB0R_ARRAY, // val is items, start == first
  CB0R_MAP  , // val is pairs, start == first
  CB0R_TAG  , // val
  CB0R_FLOAT, // (double)val
  CB0R_TRUE , // n/a
  CB0R_FALSE, // n/a
  CB0R_NULL , // n/a
  CB0R_UNDEF, // n/a
  CB0R_END
}

size_t cb0r(uint8_t *raw, size_t len, uint8_t *type, uint64_t *val, size_t *start);

#define CB0R_CODE // table and labels
#define CB0R_NEXT // sets type, start*, end*
#define CB0R_VALUE() // uses type/start/end, fills in given value