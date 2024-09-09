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
  if (v.empty()) { return; }
  int first = v.at(0);
  for (int i = 1; i < static_cast<int>(v.size()); ++i) {
    v.at(i-1) = v.at(i);
  }
  v.at(v.size()-1) = first;
}

#elif defined SLIDE_RIGHT_BUG_2
// Bug Description: The implementation doesn't copy the last element to the beginning.

void slideRight(std::vector<int> &v) {
  for (int i = static_cast<int>(v.size()) - 1; i > 0; --i) {
    v.at(i) = v.at(i-1);
  }
}

#elif defined SLIDE_RIGHT_BUG_3
// Bug Description: The implementation fails for an empty vector

void slideRight(std::vector<int> &v) {
  int last = v.at(v.size() - 1);
  for (int i = static_cast<int>(v.size()) - 1; i > 0; --i) {
    v.at(i) = v.at(i-1);
  }
  v.at(0) = last;
}

#else

void slideRight(std::vector<int> &v) {
  if (v.empty()) { return; }
  int last = v.at(v.size() - 1);
  for (int i = static_cast<int>(v.size()) - 1; i > 0; --i) {
    v.at(i) = v.at(i-1);
  }
  v.at(0) = last;
}

#endif

// MODIFIES: the elements in v
// EFFECTS:  Reverses the order of the elements in v.
// EXAMPLE:  If v contains, [1,2,3,4,5], it would be modified to
//           contain [5,4,3,2,1]
#if defined FLIP_BUG_1
// Bug Description: The function does nothing.

void flip(std::vector<int> &v) {
  for (int front = 0, back = static_cast<int>(v.size() - 1); front < back;
       ++front, --back) {
    // Accidentally swapping indices instead of values. front and back are
    // swapped and immediately "cross over" each other.
    int tmp = front;
    front = back;
    back = tmp;
  }
}

#elif defined FLIP_BUG_2
// Bug Description: Logical error in swapping middle elements.
// Only manifests for odd length vectors.

void flip(std::vector<int> &v) {
  int n = static_cast<int>(v.size());
  for (int i = 0; i < (n-1)/2; ++i) {
    int tmp = v.at(i);
    v.at(i) = v.at(n-1-i);
    v.at(n-1-i) = tmp;
  }
  
  // manually flip middle elements (incorrectly, for odd length vectors)
  int tmp = v.at(n/2);
  v.at(n/2) = v.at(n/2-1);
  v.at(n/2-1) = tmp;
}

#elif defined FLIP_BUG_3
// Bug Description: Allows left and right indices to cross over each other.
// Indices will go outside the vectors and eventually crash. Only manifests
// for even length (non-empty) vectors.
void flip(std::vector<int> &v) {
  if (v.empty()) { return; }
  for (int front = 0, back = static_cast<int>(v.size()-1); front != back;
       ++front, --back) {
    int tmp = v.at(front);
    v.at(front) = v.at(back);
    v.at(back) = tmp;
  }
}

#else

void flip(std::vector<int> &v) {
  for (int front = 0, back = static_cast<int>(v.size()-1); front < back;
       ++front, --back) {
    int tmp = v.at(front);
    v.at(front) = v.at(back);
    v.at(back) = tmp;
  }
}

#endif
