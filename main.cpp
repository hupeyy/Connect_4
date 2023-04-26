#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"

using namespace sf;
using namespace std;

int main() {
    int width = 1024;
    int height = 896;

    RenderWindow window(VideoMode(width, height), "Connect 4");

    Board gameBoard = Board();
    gameBoard.setBoard();

    // red = 1 | yellow = -1
    int player = 1;
    while (window.isOpen())
    {
        Event event;
        Text text;
        Font font;

        font.loadFromFile("PressStart2P-Regular.ttf");
        text.setFont(font);
        text.setCharacterSize(32);
        text.setFillColor(Color::White);
        text.setPosition(360, 830);

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed && player == 1)
            {
                int col = gameBoard.selectCol(event.mouseButton.x);
                if (!gameBoard.colFull(col))
                {
                    gameBoard.makeMove(player, col);
                    player *= -1;
                }
            }
            if (player == -1)
            {
                int bestMove = gameBoard.miniMax(player, gameBoard, 4, INT_MIN, INT_MAX).first;
                if (!gameBoard.colFull(bestMove))
                {
                    gameBoard.makeMove(player, bestMove);
                    player *= -1;
                }
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
            {
                gameBoard.resetBoard();
                gameBoard.setBoard();
                player = 1;
            }
        }
        window.clear();
        if (gameBoard.isTerminalState())
        {
            if (gameBoard.checkWin(1))
                text.setString("Red wins!");
            else if (gameBoard.checkWin(-1))
                text.setString("Yellow wins!");
            else
                text.setString("Draw!");
        }
        window.draw(text);
        gameBoard.drawBoard(window);
        window.display();
    }
    return 0;
}
