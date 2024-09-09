#include "tutorial.hpp"

// EFFECTS: Returns the sum of first and second.
double add(double first, double second) {
  return first + second;
}

// MODIFIES: the elements in v
// EFFECTS:  All elements are "shifted" right by one unit, with the
//           last element wrapping around to the beginning.
// EXAMPLE:  If v contains [0,1,3,3,4], it would be modified to
//           contain [4,0,1,3,3]
void slideRight(std::vector<int> &v) {
  // NOTE - This implementation contains a bug!
  for (int i = 1; i < static_cast<int>(v.size()); ++i) {
    v.at(i) = v.at(i-1);
  }
}

// MODIFIES: the elements in v
// EFFECTS:  Reverses the order of the elements in v.
// EXAMPLE:  If v contains, [1,2,3,4,5], it would be modified to
//           contain [5,4,3,2,1]
void flip(std::vector<int> &v) {
  // NOTE - This implementation contains a bug!
  int left = 0;
  int right = static_cast<int>(v.size()) - 1;
  while (left != right) {
    int temp = v.at(left);
    v.at(left) = v.at(right);
    v.at(right) = temp;
    ++left;
    --right;
  }
}
