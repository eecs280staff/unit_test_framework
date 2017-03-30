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
// YOU'RE DOING.
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
        if (argv[i] == std::string("--show_test_names")) {
            TestSuite::get().print_test_names(std::cout);
            std::cout << std::flush;
            throw ExitSuite();
        }
        else if (argv[i] == std::string("--quiet") or
                 argv[i] == std::string("-q")) {
            TestSuite::get().enable_quiet_mode();
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
    std::ostringstream reason;
    reason << "Values too far apart: " << first << " and " << second;
    throw TestFailure(reason.str(), line_number);
}
