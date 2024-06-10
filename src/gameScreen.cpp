#include "gameScreen.hpp"

int GameScreen::Run(sf::RenderWindow &App) {
    App.clear(sf::Color::White);
    App.setSize(sf::Vector2u(BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE));
    App.setView(sf::View(sf::FloatRect(0, 0, BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE)));
    App.setMouseCursorVisible(true);

	bool running = true;
    while (running) {
        sf::Event event;
        while (App.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return -1;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }

        App.clear(sf::Color::White);
        drawBoard(App);
        App.display();
    }
    return -1;
}

void GameScreen::drawBoard(sf::RenderWindow &App) {
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            tile.setPosition(col * TILE_SIZE, row * TILE_SIZE);

            if ((row + col) % 2 == 0) {
                tile.setFillColor(sf::Color::White);
            } else {
                tile.setFillColor(sf::Color(139, 69, 19)); // Brown color
            }

            App.draw(tile);

            Piece* piece = board.getPiece(row, col);
            sf::Sprite sprite;
            if (piece->getType() == PAWN) {
                sprite = (piece->getColor() == WHITE) ? whitePiece : blackPiece;
                sprite.setPosition(col * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2), row * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2));
            } else if (piece->getType() == KING) {
                sprite = (piece->getColor() == WHITE) ? whiteKing : blackKing;
                sprite.setPosition(col * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2), row * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2));
            }
            App.draw(sprite);
        }
    }

    // Draw the valid move tiles
    for (const auto& validMoveTile : validMoveTiles) {
        App.draw(validMoveTile);
    }

    // Draw the selected piece highlight
    if (pieceSelected) {
		App.draw(selectedPieceHighlight);
        Piece* piece = board.getPiece(selectedPiecePosition.x, selectedPiecePosition.y);

        sf::Sprite sprite;
        if (piece->getType() == PAWN) {
            sprite = (piece->getColor() == WHITE) ? whitePiece : blackPiece;
            sprite.setPosition(selectedPiecePosition.y * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2), selectedPiecePosition.x * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2));
        } else if (piece->getType() == KING) {
            sprite = (piece->getColor() == WHITE) ? whiteKing : blackKing;
            sprite.setPosition(selectedPiecePosition.y * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2), selectedPiecePosition.x * TILE_SIZE + ((TILE_SIZE - IMG_SIZE) / 2));
        }
        App.draw(sprite);
	}
}

void GameScreen::handleMouseClick(int mouseX, int mouseY) {
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
        std::vector<std::pair<int, int>> validMoves;
        // Check if there are any jumps available
        if(board.allAvailableJumps(board.getPiece(row, col)->getColor()).size() == 0) validMoves = board.getValidMoves(row, col);
		else {
            // If there are jumps available, only show the jumps
            if(board.getValidJumps(row, col).size() == 0) return;
            validMoves = board.getValidJumps(row, col);
        }

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

        bool moveSuccess = false;
        if (board.isJump(startRow, startCol, endRow, endCol)) {
            moveSuccess = board.movePiece(startRow, startCol, endRow, endCol);
            // If a jump was made, check if there are more jumps available
            if (moveSuccess && board.getValidJumps(endRow, endCol).size() > 0) {
				// If there are more jumps available, keep the piece selected
				pieceSelected = true;
				selectedPiecePosition = sf::Vector2i(endRow, endCol);
				selectedPieceHighlight.setPosition(endCol * TILE_SIZE + TILE_SIZE / 2, endRow * TILE_SIZE + TILE_SIZE / 2);
				validMoveTiles.clear(); // Clear the valid move tiles vector
                for (const auto& move : board.getValidJumps(endRow, endCol)) {
					int jumpEndRow = move.first;
					int jumpEndCol = move.second;
					sf::RectangleShape validMoveTile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
					validMoveTile.setPosition(jumpEndCol * TILE_SIZE, jumpEndRow * TILE_SIZE);
					validMoveTile.setFillColor(sf::Color(0, 255, 0, 128)); // Semi-transparent green
					validMoveTiles.push_back(validMoveTile); // Add the valid move tile to the vector
				}
            }
            else {
				// If there are no more jumps available, switch turns
				isWhiteTurn = !isWhiteTurn;
			}
        }
        else {
			if (board.movePiece(startRow, startCol, endRow, endCol)) isWhiteTurn = !isWhiteTurn;
		}

        // Reset selection state
        pieceSelected = false;
        validMoveTiles.clear(); // Clear the valid move tiles vector
    }
}