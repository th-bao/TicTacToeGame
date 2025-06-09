#pragma once
#include <vector>

class Board {
private:
    std::vector<std::vector<char>> grid; // Represents the Tic-Tac-Toe board grid
    int size;                            // Size of the board (e.g., 3 for 3x3)
    int filledCells;                     // Number of cells currently filled on the board

public:
    Board(int size = 3); // Constructor: Initializes the board with a given size (default 3x3)

    void drawBoard() const;               // Draws the board to the console (primarily for console-based games)
    bool isValidMove(int row, int col) const; // Checks if a move to the specified row and column is valid (within bounds and cell is empty)
    void makeMove(int row, int col, char symbol); // Places a player's symbol on the board at the specified row and column
    bool checkWin(char symbol) const;     // Checks if the given symbol has won the game
    bool isFull() const;                  // Checks if the board is completely filled
    int getSize() const;                  // Returns the size of the board
    char getCell(int row, int col) const; // Returns the symbol at the specified cell (used by GUI to render)
    void reset();                         // Resets the board to its initial empty state
};