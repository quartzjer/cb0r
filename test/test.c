#include <stdlib.h>
#include <string.h>
#include "cb0r.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
  #include "cb0r_zen.c"
  fail_unless(b0go);

  #include "cb0r_next.c"
  fail_unless(ntest);

  #include "cb0r_value.c"
  fail_unless(vtest);

  fail_unless(0);

  return 0;
}

