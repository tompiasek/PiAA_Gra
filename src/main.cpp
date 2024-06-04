#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "game.hpp"

using namespace std;

int main(int argc, char** argv)
{
	Board board(false, true, 3);

	board.display();

	int startRow;
	int startCol;
	int endRow;
	int endCol;

	char c = 'k';

	while (c != 'x') {
		cout << "\nMOVE\n";
		cout << "----------------\n";
		cout << "Enter start row: ";
		cin >> startRow;
		cout << "Enter start col: ";
		cin >> startCol;
		cout << "Valid moves: ";
		for (auto move : board.getValidMoves(startRow, startCol)) {
			cout << move.first << " " << move.second << "; ";
		}
		cout << "\nEnter end row: ";
		cin >> endRow;
		cout << "Enter end col: ";
		cin >> endCol;

		board.movePiece(startRow, startCol, endRow, endCol);
		c = _getch();

		system("cls");
		board.display();
	}
}