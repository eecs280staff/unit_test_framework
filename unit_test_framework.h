#ifndef UNIT_TEST_FRAMEWORK_H
#define UNIT_TEST_FRAMEWORK_H

#include <map>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <memory>

// Place the following line of code in your test file to generate a
// main() function:
// TEST_MAIN();

using Test_func_t = void (*)();

std::string operator"" _s(const char* str, size_t);

#define TEST(name) \
    void name(); \
    static TestRegisterer register_ ## name((#name), name); \
    void name()

#define TEST_MAIN() \
    int main(int argc, char** argv) { \
        for (int i = 1; i < argc; ++i) { \
            if (argv[i] == "--show_test_names"_s) { \
                TestSuite::get().print_test_names(std::cout); \
                std::cout << std::flush; \
                return 0; \
            } \
        } \
        return TestSuite::get().run_tests(argc, argv); \
    }


class TestSuite
{
public:
    static TestSuite& get() {
        if (not instance) {
            instance = new TestSuite;
        }
        return *instance;
    }

    void add_test(const std::string& test_name, Test_func_t test) {
        tests_[test_name] = test;
    }

    int run_tests(int argc, char** argv);

    std::ostream& print_test_names(std::ostream& os) {
        for (const auto& test_pair : tests_) {
            os << test_pair.first << '\n';
        }
        return os;
    }

    friend class TestSuiteDestroyer;

private:
    TestSuite() {}
    TestSuite(const TestSuite&) = delete;
    bool operator=(const TestSuite&) = delete;
    ~TestSuite() {}

    static TestSuite* instance;
    std::map<std::string, Test_func_t> tests_;
};

class TestSuiteDestroyer {
public:
    ~TestSuiteDestroyer() {
        delete TestSuite::instance;
    }
};

class TestRegisterer {
public:
    TestRegisterer(const std::string& test_name, Test_func_t test) {
        TestSuite::get().add_test(test_name, test);
    }
};

class TestFailure
{
public:
    TestFailure(std::string reason, int line_number)
        : reason_m(move(reason)), line_number_m(line_number)
    {}

    std::ostream& print(std::ostream& os) const
    {
        os << "FAIL\n"
           << "In test " << test_name_m << ", line " << line_number_m << ": \n"
           << reason_m << '\n';
        return os;
    }

    void set_test_name(std::string test_name)
    {
        test_name_m = move(test_name);
    }

private:
    std::string test_name_m;
    std::string reason_m;
    int line_number_m;
};
std::ostream& operator<<(std::ostream& os, const TestFailure& test_failure);

template<typename First, typename Second>
void assert_equal(
    First first, Second second, int line_number);
template<typename First, typename Second>
void assert_not_equal(
    First first, Second second, int line_number);

void assert_true(bool value, int line_number);
void assert_false(bool value, int line_number);

void assert_almost_equal(double first, double second, double precision,
                         int line_number);

#define ASSERT_EQUAL(first, second) \
    assert_equal((first), (second), __LINE__);

#define ASSERT_NOT_EQUAL(first, second) \
    assert_not_equal((first), (second), __LINE__);

#define ASSERT_TRUE(value) \
    assert_true((value), __LINE__);

#define ASSERT_FALSE(value) \
    assert_false((value), __LINE__);

#define ASSERT_ALMOST_EQUAL(first, second, precision) \
    assert_almost_equal((first), (second), (precision), __LINE__);

template<typename First, typename Second>
void assert_equal(First first, Second second, int line_number)
{
    if (first == second)
    {
        return;
    }
    std::ostringstream reason;
    reason << first << " != " << second;
    throw TestFailure(reason.str(), line_number);
}

template<typename First, typename Second>
void assert_not_equal(
    First first, Second second, int line_number)
{
    if (first != second)
    {
        return;
    }
    std::ostringstream reason;
    reason << "Values unexpectedly equal: " << first << " == " << second;
    throw TestFailure(reason.str(), line_number);
}


#endif
