# Tutorial: Unit Test Framework

In this tutorial, you will learn how to write test cases using a lightweight framework that functions similarly to unit test frameworks used in real-life software development.

# Table of Contents

<!-- toc -->

- [Setting Up](#setting-up)
- [Special Test Assertions](#special-test-assertions)
- [Example: `eecs280math_tests.cpp` from Lab 1](#example-eecs280math_testscpp-from-lab-1)
- [Write Unit Tests for `slideright()` and `flip()` (Lab 2)](#write-unit-tests-for-slideright-and-flip-lab-2)
- [Write a `compare_arrays()` Function (Lab 2)](#write-a-compare_arrays-function-lab-2)
- [Extra: Convert your Project 1 Tests to use the Framework](#extra-convert-your-project-1-tests-to-use-the-framework)

<!-- tocstop -->

# Setting Up

First, you will need the files `unit_test_framework.h` and `unit_test_framework.cpp`. This files are available with the starter files for Lab 2, but you can also download them with the following commands.

```console
$ wget https://raw.githubusercontent.com/eecs280staff/unit_test_framework/master/unit_test_framework.cpp
$ wget https://raw.githubusercontent.com/eecs280staff/unit_test_framework/master/unit_test_framework.h
```

Second, create a file called `lab02_tests.cpp`. Add the following code to `lab02_tests.cpp`:

```c++
#include "lab02.h"
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

You’re probably wondering why some of the syntax in this code looks unusual. That’s because this testing framework uses preprocessor macros to achieve functionality that wouldn’t be possible with the plain C++ you’re used to seeing. Preprocessor macros are beyond the scope of this course and in general should be used sparingly, so here’s all you need to know:

  - The `TEST(<test_name>)` essentially gets replaced (by the preprocessor) with a test function called `<test_name>`, where `<test_name>` is any valid C++ function name and `<test_name>` will be the name of the new test function. You do **NOT** need to put quotes around `<test_name>`, and if you do you’ll get a compiler error.

  - `TEST_MAIN()` gets replaced by a `main()` function that detects and runs all of the test cases you defined using the `TEST()` macro. Unlike in Project 1 where you had to explicitly call your test functions from `main()`, this framework handles that for you!

  - `ASSERT_TRUE()` is one of several special test assertion preprocessor macros that you can use to check conditions in your test cases. You’ll be using these instead of `assert()` in your unit tests. These will be demonstrated in more detail in the next section.

Compile and run this test case with the following two commands:

```console
$ g++ -Wall -Werror -pedantic -g -std=c++11 lab02_tests.cpp unit_test_framework.cpp -o lab02_tests.exe
$ ./lab02_tests.exe
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
$ ./lab02_tests.exe numbers_are_equal
Running test: numbers_are_equal
PASS

*** Results ***
** Test case 'numbers_are_equal': PASS
*** Summary ***
Out of 1 tests run:
0 failure(s), 0 error(s)
```

You can pass any number of test names as command line arguments, and it will only run the ones you’ve specified.

# Special Test Assertions

One of the main reasons for using the special assertions provided by the framework is that they allow the framework to run all of your tests and report which ones passed and which ones failed. As you may have noticed, when you use regular `assert()` in your test cases, they automatically stop at the first failure. This can make it difficult to debug errors in one test that are actually caused by function whose test cases didn’t get a chance to run yet.

Here is a summary of all the special assertions that the framework provides:

Assertion | Description
------------ | -------------
<code>ASSERT_EQUAL(<i>first</i>, <i>second</i>)</code> | If `first == second` evaluates to false, the test will fail. **Note:** Do not use this if `first` and `second` are not comparable using the == operator. Other than this restriction, `first` and `second` may be any type.
<code>ASSERT_NOT_EQUAL(<i>first</i>, <i>second</i>)</code> | If `first != second` evaluates to false, the test will fail. **Note:** Do not use this if `first` and `second` are not comparable using the != operator. Other than this restriction, `first` and `second` may be any type.
<code>ASSERT_TRUE(<i>bool value</i>)</code> | If `value` is false, the test will fail.
<code>ASSERT_FALSE(<i>bool value</i>)</code> | If `value` is true, the test will fail.
<code>ASSERT_ALMOST_EQUAL(<i>double first</i>, <i>double second</i>, <i>double precision</i>)</code> | If `first` and `second` are not equal within `precision`, the test will fail.

<!-- Lab 1 changed!
# Example: `eecs280math_tests.cpp` from Lab 1

In Lab 1, we provided you with a test case example that used `assert()` to check the correctness of an `add()` function:

```c++
void add_test_basic();
bool doubles_close(double first, double second, double range);

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

  assert(doubles_close(expected_sum, actual_sum, .001));
}

bool doubles_close(double first, double second, double range) {
  double diff = abs(first - second);
  return diff < range;
}
```

First, let’s turn `add_test_basic` into a `TEST()` and replace `assert()` with `ASSERT_TRUE()`:

```c++
TEST(add_basic) {
  double a = 0.1;
  double b = 0.2;
  double expected_sum = 0.3;
  double actual_sum = add(a, b);

  cout << setprecision(20);
  cout << "expected_sum: " << expected_sum << endl;
  cout << "actual_sum: " << actual_sum << endl;

  ASSERT_TRUE(doubles_close(expected_sum, actual_sum, .001));
}
```

Next, instead of using our own `doubles_close()` function, let’s use `ASSERT_ALMOST_EQUAL()` from the framework:

```c++
TEST(add_basic) {
  double a = 0.1;
  double b = 0.2;
  double expected_sum = 0.3;
  double actual_sum = add(a, b);

  ASSERT_ALMOST_EQUAL(expected_sum, actual_sum, .001);
}
```

`ASSERT_ALMOST_EQUAL()` and `ASSERT_EQUAL()` will print out the expected and actual values for us if they’re different.
Finally, let’s add `TEST_MAIN()` and any `#include`s that we need:

```c++
#include “eecs280math.h”
#include “unit_test_framework.h”

TEST(add_basic) {
  double a = 0.1;
  double b = 0.2;
  double expected_sum = 0.3;
  double actual_sum = add(a, b);

  ASSERT_ALMOST_EQUAL(expected_sum, actual_sum, .001);
}

TEST_MAIN()
```
-->

# Write Unit Tests for `slideright()` and `flip()` (Lab 2)

Add at least two test cases each for `slideRight()` and `flip()` to `lab02_tests.cpp`.
Compile and run the tests with the following commands:

```console
$ g++ -Wall -Werror -pedantic -O1 -std=c++11 lab02.cpp lab02_tests.cpp unit_test_framework.cpp -o lab02_tests.exe
$ ./lab02_tests.exe
```

# Write a `compare_arrays()` Function (Lab 2)

You may have noticed some duplicated code in your test cases, particularly when checking the expected and actual contents of arrays. Write a function called `compare_arrays()` that takes in two arrays and their lengths and checks that the contents of those arrays are equal using the frameworks special assertions. Then, refactor your test cases to use `compare_arrays()` instead of duplicating code.

**Note**: Since `compare_arrays()` is a test helper, it should be within `lab02_tests.cpp`!

# Extra: Convert your Project 1 Tests to use the Framework

If you want extra practice with the framework before using it for your test cases in Project 2, update your unit tests for `stats.cpp` so that they use this framework.

**Note**: Do **NOT** submit these updated test cases to the Project 1 autograder, as the framework will not be available there.

*Hint*: You can take advantage of `ASSERT_ALMOST_EQUAL()` to compare doubles in your test cases.
