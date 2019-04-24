#ifndef UNIT_TEST_FRAMEWORK_H
#define UNIT_TEST_FRAMEWORK_H

#include <map>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <memory>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <cstdlib>
#include <iterator>

// For compatibility with Visual Studio
#include <ciso646>

// Place the following line of code in your test file to generate a
// main() function:
// TEST_MAIN()

using Test_func_t = void (*)();


#define TEST(name)                                                            \
    static void name();                                                       \
    static TestRegisterer register_##name((#name), name);                     \
    static void name()

#define TEST_MAIN()                                                           \
    int main(int argc, char** argv) {                                         \
        return TestSuite::get().run_tests(argc, argv);                        \
    }


struct TestCase {
    TestCase(std::string name_, Test_func_t test_func_)
        : name(name_), test_func(test_func_) {}

    void run(bool quiet_mode);
    void print(bool quiet_mode);

    std::string name;
    Test_func_t test_func;
    std::string failure_msg{};
    std::string exception_msg{};
};


class TestSuite {
public:
    static TestSuite& get() {
        if (not instance) {
            instance = new TestSuite;
        }
        return *instance;
    }

    void add_test(const std::string& test_name, Test_func_t test) {
        tests_.insert({test_name, TestCase{test_name, test}});
    }

    int run_tests(int argc, char** argv);
    void print_results();

    void enable_quiet_mode() {
        quiet_mode = true;
    }

    std::ostream& print_test_names(std::ostream& os) {
        for (const auto& test_pair : tests_) {
            os << test_pair.first << '\n';
        }
        return os;
    }

    friend class TestSuiteDestroyer;

private:
    TestSuite() {
        auto func = []() {
            if (TestSuite::instance->incomplete) {
                std::cout << "ERROR: premature call to exit()" << std::endl;
                std::abort();
            }
        };
        std::atexit(func);
#ifdef _GLIBCXX_HAVE_AT_QUICK_EXIT
        std::at_quick_exit(func);
#endif
    }
    TestSuite(const TestSuite&) = delete;
    bool operator=(const TestSuite&) = delete;
    ~TestSuite() {}

    std::vector<std::string> get_test_names_to_run(int argc, char** argv);

    static TestSuite* instance;
    std::map<std::string, TestCase> tests_;

    bool quiet_mode = false;
    bool incomplete = false;
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

class TestFailure {
public:
    TestFailure(std::string reason, int line_number)
        : reason_m(move(reason)), line_number_m(line_number) {}

    std::ostream& print(std::ostream& os) const {
        os << "In test " << test_name_m << ", line " << line_number_m << ": \n"
           << reason_m << '\n';
        return os;
    }

    void set_test_name(std::string test_name) {
        test_name_m = move(test_name);
    }

    std::string to_string() const {
        std::ostringstream oss;
        print(oss);
        return oss.str();
    }

private:
    std::string test_name_m;
    std::string reason_m;
    int line_number_m;
};
std::ostream& operator<<(std::ostream& os, const TestFailure& test_failure);

// ----------------------------------------------------------------------------

// demangle, print_helper, and print contributed by Amir Kamil <akamil@umich.edu>

// Demangles a string produced by std::type_info::name.
std::string demangle(const char* typeinfo_name);

// forward declaration of print
template <class T>
std::ostream& print(std::ostream& os, const T& t);

// This version of print_helper will be called when T has an available
// stream insertion operator overload.
template <class T>
auto print_helper(std::ostream& os, const T& t, int, int)
    -> decltype(os << t)& {
    return os << t;
}

// This version of print_helper will be called when T is a pair.
template <class First, class Second>
auto print_helper(std::ostream& os, const std::pair<First, Second>& t, int,
                  int) -> decltype(print(os, t.first), print(os, t.second))& {
    os << '(';
    print(os, t.first);
    os << ',';
    print(os, t.second);
    return os << ')';
}

// This version of print_helper will be called when T is a sequence.
template <class Sequence>
auto print_helper(std::ostream& os, const Sequence& seq, int, ...)
    -> decltype(print(os, *seq.begin()), print(os, *seq.end()))& {
    if (seq.begin() == seq.end()) {
        return os << "{}";
    }

    auto it = seq.begin();
    os << "{ ";
    print(os, *it);
    for (++it; it != seq.end(); ++it) {
        os << ", ";
        print(os, *it);
    }
    return os << " }";
}

// This version of print_helper will be called when T is an array.
template <class Elem, std::size_t N>
std::ostream& print_helper(std::ostream& os, const Elem (&arr)[N], int, int) {
    if (N == 0) {
        return os << "{}";
    }

    auto it = std::begin(arr);
    os << "{ ";
    print(os, *it);
    for (++it; it != std::end(arr); ++it) {
        os << ", ";
        print(os, *it);
    }
    return os << " }";
}

// This version of print_helper will be called when T does not have an
// available stream insertion operator overload.
template <class T>
std::ostream& print_helper(std::ostream& os, const T&, ...) {
    return os << "<" << demangle(typeid(T).name()) << " object>";
}

// Attempts to print the given object to the given stream.
// If T has an available stream insertion operator overload, that
// operator is used. Otherwise, a generic representation of the object
// is printed to os.
template <class T>
std::ostream& print(std::ostream& os, const T& t) {
    // The extra parameters are needed so that the first overload of
    // print_helper is preferred, followed by the third one.
    return print_helper(os, t, 0, 0);
}

// ----------------------------------------------------------------------------

template <typename First, typename Second>
void assert_equal(First&& first, Second&& second, int line_number);
template <typename First, typename Second>
void assert_not_equal(First&& first, Second&& second, int line_number);

void assert_true(bool value, int line_number);
void assert_false(bool value, int line_number);

void assert_almost_equal(double first, double second, double precision,
                         int line_number);

#define ASSERT_EQUAL(first, second) assert_equal((first), (second), __LINE__);

#define ASSERT_NOT_EQUAL(first, second)                                       \
    assert_not_equal((first), (second), __LINE__);

#define ASSERT_SEQUENCE_EQUAL(first, second)                                  \
    assert_sequence_equal((first), (second), __LINE__);

#define ASSERT_TRUE(value) assert_true((value), __LINE__);

#define ASSERT_FALSE(value) assert_false((value), __LINE__);

#define ASSERT_ALMOST_EQUAL(first, second, precision)                         \
    assert_almost_equal((first), (second), (precision), __LINE__);

// Template logic to produce a static assertion failure when comparing
// incomparable types.
template <typename First, typename Second, typename = void>
struct is_equality_comparable : std::false_type {};

template <typename First, typename Second>
using enable_if_equality_comparable = typename std::enable_if<
    std::is_same<bool, decltype(std::declval<First>() ==
                                std::declval<Second>())>::value &&
        !std::is_array<typename std::remove_reference<First>::type>::value &&
        !std::is_array<typename std::remove_reference<Second>::type>::value,
    void>::type;

template <typename First, typename Second>
struct is_equality_comparable<First, Second,
                              enable_if_equality_comparable<First, Second>>
    : std::true_type {};

template <typename First, typename Second, typename = void>
struct safe_equals {
    static_assert(is_equality_comparable<First, Second>::value,
                  "types cannot be compared with ==");
    static bool equals(const First& first, const Second& second) {
        return first == second;
    }
};

// Specializations to allow size_t to be compared to int.
template <>
struct safe_equals<std::size_t, int> {
    static bool equals(std::size_t first, int second) {
        return static_cast<long long>(first) == second;
    }
};

template <>
struct safe_equals<int, std::size_t> {
    static bool equals(int first, std::size_t second) {
        return first == static_cast<long long>(second);
    }
};

template <typename First, typename Second>
void assert_equal(First&& first, Second&& second, int line_number) {
    if (safe_equals<First, Second>::equals(first, second)) {
        return;
    }
    std::ostringstream reason;
    print(reason, first);
    reason << " != ";
    print(reason, second);
    throw TestFailure(reason.str(), line_number);
}

template <typename First, typename Second>
void assert_not_equal(First&& first, Second&& second, int line_number) {
    if (not safe_equals<First, Second>::equals(first, second)) {
        return;
    }
    std::ostringstream reason;

    reason << "Values unexpectedly equal: ";
    print(reason, first);
    reason << " == ";
    print(reason, second);
    throw TestFailure(reason.str(), line_number);
}

template <typename First, typename Second>
void assert_sequence_equal(First&& first, Second&& second, int line_number) {
    using std::begin;
    using std::end;
    auto it1 = begin(first);
    auto it2 = begin(second);
    auto end1 = end(first);
    auto end2 = end(second);
    bool equal = true;
    for (; it1 != end1 and it2 != end2; ++it1, ++it2) {
        if (not safe_equals<decltype(*it1), decltype(*it2)>::equals(
                *it1, *it2)) {
            equal = false;
            break;
        }
    }

    if (not equal or it1 != end1 or
        it2 != end2) {  // different number of elements
        std::ostringstream reason;
        print(reason, first);
        reason << " != ";
        print(reason, second);
        throw TestFailure(reason.str(), line_number);
    }
}

#endif
