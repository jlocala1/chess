#include "Knight.h"

//A knight may move in an L-shape, of length either two-by-one or one-by-two. The knight is the only piece that
//is not stopped by other pieces in its way (i.e. it can move through other pieces to get to an open square).

namespace Chess
{
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    //get the magnitude of the differences between the row and col beginning and end
    
    int col_diff = abs(end.first - start.first);
    int row_diff = abs(end.second - start.second);

    
    //check if the magnitude of teh difference between the col is 2 ( so 2 over) and row is 1(so one up or down) which forms an L
    //check if the magnitude of the difference between the row is 2 (so 2 up or down) and col is 1 (so one over) which also forms an L
    
    if((col_diff == 2 && row_diff == 1) || (col_diff == 1 && row_diff == 2)) {
      return true;
    }
    
    return false;//if not a legal then return false
  }
}
