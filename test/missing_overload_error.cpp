#include "unit_test_framework.h"

struct A {
  bool operator==(const A &rhs) const {
    return true;
  }
};

TEST(foo) {
  ASSERT_EQUAL(A(), A());
  ASSERT_NOT_EQUAL(A(), A());
}

TEST_MAIN()
