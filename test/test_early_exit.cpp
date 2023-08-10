#include "unit_test_framework.hpp"

TEST(early_exit) {
    std::exit(0);
}

TEST(should_fail) {
    ASSERT_EQUAL(42, 43);
}

TEST(throws_error) {
    throw std::runtime_error("errorrr!");
}

TEST_MAIN()
