#include "King.h"

//A king may move in any direction, including diagonally, but may only move one square.

namespace Chess
{
  bool King::legal_move_shape(const Position& start, const Position& end) const {

    //find the magnitude of the the difference between the beginning and ending spot
    //of the king
    int col_diff = abs(end.first - start.first);
    int row_diff = abs(end.second - start.second);
    

    //id teh magnitude of the differnce in the rows and cols is one then it works because it moved 1 diagonally
    //if the magnitude in the difference between the row is 1 and col is 0 that also works because it just move one up and down
    // if the magnitude of the row is 0 and teh col is 1 that means it just move 1 up or down.
    //these are all valid moves for the king
    if((row_diff == 1 && col_diff == 1) || (row_diff == 1 && col_diff == 0) || (row_diff == 0 && col_diff == 1)) {
      return true;
    }
    
    return false;//return false if it does not make a valid move
  }
}
