#pragma once
#include <string>

class Player {
private:
    char symbol;      // The symbol representing the player on the board (e.g., 'X' or 'O')
    std::string name; // The name of the player
    int score;        // The player's score

public:
    Player(char sym = 'X', std::string n = "Player X"); // Constructor: Initializes player with a symbol and name
    char getSymbol() const;      // Returns the player's symbol
    std::string getName() const; // Returns the player's name
    int getScore() const;        // Returns the player's current score
    void addPoint();             // Increments the player's score by one
};