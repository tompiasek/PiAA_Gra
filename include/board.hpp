#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <vector>
#include "piece.hpp"

const int BOARD_SIZE = 8;

class Board {
	Piece* board[BOARD_SIZE][BOARD_SIZE];
	bool backwardJump = false,
		 mandatoryJump = false;
	int noStartRows = 3;
	
public:
	Board(bool backJump = false, bool mandJump = false, int noStartRows = 3);

	void display() const;
	void reset();
	//void changeBoardSize(int newSize);
	//void setBackwardJump(bool backJump);

	bool movePiece(int startRow, int startCol, int endRow, int endCol);
	bool jumpPiece(int startRow, int startCol, int endRow, int endCol);
	bool isOccupied(int row, int col) const;

	Piece* getPiece(int row, int col) const;
	std::vector<std::pair<int, int>> getValidMoves(int row, int col) const;

private:
	bool isValidMove(int startRow, int startCol, int endRow, int endCol) const;
	bool isJump(int startRow, int startCol, int endRow, int endCol) const;
	bool move(int startRow, int startCol, int endRow, int endCol);
};


#endif // !BOARD_HPP_