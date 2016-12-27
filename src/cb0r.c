// by jeremie miller - 2015,2016
// public domain UNLICENSE, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

#include "cb0r.h"

#include <stdio.h>

// start at bin, returns end pointer (== stop if complete), either performs count items or extracts result of current item
uint8_t *cb0r(uint8_t *start, uint8_t *stop, uint32_t skip, cb0r_t result)
{
  static const void *go[] RODATA_SEGMENT_CONSTANT = 
  {
    [0x00 ... 0xff] = &&l_ebad,
    [0x00 ... 0x17] = &&l_int,
    [0x18] = &&l_int1, [0x19] = &&l_int2,[0x1a] = &&l_int4, [0x1b] = &&l_int8,
    [0x20 ... 0x37] = &&l_int,
    [0x38] = &&l_int1, [0x39] = &&l_int2,[0x3a] = &&l_int4, [0x3b] = &&l_int8,
    [0x40 ... 0x57] = &&l_byte,
    [0x58] = &&l_byte1, [0x59] = &&l_byte2,[0x5a] = &&l_byte4, [0x5b] = &&l_ebig,
    [0x5f] = &&l_until,
    [0x60 ... 0x77] = &&l_byte,
    [0x78] = &&l_byte1, [0x79] = &&l_byte2,[0x7a] = &&l_byte4, [0x7b] = &&l_ebig,
    [0x7f] = &&l_until,
    [0x80 ... 0x97] = &&l_array,
    [0x98] = &&l_array1, [0x99] = &&l_array2,[0x9a] = &&l_array4, [0x9b] = &&l_ebig,
    [0x9f] = &&l_until,
    [0xa0 ... 0xb7] = &&l_array,
    [0xb8] = &&l_array1, [0xb9] = &&l_array2,[0xba] = &&l_array4, [0xbb] = &&l_ebig,
    [0xbf] = &&l_until,
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

  uint8_t *end = start + 1;
  cb0r_e type = CB0R_ERR;
  uint8_t size = 0;
  uint32_t count = 0;

  goto *go[*start];

  l_int8:
    end += 4;
  l_int4:
    end += 2;
  l_int2:
    end += 1;
  l_int1:
    end += 1;
  l_int: 
    goto l_finish;

  l_byte4:
    size = 2;
    end += (uint32_t)(start[1]) << 24;
    end += (uint32_t)(start[2]) << 16;
  l_byte2:
    size += 1;
    end += (uint32_t)(start[size]) << 8;
  l_byte1:
    size += 1;
    end += start[size] + size;
    goto l_finish;

  l_byte: 
    end += (start[0] & 0x1f);
    goto l_finish;

  l_array4:
    size = 2;
    count += (uint32_t)(start[1]) << 24;
    count += (uint32_t)(start[2]) << 16;
  l_array2:
    size += 1;
    count += (uint32_t)(start[size]) << 8;
  l_array1:
    size += 1;
    count += start[size];
    goto l_skip;

  l_array: 
    count = (start[0] & 0x1f);
    goto l_skip;

  // skip fixed count items (stored as diff between end and start)
  l_skip: {
    if(count)
    {
      // map is double array
      if(start[0] & 0x20) end = cb0r(start+size+1,stop,(count*2)-1,NULL);
      else end = cb0r(start+size+1,stop,count-1,NULL);
    }else{
      end += size;
    }
    goto l_finish;
  }

  l_until: // indefinite length wrapper
    end = cb0r(start+1,stop,UINT32_MAX,NULL);
    goto l_finish;

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
    if(skip != UINT32_MAX) goto l_eparse;
    skip = 0;
    goto l_done;
  }

  l_ebad:
    type = CB0R_EBAD;
    goto l_fail;

  l_eparse:
    type = CB0R_EPARSE;
    goto l_fail;

  l_ebig:
    type = CB0R_EBIG;
    goto l_fail;

  l_fail: // all errors
    skip = 0;

  l_finish: // only first 7 types
    type = (start[0] >> 5);

  l_done:

  // done done, extract value if result requested
  if(!skip)
  {
    if(!result) return end;
    result->start = start+1;
    result->type = type;
    result->value = 0;
    switch(type)
    {
      case CB0R_INT:
      case CB0R_NEG: {
        size = end - result->start;
        switch(size)
        {
          case 8:
            // implementations can use start[8] to get 64 bit number
          case 4:
            result->value |= (uint32_t)(start[size - 3]) << 24;
            result->value |= (uint32_t)(start[size - 2]) << 16;
          case 2:
            result->value |= (uint32_t)(start[size - 1]) << 8;
          case 1:
            result->value |= start[size];
            break;
          case 0:
            result->value = start[size] & 0x1f;
        }
      } break;

      case CB0R_BYTE:
      case CB0R_UTF8: {
        result->start += size;
        result->length = end - (result->start + size);
      } break;

      case CB0R_ARRAY:
      case CB0R_MAP: {
        result->start += size;
        result->count = count;
      } break;
      case CB0R_TAG: {

      } break;
      case CB0R_FLOAT: {

      } break;
      default: {

      }
    }
    return end;
  }

  // max means streaming mode skip
  if(skip != UINT32_MAX) skip--;

  // tail recurse while skipping
  return cb0r(end, stop, skip, result);
}
