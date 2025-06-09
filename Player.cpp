#include "Player.h"

// Constructor: Initializes a Player object with a given symbol and name, and sets the score to 0.
Player::Player(char sym, std::string n) : symbol(sym), name(n), score(0) {}

// Returns the symbol of the player.
char Player::getSymbol() const {
    return symbol;
}

// Returns the name of the player.
std::string Player::getName() const {
    return name;
}

// Returns the current score of the player.
int Player::getScore() const {
    return score;
}

// Increments the player's score by one.
void Player::addPoint() {
    score++;
}