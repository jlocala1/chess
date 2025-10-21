# Chess
To launch the program, compile with "make" and type ./chess at the command line, optionally followed by a filename command-line argument. Upon completion of the game (checkmate or stalemate) or if the user elects to quit playing a game that hasn’t ended yet, the current status of the game is output to the specified file. If no filename is specified, no output is written, and no output file is created.

Once the program is running, a chess game using the default chess board is created, and the user is presented with a list of possible actions as follows:

? — display the list of actions

Q — quit the game

L <filename> — load a game from the specified file

S <filename> — save the current game to the specified file

M <move> — try to make the specified move, where <move> is a four-character string giving the column (‘A’-‘H’) (must be an upper case to be valid!) and row (‘1’-‘8’) of the start position, followed by the column and row of the end position

Prior to the user selecting an action, the current state of the board is presented to the user on standard output. The user can repeatedly enter one of the above action specifiers until the program ends, which happens when the game reaches checkmate or stalemate, or the user elects to quit.

A file of the state of a game contains 9 lines. The first 8 are the rows of the board with characters corresponding to pieces and "_" representing an empty space. The 9th line indicates whose turn it is, denoted w for white and b for black.
