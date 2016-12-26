#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cb0r.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
  uint8_t *b0_start = NULL;
  uint8_t *b0_end = NULL;

  uint32_t b0_count = -1;
  cb0r_t b0_type = CB0R_END;
  
  #include "cb0r_zen.c"
  fail_unless(b0_go);

  uint32_t b0_length = 1;
  uint8_t b0_bin[] = {0x01};

  #include "cb0r_init.c"
  fail_unless(b0_start == b0_bin);
  fail_unless(b0_end == b0_bin+b0_length);
  fail_unless(b0_count == 0);
  fail_unless(b0_type == CB0R_ERR);

  #include "cb0r_next.c"
  fail_unless(b0_type == CB0R_INT);
  fail_unless(b0_count == 1);

  uint32_t b0_value = 0;
  #include "cb0r_value.c"
  fail_unless(b0_value == 1);

  return 0;
}

