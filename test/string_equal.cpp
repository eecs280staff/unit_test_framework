#include <string>
#include "unit_test_framework.h"

TEST(string_equal) {
  std::string s1 = "hello";
  ASSERT_EQUAL(s1, "hello");
  ASSERT_EQUAL("hello", s1);
  char cs[] = "hello";
  ASSERT_EQUAL(s1, cs);
  ASSERT_EQUAL(cs, s1);

  std::string s2 = "world";
  ASSERT_NOT_EQUAL(s2, "hello");
  ASSERT_NOT_EQUAL("hello", s2);
  ASSERT_NOT_EQUAL(s2, cs);
  ASSERT_NOT_EQUAL(cs, s2);
}

TEST_MAIN()
