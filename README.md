Unit Test Framework
===================

[![`main` Build Status](https://travis-ci.com/eecs280staff/unit_test_framework.svg?branch=main)](https://travis-ci.com/eecs280staff/unit_test_framework)

A lightweight, easy to use, C++11 unit testing framework implemented in a single `.hpp` file.

Publicly available tutorial: [https://eecs280staff.github.io/unit_test_framework/](https://eecs280staff.github.io/unit_test_framework/)


## Quickstart
Install the unit test framework by copying `unit_test_framework.hpp` into your project.
```console
$ wget https://raw.githubusercontent.com/eecs280staff/unit_test_framework/main/unit_test_framework.hpp
```

Get the unit test example.
```console
$ wget https://raw.githubusercontent.com/eecs280staff/unit_test_framework/my_tests.cpp
```

Compile and run.
```console
$ g++ --std=c++11 my_tests.cpp -o my_tests.exe
$ ./my_tests.exe
Running test: bool_is_true
PASS
Running test: numbers_equal
PASS

*** Results ***
** Test case 'bool_is_true': PASS
** Test case 'numbers_equal': PASS
*** Summary ***
Out of 2 tests run:
0 failure(s), 0 error(s)
```

The example `my_tests.cpp` looks like this:
```c++
// File: my_tests.cpp

#include "unit_test_framework.hpp"

// TEST takes in one argument: the name of the test case.
// Note that the name of the test case must be a valid function name in C++.
TEST(numbers_equal) {
    int spam = 42;
    ASSERT_EQUAL(spam, 42);  // If spam != 42, this test case will fail
}

TEST(bool_is_true) {
    bool egg = true;
    ASSERT_TRUE(egg);  // If egg is not true, this test case will fail
}

// DO NOT REMOVE
// Generates a main() function that runs all of your tests.
// Note: Some versions of g++ incorrectly produce a warning about empty
// statements when using the -pedantic flag. Therefore, we will not put
// a semicolon after the TEST_MAIN() macro.
TEST_MAIN()
```


## How to write a test case: The `TEST()` macro
Test cases can be declared using the special `TEST()` macro defined in `unit_test_framework.hpp`:
```c++
TEST(<test_name>) {
    // test case code
}
```
Replace `<test_name>` with the name of the test case. Note that `<test_name>` must be a valid C++ function name.
```c++
TEST(test_a_thing) {
    // test case code
}
```

## Special Asserts
You can use any amount of these special asserts in your test cases. You can also use the special asserts in helper functions.

`ASSERT_EQUAL(first, second)`
* If `first == second` evaluates to false, the test will fail.
* `first` and `second` may be any type

`ASSERT_NOT_EQUAL(first, second)`
* If `first != second` evaluates to false, the test will fail.
* `first` and `second` may be any type

`ASSERT_TRUE(bool value)`
* If `value` is false, the test will fail.

`ASSERT_FALSE(bool value)`
* If `value` is true, the test will fail.

`ASSERT_ALMOST_EQUAL(double first, double second, double precision)`
* If `first` and `second` are not equal within `precision`, the test will fail.

`ASSERT_SEQUENCE_EQUAL(first, second)`
* If `first` and `second` do not have equal elements, the test will fail.
* `first` and `second` may be any sequence types (e.g. arrays, vectors, lists), as long as their elements are comparable with `==`.

## Command line options
```console
$ ./my_tests.exe -h
usage: ./my_tests.exe [-h] [-n] [-q] [[TEST_NAME] ...]
optional arguments:
 -h, --help		 show this help message and exit
 -n, --show_test_names	 print the names of all discovered test cases and exit
 -q, --quiet		 print a reduced summary of test results
 TEST_NAME ...		 run only the test cases whose names are listed here. Note: If no test names are specified, all discovered tests are run by default.
```

```console
$ ./my_tests.exe --show_test_names
bool_is_true
numbers_equal
```

```console
$ ./my_tests.exe --quiet
*** Results ***
bool_is_true: PASS
numbers_equal: PASS
```

```console
$ ./my_tests.exe bool_is_true
Running test: bool_is_true
PASS

*** Results ***
** Test case 'bool_is_true': PASS
*** Summary ***
Out of 1 tests run:
0 failure(s), 0 error(s)
```

### Enabling regular expressions
By default, the unit test framework looks for exact matches to the
test names supplied at the command line. Matching via regular
expressions can be enabled by defining the `UNIT_TEST_ENABLE_REGEXP`
macro to 1 prior to #including `unit_test_framework.hpp` (or when
compiling the test executable, such as with
`-DUNIT_TEST_ENABLE_REGEXP` for GCC or Clang), and then passing the
`-e` flag to the test executable.

```console
$ g++ -std=c++11 -DUNIT_TEST_ENABLE_REGEXP -o my_tests.exe my_tests.cpp
$ ./my_tests.exe -h
usage: ./my_tests.exe [-h] [-e] [-n] [-q] [[TEST_NAME] ...]
optional arguments:
 -h, --help		 show this help message and exit
 -e, --regexp		 treat TEST_NAME as a regular expression
 -n, --show_test_names	 print the names of all discovered test cases and exit
 -q, --quiet		 print a reduced summary of test results
 TEST_NAME ...		 run only the test cases whose names are listed here. Note: If no test names are specified, all discovered tests are run by default.
```

```console
$ ./my_tests.exe -e "bool.*"
Running test: bool_is_true
PASS

*** Results ***
** Test case "bool_is_true": PASS
*** Summary ***
Out of 1 tests run:
0 failure(s), 0 error(s)
```
