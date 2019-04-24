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

TEST(assert_almost_equal_precision_round) {
    ASSERT_ALMOST_EQUAL(.75, .5, .1);
}

TEST(assert_almost_equal_precision_infinite) {
    ASSERT_ALMOST_EQUAL(.2 + .1, .3, 0);
}

TEST_MAIN();
