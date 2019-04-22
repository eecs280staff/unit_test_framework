#include "unit_test_framework.h"

TEST(size_t_and_int) {
  std::size_t s = 3u;
  int i1 = -1;
  int i2 = 3;
  ASSERT_NOT_EQUAL(s, i1);
  ASSERT_EQUAL(i2, s);
}

TEST_MAIN()
