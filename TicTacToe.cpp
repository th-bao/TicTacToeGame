#include "TicTacToe.h"
#include <iostream>
using namespace std;

// Constructor: Initializes the TicTacToe game.
// It creates the board, initializes two players with their symbols and names,
// sets the initial current player to 0 (player 1), and sets gameOver to false.
TicTacToe::TicTacToe(int boardSize, const string& name1, const string& name2)
    : board(boardSize), currentPlayerIndex(0), gameOver(false) {
    players[0] = Player('X', name1);
    players[1] = Player('O', name2);
}

// Returns a reference to the current player whose turn it is.
Player& TicTacToe::getCurrentPlayer() {
    return players[currentPlayerIndex];
}

// Switches the current player's turn from Player 1 to Player 2, or vice versa.
void TicTacToe::switchTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % 2;
}

// Returns a reference to the game board object.
Board& TicTacToe::getBoard() {
    return board;
}

// Returns a reference to a specific player based on their index (0 for Player 1, 1 for Player 2).
Player& TicTacToe::getPlayer(int index) {
    return players[index];
}

// Checks if the current game round is over.
bool TicTacToe::isGameOver() const {
    return gameOver;
}

// Resets the game state for a new round:
// Clears the board, sets the current player back to Player 1, and sets gameOver to false.
void TicTacToe::resetGame() {
    board.reset();
    currentPlayerIndex = 0;
    gameOver = false;
}

// Sets the gameOver status of the game.
void TicTacToe::setGameOver(bool status) {
    gameOver = status;
}