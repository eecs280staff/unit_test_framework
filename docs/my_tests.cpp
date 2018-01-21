// File: my_tests.cpp

#include "unit_test_framework.h"

// TEST takes in one argument: the name of the test case.
// Note that the name of the test case must be a valid function name in C++.
TEST(numbers_equal) {
    int spam = 42;
    ASSERT_EQUAL(spam, 42);  // If spam != 42, this test case will fail
}

TEST(bool_is_true) {
    bool egg = true;
    ASSERT_TRUE(egg);  // If egg is not true, this test case will fail
}

// DO NOT REMOVE
// Generates a main() function that runs all of your tests.
// Note: Some versions of g++ incorrectly produce a warning about empty
// statements when using the -pedantic flag. Therefore, we will not put
// a semicolon after the TEST_MAIN() macro.
TEST_MAIN()
