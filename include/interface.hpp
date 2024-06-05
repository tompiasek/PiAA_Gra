#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "board.hpp"

const int TILE_SIZE = 80; // Size of each tile on the board
const int IMG_SIZE = 64; // Size of each piece image

class CheckersGame {
private:
    sf::RenderWindow window;
    Board board; // Assuming you have the Board class from your question

    sf::Texture whiteTexture, blackTexture, crownTexture;
    sf::Sprite whitePiece, blackPiece, crown;

    bool isWhiteTurn;
    bool pieceSelected;
    sf::Vector2i selectedPiecePosition;

    std::vector<sf::RectangleShape> validMoveTiles; // Store the valid move tiles
    sf::CircleShape selectedPieceHighlight; // Highlight the selected piece

public:
    CheckersGame() : window(sf::VideoMode(BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE), "SFML Checkers"), board(true, true, 3), isWhiteTurn(true), pieceSelected(false) {
        if (!whiteTexture.loadFromFile("C:/Users/tompi/source/repos/PiAA-Project_3-Game/external/img/piece_white.png")) {
            // Handle error
        }
        if (!blackTexture.loadFromFile("C:/Users/tompi/source/repos/PiAA-Project_3-Game/external/img/piece_black.png")) {
            // Handle error
        }
        if (!crownTexture.loadFromFile("C:/Users/tompi/source/repos/PiAA-Project_3-Game/external/img/piece_white.png")) {
            // Handle error
        }

        whitePiece.setTexture(whiteTexture);
        blackPiece.setTexture(blackTexture);
        crown.setTexture(crownTexture);

        run();
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                }
            }

            window.clear(sf::Color::White);
            drawBoard();
            window.display();
        }
    }

    void drawBoard() {
        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);

                if ((row + col) % 2 == 0) {
                    tile.setFillColor(sf::Color::White);
                } else {
                    tile.setFillColor(sf::Color(139, 69, 19)); // Brown color
                }

                window.draw(tile);

                Piece* piece = board.getPiece(row, col);
                if (piece->getType() == PAWN) {
                    sf::Sprite sprite = (piece->getColor() == WHITE) ? whitePiece : blackPiece;
                    sprite.setPosition(col * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2), row * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2));
                    window.draw(sprite);
                } else if (piece->getType() == KING) {
                    crown.setPosition(col * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2), row * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2));
                    window.draw(crown);
                }
            }
        }

        // Draw the valid move tiles
        for (const auto& validMoveTile : validMoveTiles) {
            window.draw(validMoveTile);
        }

        // Draw the selected piece highlight
        if (pieceSelected) {
			window.draw(selectedPieceHighlight);
            Piece* piece = board.getPiece(selectedPiecePosition.x, selectedPiecePosition.y);
            sf::Sprite sprite = (piece->getColor() == WHITE) ? whitePiece : blackPiece;
            sprite.setPosition(selectedPiecePosition.y * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2), selectedPiecePosition.x * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2));
            window.draw(sprite);
		}
    }

    void handleMouseClick(int mouseX, int mouseY) {
        int row = mouseY / TILE_SIZE;
        int col = mouseX / TILE_SIZE;

        // If no piece is selected and the clicked tile has a piece of the current player's color
        if (!pieceSelected && board.getPiece(row, col)->getColor() == (isWhiteTurn ? WHITE : BLACK)) {
            pieceSelected = true;
            selectedPiecePosition = sf::Vector2i(row, col);
            selectedPieceHighlight.setRadius(IMG_SIZE / 2 + 6);
            selectedPieceHighlight.setFillColor(sf::Color(255, 255, 0));
            selectedPieceHighlight.setOrigin(IMG_SIZE / 2 + 6, IMG_SIZE / 2 + 6);
            selectedPieceHighlight.setPosition(col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2);
            std::vector<std::pair<int, int>> validMoves = board.getValidMoves(row, col);
            for (const auto& move : validMoves) {
                int endRow = move.first;
                int endCol = move.second;
                sf::RectangleShape validMoveTile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                validMoveTile.setPosition(endCol * TILE_SIZE, endRow * TILE_SIZE);
                validMoveTile.setFillColor(sf::Color(0, 255, 0, 128)); // Semi-transparent green
                validMoveTiles.push_back(validMoveTile); // Add the valid move tile to the vector
            }
        } else if (pieceSelected) {
            // Try to move the selected piece to the clicked tile
            int startRow = selectedPiecePosition.x;
            int startCol = selectedPiecePosition.y;
            int endRow = row;
            int endCol = col;

            if (board.movePiece(startRow, startCol, endRow, endCol)) {
                // Move successful, switch turns
                isWhiteTurn = !isWhiteTurn;
            }

            // Reset selection state
            pieceSelected = false;
            validMoveTiles.clear(); // Clear the valid move tiles vector
        }
    }
};

#endif // !INTERFACE_HPP_