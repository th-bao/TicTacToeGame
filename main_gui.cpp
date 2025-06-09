#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "TicTacToe.h" // Includes the main game logic class
#include "Player.h"    // Includes the Player class (though TicTacToe.h already includes it)

using namespace std;

// Constants for GUI element sizing and spacing
const int CELL_SIZE = 100;     // Size of each cell on the Tic-Tac-Toe board
const int PADDING = 5;         // Padding between elements (not widely used in this code)
const int WINDOW_PADDING = 50; // Padding from the window edges to the game board

// Enum to manage the different states of the game's GUI
enum GameState {
    MENU,        // Main menu where board size is selected
    INPUT_NAMES, // Screen for entering player names
    PLAYING,     // Game in progress
    GAME_OVER    // Game round has ended (win or draw)
};

GameState currentGameState = MENU;   // Tracks the current state of the game
int selectedBoardSize = 3;           // Stores the board size chosen by the user
string player1Name = "";             // Stores Player 1's name
string player2Name = "";             // Stores Player 2's name

TicTacToe* game = nullptr; // Pointer to the TicTacToe game object

// Function to draw text on the SFML window.
// It takes the window, text string, font, character size, color, and position.
// 'center' flag centers the text around the given x, y coordinates.
void drawText(sf::RenderWindow& window, const string& text_str, const sf::Font& font, int char_size, sf::Color color, float x, float y, bool center = false) {
    sf::Text text(text_str, font, char_size);
    text.setFillColor(color);
    if (center) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(x, y);
    } else {
        text.setPosition(x, y);
    }
    window.draw(text);
}

// Function to draw an input box for player names.
// Includes a label, the current text inside the box, font, position, and focus status.
void drawInputBox(sf::RenderWindow& window, const string& label, const string& text, const sf::Font& font, sf::Vector2f pos, bool focused) {
    sf::RectangleShape box(sf::Vector2f(300, 40));
    box.setPosition(pos);
    box.setFillColor(sf::Color(240, 240, 240)); // Light grey background
    box.setOutlineThickness(2);
    box.setOutlineColor(focused ? sf::Color::Blue : sf::Color(200, 200, 200)); // Blue outline if focused, light grey otherwise
    window.draw(box);

    drawText(window, label, font, 18, sf::Color::Black, pos.x, pos.y - 25); // Label above the box
    drawText(window, text, font, 22, sf::Color::Black, pos.x + 10, pos.y + 8); // Text inside the box
}

// Helper function to check if the mouse cursor is currently over a button.
bool isMouseOverButton(sf::Vector2f mousePos, sf::Vector2f buttonPos, sf::Vector2f buttonSize) {
    return mousePos.x > buttonPos.x && mousePos.x < buttonPos.x + buttonSize.x &&
           mousePos.y > buttonPos.y && mousePos.y < buttonPos.y + buttonSize.y;
}

// Function to draw a button with a hover effect.
// Changes the button's fill color when the mouse is over it.
void drawButtonWithHover(sf::RenderWindow& window, const string& text_str, const sf::Font& font, int char_size,
                         sf::Vector2f position, sf::Vector2f size, sf::Color baseColor, sf::Color hoverColor,
                         sf::Color textColor, sf::Vector2f mousePos) {
    sf::Color fillColor = isMouseOverButton(mousePos, position, size) ? hoverColor : baseColor; // Determine color based on hover
    sf::RectangleShape button(size);
    button.setPosition(position);
    button.setFillColor(fillColor);
    window.draw(button);

    sf::Text text(text_str, font, char_size);
    text.setFillColor(textColor);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    window.draw(text);
}

// Main function where the SFML application runs.
int main() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic Tac Toe Game");
    window.setFramerateLimit(60); // Limit frame rate to 60 FPS

    sf::Font font;
    // Load the font from a file. Important for displaying text.
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        cerr << "Can't load font Roboto-Regular.ttf\n";
        return -1; // Exit if font loading fails
    }

    bool isTypingPlayer1 = false; // Flag to indicate if player 1's input box is focused
    bool isTypingPlayer2 = false; // Flag to indicate if player 2's input box is focused

    sf::Vector2f backToMenuPos; // Position of the "Back to Menu" button
    sf::Vector2f backToMenuSize; // Size of the "Back to Menu" button
    
    // Main game loop
    while (window.isOpen()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // Get current mouse position

        sf::Event event; // Event object to handle user input

        // Calculate common positions for buttons in GAME_OVER state
        float centerX = window.getSize().x / 2.f;
        float topY = static_cast<float>(window.getSize().y) - 100;

        sf::Vector2f playAgainPos(centerX - 120, topY); // Position for "Play Again" button
        sf::Vector2f quitPos(centerX + 10, topY);       // Position for "Quit" button
        backToMenuPos = sf::Vector2f(centerX - 120, topY + 50); // Position for "Back to Menu" button
        backToMenuSize = sf::Vector2f(230, 40);         // Size for "Back to Menu" button

        // Event handling loop
        while (window.pollEvent(event)) {
            // Close window event
            if (event.type == sf::Event::Closed) window.close();

            // Handle events based on the current game state
            if (currentGameState == MENU) {
                // If mouse button is pressed in MENU state
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Check if 3x3 button is clicked
                    if (isMouseOverButton(mousePos, {325, 200}, {150, 50})) {
                        selectedBoardSize = 3; currentGameState = INPUT_NAMES;
                    } // Check if 4x4 button is clicked
                    else if (isMouseOverButton(mousePos, {325, 270}, {150, 50})) {
                        selectedBoardSize = 4; currentGameState = INPUT_NAMES;
                    } // Check if 5x5 button is clicked
                    else if (isMouseOverButton(mousePos, {325, 340}, {150, 50})) {
                        selectedBoardSize = 5; currentGameState = INPUT_NAMES;
                    }
                }
            } else if (currentGameState == INPUT_NAMES) {
                // If mouse button is pressed in INPUT_NAMES state
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Check if Player 1 input box is clicked
                    if (isMouseOverButton(mousePos, {250, 200}, {300, 40})) {
                        isTypingPlayer1 = true; isTypingPlayer2 = false;
                    } // Check if Player 2 input box is clicked
                    else if (isMouseOverButton(mousePos, {250, 280}, {300, 40})) {
                        isTypingPlayer2 = true; isTypingPlayer1 = false;
                    } // Check if "Start Game" button is clicked
                    else if (isMouseOverButton(mousePos, {325, 360}, {150, 50})) {
                        currentGameState = PLAYING;
                        if (game) delete game; // Delete existing game if any
                        // Create a new TicTacToe game instance
                        game = new TicTacToe(selectedBoardSize, player1Name, player2Name);
                        // Adjust window size based on selected board size
                        window.setSize({(unsigned)(CELL_SIZE * selectedBoardSize + 2 * WINDOW_PADDING), (unsigned)(CELL_SIZE * selectedBoardSize + 2 * WINDOW_PADDING + 100)});
                        // Reset view to match new window size
                        window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
                    } else {
                        // Unfocus all input boxes if clicked elsewhere
                        isTypingPlayer1 = isTypingPlayer2 = false;
                    }
                } // Handle text input
                else if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8) { // Backspace key
                        if (isTypingPlayer1 && !player1Name.empty()) player1Name.pop_back();
                        if (isTypingPlayer2 && !player2Name.empty()) player2Name.pop_back();
                    } else if (event.text.unicode < 128 && event.text.unicode >= 32) { // Printable characters
                        if (isTypingPlayer1 && player1Name.size() < 12) player1Name += static_cast<char>(event.text.unicode);
                        if (isTypingPlayer2 && player2Name.size() < 12) player2Name += static_cast<char>(event.text.unicode);
                    }
                }
            } else if (currentGameState == PLAYING) {
                // If mouse button is pressed in PLAYING state
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Calculate clicked row and column
                    int x = event.mouseButton.x - WINDOW_PADDING;
                    int y = event.mouseButton.y - WINDOW_PADDING;
                    // Check if click is within the board area
                    if (x >= 0 && y >= 0 && x < CELL_SIZE * selectedBoardSize && y < CELL_SIZE * selectedBoardSize) {
                        int col = x / CELL_SIZE;
                        int row = y / CELL_SIZE;
                        Player& player = game->getCurrentPlayer(); // Get current player
                        Board& board = game->getBoard();         // Get game board
                        // If the move is valid
                        if (board.isValidMove(row, col)) {
                            board.makeMove(row, col, player.getSymbol()); // Make the move
                            // Check for win condition
                            if (board.checkWin(player.getSymbol())) {
                                player.addPoint();          // Add point to winning player
                                game->setGameOver(true);    // Set game over
                                currentGameState = GAME_OVER; // Change state to GAME_OVER
                            } // Check for draw condition
                            else if (board.isFull()) {
                                game->setGameOver(true);    // Set game over (draw)
                                currentGameState = GAME_OVER; // Change state to GAME_OVER
                            } else {
                                game->switchTurn();         // Switch turn to the other player
                            }
                        }
                    }
                }
            } else if (currentGameState == GAME_OVER) {
                // If mouse button is pressed in GAME_OVER state
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    // Check if "Play Again" button is clicked
                    if (isMouseOverButton(mousePos, {window.getSize().x / 2.f - 100, static_cast<float>(window.getSize().y) - 80}, {100, 40})) {
                        game->resetGame();        // Reset the game board and turn
                        game->setGameOver(false); // Set game over status to false
                        currentGameState = PLAYING; // Change state to PLAYING
                    } // Check if "Quit" button is clicked
                    else if (isMouseOverButton(mousePos, {window.getSize().x / 2.f + 10, static_cast<float>(window.getSize().y) - 80}, {100, 40})) {
                        window.close(); // Close the window
                    } // Check if "Back to Menu" button is clicked
                    else if (isMouseOverButton(mousePos, backToMenuPos, backToMenuSize)) {
                        currentGameState = MENU; // Change state to MENU
                        delete game;             // Delete the current game object
                        game = nullptr;          // Set game pointer to null
                        player1Name = "";        // Reset player names
                        player2Name = "";
                        window.setSize({800, 600}); // Reset window size to initial menu size
                        window.setView(sf::View(sf::FloatRect(0, 0, 800, 600))); // Reset view
                    }
                }
            }
        }

        window.clear(sf::Color::White); // Clear the window with a white background

        // Drawing based on current game state
        if (currentGameState == MENU) {
            drawText(window, "TICTACTOE GAME", font, 48, sf::Color(100, 100, 250), window.getSize().x / 2, 60, true);
            drawText(window, "Select Board Size", font, 32, sf::Color::Black, window.getSize().x / 2, 120, true);
            // Draw board size selection buttons
            drawButtonWithHover(window, "3x3", font, 28, {325, 200}, {150, 50}, sf::Color::Green, sf::Color(0, 150, 0), sf::Color::White, mousePos);
            drawButtonWithHover(window, "4x4", font, 28, {325, 270}, {150, 50}, sf::Color::Green, sf::Color(0, 150, 0), sf::Color::White, mousePos);
            drawButtonWithHover(window, "5x5", font, 28, {325, 340}, {150, 50}, sf::Color::Green, sf::Color(0, 150, 0), sf::Color::White, mousePos);
        }
        else if (currentGameState == INPUT_NAMES) {
            drawText(window, "Enter Player Names", font, 36, sf::Color::Black, window.getSize().x / 2, 100, true);
            // Draw input boxes for player names
            drawInputBox(window, "Player 1 (X):", player1Name, font, {250, 200}, isTypingPlayer1);
            drawInputBox(window, "Player 2 (O):", player2Name, font, {250, 280}, isTypingPlayer2);
            // Draw "Start Game" button
            drawButtonWithHover(window, "Start Game", font, 28, {325, 360}, {150, 50}, sf::Color::Red, sf::Color(200, 0, 0), sf::Color::White, mousePos);
        }
        else if (currentGameState == PLAYING || currentGameState == GAME_OVER) {
            // Draw grid lines for the Tic-Tac-Toe board
            for (int i = 1; i < selectedBoardSize; i++) {
                sf::RectangleShape lineH({(float)(CELL_SIZE * selectedBoardSize), 2});
                lineH.setPosition(WINDOW_PADDING, WINDOW_PADDING + i * CELL_SIZE);
                lineH.setFillColor(sf::Color::Black);
                window.draw(lineH);

                sf::RectangleShape lineV({2, (float)(CELL_SIZE * selectedBoardSize)});
                lineV.setPosition(WINDOW_PADDING + i * CELL_SIZE, WINDOW_PADDING);
                lineV.setFillColor(sf::Color::Black);
                window.draw(lineV);
            }
            Board& board = game->getBoard();
            // Draw 'X' or 'O' symbols on the board
            for (int i = 0; i < selectedBoardSize; ++i) {
                for (int j = 0; j < selectedBoardSize; ++j) {
                    char symbol = board.getCell(i, j);
                    if (symbol == 'X' || symbol == 'O') {
                        sf::Text text(string(1, symbol), font, 60);
                        text.setFillColor(sf::Color::Black);
                        text.setPosition(WINDOW_PADDING + j * CELL_SIZE + 25, WINDOW_PADDING + i * CELL_SIZE + 10);
                        window.draw(text);
                    }
                }
            }
            // Display player scores
            string scoreText = game->getPlayer(0).getName() + " (X): " + to_string(game->getPlayer(0).getScore()) +
                               "   " + game->getPlayer(1).getName() + " (O): " + to_string(game->getPlayer(1).getScore());
            drawText(window, scoreText, font, 22, sf::Color::Black, window.getSize().x / 2, WINDOW_PADDING + selectedBoardSize * CELL_SIZE + 20, true);

            if (!game->isGameOver()) {
                // Display current player's turn
                drawText(window, "Current Turn: " + game->getCurrentPlayer().getName() + " (" + game->getCurrentPlayer().getSymbol() + ")", font, 22, sf::Color::Blue, window.getSize().x / 2, WINDOW_PADDING + selectedBoardSize * CELL_SIZE + 55, true);
            } else {
                // Display game result (Win or Draw)
                string result;
                if (board.checkWin(game->getPlayer(0).getSymbol())) result = game->getPlayer(0).getName() + " (X) Wins!";
                else if (board.checkWin(game->getPlayer(1).getSymbol())) result = game->getPlayer(1).getName() + " (O) Wins!";
                else result = "It's a Draw!";
                drawText(window, result, font, 28, sf::Color::Red, window.getSize().x / 2, WINDOW_PADDING - 30, true);
                
                // Redefine button positions for GAME_OVER state (redundant but harmless)
                float centerX = window.getSize().x / 2.f;
                float topY = static_cast<float>(window.getSize().y) - 100;

                sf::Vector2f playAgainPos(centerX - 120, topY);
                sf::Vector2f quitPos(centerX + 10, topY);
                sf::Vector2f backToMenuPos(centerX - 120, topY + 50);
                sf::Vector2f backToMenuSize(230, 40); 

                // Draw end-game buttons
                drawButtonWithHover(window, "Play Again", font, 24, playAgainPos, {140, 40}, sf::Color::Blue, sf::Color(100, 100, 255), sf::Color::White, mousePos);
                drawButtonWithHover(window, "Quit", font, 24, quitPos, {100, 40}, sf::Color::Red, sf::Color(200, 0, 0), sf::Color::White, mousePos);
                drawButtonWithHover(window, "Back to Menu", font, 24, backToMenuPos, backToMenuSize, sf::Color::Yellow, sf::Color(255, 215, 0), sf::Color::Black, mousePos);
            }
        }
        window.display(); // Display the rendered frame
    }

    // Clean up: delete the game object if it was created
    if (game) delete game;
    return 0;
}