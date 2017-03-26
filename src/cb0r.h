// by jeremie miller - 2015,2016
// public domain UNLICENSE, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef cb0r_h
#define cb0r_h

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RODATA_SEGMENT_CONSTANT
#define RODATA_SEGMENT_CONSTANT
#endif

// ->count value for indefinite length items
#define CB0R_STREAM UINT32_MAX

// flattened result types
typedef enum {
  CB0R_INT = 0  ,
  CB0R_NEG      ,
  CB0R_BYTE     ,
  CB0R_UTF8     ,
  CB0R_ARRAY    ,
  CB0R_MAP      ,
  // "unassigned" tag/simple values are this base type, rest expanded below
  CB0R_TAG      ,
  CB0R_SIMPLE   ,

  // expand standard base tags
  CB0R_TAGS = 8 , // if(type > CB0R_TAGS && type < CB0R_SIMPLES)
  CB0R_DATETIME ,
  CB0R_EPOCH    ,
  CB0R_BIGNUM   ,
  CB0R_BIGNEG   ,
  CB0R_FRACTION ,
  CB0R_BIGFLOAT ,
  CB0R_BASE64URL,
  CB0R_BASE64   ,
  CB0R_HEX      ,
  CB0R_DATA     ,

  // expand standard simple values
  CB0R_SIMPLES = 24,
  CB0R_FALSE    ,
  CB0R_TRUE     ,
  CB0R_NULL     ,
  CB0R_UNDEF    ,
  CB0R_FLOAT    ,

  // if(type >= CB0R_ERR) ...
  CB0R_ERR = 224,
  CB0R_EPARSE   , // invalid structure 
  CB0R_EBAD     , // invalid type byte
  CB0R_EBIG     , // unsupported size item
  CB0R_EMAX
} cb0r_e;

typedef struct cb0r_s
{
  cb0r_e type;
  uint8_t *start;
  uint8_t *end;
  union { // different names/aliases for context readability based on type
    uint64_t length;
    uint64_t count;
    uint64_t value;
  };
} cb0r_s, *cb0r_t;

// pass cbor bin via start/stop pointers, returns end pointer (== stop if complete)
// can skip a given number of items and then will fill result w/ the current item
uint8_t *cb0r(uint8_t *start, uint8_t *stop, uint32_t skip, cb0r_t result);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // cb0r_h

