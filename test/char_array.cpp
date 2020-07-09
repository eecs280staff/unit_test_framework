#include <map>
#include <string>
#include <vector>
#include <utility>
#include "unit_test_framework.h"

using namespace std;

TEST(string_eq) {
    char arr[] = "spam";
    ASSERT_SEQUENCE_EQUAL("spam", "spam");
    ASSERT_SEQUENCE_EQUAL("spam", arr);
    ASSERT_SEQUENCE_EQUAL("spam", string("spam"));
    ASSERT_EQUAL("spam", string("spam"));
}

TEST(string_uneq1) {
    ASSERT_SEQUENCE_EQUAL("spam", "egg");
}

TEST(string_uneq2) {
    char arr[] = "egg";
    ASSERT_SEQUENCE_EQUAL("spam", arr);
}

TEST(string_uneq3) {
    ASSERT_SEQUENCE_EQUAL("spam", string("egg"));
}

TEST(string_uneq4) {
    ASSERT_EQUAL("spam", string("egg"));
}

TEST(string_uneq5) {
    char arr[] = {'e', 'g', 'g'};
    ASSERT_SEQUENCE_EQUAL("spam", arr);
}

TEST(char_array_eq) {
    char arr1[] = {'s', 'p', 'a', 'm'};
    char arr2[] = {'s', 'p', 'a', 'm'};
    ASSERT_SEQUENCE_EQUAL(arr1, arr2);
}

TEST(char_array_uneq1) {
    char arr1[] = {'s', 'p', 'a', 'm'};
    char arr2[] = {'e', 'g', 'g'};
    ASSERT_SEQUENCE_EQUAL(arr1, arr2);
}

TEST(char_array_uneq2) {
    char arr1[] = {'s', 'p', 'a', 'm'};
    char arr2[] = {'e', 'g', 'g', 's'};
    ASSERT_SEQUENCE_EQUAL(arr1, arr2);
}

TEST_MAIN()
