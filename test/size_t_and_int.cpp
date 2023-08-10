#include "unit_test_framework.hpp"

TEST(size_t_and_int) {
  std::size_t s = 3u;
  int i1 = -1;
  int i2 = 3;
  ASSERT_NOT_EQUAL(s, i1);
  ASSERT_NOT_EQUAL(i1, s);
  ASSERT_EQUAL(s, i2);
  ASSERT_EQUAL(i2, s);
}

TEST_MAIN()
