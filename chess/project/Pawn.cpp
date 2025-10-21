#include "Pawn.h"
//A pawn can move only forward towards the opponent’s side of the board, but with restrictions. On its first move of the game,
//a pawn may move forward either one or two squares; on subsequent moves, a pawn may only move forward one square. A pawn may
//not move through other pieces. Furthermore, the pawn may not use a forward move to land in a square that is occupied by any
//player’s piece. In a separate type of action, a pawn may make a special move to “capture” an opponent piece 





namespace Chess
{
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const{
    //not taking magnitude and just taking difference because we want magnitude and direction
    int col_diff = end.first - start.first;
    int row_diff = end.second - start.second;

    
    //have to check different legal moves based on teh color since the pawn can't come back towards its side
    
    if(is_white()) {
      if(row_diff == 1 && col_diff == 0){
        return true;//moved 1 up, move 0 to the right or left
      }
      else if(row_diff == 2 && col_diff == 0 && start.second == '2'){
        return true;//move two up and 0 right or left. Can only do this if pawn is in it's starting position (At 2)
      }
      
      else {
	return false;//not a legal move for white
      }
    }
    else {//if the pawn is black (we just have to consider slightly different startig conditions)
      
      if(row_diff == -1 && col_diff == 0){
        return true;//moved down one(since blakc starts at the top) and 0 right or left
      }
      else if(row_diff == -2 && col_diff == 0 && start.second == '7'){//moved down 2 and 0 right or left only if position is 7 (starting position for black pawns)
        return true;
      }
      else{
        return false;//not a legal move for black
      }
    }
  }


  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const{

    //find the directional magnitude of the rows and cols
    int col_diff = end.first - start.first;
    int row_diff = end.second - start.second;

    //Pawns have different shapes for normal movement versus for capturing. Pawn movement is always forward
    //(one or two squares, depending on the original position), whereas pawn capturing is always diagonal
    // teh valid diagnoanl moves for the pawns bacically form a "V" that projects from the pawn
    
    if(is_white()){//if it is white's turn
      
      //make sure the directional magnitudes are equal for valid  movement (right one up one)
      if(col_diff == 1 && row_diff == 1) {
	return true;
      }
      //directional magnitude (left one up one)
      else if(col_diff == -1 && row_diff == 1){
	return true;
      }
      
      else{
	return false;// not a valid move
	
      }
    }

    
    else{// if the pawn is black
      
      if(col_diff == 1 && row_diff == -1) {//right one down one because balck starts at teh top
	return true;
      }
      else if(col_diff == -1 && row_diff == -1){//left one down one because black starts at the top
	return true;
      }
      else{
	return false;//if not one of the two above then it is not a legal move
      }
    }
  }
}
