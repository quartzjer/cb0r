#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cb0r.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
  #include "cb0r_zen.c"
  fail_unless(b0go);

  #include "cb0r_init.c"
  fail_unless(itest);

  #include "cb0r_next.c"
  fail_unless(ntest);

  #include "cb0r_value.c"
  fail_unless(vtest);

  return 0;
}

