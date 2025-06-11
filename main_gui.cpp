#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "TicTacToe.h"
#include "Player.h"

using namespace std;

const int CELL_SIZE = 100;
const int PADDING = 5;
const int WINDOW_PADDING = 50;

enum GameState {
    MENU,
    INPUT_NAMES,
    PLAYING,
    GAME_OVER
};

GameState currentGameState = MENU;
int selectedBoardSize = 3;
string player1Name = "";
string player2Name = "";

TicTacToe* game = nullptr;

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

void drawInputBox(sf::RenderWindow& window, const string& label, const string& text, const sf::Font& font, sf::Vector2f pos, bool focused) {
    sf::RectangleShape box(sf::Vector2f(300, 40));
    box.setPosition(pos);
    box.setFillColor(sf::Color(240, 240, 240));
    box.setOutlineThickness(2);
    box.setOutlineColor(focused ? sf::Color::Blue : sf::Color(200, 200, 200));
    window.draw(box);

    drawText(window, label, font, 18, sf::Color::Black, pos.x, pos.y - 25);
    drawText(window, text, font, 22, sf::Color::Black, pos.x + 10, pos.y + 8);
}

bool isMouseOverButton(sf::Vector2f mousePos, sf::Vector2f buttonPos, sf::Vector2f buttonSize) {
    return mousePos.x > buttonPos.x && mousePos.x < buttonPos.x + buttonSize.x &&
           mousePos.y > buttonPos.y && mousePos.y < buttonPos.y + buttonSize.y;
}

void drawButtonWithHover(sf::RenderWindow& window, const string& text_str, const sf::Font& font, int char_size,
                         sf::Vector2f position, sf::Vector2f size, sf::Color baseColor, sf::Color hoverColor,
                         sf::Color textColor, sf::Vector2f mousePos) {
    sf::Color fillColor = isMouseOverButton(mousePos, position, size) ? hoverColor : baseColor;
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

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic Tac Toe Game");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        cerr << "Can't load font Roboto-Regular.ttf\n";
        return -1;
    }

    bool isTypingPlayer1 = false;
    bool isTypingPlayer2 = false;

    sf::Vector2f backToMenuPos;
    sf::Vector2f backToMenuSize;
   
    while (window.isOpen()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event;

        float centerX = window.getSize().x / 2.f;
        float topY = static_cast<float>(window.getSize().y) - 100;

        sf::Vector2f playAgainPos(centerX - 120, topY);
        sf::Vector2f quitPos(centerX + 10, topY);
        backToMenuPos = sf::Vector2f(centerX - 120, topY + 50);
        backToMenuSize = sf::Vector2f(230, 40);

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (currentGameState == MENU) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (isMouseOverButton(mousePos, {325, 200}, {150, 50})) {
                        selectedBoardSize = 3; currentGameState = INPUT_NAMES;
                    } else if (isMouseOverButton(mousePos, {325, 270}, {150, 50})) {
                        selectedBoardSize = 4; currentGameState = INPUT_NAMES;
                    } else if (isMouseOverButton(mousePos, {325, 340}, {150, 50})) {
                        selectedBoardSize = 5; currentGameState = INPUT_NAMES;
                    }
                }
            } else if (currentGameState == INPUT_NAMES) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (isMouseOverButton(mousePos, {250, 200}, {300, 40})) {
                        isTypingPlayer1 = true; isTypingPlayer2 = false;
                    } else if (isMouseOverButton(mousePos, {250, 280}, {300, 40})) {
                        isTypingPlayer2 = true; isTypingPlayer1 = false;
                    } else if (isMouseOverButton(mousePos, {325, 360}, {150, 50})) {
                        currentGameState = PLAYING;
                        if (game) delete game;
                        game = new TicTacToe(selectedBoardSize, player1Name, player2Name);
                        window.setSize({(unsigned)(CELL_SIZE * selectedBoardSize + 2 * WINDOW_PADDING), (unsigned)(CELL_SIZE * selectedBoardSize + 2 * WINDOW_PADDING + 100)});
                        window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
                    } else {
                        isTypingPlayer1 = isTypingPlayer2 = false;
                    }
                } else if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8) {
                        if (isTypingPlayer1 && !player1Name.empty()) player1Name.pop_back();
                        if (isTypingPlayer2 && !player2Name.empty()) player2Name.pop_back();
                    } else if (event.text.unicode < 128 && event.text.unicode >= 32) {
                        if (isTypingPlayer1 && player1Name.size() < 12) player1Name += static_cast<char>(event.text.unicode);
                        if (isTypingPlayer2 && player2Name.size() < 12) player2Name += static_cast<char>(event.text.unicode);
                    }
                }
            } else if (currentGameState == PLAYING) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x - WINDOW_PADDING;
                    int y = event.mouseButton.y - WINDOW_PADDING;
                    if (x >= 0 && y >= 0 && x < CELL_SIZE * selectedBoardSize && y < CELL_SIZE * selectedBoardSize) {
                        int col = x / CELL_SIZE;
                        int row = y / CELL_SIZE;
                        Player& player = game->getCurrentPlayer();
                        Board& board = game->getBoard();
                        if (board.isValidMove(row, col)) {
                            board.makeMove(row, col, player.getSymbol());
                            if (board.checkWin(player.getSymbol())) {
                                player.addPoint();
                                game->setGameOver(true);
                                currentGameState = GAME_OVER;
                            } else if (board.isFull()) {
                                game->setGameOver(true);
                                currentGameState = GAME_OVER;
                            } else {
                                game->switchTurn();
                            }
                        }
                    }
                }
            } else if (currentGameState == GAME_OVER) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                  if (isMouseOverButton(mousePos, playAgainPos, {140, 40})) {
                        game->resetGame();
                        game->setGameOver(false);
                        currentGameState = PLAYING;
                    } else if (isMouseOverButton(mousePos, quitPos, {100, 40})) {
                        window.close();
                    } else if (isMouseOverButton(mousePos, backToMenuPos, backToMenuSize)) {
                        currentGameState = MENU;
                        delete game;
                        game = nullptr;
                        player1Name = "";
                        player2Name = "";
                        window.setSize({800, 600});
                        window.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        if (currentGameState == MENU) {
            drawText(window, "TICTACTOE GAME", font, 48, sf::Color(100, 100, 250), window.getSize().x / 2, 60, true);
            drawText(window, "Select Board Size", font, 32, sf::Color::Black, window.getSize().x / 2, 120, true);
            drawButtonWithHover(window, "3x3", font, 28, {325, 200}, {150, 50}, sf::Color::Green, sf::Color(0, 150, 0), sf::Color::White, mousePos);
            drawButtonWithHover(window, "4x4", font, 28, {325, 270}, {150, 50}, sf::Color::Green, sf::Color(0, 150, 0), sf::Color::White, mousePos);
            drawButtonWithHover(window, "5x5", font, 28, {325, 340}, {150, 50}, sf::Color::Green, sf::Color(0, 150, 0), sf::Color::White, mousePos);
        }
        else if (currentGameState == INPUT_NAMES) {
            drawText(window, "Enter Player Names", font, 36, sf::Color::Black, window.getSize().x / 2, 100, true);
            drawInputBox(window, "Player 1 (X):", player1Name, font, {250, 200}, isTypingPlayer1);
            drawInputBox(window, "Player 2 (O):", player2Name, font, {250, 280}, isTypingPlayer2);
            drawButtonWithHover(window, "Start Game", font, 28, {325, 360}, {150, 50}, sf::Color::Red, sf::Color(200, 0, 0), sf::Color::White, mousePos);
        }
        else if (currentGameState == PLAYING || currentGameState == GAME_OVER) {
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
            string scoreText = game->getPlayer(0).getName() + " (X): " + to_string(game->getPlayer(0).getScore()) +
                               "   " + game->getPlayer(1).getName() + " (O): " + to_string(game->getPlayer(1).getScore());
            drawText(window, scoreText, font, 22, sf::Color::Black, window.getSize().x / 2, WINDOW_PADDING + selectedBoardSize * CELL_SIZE + 20, true);

            if (!game->isGameOver()) {
                drawText(window, "Current Turn: " + game->getCurrentPlayer().getName() + " (" + game->getCurrentPlayer().getSymbol() + ")", font, 22, sf::Color::Blue, window.getSize().x / 2, WINDOW_PADDING + selectedBoardSize * CELL_SIZE + 55, true);
            } else {
                string result;
                if (board.checkWin(game->getPlayer(0).getSymbol())) result = game->getPlayer(0).getName() + " (X) Wins!";
                else if (board.checkWin(game->getPlayer(1).getSymbol())) result = game->getPlayer(1).getName() + " (O) Wins!";
                else result = "It's a Draw!";
                drawText(window, result, font, 28, sf::Color::Red, window.getSize().x / 2, WINDOW_PADDING - 30, true);
               
                float centerX = window.getSize().x / 2.f;
                float topY = static_cast<float>(window.getSize().y) - 100;

                sf::Vector2f playAgainPos(centerX - 120, topY);
                sf::Vector2f quitPos(centerX + 10, topY);
                sf::Vector2f backToMenuPos(centerX - 120, topY + 50);
                sf::Vector2f backToMenuSize(230, 40); // 140 + 100 - 10 spacing

                drawButtonWithHover(window, "Play Again", font, 24, playAgainPos, {140, 40}, sf::Color::Blue, sf::Color(100, 100, 255), sf::Color::White, mousePos);
                drawButtonWithHover(window, "Quit", font, 24, quitPos, {100, 40}, sf::Color::Red, sf::Color(200, 0, 0), sf::Color::White, mousePos);
                drawButtonWithHover(window, "Back to Menu", font, 24, backToMenuPos, backToMenuSize, sf::Color::Yellow, sf::Color(255, 215, 0), sf::Color::Black, mousePos);

            }
        }
        window.display();
    }

    if (game) delete game;
    return 0;
}



