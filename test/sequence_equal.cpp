#include <vector>
#include "unit_test_framework.h"

TEST(array_equal) {
  int a[] = { 1, 2 };
  int b[] = { 1, 2 };
  ASSERT_SEQUENCE_EQUAL(a, b);
}

TEST(array_vector_equal) {
  int a[] = { 1, 2 };
  std::vector<double> b = { 1.0, 2.0 };
  ASSERT_SEQUENCE_EQUAL(a, b);
  ASSERT_SEQUENCE_EQUAL(b, a);
}

TEST(array_unequal) {
  int a[] = { 1, 2 };
  int b[] = { 1, 3 };
  ASSERT_SEQUENCE_EQUAL(a, b);
}

TEST(array_vector_unequal) {
  int a[] = { 1, 2 };
  std::vector<double> b = { 1.0, 2.0, -2.1 };
  ASSERT_SEQUENCE_EQUAL(a, b);
  ASSERT_SEQUENCE_EQUAL(b, a);
}

TEST_MAIN()
