// by jeremie miller - 2015,2016
// public domain UNLICENSE, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef RODATA_SEGMENT_CONSTANT
#define RODATA_SEGMENT_CONSTANT
#endif

typedef enum {
  CB0R_INT = 0,
  CB0R_NEG    ,
  CB0R_BYTE   ,
  CB0R_UTF8   ,
  CB0R_ARRAY  ,
  CB0R_MAP    ,
  CB0R_TAG    ,
  CB0R_SIMPLE , // 7, last of the major types

  // the simple values
  CB0R_FALSE = 20 ,
  CB0R_TRUE   ,
  CB0R_NULL   ,
  CB0R_UNDEF  ,
  CB0R_FLOAT  ,

  // if(type >= CB0R_ERR) ...
  CB0R_ERR = 32,
  CB0R_EPARSE , // invalid structure 
  CB0R_EBAD   , // invalid type byte
  CB0R_EBIG   , // unsupported size item
  CB0R_EMAX
} cb0r_e;

typedef struct cb0r_s
{
  cb0r_e type;
  uint8_t *start;
  union { // different names/aliases for context readability based on type
    uint64_t length;
    uint64_t count;
    uint64_t value;
  };
} cb0r_s, *cb0r_t;

// start at bin, returns end pointer (== stop if complete), either skips items or extracts result of current item
uint8_t *cb0r(uint8_t *start, uint8_t *stop, uint32_t skip, cb0r_t result);


