#include "unit_test_framework.h"

#include <vector>
#include <exception>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

string operator"" _s(const char* str, size_t) {
    return string(str);
}


// THIS IS PART OF A WORKAROUND TO DEAL WITH STATIC
// INITIALIZATION SHENANIGANS.
// DO NOT CHANGE THIS UNLESS YOU REEEEALLY KNOW WHAT
// YOU'RE DOING.
TestSuite* TestSuite::instance = &TestSuite::get();

static TestSuiteDestroyer destroyer;


int TestSuite::run_tests(int argc, char** argv)
{
    vector<TestFailure> failures;
    vector<exception> errors;

    vector<string> tests_to_run;
    if (argc == 1)
    {
        transform(begin(tests_), end(tests_),
                  back_inserter(tests_to_run),
                  [] (pair<string, Test_func_t> p) { return p.first; });
    }
    else
    {
        for (auto i = 1; i < argc; ++i)
        {
            tests_to_run.push_back(argv[i]);
        }
    }

    for (auto test_name : tests_to_run)
    {
        cout << test_name << endl;
        if (tests_.find(test_name) == end(tests_))
        {
            cerr << "Test " << test_name << " not found" << endl;
            return 1;
        }
    }

    for (auto test_name : tests_to_run)
    {
        try
        {
            cout << "*** Running test: " << test_name << " ***" << endl;
            (tests_.at(test_name))();
            cout << "PASS" << endl;
        }
        catch (TestFailure& e)
        {
            e.set_test_name(test_name);
            failures.push_back(e);
            cout << "FAIL" << endl;
        }
        catch (exception& e)
        {
            errors.push_back(e);
            cout << "\nError in test " << test_name << ": " << endl;
            cout << e.what() << endl;
        }
    }

    cout << endl;

    for (auto failure : failures)
    {
        cout << "--------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------" << endl;
        cout << "--------------------------------------------------------" << endl;

        cout << failure << endl;
    }

    cout << "--------------------------------------------------------" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "--------------------------------------------------------" << endl;


    cout << "Out of " << tests_to_run.size() << " tests run:" << endl;
    cout << failures.size() << " failure(s), "
         << errors.size() << " error(s)" << endl;

    return (failures.size() == 0 and errors.size() == 0) ? 0 : 1;
}

ostream& operator<<(ostream& os, const TestFailure& test_failure)
{
    return test_failure.print(os);
}


//------------------------------------------------------------------------------


void assert_true(bool value, int line_number)
{
    if (value)
    {
        return;
    }
    ostringstream reason;
    reason << "Expected true, but was false";
    throw TestFailure(reason.str(), line_number);
}

void assert_false(bool value, int line_number)
{
    if (not value)
    {
        return;
    }
    ostringstream reason;
    reason << "Expected false, but was true";
    throw TestFailure(reason.str(), line_number);
}

void assert_almost_equal(double first, double second, double precision,
                         int line_number)
{
    if (abs(first - second) <= precision)
    {
        return;
    }
    std::ostringstream reason;
    reason << "Values too far apart: " << first << " and " << second;
    throw TestFailure(reason.str(), line_number);
}
