// by jeremie miller - 2015,2016
// public domain UNLICENSE, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

#include "cb0r.h"

#include <stdio.h>

// start at bin, returns end pointer (== stop if complete), either performs count items or extracts result of current item
uint8_t *cb0r(uint8_t *start, uint8_t *stop, uint32_t skip, cb0r_t result)
{
  static const void *go[] RODATA_SEGMENT_CONSTANT = 
  {
    [0x00 ... 0xff] = &&l_bad,
    [0x00 ... 0x17] = &&l_int,
    [0x18] = &&l_int1, [0x19] = &&l_int2,[0x1a] = &&l_int4, [0x1b] = &&l_int8,
    [0x20 ... 0x37] = &&l_intnv,
    [0x38 ... 0x3b] = &&l_intn,
    [0x40 ... 0x57] = &&l_bytev,
    [0x58 ... 0x5f] = &&l_byte,
    [0x60 ... 0x77] = &&l_utf8v,
    [0x78 ... 0x7f] = &&l_utf8,
    [0x80 ... 0x97] = &&l_arrayv,
    [0x98 ... 0x9f] = &&l_array,
    [0xa0 ... 0xb7] = &&l_mapv,
    [0xb8 ... 0xbf] = &&l_map,
    [0xc0] = &&l_datet,
    [0xc1] = &&l_datee,
    [0xc2] = &&l_ubig,
    [0xc3] = &&l_nbig,
    [0xc4] = &&l_fraction,
    [0xc5] = &&l_fbig,
    [0xc6 ... 0xd4] = &&l_tagv,
    [0xd5 ... 0xd7] = &&l_convert,
    [0xd8 ... 0xdb] = &&l_tag,
    [0xe0 ... 0xf3] = &&l_simplev,
    [0xf4] = &&l_false,
    [0xf5] = &&l_true,
    [0xf6] = &&l_null,
    [0xf7] = &&l_undefined,
    [0xf8] = &&l_simple,
    [0xf9 ... 0xfb] = &&l_float,
    [0xff] = &&l_break
  };

  if(start >= stop) return stop;

  uint8_t *end = NULL;
  cb0r_e type = CB0R_ERR;
  uint8_t byte = *start;
  uint8_t bytes = 0;

  goto *go[byte];

  l_int8:
    bytes += 4;
  l_int4:
    bytes += 2;
  l_int2:
    bytes += 1;
  l_int1:
    bytes += 1;
  l_int: 
    type = CB0R_INT;
    start++;
    end = start+bytes;
    goto l_done;

  l_intnv:
  l_intn:
  l_bytev:
  l_byte:
  l_utf8v:
  l_utf8:
  l_arrayv:
  l_array:
  l_mapv:
  l_map:
  l_datet:
  l_datee:
  l_ubig:
  l_nbig:
  l_fraction:
  l_fbig:
  l_tagv:
  l_convert:
  l_tag:
  l_simplev:
  l_false:
  l_true:
  l_null:
  l_undefined:
  l_simple:
  l_float:

  l_break: {
    if(skip != UINT32_MAX) goto l_err;
    skip = 0;
    goto l_done;
  }

  l_bad: {
    type = CB0R_EBAD;
    skip = 0;
    goto l_done;
  }

  l_err: {
    type = CB0R_EPARSE;
    skip = 0;
    goto l_done;
  }

  l_done:

  if(end > stop)
  {
    type = CB0R_EMORE;
    skip = 0;
  }

  // done done
  if(!skip)
  {
    if(result)
    {
      result->type = type;
      result->start = start;
      result->value = 0;
      switch(bytes)
      {
        case 8:
        case 4:
          result->value |= (uint32_t)(start[bytes - 4]) << 24;
          result->value |= (uint32_t)(start[bytes - 3]) << 16;

        case 2:
          result->value |= (uint32_t)(start[bytes - 2]) << 8;
        case 1:
          result->value |= start[bytes - 1];
          break;
        case 0:
          result->value = byte & 0x1f;
      }
    }
    return end;
  }

  // max means streaming mode skip
  if(skip != UINT32_MAX) skip--;

  // tail recurse while skipping
  return cb0r(end, stop, skip - 1, result);
}
