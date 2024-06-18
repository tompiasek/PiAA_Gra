#include "algorithm.hpp"
#include <climits>
#include <iostream>

std::pair<std::pair<int, Board>, std::pair<std::pair<int, int>, bool>> minimax(std::pair<int, int> pos, int depth, bool max_player, Game game) {
    Board board = Board(*game.getCurrentState());

    if (depth == 0 || game.isGameOver()) {
        return std::make_pair(std::make_pair((*game.getCurrentState()).evaluate(), board), std::make_pair(std::make_pair(-1, -1), false));
    }

    int maxEval = INT_MIN, minEval = INT_MAX;
    Board bestBoard;
    std::pair<int, int> bestMove = std::make_pair(-1, -1);
    bool bestJump = false;

    if (max_player) {
        for (auto& move : board.allAvailableMoves(1)) {
            Game newGame = game; // Make a new copy of the game state
            std::pair<Board*, bool> result = newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second);
            if (result.first == nullptr) {
                //std::cerr << "Invalid move! [minimax()!!!]" << std::endl;
                continue;
            }

            int eval;

            if (result.second) {  // If a jump occurred
                auto furtherJumps = newGame.getCurrentState()->getValidJumps(move.second.first, move.second.second);
                if (furtherJumps.size() > 0) {
                    eval = minimax(move.second, depth - 1, true, newGame).first.first; // Continue exploring jumps as max_player
                } else {
                    eval = minimax(move.second, depth - 1, false, newGame).first.first; // No more jumps, switch to min_player
                }
            } else {
                eval = minimax(move.second, depth - 1, false, newGame).first.first; // Standard move, switch to min_player
            }

            if (eval > maxEval) {
                maxEval = eval;
                bestBoard = Board(*newGame.getCurrentState());
                bestMove = move.second;
                bestJump = result.second;
            }
        }
        return std::make_pair(std::make_pair(maxEval, bestBoard), std::make_pair(bestMove, bestJump));
    } else {
        for (auto& move : board.allAvailableMoves(2)) {
            Game newGame = game; // Make a new copy of the game state
            std::pair<Board*, bool> result = newGame.playTurn(move.first.first, move.first.second, move.second.first, move.second.second);
            if (result.first == nullptr) {
                //std::cerr << "Invalid move! [minimax()!!!]" << std::endl;
                continue;
            }

            int eval;
            if (result.second) { // If a jump occurred
                auto furtherJumps = newGame.getCurrentState()->getValidJumps(move.second.first, move.second.second);
                if (furtherJumps.size() > 0) {
                    eval = minimax(move.second, depth - 1, false, newGame).first.first; // Continue exploring jumps as min_player
                } else {
                    eval = minimax(move.second, depth - 1, true, newGame).first.first; // No more jumps, switch to max_player
                }
            } else {
                eval = minimax(move.second, depth - 1, true, newGame).first.first; // Standard move, switch to max_player
            }

            if (eval < minEval) {
                minEval = eval;
                bestBoard = Board(*newGame.getCurrentState());
                bestMove = move.second;
                bestJump = result.second;
            }
        }
        return std::make_pair(std::make_pair(minEval, bestBoard), std::make_pair(bestMove, bestJump));
    }
}