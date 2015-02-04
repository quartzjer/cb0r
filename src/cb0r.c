// by jeremie miller - 2015
// public domain, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

#include <stdint.h>
#include <string.h> // one strncmp() is used to do key comparison, and a strlen(key) if no len passed in

// gcc started warning for the init syntax used here, is not helpful so don't generate the spam, supressing the warning is really inconsistently supported across versions
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Winitializer-overrides"
#pragma GCC diagnostic ignored "-Woverride-init"

// only at depth 1, track start pointers to match key/value
#define PUSH(i) if(depth == 1) { if(!index) { val = cur+i; }else{ if(klen && index == 1) start = cur+i; else index--; } }

// determine if key matches or value is complete
#define CAP(i) if(depth == 1) { if(val && !index) {*vlen = (cur+i+1) - val; return val;}; if(klen) index = (start && klen == (cur-start) && strncmp(key,start,klen)==0) ? 0 : 1;}

// this makes a single pass across the bytes, using each byte as an index into a jump table to build an index and transition state
uint8_t *cb0r(char *key, size_t klen, uint8_t *cbor, size_t clen, size_t *vlen)
{
  char *val = 0;
  char *cur, *end, *start;
  int index = 1;
  int depth = 0;
  
  // http://tools.ietf.org/html/rfc7049#appendix-B
  static void *table[] = 
  {
    [0x00 ... 0xff] = &&l_bad,

    [0x00 ... 0x17] = &&l_uint,
    [0x18 ... 0x1b] = &&l_uints,

    [0x20 ... 0x37] = &&l_int,
    [0x38 ... 0x3b] = &&l_ints,

    [0x40 ... 0x57] = &&l_byte,
    [0x58 ... 0x5b] = &&l_bytes,
    [0x5f] = &&l_bytev,

    [0x60 ... 0x77] = &&l_utf8,
    [0x78 ... 0x7b] = &&l_utf8s,
    [0x7f] = &&l_utf8v,

    [0x80 ... 0x97] = &&l_array,
    [0x98 ... 0x9b] = &&l_arrays,
    [0x9f] = &&l_arrayv,

    [0xa0 ... 0xb7] = &&l_map,
    [0xb8 ... 0xbb] = &&l_maps,
    [0xbf] = &&l_mapv,

    [0xc0] = &&l_time,
    [0xc1] = &&l_epoch,

    [0xc2] = &&l_bigp,
    [0xc3] = &&l_bign,
    [0xc4] = &&l_bigd,
    [0xc5] = &&l_bigf,

    [0xc6 ... 0xd4] = &&l_tag,
    [0xd5 ... 0xd7] = &&l_tagx,
    [0xd8 ... 0xdb] = &&l_tagv,
    
    [0xe0 ... 0xf3] = &&l_v,
    [0xf4] = &&l_vfalse,
    [0xf5] = &&l_vtrue,
    [0xf6] = &&l_vnull,
    [0xf7] = &&l_vundef,
    [0xf8] = &&l_vone,

    [0xf9] = &&l_f2,
    [0xfa] = &&l_f4,
    [0xfb] = &&l_f8,

    [0xff] = &&l_stop,
    
  };

  void **go = table;
  
  if(!json || jlen <= 0 || !vlen) return 0;
  *vlen = 0;
  
  // no key is array mode, klen provides requested index
  if(!key)
  {
    if(klen < 0) return 0;
    index = klen;
    klen = 0;
  }else{
    if(klen <= 0) klen = strlen(key); // convenience
  }

  for(start=cur=json,end=cur+jlen; cur<end; cur++)
  {
      goto *go[(unsigned char)*cur];
      l_loop:;
  }
  
  if(depth) *vlen = jlen; // incomplete
  return 0;
  
  l_bad:
    *vlen = cur - json; // where error'd
    return 0;
  
  l_up:
    PUSH(0);
    ++depth;
    goto l_loop;

  l_down:
    --depth;
    CAP(0);
    goto l_loop;

  l_qup:
    PUSH(1);
    go=gostring;
    goto l_loop;

  l_qdown:
    CAP(-1);
    go=gostruct;
    goto l_loop;
    
  l_esc:
    go = goesc;
    goto l_loop;
    
  l_unesc:
    go = gostring;
    goto l_loop;

  l_bare:
    PUSH(0);
    go = gobare;
    goto l_loop;

  l_unbare:
    CAP(-1);
    go = gostruct;
    goto *go[(unsigned char)*cur];

  l_utf8_2:
    go = goutf8_continue;
    utf8_remain = 1;
    goto l_loop;

  l_utf8_3:
    go = goutf8_continue;
    utf8_remain = 2;
    goto l_loop;

  l_utf8_4:
    go = goutf8_continue;
    utf8_remain = 3;
    goto l_loop;

  l_utf_continue:
    if (!--utf8_remain)
      go=gostring;
    goto l_loop;

}

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#pragma GCC diagnostic pop
#endif