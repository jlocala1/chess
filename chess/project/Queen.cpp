#include "Queen.h"

namespace Chess
//A queen may move any number of spaces in one direction, including diagonally. A queen may not move through other pieces.

{
  bool Queen::legal_move_shape(const Position& start, const Position& end) const{
    //get teh magnitude of teh difference between teh rows and columns
    int col_diff = abs(end.first - start.first);
    int row_diff = abs(end.second - start.second);
    
    //check if it move in a valid horizantal or vertical line
    if (start.first == end.first || start.second == end.second ){
      return true;
    }
    //check if it moved diagonally properly (because change in row magnitude would be the same magnitude as the change in column)
    else if(row_diff == col_diff) {
      return true;
    }
    
    return false;//return false if no valid moves
  
  }
}
