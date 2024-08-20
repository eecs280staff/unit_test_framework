#include <vector>

// EFFECTS: Returns the sum of first and second.
double add(double first, double second);

// MODIFIES: the elements in v
// EFFECTS:  All elements are "shifted" right by one unit, with the
//           last element wrapping around to the beginning.
// EXAMPLE:  If v contains [0,1,3,3,4], it would be modified to
//           contain [4,0,1,3,3]
void slideRight(std::vector<int> &v);

// MODIFIES: the elements in v
// EFFECTS:  Reverses the order of the elements in v.
// EXAMPLE:  If v contains, [1,2,3,4,5], it would be modified to
//           contain [5,4,3,2,1]
void flip(std::vector<int> &v);