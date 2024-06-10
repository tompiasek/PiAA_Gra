#include "game.hpp"

Game::Game(bool backJump, bool mandJump, int noStartRows) {
	board = new Board(backJump, mandJump, noStartRows);
	start();
}

void Game::start() {
	gameOver = 0;
	currentPlayer = WHITE - 1;
}

void Game::restart() {
	board->reset();
	start();
}

Board* Game::playTurn(int startRow, int startCol, int endRow, int endCol) {
	if (gameOver) return nullptr;
	if (board->movePiece(startRow, startCol, endRow, endCol)) {
			currentPlayer = !currentPlayer;
	}
}

// TO-DO
bool Game::isGameOver() {
	return gameOver;
}

Board Game::getCurrentState() const {
	return *board;
}

void Game::saveGame(std::string path) const {
	return;
}

void Game::loadGame(std::string path) {
	return;
}

