#include "Bishop.h"
//A bishop may move diagonally only but may move any number of spaces. A bishop may not move through other pieces.

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {


    //get the magnitude of the difference between teh rows and or columns
    int col_diff = abs(end.first - start.first);
    int row_diff = abs(end.second - start.second);




    //to move diagonallly en=sure the magnitude of the difference in the row is the same as the column
    if(row_diff == col_diff) {
      return true;
    }
    
    return false;//return false if not valid
  }
}
