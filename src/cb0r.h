// by jeremie miller - 2015,2016
// public domain UNLICENSE, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// unhelpful warning noise for syntax used in zen
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Winitializer-overrides"
#pragma GCC diagnostic ignored "-Woverride-init"

#ifndef RODATA_SEGMENT_CONSTANT
#define RODATA_SEGMENT_CONSTANT
#endif

// these are the possible types
typedef enum {
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
} cb0r_t;


// Jump Table from https://tools.ietf.org/html/rfc7049#appendix-B
/****************************************************************
0x00..0x17	Integer 0x00..0x17 (0..23)
0x18	Unsigned integer (one-byte uint8_t follows)
0x19	Unsigned integer (two-byte uint16_t follows)
0x1a	Unsigned integer (four-byte uint32_t follows)
0x1b	Unsigned integer (eight-byte uint64_t follows)
0x20..0x37	Negative Integer -1-0x00..-1-0x17 (-1..-24)
0x38	Negative Integer -1-n (one-byte uint8_t for n follows)
0x39	Negative integer -1-n (two-byte uint16_t for n follows)
0x3a	Negative integer -1-n (four-byte uint32_t for n follows)
0x3b	Negative integer -1-n (eight-byte uint64_t for n follows)
0x40..0x57	byte string (0x00..0x17 bytes follow)
0x58	byte string (one-byte uint8_t for n, and then n bytes follow)
0x59	byte string (two-byte uint16_t for n, and then n bytes follow)
0x5a	byte string (four-byte uint32_t for n, and then n bytes follow)
0x5b	byte string (eight-byte uint64_t for n, and then n bytes follow)
0x5f	byte string, byte strings follow, terminated by “break”
0x60..0x77	UTF-8 string (0x00..0x17 bytes follow)
0x78	UTF-8 string (one-byte uint8_t for n, and then n bytes follow)
0x79	UTF-8 string (two-byte uint16_t for n, and then n bytes follow)
0x7a	UTF-8 string (four-byte uint32_t for n, and then n bytes follow)
0x7b	UTF-8 string (eight-byte uint64_t for n, and then n bytes follow)
0x7f	UTF-8 string, UTF-8 strings follow, terminated by “break”
0x80..0x97	array (0x00..0x17 data items follow)
0x98	array (one-byte uint8_t for n, and then n data items follow)
0x99	array (two-byte uint16_t for n, and then n data items follow)
0x9a	array (four-byte uint32_t for n, and then n data items follow)
0x9b	array (eight-byte uint64_t for n, and then n data items follow)
0x9f	array, data items follow, terminated by “break”
0xa0..0xb7	map (0x00..0x17 pairs of data items follow)
0xb8	map (one-byte uint8_t for n, and then n pairs of data items follow)
0xb9	map (two-byte uint16_t for n, and then n pairs of data items follow)
0xba	map (four-byte uint32_t for n, and then n pairs of data items follow)
0xbb	map (eight-byte uint64_t for n, and then n pairs of data items follow)
0xbf	map, pairs of data items follow, terminated by “break”
0xc0	Text-based date/time (data item follows, see Section 2.4.1)
0xc1	Epoch-based date/time (data item follows, see Section 2.4.1)
0xc2	Positive bignum (data item “byte string” follows)
0xc3	Negative bignum (data item “byte string” follows)
0xc4	Decimal Fraction (data item “array” follows, see Section 2.4.3)
0xc5	Bigfloat (data item “array” follows, see Section 2.4.3)
0xc6..0xd4	(tagged item, tag to be assigned by IANA)
0xd5..0xd7	Expected Conversion (data item follows, see Section 2.4.4.2)
0xd8..0xdb	(more tagged items, 1/2/4/8 bytes and then a data item follow)
0xe0..0xf3	(simple value to be assigned by IANA)
0xf4	FALSE
0xf5	TRUE
0xf6	Null
0xf7	Undefined
0xf8	(simple value to be assigned by IANA, one byte follows)
0xf9	Half-Precision Float (two-byte IEEE 754)
0xfa	Single-Precision Float (four-byte IEEE 754)
0xfb	Double-Precision Float (eight-byte IEEE 754)
0xff	“break” stop code
*/
