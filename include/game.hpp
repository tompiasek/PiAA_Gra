#ifndef GAME_HPP_
#define GAME_HPP_

#include <iostream>
#include "board.hpp"

struct Move {
	int color, startRow, startCol, endRow, endCol;
	bool jump;
};

class Game {
	Board* currentState;
	bool currentPlayer;
	bool gameOver;
	std::vector<Move> moves;

public:
	Game();
	void start();
	void restart();
	void playTurn(int startRow, int startCol, int endRow, int endCol);
	bool isGameOver() const;
	Board getCurrentState() const;
	void saveGame(std::string path) const;
	void loadGame(std::string path);
};

#endif // !GAME_HPP_