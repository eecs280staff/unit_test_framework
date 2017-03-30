#include "unit_test_framework.h"

#include <stdexcept>

TEST(should_fail) {
    ASSERT_EQUAL(42, 43);
}

TEST(should_pass) {
    ASSERT_EQUAL(43, 43);
}

TEST(throws_error) {
    throw std::runtime_error("errorrr!");
}

TEST_MAIN();
