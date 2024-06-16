#include "game.hpp"
#include "algorithm.hpp"

Game::Game(bool backJump, bool mandJump, int noStartRows, bool turn) {
	board = new Board(backJump, mandJump, noStartRows);
	currentPlayer = turn;
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
			switchTurn();
	}
	else {
		std::cerr << "Invalid move" << std::endl;
		return nullptr;
	}
	return board;
}

void Game::switchTurn() {
	currentPlayer = !currentPlayer;
}

bool Game::getTurn() const {
	return currentPlayer;
}

// TO-DO
bool Game::isGameOver() {
	return gameOver;
}

Board Game::getCurrentState() const {
	return *board;
}

void Game::setCurrentState(Board* b) {
	board = b;
}

void Game::saveGame(std::string path) const {
	return;
}

void Game::loadGame(std::string path) {
	return;
}

Board* Game::aiMove(Board b) {
	//board = b;
    std::pair<int, Board> bestMove = minimax(std::pair<int,int>(-1, -1), 5, true, this);
    board = new Board(bestMove.second);
    switchTurn();
    return board;
}

