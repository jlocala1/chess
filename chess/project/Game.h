#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"

namespace Chess
{

  class Game {

  public:
    // This default constructor initializes a board with the standard
    // piece positions, and sets the state to white's turn
    Game();

    // Returns true if it is white's turn
    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
    bool turn_white() const { return is_white_turn; }
    
    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
    // Displays the game by printing it to stdout
    void display() const { board.display(); }
    
    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
    // Checks if the game is valid
    bool is_valid_game() const { return board.has_valid_kings(); }

    // Attempts to make a move. If successful, the move is made and
    // the turn is switched white <-> black. Otherwise, an exception is thrown
    void make_move(const Position& start, const Position& end);

    // Returns true if the designated player is in check
    bool in_check(const bool& white) const;

    // Returns true if the designated player is in mate
    bool in_mate(const bool& white) const;

    // Returns true if the designated player is in mate
    bool in_stalemate(const bool& white) const;

    // Return the total material point value of the designated player
    int point_value(const bool& white) const;

    // returns whether the movement path is blocked by another piece  
    bool path_clear(const Position& start, const Position& end) const;
	
    //check if a pawn is in position for a promotion
    void promote_pawn(const Position& end);

    //copy constructor
    Game(const Game& orig);
	
    //override equal operator
    Game& operator=(const Game& orig);

    //destructor
    ~Game();

    //Helper functions for path_clear

    //Search vertically
    bool verticalSearch(const Position& start, int row_diff) const;

    //Search Horizontally
    bool horizontalSearch(const Position& start, int col_diff) const;

    //Search up and to the right
    bool topRightDiagonal(const Position& start, int steps) const;

    //Search up and to the left
    bool topLeftDiagonal(const Position& start, int steps) const;

    //Search down and to the right
    bool bottomRightDiagonal(const Position& start, int steps) const;

    //Search down and to the left
    bool bottomLeftDiagonal(const Position& start, int steps) const;
	
  private:
    // The board
    Board board;

    // Is it white's turn?
    bool is_white_turn;

    // Writes the board out to a stream
    friend std::ostream& operator<< (std::ostream& os, const Game& game);

    // Reads the board in from a stream
    friend std::istream& operator>> (std::istream& is, Game& game);
  };
}
#endif // GAME_H
