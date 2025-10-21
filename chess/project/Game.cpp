#include <cassert>
#include "Game.h"
#include "Piece.h"
#include "Board.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Game::Game() : is_white_turn(true) {
    // Add the pawns
    for (int i = 0; i < 8; i++) {
      board.add_piece(Position('A' + i, '1' + 1), 'P');
      board.add_piece(Position('A' + i, '1' + 6), 'p');
    }

    // Add the rooks
    board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
    board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
    board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
    board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

    // Add the knights
    board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
    board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
    board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
    board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

    // Add the bishops
    board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
    board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
    board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
    board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

    // Add the kings and queens
    board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
    board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
    board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
    board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
  }

  void Game::make_move(const Position& start, const Position& end) {
    //checking for specific cases in order from instructions
    if (!board.position_exists(start)) {
      throw Exception("start position is not on board");
    }
    if (!board.position_exists(end)) {
      throw Exception("end position is not on board");
    }
    if (board(start) == nullptr) {
      throw Exception("no piece at start position");
    }
    if (is_white_turn != board(start)->is_white()) {
      throw Exception("piece color and turn do not match");
    }
	 
    if(board(end) == nullptr) {
      if (!(board(start)->legal_move_shape(start, end))) {
	throw Exception("illegal move shape");
      }
    }
	
    else if (board(end) != nullptr) { //if end position has a piece, check if that piece can be captured
      if (board(end)->is_white() == is_white_turn) {
	throw Exception("cannot capture own piece");
      }
      if (!(board(start)->legal_capture_shape(start, end))) {
	throw Exception("illegal capture shape");
      }
    }
    // check for clear path to piece
    if(path_clear(start, end)) {
      throw Exception("path is not clear");
    }
    // cannot move into check, create a clone to simulate move
    Game clone(*this);
    clone.board.move_piece(start, end);
    clone.promote_pawn(end);
    if (clone.in_check(is_white_turn)) {
      throw Exception("move exposes check");
    }
    else { 
      // execute if valid
      board.move_piece(start, end);
      promote_pawn(end);
      is_white_turn = !is_white_turn;
    }

  }

  bool Game::in_check(const bool& white) const {
    Position kingPos = board.find_king(white); // use find_king function to create a position holder for king
    // Check every piece on the board
    for(char col = 'A'; col <= 'H'; col++) {
      for(char row = '1'; row <= '8'; row++) {
	Position pos(col,row);
	if ((board(pos) != nullptr) && (board(pos)->is_white() == !white)
	    && board(pos)->legal_capture_shape(pos, kingPos)
	    && !path_clear(pos, kingPos)) { //check that pieces are opposite colors and in position to capture king
	  return true;
	}
      }
    }
    return false;
  }


  bool Game::in_mate(const bool& white) const {
    // Check if the player is already in check
    if (!in_check(white)) {
      return false;
    }

    Game clone(*this);  // clone current game
    for (char col = 'A'; col <= 'H'; col++) { //iterate through whole board
      for (char row = '1'; row <= '8'; row++) {
	Position startPos(col, row);
	// check that the starting position has player's piece
	if (clone.board(startPos) && clone.board(startPos)->is_white() == white) {
	  // try all moves
	  for (char i = 'A'; i <= 'H'; i++) {
	    for (char j = '1'; j <= '8'; j++) {
	      Position endPos(i, j);
	      try {
		clone.make_move(startPos, endPos); //if move is made player is not in mate
		return false;
	      }
	      catch (Exception&) {
		continue;
	      }
	    }
	  } 
	}
      }
    } 

    return true;
  }


  bool Game::in_stalemate(const bool& white) const {
    //if in check, not a stalemate
    if (in_check(white)) {
      return false;
    }
    // clone current game
    Game clone(*this);
    for (char col = 'A'; col <= 'H'; col++) {
      for (char row = '1'; row <= '8'; row++) { //iterate through whole board
	Position startPos(col, row);
	//check that there is a piece belonging to the player
	if(clone.board(startPos) && clone.board(startPos)->is_white() == white) {
	  //iterate through every possible move for that piece if it's there
	  for (char i = 'A'; i <= 'H'; i++) { 
	    for (char j = '1'; j <= '8'; j++) {
	      Position endPos(i, j);
	      try {
		clone.make_move(startPos, endPos);
		return false; //found one legal move using make_move
	      }
	      catch (Exception& e) {
		continue;
	      }
	      return true; //keep looking for a move if exception is thrown
	    }
	  } 
	}
      }
    } 
    //no legal move found
    return true;
  }

  // Return the total material point value of the designated player
  int Game::point_value(const bool& white) const {
    int totalPoints = 0;
    // go through whole board
    for (char col = 'A'; col <= 'H'; col++) {
      for (char row = '1'; row <= '8'; row++) {
	Position pos(col, row);
	const Piece* piece = board(pos);
	if (piece != nullptr && piece->is_white() == white) { //check if piece exists at spot and is that player's piece
	  totalPoints += piece->point_value(); // add points up using point_value() defined for each piece in header files
	}
      }
    }
    return totalPoints;
  }



  std::istream& operator>> (std::istream& input_stream, Game& game) {
    char piece = '0'; // Used to read piece type from input.
    // Iterate over rows and columns in reverse to read the board setup.
    for (char row = '8'; row >= '1'; row--) {
      for (char col = 'A'; col <= 'H'; col++) {
	// Check if there is a piece at the current position and remove it if present.
	if (game.board(Position(col, row)) != nullptr) {
	  game.board.remove_piece(Position(col, row));
	}
	// Read the piece type from the input stream.
	if (!(input_stream >> piece)) {
	  throw Exception("Invalid or missing board configuration");
	}
	if (piece == '-') {
	  continue; // No piece at this position.
	} else {
	  // Add the new piece to the board at the specified position.
	  game.board.add_piece(Position(col, row), piece);
	}
      }
    }
    // Read the turn information from the input stream.
    if (!(input_stream >> piece)) {
      throw Exception("No turn information specified");
    } else {
      if (piece == 'w') {
	game.is_white_turn = true; // It's white's turn.
      } else if (piece == 'b') {
	game.is_white_turn = false; // It's black's turn.
      } else {
	throw Exception("Invalid turn designator");
      }
    }
    return input_stream; // Return the modified istream object.
  }


  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<< (std::ostream& os, const Game& game) {
    // Write the board out and then either the character 'w' or the character 'b',
    // depending on whose turn it is
    return os << game.board << (game.turn_white() ? 'w' : 'b');
  }


  // function to determine if path is blocked
  bool Game::path_clear(const Position& start, const Position& end) const {
    int col_diff = end.first - start.first;
    int row_diff = end.second - start.second;

    // check vertical and horizontal
    if (col_diff == 0) { 
      return verticalSearch(start, row_diff);
    }
    if (row_diff == 0) { 
      return horizontalSearch(start, col_diff);
    }

    // check all diagonals
    if (abs(col_diff) == abs(row_diff)) {
      if (col_diff > 0 && row_diff > 0) {
	return topRightDiagonal(start, abs(col_diff));
      } else if (col_diff < 0 && row_diff > 0) {
	return topLeftDiagonal(start, abs(col_diff));
      } else if (col_diff > 0 && row_diff < 0) {
	return bottomRightDiagonal(start, abs(col_diff));
      } else if (col_diff < 0 && row_diff < 0) {
	return bottomLeftDiagonal(start, abs(col_diff));
      }
    }

    return false; // path is not clear if searches don't work
  }


  // Game constructor
  Game::Game(const Game& temp) {
    is_white_turn = temp.turn_white();
    for (char col = 'A'; col <= 'H'; col++) {
      for(char row = '8'; row >= '1'; row--) {  
        Position pos(col, row);
        if (temp.board(pos) != nullptr) {
          board.add_piece(pos, temp.board(pos)->to_ascii());
        }
      }
    }
  }


  // promote a pawn
  void Game::promote_pawn(const Position& end) {
    const Piece* piece = board(end);

    // check if piece is a pawn, if it's black check if it reaches row 1
    if (piece != nullptr && piece->to_ascii() == 'p' && end.second == '1') {
      board.remove_piece(end);
      board.add_piece(end, 'q'); // promote to queen
    } else if (piece != nullptr && piece->to_ascii() == 'P' && end.second == '8') { // if it's white check if it reaches row 8
      board.remove_piece(end);
      board.add_piece(end, 'Q'); // promote to queen
    }
  }

  // Override the equals operator so that more easily copy game state
  Game& Game::operator=(const Game& temp) {
    if (this == &temp) {
      return *this;
    }
    // copy pieces and clear board
    is_white_turn = temp.turn_white();
    for (char col = 'A'; col <= 'H'; col++) {
      for(char row = '8'; row >= '1'; row--) {
	Position pos(col, row);
	if (board(pos) != nullptr) {
	  board.remove_piece(pos);
	}
	if (temp.board(pos) != nullptr) {
	  board.add_piece(pos, temp.board(pos)->to_ascii());
	}
      }
    }
    return *this;
  }

  // Game destructor
  Game::~Game() {
    for (char col = 'A'; col <= 'H'; col++) {
      for(char row = '8'; row >= '1'; row--) {
	Position pos(col, row);
	if (board(pos) != nullptr) {
          delete board(pos);
        }
      }
    }
  }

  // Helper functions for path_clear
  // Search vertically for a piece in the way
  bool Game::verticalSearch(const Position& start, int row_diff) const {
    int step;
    if (row_diff > 0) {
      step = 1;
    } else {
      step = -1;
    }
    for (int i = 1; i < abs(row_diff); i++) {
      Position curr_pos(start.first, start.second + i * step);
      if (board(curr_pos) != nullptr) {
	return true;
      }
    }
    return false;
  }

  // search horizontally for a piece in the way
  bool Game::horizontalSearch(const Position& start, int col_diff) const {
    int step;
    if (col_diff > 0) {
      step = 1;
    } else {
      step = -1;
    }
    for (int i = 1; i < abs(col_diff); i++) {
      Position curr_pos(start.first + i * step, start.second);
      if (board(curr_pos) != nullptr) {
	return true;
      }
    }
    return false;
  }

  // search all diagonals
  bool Game::bottomRightDiagonal(const Position& start, int steps) const {
    for (int i = 1; i < steps; i++) {
      Position curr_pos(start.first + i, start.second - i);
      if (board(curr_pos) != nullptr) {
	return true;
      }
    }
    return false;
  }

  bool Game::bottomLeftDiagonal(const Position& start, int steps) const {
    for (int i = 1; i < steps; i++) {
      Position curr_pos(start.first - i, start.second - i);
      if (board(curr_pos) != nullptr) {
	return true;
      }
    }
    return false;
  }

  bool Game::topLeftDiagonal(const Position& start, int steps) const {
    for (int i = 1; i < steps; i++) {
      Position curr_pos(start.first - i, start.second + i);
      if (board(curr_pos) != nullptr) {
	return true;
      }
    }
    return false;
  }

  bool Game::topRightDiagonal(const Position& start, int steps) const {
    for (int i = 1; i < steps; i++) {
      Position curr_pos(start.first + i, start.second + i);
      if (board(curr_pos) != nullptr) {
	return true;
      }
    }
    return false;
  }
}
