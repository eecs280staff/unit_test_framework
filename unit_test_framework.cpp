#include "unit_test_framework.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

// THIS IS PART OF A WORKAROUND TO DEAL WITH STATIC
// INITIALIZATION SHENANIGANS.
// DO NOT CHANGE THIS UNLESS YOU REEEEALLY KNOW WHAT
// YOU'RE DOING. CONTACT akamil@umich.edu or jameslp@umich.edu IF
// YOU HAVE QUESTIONS ABOUT THIS.
TestSuite* TestSuite::instance = &TestSuite::get();

static TestSuiteDestroyer destroyer;

void TestCase::run(bool quiet_mode) {
    try {
        if (not quiet_mode) {
            cout << "Running test: " << name << endl;
        }

        test_func();

        if (not quiet_mode) {
            cout << "PASS" << endl;
        }
    }
    catch (TestFailure& failure) {
        failure_msg = failure.to_string();

        if (not quiet_mode) {
            cout << "FAIL" << endl;
        }
    }
    catch (exception& e) {
        ostringstream oss;
        oss << "Uncaught exception in test " << name << ": " << endl;
        oss << e.what() << endl;
        exception_msg = oss.str();

        if (not quiet_mode) {
            cout << "ERROR" << endl;
        }
    }
}

void TestCase::print(bool quiet_mode) {
    if (quiet_mode) {
        cout << name << ": ";
    }
    else {
        cout << "** Test case '" << name << "': ";
    }

    if (not failure_msg.empty()) {
        cout << "FAIL" << endl;
        if (not quiet_mode) {
            cout << failure_msg << endl;
        }
    }
    else if (not exception_msg.empty()) {
        cout << "ERROR" << endl;
        if (not quiet_mode) {
            cout << exception_msg << endl;
        }
    }
    else {
        cout << "PASS" << endl;
    }
}

// ----------------------------------------------------------------------------

class ExitSuite : public exception {
public:
    ExitSuite(int status_ = 0) : status(status_) {}
    int status;
};

int TestSuite::run_tests(int argc, char** argv) {
    vector<string> test_names_to_run;
    try {
        test_names_to_run = get_test_names_to_run(argc, argv);
    }
    catch (ExitSuite& e) {
        return e.status;
    }

    for (auto test_name : test_names_to_run) {
        if (tests_.find(test_name) == end(tests_)) {
            throw runtime_error("Test " + test_name + " not found");
        }
    }

    for (auto test_name : test_names_to_run) {
        tests_.at(test_name).run(quiet_mode);
    }

    cout << "\n*** Results ***" << endl;
    for (auto test_name : test_names_to_run) {
        tests_.at(test_name).print(quiet_mode);
    }

    auto num_failures = count_if(
        tests_.begin(), tests_.end(), [](pair<string, TestCase> test_pair) {
            return not test_pair.second.failure_msg.empty();
        });
    auto num_errors = count_if(
        tests_.begin(), tests_.end(), [](pair<string, TestCase> test_pair) {
            return not test_pair.second.exception_msg.empty();
        });

    if (not quiet_mode) {
        cout << "*** Summary ***" << endl;
        cout << "Out of " << test_names_to_run.size() << " tests run:" << endl;
        cout << num_failures << " failure(s), " << num_errors << " error(s)"
             << endl;
    }

    if (num_failures == 0 and num_errors == 0) {
        return 0;
    }
    return 1;
}

vector<string> TestSuite::get_test_names_to_run(int argc, char** argv) {
    vector<string> test_names_to_run;
    for (auto i = 1; i < argc; ++i) {
        if (argv[i] == string("--show_test_names") or
            argv[i] == string("-n")) {

            TestSuite::get().print_test_names(cout);
            cout << flush;
            throw ExitSuite();
        }
        else if (argv[i] == string("--quiet") or argv[i] == string("-q")) {
            TestSuite::get().enable_quiet_mode();
        }
        else if (argv[i] == string("--help") or argv[i] == string("-h")) {
            cout << "usage: " << argv[0]
                 << " [-h] [-n] [-q] [[TEST_NAME] ...]\n";
            cout
                << "optional arguments:\n"
                << " -h, --help\t\t show this help message and exit\n"
                << " -n, --show_test_names\t print the names of all "
                   "discovered test cases and exit\n"
                << " -q, --quiet\t\t print a reduced summary of test results\n"
                << " TEST_NAME ...\t\t run only the test cases whose names "
                   "are "
                   "listed here. Note: If no test names are specified, all "
                   "discovered tests are run by default."
                << endl;

            throw ExitSuite();
        }
        else {
            test_names_to_run.push_back(argv[i]);
        }
    }

    if (test_names_to_run.empty()) {
        transform(begin(tests_), end(tests_), back_inserter(test_names_to_run),
                  [](const pair<string, TestCase>& p) { return p.first; });
    }
    return test_names_to_run;
}

ostream& operator<<(ostream& os, const TestFailure& test_failure) {
    return test_failure.print(os);
}

//------------------------------------------------------------------------------

#if defined(__clang__) || defined(__GLIBCXX__) || defined(__GLIBCPP__)
#include <cxxabi.h>
#include <cstdlib>
string demangle(const char* typeinfo_name) {
    int status = 0;
    char* demangled =
        abi::__cxa_demangle(typeinfo_name, nullptr, nullptr, &status);
    if (status == 0) {
        string result = demangled;
        std::free(demangled);
        return result;
    }
    else {
        return typeinfo_name;
    }
}
#else
string demangle(const char* typeinfo_name) {
    return typeinfo_name;
}
#endif  // defined(__clang__) || defined(__GLIBCXX__) || defined(__GLIBCPP__)

//------------------------------------------------------------------------------

void assert_true(bool value, int line_number) {
    if (value) {
        return;
    }
    ostringstream reason;
    reason << "Expected true, but was false";
    throw TestFailure(reason.str(), line_number);
}

void assert_false(bool value, int line_number) {
    if (not value) {
        return;
    }
    ostringstream reason;
    reason << "Expected false, but was true";
    throw TestFailure(reason.str(), line_number);
}

void assert_almost_equal(double first, double second, double precision,
                         int line_number) {
    if (abs(first - second) <= precision) {
        return;
    }
    ostringstream reason;
    // For now, we'll just set the precision arbitrarily high.
    // In the future, we may decide to add an option to configure
    // the output precision.
    reason.precision(20);
    reason << "Values too far apart: " << first << " and " << second;
    throw TestFailure(reason.str(), line_number);
}
