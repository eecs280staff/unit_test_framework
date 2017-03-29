#include "unit_test_framework.h"

#include <iostream>
#include <cassert>

using namespace std;


void test_assert_equal();
void test_assert_not_equal();
void test_assert_true();
void test_assert_false();
void test_assert_almost_equal();


#define ASSERT_TEST_FAILURE_RAISED(expr) \
    try { \
        expr; \
        cout << "TestFailure not raised" << endl; \
        assert(false); \
    } \
    catch (TestFailure& e) {}



int main() {
    test_assert_equal();
    test_assert_not_equal();
    test_assert_true();
    test_assert_false();
    test_assert_almost_equal();

    cout << "All tests passed" << endl;
}


void test_assert_equal() {
    cout << "test_assert_equal" << endl;
    int i = 42;

    ASSERT_EQUAL(42, 42);
    ASSERT_EQUAL(42.5, 42.5);
    ASSERT_EQUAL(string("spam"), string("spam"));
    ASSERT_EQUAL(&i, &i);
    ASSERT_EQUAL(true, true);

    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_EQUAL(42, 3)
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_EQUAL(42.6, 42.5)
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_EQUAL(string("spam"), string("EGG"))
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_EQUAL(&i + 1, &i)
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_EQUAL(true, false)
    );
}

void test_assert_not_equal() {
    cout << "test_assert_not_equal" << endl;

    int i = 42;

    ASSERT_NOT_EQUAL(42, 43);
    ASSERT_NOT_EQUAL(42.5, 42.6);
    ASSERT_NOT_EQUAL(string("spam"), string("egg"));
    ASSERT_NOT_EQUAL(&i, &i + 1);
    ASSERT_NOT_EQUAL(true, false);

    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_NOT_EQUAL(42, 42)
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_NOT_EQUAL(42.5, 42.5)
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_NOT_EQUAL(string("spam"), string("spam"))
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_NOT_EQUAL(&i, &i)
    );
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_NOT_EQUAL(true, true)
    );
}

void test_assert_true() {
    cout << "test_assert_true" << endl;

    ASSERT_TRUE(true);
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_TRUE(false)
    );
}

void test_assert_false() {
    cout << "test_assert_false" << endl;

    ASSERT_FALSE(false);
    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_FALSE(true)
    );
}

void test_assert_almost_equal() {
    cout << "test_assert_almost_equal" << endl;

    ASSERT_ALMOST_EQUAL(42.1, 42.2, 0.2);
    ASSERT_ALMOST_EQUAL(0.2 + 0.1, 0.3, 0.01);

    ASSERT_TEST_FAILURE_RAISED(
        ASSERT_ALMOST_EQUAL(42.1, 42.3, 0.1)
    );
}
