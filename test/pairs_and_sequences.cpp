#include <map>
#include <string>
#include <vector>
#include <utility>
#include "unit_test_framework.h"

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

TEST_MAIN()
