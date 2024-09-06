#include <vector>
#include "tutorial.hpp"
#include "unit_test_framework.hpp"

using namespace std;

TEST(test_slide_right_1) {
  vector<int> v = { 4, 0, 1, 3, 3 };
  vector<int> expected = { 3, 4, 0, 1, 3 };
  slideRight(v);
  ASSERT_SEQUENCE_EQUAL(v, expected);
}

TEST(test_flip_1) {
  vector<int> v = { 4, 0, 1, 3, 3 };
  vector<int> expected = { 3, 3, 1, 0, 4 };
  flip(v);
  ASSERT_SEQUENCE_EQUAL(v, expected);
}

TEST_MAIN() // No semicolon!