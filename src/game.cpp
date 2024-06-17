#include "game.hpp"
#include "algorithm.hpp"

Game::Game(bool backJump, bool mandJump, int noStartRows, bool turn) {
	board = new Board(backJump, mandJump, noStartRows);
	currentPlayer = turn;
	start();
}

Game::Game(const Game& g) {
	this->board = new Board(*g.board);
	this->currentPlayer = g.currentPlayer;
	this->gameOver = g.gameOver;
	this->moves = g.moves;
}

Game::~Game() {
	delete board;
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
	else return nullptr;
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
	if (board->whitePieces == 0 || board->blackPieces == 0) {
		gameOver = 1;
	}
	return gameOver;
}

Board* Game::getCurrentState() const {
	return board;
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

Board* Game::aiMove(Board* b) {
	this->board = b;
    std::pair<int, Board> bestMove = minimax(std::pair<int,int>(-1, -1), 3, true, *this);
    this->board = new Board(bestMove.second);
	// TO-DO: Implement multiple jumps for AI
	/*auto jumps = this->board->allAvailableJumps(1);
	if (jumps.size() > 0) {
		std::pair<int, Board> bestMove = minimax(std::pair<int,int>(-1, -1), 2, true, *this);
		this->board = new Board(bestMove.second);
	}*/
    switchTurn();
    return board;
}
