/*
 * CONTAINS BUGGY IMPLEMENTATIONS FOR tutorial.ccp
 *
 * You can turn on buggy solutions by compiling with the following
 * preprocessor symbols defined.
 *
 * SLIDE_RIGHT_BUG_1
 * The implementation slides left instead of right.
 * 
 * SLIDE_RIGHT_BUG_2
 * The implementation doesn't copy the last element to the beginning.
 * 
 * SLIDE_RIGHT_BUG_3
 * 
 * FLIP_BUG_1
 * 
 * FLIP_BUG_2
 * 
 * FLIP_BUG_3
 * 
 * 
 */

#include "tutorial.hpp"
#include <iostream>

using namespace std;

// MODIFIES: the elements in v
// EFFECTS:  All elements are "shifted" right by one unit, with the
//           last element wrapping around to the beginning.
// EXAMPLE:  If v contains [0,1,3,3,4], it would be modified to
//           contain [4,0,1,3,3]

#if defined SLIDE_RIGHT_BUG_1
// Bug Description: The implementation slides left instead of right.

void slideRight(std::vector<int> &v) {
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  int first = *arr;
  for(int *ptr = arr+1; ptr < arr + n; ++ptr) {
    *(ptr-1) = *ptr;
  }
  *(arr+n-1) = first;
}

#elif defined SLIDE_RIGHT_BUG_2
// Bug Description: The implementation doesn't copy the last element to the beginning.

void slideRight(std::vector<int> &v) {
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  for (int* p = arr+n-1; arr < p; --p)
    *p = *(p-1);
}

#elif defined SLIDE_RIGHT_BUG_3
// Bug Description: The implementation fails for an empty vector

void slideRight(std::vector<int> &v) {
  if (v.empty()) { v[0] = v[100000]; return; }
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  int last = *(arr+n-1);
  for (int* p = arr+n-1; arr < p; --p)
    *p = *(p-1);
  *arr = last;
}

#else

void slideRight(std::vector<int> &v) {
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  int last = *(arr+n-1);
  for (int* p = arr+n-1; arr < p; --p)
    *p = *(p-1);
  *arr = last;
}

#endif

// MODIFIES: the elements in v
// EFFECTS:  Reverses the order of the elements in v.
// EXAMPLE:  If v contains, [1,2,3,4,5], it would be modified to
//           contain [5,4,3,2,1]
#if defined FLIP_BUG_1
// Bug Description: The function does nothing.

void flip(std::vector<int> &v) {
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  for (int *p = arr+n-1; arr < p; ++arr, --p) {
    // Accidentally swapping pointers instead of values. arr and p are
    // swapped and immediately "cross over" each other.
    int *t = arr;
    arr = p;
    p = t;
  }
}

#elif defined FLIP_BUG_2
// Bug Description: Logical error in swapping middle elements.
// Only manifests for odd length arrays.

void flip(std::vector<int> &v) {
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  for (int i = 0; i < (n-1)/2; ++i) {
    int t = arr[i];
    arr[i] = arr[n-1-i];
    arr[n-1-i] = t;
  }
  
  // manually flip middle elements (incorrectly, for odd length arrays)
  int t = arr[n/2];
  arr[n/2] = arr[n/2-1];
  arr[n/2-1] = t;
}

#elif defined FLIP_BUG_3
// Bug Description: Allows left and right pointers to cross over each other.
// Pointers will go outside the arrays and eventually crash. Only manifests
// for even length arrays.
void flip(std::vector<int> &v) {
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  for (int *p = arr+n-1; arr != p; ++arr, --p) {
    int t = *arr;
    *arr = *p;
    *p = t;
  }
}

#else

void flip(std::vector<int> &v) {
  int *arr = v.data();
  int n = static_cast<int>(v.size());
  for (int *p = arr+n-1; arr < p; ++arr, --p) {
    int t = *arr;
    *arr = *p;
    *p = t;
  }
}

#endif
