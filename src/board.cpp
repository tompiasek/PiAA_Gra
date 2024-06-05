#include "board.hpp"

#include <iostream>

Board::Board(bool backJump, bool mandJump, int startRows) : backwardJump(backJump), mandatoryJump(mandJump) {
	if(startRows * 2  >= BOARD_SIZE || startRows < 1) {
		std::cerr << "Wrong number of start rows!\nNumber of start rows set to default [3]\n";
		startRows = 3;
	}

	noStartRows = startRows;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = new Piece(EMPTY);
		}
	}

	for (int i = 0; i < noStartRows; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2 == 0) board[i][j] = new Piece(PAWN, WHITE);
		}
	}
	for (int i = BOARD_SIZE - 1; i > BOARD_SIZE - 1 - noStartRows; i--) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2 == 0) board[i][j] = new Piece(PAWN, BLACK);
		}
	}
}

void Board::display() const {
	std::cout << "  ";
	for (int i = 65; i < 65 + BOARD_SIZE; i++) {
		std::cout << char(i) << " ";
	}
	std::cout << "\n  ";
	for (int i = 0; i < BOARD_SIZE; i++) {
		std::cout << "--";
	}
	std::cout << "\n";
	for (int i = 0; i < BOARD_SIZE; i++) {
		std::cout << i << "|";
		for (int j = 0; j < BOARD_SIZE; j++) {
			std::cout << board[i][j]->getColor() << " ";
		}
		std::cout << std::endl;
	}
}

void Board::reset() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = new Piece(EMPTY);
		}
	}

	for (int i = 0; i < noStartRows; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2 == 0) board[i][j] = new Piece(PAWN, WHITE);
		}
	}
	for (int i = BOARD_SIZE - 1; i > BOARD_SIZE - 1 - noStartRows; i--) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2 == 0) board[i][j] = new Piece(PAWN, BLACK);
		}
	}
}

bool Board::movePiece(int startRow, int startCol, int endRow, int endCol) {
	if (isJump(startRow, startCol, endRow, endCol)) {
		jumpPiece(startRow, startCol, endRow, endCol);
	}
	else {
		if (!isValidMove(startRow, startCol, endRow, endCol)) {
			std::cerr << "Invalid move! [movePiece()]\n";
			return false;
		}
		return move(startRow, startCol, endRow, endCol);
	}
}

bool Board::jumpPiece(int startRow, int startCol, int endRow, int endCol) {
	if (!isJump(startRow, startCol, endRow, endCol)) {
		std::cerr << "Invalid jump! [jumpPiece()]\n";
		return false;
	}

	int jumpRow = (startRow + endRow) / 2;
	int jumpCol = (startCol + endCol) / 2;

	board[jumpRow][jumpCol]->setType(EMPTY);
	board[jumpRow][jumpCol]->setColor(NONE);

	board[endRow][endCol]->setColor(board[startRow][startCol]->getColor());
	board[endRow][endCol]->setType(board[startRow][startCol]->getType());

	if ((endRow == 0 && board[endRow][endCol]->getColor() == BLACK) || (endRow == BOARD_SIZE - 1 && board[endRow][endCol]->getColor() == WHITE)) {
		board[endRow][endCol]->promote();
		std::cout << "Promoted!\n";
	}
	board[startRow][startCol]->setType(EMPTY);
	board[startRow][startCol]->setColor(NONE);

	std::cout << "Piece jumped from (" << startRow << ", " << startCol << ") to (" << endRow << ", " << endCol << ")\n";

	return true;
}

std::vector<std::pair<int, int>> Board::getValidMoves(int row, int col) const {
	std::vector<std::pair<int, int>> validMoves;

	if (!isOccupied(row, col)) {
		std::cerr << "Field is not occupied! [getValidMoves()]\n";
		return validMoves;
	}

	Piece* piece = board[row][col];

	std::vector<std::pair<int, int>> directions;
	// First two directions are forward and two last are backward for each color (BLACK->UP, WHITE->DOWN)
	if (piece->getColor() == BLACK) directions = { {-1, 1}, {-1, -1}, {1, 1}, {1, -1} };
	else directions = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	int availableDirections = 2;
	if (backwardJump) availableDirections = 4;

	bool jumped = false;

	// Check for jumps
	for (int i = 0; i < availableDirections; i++) {
		auto direction = directions[i];
		int newRow = row + direction.first;
		int newCol = col + direction.second;

		if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) continue; // Out of bounds

		if (board[row][col]->getColor() != board[newRow][newCol]->getColor() && board[newRow][newCol]->getColor() != NONE) {
			int jumpRow = newRow + direction.first;
			int jumpCol = newCol + direction.second;

			if (jumpRow < 0 || jumpRow >= BOARD_SIZE || jumpCol < 0 || jumpCol >= BOARD_SIZE) continue; // Out of bounds

			if (board[jumpRow][jumpCol]->getType() == EMPTY) {
				validMoves.push_back(std::make_pair(jumpRow, jumpCol));
				jumped = true;
			}
		}
	}
	if (jumped) return validMoves;

	// Check for moves
	for (int i = 0; i < 2; i++) {
		auto direction = directions[i];
		int newRow = row + direction.first;
		int newCol = col + direction.second;

		if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) continue; // Out of bounds

		if (board[newRow][newCol]->getType() == EMPTY) {
			validMoves.push_back(std::make_pair(newRow, newCol));
		}
	}

	return validMoves;
}

bool Board::isOccupied(int row, int col) const {
	if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return false;

	if (board[row][col]->getType() == EMPTY) return false;
	else return true;
}

Piece* Board::getPiece(int row, int col) const {
	if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
		std::cerr << "Out of bounds! [getPiece()]\n";
		return nullptr;
	}

	return board[row][col];
}

bool Board::isValidMove(int startRow, int startCol, int endRow, int endCol) const {
	if (startRow < 0 || startRow >= BOARD_SIZE || startCol < 0 || startCol >= BOARD_SIZE || endRow < 0 || endRow >= BOARD_SIZE || endCol < 0 || endCol >= BOARD_SIZE) {
		std::cerr << "Out of bounds! [isValidMove()]\n";
		return false;
	}

	if (!isOccupied(startRow, startCol)) {
		std::cerr << "Start field is empty! [isValidMove()]\n";
		return false;
	}

	if (isOccupied(endRow, endCol)) {
		std::cerr << "End field is occupied by other piece! [isValidMove()]\n";
		return false;
	}

	if (isJump(startRow, startCol, endRow, endCol)) {
		int jumpRow = (startRow + endRow) / 2;
		int jumpCol = (startCol + endCol) / 2;

		if (board[jumpRow][jumpCol]->getType() == EMPTY) {
			std::cerr << "Jump field is empty! [isValidMove()]\n";
			return false;
		}
	}

	if (board[startRow][startCol]->getColor() == WHITE) {
		if (endRow - startRow != 1 || abs(startCol - endCol) != 1) {
		std::cerr << "Invalid move! [isValidMove()]\n";
		return false;
		}
	} else if (board[startRow][startCol]->getColor() == BLACK) {
		if (endRow - startRow != -1 || abs(startCol - endCol) != 1) {
			std::cerr << "Invalid move! [isValidMove()]\n";
			return false;
		}
	}

	return true;
}

bool Board::isJump(int startRow, int startCol, int endRow, int endCol) const {
	if (startRow < 0 || startRow >= BOARD_SIZE || startCol < 0 || startCol >= BOARD_SIZE || endRow < 0 || endRow >= BOARD_SIZE || endCol < 0 || endCol >= BOARD_SIZE) {
		std::cerr << "Out of bounds! [isJump()]\n";
		return false;
	}

	if (!isOccupied(startRow, startCol)) {
		std::cerr << "Start field is empty! [isJump()]\n";
		return false;
	}

	if (isOccupied(endRow, endCol)) {
		return false;
	}

	int jumpRow = (startRow + endRow) / 2;
	int jumpCol = (startCol + endCol) / 2;

	// Check if there is a piece to jump over
	if (board[jumpRow][jumpCol]->getType() == EMPTY) {
		std::cerr << "No piece to jump over! [isJump()]\n";
		return false;
	}

	// Check if jumped piece is of different color
	if (board[startRow][startCol]->getColor() == board[jumpRow][jumpCol]->getColor()) {
		std::cerr << "Jumped piece is of the same color! [isJump()]\n";
		return false;
	}

	// Check if number of fields jumped is correct
	if (backwardJump) {
		if (abs(startRow - endRow) != 2 || abs(startCol - endCol) != 2) {
			return false;
		}
	}
	else {
		if (board[startRow][startCol]->getColor() == WHITE) {
			if (endRow - startRow != 2 || abs(startCol - endCol) != 2) {
				return false;
			}
		}
		else if (board[startRow][startCol]->getColor() == BLACK) {
			if (endRow - startRow != -2 || abs(startCol - endCol) != 2) {
				return false;
			}
		
		}
	}

	return true;
}

bool Board::move(int startRow, int startCol, int endRow, int endCol) {
	if (isJump(startRow, startCol, endRow, endCol)) {
		std::cerr << "Jump is required! [move()]\n";
		return false;
	}
	
	if (!isValidMove(startRow, startCol, endRow, endCol)) {
		std::cerr << "Invalid move! [move()]\n";
		return false;
	}

	// Move piece
	board[endRow][endCol]->setColor(board[startRow][startCol]->getColor());
	board[endRow][endCol]->setType(board[startRow][startCol]->getType());

	if ((endRow == 0 && board[endRow][endCol]->getColor() == BLACK) || (endRow == BOARD_SIZE - 1 && board[endRow][endCol]->getColor() == WHITE)) {
		board[endRow][endCol]->promote();
		std::cout << "Promoted!\n";
	}
	board[startRow][startCol]->setType(EMPTY);
	board[startRow][startCol]->setColor(NONE);

	std::cout << "Piece moved from (" << startRow << ", " << startCol << ") to (" << endRow << ", " << endCol << ")\n";

	return true;
}