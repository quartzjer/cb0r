// by jeremie miller - 2015
// public domain, contributions/improvements welcome via github at https://github.com/quartzjer/cb0r

#include "cb0r.h"

size_t cb0r(uint8_t *raw, size_t len, uint8_t *type, uint64_t *val, size_t *start)
{
  uint64_t v = 0;
  size_t data = 0;
  size_t next = 0;

  if(!raw || !len || !type) return 0;
  *type = CB0R_ERR;

  // TODO!
  /* from the spec http://tools.ietf.org/html/rfc7049#appendix-C
  well_formed (breakable = false) {
     // process initial bytes
     ib = uint(take(1));
     mt = ib >> 5;
     val = ai = ib & 0x1f;
     switch (ai) {
       case 24: val = uint(take(1)); break;
       case 25: val = uint(take(2)); break;
       case 26: val = uint(take(4)); break;
       case 27: val = uint(take(8)); break;
       case 28: case 29: case 30: fail();
       case 31:
         return well_formed_indefinite(mt, breakable);
     }
     // process content
     switch (mt) {
       // case 0, 1, 7 do not have content; just use val
       case 2: case 3: take(val); break; // bytes/UTF-8
       case 4: for (i = 0; i < val; i++) well_formed(); break;
       case 5: for (i = 0; i < val*2; i++) well_formed(); break;
       case 6: well_formed(); break;     // 1 embedded data item
     }
     return mt;                    // finite data item
   }

   well_formed_indefinite(mt, breakable) {
     switch (mt) {
       case 2: case 3:
         while ((it = well_formed(true)) != -1)
           if (it != mt)           // need finite embedded
             fail();               //    of same type
         break;
       case 4: while (well_formed(true) != -1); break;
       case 5: while (well_formed(true) != -1) well_formed(); break;
       case 7:
         if (breakable)
           return -1;              // signal break out
         else fail();              // no enclosing indefinite
       default: fail();            // wrong mt
     }
     return 0;                     // no break out
   }
  */

  // if given a place to copy results, return them
  if(val) *val = v;
  if(start) *start = data;

  // where's the next value
  return next;
}
