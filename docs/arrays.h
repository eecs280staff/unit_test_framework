// REQUIRES: there are at least n elements in arr;
//           n >= 1
// MODIFIES: the elements in arr
// EFFECTS:  All elements are "shifted" right by one unit, with the
//           last element wrapping around to the beginning.
// EXAMPLE:  If arr contains [0,1,3,3,4], it would be modified to
//           contain [4,0,1,3,3]
void slideRight(int arr[], int n);

// REQUIRES: there are at least n elements in arr;
//           n >= 0
// MODIFIES: the elements in arr
// EFFECTS:  Reverses the order of the elements in arr.
// EXAMPLE:  If arr contains, [1,2,3,4,5], it would be modified to
//           contain [5,4,3,2,1]
void flip(int arr[], int n);