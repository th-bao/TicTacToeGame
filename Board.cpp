#include "Board.h"
#include <iostream>
using namespace std;

// Constructor: Initializes the board with the given size and sets all cells to ' ' (empty).
Board::Board(int s) : size(s), filledCells(0) {
    grid.resize(size, vector<char>(size, ' '));
}

// Draws the current state of the board to the console.
void Board::drawBoard() const {
    cout << "-------------" << endl;
    for (int i = 0; i < size; i++) {
        cout << "| ";
        for (int j = 0; j < size; j++) {
            cout << grid[i][j] << " | ";
        }
        cout << endl << "-------------" << endl;
    }
}

// Checks if a proposed move (row, col) is valid.
// A move is valid if the row and column are within the board's bounds and the cell is empty.
bool Board::isValidMove(int row, int col) const {
    return row >= 0 && row < size && col >= 0 && col < size && grid[row][col] == ' ';
}

// Makes a move on the board by placing the given symbol at the specified row and column.
// Increments the filledCells counter if the move is valid.
void Board::makeMove(int row, int col, char symbol) {
    if (isValidMove(row, col)) {
        grid[row][col] = symbol;
        filledCells++;
    }
}

// Checks if the player with the given symbol has won the game.
// It checks all rows, columns, and both diagonals for a win.
bool Board::checkWin(char symbol) const {
    // Check rows and columns
    for (int i = 0; i < size; ++i) {
        bool rowWin = true, colWin = true;
        for (int j = 0; j < size; ++j) {
            if (grid[i][j] != symbol) rowWin = false;
            if (grid[j][i] != symbol) colWin = false;
        }
        if (rowWin || colWin) return true;
    }

    // Check diagonals
    bool diag1 = true, diag2 = true;
    for (int i = 0; i < size; ++i) {
        if (grid[i][i] != symbol) diag1 = false;                // Main diagonal
        if (grid[i][size - 1 - i] != symbol) diag2 = false;     // Anti-diagonal
    }
    return diag1 || diag2;
}

// Checks if the board is completely filled, indicating a draw if no one has won.
bool Board::isFull() const {
    return filledCells == size * size;
}

// Returns the size of the board.
int Board::getSize() const {
    return size;
}

// Returns the character (symbol) at the specified row and column.
// Returns ' ' if the coordinates are out of bounds.
char Board::getCell(int row, int col) const {
    if (row >= 0 && row < size && col >= 0 && col < size) {
        return grid[row][col];
    }
    return ' '; // Return empty space for invalid access
}

// Resets the board by clearing all cells and setting filledCells back to 0.
void Board::reset() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            grid[i][j] = ' ';
        }
    }
    filledCells = 0;
}