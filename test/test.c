#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cb0r.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
  struct cb0r_s res_s = {0,};
  cb0r_t res = &res_s;
  uint8_t *end = NULL;
  uint8_t start[16] = {0,};

  start[0] = 0x01;
  end = cb0r(start, start+1, 0, res);
  fail_unless(end == start+1);
  fail_unless(res->type == CB0R_INT);
  fail_unless(res->value == 1);

  start[0] = 0x17;
  end = cb0r(start, start+1, 0, res);
  fail_unless(end == start+1);
  fail_unless(res->type == CB0R_INT);
  fail_unless(res->value == 23);

  return 0;
}

