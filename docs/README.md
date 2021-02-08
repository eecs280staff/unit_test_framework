# Tutorial: Unit Test Framework

In this tutorial, you will learn how to write test cases using a lightweight framework that functions similarly to unit test frameworks used in real-life software development.

# Table of Contents

<!-- toc -->

- [Setting Up](#setting-up)
- [Special Test Assertions](#special-test-assertions)
- [Example: Tests for an `add()` function](#example-tests-for-an-add-function)
- [Writing Unit Tests for `slideright()` and `flip() array functions`](#writing-unit-tests-for-slideright-and-flip-array-functions)
- [Write a `compare_arrays()` Function](#write-a-compare_arrays-function)
- [Writing Helper Functions for Tests](#writing-helper-functions-for-tests)

<!-- tocstop -->

# Setting Up

First, you will need the file `unit_test_framework.h`. This file is included with the starter code for EECS 280 projects where you are expected to use it. You can also download it directly (e.g. for this tutorial) with the following command.

```console
$ wget https://raw.githubusercontent.com/eecs280staff/unit_test_framework/master/unit_test_framework.h
```

For this tutorial, you'll need two files, `arrays.h` and `arrays.cpp`.

```console
$ wget https://raw.githubusercontent.com/eecs280staff/unit_test_framework/arrays.h
$ wget https://raw.githubusercontent.com/eecs280staff/unit_test_framework/arrays.cpp
```

These functions contain implementations of two functions (`slideRight()` and `flip()`) that work with arrays. The implementations each contain a bug! You'll catch the bugs by writing tests.

Your tests should go in a new file, called `arrays_tests.cpp`. Add the following code to `arrays_tests.cpp`:

```c++
#include "arrays.h"
#include "unit_test_framework.h"

// We define a test case with the TEST(<test_name>) macro.
// <test_name> can be any valid C++ function name.
TEST(true_is_true) {
  ASSERT_TRUE(true);
}

TEST(numbers_are_equal) {
  ASSERT_EQUAL(42, 42);
}

TEST_MAIN() // No semicolon!
```

These are just example tests...we'll get to "real" tests for our array functions soon.

You're probably wondering why some of the syntax in this code looks unusual. That's because this testing framework uses preprocessor macros to achieve functionality that wouldn't be possible with the plain C++ you're used to seeing. Preprocessor macros are beyond the scope of this course and in general should be used sparingly, so here's all you need to know:

  - The `TEST(<test_name>)` essentially gets replaced (by the preprocessor) with a test function called `<test_name>`, where `<test_name>` is any valid C++ function name and `<test_name>` will be the name of the new test function. You do **NOT** need to put quotes around `<test_name>`, and if you do you'll get a compiler error.

  - `TEST_MAIN()` gets replaced by a `main()` function that detects and runs all of the test cases you defined using the `TEST()` macro. Unlike in Project 1 where you had to explicitly call your test functions from `main()`, this framework handles that for you!

  - `ASSERT_TRUE()` is one of several special test assertion preprocessor macros that you can use to check conditions in your test cases. You'll be using these instead of `assert()` in your unit tests. These will be demonstrated in more detail in the next section.

Compile and run this test case with the following two commands:

```console
$ g++ -Wall -Werror -pedantic -g -std=c++11 arrays.cpp arrays_tests.cpp -o arrays_tests.exe
$ ./arrays_tests.exe
Running test: numbers_are_equal
PASS
Running test: true_is_true
PASS

*** Results ***
** Test case 'numbers_are_equal': PASS
** Test case 'true_is_true': PASS
*** Summary ***
Out of 2 tests run:
0 failure(s), 0 error(s)
```

Another nice feature of the framework is that we can tell it to run only a subset of our test cases. If we wanted to only run the test `numbers_are_equal`, we could do it with this command:

```console
$ ./arrays_tests.exe numbers_are_equal
Running test: numbers_are_equal
PASS

*** Results ***
** Test case 'numbers_are_equal': PASS
*** Summary ***
Out of 1 tests run:
0 failure(s), 0 error(s)
```

You can pass any number of test names as command line arguments, and it will only run the ones you've specified.

# Special Test Assertions

One of the main reasons for using the special assertions provided by the framework is that they allow the framework to run all of your tests and report which ones passed and which ones failed. As you may have noticed, when you use regular `assert()` in your test cases, they automatically stop at the first failure. This can make it difficult to debug errors in one test that are actually caused by a function whose test cases didn't get a chance to run yet.

Here is a summary of all the special assertions that the framework provides:

Assertion | Description
------------ | -------------
<code>ASSERT_EQUAL(<i>first</i>, <i>second</i>)</code> | If `first == second` evaluates to false, the test will fail. **Note:** Do not use this if `first` and `second` are not comparable using the == operator. Other than this restriction, `first` and `second` may be any type.
<code>ASSERT_NOT_EQUAL(<i>first</i>, <i>second</i>)</code> | If `first != second` evaluates to false, the test will fail. **Note:** Do not use this if `first` and `second` are not comparable using the != operator. Other than this restriction, `first` and `second` may be any type.
<code>ASSERT_TRUE(<i>bool value</i>)</code> | If `value` is false, the test will fail.
<code>ASSERT_FALSE(<i>bool value</i>)</code> | If `value` is true, the test will fail.
<code>ASSERT_ALMOST_EQUAL(<i>double first</i>, <i>double second</i>, <i>double precision</i>)</code> | If `first` and `second` are not equal within `precision`, the test will fail.
<code>ASSERT_SEQUENCE_EQUAL(<i>first</i>, <i>second</i>)</code> | `first` and `second` must be sequences (e.g. arrays, vectors, lists). If `first` and `second` do not have equal elements, the test will fail.

# Example: Tests for an `add()` function

Suppose we have an `add()` function that computes the sum of two
`double` arguments:

```c++
double add(double first, double second) {
  return first + second;
}
```

We could write a test case for `add()` using `assert()` as follows:

```c++
void add_test_basic();
bool doubles_almost_equal(double first, double second, double range);

int main() {
  add_test_basic();
}

void add_test_basic() {
  double a = 0.1;
  double b = 0.2;
  double expected_sum = 0.3;
  double actual_sum = add(a, b);

  cout << setprecision(20);
  cout << "expected_sum: " << expected_sum << endl;
  cout << "actual_sum: " << actual_sum << endl;

  assert(doubles_almost_equal(expected_sum, actual_sum, .001));
}

bool doubles_almost_equal(double first, double second, double range) {
  double diff = abs(first - second);
  return diff < range;
}
```

The `doubles_almost_equal()` function above determines if two `double` values
are equal to each other within a given range of precision. Since
`double` values are not exact, it isn't safe to compare non-integral
`double` values with the == operator.

Let's rewrite the test case above using the unit test framework.
First, let's turn `add_test_basic` into a `TEST()` and replace `assert()` with `ASSERT_TRUE()`:

```c++
TEST(add_basic) {
  double a = 0.1;
  double b = 0.2;
  double expected_sum = 0.3;
  double actual_sum = add(a, b);

  cout << setprecision(20);
  cout << "expected_sum: " << expected_sum << endl;
  cout << "actual_sum: " << actual_sum << endl;

  ASSERT_TRUE(doubles_almost_equal(expected_sum, actual_sum, .001));
}
```

Next, instead of using our own `doubles_almost_equal()` function, let's use `ASSERT_ALMOST_EQUAL()` from the framework. You should always use the "almost" version when comparing results that might be slightly different due to limited numeric precision.

```c++
TEST(add_basic) {
  double a = 0.1;
  double b = 0.2;
  double expected_sum = 0.3;
  double actual_sum = add(a, b);

  ASSERT_ALMOST_EQUAL(expected_sum, actual_sum, .001);
}
```

`ASSERT_ALMOST_EQUAL()` and `ASSERT_EQUAL()` will print out the expected and actual values for us if they're different.
Finally, let's add `TEST_MAIN()` and any `#include`s that we need:

```c++
#include "eecs280math.h"
#include "unit_test_framework.h"

TEST(add_basic) {
  double a = 0.1;
  double b = 0.2;
  double expected_sum = 0.3;
  double actual_sum = add(a, b);

  ASSERT_ALMOST_EQUAL(expected_sum, actual_sum, .001);
}

TEST_MAIN()
```

# Writing Unit Tests for `slideright()` and `flip()` array functions

Now, let's add some real test cases for `slideRight()` and `flip()` to `arrays_tests.cpp`.

For example, here's one test for each function (you can replace your existing code in `arrays_tests.cpp` with the code below):

```c++
#include "arrays.h"
#include "unit_test_framework.h"

// A helper function for comparing arrays. Returns true if the
// arrays are the same size and contain the same values.
bool compare_arrays(int arr1[], int size1, int arr2[], int size2) {
  if (size1 != size2) {
    return false;
  }

  for (int i = 0; i < size1; ++i) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }

  return true;
}

TEST(test_slide_right_1) {
  int testing[] = { 4, 0, 1, 3, 3 };
  int correct[] = { 3, 4, 0, 1, 3 };
  slideRight(testing, 5);
  ASSERT_TRUE(compare_arrays(testing, 5, correct, 5));
}

TEST(test_flip_1) {
  int testing[] = { 4, 0, 1, 3, 3 };
  int correct[] = { 3, 3, 1, 0, 4 };
  flip(testing, 5);
  ASSERT_TRUE(compare_arrays(testing, 5, correct, 5));
}

TEST_MAIN() // No semicolon!
```

Compile and run the tests with the following commands:

```console
$ g++ -Wall -Werror -pedantic -O1 -std=c++11 arrays.cpp arrays_tests.cpp -o arrays_tests.exe
$ ./arrays_tests.exe
```

A failed test indicates there's a bug in our `arrays.cpp` code. The tests above caught
the bug in `slideRight` but not the one in `flip` (recall we mentioned there is a bug in both).
You'll need to write more tests to create a thorough testing suite!
Ideally, you should have enough tests that any reasonable bug will cause at least
one test to fail.

On the other hand, when run against a correct implementation, all of your tests should pass. (Otherwise, it would be giving a false positive for detecting a bug.) If a test passes on a correct implementation,
we call it _valid_.

Once you feel your tests are thorough, submit `arrays_tests.cpp` to the unit test framework tutorial
[autograder](https://autograder.io/). It will check your tests against a set of buggy implementations
of `slideRight` and `flip`, similar to the buggy versions functions provided with
this tutorial. To earn points, your tests must detect
(i.e. fail when run against) each of the bugs. Note that the autograder will
discard any tests that are not valid when checked against a correct solution.

**Note**: Because these functions work with arrays, it's possible that a buggy version might go outside the bounds of the array when given one of your tests, causing a segfault or other crash. For example:

```console
$ ./arrays_tests.exe
Running test: test_flip_1
PASS
Running test: test_flip_2
Segmentation fault
```

This still "counts" as catching the bug, because the program exits with a non-zero exit status (as is the case failed assertion) and we are alerted to the presence of a bug.

# Writing Helper Functions for Tests

You may have noticed some duplicated code in your test cases, particularly when checking the expected and actual contents of arrays. It would be a good idea to write a function called `compare_arrays()` that takes in two arrays and their lengths and checks that the contents of those arrays are equal using the framework's special assertions. Then, you can refactor your test cases to use `compare_arrays()` instead of duplicating code.

**Note**: Since `compare_arrays()` is a test helper, it should be within `arrays_tests.cpp`!

**Note**: `compare_arrays()` is different from
`ASSERT_SEQUENCE_EQUAL()`. The latter works on arrays, but not on
pointers that decayed from arrays. You will not be able to use
`ASSERT_SEQUENCE_EQUAL()` in `compare_arrays()`.
