// by jeremie miller - 2015,2016
// public domain UNLICENSE, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

static const void *b0_go[] RODATA_SEGMENT_CONSTANT = 
{
  [0x00 ... 0xff] = &&l_bad,
  [0x00 ... 0x17] = &&l_uintv,
  [0x18 ... 0x1b] = &&l_uint,
  [0x20 ... 0x37] = &&l_nintv,
  [0x38 ... 0x3b] = &&l_nint,
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

l_bad:
l_uintv: {
  b0_count = 1;
  b0_type = CB0R_INT;
  goto l_next;
}
l_uint:
l_nintv:
l_nint:
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
l_break:















