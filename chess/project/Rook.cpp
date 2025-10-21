#include "Rook.h"
//A rook may move any number of squares, but only along a horizontal or vertical line on the board. A rook may not move through other pieces.

namespace Chess
{
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {
    //if the columns at the beginning is the same as the column at the end then the rook stayed in the same vertical line
    //if the rooks row is the same at the beginning as it is at the end then it stayed in the same horizontal line
    //capture logic and legality will be implemented in make_move since a lot of pieces have the same kind of restriction
    
    if (start.first == end.first || start.second == end.second ){
      return true;
    }

    return false;//return false if not in the same vertcial or horizantal line
  }
}
