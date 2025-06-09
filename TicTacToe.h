#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <string>
#include "Player.h" // Includes the Player class definition
#include "Board.h"  // Includes the Board class definition

class TicTacToe {
private:
    Board board;          // The game board object
    Player players[2];    // An array holding two Player objects (Player 1 and Player 2)
    int currentPlayerIndex; // Index to keep track of whose turn it is (0 for player 1, 1 for player 2)
    bool gameOver;        // Flag to indicate if the current game round is over

public:
    // Constructor: Initializes the TicTacToe game with board size and player names.
    TicTacToe(int boardSize, const std::string& name1, const std::string& name2);

    // Returns a reference to the current player whose turn it is.
    Player& getCurrentPlayer();
    // Switches the turn to the other player.
    void switchTurn();
    // Returns a reference to the game board. (Used by GUI)
    Board& getBoard();
    // Returns a reference to a specific player (0 for player 1, 1 for player 2). (Used by GUI)
    Player& getPlayer(int index);
    // Returns true if the current game round is over, false otherwise.
    bool isGameOver() const;
    // Resets the game to start a new round without changing player scores.
    void resetGame();
    // Sets the game over status.
    void setGameOver(bool status);
};
#endif