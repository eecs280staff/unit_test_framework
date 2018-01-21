# Simple C++ Unit Testing
This library is intended to be a lightweight C++ unit testing framework suitable for students to use in their programming projects. It consists of one `.h`/`.cpp` compilation unit, which makes it easy to get up and running.

Publicly available tutorial: [https://eecs280staff.github.io/unit_test_framework/](https://eecs280staff.github.io/unit_test_framework/)


## Dependencies
This library relies on C++11 features.

## Installation
Simply copy the files `unit_test_framework.cpp` and `unit_test_framework.h` into your project, `#include` the `.h` file, and link with the `.cpp` file. No special linker flags are needed.

## Quickstart
Below is a working example of a test suite:
```c++
// File: my_tests.cpp

#include "unit_test_framework.h"

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

To compile with g++ and run the test cases:
```console
$ g++ --std=c++11 my_tests.cpp unit_test_framework.cpp -o my_tests.exe
./my_tests.exe
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


## How to write a test case: The `TEST()` macro
Test cases can be declared using the special `TEST()` macro defined in `unit_test_framework.cpp`:

Skeleton:
```c++
TEST(<test_name>) {
    // test case code
}
```
Replace `<test_name>` with the name of the test case. Note that `<test_name>` must be a valid C++ function name.

Full:
```c++
TEST(test_a_thing) {
    // test case code
}
```

## Special Asserts
You can use any amount of these special asserts in your test cases. You can also use the special asserts in helper functions.

### Equality checks:
For these special asserts, first and second must be comparable using the `==` (for ASSERT_EQUAL) operator or the `!=` operator (for ASSERT_NOT_EQUAL). Other than this restriction, `first` and `second` may be any type.

* **ASSERT_EQUAL**(*first*, *second*)
    * If `first == second` evaluates to false, the test will fail.
* **ASSERT_NOT_EQUAL**(*first*, *second*)
    * If `first != second` evaluates to false, the test will fail.

### Truth checks:
* **ASSERT_TRUE**(*bool value*)
    * If `value` is false, the test will fail.
* **ASSERT_FALSE**(*bool value*)
    * If `value` is true, the test will fail.

### Floating-point comparison:
* **ASSERT_ALMOST_EQUAL**(*double first*, *double second*, *double precision*)
    * If `first` and `second` are not equal within `precision`, the test will fail.

## Command line options
```console
$ ./my_tests.exe -h
usage: ./my_tests.exe [-h] [-n] [-q] [[TEST_NAME] ...]
optional arguments:
 -h, --help	             show this help message and exit
 -n, --show_test_names	 print the names of all discovered test cases and exit
 -q, --quiet             print a reduced summary of test results
 TEST_NAME ...           run only the test cases whose names are listed here. Note: If no test names are specified, all discovered tests are run by default.
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
