#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

  const Piece* Board::operator()(const Position& position) const {
    if (occ.find(position) == occ.end()) {
      return nullptr;
    }
    else {
      return occ.at(position);
    }
  }
  
  void Board::add_piece(const Position& position, const char& piece_designator) {
    // check if position is on board, position is free, and designator is valid
    if (!position_exists(position)) {
      throw Exception("invalid position");
    }
    else if (occ[position] != nullptr) {
      throw Exception("position is occupied");
    }
    
    occ[position] = create_piece(piece_designator);
    if (occ[position] == nullptr) {
      throw Exception("invalid designator");
    }
  }

  void Board::move_piece(const Position& start, const Position& end) {
    remove_piece(end);
    occ[end] = occ[start];
    occ.erase(start);
  }


  void Board::remove_piece(const Position& pos) {
    delete occ[pos];
    occ.erase(pos);
  }


  void Board::display() const {
    std::cout << "b" << std::endl;
    std::cout<< " A B C D E F G H" << std::endl; //prints the header for the function
    // Loop through each row and column of the board
    for (char row = '8'; row >= '1'; row--) {
      std::cout<< row;
      for (char col = 'A'; col <= 'H'; col++) {
	Position pos(col, row);
	const Piece* piece = operator()(pos); // Get the piece at the current position
	    
	// Determine the foreground and background colors based on the position
	if ((col + row) % 2 == 0) {
	  Terminal::color_bg(Terminal::WHITE); // Set background color to yellow for light squares
	  Terminal::color_fg(true, Terminal::BLUE);
	}
	else {
	  Terminal::color_bg(Terminal::BLUE); // Set background color to blue for dark squares
	  Terminal::color_fg(true, Terminal::WHITE);
	}

	// Print the piece symbol or a dash if the square is empty
	if (piece != nullptr) {
	  std::cout << piece->to_ascii() << ' '; // Print the ASCII representation of the piece
	}
	    
	else {
	     
	  std::cout << "  "; // Print two spaces
	}
      }
      // Reset the colors to default after printing the row
      Terminal::set_default(); // Reset colors to default after printing the row

      // Move to the next row
      std::cout << row << std::endl;
    }
    std::cout<< " A B C D E F G H" << std::endl;	
    std::cout << "w" << std::endl;    
  }
  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      if (it->second) {
	switch (it->second->to_ascii()) {
	case 'K':
	  white_king_count++;
	  break;
	case 'k':
	  black_king_count++;
	  break;
	}
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }



  // return true if position is valid square on board
  bool Board::position_exists(const Position& pos) {
    if(pos.first >= 'A' && pos.first <= 'H'
       && pos.second >= '1' && pos.second <= '8') {
      return true;
    }
    else {
      return false;
    }
  }

  Position Board::find_king(const bool& white) const {
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      if (it->second) {
	if (white && it->second->to_ascii() == 'K') {
	  return it->first;
	}
	else if(!white && it->second->to_ascii() == 'k') {
	  return it->first;
	}
      }
    }
    // should never reach here
    return Position('A',1);
  }



}
