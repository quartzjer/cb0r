#include <stdlib.h>
#include <string.h>
#include "../src/cb0r.h"
#include "unit_test.h"

int main(int argc, char **argv)
{
  fail_unless(cb0r(NULL,0,NULL,NULL,NULL) == 0);
  fail_unless(0);

  return 0;
}

