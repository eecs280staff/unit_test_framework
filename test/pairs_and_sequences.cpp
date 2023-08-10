#include <map>
#include <string>
#include <vector>
#include <utility>
#include "unit_test_framework.hpp"

using namespace std;

TEST(vector_print) {
    vector<int> v1;
    vector<int> v2 = { 3, 4 };
    ASSERT_EQUAL(v1, v2);
}

TEST(pair_print) {
    pair<string, int> p1 = { "hello", 3 };
    pair<string, int> p2 = { "hello", 4 };
    ASSERT_EQUAL(p1, p2);
}

TEST(map_print) {
    map<string, int> m1;
    map<string, int> m2;
    m1["hello"] = 3;
    m2["hello"] = 4;
    m1["world"] = -1;
    m2["world"] = -1;
    ASSERT_EQUAL(m1, m2);
}

TEST(nested_vector_print) {
  vector<vector<double>> vs1 = { { 1.1, 2.3 }, { -4.6 } };
  vector<vector<double>> vs2 = { { 1.1, 2.3 }, { -4.6, 8.7 } };
  ASSERT_EQUAL(vs1, vs2);
}

TEST(nested_pair_print) {
  map<pair<string, string>, int> m1;
  map<pair<string, string>, int> m2;
  m1[{ "hello", "world" }] = 3;
  m1[{ "foo", "bar" }] = -1;
  m2[{ "hello", "world" }] = 3;
  m2[{ "bar", "foo" }] = -1;
  ASSERT_EQUAL(m1, m2);
}

TEST_MAIN()
