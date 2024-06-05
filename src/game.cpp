#include "game.hpp"

Game::Game(bool backJump, bool mandJump, int noStartRows) {
	currentState = new Board(backJump, mandJump, noStartRows);
	start();
}

void Game::start() {
	gameOver = 0;
	currentPlayer = WHITE - 1;
}

void Game::restart() {
	currentState->reset();
	start();
}

void Game::playTurn(int startRow, int startCol, int endRow, int endCol) {
	if (gameOver) return;
	if (currentState->movePiece(startRow, startCol, endRow, endCol)) {
			currentPlayer = !currentPlayer;
	}
}

// TO-DO
bool Game::isGameOver() {
	return gameOver;
}

Board Game::getCurrentState() const {
	return *currentState;
}

void Game::saveGame(std::string path) const {
	return;
}

void Game::loadGame(std::string path) {
	return;
}

