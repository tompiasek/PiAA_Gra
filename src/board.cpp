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
	
	whitePieces = 4 * startRows;
	blackPieces = 4 * startRows;
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
	if (mandatoryJump) {
		if (!isJump(startRow, startCol, endRow, endCol)) {
			std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> jumps = allAvailableJumps(board[startRow][startCol]->getColor());
			if (jumps.size() > 1) {
				std::cerr << "Mandatory jump! [movePiece()]\n";
				std::cout << "Available jumps:\n";
				for (const auto& jump : jumps) {
					std::cout << "[" << jump.first.first << ", " << jump.first.second << "] -> [" << jump.second.first << ", " << jump.second.second << "]\n";
				}
				return false;
			}
			else if (jumps.size() == 1) {
				std::cerr << "Mandatory jump! [movePiece()]\n";
				std::cout << "Available jump: [" << jumps[0].first.first << ", " << jumps[0].first.second << "] -> [" << jumps[0].second.first << ", " << jumps[0].second.second << "]\n";
				return false;
			}
		}
	}

	std::vector<std::pair<int, int>> validMoves = getValidMoves(startRow, startCol);
	bool valid = false;
	for (const auto& move : validMoves) {
		if (move.first == endRow && move.second == endCol) {
			valid = true;
		}
	}
	if (!valid) {
		std::cerr << "Invalid move! [movePiece()]\n";
		return false;
	}

	if (isJump(startRow, startCol, endRow, endCol)) {
		jumpPiece(startRow, startCol, endRow, endCol);
	}
	else return move(startRow, startCol, endRow, endCol);
}

bool Board::jumpPiece(int startRow, int startCol, int endRow, int endCol) {
	if (!isJump(startRow, startCol, endRow, endCol)) {
		std::cerr << "Invalid jump! [jumpPiece()]\n";
		return false;
	}

	int jumpRow = (startRow + endRow) / 2;
	int jumpCol = (startCol + endCol) / 2;

	if (board[jumpRow][jumpCol]->isKing()) {
		board[jumpRow][jumpCol]->getColor() == WHITE ? whiteKings-- : blackKings--;
	}
	

	board[jumpRow][jumpCol]->setType(EMPTY);
	board[jumpRow][jumpCol]->setColor(NONE);

	board[endRow][endCol]->setColor(board[startRow][startCol]->getColor());
	board[endRow][endCol]->setType(board[startRow][startCol]->getType());

	if ((endRow == 0 && board[endRow][endCol]->getColor() == BLACK) || (endRow == BOARD_SIZE - 1 && board[endRow][endCol]->getColor() == WHITE)) {
		board[endRow][endCol]->promote();
		std::cout << "Promoted!\n";
	}

	board[startRow][startCol]->getColor() == WHITE ? whitePieces-- : blackPieces--;

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
	if (backwardJump || piece->isKing()) availableDirections = 4;

	bool jumped = false;

	// Check for jumps
	std::vector<std::pair<int, int>> validJumps = getValidJumps(row, col);
	if (validJumps.size() > 0) jumped = true;
	for (const auto& jump : validJumps) {
		validMoves.push_back(jump);
	}

	if (jumped && mandatoryJump) {
		std::cout << "Mandatory jump! Returning just jumps.\n";
		return validMoves;
	}

	// Check for moves
	for (int i = 0; i < 2; i++) {
		auto direction = directions[i];
		int newRow = row + direction.first;
		int newCol = col + direction.second;

		if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) continue; // Out of bounds

		if (board[newRow][newCol]->getType() == EMPTY) {
			validMoves.push_back(std::make_pair(newRow, newCol));
			std::cout << "Valid move: (" << newRow << ", " << newCol << ")\n";
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

std::vector<std::pair<int, int>> Board::getValidJumps(int row, int col) const {
	std::vector<std::pair<int, int>> validJumps;

	if (!isOccupied(row, col)) {
		std::cerr << "Field is not occupied! [getValidJumps()]\n";
		return validJumps;
	}

	Piece* piece = board[row][col];

	std::vector<std::pair<int, int>> directions;
	// First two directions are forward and two last are backward for each color (BLACK->UP, WHITE->DOWN)
	if (piece->getColor() == BLACK) directions = { {-1, 1}, {-1, -1}, {1, 1}, {1, -1} };
	else directions = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	int availableDirections = 2;
	if (backwardJump || piece->isKing()) availableDirections = 4;

	int newRow, newCol;

	// Check for jumps
	if (!piece->isKing()) {
		for (int i = 0; i < availableDirections; i++) {
			auto direction = directions[i];
			newRow = row + direction.first;
			newCol = col + direction.second;

			if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) continue; // Out of bounds

			if (board[row][col]->getColor() != board[newRow][newCol]->getColor() && board[newRow][newCol]->getColor() != NONE) {
				int jumpRow = newRow + direction.first;
				int jumpCol = newCol + direction.second;

				if (jumpRow < 0 || jumpRow >= BOARD_SIZE || jumpCol < 0 || jumpCol >= BOARD_SIZE) continue; // Out of bounds

				if (board[jumpRow][jumpCol]->getType() == EMPTY) {
					validJumps.push_back(std::make_pair(jumpRow, jumpCol));
					std::cout << "Valid jump: (" << jumpRow << ", " << jumpCol << ")\n";
				}
			}
		}
		return validJumps;
	}
	else {
		std::cout << "King! [getValidjumps()]\n";
		for (int i = 0; i < availableDirections; i++) {
			auto direction = directions[i];
			int jumpRow = row + direction.first;
			int jumpCol = col + direction.second;
			newRow = jumpRow + direction.first;
			newCol = jumpCol + direction.second;

			while (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE)  {
				if (board[jumpRow][jumpCol]->getColor() != piece->getColor() && board[jumpRow][jumpCol]->getColor() != NONE && board[newRow][newCol]->getColor() == NONE) {
					validJumps.push_back(std::make_pair(newRow, newCol));
					std::cout << "Valid jump: (" << newRow << ", " << newCol << ")\n";
				}

				jumpRow += direction.first;
				jumpCol += direction.second;
				newRow += direction.first;
				newCol += direction.second;
					
			}
		}
	}
	
	return validJumps;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Board::allAvailableMoves(int player) const {
	if (player != WHITE && player != BLACK) {
		std::cerr << "Invalid player! [allAvailableMoves()]\n";
		return std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>();
	}

	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> moves;

	if (mandatoryJump) {
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> jumps = allAvailableJumps(player);
		if (jumps.size() > 0) return jumps;
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j]->getColor() == player) {
				std::vector<std::pair<int, int>> validMoves = getValidMoves(i, j);
				for (const auto& move : validMoves) {
					moves.push_back(std::make_pair(std::make_pair(i, j), move));
				}
			}
		}
	}

	return moves;
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Board::allAvailableJumps(int player) const {
	if (player != WHITE && player != BLACK) {
		std::cerr << "Invalid player! [allAvailableJumps()]\n";
		return std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>();
	}

	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> jumps;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j]->getColor() == player) {
				std::vector<std::pair<int, int>> validMoves = getValidMoves(i, j);
				for (const auto& move : validMoves) {
					if (isJump(i, j, move.first, move.second)) {
						jumps.push_back(std::make_pair(std::make_pair(i, j), std::make_pair(move.first, move.second)));
					}
				}
			}
		}
	}

	return jumps;
}

int Board::evaluate() const {
	return whitePieces - blackPieces + 2 * (whiteKings - blackKings);
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
		if (!board[startRow][startCol]->isKing()) {
			int jumpRow = (startRow + endRow) / 2;
			int jumpCol = (startCol + endCol) / 2;

			if (board[jumpRow][jumpCol]->getType() == EMPTY) {
				std::cerr << "Jump field is empty! [isValidMove()]\n";
				return false;
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
		}
		else {
			// Is king
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

	if (!board[startRow][startCol]->isKing()) {
		int jumpRow = (startRow + endRow) / 2;
		int jumpCol = (startCol + endCol) / 2;

		// Check if there is a piece to jump over
		if (!isOccupied(jumpRow, jumpCol)) {
			std::cerr << "No piece to jump over! [isJump()]\n";
			return false;
		}

		// Check if jumped piece is of different color
		if (board[startRow][startCol]->getColor() == board[jumpRow][jumpCol]->getColor()) {
			std::cerr << "Jumped piece is of the same color! [isJump()]\n";
			return false;
		}

		// Check if number of fields jumped is correct
		if (abs(startRow - endRow) != 2 || abs(startCol - endCol) != 2) {
			return false;
		}
	}
	else {
		std::vector<std::pair<int, int>> directions;
		if (board[startRow][startCol]->getColor() == BLACK) directions = { {-1, 1}, {-1, -1}, {1, 1}, {1, -1} };
		else directions = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

		for (int i = 0; i < 4; i++) {
			auto direction = directions[i];
			std::cout << "Direction: " << direction.first << ", " << direction.second << "\n"; // Debug
			int newRow = startRow + direction.first;
			int newCol = startCol + direction.second;

			if (newRow < 0 || newRow >= BOARD_SIZE || newCol < 0 || newCol >= BOARD_SIZE) continue; // Out of bounds

			if (board[startRow][startCol]->getColor() != board[newRow][newCol]->getColor() && board[newRow][newCol]->getColor() != NONE) {
				int jumpRow = newRow + direction.first;
				int jumpCol = newCol + direction.second;

				std::cout << "Jump: " << jumpRow << ", " << jumpCol << "\n"; // Debug

				if (jumpRow < 0 || jumpRow >= BOARD_SIZE || jumpCol < 0 || jumpCol >= BOARD_SIZE) continue; // Out of bounds

				if (board[jumpRow][jumpCol]->getType() == EMPTY) {
					if (jumpRow == endRow && jumpCol == endCol) return true;
				}
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