#include "algorithm.hpp"
#include <climits>
#include <iostream>

std::pair<int, Board> minimax(std::pair<int, int> pos, int depth, bool max_player, Game game) {
    Board board = game.getCurrentState();

    if (depth == 0 || game.isGameOver()) {
        return std::make_pair(game.getCurrentState().evaluate(), board);
    }

    int maxEval = INT_MIN, minEval = INT_MAX;
    Board bestBoard;

    if (max_player) {
        for (auto& move : board.allAvailableMoves(1)) {
            Game newGame = game; // Make a new copy of the game state
            if (newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second) == nullptr) {
                std::cerr << "Invalid move! [minimax()!!!]" << std::endl;
                continue;
            }
            int eval = minimax(std::make_pair(move.second.first, move.second.second), depth - 1, false, newGame).first;
            if (eval > maxEval) {
                maxEval = eval;
                bestBoard = newGame.getCurrentState();
            }
        }
        return std::make_pair(maxEval, bestBoard);
    } else {
        for (auto& move : board.allAvailableMoves(2)) {
            Game newGame = game; // Make a new copy of the game state
            if (newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second) == nullptr) {
                std::cerr << "Invalid move! [minimax()!!!]" << std::endl;
                continue;
            }
            int eval = minimax(std::make_pair(move.second.first, move.second.second), depth - 1, true, newGame).first;
            if (eval < minEval) {
                minEval = eval;
                bestBoard = newGame.getCurrentState();
            }
        }
        return std::make_pair(minEval, bestBoard);
    }
}