#include "unit_test_framework.h"

TEST(should_fail) {
    ASSERT_EQUAL(42, 43);
}

TEST(should_pass) {
    ASSERT_EQUAL(43, 43);
}

TEST_MAIN();
